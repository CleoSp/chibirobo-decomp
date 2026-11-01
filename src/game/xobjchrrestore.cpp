#include "game/xchr.h"
#include "game/xobjs.h"
#include "game/cgame.h"
#include "game/stagedata.h"

/**
 * @note Address: 0x800B78FC
 * @note Size: 0x260
 */
// picks up again what he was holding before the reset, true once it's back in his hands
extern "C" BOOL fn_800B78FC(XObjChr* chr)
{
	if (chr->mLift.mState != 0) {
		XObj* target = chr->mLift.mTarget;
		if (target != NULL) {
			if ((s16)target->GetPool() == 24) {
				XObjPlug* plug          = &chr->mPlug;
				chr->mLift.mTarget      = plug;
				chr->mLift.mTargetFloor = chr->mLift.mTarget->_784;
				plug->mStatus.mFlags |= 0x0400000000000000ULL;
				chr->SetAnim(8, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				return TRUE;
			}
		}
		fn_800D5034(&chr->mLift);
	}

	s16 id = chr->mSavedTarget;
	if (id >= 0) {
		XObj* loaded        = fn_80132698((UnkXObjList*)lbl_8025C400._9694, &CoordInfo(fn_8006EDB4(id)), 0, TRUE);
		chr->mPendingTarget = loaded;
		chr->mSavedTarget   = -1;
	}
	if (chr->mPendingTarget != NULL) {
		if (!(chr->mPendingTarget->mStatus.mFlags & 0x0040000000000000ULL)) {
			fn_800D5034(&chr->mLift);
			chr->mLift.mState = 4;
			chr->mLift.SetTarget(chr->mPendingTarget);
			chr->mLift._24 = 4;
			chr->mLift.mTarget->SetAnim(chr->mSavedTargetAnim, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			chr->mLift.mTarget->mStatus.mFlags |= 0x0400000000000000ULL;
			chr->SetAnim(8, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			chr->mPendingTarget = NULL;
			if (fn_800D4C78(&chr->mLift)) {
				fn_800D482C(&chr->mLift);
			}
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}
