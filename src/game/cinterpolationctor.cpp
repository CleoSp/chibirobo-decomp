#include "game/ccameraaction.h"

extern "C" {
extern void* __vt__23CInterfaceInterpolation[];
extern void* __vt__27CInterpolationEaseOutDivide[];
extern void* __vt__21CInterpolationEaseOut[];
extern void* __vt__20CInterpolationLinear[];
extern void* __vt__23CInterpolationEaseInOut[];
}

// todo: weak copy
/**
 * @note Address: 0x8001AC64
 * @note Size: 0x1C
 */
extern "C" CInterpolationEaseOutDivide* fn_8001AC64(CInterpolationEaseOutDivide* interp)
{
	*(void***)interp = __vt__23CInterfaceInterpolation;
	*(void***)interp = __vt__27CInterpolationEaseOutDivide;
	return interp;
}

// todo: weak copy
/**
 * @note Address: 0x8001AC80
 * @note Size: 0x1C
 */
extern "C" CInterpolationEaseOut* fn_8001AC80(CInterpolationEaseOut* interp)
{
	*(void***)interp = __vt__23CInterfaceInterpolation;
	*(void***)interp = __vt__21CInterpolationEaseOut;
	return interp;
}

// todo: weak copy
/**
 * @note Address: 0x8001AC9C
 * @note Size: 0x1C
 */
extern "C" CInterpolationLinear* fn_8001AC9C(CInterpolationLinear* interp)
{
	*(void***)interp = __vt__23CInterfaceInterpolation;
	*(void***)interp = __vt__20CInterpolationLinear;
	return interp;
}

// todo: weak copy
/**
 * @note Address: 0x8001ACB8
 * @note Size: 0x1C
 */
extern "C" CInterpolationEaseInOut* fn_8001ACB8(CInterpolationEaseInOut* interp)
{
	*(void***)interp = __vt__23CInterfaceInterpolation;
	*(void***)interp = __vt__23CInterpolationEaseInOut;
	return interp;
}
