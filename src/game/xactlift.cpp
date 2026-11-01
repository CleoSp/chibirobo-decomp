#include "game/xact.h"

// todo: cleanup
static inline bool TargetHasFlag(XObj* target)
{
	return target != NULL && (u8)((target->mStatus.mFlags & 0x0000400000000000ULL) != 0);
}

// todo: cleanup
static inline bool MatchesTarget(XActLift* action, s16 kind)
{
	XObj* target = action->mTarget;
	return TargetHasFlag(target) && (s16)target->GetPool() == 8 && (s16)target->GetObjFlags() == kind && action->_24 != 15;
}

/**
 * @note Address: 0x800D4B60
 * @note Size: 0x118
 */
extern "C" u8 fn_800D4B60(XActLift* action)
{
	return MatchesTarget(action, 22) && (action->mState == 31 || action->mState == 32 || action->mState == 33);
}

/**
 * @note Address: 0x800D4C78
 * @note Size: 0xDC
 */
extern "C" u8 fn_800D4C78(XActLift* action)
{
	return MatchesTarget(action, 56);
}

/**
 * @note Address: 0x800D4D54
 * @note Size: 0xDC
 */
extern "C" BOOL fn_800D4D54(XActLift* action)
{
	return MatchesTarget(action, 22);
}

/**
 * @note Address: 0x800D4E30
 * @note Size: 0xDC
 */
extern "C" u8 fn_800D4E30(XActLift* action)
{
	return MatchesTarget(action, 21);
}

/**
 * @note Address: 0x800D4F0C
 * @note Size: 0xDC
 */
extern "C" u8 fn_800D4F0C(XActLift* action)
{
	return MatchesTarget(action, 16);
}

/**
 * @note Address: 0x800D4FE8
 * @note Size: 0x4C
 */
extern "C" BOOL fn_800D4FE8(XActLift* action)
{
	return action->mTarget != NULL && (s16)action->mTarget->GetPool() == 24;
}

// todo: cleanup
static inline void ClearTargetFlags(XActLift* action)
{
	if (action->mTarget != NULL) {
		action->mTarget->mStatus.mFlags &= ~0x0000000000010000ULL;
		action->mTarget->mStatus.mFlags &= ~0x0400000000000000ULL;
	}
}

/**
 * @note Address: 0x800D5034
 * @note Size: 0xA0
 */
extern "C" void fn_800D5034(XActLift* action)
{
	ClearTargetFlags(action);
	action->Reset();
	action->_60 = 0;
	action->_6C = 0;
	action->_74 = 0;
}

/**
 * @note Address: 0x800D50D4
 * @note Size: 0xBC
 */
extern "C" void fn_800D50D4(XActLift* action, XObjChr* owner)
{
	action->Init(owner);
	action->_2C = 0.0f;
	action->_48 = -1;
	ClearTargetFlags(action);
	action->Reset();
	action->_60 = 0;
	action->_6C = 0;
	action->_74 = 0;
}
