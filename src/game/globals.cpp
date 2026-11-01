#include "types.h"
#include "Version.h"

#include "game/globals.h"
#include "game/stage.h"
#include <stdio.h>
#include "game/cgame.h"

extern "C" {
#if defined(VERSION_JP)
// japan has one language, so one table each
extern const char* lbl_802163C4[];
extern const char* lbl_8021617C[];
#else
// a string table per language
extern const char** lbl_802246EC[];
extern const char** lbl_802246D0[];
#endif
extern s32 lbl_802239F0[];
// the globals file's name
extern const char* lbl_8065A390;
}

// an entry of one of the data part's tables, the table offsets sit at the start of it

static inline u8* ResourceField(s32 table, s32 index, s32 stride, s32 member)
{
	u8* data = lbl_803675CC.mData;
	return data + ReadResourceOffset(data + table) + member + index * stride;
}

// the same, through an offset the entry holds
static inline u8* ResourcePointer(s32 table, s32 index, s32 stride, s32 member)
{
	u8* data   = lbl_803675CC.mData;
	u8* record = data + ReadResourceOffset(data + table) + index * stride;
	return data + ReadResourceOffset(record + member);
}

static inline BOOL IsGlobalScript(const u8* script)
{
	bool inside = false;
	if (script >= lbl_803675CC.mScripts && script < lbl_803675CC.mScripts + ReadResourceOffset(lbl_803675CC.mHeader + 0x14)) {
		inside = true;
	}
	return inside != false;
}

/**
 * @note Address: 0x8006DBC8
 * @note Size: 0x28
 */
extern "C" const char* fn_8006DBC8(s32 index)
{
#if defined(VERSION_JP)
	return lbl_802163C4[index];
#else
	return lbl_802246EC[lbl_8025C400.mLanguage][index];
#endif
}

/**
 * @note Address: 0x8006DBF0
 * @note Size: 0x28
 */
extern "C" const char* fn_8006DBF0(s32 index)
{
#if defined(VERSION_JP)
	return lbl_8021617C[index];
#else
	return lbl_802246D0[lbl_8025C400.mLanguage][index];
#endif
}

/**
 * @note Address: 0x8006DC18
 * @note Size: 0x74
 */
extern "C" u8* fn_8006DC18(s32 index)
{
	return ResourcePointer(0x50, index, 8, 4);
}

/**
 * @note Address: 0x8006DC8C
 * @note Size: 0x70
 */
extern "C" u8* fn_8006DC8C(s32 index)
{
	return ResourcePointer(0x50, index, 8, 0);
}

/**
 * @note Address: 0x8006DCFC
 * @note Size: 0x48
 */
extern "C" u32 fn_8006DCFC(s32 index)
{
	return *(u32*)(ResourceField(0x4C, index, 12, 8));
}

/**
 * @note Address: 0x8006DD44
 * @note Size: 0x74
 */
extern "C" u8* fn_8006DD44(s32 index)
{
	return ResourcePointer(0x4C, index, 12, 4);
}

/**
 * @note Address: 0x8006DDB8
 * @note Size: 0x70
 */
extern "C" u8* fn_8006DDB8(s32 index)
{
	return ResourcePointer(0x4C, index, 12, 0);
}

/**
 * @note Address: 0x8006DE28
 * @note Size: 0x48
 */
extern "C" s16 fn_8006DE28(s32 index)
{
	return *(s16*)(ResourceField(0x48, index, 12, 8));
}

/**
 * @note Address: 0x8006DE70
 * @note Size: 0x74
 */
extern "C" u8* fn_8006DE70(s32 index)
{
	return ResourcePointer(0x48, index, 12, 4);
}

/**
 * @note Address: 0x8006DEE4
 * @note Size: 0x70
 */
extern "C" u8* fn_8006DEE4(s32 index)
{
	return ResourcePointer(0x48, index, 12, 0);
}

/**
 * @note Address: 0x8006DF54
 * @note Size: 0x70
 */
extern "C" u8* fn_8006DF54(s32 index)
{
	return ResourcePointer(0x44, index, 4, 0);
}

/**
 * @note Address: 0x8006DFC4
 * @note Size: 0x70
 */
extern "C" u8* fn_8006DFC4(s32 index)
{
	return ResourcePointer(0x38, index, 8, 0);
}

/**
 * @note Address: 0x8006E034
 * @note Size: 0x70
 */
extern "C" u8* fn_8006E034(s32 index)
{
	return ResourcePointer(0x34, index, 4, 0);
}

