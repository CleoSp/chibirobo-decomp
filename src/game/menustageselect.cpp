#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/menu.h"

// todo: cleanup
static inline void PlayMenuSound(s32 effect)
{
	fn_8001E904((CSound*)&lbl_8025C400.mSound, effect, 255);
}

/**
 * @note Address: 0x800336DC
 * @note Size: 0x38C
 */
extern "C" BOOL fn_800336DC(MenuStageSelect* menu, u32 frames)
{
	if ((s32)fn_800617DC(menu, frames) == 4) {
		CPad pad(0);
		u32 repeat = pad.mPad->mRepeat;
		if (repeat & 0x10008) {
			do {
				if (--menu->mSelection < 0)
					menu->mSelection = menu->mCount - 1;
			} while (menu->mLabels[menu->mSelection][0] == lbl_8065C440[0]);
			PlayMenuSound(0x87);
		} else if (repeat & 0x20004) {
			do {
				if (++menu->mSelection > menu->mCount - 1)
					menu->mSelection = 0;
			} while (menu->mLabels[menu->mSelection][0] == lbl_8065C440[0]);
			PlayMenuSound(0x87);
		} else if (repeat & 0x40001) {
			if (menu->mSelection - 20 >= 0 && menu->mLabels[menu->mSelection - 20][0] != lbl_8065C440[0]) {
				menu->mSelection -= 20;
				PlayMenuSound(0x87);
			}
		} else if (repeat & 0x80002) {
			if (menu->mSelection + 20 < menu->mCount && menu->mLabels[menu->mSelection + 20][0] != lbl_8065C440[0]) {
				menu->mSelection += 20;
				PlayMenuSound(0x87);
			}
		} else if (pad.mPad->mTrigger & 0x200) {
			menu->Close();
			if (lbl_803622B4.mSuspended)
				lbl_8025C400.mStatus.mFlags |= 2ULL;
			lbl_803622B4.mSuspended   = FALSE;
			lbl_803622B4.mSuspendFade = 0;
			PlayMenuSound(0x86);
		} else if (pad.mPad->mTrigger & 0x100) {
			menu->Close();
			lbl_803622B4.CMenu::Close();
			PlayMenuSound(0x86);
			lbl_8025C400.mStatus.mFlags &= ~2ULL;
			if (lbl_8025C400.mStatus.mFlags & 0x0008000000000000ULL) {
				fn_8000B394(&lbl_8025C400, lbl_8021240C[menu->mSelection], 1);
			} else {
				fn_8000B394(&lbl_8025C400, menu->mSelection, 1);
			}
			PlayMenuSound(0x88);
		}
	}
	return menu->IsOpen();
}
