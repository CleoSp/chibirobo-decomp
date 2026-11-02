#include "game/xobjattachment.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__10XObjPopper[];
void* __dt__14XObjAttachmentFv(void* object, s32 flags);
}

/**
 * @note Address: 0x800BCEA4
 * @note Size: 0x60
 */
// todo: weak copy
extern "C" XObjPopper* __dt__10XObjPopperFv(XObjPopper* object, s32 flags)
{
	if (object != NULL) {
		*(void***)((u8*)object + 0x34) = __vt__10XObjPopper;
		__dt__14XObjAttachmentFv(object, 0);
		if ((s16)flags > 0)
			::operator delete(object);
	}
	return object;
}
