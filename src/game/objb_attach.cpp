#include "game/xobjattachment.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/cjobjinlineattach.h"
#include "game/evt.h"
#include <math.h>

// XObjAttachment, the things chibi robo carries, and the four that have their own code:
// XObjOX, XObjRadar, XObjPopper and XObjCopter

extern "C" {

/**
 * @note Address: 0x800D552C
 * @note Size: 0x54
 */
void fn_800D552C(XObjAttachment* object)
{
	object->mVisible = TRUE;
	if (object->mStatus.mFlags & 1) {
		HSD_JObjClearFlagsAll(object->mJObj, 0x10);
	}
}

/**
 * @note Address: 0x800D5580
 * @note Size: 0x3C
 */
void fn_800D5580(XObjAttachment* object)
{
	if (object->mVisible) {
		object->vfB0(NULL);
	}
}

/**
 * @note Address: 0x800D55BC
 * @note Size: 0x248
 */
void fn_800D55BC(XObjOX* object, u32 frames)
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
		fn_80090ED4(object, frames);
	}
}

/**
 * @note Address: 0x800D5804
 * @note Size: 0x70
 */
void fn_800D5804(XObjOX* object)
{
	object->Show();
	object->SetAnim(1, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

/**
 * @note Address: 0x800D5874
 * @note Size: 0x70
 */
void fn_800D5874(XObjOX* object)
{
	object->Show();
	object->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

// tilts the dish
/**
 * @note Address: 0x800D58E4
 * @note Size: 0x374
 */
// NONMATCHING: rodata order
void fn_800D58E4(XObjRadar* object, f32 angle)
{
	s32 path[4] = { 0, 0, 0, 0 };
	CJObj joint(FindJointByPath(object->mJObj, path, 4));
	joint.SetRotationX(object->mPitch + angle, 0);
}

/**
 * @note Address: 0x800D5F94
 * @note Size: 0x5C
 */
CVec fn_800D5F94(XObjRadar* object)
{
	s32 path[4] = { 0, 0, 0, 0 };
	return fn_8007E7F4(object, 4, path);
}

/**
 * @note Address: 0x800D5FF0
 * @note Size: 0x27C
 */
void fn_800D5FF0(XObjRadar* object, u32 frames)
{
	if (object->mVisible) {
		CopyParentMtx(object);
		HSD_AObj* aobj = GetAObj(object->mJObj);
		switch (object->mState) {
		case 1:
			if (aobj->flags & 0x44000000) {
				object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				object->mState = 10;
			}
			break;
		case 2:
			if (aobj->flags & 0x44000000) {
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

/**
 * @note Address: 0x800D626C
 * @note Size: 0xD4
 */
void fn_800D626C(XObjRadar* object)
{
	object->mState = 10;
	object->SetAnim(2, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	object->UpdateAnim(0);
	object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	if (!object->mVisible) {
		object->Show();
	}
}

/**
 * @note Address: 0x800D6344
 * @note Size: 0xD4
 */
void fn_800D6344(XObjRadar* object)
{
	object->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	object->mState = 2;
	lbl_8025C400.mSound.StopIfPlaying(0x18C);
	lbl_8025C400.mSound.StopIfPlaying(0x18D);
	lbl_8025C400.mSound.StopIfPlaying(0x18E);
}

/**
 * @note Address: 0x800D6340
 * @note Size: 0x4
 */
void fn_800D6340(XObjRadar* object)
{
}

} // extern "C"
