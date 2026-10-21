#include "game/ccameraactioneye.h"
#include "game/gmath.h"
#include "Version.h"

// game object fields, by offset
extern "C" u8 lbl_8025C400[];
#define GAME_FLAGS    (*(u64*)(lbl_8025C400 + 0x1B10))
#define GAME_PLAYER   (*(XObj**)(lbl_8025C400 + 0x96BC))
#define GAME_SCREEN   ((void*)(lbl_8025C400 + 0x4198))
#define GAME_SOUND    ((void*)(lbl_8025C400 + 0xE7E24))
#define GAME_DEMO     (*(void**)(lbl_8025C400 + 0x5D080))
#define GAME_WORD(o)  (*(s32*)(lbl_8025C400 + (o)))
#define GAME_FLOAT(o) (*(f32*)(lbl_8025C400 + (o)))

#define EYE_PLAYER ((CameraPlayer*)GAME_PLAYER)

extern "C" {
Vec fn_800A09BC(XObj* player);
void fn_80023438(void* screen, s32 r0, s32 g0, s32 b0, s32 r1, s32 g1, s32 b1, s32 a, f32 x, f32 y, f32 x0, f32 y0, f32 x1, f32 y1, f32 z,
                 s32 a1, s32 flag);
BOOL fn_8001E6C8(void* sound, s32 id);
void fn_8001E904(void* sound, s32 id, s32 volume);
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

	Vec angle = { 0.05235988f, EYE_PLAYER->mRot.y, 0.0f };
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
	if (EYE_PLAYER->mState == 7 || EYE_PLAYER->mState == 8 || EYE_PLAYER->mState == 9 || EYE_PLAYER->mState == 10
	    || EYE_PLAYER->mState == 11 || EYE_PLAYER->mState == 12 || EYE_PLAYER->mState == 13 || EYE_PLAYER->mState == 14
	    || EYE_PLAYER->mState == 15 || EYE_PLAYER->mState == 16 || EYE_PLAYER->mState == 17 || EYE_PLAYER->mState == 18) {
		busy = true;
	}
	if (!busy && EYE_PLAYER->_AC4 == 0 && EYE_PLAYER->_B14 == 0 && EYE_PLAYER->_B68 == 0 && EYE_PLAYER->_BAC == 0 && EYE_PLAYER->_BEC == 0
	    && EYE_PLAYER->_C44 == 0) {
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
	if (!fn_8001E6C8(GAME_SOUND, 3)) {
		fn_8001E904(GAME_SOUND, 3, 0xFF);
	}
}

/**
 * @note Address: 0x801469EC
 * @note Size: 0x96C
 */
void CCameraActionEye::Update(u32 step)
{
	SetEye(fn_800A09BC(GAME_PLAYER));
	CPad pad(0);

	if (!(GAME_FLAGS & 0xC00)) {
		bool active = false;
		if (GAME_FLAGS & 1) {
			if (!(GAME_FLAGS & 0x22)) {
				bool held = false;
				if (GAME_WORD(0x331C) != 0 || GAME_WORD(0x3320) != 0) {
					if (GAME_WORD(0x3324) == 0) {
						held = true;
					}
				}
				if (!held) {
					active = true;
				}
			}
		}
		if (active) {
			if ((pad.mStatus->_14 & 0x40) && (pad.mStatus->_04 & 0x20) && (GAME_FLAGS & 0x80000000)) {
				GAME_FLAGS ^= 0x200;
				if (GAME_FLAGS & 0x200) {
					fn_80023438(GAME_SCREEN, 0x80, 0x80, 0xFF, 0xFF, 0x80, 0x80, 0xFF, 320.0f, 224.0f, 430.0f, 400.0f, 430.0f, 400.0f, 0.0f,
					            0xFF, 0);
				} else {
					fn_80023438(GAME_SCREEN, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 320.0f, 224.0f, 430.0f, 400.0f, 430.0f, 400.0f, 0.0f,
					            0xFF, 0);
				}
			}

			f32 zoom;
			if (GAME_FLAGS & 0x200) {
				f32 x = Clamp(pad.mStatus->_3C, 0.8f, -0.8f);
				f32 y = Clamp(pad.mStatus->_40, 0.8f, -0.8f);
				if (GAME_DEMO != NULL || !PlayerCanLook()) {
					y = 0.0f;
					x = y;
				}
				mLook.mTarget.y = -HALF_PI * x;
				mLook.mTarget.x = -0.87266463f * y;
			} else {
				f32 turn = Clamp(pad.mStatus->_34, 0.8f, -0.8f);
				f32 tilt = Clamp(pad.mStatus->_38, 0.8f, -0.8f);
				if (GAME_DEMO != NULL || !PlayerCanLook()) {
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
				GAME_PLAYER->vf28(EYE_PLAYER->mRot.y - 0.034906585f * turn * step, 0);

				zoom = pad.mStatus->_40;
				if (GAME_DEMO != NULL || !PlayerCanLook()) {
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
			if (57.29578f * speedY > 0.6f || 57.29578f * speedX > 0.6f) {
				PlayTurnSound();
			}
			mLook.Step(step);

			Vec look = { mLook.x, mLook.y, 0.0f };
			SetAngle(EYE_PLAYER->mRot + look);
			SetFov(40.0f + mLook.z);

			static f32 sLastSpeed = 0.0f;
			f32 stick             = pad.mStatus->_40;
			if (Abs(speedZ - sLastSpeed) < 0.02f) {
				GAME_FLOAT(0x4294) = 0.0f;
				GAME_FLOAT(0x4298) = 0.1f;
				GAME_FLOAT(0x429C) = GAME_FLOAT(0x42A0) = 2.0f * -stick;
			} else {
				GAME_FLOAT(0x4290) = 0.8f;
				GAME_FLOAT(0x4294) = 0.0f;
				GAME_FLOAT(0x4298) = 0.1f;
				GAME_FLOAT(0x429C) = GAME_FLOAT(0x42A0) = 2.0f * -stick;
			}
			sLastSpeed = speedZ;
		}
	}
	GAME_FLOAT(0x41F4) = mAngle.y;
}
