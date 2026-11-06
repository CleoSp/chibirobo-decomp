#include "game/xobjactor.h"

#pragma section RX ".text.decastatedispatch"

/**
 * @note Address: 0x8012F428
 * @note Size: 0x24
 */
__declspec(section ".text.decastatedispatch") void XObjActorDeca::ClearStateHandler()
{
	mState = NULL;
}

/**
 * @note Address: 0x8012F44C
 * @note Size: 0x58
 */
__declspec(section ".text.decastatedispatch") void XObjActorDeca::RunStateHandler(u32 frames)
{
	if (mState != NULL) {
		(this->*mState)(frames);
	}
}

/**
 * @note Address: 0x8012F4A4
 * @note Size: 0x8C
 */
__declspec(section ".text.decastatedispatch") XObjActorDeca::~XObjActorDeca()
{
}
