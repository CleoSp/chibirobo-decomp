#include "game/xchr.h"
#include "game/cgame.h"
#include "game/cshadow.h"
#include "game/video.h"

// the anim rate SetAnimRate would give him
/**
 * @note Address: 0x800A4574
 * @note Size: 0x44
 */
extern "C" f32 fn_800A4574(XObjChr* chr)
{
	f32 rate;
	if (chr->mLift.mState == 4 && fn_800D4FE8(&chr->mLift)) {
		rate = 1.135f;
	} else {
		rate = 1.0f;
	}
#if defined(VERSION_PAL)
	return rate * (60.0f / (f32)lbl_80660D4C);
#else
	return rate;
#endif
}

// XObjChr::vfD0, his attachments cast shadows too, the plug only under game flag 0x0040000000000000
/**
 * @note Address: 0x800A45B8
 * @note Size: 0xC0
 */
extern "C" void fn_800A45B8(XObjChr* chr, CShadow* shadow)
{
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mJObj);
	if (lbl_8025C400.mStatus.mFlags & 0x0040000000000000ULL) {
		HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mPlug.mJObj);
	}
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mCopterObj.mJObj);
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mPopperObj.mJObj);
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mRadarObj.mJObj);
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mOXObj.mJObj);
}
