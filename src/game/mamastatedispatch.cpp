#include "game/xobjactor.h"

#pragma section RX ".text.mamastatedispatch"

__declspec(section ".text.mamastatedispatch") void XObjActorMama::ClearStateHandler()
{
	mState = NULL;
}

/**
 * @note Address: 0x80126440
 * @note Size: 0x58
 */
__declspec(section ".text.mamastatedispatch") void XObjActorMama::RunStateHandler(u32 frames)
{
	if (mState != NULL) {
		(this->*mState)(frames);
	}
}

/**
 * @note Address: 0x80126498
 * @note Size: 0x78
 */
__declspec(section ".text.mamastatedispatch") XObjActorMama::~XObjActorMama()
{
}
