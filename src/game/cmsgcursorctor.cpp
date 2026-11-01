#include "game/hsd_jobj.h"
#include "game/msg.h"

extern "C" Cursor* fn_8001AF70(Cursor* cursor);

// todo: weak copy
/**
 * @note Address: 0x8001AF40
 * @note Size: 0x30
 */
extern "C" MsgCursor* __ct__9MsgCursorFv(MsgCursor* cursor)
{
	fn_8001AF70(&cursor->mCursor);
	return cursor;
}
