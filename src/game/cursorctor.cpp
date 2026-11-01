#include "game/hsd_jobj.h"
#include "game/title.h"

extern "C" {
extern void* __vt__6Cursor[];
XObj* fn_8001AFAC(XObj* obj);
}

// todo: weak copy
/**
 * @note Address: 0x8001AF70
 * @note Size: 0x3C
 */
extern "C" Cursor* fn_8001AF70(Cursor* cursor)
{
	fn_8001AFAC(cursor);
	*(void***)((u8*)cursor + 0x34) = __vt__6Cursor; // _34 vtable
	return cursor;
}
