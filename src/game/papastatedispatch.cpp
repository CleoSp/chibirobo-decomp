#include "game/xobjactor.h"

#pragma section RX ".text.papastatedispatch"

/**
 * @note Address: 0x8012931C
 * @note Size: 0x24
 */
__declspec(section ".text.papastatedispatch") void XObjActorPapa::ClearStateHandler()
{
	mState = NULL;
}

/**
 * @note Address: 0x80129340
 * @note Size: 0x58
 */
__declspec(section ".text.papastatedispatch") void XObjActorPapa::RunStateHandler(u32 frames)
{
	if (mState != NULL) {
		(this->*mState)(frames);
	}
}

/**
 * @note Address: 0x80129398
 * @note Size: 0x78
 */
__declspec(section ".text.papastatedispatch") XObjActorPapa::~XObjActorPapa()
{
}
