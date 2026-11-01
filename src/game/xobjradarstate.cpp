#include "game/xobjattachment.h"
#include "game/cgame.h"
#include "game/csound.h"

/**
 * @note Address: 0x800D626C
 * @note Size: 0xD4
 */
extern "C" void fn_800D626C(XObjRadar* object)
{
	object->mState = 10;
	object->SetAnim(2, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	object->UpdateAnim(0);
	object->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	if (object->mVisible == FALSE)
		object->Show();
}

/**
 * @note Address: 0x800D6340
 * @note Size: 0x4
 */
extern "C" void fn_800D6340(XObjRadar* object)
{
}

// todo: cleanup
static inline void StopIfPlaying(CSound* sound, s32 effect)
{
	if (fn_8001E6C8(sound, effect))
		fn_8001E660(sound, effect);
}

/**
 * @note Address: 0x800D6344
 * @note Size: 0xD4
 */
extern "C" void fn_800D6344(XObjRadar* object)
{
	object->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	object->mState = 2;
	CSound* sound  = (CSound*)&lbl_8025C400.mSound;
	StopIfPlaying(sound, 0x18C);
	StopIfPlaying(sound, 0x18D);
	StopIfPlaying(sound, 0x18E);
}
