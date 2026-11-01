#include "game/xobjarmy.h"
#include "game/cgame.h"
#include "game/xchr.h"
#include "game/csound.h"
#include "game/globals.h"
extern "C" {
#include <dolphin/mtx/vec.h>
}

/**
 * @note Address: 0x800E6E78
 * @note Size: 0x158
 */
extern "C" void fn_800E6E78(XObjArmy* army, f32 strength)
{
	if (!(army->mFlags.mFlags & 1)) {
		army->_994 = 0.0f;
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x83, 255);
		if (army->_994 <= 0.0f) {
			if ((s32)fn_8006EBC8(31) / 100.0f <= strength
			    || PSVECDistance(&army->mMoveTranslate, &lbl_8025C400.mPlayer->mMoveTranslate) < 20.0f) {
				army->mStep = 0;
				army->mFunc = lbl_8022A6B0[2];
			} else {
				army->mStep = 0;
				army->mFunc = lbl_8022A6B0[3];
			}
		} else {
			army->mStep = 0;
			army->mFunc = lbl_8022A6B0[4];
		}
	}
}
