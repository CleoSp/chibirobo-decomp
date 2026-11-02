#include "game/arcache.h"

void operator delete(void* object);
extern "C" void* __vt__11ARCacheInfo[];

// todo: weak copy
/**
 * @note Address: 0x8001975C
 * @note Size: 0x48
 */
extern "C" ARCacheInfo* __dt__11ARCacheInfoFv(ARCacheInfo* info, s32 flags)
{
	if (info != NULL) {
		*(void***)((u8*)info + 0x4008) = __vt__11ARCacheInfo; // _4008 vtable
		if ((s16)flags > 0) {
			::operator delete(info);
		}
	}
	return info;
}
