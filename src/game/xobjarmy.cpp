#include "game/xobjarmy.h"
#include "game/cgame.h"

extern "C" {
extern s32 lbl_8065B2A8;
extern AnimSfxEvent lbl_8022A53C[];
extern AnimSfxEvent lbl_8022A5FC[];
}

/**
 * @note Address: 0x800EA17C
 * @note Size: 0x144
 */
extern "C" void fn_800EA17C(XObjArmy* army)
{
	fn_800928E8(army);
	lbl_8065B2A8        = 0;
	lbl_8065B2AC        = 0;
	lbl_8065B2B0.mFlags = 0;
	army->mFunc         = NULL;
	if (army->_258 == 0) {
		lbl_8065B2A0 = NULL;
		s32 count    = lbl_8025C400.mObjectCount;
		for (s32 i = 0; lbl_8065B2A0 == NULL && i < count; ++i) {
			XObj* obj = lbl_8025C400.mObjects[i];
			if ((obj->mStatus.mFlags & 1ULL) && obj->mCoord._00->_00 == 0x12A) {
				lbl_8065B2A0 = obj;
			}
		}
	}
	switch (army->GetKind()) {
	case 0xF7:
		army->mSfx.mEvents = lbl_8022A53C;
		break;
	case 0xF8:
		army->mSfx.mEvents = lbl_8022A5FC;
		break;
	}
	army->mShots[0].mActive = 0;
	army->mShots[1].mActive = 0;
	army->mShots[2].mActive = 0;
	army->mShots[3].mActive = 0;
	army->mShots[4].mActive = 0;
}
