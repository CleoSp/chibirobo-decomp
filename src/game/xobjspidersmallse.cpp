#include "game/xobjspidersmall.h"
#include "game/cgame.h"
#include "game/infoscrn.h"
#include "game/hsd_cobj.h"

/**
 * @note Address: 0x800F4AAC
 * @note Size: 0x164
 */
extern "C" void fn_800F4AAC(XObjSpiderSmall* spider)
{
	if (spider->mStatus.mFlags & 1ULL) {
		if (spider->mStatus.mFlags & 0x10000ULL) {
			if (!fn_800665E0((InfoScrn*)lbl_8025C400.mInfoScrn)) {
				HSD_JObj* jobj = spider->mJObj;
				s32 id         = spider->mAnimId;
				HSD_AObj* aobj = GetAObj(jobj);
				f32 frame      = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
				spider->mAnimSfx.Update(id, frame, fn_8001DF00((CSound*)&lbl_8025C400.mSound, &spider->mMoveTranslate, 0));
			}
		}
	}
}

/**
 * @note Address: 0x800F4C10
 * @note Size: 0x1A8
 */
extern "C" BOOL fn_800F4C10(void* unused, u32 index, u32 frames)
{
	u32 i = (u8)index;
	if (lbl_8037E910[i] == NULL) {
		return TRUE;
	}
	HSD_AObj* aobj = GetAObj(lbl_8037E910[i]->mJObj);
	f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
	aobj           = GetAObj(lbl_8037E910[i]->mJObj);
	f32 frame      = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
	if (end - (f32)frames <= frame) {
		return TRUE;
	}
	return FALSE;
}
