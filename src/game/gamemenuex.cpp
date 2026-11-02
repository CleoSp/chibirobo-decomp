#include "game/menuex.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__6MenuEx[];
MenuCursor* __dt__10MenuCursorFv(MenuCursor* cursor, s32 flags);
}

/**
 * @note Address: 0x80008C74
 * @note Size: 0x64
 */
// todo: weak copy
extern "C" MenuEx* __dt__6MenuExFv(MenuEx* menu, s32 flags)
{
	if (menu != NULL) {
		*(void***)((u8*)menu + 0x9B0) = __vt__6MenuEx;
		__dt__10MenuCursorFv(&menu->mCursor, -1);
		if ((s16)flags > 0)
			::operator delete(menu);
	}
	return menu;
}
