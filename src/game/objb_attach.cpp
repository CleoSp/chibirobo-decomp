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

/**
 * @note Address: 0x800D626C
 * @note Size: 0xD4
 */
void fn_800D626C(XObjRadar* obj)
{
	obj->mState = 10;
	obj->PlayAnim(2, TRUE);
	obj->UpdateAnim(0);
	obj->PlayAnim(0, FALSE);
	if (!obj->mShown) {
		obj->Show();
	}
}

/**
 * @note Address: 0x800D6344
 * @note Size: 0xD4
 */
void fn_800D6344(XObjRadar* obj)
{
	obj->PlayAnim(2, FALSE);
	obj->mState = 2;
	StopSound(0x18C);
	StopSound(0x18D);
	StopSound(0x18E);
}

/**
 * @note Address: 0x800D6340
 * @note Size: 0x4
 */
void fn_800D6340(XObjRadar* obj)
{
}

/**
 * @note Address: 0x800D6F98
 * @note Size: 0x50
 */
void fn_800D6F98(XObjRadar* obj, u32 id)
{
	fn_800929C4(obj, id);
	obj->vf94();
	obj->_98C = obj->_990 = NAN;
}

/**
 * @note Address: 0x800D6FE8
 * @note Size: 0x398
 */
void fn_800D6FE8(XObjPopper* obj, u32 frames)
{
	if (obj->mShown) {
		CopyOwnerMtx(obj);
		HSD_AObj* aobj = GetAObj(obj->mJObj);
		s32 anim       = obj->mAnimId;
		f32 frame      = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		switch (anim) {
		case 1:
			if (aobj->flags & 0x44000000) {
				obj->PlayAnim(0, FALSE);
			}
			break;
		case 2:
			if (aobj->flags & 0x44000000) {
				obj->PlayAnim(0, FALSE);
				obj->Hide();
			}
			break;
		case 3:
			if (aobj->flags & 0x44000000) {
				obj->PlayAnim(4, FALSE);
			}
			break;
		case 4:
			break;
		case 5:
			if (aobj->flags & 0x44000000) {
				obj->PlayAnim(4, FALSE);
			}
			break;
		case 6:
			if (aobj->flags & 0x44000000) {
				obj->PlayAnim(0, FALSE);
			}
			break;
		}
		fn_80090ED4(obj, frames);
	}
}

/**
 * @note Address: 0x800D7380
 * @note Size: 0xCC
 */
void fn_800D7380(XObjPopper* obj)
{
	obj->PlayAnim(2, TRUE);
	obj->UpdateAnim(0);
	obj->PlayAnim(4, FALSE);
	if (!obj->mShown) {
		obj->Show();
	}
}

/**
 * @note Address: 0x800D744C
 * @note Size: 0x50
 */
void fn_800D744C(XObjPopper* obj)
{
	obj->PlayAnim(6, FALSE);
}

/**
 * @note Address: 0x800D749C
 * @note Size: 0x14
 */
BOOL fn_800D749C(XObjPopper* obj)
{
	return obj->mAnimId == 5;
}

/**
 * @note Address: 0x800D74B0
 * @note Size: 0x50
 */
void fn_800D74B0(XObjPopper* obj)
{
	obj->PlayAnim(5, FALSE);
}

/**
 * @note Address: 0x800D7500
 * @note Size: 0x50
 */
void fn_800D7500(XObjPopper* obj)
{
	obj->PlayAnim(3, FALSE);
}

/**
 * @note Address: 0x800D7550
 * @note Size: 0xA8
 */
void fn_800D7550(XObjPopper* obj)
{
	obj->PlayAnim(2, FALSE);
	StopSound(0x186);
	StopSound(0x18B);
}

/**
 * @note Address: 0x800D75F8
 * @note Size: 0x70
 */
void fn_800D75F8(XObjPopper* obj)
{
	obj->Show();
	obj->PlayAnim(1, FALSE);
}

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

/**
 * @note Address: 0x800D79AC
 * @note Size: 0xCC
 */
void fn_800D79AC(XObjCopter* obj)
{
	obj->PlayAnim(8, TRUE);
	obj->UpdateAnim(0);
	obj->PlayAnim(0, FALSE);
	if (!obj->mShown) {
		obj->Show();
	}
}

/**
 * @note Address: 0x800D7A78
 * @note Size: 0x50
 */
void fn_800D7A78(XObjCopter* obj)
{
	obj->PlayAnim(7, FALSE);
}

/**
 * @note Address: 0x800D7AC8
 * @note Size: 0x50
 */
void fn_800D7AC8(XObjCopter* obj)
{
	obj->PlayAnim(3, FALSE);
}

/**
 * @note Address: 0x800D7B18
 * @note Size: 0x50
 */
void fn_800D7B18(XObjCopter* obj)
{
	obj->PlayAnim(2, FALSE);
}

/**
 * @note Address: 0x800D7B68
 * @note Size: 0x88
 */
void fn_800D7B68(XObjCopter* obj)
{
	obj->PlayAnim(8, FALSE);
	StopSound(8);
}

/**
 * @note Address: 0x800D7BF0
 * @note Size: 0x70
 */
void fn_800D7BF0(XObjCopter* obj)
{
	obj->Show();
	obj->PlayAnim(5, FALSE);
}

/**
 * @note Address: 0x800D7C60
 * @note Size: 0x40
 */
void fn_800D7C60(XObjAttachment* obj)
{
	obj->Hide();
	fn_800911C0(obj);
}

/**
 * @note Address: 0x800D7CA0
 * @note Size: 0x20
 */
void fn_800D7CA0(XObjAttachment* obj)
{
	fn_800928E8(obj);
}

/**
 * @note Address: 0x800D7CC0
 * @note Size: 0x8
 */
void fn_800D7CC0(XObjAttachment* obj, HSD_JObj* owner)
{
	obj->mOwner = owner;
}

/**
 * @note Address: 0x800D7CC8
 * @note Size: 0x34
 */
void fn_800D7CC8(XObjAttachment* obj)
{
	obj->mOwner = NULL;
	obj->Hide();
}

/**
 * @note Address: 0x800D7CFC
 * @note Size: 0x40
 */
void fn_800D7CFC(XObjAttachment* obj, u32 id)
{
	fn_800929C4(obj, id);
	obj->vf94();
}

} // extern "C"
