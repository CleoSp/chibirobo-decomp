#include "game/ccameraaction.h"

extern "C" {
extern void* __vt__23CInterfaceInterpolation[];
extern void* __vt__18CInterpolationNone[];
}

// todo: weak copy
/**
 * @note Address: 0x8001A3A8
 * @note Size: 0x1C
 */
extern "C" CInterpolationNone* fn_8001A3A8(CInterpolationNone* interp)
{
	*(void***)interp = __vt__23CInterfaceInterpolation;
	*(void***)interp = __vt__18CInterpolationNone;
	return interp;
}
