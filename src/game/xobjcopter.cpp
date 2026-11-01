#include "game/xobjattachment.h"
#include "game/hsd_cobj.h"

// XObjCopter::vfA4
/**
 * @note Address: 0x800D7668
 * @note Size: 0x344
 */
extern "C" void fn_800D7668(XObjCopter* object, u32 frames)
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
		case 5:
			if (animation->flags & 0x44000000)
				object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			break;
		case 8:
			if (animation->flags & 0x44000000) {
				object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				object->Hide();
			}
			break;
		case 6:
			if (animation->flags & 0x44000000)
				object->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			break;
		case 7:
			if (animation->flags & 0x44000000)
				object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			break;
		}
		fn_80090ED4(object, frames);
	}
}
