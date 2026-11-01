#include "game/xobjspidersmall.h"

extern "C" void* __vt__15XObjSpiderSmall[];

/**
 * @note Address: 0x800F49F0
 * @note Size: 0xBC
 */
// todo: weak copy
extern "C" XObjSpiderSmall* __dt__15XObjSpiderSmallFv(XObjSpiderSmall* spider, s32 flags)
{
	if (spider != NULL) {
		*(void***)((u8*)spider + 0x34) = __vt__15XObjSpiderSmall;
		spider->mFx.~XObjSpiderSmallFx();
		spider->mSpiderStatus.CStatus::~CStatus();
		__dt__4XObjFv(spider, 0);
		if ((s16)flags > 0) {
			::operator delete(spider);
		}
	}
	return spider;
}
