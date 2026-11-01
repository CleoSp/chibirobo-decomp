#include "game/xchr.h"
#include "game/cgame.h"
#include "game/csound.h"

// closes both rings, 0 cancels, above 0 confirms, below 0 backs out
/**
 * @note Address: 0x80097284
 * @note Size: 0xF4
 */
extern "C" void fn_80097284(XObjChrMenu* menu, s32 result)
{
	if (result == 0) {
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x86, 255);
		if (menu->mAtc._280 != 3)
			menu->mSel = menu->mPrevSel;
	} else if (result > 0) {
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
	} else if (result < 0) {
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x86, 255);
		if (menu->mAtc._280 != 3)
			menu->mSel = menu->mPrevSel;
	}
	menu->mAtc.Select(result);
	menu->mDrs.Select(result);
}
