#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include <stdio.h>
#include "game/csound.h"
#include "game/memcard.h"
#include "game/menu.h"

extern "C" {
extern MenuMCTest lbl_803657E8;
}

// todo: cleanup
static inline void PlaySound(s32 effect)
{
	fn_8001E904((CSound*)&lbl_8025C400.mSound, effect, 255);
}

/**
 * @note Address: 0x80031AD8
 * @note Size: 0x3F4
 */
extern "C" BOOL fn_80031AD8(MenuMCTestSub* menu, u32 frames)
{
	if ((s32)fn_800617DC(menu, frames) == 4) {
		CPad pad(0);
		u32 repeat = pad.mPad->mRepeat;
		if (repeat & 0x10008) {
			do {
				if (--menu->mSelection < 0)
					menu->mSelection = 3;
			} while (menu->mText[menu->mSelection][0] == lbl_8065C440[0]);
			PlaySound(0x87);
		} else if (repeat & 0x20004) {
			do {
				if (++menu->mSelection > 3)
					menu->mSelection = 0;
			} while (menu->mText[menu->mSelection][0] == lbl_8065C440[0]);
			PlaySound(0x87);
		} else if (pad.mPad->mTrigger & 0x200) {
			menu->Close();
			lbl_803657E8.Resume();
			PlaySound(0x86);
		} else if (pad.mPad->mTrigger & 0x100) {
			s32 action              = lbl_80212450[menu->mSelection].mAction;
			Memcard* card           = &lbl_8025C400.mMemcard;
			SavePreviewRecord* save = &card->mSaves[0][lbl_803657E8.mSelection];
			if ((action == 4 || action == 6) && save->mState == 0) {
				PlaySound(0x85);
				goto finished;
			}
			if (action == 4 && save->mState != 0 && (save->mStatus != 0 || !fn_80086F24(save))) {
				PlaySound(0x85);
				goto finished;
			}
			menu->Close();
			lbl_803657E8.Resume();
			switch (action) {
			case 0:
				lbl_803657E8.mOperation = 0;
				break;
			case 1:
				fn_80089890(card, lbl_803622B4.mChannel, TRUE);
				lbl_803657E8.mOperation = 1;
				break;
			case 2:
				sprintf(lbl_803657E8.mText[lbl_803657E8.mSelection], "(%d) saving...", lbl_803657E8.mSelection);
				fn_8008A1F4(card, lbl_803622B4.mChannel, lbl_803657E8.mSelection, TRUE);
				lbl_803657E8.mOperation = 3;
				break;
			case 4:
				sprintf(lbl_803657E8.mText[lbl_803657E8.mSelection], "(%d) loading...", lbl_803657E8.mSelection);
				fn_80089C90(card, lbl_803622B4.mChannel, lbl_803657E8.mSelection, TRUE);
				lbl_803657E8.mOperation = 5;
				break;
			case 6:
				sprintf(lbl_803657E8.mText[lbl_803657E8.mSelection], "(%d) deleting...", lbl_803657E8.mSelection);
				fn_80088B4C(card, lbl_803622B4.mChannel, lbl_803657E8.mSelection, TRUE);
				lbl_803657E8.mOperation = 7;
				break;
			}
			PlaySound(0x88);
		}
	finished:;
	}
	return menu->IsOpen();
}
