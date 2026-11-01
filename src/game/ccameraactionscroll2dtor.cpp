#include "game/hsd_jobj.h"
#include "game/ccameraaction.h"

void operator delete(void* object);
extern "C" void* __vt__20CCameraActionScroll2[];

// todo: weak copy
/**
 * @note Address: 0x8001A784
 * @note Size: 0x98
 */
extern "C" CCameraActionScroll2* __dt__20CCameraActionScroll2Fv(CCameraActionScroll2* action, s32 flags)
{
	if (action != NULL) {
		*(void***)((u8*)action + 0x3C) = __vt__20CCameraActionScroll2; // _3C vtable
		action->CInterfaceCameraAction::~CInterfaceCameraAction();
		if ((s16)flags > 0) {
			::operator delete(action);
		}
	}
	return action;
}
