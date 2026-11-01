#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/memcard.h"
#include "game/video.h"
#include "game/menu.h"

/**
 * @note Address: 0x8003C598
 * @note Size: 0x200
 */
extern "C" void fn_8003C598(MenuSave* menu)
{
	if (menu->mReady == 0) {
		menu->ResetResults();
		return;
	}
	CPad pad(0);
	u32 repeat = pad.mPad->mRepeat;
	if (repeat & 0x40001) {
		if (menu->mChoice > 0) {
			--menu->mChoice;
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x87, 255);
		}
	} else if (repeat & 0x80002) {
		if (menu->mChoice < 1) {
			menu->mChoice++;
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x87, 255);
		}
	}
	if (pad.mPad->mTrigger & 0x100) {
		switch (menu->mChoice) {
		case 0:
			fn_8008A1F4(&lbl_8025C400.mMemcard, menu->mChannel, menu->mFile, TRUE);
			menu->SetMode(5);
			menu->mOffset = -32.0f;
#if defined(VERSION_PAL)
			menu->mDelay = 1.5f * lbl_80660D4C;
#else
			menu->mDelay = 90;
#endif
			break;
		case 1:
			menu->SetMode(2);
			break;
		}
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
	}
}
