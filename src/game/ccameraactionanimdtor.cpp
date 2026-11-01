#include "game/hsd_jobj.h"
#include "game/ccameraaction.h"

void operator delete(void* object);
extern "C" void* __vt__17CCameraActionAnim[];

// todo: weak copy
/**
 * @note Address: 0x8001A5EC
 * @note Size: 0x98
 */
extern "C" CCameraActionAnim* __dt__17CCameraActionAnimFv(CCameraActionAnim* action, s32 flags)
{
	if (action != NULL) {
		*(void***)((u8*)action + 0x3C) = __vt__17CCameraActionAnim; // _3C vtable
		action->CInterfaceCameraAction::~CInterfaceCameraAction();
		if ((s16)flags > 0) {
			::operator delete(action);
		}
	}
	return action;
}