/**
 * @note Address: 0x8006E0A4
 * @note Size: 0x48
 */
extern "C" s16 fn_8006E0A4(s32 index)
{
	return *(s16*)(ResourceField(0x30, index, 12, 8));
}

/**
 * @note Address: 0x8006E0EC
 * @note Size: 0x74
 */
extern "C" u8* fn_8006E0EC(s32 index)
{
	return ResourcePointer(0x30, index, 12, 4);
}

/**
 * @note Address: 0x8006E160
 * @note Size: 0x70
 */
extern "C" u8* fn_8006E160(s32 index)
{
	return ResourcePointer(0x30, index, 12, 0);
}

/**
 * @note Address: 0x8006E1D0
 * @note Size: 0x70
 */
extern "C" u8* fn_8006E1D0(s32 index)
{
	return ResourcePointer(0x2C, index, 4, 0);
}

/**
 * @note Address: 0x8006E240
 * @note Size: 0x4C
 */
extern "C" u8 fn_8006E240(s32 index, s32 member)
{
	return ResourceField(0x28, index, 20, 18)[member];
}

/**
 * @note Address: 0x8006E28C
 * @note Size: 0x48
 */
extern "C" s16 fn_8006E28C(s32 index)
{
	return *(s16*)(ResourceField(0x28, index, 20, 16));
}

/**
 * @note Address: 0x8006E2D4
 * @note Size: 0x48
 */
extern "C" s16 fn_8006E2D4(s32 index)
{
	return *(s16*)(ResourceField(0x28, index, 20, 14));
}

/**
 * @note Address: 0x8006E31C
 * @note Size: 0x48
 */
extern "C" s16 fn_8006E31C(s32 index)
{
	return *(s16*)(ResourceField(0x28, index, 20, 12));
}

/**
 * @note Address: 0x8006E364
 * @note Size: 0x48
 */
extern "C" s16 fn_8006E364(s32 index)
{
	return *(s16*)(ResourceField(0x28, index, 20, 10));
}

/**
 * @note Address: 0x8006E3AC
 * @note Size: 0x50
 */
extern "C" void fn_8006E3AC(s32 index, u16 mask)
{
	u16* flags = (u16*)(ResourceField(0x28, index, 20, 8));
	*flags &= ~mask;
}

/**
 * @note Address: 0x8006E3FC
 * @note Size: 0x50
 */
extern "C" void fn_8006E3FC(s32 index, u16 mask)
{
	u16* flags = (u16*)(ResourceField(0x28, index, 20, 8));
	*flags |= mask;
}

/**
 * @note Address: 0x8006E44C
 * @note Size: 0x50
 */
extern "C" u16 fn_8006E44C(s32 index, u16 mask)
{
	return *(u16*)(ResourceField(0x28, index, 20, 8)) & mask;
}

/**
 * @note Address: 0x8006E49C
 * @note Size: 0x74
 */
extern "C" u8* fn_8006E49C(s32 index)
{
	return ResourcePointer(0x28, index, 20, 4);
}

/**
 * @note Address: 0x8006E510
 * @note Size: 0x70
 */
extern "C" u8* fn_8006E510(s32 index)
{
	return ResourcePointer(0x28, index, 20, 0);
}

// script offsets count from the buffer the operand is in, the globals file or the stage's, 0 is none
/**
 * @note Address: 0x8006E580
 * @note Size: 0xEC
 */
extern "C" u8* fn_8006E580(u8* operand)
{
	if (IsGlobalScript(operand)) {
		u32 offset = ReadResourceOffset(operand);
		return offset == 0 ? NULL : lbl_803675CC.mScripts + offset;
	}
	u32 offset = ReadResourceOffset(operand);
	return offset == 0 ? NULL : (u8*)lbl_803672CC.mDataFile.mBuffer + offset;
}

/**
 * @note Address: 0x8006E66C
 * @note Size: 0x68
 */
extern "C" BOOL fn_8006E66C(u8* script)
{
	return IsGlobalScript(script);
}

/**
 * @note Address: 0x8006E6D4
 * @note Size: 0x50
 */
extern "C" u8* fn_8006E6D4(s32 index)
{
	u8* scripts = lbl_803675CC.mScripts;
	u32 offset  = ReadResourceOffset(scripts + index * 4);
	return offset == 0 ? NULL : scripts + offset;
}

/**
 * @note Address: 0x8006E724
 * @note Size: 0x10
 */
extern "C" u8* fn_8006E724(s32)
{
	return lbl_803675CC.mScripts;
}

// a text entry, ids from 10000 go through a remap table first
/**
 * @note Address: 0x8006E734
 * @note Size: 0x88
 */
