#include "game/xobjactor.h"

/**
 * @note Address: 0x801220A8
 * @note Size: 0xA4
 */
extern "C" void fn_801220A8(XObjActorToy* object)
{
	fn_80107600(object);
	if (object->mFirst.mStatus.mFlags & 0x10000ULL)
		object->mFirst.vfC8();
	if (object->mSecond.mStatus.mFlags & 0x10000ULL)
		object->mSecond.vfC8();
}
