#include "game/ccameraaction.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);
extern "C" void* fn_80009910(void* interp, s32 flags);
extern "C" void* __vt__27CVectorEaseOutInterpolation[];

// todo: weak copy
/**
 * @note Address: 0x80016918
 * @note Size: 0x84
 */
extern "C" CVectorEaseOutInterpolation* fn_80016918(CVectorEaseOutInterpolation* interp, s32 flags)
{
	if (interp != NULL) {
		*(void***)interp = __vt__27CVectorEaseOutInterpolation;
		__destroy_arr(interp->mAxes, (ConstructorDestructor*)fn_80009910, sizeof(CInterpolationEaseOut), 3);
		interp->CInterfaceVectorInterpolation::~CInterfaceVectorInterpolation();
		if ((s16)flags > 0) {
			::operator delete(interp);
		}
	}
	return interp;
}
