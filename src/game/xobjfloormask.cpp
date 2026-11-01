#include "game/xobjs.h"

/**
 * @note Address: 0x8008DEC0
 * @note Size: 0x74
 */
extern "C" void fn_8008DEC0(XObj* object)
{
	if (object->mStatus.mFlags & 0x0200000000000000ULL) {
		object->_250[1] = object->_250[0];
		for (s32 i = 0; object->mFloors[i].mId != -1; ++i) {
			u32 mask = 1U << i;
			if (object->_250[0] & mask)
				object->_250[0] &= ~mask;
		}
	}
}
