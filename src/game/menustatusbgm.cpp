#include "game/menu.h"
#include "game/cgame.h"
#include "game/csound.h"

/**
 * @note Address: 0x8005B39C
 * @note Size: 0x9C
 */
extern "C" void fn_8005B39C(MenuStatus* menu)
{
	s32 bgm;
	if (lbl_8025C400.mSound._2C4 <= 0)
		bgm = lbl_8025C400.mSound._2B4;
	else
		bgm = 0;
	menu->mSavedBgm       = bgm;
	menu->mSavedBgmVolume = lbl_8025C400.mSound._2CC;
	fn_8001E904((CSound*)&lbl_8025C400.mSound, -65532, 255);
	menu->mPrevMode = menu->mMode;
	menu->mMode     = 4;
	menu->mNextMode = 1;
	menu->mPhase    = 0;
}
