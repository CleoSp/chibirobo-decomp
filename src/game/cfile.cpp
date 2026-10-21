#include "game/cfile.h"
#include "game/mm.h"
#include "Dolphin/os.h"
#include <string.h>

extern "C" {
s32 HSD_DevComRequest(s32 file, u32 src, void* dest, u32 size, s32 type, s32 pri, void (*callback)(s32, DVDFileInfo*, void*, BOOL),
                      DVDFileInfo* args);
}

// report, then panic with the function name. the line goes in by hand like the asserts
#define HALT(line)              \
	OSReport("\n!!!HALT!!!\n"); \
	OSPanic(__FILE__, line, "%s%s", "", __FUNCTION__)

// the archive files get read out of, set by CFileArchive::ReadAsync
static CFileArchive* currentArchive;

/**
 * @note Address: 0x8019FD78
 * @note Size: 0x24
 */
static void ReadCallback(s32, DVDFileInfo* info, void*, BOOL cancel)
{
	s32* result = (s32*)info->cBlock.userData;
	if (cancel) {
		*result = -2;
	} else {
		*result = 0;
	}
}

/**
 * @note Address: 0x8019FD4C
 * @note Size: 0x2C
 */
void* CFile::Alloc(u32 size)
{
	return mm::Alloc(size, mPath);
}

/**
 * @note Address: 0x8019FD08
 * @note Size: 0x44
 */
void CFile::Free()
{
	if (mBuffer != NULL) {
		mm::Free(mBuffer, 1);
		mBuffer = NULL;
	}
}

/**
 * @note Address: 0x8019FCE4
 * @note Size: 0x24
 */
void* CFileA::Alloc(u32 size)
{
	return OSAlloc(size);
}

/**
 * @note Address: 0x8019FCA0
 * @note Size: 0x44
 */
void CFileA::Free()
{
	if (mBuffer != NULL) {
		OSFree(mBuffer);
		mBuffer = NULL;
	}
}

/**
 * @note Address: 0x8019FC50
 * @note Size: 0x50
 */
s32 CFile::ConvertPathToEntrynum(const char* path)
{
	s32 entrynum = -1;
	if (currentArchive != NULL) {
		entrynum = ARCConvertPathToEntrynum(&currentArchive->mHandle, path);
	}
	if (entrynum < 0) {
		entrynum = DVDConvertPathToEntrynum((char*)path);
	}
	return entrynum;
}

// todo: cleanup
inline s32 CFile::StartRead(BOOL fromAram)
{
	if (fromAram) {
		u32 pos               = mOffset;
		u32 len               = OSRoundUp32B(mSize);
		DVDFileInfo* info     = mArchive != NULL ? (DVDFileInfo*)mArchive : this;
		mResult               = 1;
		info->cBlock.userData = (void*)&mResult;
		HSD_DevComRequest(0, pos, mBuffer, len, 0x19, 1, ReadCallback, info);
		return 0;
	}
	u32 pos = mOffset;
	s32 entrynum;
	u32 len = OSRoundUp32B(mSize);
	if (mArchive != NULL) {
		entrynum = mArchive->mEntrynum;
		memcpy(this, mArchive, sizeof(DVDFileInfo));
		pos += ARCGetStartOffset(&mArcInfo);
	} else {
		entrynum = mEntrynum;
	}
	mResult         = 1;
	cBlock.userData = (void*)&mResult;
	HSD_DevComRequest(entrynum, pos, mBuffer, len, 0x21, 1, ReadCallback, this);
	return 0;
}

// todo: cleanup
inline s32 CFile::Open()
{
	if (mArchive != NULL) {
		mEntrynum = ARCConvertPathToEntrynum(&mArchive->mHandle, mPath);
		if (mEntrynum < 0) {
			return -3;
		}
		if (!ARCFastOpen(&mArchive->mHandle, mEntrynum, &mArcInfo)) {
			return -4;
		}
		mSize = ARCGetLength(&mArcInfo);
	} else {
		mEntrynum = DVDConvertPathToEntrynum(mPath);
		if (mEntrynum < 0) {
			return -3;
		}
		if (!DVDFastOpen(mEntrynum, this)) {
			return -4;
		}
		mSize = length;
	}
	return mEntrynum;
}

/**
 * @note Address: 0x8019F91C
 * @note Size: 0x334
 */
