#include "game/xobjactor.h"

#pragma section RX ".text.taostatedispatch"

/**
 * @note Address: 0x8012C858
 * @note Size: 0x24
 */
__declspec(section ".text.taostatedispatch") void XObjActorTao::ClearStateHandler()
{
	mState = NULL;
}

/**
 * @note Address: 0x8012C87C
 * @note Size: 0x58
 */
__declspec(section ".text.taostatedispatch") void XObjActorTao::RunStateHandler(u32 frames)
{
	if (mState != NULL) {
		(this->*mState)(frames);
	}
}

/**
 * @note Address: 0x8012C8D4
 * @note Size: 0x78
 */
__declspec(section ".text.taostatedispatch") XObjActorTao::~XObjActorTao()
{
}
