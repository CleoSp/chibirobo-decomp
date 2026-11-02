#include "game/hsd_jobj.h"
#include "game/msg.h"

extern "C" Cursor* __ct__6CursorFv(Cursor* cursor);

// todo: weak copy
/**
 * @note Address: 0x8001AF40
 * @note Size: 0x30
 */
extern "C" MsgCursor* __ct__9MsgCursorFv(MsgCursor* cursor)
{
	__ct__6CursorFv(&cursor->mCursor);
	return cursor;
}
