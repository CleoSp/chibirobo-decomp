#include "game/xchr.h"
#include "game/xobjmap.h"
#include "game/cgame.h"
#include <math.h>

// the ground type under chibi robo, from what he hangs or climbs on, the object he stands
// on, or else the map's floor color. 13 and up count as none
/**
 * @note Address: 0x8009DAA8
 * @note Size: 0x1DC
 */
extern "C" s32 fn_8009DAA8(XObjChr* chr)
{
	s32 type = 0;
	if (chr->mHang.mState == 5 && chr->mHang.mTarget != NULL) {
		XObj* owner = chr->mHang.mTarget;
		type        = owner->mFloors->mType;
	} else if (chr->mRope.mState == 5 && chr->mRope.mTarget != NULL) {
		XObj* owner = chr->mRope.mTarget;
		type        = owner->mFloors->mType;
	} else if (chr->mLadder.mState == 5 && chr->mLadder.mTarget != NULL) {
		XObj* owner = chr->mLadder.mTarget;
		type        = owner->mFloors->mType;
	} else if (chr->_9D4[0] >= 0) {
		XObj* owner = lbl_8025C400.mObjects[chr->_9D4[0]];
		f32 best    = NAN;
		for (s32 i = 0; owner->mFloors[i].mId != -1; ++i) {
			if ((owner->_250[0] & (1U << i)) && (__fpclassifyf(best) == FP_NAN || owner->_6FC[i] > best)) {
				f32 height = owner->_6FC[i];
				if (height <= 1.0f + chr->mMoveTranslate.y) {
					best = height;
					type = owner->mFloors[i].mType;
				}
			}
		}
	}
	type = (u8)type;
	if (type == 0 && chr->_788 >= 0 && (s32)chr->_78C >= 0) {
		type = fn_80093210((XObjMap*)lbl_8025C400.mPools[0].mObjects, (s32)chr->_78C, chr->_788);
	}
	if (type >= 13)
		type = 0;
	return type;
}

// the highest floor of the object under a floor group that's still below chibi robo,
// stops early on the preferred one, -1 for none
/**
 * @note Address: 0x8009DC84
 * @note Size: 0x134
 */
extern "C" s32 fn_8009DC84(XObjChr* chr, s32 group, s32 preferred)
{
	s32 result = -1;
	if (chr->_9D4[group] >= 0) {
		XObj* owner = lbl_8025C400.mObjects[chr->_9D4[group]];
		f32 best    = NAN;
		for (s32 i = 0; owner->mFloors[i].mId != -1; ++i) {
			if ((owner->_250[group] & (1U << i)) && (__fpclassifyf(best) == FP_NAN || owner->_6FC[i] >= best)) {
				f32 height = owner->_6FC[i];
				if (height <= 1.0f + chr->mMoveTranslate.y) {
					best   = height;
					result = i;
					if (i == preferred)
						break;
				}
			}
		}
	}
	return result;
}
