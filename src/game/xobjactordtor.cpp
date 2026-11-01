#include "game/xobjactor.h"

extern "C" void* __vt__9XObjActor[];

/**
 * @note Address: 0x80106A14
 * @note Size: 0x78
 */
// todo: weak copy
extern "C" XObjActor* fn_80106A14(XObjActor* actor, s32 flags)
{
	if (actor != NULL) {
		*(void***)((u8*)actor + 0x34) = __vt__9XObjActor;
		actor->_998.CStatus::~CStatus();
		fn_8000877C(actor, 0);
		if ((s16)flags > 0) {
			::operator delete(actor);
		}
	}
	return actor;
}
