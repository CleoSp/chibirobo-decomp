#include "game/coordinfo.h"

void operator delete(void* object);

extern "C" void* __vt__9CoordInfo[];

/**
 * @note Address: 0x80012A34
 * @note Size: 0x48
 */
// todo: weak copy
extern "C" CoordInfo* __dt__9CoordInfoFv(CoordInfo* info, s32 flags)
{
	if (info != NULL) {
		*(void***)((u8*)info + 0x4) = __vt__9CoordInfo;
		if ((s16)flags > 0)
			::operator delete(info);
	}
	return info;
}
