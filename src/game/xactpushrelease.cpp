#include "game/xchr.h"
#include "game/cgame.h"

static inline bool IsGameRunning()
{
	bool active = false;
	if (lbl_8025C400.mStatus.mFlags & 1ULL) {
		if (!(lbl_8025C400.mStatus.mFlags & 0x22ULL)) {
			bool held = false;
			if (lbl_8025C400.mDvdStatus != 0 || lbl_8025C400.mPrevDvdStatus != 0) {
				if (lbl_8025C400.mDiskOk == 0)
					held = true;
			}
			if (!held)
				active = true;
		}
	}
	return active;
}

static inline void ReleaseTarget(XActPush* act)
{
	XObj* target = act->mTarget;
	if (target != NULL && (target->mStatus.mFlags & 0x0800000000000000ULL)) {
		target->mStatus.mFlags &= ~0x0800000000000000ULL;
	}
}

/**
 * @note Address: 0x800C56D4
 * @note Size: 0x248
 */
// lets go of what he's pushing once a is let go, japan uses b while the game flag is up
extern "C" void fn_800C56D4(XActPush* act, const CPadOne* pad, f32* amount)
{
	if (act->mState == 2) {
		if (IsGameRunning() && lbl_8025C400._5D080 == NULL) {
#if defined(VERSION_JP)
			if (lbl_8025C400.mStatus.mFlags & 0x0000080000000000ULL) {
				if (pad->mTrigger & 0x200) {
					ReleaseTarget(act);
					act->Reset();
					act->mState = 3;
					*amount     = 0.0f;
				}
			} else
#endif
			    if (!(pad->mButton & 0x100)) {
				ReleaseTarget(act);
				act->Reset();
				act->mState = 3;
				*amount     = 0.0f;
			}
		}
	} else if (act->mState != 0) {
		if (act->mState != 3 && !(pad->mButton & 0x100)
#if defined(VERSION_JP)
		    && !(lbl_8025C400.mStatus.mFlags & 0x0000080000000000ULL)
#endif
		) {
			ReleaseTarget(act);
			act->Reset();
			XObjChr* owner = act->mOwner;
			owner->SetAnim(17, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			owner->_A2C = 3;
			act->mState = 0;
		}
		*amount = 0.0f;
	}
}
