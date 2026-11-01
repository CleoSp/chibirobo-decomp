#include "game/xchr.h"
#include "game/cgame.h"
#include <math.h>
#include "game/video.h"

#if defined(VERSION_PAL)
#define IDLE_SECOND lbl_80660D4C
#else
#define IDLE_SECOND 60
#endif

// todo: cleanup
static inline u8 DiskBusy(CGame* game)
{
	return (u8)((game->mDvdStatus || game->mPrevDvdStatus) && lbl_8025C400.mDiskOk == 0);
}

// todo: cleanup
static inline u8 InputEnabled(CGame* game)
{
	return (u8)((game->mStatus.mFlags & 1) && !(game->mStatus.mFlags & 0x22) && !DiskBusy(game));
}

// XObjChr::vf144, what chibi robo does when he's left alone: looks at the camera first,
// then two longer idles at 3000 and 6000
/**
 * @note Address: 0x8009D778
 * @note Size: 0x330
 */
extern "C" void fn_8009D778(XObjChr* chr, s32 input, s32 elapsed)
{
	s32 idle;
	if (lbl_8025C400._5D080 != NULL || !InputEnabled(&lbl_8025C400) || (lbl_8025C400.mStatus.mFlags & 0xC00)
	    || (lbl_8025C400.mStatus.mFlags & 0x100) || !(chr->_980.mFlags & 0x20000000) || chr->mCopter.mState != 0
	    || (chr->mPopper.mState != 0 && chr->mPopper.mState != 4) || chr->mRadar.mState != 0
	    || (!(idle = (u8)(chr->mLift.mState == 0)) && chr->mLift.mState != 4) || chr->mHang.mState != 0 || chr->mJump.mState != 0
	    || chr->mLadder.mState != 0 || chr->mRope.mState != 0 || chr->mPush.mState != 0 || chr->mTumble.mState != 0
	    || chr->mAttachmentRequest != -1 || chr->mSuitChanged != 0)
		return;
	if (input != -1)
		return;
	if (elapsed > 6000 && (idle || (chr->mLift.mState == 4 && fn_800D4FE8(&chr->mLift)))) {
		u8 release = chr->mLift.mState == 4 && fn_800D4FE8(&chr->mLift);
		if (release) {
			if (chr->mLift.mTarget) {
				chr->mLift.mTarget->mStatus.mFlags &= 0xFBFFFFFFFFFFFFFFULL;
				chr->mLift.mTarget = NULL;
			}
			chr->mLift.mState = 0;
		}
		chr->mJointRot[1].mMode    = 2;
		chr->mJointRot[1].mTarget  = 0.0f;
		chr->mJointRot[1].mLookAt  = NULL;
		chr->mTumble.mState        = 15;
		chr->mTumble.mFade.mActive = 1;
		chr->mTumble.mFade.mAmount = 0.0f;
		chr->_158BC                = 3 * IDLE_SECOND;
	} else if (elapsed > 3000) {
		chr->mTumble.mState       = 12;
		chr->mJointRot[1].mMode   = 2;
		chr->mJointRot[1].mTarget = 0.0f;
		chr->mJointRot[1].mLookAt = NULL;
		chr->_158B8               = 2;
		chr->_158BC               = IDLE_SECOND;
	} else {
		f32 angle = (f32)atan2(lbl_8025C400._E5DCC[0] - chr->mMoveTranslate.x, lbl_8025C400._E5DCC[2] - chr->mMoveTranslate.z)
		          - chr->mMoveRotate.y;
		chr->mJointRot[1].mMode   = 1;
		chr->mJointRot[1].mTarget = angle;
		chr->_158B8               = 1;
		chr->_158BC               = IDLE_SECOND;
	}
}
