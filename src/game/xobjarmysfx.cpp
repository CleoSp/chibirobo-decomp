#include "game/xobjarmy.h"
#include "game/cgame.h"
#include "game/hsd_cobj.h"

/**
 * @note Address: 0x800E6A5C
 * @note Size: 0x114
 */
extern "C" void fn_800E6A5C(XObjArmy* army)
{
	u8 volume;
	if (army->mFlags.mFlags & 0x100)
		volume = 255;
	else
		volume = fn_8001DF00((CSound*)&lbl_8025C400.mSound, &army->mMoveTranslate, 0);
	HSD_JObj* joint = army->mJObj;
	s32 id          = army->mAnimId;
	HSD_AObj* aobj  = GetAObj(joint);
	f32 frame       = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
	army->mSfx.Update(id, frame, volume);
}
