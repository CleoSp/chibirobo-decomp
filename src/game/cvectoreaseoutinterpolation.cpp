#include "game/hsd_jobj.h"
#include "game/ccameraaction.h"

/**
 * @note Address: 0x80016610
 * @note Size: 0xD0
 */
extern "C" bool fn_80016610(CVectorEaseOutInterpolation* interp, Vec from, Vec to, f32 time)
{
	if (time <= 0.0f) {
		return false;
	}
	interp->mAxes[0].Start(from.x, to.x, time);
	interp->mAxes[1].Start(from.y, to.y, time);
	interp->mAxes[2].Start(from.z, to.z, time);
	return true;
}

/**
 * @note Address: 0x800166E0
 * @note Size: 0xC8
 */
extern "C" bool fn_800166E0(CVectorEaseOutInterpolation* interp, u32 step, f32 dt)
{
	bool x = interp->mAxes[0].Update(step, dt);
	bool y = interp->mAxes[1].Update(step, dt);
	bool z = interp->mAxes[2].Update(step, dt);
	return x && y && z;
}

/**
 * @note Address: 0x800167A8
 * @note Size: 0xB4
 */
extern "C" bool fn_800167A8(CVectorEaseOutInterpolation* interp, f32 dt)
{
	bool x = interp->mAxes[0].Step(dt);
	bool y = interp->mAxes[1].Step(dt);
	bool z = interp->mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x8001685C
 * @note Size: 0xBC
 */
extern "C" Vec fn_8001685C(CVectorEaseOutInterpolation* interp)
{
	return CVec(interp->mAxes[0].Get(), interp->mAxes[1].Get(), interp->mAxes[2].Get());
}
