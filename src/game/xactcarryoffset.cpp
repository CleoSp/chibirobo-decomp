#include "game/xchr.h"
#include "game/cjobjinlineattach.h"
extern "C" {
#include <sysdolphin/mtx.h>
extern const s32 lbl_80660158;
}

// todo: cleanup
static inline HSD_JObj* FindRelativeOffsetJoint(HSD_JObj* joint, s32 index)
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

/**
 * @note Address: 0x800CE9A8
 * @note Size: 0x354
 */
extern "C" Vec fn_800CE9A8(XAction* action, XObj* object)
{
	if ((s16)object->GetPool() == 0x18) {
		CJObj joint(FindRelativeOffsetJoint(object->mJObj, lbl_80660158));
		CVec position(GetJointPosition(joint.mJObj));
		return position - object->mMoveTranslate;
	}
	Vec offset = object->mMoveTranslate * -1.0f;
	return fn_8008C274(object, action->mTargetFloor, &offset);
}
