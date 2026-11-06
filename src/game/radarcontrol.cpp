extern "C" const char sAttachmentEulerCondition[], sAttachmentTranslationCondition[];
#define JOBJ_EULER_CONDITION       sAttachmentEulerCondition
#define JOBJ_TRANSLATION_CONDITION sAttachmentTranslationCondition
#include "game/xobjattachment.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/cjobjinlineattach.h"

#pragma section RX ".text.radarcontrol"
extern "C" {
extern const s32 sRadarYawRoot;

/**
 * @note Address: 0x800D5C58
 * @note Size: 0x33C
 */
__declspec(section ".text.radarcontrol") void SetRadarDishYaw(XObjRadar* object, f32 angle)
{
	s32 path[] = { sRadarYawRoot };
	CJObj joint(FindJointByPath(object->mJObj, path, 1));
	joint.SetRotationY(object->mYaw + angle, 0);
}

/**
 * @note Address: 0x800D5F94
 * @note Size: 0x5C
 */
__declspec(section ".text.radarcontrol") CVec GetRadarDishWorldPosition(XObjRadar* object)
{
	s32 path[4] = { 0, 0, 0, 0 };
	return CJObj_GetJointWorldPositionByPath(object, 4, path);
}

/**
 * @note Address: 0x800D5FF0
 * @note Size: 0x27C
 */
__declspec(section ".text.radarcontrol") void UpdateObject__9XObjRadarFUl(XObjRadar* object, u32 frames)
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
		UpdateObject__4XObjFUl(object, frames);
	}
}

/**
 * @note Address: 0x800D626C
 * @note Size: 0xD4
 */
__declspec(section ".text.radarcontrol") void ShowIdlePlayerRadar(XObjRadar* object)
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
 * @note Address: 0x800D6340
 * @note Size: 0x4
 */
__declspec(section ".text.radarcontrol") void fn_800D6340(XObjRadar* object)
{
}

/**
 * @note Address: 0x800D6344
 * @note Size: 0xD4
 */
__declspec(section ".text.radarcontrol") void DeactivatePlayerRadar(XObjRadar* object)
{
	object->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	object->mState = 2;
	gGame.mSound.StopIfPlaying(0x18C);
	gGame.mSound.StopIfPlaying(0x18D);
	gGame.mSound.StopIfPlaying(0x18E);
}

} // extern "C"
