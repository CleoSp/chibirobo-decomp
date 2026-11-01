#include "game/xobjattachment.h"
#include "game/hsd_cobj.h"

extern "C" {
#include <dolphin/mtx/mtx.h>
}

// XObjAttachment::Show
/**
 * @note Address: 0x800D552C
 * @note Size: 0x54
 */
extern "C" void fn_800D552C(XObjAttachment* object)
{
	object->mVisible = TRUE;
	if (object->mStatus.mFlags & 1ULL)
		HSD_JObjClearFlagsAll(object->mJObj, 0x10);
}

// XObjAttachment::vfAC
/**
 * @note Address: 0x800D5580
 * @note Size: 0x3C
 */
extern "C" void fn_800D5580(XObjAttachment* object)
{
	if (object->mVisible)
		object->vfB0(NULL);
}

// XObjOX::vfA4
/**
 * @note Address: 0x800D55BC
 * @note Size: 0x248
 */
extern "C" void fn_800D55BC(XObjOX* object, u32 frames)
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
		case 0:
			if (animation->flags & 0x44000000)
				object->Hide();
			break;
		case 1:
			if (animation->flags & 0x44000000)
				object->Hide();
			break;
		}
		fn_80090ED4(object, frames);
	}
}

/**
 * @note Address: 0x800D5804
 * @note Size: 0x70
 */
extern "C" void fn_800D5804(XObjOX* object)
{
	object->Show();
	object->SetAnim(1, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

/**
 * @note Address: 0x800D5874
 * @note Size: 0x70
 */
extern "C" void fn_800D5874(XObjOX* object)
{
	object->Show();
	object->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}
