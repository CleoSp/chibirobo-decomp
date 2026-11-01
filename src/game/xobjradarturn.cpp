#include "game/xobjattachment.h"
#include "game/cjobjinlineattach.h"

extern "C" const s32 lbl_80660230;

// todo: cleanup
static inline HSD_JObj* FindYawJoint(HSD_JObj* joint, s32 index)
{
	if (!joint)
		return NULL;
	joint = HSD_JObjGetChild(joint);
	for (s32 i = 0; i < index; i++) {
		joint = HSD_JObjGetNext(joint);
		if (!joint)
			return NULL;
	}
	return joint;
}

// turns the dish
/**
 * @note Address: 0x800D5C58
 * @note Size: 0x33C
 */
extern "C" void fn_800D5C58(XObjRadar* object, f32 angle)
{
	CJObj joint(FindYawJoint(object->mJObj, lbl_80660230));
	joint.SetRotationY(object->mYaw + angle, 0);
}
