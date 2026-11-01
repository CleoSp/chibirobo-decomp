#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/memcard.h"
#include "game/menu.h"

/**
 * @note Address: 0x8003ADC8
 * @note Size: 0x294
 */
extern "C" void fn_8003ADC8(MenuSave* menu)
{
	CPad pad(0);
	if (menu->mResult == -3) {
		if (menu->mReady == 1) {
			for (s32 i = 0; i < 3; ++i)
				ClearSaveSlot(&menu->mSlots[i]);
			fn_80089890(&lbl_8025C400.mMemcard, menu->mChannel, TRUE);
			menu->SetMode(1);
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
			return;
		}
	} else if (menu->mReady == 0) {
		menu->ResetResults();
		return;
	}
	if (pad.mPad->mTrigger & 0x100) {
		if (menu->mResult == -3) {
			menu->ResetResults();
		} else if (menu->mResult == -5) {
			menu->SetMode(13);
			menu->mShowMessage = 1;
		} else {
			menu->SetMode(13);
		}
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
	}
}
