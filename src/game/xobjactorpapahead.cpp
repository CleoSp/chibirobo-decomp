#include "game/xobjactor.h"

/**
 * @note Address: 0x80129410
 * @note Size: 0xA4
 */
extern "C" HSD_JObj* fn_80129410(XObjActorPapa* object)
{
	s32 path[3] = { 0, 2, 0 };
	return GetJointByPath(object->mJObj, path, 3);
}

/**
 * @note Address: 0x801294B4
 * @note Size: 0x54
 */
extern "C" CVec fn_801294B4(XObjActorPapa* object)
{
	s32 path[3] = { 0, 2, 0 };
	return fn_8007E7C8(object, 3, path);
}

/**
 * @note Address: 0x80129508
 * @note Size: 0x44
 */
extern "C" bool fn_80129508(XObjActor* object)
{
	if (!(object->mStatus.mFlags & 1))
		return true;
	return !(object->_998.mFlags & 0x2000);
}

/**
 * @note Address: 0x8012954C
 * @note Size: 0x8
 */
extern "C" f32 fn_8012954C()
{
	return -0.17453292f;
}

/**
 * @note Address: 0x80129554
 * @note Size: 0x8
 */
extern "C" f32 fn_80129554()
{
	return 0.17453292f;
}
