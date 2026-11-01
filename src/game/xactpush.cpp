#include "game/xchr.h"
#include "game/cvec.h"
#include <math.h>

extern "C" Vec lbl_8020A650;

static inline void ReleaseTarget(XActPush* act)
{
	XObj* target = act->mTarget;
	if (target != NULL && (target->mStatus.mFlags & 0x0800000000000000ULL)) {
		target->mStatus.mFlags &= ~0x0800000000000000ULL;
	}
}

/**
 * @note Address: 0x800C591C
 * @note Size: 0xE4
 */
extern "C" void fn_800C591C(XActPush* act)
{
	if (act->mState != 0) {
		ReleaseTarget(act);
		act->Reset();
		act->mState    = 0;
		XObjChr* owner = act->mOwner;
		owner->SetAnim(17, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
		owner->_A2C = 3;
	}
}

/**
 * @note Address: 0x800C5A00
 * @note Size: 0x7C
 */
extern "C" void fn_800C5A00(XActPush* action)
{
	ReleaseTarget(action);
	action->Reset();
}

/**
 * @note Address: 0x800C5A7C
 * @note Size: 0x11C
 */
// walks him around the target at the same distance
extern "C" void fn_800C5A7C(XActPush* act, f32 angle)
{
	if (act->mTarget != NULL) {
		Vec flat = lbl_8020A650;
		flat.x   = act->_10.x;
		flat.z   = act->_10.z;
		CVec offset(flat);
		f32 radius = offset.Length();
		f32 sine   = (f32)sin(angle);
		act->mOwner->SetTranslateX(act->mTarget->mMoveTranslate.x - radius * sine, 0);
		f32 cosine = (f32)cos(angle);
		act->mOwner->SetTranslateZ(act->mTarget->mMoveTranslate.z - radius * cosine, 0);
	}
}
