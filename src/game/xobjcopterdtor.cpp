#include "game/xobjattachment.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__10XObjCopter[];
void* fn_800BCE44(void* object, s32 flags);
}

/**
 * @note Address: 0x800BCDE4
 * @note Size: 0x60
 */
// todo: weak copy
extern "C" XObjCopter* fn_800BCDE4(XObjCopter* object, s32 flags)
{
	if (object != NULL) {
		*(void***)((u8*)object + 0x34) = __vt__10XObjCopter;
		fn_800BCE44(object, 0);
		if ((s16)flags > 0)
			::operator delete(object);
	}
	return object;
}
