#include "game/xchr.h"

extern "C" {
BOOL fn_8009FEDC(void* player);
void fn_800D7AC8(XObj* object);
}

/**
 * @note Address: 0x800C361C
 * @note Size: 0xC8
 */
extern "C" void fn_800C361C(XActCopter* act, const CPadOne* pad, f32* weight)
{
	if (act->mState == 3) {
		if ((pad->mTrigger & 0x200) || fn_8009FEDC(act->mOwner)) {
			act->mState = 4;
			fn_800D7AC8(&act->mOwner->mCopterObj);
		}
		*weight = 1.0f;
	} else if (act->mState == 4 || act->mState == 7 || act->mState == 8) {
		if (!(pad->mTrigger & 0x200))
			fn_8009FEDC(act->mOwner);
		*weight = 1.0f;
	} else if (act->mState != 0) {
		*weight = 0.0f;
	}
}
