#include "game/xobjactor.h"

/**
 * @note Address: 0x8012F628
 * @note Size: 0x318
 */
extern "C" Vec fn_8012F628(XObjActorDeca* object)
{
	if (object->_C90.mFlags & 4) {
		CJObj joint;
		s32 path[2] = { 0, 3 };
		joint.Attach(FindJointByPath(object->mJObj, path, 2));
		Vec position = GetJointPosition(joint.mJObj);
		Vec result;
		result.x = position.x;
		result.y = position.y;
		result.z = position.z;
		result.y -= 0.5f;
		return result;
	}
	return fn_80107084(object);
}
