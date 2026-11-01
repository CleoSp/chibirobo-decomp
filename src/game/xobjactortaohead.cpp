#include "game/xobjactor.h"

struct JointPathData {
	s32 steps[4];
};

extern "C" {
extern JointPathData lbl_8037FE98;
extern JointPathData lbl_8037FEA8;
}

/**
 * @note Address: 0x8012C94C
 * @note Size: 0xAC
 */
extern "C" HSD_JObj* fn_8012C94C(XObjActorTao* object)
{
	JointPathData path = lbl_8037FE98;
	return GetJointByPath(object->mJObj, path.steps, 4);
}

/**
 * @note Address: 0x8012C9F8
 * @note Size: 0x5C
 */
extern "C" CVec fn_8012C9F8(XObjActorTao* object)
{
	JointPathData path = lbl_8037FEA8;
	return fn_8007E7C8(object, 4, path.steps);
}

/**
 * @note Address: 0x8012CA54
 * @note Size: 0x44
 */
extern "C" bool fn_8012CA54(XObjActor* object)
{
	if (!(object->mStatus.mFlags & 1ULL))
		return true;
	return !(object->_998.mFlags & 0x2000);
}
