#include "game/hsd_jobj.h"
#include "game/ccameraactioneye.h"

void operator delete(void* object);
extern "C" void* __vt__16CCameraActionEye[];

// todo: weak copy
/**
 * @note Address: 0x8001A684
 * @note Size: 0x100
 */
extern "C" CCameraActionEye* __dt__16CCameraActionEyeFv(CCameraActionEye* action, s32 flags)
{
	if (action != NULL) {
		*(void***)((u8*)action + 0x3C) = __vt__16CCameraActionEye; // _3C vtable
		action->mOverride.CVec::~CVec();
		action->mLook.CObjVec::~CObjVec();
		action->CInterfaceCameraAction::~CInterfaceCameraAction();
		if ((s16)flags > 0) {
			::operator delete(action);
		}
	}
	return action;
}
