#include "game/xobjspiderboss.h"

extern "C" const f32 lbl_8065E0BC;
extern "C" const f32 lbl_8065E0C0;
extern "C" const f32 lbl_8065E028;

/**
 * @note Address: 0x80103EC0
 * @note Size: 0x94
 */
extern "C" void State0__14XObjSpiderBossFUl(XObjSpiderBoss* spider)
{
	spider->mSpiderStatus.mFlags &= ~0x80000U;
	u32 flags = spider->mSpiderStatus.mFlags;
	if ((flags >> 25) & 1) {
		return;
	}
	if (flags & 0x40000000) {
		spider->mStep      = 0;
		spider->mTimer     = 0;
		spider->mPrevState = spider->mState;
		spider->mState     = 4;
		spider->mAreaPos.x = lbl_8065E0BC;
		spider->mAreaPos.y = lbl_8065E028;
		spider->mAreaPos.z = lbl_8065E0BC;
		spider->mAreaWidth = lbl_8065E0C0;
		spider->mAreaDepth = lbl_8065E0C0;
	} else {
		spider->mStep      = 0;
		spider->mTimer     = 0;
		spider->mPrevState = spider->mState;
		spider->mState     = 3;
		spider->mStep      = 100;
	}
	spider->mSpiderStatus.mFlags &= ~0x8000U;
}
