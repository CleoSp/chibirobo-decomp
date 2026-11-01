#include "game/hsd_jobj.h"
#include "game/ut.h"
#include "game/ccameraaction.h"

/**
 * @note Address: 0x8001585C
 * @note Size: 0x1C
 */
// CInterpolationNone's Start
extern "C" bool fn_8001585C(CInterpolationNone* interp, f32 from, f32 to, f32 time)
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
extern "C" bool fn_80015878(CInterpolationNone* interp, f32 dt)
{
	interp->mTime = 1.0f;
	return interp->mTime <= 0.0f;
}

/**
 * @note Address: 0x8001589C
 * @note Size: 0x2C
 */
// CInterpolationNone's Update
extern "C" bool fn_8001589C(CInterpolationNone* interp, u32 step, f32 dt)
{
	return interp->Step(dt);
}

/**
 * @note Address: 0x800158C8
 * @note Size: 0x8
 */
// CInterpolationNone's Get
extern "C" f32 fn_800158C8(CInterpolationNone* interp)
{
	return interp->mValue;
}

/**
 * @note Address: 0x800158D0
 * @note Size: 0xCC
 */
// CVectorNoneInterpolation's Start
extern "C" bool fn_800158D0(CVectorNoneInterpolation* interp, const Vec& from, const Vec& to, f32 time)
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
extern "C" bool fn_8001599C(CVectorNoneInterpolation* interp, f32 dt)
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
extern "C" bool fn_80015A50(CVectorNoneInterpolation* interp, u32 step, f32 dt)
{
	return interp->Step(dt);
}

/**
 * @note Address: 0x80015A7C
 * @note Size: 0xBC
 */
// CVectorNoneInterpolation's Get
extern "C" Vec fn_80015A7C(CVectorNoneInterpolation* interp)
{
	return CVec(interp->mAxes[0].Get(), interp->mAxes[1].Get(), interp->mAxes[2].Get());
}
