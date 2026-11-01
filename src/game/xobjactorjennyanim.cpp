#include "game/xobjactor.h"

/**
 * @note Address: 0x80124980
 * @note Size: 0x430
 */
extern "C" void fn_80124980(XObjActorJenny* object, u32 frames)
{
	object->CXObjIntp::UpdateAnim(frames);
	CJObj joint;
	if (object->_C90.mFlags & 8) {
		s32 path[7] = { 0, 3, 2, 0, 0, 1, 0 };
		joint.Attach(FindJointByPath(object->mJObj, path, 7));
		HSD_JObjClearFlags(joint.mJObj, 0x10);
	} else {
		s32 path[7] = { 0, 3, 2, 0, 0, 1, 0 };
		joint.Attach(FindJointByPath(object->mJObj, path, 7));
		HSD_JObjSetFlags(joint.mJObj, 0x10);
	}
}
