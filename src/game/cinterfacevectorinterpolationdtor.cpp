#include "game/ccameraaction.h"

void operator delete(void* object);
extern "C" void* __vt__29CInterfaceVectorInterpolation[];

// todo: weak copy
/**
 * @note Address: 0x8001699C
 * @note Size: 0x48
 */
extern "C" CInterfaceVectorInterpolation* fn_8001699C(CInterfaceVectorInterpolation* interp, s32 flags)
{
	if (interp != NULL) {
		*(void***)interp = __vt__29CInterfaceVectorInterpolation;
		if ((s16)flags > 0) {
			::operator delete(interp);
		}
	}
	return interp;
}
