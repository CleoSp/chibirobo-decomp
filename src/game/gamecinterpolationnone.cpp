#include "game/ccameraaction.h"

void operator delete(void* object);

extern "C" void* __vt__18CInterpolationNone[];

/**
 * @note Address: 0x80009EC4
 * @note Size: 0x5C
 */
// todo: weak copy
extern "C" CInterpolationNone* fn_80009EC4(CInterpolationNone* interp, s32 flags)
{
	if (interp != NULL) {
		*(void***)interp = __vt__18CInterpolationNone;
		interp->CInterfaceInterpolation::~CInterfaceInterpolation();
		if ((s16)flags > 0)
			::operator delete(interp);
	}
	return interp;
}
