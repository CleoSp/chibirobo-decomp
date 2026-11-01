#include "game/menu.h"
#include "game/cgame.h"
#include "game/csound.h"

/**
 * @note Address: 0x80049418
 * @note Size: 0xF0
 */
extern "C" void fn_80049418(MenuPC* menu)
{
	if (menu->mFade <= 2.0f) {
		lbl_8025C400._5D08C = menu->mExitState;
		menu->Close();
		// MenuMain::Resume
		if (lbl_803622B4.mSuspended)
			lbl_8025C400.mStatus.mFlags |= 2ULL;
		lbl_803622B4.mSuspended   = FALSE;
		lbl_803622B4.mSuspendFade = FALSE;
		CSound* sound             = (CSound*)&lbl_8025C400.mSound;
		fn_8001E904(sound, 0x86, 255);
		lbl_8025C400.mStatus.mFlags &= ~2ULL;
		fn_8001E660(sound, 0x30008);
	}
}
