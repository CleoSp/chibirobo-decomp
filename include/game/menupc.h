#ifndef _GAME_MENUPC_H
#define _GAME_MENUPC_H

#include "game/menu.h"
#include <string.h>

#if !defined(VERSION_JP)
extern "C" {
extern f32 lbl_8065B1A0;
extern u8 lbl_803620A8[512];
}
#endif

// back to mode 2 with an empty line of text
inline void MenuPC::ResetText()
{
	mPrevMode = mMode;
	mMode     = 2;
	mPhase    = 0;
	_16C      = 0;
	mTextX    = 210.0f;
	mTextY    = 129.0f;
	mTargetX  = mTextX;
	mTargetY  = mTextY;
#if !defined(VERSION_JP)
	switch (mMode) {
	case 3:
	case 7:
	case 10:
		mTextY   = 121.0f;
		mTargetY = mTextY;
		break;
	}
#endif
	mCharIndex = 0;
	_378       = 0;
	_37C       = 0;
	// a full width space
	char blank[3] = "\x81\x40";
	u16 space     = *(u16*)blank;
	for (u32 i = 0; i < 40; ++i)
		mChars[i] = space;
	mTerminator = 0;
	for (u32 i = 0; i < 40; ++i) {
		mCharX[i]             = 640.0f;
		mCharFlags[i]         = 0;
		*(u16*)mCharCopies[i] = space;
		mCharCopies[i][2]     = 0;
	}
	_364 = 0;
	_368 = 0;
	_36C = 0;
	_370 = 0;
	_374 = 0;
#if !defined(VERSION_JP)
	lbl_8065B1A0 = 640.0f;
	memset(lbl_803620A8, 0, sizeof(lbl_803620A8));
#endif
}

#endif
