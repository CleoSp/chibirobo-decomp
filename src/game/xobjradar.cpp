#include "game/xobjattachment.h"
extern "C" {
#include <dolphin/mtx/mtx.h>
#include <dolphin/mtx/vec.h>
}
#include "game/cvec.h"

struct RadarJointPath {
	s32 mStep[4];
};

extern "C" const RadarJointPath lbl_8020A7D0;

// where the dish points from
/**
 * @note Address: 0x800D5F94
 * @note Size: 0x5C
 */
extern "C" CVec fn_800D5F94(CJObj* object)
{
	RadarJointPath path = lbl_8020A7D0;
	return fn_8007E7F4(object, 4, path.mStep);
}

// XObjRadar::vfA4
/**
 * @note Address: 0x800D5FF0
 * @note Size: 0x27C
 */
extern "C" void fn_800D5FF0(XObjRadar* object, u32 frames)
{
	if (object->mVisible) {
		HSD_JObjCopyMtx(object->mJObj, HSD_JObjGetMtxPtr(object->mParent));
		HSD_AObj* animation = GetAObj(object->mJObj);
		switch (object->mState) {
		case 1:
			if (animation->flags & 0x44000000) {
				object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				object->mState = 10;
			}
			break;
		case 2:
			if (animation->flags & 0x44000000) {
				object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				object->Hide();
				object->mState = 0;
			}
			break;
		case 10:
			break;
		}
		fn_80090ED4(object, frames);
	}
}
