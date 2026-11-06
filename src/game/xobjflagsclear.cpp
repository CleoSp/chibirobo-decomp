#include "game/xobjflags.h"

/**
 * @note Address: 0x800E2300
 * @note Size: 0x9C
 */
#pragma section RX ".text.clearcollectionflags"
__declspec(section ".text.clearcollectionflags") void XObjFlags::vf94()
{
	u32* flags = mFlags->mFlags;
	for (u32 i = 0; i < ARRAY_SIZE(mFlags->mFlags); ++i) {
		flags[i] = 0;
	}
}