s32 CFile::ReadAsync(const char* path, u32 offset, u32 size, BOOL fromAram)
{
	// NONMATCHING: read result register and pooled string relocations
	strcpy(mPath, path);
	BOOL inArchive = currentArchive != NULL ? ARCConvertPathToEntrynum(&currentArchive->mHandle, mPath) >= 0 : 0;
	if (inArchive) {
		mArchive = currentArchive;
	} else {
		mArchive = NULL;
	}

	if (mBuffer != NULL) {
		Close();
	}
	mOffset = offset;

	s32 result = Open();
	if (result < 0) {
		if (mArchive != NULL) {
			OSReport("file not found - '%s//%s'!\n", mArchive->mPath, mPath);
		} else {
			OSReport("file not found - '%s'!\n", mPath);
		}
		return result;
	}

	if (size != 0) {
		mSize = size;
	}
	mBuffer = Alloc(OSRoundUp32B(mSize));
	if (mBuffer == NULL) {
		if (mArchive != NULL) {
			OSReport("cannot allocate memory for '%s//%s'!\n", mArchive->mPath, mPath);
		} else {
			OSReport("cannot allocate memory for '%s'!\n", mPath);
		}
		return -7;
	}

	s32 error = StartRead(fromAram);
	if (error < 0) {
		if (mArchive != NULL) {
			OSReport("cannot read file - '%s//%s'!\n", mArchive->mPath, mPath);
		} else {
			OSReport("cannot read file - '%s'!\n", mPath);
		}
		return error;
	}
	return mResult;
}
/*
    stwu     r1, -0x20(r1)
    mflr     r0
    stw      r0, 0x24(r1)
    stmw     r27, 0xc(r1)
    mr       r29, r3
    lis      r3, lbl_8020EE38@ha
    mr       r28, r5
    addi     r31, r3, lbl_8020EE38@l
    mr       r27, r6
    mr       r30, r7
    addi     r3, r29, 0x3c
    bl       strcpy
    lwz      r3, currentArchive@sda21(r0)
    cmplwi   r3, 0x0
    beq      .L_8019F974
    addi     r3, r3, 0xec
    addi     r4, r29, 0x3c
    bl       ARCConvertPathToEntrynum
    srwi     r0, r3, 31
    xori     r0, r0, 0x1
    clrlwi   r0, r0, 24
    b        .L_8019F978
.L_8019F974:
    li       r0, 0x0
.L_8019F978:
    cmpwi    r0, 0x0
    beq      .L_8019F98C
    lwz      r0, currentArchive@sda21(r0)
    stw      r0, 0xd4(r29)
    b        .L_8019F994
.L_8019F98C:
    li       r0, 0x0
    stw      r0, 0xd4(r29)
.L_8019F994:
    lwz      r0, 0xcc(r29)
    cmplwi   r0, 0x0
    beq      .L_8019F9B4
    mr       r3, r29
    lwz      r12, 0xe8(r29)
    lwz      r12, 0x20(r12)
    mtctr    r12
    bctrl
.L_8019F9B4:
    stw      r28, 0xc4(r29)
    lwz      r3, 0xd4(r29)
    cmplwi   r3, 0x0
    beq      .L_8019FA18
    addi     r3, r3, 0xec
    addi     r4, r29, 0x3c
    bl       ARCConvertPathToEntrynum
    stw      r3, 0xc0(r29)
    lwz      r4, 0xc0(r29)
    cmpwi    r4, 0x0
    bge      .L_8019F9E8
    li       r28, -0x3
    b        .L_8019FA5C
.L_8019F9E8:
    lwz      r3, 0xd4(r29)
    addi     r5, r29, 0xd8
    addi     r3, r3, 0xec
    bl       ARCFastOpen
    cmpwi    r3, 0x0
    bne      .L_8019FA08
    li       r28, -0x4
    b        .L_8019FA5C
.L_8019FA08:
    addi     r3, r29, 0xd8
    bl       ARCGetLength
    stw      r3, 0xc8(r29)
    b        .L_8019FA58
.L_8019FA18:
    addi     r3, r29, 0x3c
    bl       DVDConvertPathToEntrynum
    stw      r3, 0xc0(r29)
    lwz      r3, 0xc0(r29)
    cmpwi    r3, 0x0
    bge      .L_8019FA38
    li       r28, -0x3
    b        .L_8019FA5C
.L_8019FA38:
    mr       r4, r29
    bl       DVDFastOpen
    cmpwi    r3, 0x0
    bne      .L_8019FA50
    li       r28, -0x4
    b        .L_8019FA5C
.L_8019FA50:
    lwz      r0, 0x34(r29)
    stw      r0, 0xc8(r29)
.L_8019FA58:
    lwz      r28, 0xc0(r29)
.L_8019FA5C:
    cmpwi    r28, 0x0
    bge      .L_8019FAA0
    lwz      r4, 0xd4(r29)
    cmplwi   r4, 0x0
    beq      .L_8019FA88
    addi     r3, r31, 0x50
    addi     r4, r4, 0x3c
    addi     r5, r29, 0x3c
    crclr    cr1eq
    bl       OSReport
    b        .L_8019FA98
.L_8019FA88:
    addi     r3, r31, 0x6c
    addi     r4, r29, 0x3c
    crclr    cr1eq
    bl       OSReport
.L_8019FA98:
    mr       r3, r28
    b        .L_8019FC3C
.L_8019FAA0:
    cmplwi   r27, 0x0
    beq      .L_8019FAAC
    stw      r27, 0xc8(r29)
.L_8019FAAC:
    mr       r3, r29
    lwz      r4, 0xc8(r29)
    lwz      r12, 0xe8(r29)
    addi     r0, r4, 0x1f
    lwz      r12, 0xc(r12)
    clrrwi   r4, r0, 5
    mtctr    r12
    bctrl
    stw      r3, 0xcc(r29)
    lwz      r0, 0xcc(r29)
    cmplwi   r0, 0x0
    bne      .L_8019FB18
    lwz      r4, 0xd4(r29)
    cmplwi   r4, 0x0
    beq      .L_8019FB00
    addi     r3, r31, 0x84
    addi     r4, r4, 0x3c
    addi     r5, r29, 0x3c
    crclr    cr1eq
    bl       OSReport
    b        .L_8019FB10
.L_8019FB00:
    addi     r3, r31, 0xac
    addi     r4, r29, 0x3c
    crclr    cr1eq
    bl       OSReport
.L_8019FB10:
    li       r3, -0x7
    b        .L_8019FC3C
.L_8019FB18:
    cmpwi    r30, 0x0
    beq      .L_8019FB78
    lwz      r10, 0xd4(r29)
    lwz      r3, 0xc8(r29)
    cmplwi   r10, 0x0
    lwz      r4, 0xc4(r29)
    addi     r0, r3, 0x1f
    clrrwi   r6, r0, 5
    beq      .L_8019FB40
    b        .L_8019FB44
.L_8019FB40:
    mr       r10, r29
.L_8019FB44:
    li       r0, 0x1
    lis      r3, ReadCallback__FlP11DVDFileInfoPvi@ha
    stw      r0, 0xd0(r29)
    addi     r0, r29, 0xd0
    addi     r9, r3, ReadCallback__FlP11DVDFileInfoPvi@l
    li       r3, 0x0
    stw      r0, 0x2c(r10)
    li       r7, 0x19
    li       r8, 0x1
    lwz      r5, 0xcc(r29)
    bl       HSD_DevComRequest
    li       r27, 0x0
    b        .L_8019FBF4
.L_8019FB78:
    lwz      r4, 0xd4(r29)
    lwz      r3, 0xc8(r29)
    cmplwi   r4, 0x0
    lwz      r27, 0xc4(r29)
    addi     r0, r3, 0x1f
    clrrwi   r28, r0, 5
    beq      .L_8019FBB4
    lwz      r30, 0xc0(r4)
    mr       r3, r29
    li       r5, 0x3c
    bl       memcpy
    addi     r3, r29, 0xd8
    bl       ARCGetStartOffset
    add      r27, r27, r3
    b        .L_8019FBB8
.L_8019FBB4:
    lwz      r30, 0xc0(r29)
.L_8019FBB8:
    li       r0, 0x1
    lis      r3, ReadCallback__FlP11DVDFileInfoPvi@ha
    stw      r0, 0xd0(r29)
    addi     r0, r29, 0xd0
    addi     r9, r3, ReadCallback__FlP11DVDFileInfoPvi@l
    mr       r3, r30
    stw      r0, 0x2c(r29)
    mr       r4, r27
    mr       r6, r28
    mr       r10, r29
    lwz      r5, 0xcc(r29)
    li       r7, 0x21
    li       r8, 0x1
    bl       HSD_DevComRequest
    li       r27, 0x0
.L_8019FBF4:
    cmpwi    r27, 0x0
    bge      .L_8019FC38
    lwz      r4, 0xd4(r29)
    cmplwi   r4, 0x0
    beq      .L_8019FC20
    addi     r3, r31, 0xd0
    addi     r4, r4, 0x3c
    addi     r5, r29, 0x3c
    crclr    cr1eq
    bl       OSReport
    b        .L_8019FC30
.L_8019FC20:
    addi     r3, r31, 0xf0
    addi     r4, r29, 0x3c
    crclr    cr1eq
    bl       OSReport
.L_8019FC30:
    mr       r3, r27
    b        .L_8019FC3C
.L_8019FC38:
    lwz      r3, 0xd0(r29)
.L_8019FC3C:
    lmw      r27, 0xc(r1)
    lwz      r0, 0x24(r1)
    mtlr     r0
    addi     r1, r1, 0x20
    blr
*/

