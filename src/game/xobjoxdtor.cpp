#include "game/xobjattachment.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__6XObjOX[];
void* __dt__14XObjAttachmentFv(void* object, s32 flags);
}

/**
 * @note Address: 0x800BCF64
 * @note Size: 0x60
 */
// todo: weak copy
extern "C" XObjOX* __dt__6XObjOXFv(XObjOX* object, s32 flags)
{
	if (object != NULL) {
		*(void***)((u8*)object + 0x34) = __vt__6XObjOX;
		__dt__14XObjAttachmentFv(object, 0);
		if ((s16)flags > 0)
			::operator delete(object);
	}
	return object;
}
