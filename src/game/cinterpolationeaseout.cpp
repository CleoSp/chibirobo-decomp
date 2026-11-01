#include "game/hsd_jobj.h"
#include "game/ccameraaction.h"

/**
 * @note Address: 0x80135118
 * @note Size: 0x8
 */
extern "C" f32 fn_80135118(CInterpolationEaseOut* interp)
{
	return interp->mValue;
}

/**
 * @note Address: 0x80135120
 * @note Size: 0x7C
 */
extern "C" bool fn_80135120(CInterpolationEaseOut* interp, f32 dt)
{
	interp->mTime -= dt;
	f32 elapsed    = interp->mDuration - interp->mTime;
	interp->mSpeed = (interp->mAccel * dt) * 0.5f + interp->mSpeed;
	if (interp->mTime <= 0.0f) {
		interp->mValue = interp->mTarget;
	} else {
		interp->mValue = interp->mSpeed * elapsed + interp->mStart;
	}
	return interp->mTime <= 0.0f;
}

/**
 * @note Address: 0x8013519C
 * @note Size: 0x84
 */
extern "C" bool fn_8013519C(CInterpolationEaseOut* interp, u32 step, f32 dt)
{
	bool done = false;
	for (u32 i = 0; i < step; ++i) {
		done = interp->Step(dt);
		if (done == true) {
			break;
		}
	}
	return done;
}

/**
 * @note Address: 0x80135220
 * @note Size: 0x58
 */
extern "C" bool fn_80135220(CInterpolationEaseOut* interp, f32 from, f32 to, f32 time)
{
	if (time <= 0.0f) {
		return false;
	}
	interp->mValue    = from;
	interp->mStart    = from;
	interp->mTarget   = to;
	interp->mSpeed    = (2.0f * (to - from)) / time;
	interp->mAccel    = -interp->mSpeed / time;
	interp->mDuration = time;
	interp->mTime     = time;
	return true;
}

/**
 * @note Address: 0x80135278
 * @note Size: 0x8
 */
extern "C" f32 fn_80135278(CInterpolationEaseInOut* interp)
{
	return interp->mValue;
}

/**
 * @note Address: 0x80135280
 * @note Size: 0x94
 */
extern "C" bool fn_80135280(CInterpolationEaseInOut* interp, f32 dt)
{
	interp->mTime -= dt;
	if (interp->mTime < interp->mDuration / 2) {
		interp->mSpeed -= interp->mAccel * dt;
	} else {
		interp->mSpeed += interp->mAccel * dt;
	}
	if (interp->mTime <= 0.0f) {
		interp->mValue = interp->mTarget;
	} else {
		interp->mValue += interp->mSpeed * dt;
	}
	return interp->mTime <= 0.0f;
}

/**
 * @note Address: 0x80135314
 * @note Size: 0x84
 */
extern "C" bool fn_80135314(CInterpolationEaseInOut* interp, u32 step, f32 dt)
{
	bool done = false;
	for (u32 i = 0; i < step; ++i) {
		done = interp->Step(dt);
		if (done == true) {
			break;
		}
	}
	return done;
}

/**
 * @note Address: 0x80135398
 * @note Size: 0x4C
 */
extern "C" bool fn_80135398(CInterpolationEaseInOut* interp, f32 from, f32 to, f32 time)
{
	if (time <= 0.0f) {
		return false;
	}
	interp->mValue    = from;
	interp->mTarget   = to;
	interp->mSpeed    = 0.0f;
	interp->mAccel    = (to - from) / (time * time * 0.25f);
	interp->mDuration = time;
	interp->mTime     = time;
	return true;
}
