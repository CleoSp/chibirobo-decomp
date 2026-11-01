#include "game/ccameraaction.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);
extern "C" void* fn_8000996C(void* interp, s32 flags);
extern "C" void* __vt__33CVectorEaseOutDivideInterpolation[];

// todo: weak copy
/**
 * @note Address: 0x80016E1C
 * @note Size: 0x84
 */
extern "C" CVectorEaseOutDivideInterpolation* fn_80016E1C(CVectorEaseOutDivideInterpolation* interp, s32 flags)
{
	if (interp != NULL) {
		*(void***)interp = __vt__33CVectorEaseOutDivideInterpolation;
		__destroy_arr(interp->mAxes, (ConstructorDestructor*)fn_8000996C, sizeof(CInterpolationEaseOutDivide), 3);
		interp->CInterfaceVectorInterpolation::~CInterfaceVectorInterpolation();
		if ((s16)flags > 0) {
			::operator delete(interp);
		}
	}
	return interp;
}
