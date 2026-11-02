#include "game/xobjactor.h"

extern "C" {
void fn_801095E8(XObjActor3* actor);
void fn_8010904C(XObjActor3* actor);
}

/**
 * @note Address: 0x80109DD8
 * @note Size: 0x6C
 */
extern "C" void vfC8__10XObjActor3Fv(XObjActor3* actor)
{
	UnkObjData* data = actor->mAttr._00;
	s16 type         = data != NULL ? (data != NULL ? data->_16 : 0) : -1; // todo: cleanup, probably an inline
	switch (type) {
	case 14:
		fn_801095E8(actor);
		fn_8010904C(actor);
		break;
	}
}

/**
 * @note Address: 0x80109E44
 * @note Size: 0x3C
 */
extern "C" void vfAC__10XObjActor3Fv(XObjActor3* actor)
{
	if (!(actor->_B8C.mFlags & 0x10)) {
		actor->vfB0(NULL);
	}
}
