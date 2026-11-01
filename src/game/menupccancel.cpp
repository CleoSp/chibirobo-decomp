#include "game/menupc.h"
#include "game/cgame.h"
#include "game/csound.h"

/**
 * @note Address: 0x8004BBC0
 * @note Size: 0x288
 */
extern "C" void fn_8004BBC0(MenuPC* menu)
{
	CPad pad(0);
	if (pad.mPad->mTrigger & 0x200) {
		menu->ResetText();
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x86, 255);
	}
}
