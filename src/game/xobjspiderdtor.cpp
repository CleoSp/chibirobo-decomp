#include "game/xobjspider.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__10XObjSpider[];
}

/**
 * @note Address: 0x800EA30C
 * @note Size: 0x100
 */
// todo: weak copy
extern "C" XObjSpider* __dt__10XObjSpiderFv(XObjSpider* spider, s32 del)
{
	if (spider != NULL) {
		*(void***)((u8*)spider + 0x34) = __vt__10XObjSpider;
		spider->_AC8.~CarryInfo();
		spider->mCarry.~CarryInfo();
		spider->mFlags.CStatus::~CStatus();
		__dt__4XObjFv((XObj*)spider, 0);
		if ((s16)del > 0)
			::operator delete(spider);
	}
	return spider;
}
