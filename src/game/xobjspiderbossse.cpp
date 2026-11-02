#include "game/xobjspiderboss.h"
#include "game/cgame.h"
#include "game/infoscrn.h"
#include "game/hsd_cobj.h"

/**
 * @note Address: 0x801002DC
 * @note Size: 0x148
 */
extern "C" void fn_801002DC(XObjSpiderBoss* spider)
{
	if (spider->mStatus.mFlags & 1ULL) {
		if (spider->mStatus.mFlags & 0x10000ULL) {
			if (!fn_800665E0(&lbl_8025C400.mInfoScrn)) {
				s32 id         = (spider->mSpiderStatus.mFlags & 0x40000000) ? spider->mAltAnimId : spider->mAnimId;
				HSD_AObj* aobj = GetAObj(spider->mJObj);
				f32 frame      = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
				spider->mAnimSfx.Update(id, frame, 255);
			}
		}
	}
}

/**
 * @note Address: 0x80100424
 * @note Size: 0xC
 */
extern "C" bool fn_80100424(XObjSpiderBoss* spider)
{
	return (spider->mSpiderStatus.mFlags & 0x200) != 0;
}
