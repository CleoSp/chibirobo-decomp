#include "game/xobjattachment.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__9XObjRadar[];
void* __dt__14XObjAttachmentFv(void* object, s32 flags);
}

/**
 * @note Address: 0x800BCF04
 * @note Size: 0x60
 */
// todo: weak copy
extern "C" XObjRadar* __dt__9XObjRadarFv(XObjRadar* object, s32 flags)
{
	if (object != NULL) {
		*(void***)((u8*)object + 0x34) = __vt__9XObjRadar;
		__dt__14XObjAttachmentFv(object, 0);
		if ((s16)flags > 0)
			::operator delete(object);
	}
	return object;
}
