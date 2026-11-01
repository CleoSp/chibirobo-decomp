#include "game/ccameraaction.h"
#include "game/cgame.h"
#include "game/gmath.h"

#if !defined(VERSION_PAL)
extern "C" {
extern const char lbl_8020AA7C[];
extern const char lbl_8020AA8C[];
}
#endif

static inline bool CanTurn(CCameraActionLeticker* camera, f32 angle)
{
	if (!camera->mBounded)
		return true;
	Vec eye;
	camera->CalcEye(&eye, angle);
#if !defined(VERSION_PAL)
	OSReport(lbl_8020AA7C, eye.x, eye.z);
#endif
	if (eye.x < camera->mMinX || camera->mMaxX < eye.x || eye.z < camera->mMinZ || camera->mMaxZ < eye.z) {
#if !defined(VERSION_PAL)
		OSReport(lbl_8020AA8C, (s32)(RAD2DEG * camera->mTurnTo), eye.x, eye.z);
#endif
		return false;
	}
	return true;
}

/**
 * @note Address: 0x800E64A8
 * @note Size: 0x2E8
 */
extern "C" void fn_800E64A8(CCameraActionLeticker* camera, u32 frames)
{
	CPad pad(0);
	u64 flags = lbl_8025C400.mStatus.mFlags;
	u32 left  = (flags & 0x80000000ULL) ? 1 : 0x400000;
	u32 right = (flags & 0x80000000ULL) ? 2 : 0x800000;
	if (camera->mTurnFrames == 0) {
		if (pad.mPad->mTrigger & left) {
			if (CanTurn(camera, camera->mTurnTo - HALF_PI)) {
				camera->mTurnTo -= HALF_PI;
				camera->mTurnSpeed  = -(5.0f * DEG2RAD);
				camera->mTurnFrames = 18;
			}
		} else if (pad.mPad->mTrigger & right) {
			if (CanTurn(camera, HALF_PI + camera->mTurnTo)) {
				camera->mTurnTo += HALF_PI;
				camera->mTurnSpeed  = (5.0f * DEG2RAD);
				camera->mTurnFrames = 18;
			}
		}
	} else {
		camera->mTurnFrames -= frames;
		if (camera->mTurnFrames <= 0) {
			camera->mAngle      = camera->mTurnTo;
			camera->mTurnFrames = 0;
		} else {
			camera->mAngle += camera->mTurnSpeed * frames;
		}
	}
	camera->CCameraActionLadr::Update(frames);
}
