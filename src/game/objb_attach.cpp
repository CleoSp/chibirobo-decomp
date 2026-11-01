#include "game/particle_srt.h"
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

// ---- functions ----

extern "C" {

/**
 * @note Address: 0x800D552C
 * @note Size: 0x54
 */
void fn_800D552C(XObjAttachment* obj)
{
	obj->mShown = TRUE;
	if (obj->mFlags & 1) {
		HSD_JObjClearFlagsAll(obj->mJObj, 0x10);
	}
}

/**
 * @note Address: 0x800D5580
 * @note Size: 0x3C
 */
void fn_800D5580(XObjAttachment* obj)
{
	if (obj->mShown) {
		obj->vfB0(0);
	}
}

/**
 * @note Address: 0x800D55BC
 * @note Size: 0x248
 */
void fn_800D55BC(XObjOX* obj, u32 frames)
{
	if (obj->mShown) {
		CopyOwnerMtx(obj);
		HSD_AObj* aobj = GetAObj(obj->mJObj);
		s32 anim       = obj->mAnimId;
		f32 frame      = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		switch (anim) {
		case 0:
			if (aobj->flags & 0x44000000) {
				obj->Hide();
			}
			break;
		case 1:
			if (aobj->flags & 0x44000000) {
				obj->Hide();
			}
			break;
		}
		fn_80090ED4(obj, frames);
	}
}

/**
 * @note Address: 0x800D5804
 * @note Size: 0x70
 */
void fn_800D5804(XObjOX* obj)
{
	obj->Show();
	obj->PlayAnim(1, TRUE);
}

/**
 * @note Address: 0x800D5874
 * @note Size: 0x70
 */
void fn_800D5874(XObjOX* obj)
{
	obj->Show();
	obj->PlayAnim(0, TRUE);
}

} // extern "C"