/**
 * @note Address: 0x8019F8D8
 * @note Size: 0x44
 */
s32 CFile::GetResult()
{
	if (mResult == 0) {
		DCInvalidateRange(mBuffer, mSize);
	}
	return mResult;
}

/**
 * @note Address: 0x8019F878
 * @note Size: 0x60
 */
s32 CFile::Read(const char* path, u32 offset, u32 size)
{
	s32 result = ReadAsync(path, offset, size, FALSE);
	if (result >= 0) {
		do {
			result = GetResult();
		} while (result == 1);
	}
	return result;
}

/**
 * @note Address: 0x8019F804
 * @note Size: 0x74
 */
void CFile::Close()
{
	if (mEntrynum >= 0) {
		mEntrynum = -1;
		if (mArchive != NULL) {
			ARCClose(&mArcInfo);
		} else {
			DVDClose(this);
		}
	}
	Free();
	mState = 1;
}

/**
 * @note Address: 0x8019F6EC
 * @note Size: 0x118
 */
BOOL CFile::ReadEx(const char* path, u32 offset, u32 size)
{
	switch (mState) {
	case 0:
		return TRUE;
	case 1:
		if (ReadAsync(path, offset, size, FALSE) >= 0) {
			mState++;
		} else {
			HALT(314);
		}
	case 2:
		s32 result = GetResult();
		if (result == 0) {
			mState = 0;
			return TRUE;
		}
		if (result < 0) {
			HALT(323);
		}
		break;
	}
	return FALSE;
}

