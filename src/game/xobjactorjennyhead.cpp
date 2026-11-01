#include "game/xobjactor.h"

/**
 * @note Address: 0x80122F14
 * @note Size: 0xA4
 */
extern "C" HSD_JObj* fn_80122F14(XObjActorJenny* object)
{
	s32 path[3] = { 0, 3, 0 };
	return GetJointByPath(object->mJObj, path, 3);
}

/**
 * @note Address: 0x80122FB8
 * @note Size: 0x54
 */
extern "C" CVec fn_80122FB8(XObjActorJenny* object)
{
	s32 path[3] = { 0, 3, 0 };
	return fn_8007E7C8(object, 3, path);
}

/**
 * @note Address: 0x8012300C
 * @note Size: 0x28
 */
extern "C" f32 fn_8012300C(XObjActorJenny* object)
{
	switch (object->mAnimId) {
	case 0x412:
	case 0x413:
	case 0x414:
		return 0.34906584f;
	default:
		return -0.34906584f;
	}
}

/**
 * @note Address: 0x80123034
 * @note Size: 0x28
 */
extern "C" f32 fn_80123034(XObjActorJenny* object)
{
	switch (object->mAnimId) {
	case 0x412:
	case 0x413:
	case 0x414:
		return 1.0471976f;
	default:
		return 0.17453292f;
	}
}
