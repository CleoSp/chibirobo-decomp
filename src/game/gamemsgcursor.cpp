#include "game/hsd_jobj.h"
#include "game/msg.h"

void operator delete(void* object);
extern "C" Cursor* __dt__6CursorFv(Cursor* cursor, s32 flags);

/**
 * @note Address: 0x80008F5C
 * @note Size: 0x54
 */
// todo: weak copy
extern "C" MsgCursor* __dt__9MsgCursorFv(MsgCursor* cursor, s32 flags)
{
	if (cursor != NULL) {
		__dt__6CursorFv(&cursor->mCursor, -1);
		if ((s16)flags > 0)
			::operator delete(cursor);
	}
	return cursor;
}
