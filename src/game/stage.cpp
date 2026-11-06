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
// the globals file's name
extern const char* gGlobalResourceFileName;
extern s32 lbl_80215050[];
extern UnkStageObj gStageObjectPlacements[];
extern UnkStageObj spawnTable[];
extern UnkObjData objTable[];
extern UnkObjData objTableEx[];
extern u8 lbl_80216C30[];
// HSD_VIInfo, its header needs the hsd include path
extern u8 HSD_VIData[];
BOOL fn_8001F97C(XSound* sound, s32 stage);
}

Stage gStageResources;
Globals gGlobalScriptResources;

// reads the globals file and finds its three parts
/**
 * @note Address: 0x8006EC50
 * @note Size: 0x140
 */
extern "C" BOOL LoadGlobalResourceData()
{
#if defined(VERSION_JP)
	const char* path = gGlobalResourceFileName;
#else
	char path[0x40];
#if defined(VERSION_PAL)
	sprintf(path, "bin%s/%s", lbl_8022AE28[gGame.mLanguage], gGlobalResourceFileName);
#else
	sprintf(path, "bin%s/%s", lbl_802244C8[gGame.mLanguage], gGlobalResourceFileName);
#endif
#endif
	CFile* file = &gGlobalScriptResources.mFile;
	if (file->ReadEx(path, 0, 0)) {
		gGlobalScriptResources.mFile.CloseHandle();
		gGlobalScriptResources.mHeader  = (u8*)gGlobalScriptResources.mFile.mBuffer;
		u8* header                      = gGlobalScriptResources.mHeader;
		gGlobalScriptResources.mData    = header + ReadResourceOffset(header) + 0x18;
		gGlobalScriptResources.mText    = header + ReadResourceOffset(header + 8) + 0x18;
		gGlobalScriptResources.mScripts = header + ReadResourceOffset(header + 16) + 0x18;
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x8006ED90
 * @note Size: 0x24
 */
extern "C" s32 GetItemIconTextureIndex(s32 index)
{
	if (index < 0)
		return -1;
	return lbl_80215050[index];
}

/**
 * @note Address: 0x8006EDB4
 * @note Size: 0x14
 */
extern "C" UnkStageObj* GetStageObjectPlacementByIndex(s32 index)
{
	return &gStageObjectPlacements[index];
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
extern "C" UnkObjData* GetObjData(const CoordInfo* info)
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
extern "C" BOOL AdvanceStageLoad(Stage* stage, s32 index)
{
	switch (stage->mState) {
	case 0: {
#if defined(VERSION_JP)
		strcpy(stage->mDataPath, FormatToStaticBuffer("bin/%s.bin", stageTable[index].fileName));
#elif defined(VERSION_PAL)
		StageDesc* desc = &stageTable[index];
		strcpy(stage->mDataPath, FormatToStaticBuffer("bin%s/%s.bin", lbl_8022AE28[gGame.mLanguage], desc->fileName));
#else
		strcpy(stage->mDataPath, FormatToStaticBuffer("bin%s/%s.bin", lbl_802244C8[gGame.mLanguage], stageTable[index].fileName));
#endif
#if defined(VERSION_PAL)
		strcpy(stage->mModulePath, FormatToStaticBuffer("rel/%s%s.rel", desc->moduleName, ""));
#else
		strcpy(stage->mModulePath, FormatToStaticBuffer("rel/%s%s.rel", stageTable[index].fileName, ""));
#endif
		if (!(gGame.mStatus.mFlags & 0x100000000ULL))
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
		if (fn_8001F97C(&gGame.mSound, index))
			++stage->mState;
		break;
	}
	return stage->mState > 4;
}

/**
 * @note Address: 0x8006F338
 * @note Size: 0x58
 */
extern "C" s32 CallStageModule(s32 function, s32 count, s32* args)
{
	if (IsModuleLoaded(&gStageResources.mModule))
		return gStageResources.CallModule(function, count, args);
	return -1;
}

/**
 * @note Address: 0x8006F390
 * @note Size: 0x1C
 */
extern "C" s32 GetStageCollectionFlagBase(s32 index)
{
	return ((s16*)(gStageResources.mEnvironment.mData + 0xA))[index];
}

/**
 * @note Address: 0x8006F3AC
 * @note Size: 0x10
 */
extern "C" CamAreaRecord* fn_8006F3AC()
{
	return (CamAreaRecord*)gStageResources._1A8;
}

/**
 * @note Address: 0x8006F3BC
 * @note Size: 0x10
 */
extern "C" CamInfoData* GetStageCameraInfoRecords()
{
	return (CamInfoData*)gStageResources._1A4;
}

/**
 * @note Address: 0x8006F3CC
 * @note Size: 0x10
 */
extern "C" CamAreaRecord* fn_8006F3CC()
{
	return (CamAreaRecord*)gStageResources._1A0;
}

/**
 * @note Address: 0x8006F3DC
 * @note Size: 0x10
 */
extern "C" void* fn_8006F3DC()
{
	return gStageResources._19C;
}

/**
 * @note Address: 0x8006F3EC
 * @note Size: 0x10
 */
extern "C" void* fn_8006F3EC()
{
	return gStageResources._194;
}

/**
 * @note Address: 0x8006F3FC
 * @note Size: 0x10
 */
extern "C" void* fn_8006F3FC()
{
	return gStageResources._190;
}

/**
 * @note Address: 0x8006F40C
 * @note Size: 0x10
 */
extern "C" void* fn_8006F40C()
{
	return gStageResources._18C;
}

/**
 * @note Address: 0x8006F41C
 * @note Size: 0x10
 */
extern "C" void* fn_8006F41C()
{
	return gStageResources._188;
}

/**
 * @note Address: 0x8006F42C
 * @note Size: 0x10
 */
extern "C" void* fn_8006F42C()
{
	return gStageResources._184;
}

/**
 * @note Address: 0x8006F43C
 * @note Size: 0x54
 */
extern "C" u8* GetStageScriptByIndex(s32 index)
{
	u32 offset = ReadResourceOffset(gStageResources.mScriptOffsets + index * 4);
	return offset == 0 ? NULL : (u8*)gStageResources.mDataFile.mBuffer + offset;
}

/**
 * @note Address: 0x8006F490
 * @note Size: 0x10
 */
extern "C" u8* GetStageDataBuffer()
{
	return (u8*)gStageResources.mDataFile.mBuffer;
}

/**
 * @note Address: 0x8006F4A0
 * @note Size: 0x18
 */
extern "C" UnkStageObj* GetStagePlacementByIndex(s32 index)
{
	return gStageResources.mPlacements._00 + index;
}

/**
 * @note Address: 0x8006F4B8
 * @note Size: 0x10
 */
extern "C" EnvInfo* GetStageEnvironment()
{
	return &gStageResources.mEnvironment;
}

/**
 * @note Address: 0x8006F4C8
 * @note Size: 0x1C
 */
extern "C" BOOL IsStageDataLoaded()
{
	return gStageResources.mDataFile.mBuffer != NULL;
}

/**
 * @note Address: 0x8006F4E4
 * @note Size: 0x60
 */
extern "C" void fn_8006F4E4()
{
	GXColor color                  = *(GXColor*)gStageResources.mEnvironment.mData;
	*(GXColor*)(HSD_VIData + 0x48) = color; // current.vi.bg_color
	gStageResources.mModule.Print();
}

/**
 * @note Address: 0x8006F544
 * @note Size: 0x90
 */
extern "C" void UnloadStageDataAndModule()
{
	if (IsModuleLoaded(&gStageResources.mModule))
		gStageResources.CallModule(6, 0, NULL);
	gStageResources.mModule.Unload();
	CFile* file = &gStageResources.mDataFile;
	file->Close();
	gStageResources.mState = 0;
}

/**
 * @note Address: 0x8006F5D4
 * @note Size: 0x2C
 */
extern "C" BOOL LoadStageByIndex(s32 index)
{
	return AdvanceStageLoad(&gStageResources, index);
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
extern "C" s32 FindStageIndexByName(const char* name)
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
		u8* data   = gGlobalScriptResources.mData;
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
// __sinit_stage_cpp, builds gStageResources and gGlobalScriptResources
