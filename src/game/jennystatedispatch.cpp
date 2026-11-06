#include "game/xobjactor.h"

#pragma section RX ".text.jennystatedispatch"

__declspec(section ".text.jennystatedispatch") void XObjActorJenny::ClearStateHandler()
{
	mState = NULL;
}

/**
 * @note Address: 0x80122E30
 * @note Size: 0x58
 */
__declspec(section ".text.jennystatedispatch") void XObjActorJenny::RunStateHandler(u32 frames)
{
	if (mState != NULL) {
		(this->*mState)(frames);
	}
}

/**
 * @note Address: 0x80122E88
 * @note Size: 0x8C
 */
__declspec(section ".text.jennystatedispatch") XObjActorJenny::~XObjActorJenny()
{
}
