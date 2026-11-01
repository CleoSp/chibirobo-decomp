#include "game/hsd_jobj.h"
#include "game/ccameraaction.h"

void operator delete(void* object);
extern "C" void* __vt__19CCameraActionScroll[];

// todo: weak copy
/**
 * @note Address: 0x8001A2E4
 * @note Size: 0xC4
 */
extern "C" CCameraActionScroll* __dt__19CCameraActionScrollFv(CCameraActionScroll* action, s32 flags)
{
	if (action != NULL) {
		*(void***)((u8*)action + 0x3C) = __vt__19CCameraActionScroll; // _3C vtable
		action->mDistanceEase.CInterpolationEaseOutDivide::~CInterpolationEaseOutDivide();
		action->CInterfaceCameraAction::~CInterfaceCameraAction();
		if ((s16)flags > 0) {
			::operator delete(action);
		}
	}
	return action;
}
