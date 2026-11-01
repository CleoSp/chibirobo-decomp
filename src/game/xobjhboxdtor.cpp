#include "game/xobjhbox.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__8XObjHBox[];
XObjHBoxPart* fn_800E240C(XObjHBoxPart* part, s32 del);
}

/**
 * @note Address: 0x800E239C
 * @note Size: 0x70
 */
// todo: weak copy
extern "C" XObjHBox* fn_800E239C(XObjHBox* box, s32 del)
{
	if (box != NULL) {
		*(void***)((u8*)box + 0x34) = __vt__8XObjHBox;
		fn_800E240C(&box->mPart, -1);
		fn_8000877C((XObj*)box, 0);
		if ((s16)del > 0)
			::operator delete(box);
	}
	return box;
}
