#include "game/xobjattachment.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__11XObjWireEff[];
}

/**
 * @note Address: 0x800BCD84
 * @note Size: 0x60
 */
// todo: weak copy
extern "C" XObjWireEff* fn_800BCD84(XObjWireEff* object, s32 flags)
{
	if (object != NULL) {
		*(void***)((u8*)object + 0x34) = __vt__11XObjWireEff;
		__dt__4XObjFv((XObj*)object, 0);
		if ((s16)flags > 0)
			::operator delete(object);
	}
	return object;
}
