#include "game/hsd_jobj.h"
#include "game/ccameraaction.h"

void operator delete(void* object);
extern "C" void* __vt__18CCameraActionChase[];

// todo: weak copy
/**
 * @note Address: 0x8001ABA0
 * @note Size: 0xC4
 */
extern "C" CCameraActionChase* __dt__18CCameraActionChaseFv(CCameraActionChase* action, s32 flags)
{
	if (action != NULL) {
		*(void***)((u8*)action + 0x3C) = __vt__18CCameraActionChase; // _3C vtable
		action->mPos.CVec::~CVec();
		action->mLastPos.CVec::~CVec();
		action->CInterfaceCameraAction::~CInterfaceCameraAction();
		if ((s16)flags > 0) {
			::operator delete(action);
		}
	}
	return action;
}
