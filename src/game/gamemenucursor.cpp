#include "game/menu.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__10MenuCursor[];
Cursor* __dt__6CursorFv(Cursor* cursor, s32 flags);
}

/**
 * @note Address: 0x80008CD8
 * @note Size: 0x60
 */
// todo: weak copy
extern "C" MenuCursor* __dt__10MenuCursorFv(MenuCursor* cursor, s32 flags)
{
	if (cursor != NULL) {
		*(void***)((u8*)cursor + 0x34) = __vt__10MenuCursor;
		__dt__6CursorFv(cursor, 0);
		if ((s16)flags > 0)
			::operator delete(cursor);
	}
	return cursor;
}
