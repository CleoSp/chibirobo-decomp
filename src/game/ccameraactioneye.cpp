#include "game/ccameraactioneye.h"
#include "game/gmath.h"
#include "Version.h"
#include "game/csound.h"
#include "game/cscrn.h"
#include "game/cgame.h"
#include "game/xchr.h"

#define EYE_PLAYER lbl_8025C400.mPlayer

extern "C" {
Vec fn_800A09BC(XObj* player);
}

// reversed for deferred inlining

/**
 * @note Address: 0x80147460
 * @note Size: 0x4
 */

/**
 * @note Address: 0x80147358
 * @note Size: 0x108
 */
void CCameraActionEye::vf14()
{
	mLook.x         = 0.0f;
	mLook.y         = 0.0f;
	mLook.z         = 0.0f;
	mLook.mTarget.x = 0.0f;
	mLook.mTarget.y = 0.0f;
	mLook.mTarget.z = 0.0f;
	mLook.mSpeed.x  = 0.0f;
	mLook.mSpeed.y  = 0.0f;
	mLook.mSpeed.z  = 0.0f;
	mOverride.x     = 0.0f;
	mOverride.y     = 0.0f;
	mOverride.z     = 0.0f;
	mLook.mSpeed.x  = 0.05f;
	mLook.mSpeed.y  = 0.05f;
	mLook.mSpeed.z  = 0.05f;

	Vec angle = { 0.05235988f, EYE_PLAYER->mMoveRotate.y, 0.0f };
	SetAngle(angle);
	SetDistance(100.0f);
	SetFov(40.0f);
	mLook.x         = 0.05235988f;
	mLook.y         = 0.0f;
	mLook.z         = 0.0f;
	mLook.mTarget.x = 0.05235988f;
	mLook.mTarget.y = 0.0f;
	mLook.mTarget.z = 0.0f;
}

// todo: cleanup
/**
 * @note Address: N/A
 * @note Size: ?
 */
inline bool PlayerCanLook()
{
	bool result = false;
	bool busy   = false;
	if (EYE_PLAYER->mLift.mState == 7 || EYE_PLAYER->mLift.mState == 8 || EYE_PLAYER->mLift.mState == 9 || EYE_PLAYER->mLift.mState == 10
	    || EYE_PLAYER->mLift.mState == 11 || EYE_PLAYER->mLift.mState == 12 || EYE_PLAYER->mLift.mState == 13
	    || EYE_PLAYER->mLift.mState == 14 || EYE_PLAYER->mLift.mState == 15 || EYE_PLAYER->mLift.mState == 16
	    || EYE_PLAYER->mLift.mState == 17 || EYE_PLAYER->mLift.mState == 18) {
		busy = true;
	}
	if (!busy && EYE_PLAYER->mHang.mState == 0 && EYE_PLAYER->mJump.mState == 0 && EYE_PLAYER->mLadder.mState == 0
	    && EYE_PLAYER->mRope.mState == 0 && EYE_PLAYER->mPush.mState == 0 && EYE_PLAYER->mTumble.mState == 0) {
		result = true;
	}
	return result;
}

// todo: cleanup
/**
 * @note Address: N/A
 * @note Size: ?
 */
inline f32 Clamp(f32 x, f32 max, f32 min)
{
	f32 v = x;
	if (x > max) {
		v = max;
	} else if (x < min) {
		v = min;
	}
	return v;
}

// todo: cleanup
/**
 * @note Address: N/A
 * @note Size: ?
 */
inline void PlayTurnSound()
{
	if (!fn_8001E6C8((CSound*)&lbl_8025C400.mSound, 3)) {
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 3, 0xFF);
	}
}

/**
 * @note Address: 0x801469EC
 * @note Size: 0x96C
 */
