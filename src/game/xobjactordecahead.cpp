#include "game/xobjactor.h"

struct JointChainPath {
	s32 steps[3];
};

extern "C" {
extern JointChainPath lbl_8022DC88;
extern JointChainPath lbl_8022DC94;
}

/**
 * @note Address: 0x8012F530
 * @note Size: 0xA4
 */
extern "C" HSD_JObj* fn_8012F530(XObjActorDeca* object)
{
	JointChainPath path = lbl_8022DC88;
	return GetJointByPath(object->mJObj, path.steps, 3);
}

/**
 * @note Address: 0x8012F5D4
 * @note Size: 0x54
 */
extern "C" CVec fn_8012F5D4(XObjActorDeca* object)
{
	JointChainPath path = lbl_8022DC94;
	return fn_8007E7C8(object, 3, path.steps);
}