/**
 * @note Address: 0x8019F668
 * @note Size: 0x84
 */
s32 CFileArchive::ReadAsync(const char* path, u32 offset, u32 size, BOOL fromAram)
{
	if (currentArchive != NULL) {
		currentArchive->Close();
	}
	currentArchive = NULL;
	s32 result     = CFile::ReadAsync(path, offset, size, fromAram);
	if (result >= 0) {
		currentArchive = this;
	}
	return result;
}

/**
 * @note Address: 0x8019F594
 * @note Size: 0xD4
 */
s32 CFileArchive::GetResult()
{
	s32 result = CFile::GetResult();
	if (result == 0 && !ARCInitHandle(mBuffer, &mHandle)) {
		OSReport("cannot open - '%s'!\n", mPath);
		CFile::Close();
		result = -4;
	}
	return result;
}

/**
 * @note Address: 0x8019F40C
 * @note Size: 0x188
 */
BOOL CFileArchive::ReadEx(const char* path, u32 offset, u32 size)
{
	switch (mState) {
	case 0:
		return TRUE;
	case 1:
		// the header first, it says how much of the image to read
		if (CFile::ReadAsync(path, offset, 0x20, FALSE) >= 0) {
			mState++;
		} else {
			return TRUE;
		}
	case 2: {
		s32 result = CFile::GetResult();
		if (result == 0) {
			// the header's file data offset, only the part before it is read
			size = ((u32*)mBuffer)[3];
			Close();
			mState = 3;
		} else if (result < 0) {
			return TRUE;
		} else {
			break;
		}
	}
	case 3:
		if (ReadAsync(path, offset, size, FALSE) >= 0) {
			mState++;
		} else {
			return TRUE;
		}
	case 4: {
		s32 result = GetResult();
		if (result == 0) {
			mState = 0;
			return TRUE;
		}
		if (result < 0) {
			return TRUE;
		}
		break;
	}
	}
	return FALSE;
}

/**
 * @note Address: 0x8019F378
 * @note Size: 0x94
 */
BOOL CFileText::GetLine(char* line)
{
	if (mPos >= mSize) {
		return FALSE;
	}

	char* p      = (char*)mBuffer + mPos;
	BOOL lineEnd = FALSE;
	do {
		char c = *p;
		if (c == '\r' || c == '\n' || c == 0x1A) {
			lineEnd = TRUE;
			p++;
		} else {
			if (lineEnd) {
				break;
			}
			*line++ = c;
			p++;
		}
	} while ((u32)(p - (char*)mBuffer) < mSize);

	*line = '\0';
	mPos  = p - (char*)mBuffer;
	return TRUE;
}
