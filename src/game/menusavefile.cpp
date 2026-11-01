#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/memcard.h"
#include "game/video.h"
#include "game/menu.h"

/**
 * @note Address: 0x8003C798
 * @note Size: 0x294
 */
extern "C" void fn_8003C798(MenuSave* menu)
{
	if (menu->mReady == 0) {
		menu->ResetResults();
		return;
	}
	CPad pad(0);
	u32 repeat = pad.mPad->mRepeat;
	if (repeat & 0x40001) {
		menu->mFile--;
		if (menu->mFile < 0)
			menu->mFile = 2;
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x87, 255);
	} else if (repeat & 0x80002) {
		menu->mFile++;
		if (menu->mFile > 2)
			menu->mFile = 0;
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x87, 255);
	}
	u32 trigger = pad.mPad->mTrigger;
	if (trigger & 0x100) {
		if (menu->mSlots[menu->mFile].mUsed == 1U) {
			menu->mChoice = 1;
			menu->SetMode(3);
		} else if (menu->_250 >= 6 && menu->_254 > 0) {
			fn_8008A1F4(&lbl_8025C400.mMemcard, menu->mChannel, menu->mFile, TRUE);
			menu->SetMode(5);
#if defined(VERSION_PAL)
			menu->mDelay = 1.5f * lbl_80660D4C;
#else
			menu->mDelay = 90;
#endif
		} else {
			menu->mResult = -9;
			menu->SetMode(11);
			menu->mOffset = -32.0f;
		}
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
	} else if (trigger & 0x200) {
		menu->SetMode(16);
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x86, 255);
	}
}
