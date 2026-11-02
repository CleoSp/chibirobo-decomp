#include "game/xobj.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__8AttrInfo[];
extern void* __vt__9CAnmQueue[];
}

// todo: weak copy
/**
 * @note Address: 0x800185B0
 * @note Size: 0x48
 */
extern "C" AttrInfo* __dt__8AttrInfoFv(AttrInfo* info, s32 flags)
{
	if (info != NULL) {
		*(void***)((u8*)info + 0x8) = __vt__8AttrInfo; // _08 vtable
		if ((s16)flags > 0) {
			::operator delete(info);
		}
	}
	return info;
}

// todo: weak copy
/**
 * @note Address: 0x800185F8
 * @note Size: 0x48
 */
extern "C" CAnmQueue* __dt__9CAnmQueueFv(CAnmQueue* queue, s32 flags)
{
	if (queue != NULL) {
		*(void***)((u8*)queue + 0x84) = __vt__9CAnmQueue; // _84 vtable
		if ((s16)flags > 0) {
			::operator delete(queue);
		}
	}
	return queue;
}
