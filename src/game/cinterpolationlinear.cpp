#include "game/hsd_jobj.h"
#include "game/ccameraaction.h"

/**
 * @note Address: 0x80015F08
 * @note Size: 0xCC
 */
extern "C" bool Start__26CVectorLinearInterpolationF3Vec3Vecf(CVectorLinearInterpolation* interp, Vec from, Vec to, f32 time)
{
	if (time < 0.0f) {
		return false;
	}
	interp->mAxes[0].Start(from.x, to.x, time);
	interp->mAxes[1].Start(from.y, to.y, time);
	interp->mAxes[2].Start(from.z, to.z, time);
	return true;
}

/**
 * @note Address: 0x80015FD4
 * @note Size: 0x30
 */
extern "C" bool Start__20CInterpolationLinearFfff(CInterpolationLinear* interp, f32 from, f32 to, f32 time)
{
	// a zero time gets through and divides by zero
	if (time < 0.0f) {
		return false;
	}
	interp->mTime  = time;
	interp->mValue = from;
	interp->mSpeed = (to - from) / time;
	return true;
}

/**
 * @note Address: 0x80016004
 * @note Size: 0xB4
 */
extern "C" bool Step__26CVectorLinearInterpolationFf(CVectorLinearInterpolation* interp, f32 dt)
{
	bool x = interp->mAxes[0].Step(dt);
	bool y = interp->mAxes[1].Step(dt);
	bool z = interp->mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x800160B8
 * @note Size: 0x38
 */
extern "C" bool Step__20CInterpolationLinearFf(CInterpolationLinear* interp, f32 dt)
{
	interp->mTime -= dt;
	interp->mValue = interp->mSpeed * dt + interp->mValue;
	return interp->mTime <= 0.0f;
}

/**
 * @note Address: 0x800160F0
 * @note Size: 0x2C
 */
extern "C" bool Update__26CVectorLinearInterpolationFUlf(CVectorLinearInterpolation* interp, u32 step, f32 dt)
{
	return interp->Step(dt);
}

/**
 * @note Address: 0x8001611C
 * @note Size: 0xBC
 */
extern "C" Vec Get__26CVectorLinearInterpolationFv(CVectorLinearInterpolation* interp)
{
	return CVec(interp->mAxes[0].Get(), interp->mAxes[1].Get(), interp->mAxes[2].Get());
}

/**
 * @note Address: 0x800161D8
 * @note Size: 0x8
 */
extern "C" f32 Get__20CInterpolationLinearFv(CInterpolationLinear* interp)
{
	return interp->mValue;
}
