#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/memcard.h"
#include "game/video.h"
#include "game/menu.h"

// todo: cleanup
static inline void MoveChoice(MenuSave* menu, CPad* pad)
{
	u32 repeat = pad->mPad->mRepeat;
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
}

// todo: cleanup
static inline void CancelFormat(MenuSave* menu)
{
	menu->SetMode(13);
	fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
#if !defined(VERSION_JP)
	menu->_538 = TRUE;
#endif
}

/**
 * @note Address: 0x8003BB18
 * @note Size: 0x378
 */
extern "C" void fn_8003BB18(MenuSave* menu)
{
	CPad pad(0);
	if (menu->mReady == 0) {
		menu->ResetResults();
		return;
	}
	switch (menu->mPhase) {
	case 0:
		MoveChoice(menu, &pad);
		if (pad.mPad->mTrigger & 0x100) {
			if (menu->mChoice == 0) {
				fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
				menu->mChoice = 1;
				menu->mPhase++;
			} else {
				CancelFormat(menu);
			}
			return;
		}
		return;
	case 1:
		MoveChoice(menu, &pad);
		if (pad.mPad->mTrigger & 0x100) {
			if (menu->mChoice == 0) {
				fn_80088E78(&lbl_8025C400.mMemcard, menu->mChannel, TRUE);
				menu->SetMode(9);
#if defined(VERSION_PAL)
				menu->mDelay = 1.5f * lbl_80660D4C;
#else
				menu->mDelay = 90;
#endif
				fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
			} else {
				CancelFormat(menu);
			}
			return;
		}
	}
}
