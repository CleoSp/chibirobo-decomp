#include "game/xobjattachment.h"
#include "game/cgame.h"

// XObjPopper and XObjCopter, and the XObjAttachment base they share

extern "C" {

/**
 * @note Address: 0x800D6FE8
 * @note Size: 0x398
 */
void fn_800D6FE8(XObjPopper* object, u32 frames)
{
	if (object->mVisible) {
		CopyParentMtx(object);
		HSD_AObj* aobj = GetAObj(object->mJObj);
		s32 anim       = object->mAnimId;
		f32 frame      = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		switch (anim) {
		case 1:
			if (aobj->flags & 0x44000000) {
				object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
			break;
		case 2:
			if (aobj->flags & 0x44000000) {
				object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				object->Hide();
			}
			break;
		case 3:
			if (aobj->flags & 0x44000000) {
				object->SetAnim(4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
			break;
		case 4:
			break;
		case 5:
			if (aobj->flags & 0x44000000) {
				object->SetAnim(4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
			break;
		case 6:
			if (aobj->flags & 0x44000000) {
				object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
			break;
		}
		fn_80090ED4(object, frames);
	}
}

/**
 * @note Address: 0x800D7380
 * @note Size: 0xCC
 */
void fn_800D7380(XObjPopper* object)
{
	object->SetAnim(2, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	object->UpdateAnim(0);
	object->SetAnim(4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	if (!object->mVisible) {
		object->Show();
	}
}

/**
 * @note Address: 0x800D744C
 * @note Size: 0x50
 */
void fn_800D744C(XObjPopper* object)
{
	object->SetAnim(6, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

/**
 * @note Address: 0x800D749C
 * @note Size: 0x14
 */
BOOL fn_800D749C(XObjPopper* object)
{
	return object->mAnimId == 5;
}

/**
 * @note Address: 0x800D74B0
 * @note Size: 0x50
 */
void fn_800D74B0(XObjPopper* object)
{
	object->SetAnim(5, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

/**
 * @note Address: 0x800D7500
 * @note Size: 0x50
 */
void fn_800D7500(XObjPopper* object)
{
	object->SetAnim(3, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

/**
 * @note Address: 0x800D7550
 * @note Size: 0xA8
 */
void fn_800D7550(XObjPopper* object)
{
	object->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	lbl_8025C400.mSound.StopIfPlaying(0x186);
	lbl_8025C400.mSound.StopIfPlaying(0x18B);
}

/**
 * @note Address: 0x800D75F8
 * @note Size: 0x70
 */
void fn_800D75F8(XObjPopper* object)
{
	object->Show();
	object->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

/**
 * @note Address: 0x800D7668
 * @note Size: 0x344
 */
void fn_800D7668(XObjCopter* object, u32 frames)
{
	if (object->mVisible) {
		CopyParentMtx(object);
		HSD_AObj* aobj = GetAObj(object->mJObj);
		s32 anim       = object->mAnimId;
		f32 frame      = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		switch (anim) {
		case 5:
			if (aobj->flags & 0x44000000) {
				object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
			break;
		case 8:
			if (aobj->flags & 0x44000000) {
				object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				object->Hide();
			}
			break;
		case 6:
			if (aobj->flags & 0x44000000) {
				object->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
			break;
		case 7:
			if (aobj->flags & 0x44000000) {
				object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
			break;
		}
		fn_80090ED4(object, frames);
	}
}

/**
 * @note Address: 0x800D79AC
 * @note Size: 0xCC
 */
void fn_800D79AC(XObjCopter* object)
{
	object->SetAnim(8, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	object->UpdateAnim(0);
	object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	if (!object->mVisible) {
		object->Show();
	}
}

/**
 * @note Address: 0x800D7A78
 * @note Size: 0x50
 */
void fn_800D7A78(XObjCopter* object)
{
	object->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

/**
 * @note Address: 0x800D7AC8
 * @note Size: 0x50
 */
void fn_800D7AC8(XObjCopter* object)
{
	object->SetAnim(3, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

/**
 * @note Address: 0x800D7B18
 * @note Size: 0x50
 */
void fn_800D7B18(XObjCopter* object)
{
	object->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

/**
 * @note Address: 0x800D7B68
 * @note Size: 0x88
 */
void fn_800D7B68(XObjCopter* object)
{
	object->SetAnim(8, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	lbl_8025C400.mSound.StopIfPlaying(8);
}

/**
 * @note Address: 0x800D7BF0
 * @note Size: 0x70
 */
void fn_800D7BF0(XObjCopter* object)
{
	object->Show();
	object->SetAnim(5, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

/**
 * @note Address: 0x800D7C60
 * @note Size: 0x40
 */
void fn_800D7C60(XObjAttachment* object)
{
	object->Hide();
	fn_800911C0(object);
}

/**
 * @note Address: 0x800D7CA0
 * @note Size: 0x20
 */
void fn_800D7CA0(XObjAttachment* object)
{
	fn_800928E8(object);
}

/**
 * @note Address: 0x800D7CC0
 * @note Size: 0x8
 */
void fn_800D7CC0(XObjAttachment* object, HSD_JObj* parent)
{
	object->mParent = parent;
}

/**
 * @note Address: 0x800D7CC8
 * @note Size: 0x34
 */
void fn_800D7CC8(XObjAttachment* object)
{
	object->mParent = NULL;
	object->Hide();
}

/**
 * @note Address: 0x800D7CFC
 * @note Size: 0x40
 */
void fn_800D7CFC(XObjAttachment* object, u32 id)
{
	fn_800929C4(object, id);
	object->vf94();
}

} // extern "C"
