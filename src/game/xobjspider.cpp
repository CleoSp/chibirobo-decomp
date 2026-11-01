#include "game/xobjspider.h"
#include "game/cgame.h"

extern "C" {
extern AnimSfxEvent lbl_8022AB90[];
extern u8 lbl_8065B2C0;
extern u32 lbl_8065B2BC;
}

/**
 * @note Address: 0x800F373C
 * @note Size: 0x15C
 */
extern "C" void fn_800F373C(XObjSpider* spider)
{
	fn_800928E8(spider);
	spider->mSfx.mEvents = lbl_8022AB90;
	if (spider->_258 == 0) {
		for (u32 i = 0; i < 16; ++i) {
			lbl_8037E5C0[i] = NULL;
			lbl_8037E600[i] = -1;
		}
		lbl_8065B2C0  = 0;
		lbl_8065B2BC  = 0;
		s32 found     = 0;
		s32 count     = lbl_8025C400.mObjectCount;
		XObj** output = lbl_8037E5C0;
		for (s32 i = 0; i < count; ++i) {
			XObj* candidate = lbl_8025C400.mObjects[i];
			if (candidate->mStatus.mFlags & 1ULL) {
				UnkStageObj* placement = candidate->mCoord._00;
				if (placement->_00 == 0x13C) {
					++found;
					*output++ = candidate;
					if (found >= 16)
						break;
				}
			}
		}
	}
}

/**
 * @note Address: 0x800F3898
 * @note Size: 0x5C
 */
extern "C" BOOL fn_800F3898(XObjSpider* spider, CoordInfo* info)
{
	if (fn_80091C80(spider, info)) {
		spider->mFlags.mFlags &= ~0x04000000U;
		fn_800F4350(spider);
		fn_800F38F4(spider);
		return TRUE;
	}
	return FALSE;
}
