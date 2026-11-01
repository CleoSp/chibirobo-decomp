#include "game/xobjactor.h"

/**
 * @note Address: 0x80115538
 * @note Size: 0x24
 */
extern "C" void vf150__12XObjActorToyFv(XObjActorToy* object)
{
	object->mState = NULL;
}

/**
 * @note Address: 0x8011555C
 * @note Size: 0x58
 */
extern "C" void vf14C__12XObjActorToyFUl(XObjActorToy* object, u32 frames)
{
	if (object->mState != NULL)
		(object->*object->mState)(frames);
}
