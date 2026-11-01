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
 * @note Address: 0x800D5F94
 * @note Size: 0x5C
 */
CVec fn_800D5F94(XObjRadar* obj)
{
	s32 path[4] = { 0, 0, 0, 0 };
	return fn_8007E7F4(obj, 4, path);
}

/**
 * @note Address: 0x800D5FF0
 * @note Size: 0x27C
 */
void fn_800D5FF0(XObjRadar* obj, u32 frames)
{
	if (obj->mShown) {
		CopyOwnerMtx(obj);
		HSD_AObj* aobj = GetAObj(obj->mJObj);
		switch (obj->mState) {
		case 1:
			if (aobj->flags & 0x44000000) {
				obj->PlayAnim(0, FALSE);
				obj->mState = 10;
			}
			break;
		case 2:
			if (aobj->flags & 0x44000000) {
				obj->PlayAnim(0, FALSE);
				obj->Hide();
				obj->mState = 0;
			}
			break;
		case 10:
			break;
		}
		fn_80090ED4(obj, frames);
	}
}
}
