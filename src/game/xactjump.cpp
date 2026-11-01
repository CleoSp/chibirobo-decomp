#include "game/xchr.h"
#include "game/xobjflags.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/inventory.h"

static inline s16 GetPool(XObj* item)
{
	return item->GetPool();
}

static inline s16 GetFlags(XObj* item)
{
	UnkObjData* data = item->mAttr._00;
	return data != NULL ? GetObjDataFlags(data) : -1;
}

static inline bool IsHeld(XObj* item)
{
	return item != NULL && (u8)((item->mStatus.mFlags & 0x0000400000000000ULL) != 0);
}

static inline bool IsLiftingItem(XActLift* lift)
{
	XObj* item = lift->mTarget;
	return IsHeld(item) && GetPool(item) == 8 && GetFlags(item) == 22 && lift->_24 != 15;
}

/**
 * @note Address: 0x800C9D54
 * @note Size: 0x360
 */
// the jump's charge, a full one throws what he holds
extern "C" void fn_800C9D54(XActJump* act, f32 strength)
{
	if (lbl_8025C400._5D080 != NULL) {
		act->mStrength = act->mTargetStrength = 0.0f;
		return;
	}
	act->mTargetStrength = strength;
	act->mStrength       = strength;
	if (act->mTargetStrength >= 50.0f) {
		XObjChr* chr = act->mOwner;
		if (chr->mLift.mState != 0 && IsLiftingItem(&chr->mLift)) {
			XObjChr* owner = chr->mLift.mOwner;
			if (owner->mAnimId == 0x8B1 || owner->mAnimId == 0x8B3) {
				owner->SetAnim(0x8B2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (owner->mAnimId == 11) {
				owner->SetAnim(8, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
			chr->mLift.mState              = 0;
			chr->mLift.mOwner->mAttachment = 0;
			chr->mLift.mTarget->mStatus.mFlags &= ~0x0200000000000000ULL;
			chr->mLift.mTarget->SetAnim(7, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			chr->mLift._24   = 20;
			XObjFlags* item  = (XObjFlags*)chr->mLift.mTarget;
			UnkObjData* data = item->mAttr._00;
			s32 id           = data != NULL ? GetObjDataFlags(data) : -1;
			fn_800DC9B0(&chr->mLift.mOwner->mParams.mInventory, id, 1, TRUE);
			item->SetFlag();
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x39, 255);
			if (act->mTargetStrength >= 60.0f) {
				act->mState                 = 0;
				act->mOwner->mTumble.mState = 1;
				act->mOwner->SetAnim(0x35, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
		}
	}
}
