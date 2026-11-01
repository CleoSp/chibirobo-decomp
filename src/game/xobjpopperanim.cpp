#include "game/xobjattachment.h"
#include "game/cgame.h"
#include "game/csound.h"

// todo: cleanup
static inline void SetAnimation(XObj* object, s32 id, BOOL force)
{
	object->SetAnim(id, force, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

/**
 * @note Address: 0x800D7380
 * @note Size: 0xCC
 */
extern "C" void fn_800D7380(XObjPopper* object)
{
	SetAnimation(object, 2, TRUE);
	object->UpdateAnim(0);
	SetAnimation(object, 4, FALSE);
	if (object->mVisible == FALSE)
		object->Show();
}

/**
 * @note Address: 0x800D744C
 * @note Size: 0x50
 */
extern "C" void fn_800D744C(XObjPopper* object)
{
	SetAnimation(object, 6, FALSE);
}
/**
 * @note Address: 0x800D749C
 * @note Size: 0x14
 */
extern "C" u8 fn_800D749C(XObjPopper* object)
{
	return object->mAnimId == 5;
}
/**
 * @note Address: 0x800D74B0
 * @note Size: 0x50
 */
extern "C" void fn_800D74B0(XObjPopper* object)
{
	SetAnimation(object, 5, FALSE);
}
/**
 * @note Address: 0x800D7500
 * @note Size: 0x50
 */
extern "C" void fn_800D7500(XObjPopper* object)
{
	SetAnimation(object, 3, FALSE);
}

/**
 * @note Address: 0x800D7550
 * @note Size: 0xA8
 */
extern "C" void fn_800D7550(XObjPopper* object)
{
	SetAnimation(object, 2, FALSE);
	CSound* sound = (CSound*)&lbl_8025C400.mSound;
	if (fn_8001E6C8(sound, 0x186))
		fn_8001E660(sound, 0x186);
	if (fn_8001E6C8(sound, 0x18B))
		fn_8001E660(sound, 0x18B);
}

/**
 * @note Address: 0x800D75F8
 * @note Size: 0x70
 */
extern "C" void fn_800D75F8(XObjPopper* object)
{
	object->Show();
	SetAnimation(object, 1, FALSE);
}
