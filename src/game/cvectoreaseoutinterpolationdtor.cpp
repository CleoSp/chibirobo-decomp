#include "game/ccameraaction.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);
extern "C" void* __dt__21CInterpolationEaseOutFv(void* interp, s32 flags);
extern "C" void* __vt__27CVectorEaseOutInterpolation[];

// todo: weak copy
/**
 * @note Address: 0x80016918
 * @note Size: 0x84
 */
extern "C" CVectorEaseOutInterpolation* __dt__27CVectorEaseOutInterpolationFv(CVectorEaseOutInterpolation* interp, s32 flags)
{
	if (interp != NULL) {
		*(void***)interp = __vt__27CVectorEaseOutInterpolation;
		__destroy_arr(interp->mAxes, (ConstructorDestructor*)__dt__21CInterpolationEaseOutFv, sizeof(CInterpolationEaseOut), 3);
		interp->CInterfaceVectorInterpolation::~CInterfaceVectorInterpolation();
		if ((s16)flags > 0) {
			::operator delete(interp);
		}
	}
	return interp;
}
