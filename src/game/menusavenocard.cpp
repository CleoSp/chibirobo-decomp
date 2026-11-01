#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/menu.h"

/**
 * @note Address: 0x8003B254
 * @note Size: 0x1B8
 */
extern "C" void fn_8003B254(MenuSave* menu)
{
	CPad pad(0);
	if (menu->mResult == -2) {
		if (menu->_262 == 0) {
			menu->ResetResults();
			return;
		}
	} else if (menu->mReady == 0) {
		menu->ResetResults();
		return;
	}
	if (pad.mPad->mTrigger & 0x100) {
		menu->SetMode(16);
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
	}
}