extern "C" u8* fn_8006E734(s32 index)
{
	if (index >= 10000) {
		index = lbl_802239F0[index - 10000];
		if (index < 0)
			return NULL;
	}
	u8* text  = lbl_803675CC.mText;
	u8* entry = text + index * 4;
	if (*(u32*)entry != 0)
		return text + ReadResourceOffset(entry);
	return NULL;
}

/**
 * @note Address: 0x8006E7BC
 * @note Size: 0x40
 */
extern "C" s32 fn_8006E7BC()
{
	u64 flags = lbl_8025C400.mStatus.mFlags;
	if (flags & 0x8000000000000ULL)
		return 0;
	return 14;
}

/**
 * @note Address: 0x8006E7FC
 * @note Size: 0x6C
 */
extern "C" u32 fn_8006E7FC(s32 index)
{
	return ReadResourceOffset(ResourceField(0x24, index, 4, 0));
}

/**
 * @note Address: 0x8006E868
 * @note Size: 0x6C
 */
extern "C" u32 fn_8006E868(s32 index)
{
	return ReadResourceOffset(ResourceField(0x20, index, 4, 0));
}

/**
 * @note Address: 0x8006E8D4
 * @note Size: 0x6C
 */
extern "C" u32 fn_8006E8D4(s32 index)
{
	return ReadResourceOffset(ResourceField(0x1C, index, 4, 0));
}

/**
 * @note Address: 0x8006E940
 * @note Size: 0x6C
 */
extern "C" u32 fn_8006E940(s32 index)
{
	return ReadResourceOffset(ResourceField(0x18, index, 4, 0));
}

/**
 * @note Address: 0x8006E9AC
 * @note Size: 0x6C
 */
extern "C" u32 fn_8006E9AC(s32 index)
{
	return ReadResourceOffset(ResourceField(0x14, index, 4, 0));
}

/**
 * @note Address: 0x8006EA18
 * @note Size: 0x6C
 */
extern "C" u32 fn_8006EA18(s32 index)
{
	return ReadResourceOffset(ResourceField(0x10, index, 4, 0));
}

/**
 * @note Address: 0x8006EA84
 * @note Size: 0x6C
 */
extern "C" u32 fn_8006EA84(s32 index)
{
	return ReadResourceOffset(ResourceField(0xC, index, 4, 0));
}

/**
 * @note Address: 0x8006EAF0
 * @note Size: 0x6C
 */
extern "C" u32 fn_8006EAF0(s32 index)
{
	return ReadResourceOffset(ResourceField(0x8, index, 4, 0));
}

/**
 * @note Address: 0x8006EB5C
 * @note Size: 0x6C
 */
extern "C" u32 fn_8006EB5C(s32 index)
{
	return ReadResourceOffset(ResourceField(0x4, index, 4, 0));
}

/**
 * @note Address: 0x8006EBC8
 * @note Size: 0x6C
 */
extern "C" u32 fn_8006EBC8(s32 index)
{
	return ReadResourceOffset(ResourceField(0x0, index, 4, 0));
}

/**
 * @note Address: 0x8006EC34
 * @note Size: 0x1C
 */
extern "C" BOOL fn_8006EC34()
{
	return lbl_803675CC.mFile.mBuffer != NULL;
}

// reads the globals file and finds its three parts
/**
 * @note Address: 0x8006EC50
 * @note Size: 0x140
 */
extern "C" BOOL fn_8006EC50()
{
#if defined(VERSION_JP)
	const char* path = lbl_8065A390;
#else
	char path[0x40];
#if defined(VERSION_PAL)
	sprintf(path, "bin%s/%s", lbl_8022AE28[lbl_8025C400.mLanguage], lbl_8065A390);
#else
	sprintf(path, "bin%s/%s", lbl_802244C8[lbl_8025C400.mLanguage], lbl_8065A390);
#endif
#endif
	CFile* file = &lbl_803675CC.mFile;
	if (file->ReadEx(path, 0, 0)) {
		lbl_803675CC.mFile.CloseHandle();
		u8* header            = (u8*)lbl_803675CC.mFile.mBuffer;
		lbl_803675CC.mHeader  = header;
		lbl_803675CC.mData    = header + ReadResourceOffset(header) + 0x18;
		lbl_803675CC.mText    = header + ReadResourceOffset(header + 8) + 0x18;
		lbl_803675CC.mScripts = header + ReadResourceOffset(header + 16) + 0x18;
		return TRUE;
	}
	return FALSE;
}
