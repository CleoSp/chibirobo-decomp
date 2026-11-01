#include "game/hsd_jobj.h"
#include "game/cparticle.h"
#include "game/objb.h"
#include <math.h>

// XObjAttachment, the things the robot carries, and the four that have their own code:
// XObjOX, XObjRadar, XObjPopper and XObjCopter

// where a joint of the object is, found by its path down the tree
extern "C" CVec fn_8007E7F4(XObj* obj, s32 depth, s32* path);

// follows the joint it hangs from
static inline void CopyOwnerMtx(XObjAttachment* obj)
{
	MtxPtr mtx = HSD_JObjGetMtxPtr(obj->mOwner);
	HSD_JObjCopyMtx(obj->mJObj, mtx);
}

static inline void StopSound(s32 id)
{
	if (fn_8001E6C8(GAME_SOUND, id)) {
		fn_8001E660(GAME_SOUND, id);
	}
}
extern "C" {

/**
 * @note Address: 0x800D7668
 * @note Size: 0x344
 */
void fn_800D7668(XObjCopter* obj, u32 frames)
{
	if (obj->mShown) {
		CopyOwnerMtx(obj);
		HSD_AObj* aobj = GetAObj(obj->mJObj);
		s32 anim       = obj->mAnimId;
		f32 frame      = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		switch (anim) {
		case 5:
			if (aobj->flags & 0x44000000) {
				obj->PlayAnim(0, FALSE);
			}
			break;
		case 8:
			if (aobj->flags & 0x44000000) {
				obj->PlayAnim(0, FALSE);
				obj->Hide();
			}
			break;
		case 6:
			if (aobj->flags & 0x44000000) {
				obj->PlayAnim(2, FALSE);
			}
			break;
		case 7:
			if (aobj->flags & 0x44000000) {
				obj->PlayAnim(0, FALSE);
			}
			break;
		}
		fn_80090ED4(obj, frames);
	}
}
}
