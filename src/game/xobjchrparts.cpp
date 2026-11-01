#include "game/xchr.h"
#include "game/cgame.h"
#include <math.h>

// one of the numbers that float up when he gets points, ten of them at most
struct PointPop {
	u8 _00[0xC - 0x0];    // _00
	s32 mState;           // _0C
	u8 _10[0x74 - 0x10];  // _10
	s32 mPending;         // _74
	u8 _78[0x61C - 0x78]; // _78

	// todo: cleanup
	u8 IsFree() const { return mState == 0 && mPending == 0; }
};
STATIC_ASSERT(sizeof(PointPop) == 0x61C);

extern "C" {
void fn_80091C44(CXObj* object, u32* flags);
void fn_8008B0D8(CXObj* object, s32 mode);
void fn_80095AD8(PointPop* pop, s32 type, s32 value);
extern PointPop lbl_80378300[10];
extern PointPop* lbl_8037C018[10];
}

/**
 * @note Address: 0x800B5808
 * @note Size: 0x4
 */
extern "C" void fn_800B5808()
{
}

/**
 * @note Address: 0x800B580C
 * @note Size: 0x168
 */
extern "C" void fn_800B580C(XObjChr* chr, BOOL enabled, u32 value)
{
	fn_80091834(chr, enabled, value);
	if (chr->mPlug.mJObj != NULL)
		chr->mPlug.vfDC(enabled, value);
	if (chr->mCopterObj.mJObj != NULL)
		chr->mCopterObj.vfDC(enabled, value);
	if (chr->mPopperObj.mJObj != NULL)
		chr->mPopperObj.vfDC(enabled, value);
	if (chr->mRadarObj.mJObj != NULL)
		chr->mRadarObj.vfDC(enabled, value);
	if (chr->mOXObj.mJObj != NULL)
		chr->mOXObj.vfDC(enabled, value);
	for (s32 i = 0; i < 9; ++i) {
		if (chr->mSuitObj[i].mJObj != NULL)
			chr->mSuitObj[i].vfDC(enabled, value);
	}
}

/**
 * @note Address: 0x800B5974
 * @note Size: 0x144
 */
extern "C" void fn_800B5974(XObjChr* chr, u32* flags)
{
	fn_80091C44(chr, flags);
	if (chr->mPlug.mJObj != NULL)
		chr->mPlug.vfD8(flags);
	if (chr->mCopterObj.mJObj != NULL)
		chr->mCopterObj.vfD8(flags);
	if (chr->mPopperObj.mJObj != NULL)
		chr->mPopperObj.vfD8(flags);
	if (chr->mRadarObj.mJObj != NULL)
		chr->mRadarObj.vfD8(flags);
	if (chr->mOXObj.mJObj != NULL)
		chr->mOXObj.vfD8(flags);
	for (s32 i = 0; i < 9; ++i) {
		if (chr->mSuitObj[i].mJObj != NULL)
			chr->mSuitObj[i].vfD8(flags);
	}
}

/**
 * @note Address: 0x800B5AB8
 * @note Size: 0xC4
 */
extern "C" void fn_800B5AB8(XObjChr* chr, s32 mode)
{
	fn_8008B0D8(chr, mode);
	chr->mPlug.vf120(mode);
	chr->mCopterObj.vf120(mode);
	chr->mPopperObj.vf120(mode);
	chr->mRadarObj.vf120(mode);
	chr->mOXObj.vf120(mode);
}

/**
 * @note Address: 0x800B5B7C
 * @note Size: 0xC0
 */
extern "C" void fn_800B5B7C(XObjChr*, s32 type, s32 value)
{
	for (s32 i = 0; i < 10; ++i) {
		if (lbl_80378300[i].IsFree()) {
			for (s32 j = 0; j < 10; ++j) {
				if (lbl_8037C018[j] == NULL) {
					lbl_8037C018[j] = &lbl_80378300[i];
					fn_80095AD8(lbl_8037C018[j], type, value);
					return;
				}
			}
			return;
		}
	}
}

// todo: cleanup
static inline u8 IsRopeState(XObjChr* chr, s32 state)
{
	return chr->mRope.mState == state;
}

/**
 * @note Address: 0x800B5C3C
 * @note Size: 0x1E0
 */
extern "C" void fn_800B5C3C(XObjChr* chr, u32 frames)
{
	if (chr->mHeadOffsetReady == 0 && lbl_8025C400._5D080 == NULL)
		chr->mHeadOffsetReady = 1;
	f32 target, rate;
	if (chr->mPush.mState == 1 || chr->mPush.mState == 2) {
		if (chr->mPush._0C == 1)
			target = 8.0f;
		else if (chr->mPush._0C == -1)
			target = -5.0f;
		else
			target = 0.0f;
		rate = 0.4f;
	} else if (lbl_8025C400._5D080 != NULL && chr->mHeadOffsetReady != 0) {
		target = 0.0f;
		rate   = 0.2f;
	} else if (IsRopeState(chr, 1) || IsRopeState(chr, 2) || IsRopeState(chr, 3) || IsRopeState(chr, 4) || IsRopeState(chr, 5)
	           || IsRopeState(chr, 6) || IsRopeState(chr, 7) || IsRopeState(chr, 8)) {
		target = 0.0f;
		rate   = 0.2f;
	} else {
		target = 5.0f;
		rate   = 0.2f;
	}
	if (__fpclassifyf(target) != FP_NAN) {
		f32 current = chr->mHeadOffset;
		if (current > target) {
			current -= rate * frames;
			chr->mHeadOffset = current;
			if (current < target)
				chr->mHeadOffset = target;
		} else if (current < target) {
			current += rate * frames;
			chr->mHeadOffset = current;
			if (current > target)
				chr->mHeadOffset = target;
		}
	}
}

/**
 * @note Address: 0x800B5E1C
 * @note Size: 0x18
 */
extern "C" void fn_800B5E1C(XObjChr* chr)
{
	chr->mHeadOffset      = 5.0f;
	chr->mHeadOffsetReady = 0;
}
