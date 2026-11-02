#include "game/hsd_jobj.h"
#include "game/msg.h"

void operator delete(void* object);

extern "C" void* __vt__4CMsg[];

/**
 * @note Address: 0x80008E70
 * @note Size: 0xEC
 */
// todo: weak copy
extern "C" CMsg* __dt__4CMsgFv(CMsg* msg, s32 flags)
{
	if (msg != NULL) {
#if defined(VERSION_JP)
		*(void***)((u8*)msg + 0x43FC8) = __vt__4CMsg; // _43FC8 vtable
#else
		*(void***)((u8*)msg + 0x44618) = __vt__4CMsg; // _44618 vtable
#endif
		msg->mFont.CMsgFont::~CMsgFont();
		msg->mData.CMsgData::~CMsgData();
		if ((s16)flags > 0)
			::operator delete(msg);
	}
	return msg;
}
