#include "game/ccameraaction.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);
extern "C" void* __dt__18CInterpolationNoneFv(void* interp, s32 flags);
extern "C" {
extern void* __vt__24CVectorNoneInterpolation[];
extern void* __vt__20CCameraActionAreaExM[];
}

// todo: weak copy
/**
 * @note Address: 0x80015B38
 * @note Size: 0x84
 */
extern "C" CVectorNoneInterpolation* __dt__24CVectorNoneInterpolationFv(CVectorNoneInterpolation* interp, s32 flags)
{
	if (interp != NULL) {
		*(void***)interp = __vt__24CVectorNoneInterpolation;
		__destroy_arr(interp->mAxes, (ConstructorDestructor*)__dt__18CInterpolationNoneFv, sizeof(CInterpolationNone), 3);
		interp->CInterfaceVectorInterpolation::~CInterfaceVectorInterpolation();
		if ((s16)flags > 0) {
			::operator delete(interp);
		}
	}
	return interp;
}

// todo: weak copy
/**
 * @note Address: 0x80015BBC
 * @note Size: 0x98
 */
extern "C" CCameraActionAreaExM* __dt__20CCameraActionAreaExMFv(CCameraActionAreaExM* action, s32 flags)
{
	if (action != NULL) {
		*(void***)((u8*)action + 0x3C) = __vt__20CCameraActionAreaExM; // _3C vtable
		action->CInterfaceCameraAction::~CInterfaceCameraAction();
		if ((s16)flags > 0) {
			::operator delete(action);
		}
	}
	return action;
}
