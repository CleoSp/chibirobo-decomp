#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include <stdlib.h>
#include "game/gmath.h"
#include "game/csound.h"
#include "game/globals.h"
#include "game/video.h"
#include "game/menu.h"

// todo: cleanup
static inline bool IsLeadByte(u8 c)
{
	u8 lead = 0;
	if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xFC))
		lead = 1;
	return lead;
}

/**
 * @note Address: 0x8003D350
 * @note Size: 0x3AC
 */
extern "C" void fn_8003D350(MenuSave* menu)
{
	menu->mState = 1;
	menu->Resume();
	menu->mWindow.mOpen = TRUE;
	lbl_8025C400.mStatus.mFlags |= 2;
	menu->mWindow.mWidth  = 0.0f;
	menu->mWindow.mHeight = 0.0f;
	menu->mWindow.mX      = 0.0f;
	menu->mWindow.mY      = 0.0f;
	menu->_158            = -10.0f;
	menu->_15C            = -200.0f;
	menu->_160            = -10.0f;
	menu->_164            = 0.0f;
	menu->_168            = 0.0f;
	menu->_16C            = 0.0f;
	menu->_170            = 0.0f;
	menu->mOffset         = 0.0f;
	menu->_264            = 0.0f;
	menu->mAlpha          = 1.0f;
	menu->mMode           = 0;
	menu->mPrevMode       = menu->mMode;
	menu->mPhase          = 0;
	menu->mTime           = 0.0f;
	menu->mDelay          = 0;
	if (menu->mLastFile == -1)
		menu->mFile = lbl_8025C400._9108;
	else
		menu->mFile = menu->mLastFile;
	menu->mChoice      = 0;
	menu->mResult      = 0;
	menu->mMessage     = 0;
	menu->mReady       = 0;
	menu->mShowMessage = 0;
	menu->_262         = 0;
	menu->mSaved       = 0;
	menu->mChannel     = 0;
	menu->_2A4         = 0.0f;
#if !defined(VERSION_JP)
	menu->_538 = FALSE;
#endif
	fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x14B, 255);
	u32 color = 0xFFFFFF00 | (u32)(255.0f * menu->mAlpha);
#if defined(VERSION_JP)
	const char* text = fn_8006DBF0(0x3D);
#else
	const char* text = fn_8006DBF0(0x62);
#endif
	s32 i             = 0;
	menu->mTitleState = 1;
	menu->mTitleCount = 0;
	if (text != NULL) {
		menu->mTitleColor = color;
		for (; i < 20 && *text != 0; ++i, ++text) {
			menu->mTitle[i].mText  = text;
			menu->mTitle[i].mDelay = -0.2f * i;
#if defined(VERSION_PAL)
			menu->mTitle[i].mTime = (f32)lbl_80660D4C;
#else
			menu->mTitle[i].mTime = 60.0f;
#endif
			menu->mTitle[i].mAngle0 = DEG2RAD * (rand() % 360);
			menu->mTitle[i].mAngle1 = DEG2RAD * (rand() % 360);
			menu->mTitleCount++;
#if !defined(VERSION_JP)
			if (lbl_8025C400.mLanguage == 6)
#endif
			{
				if (IsLeadByte(*text))
					++text;
			}
		}
		for (s32 i = menu->mTitleCount; i < 20; ++i) {
			menu->mTitle[i].mText  = NULL;
			menu->mTitle[i].mDelay = 0.0f;
			menu->mTitle[i].mTime  = 0.0f;
		}
	}
}
