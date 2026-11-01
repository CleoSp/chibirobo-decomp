#include "game/xobjattachment.h"
#include "game/cjobjinlineattach.h"

// tilts the dish
/**
 * @note Address: 0x800D58E4
 * @note Size: 0x374
 */
extern "C" void fn_800D58E4(XObjRadar* object, f32 angle)
{
	s32 path[4] = { 0, 0, 0, 0 };
	CJObj joint(FindJointByPath(object->mJObj, path, 4));
	joint.SetRotationX(object->mPitch + angle, 0);
}
