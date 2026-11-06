#include "game/cgame.h"
#include "game/globals.h"
#include "game/menusave.h"
#include "game/csound.h"
#include "game/title.h"
#include <stdlib.h>

#pragma section RX ".text.titleinit"

/**
 * @note Address: 0x80030CE8
 * @note Size: 0x1FC
 */
extern "C" __declspec(section ".text.titleinit") void ResetTitleScreenState(Title* title)
{
	title->mState   = 0;
	title->mMode    = 0;
	title->mTimeout = -1;
	title->_10      = 0.0f;
	title->_14      = 0.0f;
	title->_18      = 90.0f;
	title->_1C      = 0;
	title->_20      = 0;
	title->mCursor.InitializeObject(0);
	title->mCursor.mAngle      = 0.0f;
	title->mCursor.mAngleSpeed = 0.0f;
	title->_37A8               = 0;
	title->mFile               = 0;
	title->mChoice             = 0;
	title->_37E4               = 0.0f;
	title->mAlpha              = 0.0f;
	title->mPrevMode           = 0;
	title->mPhase              = 0;
	title->mDelay              = 0;
	title->_37B0               = -10.0f;
	title->_37B4               = -200.0f;
	title->_37B8               = -10.0f;
	title->_37BC               = -200.0f;
	title->_37C0               = 0.0f;
	title->_37C4               = 0.0f;
	title->_37C8               = 0.0f;
	title->mOffset             = 0.0f;
	title->_380C               = 0;
	title->_08                 = 0;
	title->_3810               = 0.0f;
	title->_3814               = 0.0f;
	CPad pad(0);
	if (pad.mPad->mConnected != 0) {
		if (gGame.mStatus.mFlags & 0x800000000ULL)
			title->_3818 = 1;
		else
			title->_3818 = 0;
	} else {
		title->_3818 = 0;
	}
	if (gGame.mStatus.mFlags & 0x1000000000ULL)
		title->_3819 = 1;
	else
		title->_3819 = 0;
	title->mChannel = 0;
	for (s32 i = 0; i < 3; ++i)
		ResetSaveSlotRecord(&gTitleSaveSlots[i]);
	title->_37AC = 0.0f;
	title->_3809 = 0;
	title->_3808 = 0;
	KeyOffGameSoundTrack(&gGame.mSound, 1);
#if !defined(VERSION_JP)
	title->_3C44 = 0;
#endif
}

/**
 * @note Address: 0x80030EE4
 * @note Size: 0x120
 */
extern "C" __declspec(section ".text.titleinit") void InitTitleScreen(Title* title)
{
	ResetTitleScreenState(title);
	title->mDemo.InitializeObject(0);
	for (s32 i = 0; i < 16; ++i) {
		title->mDust[i].mState  = 0;
		title->mDust[i].mTimer  = 0;
		title->mDust[i].mX      = rand() % 640;
		title->mDust[i].mY      = rand() % 480;
		title->mDust[i].mSpeedX = 0.0f;
		title->mDust[i].mSpeedY = 0.0f;
	}
#if !defined(VERSION_JP)
	title->_3C44 = 0;
#endif
}
