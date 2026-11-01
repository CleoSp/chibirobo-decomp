#include "game/xobjattachment.h"
#include "game/cgame.h"
#include "game/csound.h"

// todo: cleanup
static inline void SetAnimation(XObj* object, s32 id, BOOL force)
{
	object->SetAnim(id, force, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

/**
 * @note Address: 0x800D79AC
 * @note Size: 0xCC
 */
extern "C" void fn_800D79AC(XObjCopter* object)
{
	SetAnimation(object, 8, TRUE);
	object->UpdateAnim(0);
	SetAnimation(object, 0, FALSE);
	if (object->mVisible == FALSE)
		object->Show();
}

/**
 * @note Address: 0x800D7A78
 * @note Size: 0x50
 */
extern "C" void fn_800D7A78(XObjCopter* object)
{
	SetAnimation(object, 7, FALSE);
}
/**
 * @note Address: 0x800D7AC8
 * @note Size: 0x50
 */
extern "C" void fn_800D7AC8(XObjCopter* object)
{
	SetAnimation(object, 3, FALSE);
}
/**
 * @note Address: 0x800D7B18
 * @note Size: 0x50
 */
extern "C" void fn_800D7B18(XObjCopter* object)
{
	SetAnimation(object, 2, FALSE);
}

/**
 * @note Address: 0x800D7B68
 * @note Size: 0x88
 */
extern "C" void fn_800D7B68(XObjCopter* object)
{
	SetAnimation(object, 8, FALSE);
	CSound* sound = (CSound*)&lbl_8025C400.mSound;
	if (fn_8001E6C8(sound, 8))
		fn_8001E660(sound, 8);
}

/**
 * @note Address: 0x800D7BF0
 * @note Size: 0x70
 */
extern "C" void fn_800D7BF0(XObjCopter* object)
{
	object->Show();
	SetAnimation(object, 5, FALSE);
}

// XObjAttachment::Release
/**
 * @note Address: 0x800D7C60
 * @note Size: 0x40
 */
extern "C" void fn_800D7C60(XObjAttachment* object)
{
	object->Hide();
	fn_800911C0(object);
}

// XObjAttachment::vf98
/**
 * @note Address: 0x800D7CA0
 * @note Size: 0x20
 */
extern "C" void fn_800D7CA0(XObjAttachment* object)
{
	fn_800928E8(object);
}

// XObjAttachment::SetParent
/**
 * @note Address: 0x800D7CC0
 * @note Size: 0x8
 */
extern "C" void fn_800D7CC0(XObjAttachment* object, HSD_JObj* parent)
{
	object->mParent = parent;
}

// XObjAttachment::vf94
/**
 * @note Address: 0x800D7CC8
 * @note Size: 0x34
 */
extern "C" void fn_800D7CC8(XObjAttachment* object)
{
	object->mParent = NULL;
	object->Hide();
}

// XObjAttachment::vf90
/**
 * @note Address: 0x800D7CFC
 * @note Size: 0x40
 */
extern "C" void fn_800D7CFC(XObjAttachment* object, u32 id)
{
	fn_800929C4(object, id);
	object->vf94();
}
