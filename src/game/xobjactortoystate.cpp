#include "game/xobjactor.h"

/**
 * @note Address: 0x80115538
 * @note Size: 0x24
 */
void XObjActorToy::vf150()
{
	mState = NULL;
}

/**
 * @note Address: 0x8011555C
 * @note Size: 0x58
 */
void XObjActorToy::vf14C(u32 frames)
{
	if (mState != NULL)
		(this->*mState)(frames);
}
