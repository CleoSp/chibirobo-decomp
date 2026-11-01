#include "game/hsd_jobj.h"
#include "game/menu.h"

extern "C" {
extern void* __vt__6MenuEx[];
MenuCursor* fn_8001B3CC(MenuCursor* cursor);
}

// todo: weak copy
/**
 * @note Address: 0x8001B38C
 * @note Size: 0x40
 */
extern "C" MenuEx* fn_8001B38C(MenuEx* menu)
{
	*(void***)((u8*)menu + 0x9B0) = __vt__6MenuEx; // _9B0 vtable
	fn_8001B3CC(&menu->mCursor);
	return menu;
}
