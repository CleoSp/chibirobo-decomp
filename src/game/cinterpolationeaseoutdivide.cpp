#include "game/hsd_jobj.h"
#include "game/ccameraaction.h"

/**
 * @note Address: 0x800169E4
 * @note Size: 0xD8
 */
extern "C" bool fn_800169E4(CVectorEaseOutDivideInterpolation* interp, Vec from, Vec to, f32 divisor)
{
	if (divisor <= 0.0f) {
		return false;
	}
	interp->mAxes[0].Start(from.x, to.x, divisor);
	interp->mAxes[1].Start(from.y, to.y, divisor);
	interp->mAxes[2].Start(from.z, to.z, divisor);
	interp->mXZDone = FALSE;
	return true;
}

/**
 * @note Address: 0x80016ABC
 * @note Size: 0x2C
 */
extern "C" bool fn_80016ABC(CInterpolationEaseOutDivide* interp, f32 from, f32 to, f32 divisor)
{
	if (divisor <= 0.0f) {
		return false;
	}
	interp->mValue   = from;
	interp->mTarget  = to;
	interp->mDivisor = divisor;
	return true;
}

/**
 * @note Address: 0x80016AE8
 * @note Size: 0xE0
 */
extern "C" bool fn_80016AE8(CVectorEaseOutDivideInterpolation* interp, u32 step, f32 dt)
{
	bool x = interp->mAxes[0].Update(step, dt);
	bool y = interp->mAxes[1].Update(step, dt);
	bool z = interp->mAxes[2].Update(step, dt);
	if (x && z) {
		interp->mXZDone = TRUE;
	}
	return x && y && z;
}

/**
 * @note Address: 0x80016BC8
 * @note Size: 0x84
 */
extern "C" bool fn_80016BC8(CInterpolationEaseOutDivide* interp, u32 step, f32 dt)
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
 * @note Address: 0x80016C4C
 * @note Size: 0x58
 */
extern "C" bool fn_80016C4C(CInterpolationEaseOutDivide* interp, f32 dt)
{
	f32 prev = interp->mValue;
	if (interp->mDivisor > 0.0f) {
		interp->mValue = (prev * (interp->mDivisor - 1.0f) + interp->mTarget) / interp->mDivisor;
	}
	return (f32)fabs(interp->mValue - prev) < 0.001f;
}

/**
 * @note Address: 0x80016CA4
 * @note Size: 0xB4
 */
extern "C" bool fn_80016CA4(CVectorEaseOutDivideInterpolation* interp, f32 dt)
{
	bool x = interp->mAxes[0].Step(dt);
	bool y = interp->mAxes[1].Step(dt);
	bool z = interp->mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x80016D58
 * @note Size: 0xBC
 */
extern "C" Vec fn_80016D58(CVectorEaseOutDivideInterpolation* interp)
{
	return CVec(interp->mAxes[0].Get(), interp->mAxes[1].Get(), interp->mAxes[2].Get());
}

/**
 * @note Address: 0x80016E14
 * @note Size: 0x8
 */
extern "C" f32 fn_80016E14(CInterpolationEaseOutDivide* interp)
{
	return interp->mValue;
}
