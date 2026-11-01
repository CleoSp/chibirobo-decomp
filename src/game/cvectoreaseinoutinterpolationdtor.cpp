#include "game/ccameraaction.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);
extern "C" void* fn_800098B4(void* interp, s32 flags);
extern "C" void* __vt__29CVectorEaseInOutInterpolation[];

// todo: weak copy
/**
 * @note Address: 0x8001658C
 * @note Size: 0x84
 */
extern "C" CVectorEaseInOutInterpolation* fn_8001658C(CVectorEaseInOutInterpolation* interp, s32 flags)
{
	if (interp != NULL) {
		*(void***)interp = __vt__29CVectorEaseInOutInterpolation;
		__destroy_arr(interp->mAxes, (ConstructorDestructor*)fn_800098B4, sizeof(CInterpolationEaseInOut), 3);
		interp->CInterfaceVectorInterpolation::~CInterfaceVectorInterpolation();
		if ((s16)flags > 0) {
			::operator delete(interp);
		}
	}
	return interp;
}
