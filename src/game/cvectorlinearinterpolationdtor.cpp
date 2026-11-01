#include "game/ccameraaction.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);
extern "C" void* fn_80009858(void* interp, s32 flags);
extern "C" void* __vt__26CVectorLinearInterpolation[];

// todo: weak copy
/**
 * @note Address: 0x800161E0
 * @note Size: 0x84
 */
extern "C" CVectorLinearInterpolation* fn_800161E0(CVectorLinearInterpolation* interp, s32 flags)
{
	if (interp != NULL) {
		*(void***)interp = __vt__26CVectorLinearInterpolation;
		__destroy_arr(interp->mAxes, (ConstructorDestructor*)fn_80009858, sizeof(CInterpolationLinear), 3);
		interp->CInterfaceVectorInterpolation::~CInterfaceVectorInterpolation();
		if ((s16)flags > 0) {
			::operator delete(interp);
		}
	}
	return interp;
}
