#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/memcard.h"
#include "game/menu.h"

// todo: cleanup
static inline void RetryRead(MenuSave* menu)
{
	for (s32 i = 0; i < 3; ++i)
		ClearSaveSlot(&menu->mSlots[i]);
	fn_80089890(&lbl_8025C400.mMemcard, menu->mChannel, TRUE);
	menu->SetMode(1);
	fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
	menu->mOffset = 0.0f;
}

/**
 * @note Address: 0x8003B40C
 * @note Size: 0x3A0
 */
extern "C" void fn_8003B40C(MenuSave* menu)
{
	CPad pad(0);
	s32 result = menu->mResult;
	if (result == -3) {
		if (menu->_262 == 1) {
			RetryRead(menu);
			return;
		}
		if (menu->mReady == 1) {
			RetryRead(menu);
			return;
		}
	} else if (result == -2) {
		if (menu->_262 == 0) {
			menu->ResetResults();
			return;
		}
	} else if (menu->mReady == 0) {
		menu->ResetResults();
		return;
	}
	if (pad.mPad->mTrigger & 0x100) {
		if (result == -9) {
			menu->SetMode(12);
			menu->mMessage = 2;
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
			menu->mOffset = 0.0f;
		} else {
			menu->SetMode(16);
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
		}
	}
}
