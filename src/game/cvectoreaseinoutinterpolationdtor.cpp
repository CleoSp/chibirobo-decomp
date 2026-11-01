#include "game/ccameraaction.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);
extern "C" void* __dt__23CInterpolationEaseInOutFv(void* interp, s32 flags);
extern "C" void* __vt__29CVectorEaseInOutInterpolation[];

// todo: weak copy
/**
 * @note Address: 0x8001658C
 * @note Size: 0x84
 */
extern "C" CVectorEaseInOutInterpolation* __dt__29CVectorEaseInOutInterpolationFv(CVectorEaseInOutInterpolation* interp, s32 flags)
{
	if (interp != NULL) {
		*(void***)interp = __vt__29CVectorEaseInOutInterpolation;
		__destroy_arr(interp->mAxes, (ConstructorDestructor*)__dt__23CInterpolationEaseInOutFv, sizeof(CInterpolationEaseInOut), 3);
		interp->CInterfaceVectorInterpolation::~CInterfaceVectorInterpolation();
		if ((s16)flags > 0) {
			::operator delete(interp);
		}
	}
	return interp;
}
