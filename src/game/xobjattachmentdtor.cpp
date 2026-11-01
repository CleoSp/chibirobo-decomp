#include "game/xobjattachment.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__14XObjAttachment[];
}

/**
 * @note Address: 0x800BCE44
 * @note Size: 0x60
 */
// todo: weak copy
extern "C" XObjAttachment* fn_800BCE44(XObjAttachment* object, s32 flags)
{
	if (object != NULL) {
		*(void***)((u8*)object + 0x34) = __vt__14XObjAttachment;
		__dt__4XObjFv((XObj*)object, 0);
		if ((s16)flags > 0)
			::operator delete(object);
	}
	return object;
}
