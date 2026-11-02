#ifndef _GAME_STAGE_H
#define _GAME_STAGE_H

#include "game/cmodule.h"
#include "game/coordinfo.h"
#include "Version.h"
#include "game/stagedata.h"

struct StageDesc {
	const char* fileName;
#if defined(VERSION_PAL)
	const char* moduleName;
#endif
	s32 message;
	u32 flags;
};
extern "C" StageDesc stageTable[];

// the stage file's environment block, class name from RTTI
class EnvInfo {
public:
	u8* mData; // _00
	// _04 vtable

	EnvInfo(u8* data)
	    : mData(data)
	{
	}
	virtual ~EnvInfo() { } // _08
};
STATIC_ASSERT(sizeof(EnvInfo) == 8);

typedef s32 (*StageFunction)(s32 function, s32 count, s32* args);

// the loaded stage, its file and rel, class name from RTTI
class Stage {
public:
	Stage()
	    : mEnvironment(NULL)
	    , mPlacements(NULL)
	{
		mState = 0;
	}

	char mDataPath[0x40];   // _00
	char mModulePath[0x40]; // _40
	s32 mState;             // _80
	CFile mDataFile;        // _84
	EnvInfo mEnvironment;   // _170
	CoordInfo mPlacements;  // _178
	u8* mScriptOffsets;     // _180
	void* _184;             // _184
	void* _188;             // _188
	void* _18C;             // _18C
	void* _190;             // _190
	void* _194;             // _194
	void* _198;             // _198
	void* _19C;             // _19C
	void* _1A0;             // _1A0
	void* _1A4;             // _1A4
	void* _1A8;             // _1A8
	CModule mModule;        // _1AC
	// _2F0 vtable, the 12 bytes after it in the bss symbol are Globals' dtor chain entry

	virtual ~Stage() { } // _08

	// calls into the stage's rel, -1 without one
	s32 CallModule(s32 function, s32 count, s32* args)
	{
		if (mModule.IsLoaded())
			return ((StageFunction)mModule.mModule->prolog)(function, count, args);
		return -1;
	}
};
STATIC_ASSERT(sizeof(Stage) == 0x2F4);
extern "C" Stage sStage;

static inline bool IsModuleLoaded(CModule* module)
{
	return module->IsLoaded();
}

#endif
