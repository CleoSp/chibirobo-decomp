#include "game/xobjactor.h"

/**
 * @note Address: 0x80126510
 * @note Size: 0xAC
 */
extern "C" HSD_JObj* fn_80126510(XObjActorMama* object)
{
	s32 path[4] = { 0, 4, 0, 0 };
	return GetJointByPath(object->mJObj, path, 4);
}

/**
 * @note Address: 0x801265BC
 * @note Size: 0x5C
 */
extern "C" CVec fn_801265BC(XObjActorMama* object)
{
	s32 path[4] = { 0, 4, 0, 0 };
	return fn_8007E7C8(object, 4, path);
}

/**
 * @note Address: 0x80126618
 * @note Size: 0x8
 */
extern "C" f32 fn_80126618()
{
	return -0.17453292f;
}

/**
 * @note Address: 0x80126620
 * @note Size: 0x8
 */
extern "C" f32 fn_80126620()
{
	return 0.17453292f;
}

/**
 * @note Address: 0x80126628
 * @note Size: 0x8
 */
extern "C" f32 fn_80126628()
{
	return -0.6981317f;
}

/**
 * @note Address: 0x80126630
 * @note Size: 0x8
 */
extern "C" f32 fn_80126630()
{
	return 0.6981317f;
}
