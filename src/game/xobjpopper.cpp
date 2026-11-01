#include "game/xobjattachment.h"
#include "game/hsd_cobj.h"

extern "C" {
#include <dolphin/mtx/mtx.h>
}

// XObjPopper::vfA4
/**
 * @note Address: 0x800D6FE8
 * @note Size: 0x398
 */
extern "C" void fn_800D6FE8(XObjPopper* object, u32 frames)
{
	if (object->mVisible) {
		HSD_JObjCopyMtx(object->mJObj, HSD_JObjGetMtxPtr(object->mParent));
		HSD_AObj* animation = GetAObj(object->mJObj);
		s32 id              = object->mAnimId;
		if (animation != NULL)
			HSD_AObjGetCurrentFrame(animation);
		if (animation != NULL)
			HSD_AObjGetEndFrame(animation);
		switch (id) {
		case 1:
			if (animation->flags & 0x44000000)
				object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			break;
		case 2:
			if (animation->flags & 0x44000000) {
				object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				object->Hide();
			}
			break;
		case 3:
			if (animation->flags & 0x44000000)
				object->SetAnim(4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			break;
		case 4:
			break;
		case 5:
			if (animation->flags & 0x44000000)
				object->SetAnim(4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			break;
		case 6:
			if (animation->flags & 0x44000000)
				object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			break;
		}
		fn_80090ED4(object, frames);
	}
}
