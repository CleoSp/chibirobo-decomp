#include "game/stage.h"
#include "game/globals.h"
#include "game/cgame.h"
#include "Dolphin/mtx.h"
#include "game/ut.h"
#include "Dolphin/GX/GXTypes.h"
#include <string.h>

extern "C" {
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

typedef s32 (*StageFunction)(s32 function, s32 count, s32* args);

static inline s32 CallStageFunction(s32 function, s32 count, s32* args)
{
	if (lbl_803672CC.mModule.mModule != NULL) {
		return ((StageFunction)lbl_803672CC.mModule.mModule->prolog)(function, count, args);
	}
	return -1;
}

/**
 * @note Address: 0x8006F338
 * @note Size: 0x58
 */
extern "C" s32 fn_8006F338(s32 function, s32 count, s32* args)
{
	if (lbl_803672CC.mModule.IsLoaded())
		return CallStageFunction(function, count, args);
	return -1;
}

/**
 * @note Address: 0x8006F390
 * @note Size: 0x1C
 */
extern "C" s32 fn_8006F390(s32 index)
{
	return *(s16*)(lbl_803672CC.mEnvironment.mData + index * 2 + 0xA);
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
	if (lbl_803672CC.mModule.IsLoaded())
		CallStageFunction(6, 0, NULL);
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
	index += StageCount();
	index -= 99;
	return index;
}

/**
 * @note Address: 0x8006F600
 * @note Size: 0x54
 */
extern "C" u32 CheckStageFlag(s32 index, u32 mask)
{
	return stageTable[StageIndex(index)].flags & mask;
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
	s32 message = stageTable[StageIndex(index)].message;
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
	return stageTable[StageIndex(index)].fileName;
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
