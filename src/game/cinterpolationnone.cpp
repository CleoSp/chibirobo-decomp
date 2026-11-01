#include "game/hsd_jobj.h"
#include "game/ut.h"
#include "game/ccameraaction.h"

/**
 * @note Address: 0x8001585C
 * @note Size: 0x1C
 */
// CInterpolationNone's Start
extern "C" bool Start__18CInterpolationNoneFfff(CInterpolationNone* interp, f32 from, f32 to, f32 time)
{
	interp->mTime  = 1.0f;
	interp->mValue = to;
	interp->mSpeed = 0.0f;
	return true;
}

/**
 * @note Address: 0x80015878
 * @note Size: 0x24
 */
// CInterpolationNone's Step, retail sets the time again here
extern "C" bool Step__18CInterpolationNoneFf(CInterpolationNone* interp, f32 dt)
{
	interp->mTime = 1.0f;
	return interp->mTime <= 0.0f;
}

/**
 * @note Address: 0x8001589C
 * @note Size: 0x2C
 */
// CInterpolationNone's Update
extern "C" bool Update__18CInterpolationNoneFUlf(CInterpolationNone* interp, u32 step, f32 dt)
{
	return interp->Step(dt);
}

/**
 * @note Address: 0x800158C8
 * @note Size: 0x8
 */
// CInterpolationNone's Get
extern "C" f32 Get__18CInterpolationNoneFv(CInterpolationNone* interp)
{
	return interp->mValue;
}

/**
 * @note Address: 0x800158D0
 * @note Size: 0xCC
 */
// CVectorNoneInterpolation's Start
extern "C" bool Start__24CVectorNoneInterpolationF3Vec3Vecf(CVectorNoneInterpolation* interp, const Vec& from, const Vec& to, f32 time)
{
	if (time < 0.0f)
		return false;
	interp->mAxes[0].Start(from.x, to.x, time);
	interp->mAxes[1].Start(from.y, to.y, time);
	interp->mAxes[2].Start(from.z, to.z, time);
	return true;
}

/**
 * @note Address: 0x8001599C
 * @note Size: 0xB4
 */
// CVectorNoneInterpolation's Step
extern "C" bool Step__24CVectorNoneInterpolationFf(CVectorNoneInterpolation* interp, f32 dt)
{
	bool x = interp->mAxes[0].Step(dt);
	bool y = interp->mAxes[1].Step(dt);
	bool z = interp->mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x80015A50
 * @note Size: 0x2C
 */
// CVectorNoneInterpolation's Update
extern "C" bool Update__24CVectorNoneInterpolationFUlf(CVectorNoneInterpolation* interp, u32 step, f32 dt)
{
	return interp->Step(dt);
}

/**
 * @note Address: 0x80015A7C
 * @note Size: 0xBC
 */
// CVectorNoneInterpolation's Get
extern "C" Vec Get__24CVectorNoneInterpolationFv(CVectorNoneInterpolation* interp)
{
	return CVec(interp->mAxes[0].Get(), interp->mAxes[1].Get(), interp->mAxes[2].Get());
}
