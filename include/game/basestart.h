#ifndef _GAME_BASESTART_H
#define _GAME_BASESTART_H

#include "game/cgame.h"

extern "C" {
void OSInitStopwatch(void* sw, char* name);
void fn_8001D8C0();
void InitGameRenderState(CBase* base);
void InitGameARAM();
void InitGameDVD();
#if defined(VERSION_PAL)
u8 OSGetLanguage();
// os language and game language pairs
extern u8 lbl_802162E0[12];
#endif
}

static inline void StartGameBase(CBase* base)
{
	fn_8001D8C0();
	InitGameRenderState(base);
	InitGameARAM();
	InitGameDVD();
#if defined(VERSION_PAL)
	u8 language = OSGetLanguage();
	for (s32 i = 0; i < 6; ++i) {
		if (language == lbl_802162E0[i * 2]) {
			base->mLanguage = lbl_802162E0[i * 2 + 1];
			break;
		}
	}
#elif !defined(VERSION_JP)
	base->mLanguage = 0;
#endif
#if defined(VERSION_JP)
	Init__5CFontFv(&base->mFont);
#else
	Init__5CFontFi(&base->mFont, gGame.mLanguage != 6);
#endif
	base->mPad.Init();
	base->mLastRetrace   = 0;
	base->mRetraceStep   = 0;
	base->mStatus.mFlags = 0;
	InitGamePerspectiveCamera(&base->mCameras[0], &base->mGs.mScreen);
	InitGameOrthoCamera(&base->mCameras[1], &base->mGs.mScreen);
	InitFourLightSet(&base->mLights[0]);
	InitTwoLightSet(&base->mLights[4]);
	OSInitStopwatch(&base->mStopwatch[0], "__$dsync$ ");
	OSInitStopwatch(&base->mStopwatch[1], "__$update$");
	OSInitStopwatch(&base->mStopwatch[2], "__$draw$  ");
	base->mPrevDvdStatus = 0;
	base->mDvdStatus     = 0;
	base->mDiskOk        = 1;
	base->mDvdError      = 0;
	base->mResetHold     = 0;
	base->mReset         = 0;
}

#endif
