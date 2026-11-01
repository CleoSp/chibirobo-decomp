#include "game/xobjleticker.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__12XObjLeticker[];
}

/**
 * @note Address: 0x800E25AC
 * @note Size: 0x60
 */
// todo: weak copy
extern "C" XObjLeticker* fn_800E25AC(XObjLeticker* leticker, s32 del)
{
	if (leticker != NULL) {
		*(void***)((u8*)leticker + 0x34) = __vt__12XObjLeticker;
		fn_8000877C((XObj*)leticker, 0);
		if ((s16)del > 0)
			::operator delete(leticker);
	}
	return leticker;
}
