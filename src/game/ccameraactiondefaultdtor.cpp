#include "game/hsd_jobj.h"
#include "game/ccameraaction.h"

void operator delete(void* object);
extern "C" void* __vt__20CCameraActionDefault[];

// todo: weak copy
/**
 * @note Address: 0x8001ACD4
 * @note Size: 0x98
 */
extern "C" CCameraActionDefault* __dt__20CCameraActionDefaultFv(CCameraActionDefault* action, s32 flags)
{
	if (action != NULL) {
		*(void***)((u8*)action + 0x3C) = __vt__20CCameraActionDefault; // _3C vtable
		action->CInterfaceCameraAction::~CInterfaceCameraAction();
		if ((s16)flags > 0) {
			::operator delete(action);
		}
	}
	return action;
}