void CCameraActionEye::Update(u32 step)
{
	SetEye(fn_800A09BC(lbl_8025C400.mPlayer));
	CPad pad(0);

	if (!(lbl_8025C400.mStatus.mFlags & 0xC00)) {
		bool active = false;
		if (lbl_8025C400.mStatus.mFlags & 1) {
			if (!(lbl_8025C400.mStatus.mFlags & 0x22)) {
				bool held = false;
				if (lbl_8025C400.mDvdStatus != 0 || lbl_8025C400.mPrevDvdStatus != 0) {
					if (lbl_8025C400.mDiskOk == 0) {
						held = true;
					}
				}
				if (!held) {
					active = true;
				}
			}
		}
		if (active) {
			if ((pad.mPad->mTrigger & 0x40) && (pad.mPad->mButton & 0x20) && (lbl_8025C400.mStatus.mFlags & 0x80000000)) {
				lbl_8025C400.mStatus.mFlags ^= 0x200;
				if (lbl_8025C400.mStatus.mFlags & 0x200) {
					fn_80023438((CScrnWipe*)lbl_8025C400.mRoboeyes, 0x80, 0x80, 0xFF, 0xFF, 0x80, 0x80, 0xFF, 320.0f, 224.0f, 430.0f,
					            400.0f, 430.0f, 400.0f, 0.0f, 0xFF, 0);
				} else {
					fn_80023438((CScrnWipe*)lbl_8025C400.mRoboeyes, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 320.0f, 224.0f, 430.0f,
					            400.0f, 430.0f, 400.0f, 0.0f, 0xFF, 0);
				}
			}

			f32 zoom;
			if (lbl_8025C400.mStatus.mFlags & 0x200) {
				f32 x = Clamp(pad.mPad->mSubStickX, 0.8f, -0.8f);
				f32 y = Clamp(pad.mPad->mSubStickY, 0.8f, -0.8f);
				if (lbl_8025C400._5D080 != NULL || !PlayerCanLook()) {
					y = 0.0f;
					x = y;
				}
				mLook.mTarget.y = -HALF_PI * x;
				mLook.mTarget.x = -0.87266463f * y;
			} else {
				f32 turn = Clamp(pad.mPad->mStickX, 0.8f, -0.8f);
				f32 tilt = Clamp(pad.mPad->mStickY, 0.8f, -0.8f);
				if (lbl_8025C400._5D080 != NULL || !PlayerCanLook()) {
					tilt = 0.0f;
					turn = tilt;
				}
				f32 pitch       = 0.0f;
				mLook.mTarget.y = 0.0f;
				if (tilt > 0.0f) {
					pitch = mLook.x + 0.06981317f * Abs(tilt);
					if (pitch > 0.87266463f) {
						pitch = 0.87266463f;
					}
				} else if (tilt < 0.0f) {
					pitch = mLook.x - 0.06981317f * Abs(tilt);
					if (pitch < -0.87266463f) {
						pitch = -0.87266463f;
					}
				}
				if (mOverride.y != 0.0f) {
					mLook.mTarget.y = mOverride.y;
				}
				if (mOverride.x == 0.0f) {
					if (pitch != 0.0f) {
						mLook.mTarget.x = pitch;
						mLook.x         = pitch;
						PlayTurnSound();
					}
				} else {
					mLook.mTarget.x = mOverride.x;
				}
				lbl_8025C400.mPlayer->SetRotationY(EYE_PLAYER->mMoveRotate.y - (2.0f * DEG2RAD) * turn * step, 0);

				zoom = pad.mPad->mSubStickY;
				if (lbl_8025C400._5D080 != NULL || !PlayerCanLook()) {
					zoom = 0.0f;
				}
				if (mOverride.z == 0.0f) {
					mLook.mTarget.z = -30.0f * zoom;
				} else {
					mLook.mTarget.z = mOverride.z;
				}
			}

			f32 speedY     = Abs(mLook.mTarget.y - mLook.y) / 30.0f;
			f32 speedX     = Abs(mLook.mTarget.x - mLook.x) / 30.0f;
			f32 speedZ     = Abs(mLook.mTarget.z - mLook.z) / 20.0f;
			mLook.mSpeed.x = speedX;
			mLook.mSpeed.y = speedY;
			mLook.mSpeed.z = speedZ;
			if (RAD2DEG * speedY > 0.6f || RAD2DEG * speedX > 0.6f) {
				PlayTurnSound();
			}
			mLook.Step(step);

			Vec look = { mLook.x, mLook.y, 0.0f };
			SetAngle(EYE_PLAYER->mMoveRotate + look);
			SetFov(40.0f + mLook.z);

			static f32 sLastSpeed = 0.0f;
			f32 stick             = pad.mPad->mSubStickY;
			if (Abs(speedZ - sLastSpeed) < 0.02f) {
				lbl_8025C400._4294 = 0.0f;
				lbl_8025C400._4298 = 0.1f;
				lbl_8025C400._429C = lbl_8025C400._42A0 = 2.0f * -stick;
			} else {
				lbl_8025C400._4290 = 0.8f;
				lbl_8025C400._4294 = 0.0f;
				lbl_8025C400._4298 = 0.1f;
				lbl_8025C400._429C = lbl_8025C400._42A0 = 2.0f * -stick;
			}
			sLastSpeed = speedZ;
		}
	}
	lbl_8025C400._41F4 = mAngle.y;
}
