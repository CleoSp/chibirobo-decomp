#include "game/cgame.h"
#include "types.h"
#include "Version.h"

#include "game/globals.h"
#include "game/stage.h"
#include "game/ut.h"
#include "Dolphin/GX/GXTypes.h"
#include <stdio.h>
#include <string.h>

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
extern s32 lbl_80215050[];
extern UnkStageObj lbl_80212D88[];
extern UnkStageObj spawnTable[];
extern UnkObjData objTable[];
extern UnkObjData objTableEx[];
extern u8 lbl_80216C30[];
// HSD_VIInfo, its header needs the hsd include path
extern u8 HSD_VIData[];
BOOL fn_8001F97C(XSound* sound, s32 stage);
}

Stage lbl_803672CC;
Globals lbl_803675CC;

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

// a 4 byte offset from base, 0 is none
static inline u8* ReadScriptPointer(u8* base, const u8* operand)
{
	u32 offset = ReadResourceOffset(operand);
	if (offset == 0)
		return NULL;
	return base + offset;
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
	return ReadScriptPointer(lbl_803675CC.mScripts, lbl_803675CC.mScripts + index * 4);
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
		lbl_803675CC.mHeader  = (u8*)lbl_803675CC.mFile.mBuffer;
		u8* header            = lbl_803675CC.mHeader;
		lbl_803675CC.mData    = header + ReadResourceOffset(header) + 0x18;
		lbl_803675CC.mText    = header + ReadResourceOffset(header + 8) + 0x18;
		lbl_803675CC.mScripts = header + ReadResourceOffset(header + 16) + 0x18;
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x8006ED90
 * @note Size: 0x24
 */
extern "C" s32 fn_8006ED90(s32 index)
{
	if (index < 0)
		return -1;
	return lbl_80215050[index];
}

/**
 * @note Address: 0x8006EDB4
 * @note Size: 0x14
 */
extern "C" UnkStageObj* fn_8006EDB4(s32 index)
{
	return &lbl_80212D88[index];
}

/**
 * @note Address: 0x8006EDC8
 * @note Size: 0x14
 */
extern "C" UnkStageObj* GetSpawnData(s32 index)
{
	return &spawnTable[index];
}

/**
 * @note Address: 0x8006EDDC
 * @note Size: 0x3C
 */
extern "C" UnkObjData* GetObjData(CoordInfo* info)
{
	s32 index = info->_00->_00;
	if (index >= 10000)
		return &objTableEx[index - 10000];
	return &objTable[index];
}

/**
 * @note Address: 0x8006EE18
 * @note Size: 0xC
 */
extern "C" void* fn_8006EE18()
{
	return lbl_80216C30;
}

/**
 * @note Address: 0x8006EE24
 * @note Size: 0x514
 */
extern "C" BOOL fn_8006EE24(Stage* stage, s32 index)
{
	switch (stage->mState) {
	case 0: {
#if defined(VERSION_JP)
		strcpy(stage->mDataPath, fn_801911B4("bin/%s.bin", stageTable[index].fileName));
#elif defined(VERSION_PAL)
		StageDesc* desc = &stageTable[index];
		strcpy(stage->mDataPath, fn_801911B4("bin%s/%s.bin", lbl_8022AE28[lbl_8025C400.mLanguage], desc->fileName));
#else
		strcpy(stage->mDataPath, fn_801911B4("bin%s/%s.bin", lbl_802244C8[lbl_8025C400.mLanguage], stageTable[index].fileName));
#endif
#if defined(VERSION_PAL)
		strcpy(stage->mModulePath, fn_801911B4("rel/%s%s.rel", desc->moduleName, ""));
#else
		strcpy(stage->mModulePath, fn_801911B4("rel/%s%s.rel", stageTable[index].fileName, ""));
#endif
		if (!(lbl_8025C400.mStatus.mFlags & 0x100000000ULL))
			stage->mModulePath[0] = 0;
		++stage->mState;
	}
	// fall through, every step that's done moves on in the same frame
	case 1: {
		if (!stage->mDataFile.ReadEx(stage->mDataPath, 0, 0))
			break;
		stage->mDataFile.CloseHandle();
		u8* data                  = (u8*)stage->mDataFile.mBuffer;
		stage->mEnvironment.mData = data + ReadResourceOffset(data);
		stage->mPlacements._00    = (UnkStageObj*)(data + ReadResourceOffset(data + 4));
		stage->mScriptOffsets     = data + ReadResourceOffset(data + 8);
		stage->_184               = data + ReadResourceOffset(data + 0xC);
		if (stage->_184 == data)
			stage->_184 = NULL;
		stage->_188 = data + ReadResourceOffset(data + 0x10);
		if (stage->_188 == data)
			stage->_188 = NULL;
		stage->_18C = data + ReadResourceOffset(data + 0x14);
		if (stage->_18C == data)
			stage->_18C = NULL;
		stage->_190 = data + ReadResourceOffset(data + 0x18);
		if (stage->_190 == data)
			stage->_190 = NULL;
		stage->_194 = data + ReadResourceOffset(data + 0x1C);
		if (stage->_194 == data)
			stage->_194 = NULL;
		stage->_198 = data + ReadResourceOffset(data + 0x20);
		if (stage->_198 == data)
			stage->_198 = NULL;
		stage->_19C = data + ReadResourceOffset(data + 0x24);
		stage->_1A0 = data + ReadResourceOffset(data + 0x28);
		if (stage->_1A0 == data)
			stage->_1A0 = NULL;
		stage->_1A4 = data + ReadResourceOffset(data + 0x2C);
		if (stage->_1A4 == data)
			stage->_1A4 = NULL;
		stage->_1A8 = data + ReadResourceOffset(data + 0x30);
		if (stage->_1A8 == data)
			stage->_1A8 = NULL;
		++stage->mState;
	}
	case 2:
		if ((s8)stage->mModulePath[0] != 0) {
			if (!stage->mModule.Load(stage->mModulePath))
				break;
			++stage->mState;
		} else {
			++stage->mState;
		}
	case 3:
		++stage->mState;
	case 4:
		if (fn_8001F97C(&lbl_8025C400.mSound, index))
			++stage->mState;
		break;
	}
	return stage->mState > 4;
}

/**
 * @note Address: 0x8006F338
 * @note Size: 0x58
 */
extern "C" s32 fn_8006F338(s32 function, s32 count, s32* args)
{
	if (IsModuleLoaded(&lbl_803672CC.mModule))
		return lbl_803672CC.CallModule(function, count, args);
	return -1;
}

/**
 * @note Address: 0x8006F390
 * @note Size: 0x1C
 */
extern "C" s32 fn_8006F390(s32 index)
{
	return ((s16*)(lbl_803672CC.mEnvironment.mData + 0xA))[index];
}

/**
 * @note Address: 0x8006F3AC
 * @note Size: 0x10
 */
extern "C" CamAreaRecord* fn_8006F3AC()
{
	return (CamAreaRecord*)lbl_803672CC._1A8;
}

/**
 * @note Address: 0x8006F3BC
 * @note Size: 0x10
 */
extern "C" CamInfoData* fn_8006F3BC()
{
	return (CamInfoData*)lbl_803672CC._1A4;
}

/**
 * @note Address: 0x8006F3CC
 * @note Size: 0x10
 */
extern "C" CamAreaRecord* fn_8006F3CC()
{
	return (CamAreaRecord*)lbl_803672CC._1A0;
}

/**
 * @note Address: 0x8006F3DC
 * @note Size: 0x10
 */
extern "C" void* fn_8006F3DC()
{
	return lbl_803672CC._19C;
}

/**
 * @note Address: 0x8006F3EC
 * @note Size: 0x10
 */
extern "C" void* fn_8006F3EC()
{
	return lbl_803672CC._194;
}

/**
 * @note Address: 0x8006F3FC
 * @note Size: 0x10
 */
extern "C" void* fn_8006F3FC()
{
	return lbl_803672CC._190;
}

/**
 * @note Address: 0x8006F40C
 * @note Size: 0x10
 */
extern "C" void* fn_8006F40C()
{
	return lbl_803672CC._18C;
}

/**
 * @note Address: 0x8006F41C
 * @note Size: 0x10
 */
extern "C" void* fn_8006F41C()
{
	return lbl_803672CC._188;
}

/**
 * @note Address: 0x8006F42C
 * @note Size: 0x10
 */
extern "C" void* fn_8006F42C()
{
	return lbl_803672CC._184;
}

/**
 * @note Address: 0x8006F43C
 * @note Size: 0x54
 */
extern "C" u8* fn_8006F43C(s32 index)
{
	u32 offset = ReadResourceOffset(lbl_803672CC.mScriptOffsets + index * 4);
	return offset == 0 ? NULL : (u8*)lbl_803672CC.mDataFile.mBuffer + offset;
}

/**
 * @note Address: 0x8006F490
 * @note Size: 0x10
 */
extern "C" u8* fn_8006F490()
{
	return (u8*)lbl_803672CC.mDataFile.mBuffer;
}

/**
 * @note Address: 0x8006F4A0
 * @note Size: 0x18
 */
extern "C" UnkStageObj* fn_8006F4A0(s32 index)
{
	return lbl_803672CC.mPlacements._00 + index;
}

/**
 * @note Address: 0x8006F4B8
 * @note Size: 0x10
 */
extern "C" EnvInfo* fn_8006F4B8()
{
	return &lbl_803672CC.mEnvironment;
}

/**
 * @note Address: 0x8006F4C8
 * @note Size: 0x1C
 */
extern "C" BOOL fn_8006F4C8()
{
	return lbl_803672CC.mDataFile.mBuffer != NULL;
}

/**
 * @note Address: 0x8006F4E4
 * @note Size: 0x60
 */
extern "C" void fn_8006F4E4()
{
	GXColor color                  = *(GXColor*)lbl_803672CC.mEnvironment.mData;
	*(GXColor*)(HSD_VIData + 0x48) = color; // current.vi.bg_color
	lbl_803672CC.mModule.Print();
}

/**
 * @note Address: 0x8006F544
 * @note Size: 0x90
 */
extern "C" void fn_8006F544()
{
	if (IsModuleLoaded(&lbl_803672CC.mModule))
		lbl_803672CC.CallModule(6, 0, NULL);
	lbl_803672CC.mModule.Unload();
	CFile* file = &lbl_803672CC.mDataFile;
	file->Close();
	lbl_803672CC.mState = 0;
}

/**
 * @note Address: 0x8006F5D4
 * @note Size: 0x2C
 */
extern "C" BOOL fn_8006F5D4(s32 index)
{
	return fn_8006EE24(&lbl_803672CC, index);
}

static inline s32 StageCount()
{
	s32 count = 0;
	while (stageTable[count].fileName != NULL)
		++count;
	return count;
}

// 100 and up are the stages after the table's null entry
static inline s32 StageIndex(s32 index)
{
	if (index < 100)
		return index;
	s32 count = StageCount();
	count += index;
	return count - 99;
}

/**
 * @note Address: 0x8006F600
 * @note Size: 0x54
 */
extern "C" u32 CheckStageFlag(s32 index, u32 mask)
{
	StageDesc* desc = &stageTable[StageIndex(index)];
	return mask & desc->flags;
}

/**
 * @note Address: 0x8006F654
 * @note Size: 0xE8
 */
extern "C" s32 fn_8006F654(const char* name)
{
	s32 index = 0;
	while (stageTable[index].fileName != NULL) {
		if (strcmp(name, stageTable[index].fileName) == 0)
			return index;
		++index;
	}
	++index;
	while (stageTable[index].fileName != NULL) {
		if (strcmp(name, stageTable[index].fileName) == 0)
			return index - StageCount() + 99;
		++index;
	}
	return -1;
}

static inline const char* StageMessage(s32 index, s32 member)
{
	StageDesc* desc = &stageTable[StageIndex(index)];
	s32 message     = desc->message;
	if (message >= 0) {
		u8* data   = lbl_803675CC.mData;
		u8* record = data + ReadResourceOffset(data + 0x38) + message * 8;
		return (const char*)(data + ReadResourceOffset(record + member));
	}
	return NULL;
}

/**
 * @note Address: 0x8006F73C
 * @note Size: 0xD0
 */
extern "C" const char* GetStageDesc(s32 index)
{
	return StageMessage(index, 4);
}

/**
 * @note Address: 0x8006F80C
 * @note Size: 0xCC
 */
extern "C" const char* GetStageName(s32 index)
{
	return StageMessage(index, 0);
}

/**
 * @note Address: 0x8006F8D8
 * @note Size: 0x4C
 */
extern "C" const char* GetStageFileName(s32 index)
{
	index = StageIndex(index);
	return stageTable[index].fileName;
}

/**
 * @note Address: 0x8006F924
 * @note Size: 0x28
 */
extern "C" s32 GetStageNum()
{
	return StageCount();
}

/**
 * @note Address: 0x8006F94C
 * @note Size: 0x128
 */
// __sinit_stage_cpp, builds lbl_803672CC and lbl_803675CC
