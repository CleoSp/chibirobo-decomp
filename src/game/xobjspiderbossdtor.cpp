#include "game/xobjspiderboss.h"

extern "C" void* __vt__14XObjSpiderBoss[];

/**
 * @note Address: 0x80100264
 * @note Size: 0x78
 */
// todo: weak copy
extern "C" XObjSpiderBoss* fn_80100264(XObjSpiderBoss* spider, s32 flags)
{
	if (spider != NULL) {
		*(void***)((u8*)spider + 0x34) = __vt__14XObjSpiderBoss;
		spider->mSpiderStatus.CStatus::~CStatus();
		__dt__4XObjFv(spider, 0);
		if ((s16)flags > 0) {
			::operator delete(spider);
		}
	}
	return spider;
}
