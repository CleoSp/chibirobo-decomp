#include "game/xchr.h"
#include "game/gmath.h"

/**
 * @note Address: 0x800BE648
 * @note Size: 0xF0
 */
// fades the radar's marker in while it has something, and spins it
extern "C" void fn_800BE648(XActRadar* act, u32 frames)
{
	if (act->mState == 0 && act->_2C != NULL && act->mOwner->mTumble.mState == 0) {
		if (act->mFade < 1.0f) {
			f32 fade   = act->mFade + 0.05f * frames;
			act->mFade = fade;
			if (fade > 1.0f)
				act->mFade = 1.0f;
		}
		f32 angle = 5.0f * DEG2RAD * frames;
		act->mAngle[0] -= angle;
		act->mAngle[1] -= angle;
	} else if (act->mFade > 0.0f) {
		f32 fade   = act->mFade - 0.05f * frames;
		act->mFade = fade;
		if (fade < 0.0f)
			act->mFade = 0.0f;
	}
}
