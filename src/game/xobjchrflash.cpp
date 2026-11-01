#include "game/xchr.h"
#include "game/cgame.h"
#include "game/video.h"

// hit, flinches when he's free to and starts the flash timer
/**
 * @note Address: 0x800A0014
 * @note Size: 0x220
 */
extern "C" void fn_800A0014(XObjChr* chr, BOOL start, BOOL cancel, f32 value)
{
	if (!(lbl_8025C400.mStatus.mFlags & 0xC00ULL) && chr->mAttachmentRequest < 0 && lbl_8025C400._5D080 == NULL) {
		BOOL idle;
		if (start && ((idle = chr->mLift.mState == 0) || chr->mLift.mState == 4) && chr->mHang.mState == 0 && chr->mJump.mState == 0
		    && chr->mLadder.mState == 0 && chr->mRope.mState == 0 && chr->mPush.mState == 0 && chr->mTumble.mState == 0
		    && chr->mCopter.mState == 0 && (chr->mPopper.mState == 0 || chr->mPopper.mState == 4 || chr->mPopper.mState == 8)
		    && chr->mRadar.mState == 0) {
			if (idle)
				chr->SetAnim(0xD4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			else
				chr->SetAnim(0x94, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			chr->mTumble.mState = 8;
		} else if (cancel) {
			if (chr->mHang.mState == 5)
				chr->mHang.mState = 8;
			else if (chr->mLadder.mState == 5)
				chr->mLadder.mState = 8;
			else if (chr->mRope.mState == 5)
				chr->mRope.mState = 7;
		}
	}
	if (0.0f != value) {
#if defined(VERSION_PAL)
		chr->_A34 = lbl_80660D4C;
#else
		chr->_A34 = 60;
#endif
		chr->_A38 = 0;
		chr->_A3C = value;
	}
}

// XObjChr::vf12C, the color goes to everything he wears and carries too. while a costume
// change is running, blend 2 turns into 1 and 1 is dropped
/**
 * @note Address: 0x800A0234
 * @note Size: 0x1E0
 */
extern "C" void fn_800A0234(XObjChr* chr, s32 blend, u8 red, u8 green, u8 blue, f32 time)
{
	if (chr->mSuitChanged != 0) {
		if (blend == 2)
			blend = 1;
		else if (blend == 1)
			return;
	}
	fn_8008A820(chr, blend, red, green, blue, time);
	chr->mPlug.vf12C(blend, red, green, blue, time);
	chr->mCopterObj.vf12C(blend, red, green, blue, time);
	chr->mPopperObj.vf12C(blend, red, green, blue, time);
	chr->mRadarObj.vf12C(blend, red, green, blue, time);
	chr->mOXObj.vf12C(blend, red, green, blue, time);
	for (s32 i = 0; i < 9; ++i)
		chr->mSuitObj[i].vf12C(blend, red, green, blue, time);
	if (fn_800D4D54(&chr->mLift))
		chr->mLift.mTarget->vf12C(blend, red, green, blue, time);
}
