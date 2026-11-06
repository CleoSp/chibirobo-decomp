#include "game/xobjattachment.h"

extern "C" {

/**
 * @note Address: 0x800D55BC
 * @note Size: 0x248
 */
void UpdateObject__6XObjOXFUl(XObjOX* object, u32 frames)
{
	if (object->mVisible) {
		CopyParentMtx(object);
		HSD_AObj* aobj = GetAObj(object->mJObj);
		s32 anim       = object->mAnimId;
		f32 frame      = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		switch (anim) {
		case 0:
			if (aobj->flags & 0x44000000) {
				object->Hide();
			}
			break;
		case 1:
			if (aobj->flags & 0x44000000) {
				object->Hide();
			}
			break;
		}
		UpdateObject__4XObjFUl(object, frames);
	}
}

/**
 * @note Address: 0x800D5804
 * @note Size: 0x70
 */
void ShowOXCross(XObjOX* object)
{
	object->Show();
	object->SetAnim(1, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

/**
 * @note Address: 0x800D5874
 * @note Size: 0x70
 */
void ShowOXCircle(XObjOX* object)
{
	object->Show();
	object->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

} // extern "C"
