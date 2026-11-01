#include "game/ccameraaction.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);
extern "C" void* __dt__20CInterpolationLinearFv(void* interp, s32 flags);
extern "C" void* __vt__26CVectorLinearInterpolation[];

// todo: weak copy
/**
 * @note Address: 0x800161E0
 * @note Size: 0x84
 */
extern "C" CVectorLinearInterpolation* __dt__26CVectorLinearInterpolationFv(CVectorLinearInterpolation* interp, s32 flags)
{
	if (interp != NULL) {
		*(void***)interp = __vt__26CVectorLinearInterpolation;
		__destroy_arr(interp->mAxes, (ConstructorDestructor*)__dt__20CInterpolationLinearFv, sizeof(CInterpolationLinear), 3);
		interp->CInterfaceVectorInterpolation::~CInterfaceVectorInterpolation();
		if ((s16)flags > 0) {
			::operator delete(interp);
		}
	}
	return interp;
}
