#include "game/menupc.h"
#include "game/cgame.h"
#include "game/csound.h"

/**
 * @note Address: 0x8004B92C
 * @note Size: 0x294
 */
extern "C" void fn_8004B92C(MenuPC* menu)
{
	CPad pad(0);
	if (pad.mPad->mTrigger & 0x200) {
		lbl_8025C400._43D0 = 0;
		menu->ResetText();
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x86, 255);
	}
}
