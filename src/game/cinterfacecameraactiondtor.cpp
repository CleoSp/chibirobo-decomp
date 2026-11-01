#include "game/hsd_jobj.h"
#include "game/ccameraaction.h"

void operator delete(void* object);
extern "C" void* __vt__22CInterfaceCameraAction[];

// todo: weak copy
/**
 * @note Address: 0x80016ECC
 * @note Size: 0x88
 */
extern "C" CInterfaceCameraAction* __dt__22CInterfaceCameraActionFv(CInterfaceCameraAction* action, s32 flags)
{
	if (action != NULL) {
		*(void***)((u8*)action + 0x3C) = __vt__22CInterfaceCameraAction; // _3C vtable
		action->mAngle.CVec::~CVec();
		action->mInterest.CVec::~CVec();
		action->mEye.CVec::~CVec();
		if ((s16)flags > 0) {
			::operator delete(action);
		}
	}
	return action;
}
