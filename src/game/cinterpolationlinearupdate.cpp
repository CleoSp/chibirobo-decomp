#include "game/ccameraaction.h"

/**
 * @note Address: 0x80016EA0
 * @note Size: 0x2C
 */
extern "C" bool fn_80016EA0(CInterpolationLinear* interp, u32 step, f32 dt)
{
	return interp->Step(dt);
}
