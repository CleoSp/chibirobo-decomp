#include "types.h"

// the object pools and table, CGame has its members spelled out from _9694 on
class XOBJS;

void operator delete(void* object);
extern "C" void* __vt__5XOBJS[];

// todo: weak copy
/**
 * @note Address: 0x80018FE8
 * @note Size: 0x48
 */
extern "C" XOBJS* fn_80018FE8(XOBJS* objs, s32 flags)
{
	if (objs != NULL) {
		*(void***)((u8*)objs + 0x11D8) = __vt__5XOBJS; // _11D8 vtable
		if ((s16)flags > 0) {
			::operator delete(objs);
		}
	}
	return objs;
}
