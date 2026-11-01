#include "game/hsd_jobj.h"
#include "game/title.h"
#include <stdlib.h>

extern "C" void fn_80030CE8(Title* title);

/**
 * @note Address: 0x80030EE4
 * @note Size: 0x120
 */
extern "C" void fn_80030EE4(Title* title)
{
	fn_80030CE8(title);
	title->mDemo.vf90(0);
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
