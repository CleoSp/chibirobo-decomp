#include "game/xchr.h"
#include "game/video.h"

// joints that follow an item's own anim
struct XObjChrItemAnim {
	s32 mItem;     // _00, -1 ends the table
	s32 mDepth;    // _04
	s32 mPath[17]; // _08
};
STATIC_ASSERT(sizeof(XObjChrItemAnim) == 0x4C);

extern "C" {
extern XObjChrItemAnim lbl_80226130[];
}

// XObjChr::SetAnimRate, 1.135 times faster in lift state 4, and pushed onto the item's joints too
/**
 * @note Address: 0x800A36E8
 * @note Size: 0x1A4
 */
extern "C" void fn_800A36E8(XObjChr* chr, f32 rate, u32 mask)
{
	f32 multiplier;
	if (chr->mLift.mState == 4 && fn_800D4FE8(&chr->mLift)) {
		multiplier = 1.135f;
	} else {
		multiplier = 1.0f;
	}
#if defined(VERSION_PAL)
	multiplier *= 60.0f / (f32)lbl_80660D4C;
#endif
	chr->CXObj::SetAnimRate(rate * multiplier, mask);
	for (XObjChrItemAnim* entry = lbl_80226130; entry->mItem >= 0; ++entry) {
		if (entry->mItem == chr->mSuit) {
			HSD_JObj* joint = GetJointByPath(chr->mJObj, entry->mPath, entry->mDepth);
			if (GetAObj(joint) != NULL)
				chr->Push(joint, 0x200, 1.0f);
		}
	}
}

// todo: cleanup
static inline s32 SuitAnimGroup(XObjChr* chr, s32 id)
{
	bool extra = (id >= 3000 && chr->mSuitAnim.mArchive != NULL) ? true : false;
	return extra ? id / 3000 : 0;
}

// todo: cleanup
static inline s32 AtcAnimGroup(XObjChr* chr, s32 id)
{
	bool extra = (id >= 2000 && chr->mAttachmentAnim.mArchive != NULL) ? true : false;
	return extra ? id / 2000 : 0;
}

// todo: cleanup
static inline s32 AnimGroup(XObjChr* chr, s32 id)
{
	bool extra = id >= 1000;
	if (extra)
		extra = chr->mAnim.mArchive != NULL;
	return extra ? id / 1000 : 0;
}

// XObjChr::vfF8, the shape anim for an id, 3000 up from the suit, 2000 up from the attachment,
// 1000 up from XObj's second archive
/**
 * @note Address: 0x800A388C
 * @note Size: 0x254
 */
extern "C" HSD_ShapeAnimJoint* fn_800A388C(XObjChr* chr, s32 id)
{
	if (SuitAnimGroup(chr, id)) {
		HSD_ShapeAnimJoint** anims = chr->mSuitAnim.mShapeAnimJoint;
		return anims != NULL ? anims[(id % 3000) % 1000] : NULL;
	}
	if (AtcAnimGroup(chr, id)) {
		HSD_ShapeAnimJoint** anims = chr->mAttachmentAnim.mShapeAnimJoint;
		return anims != NULL ? anims[(id % 2000) % 1000] : NULL;
	}
	if (AnimGroup(chr, id)) {
		HSD_ShapeAnimJoint** anims = chr->mAnim.mShapeAnimJoint;
		return anims != NULL ? anims[(id % 1000) % 1000] : NULL;
	}
	HSD_ShapeAnimJoint** anims = chr->mArchive.mShapeAnimJoint;
	return anims != NULL ? anims[id % 1000] : NULL;
}

// XObjChr::vfF4, the material anim
/**
 * @note Address: 0x800A3AE0
 * @note Size: 0x254
 */
extern "C" HSD_MatAnimJoint* fn_800A3AE0(XObjChr* chr, s32 id)
{
	if (SuitAnimGroup(chr, id)) {
		HSD_MatAnimJoint** anims = chr->mSuitAnim.mMatAnimJoint;
		return anims != NULL ? anims[(id % 3000) % 1000] : NULL;
	}
	if (AtcAnimGroup(chr, id)) {
		HSD_MatAnimJoint** anims = chr->mAttachmentAnim.mMatAnimJoint;
		return anims != NULL ? anims[(id % 2000) % 1000] : NULL;
	}
	if (AnimGroup(chr, id)) {
		HSD_MatAnimJoint** anims = chr->mAnim.mMatAnimJoint;
		return anims != NULL ? anims[(id % 1000) % 1000] : NULL;
	}
	HSD_MatAnimJoint** anims = chr->mArchive.mMatAnimJoint;
	return anims != NULL ? anims[id % 1000] : NULL;
}

// XObjChr::vfF0, the joint anim
/**
 * @note Address: 0x800A3D34
 * @note Size: 0x254
 */
extern "C" HSD_AnimJoint* fn_800A3D34(XObjChr* chr, s32 id)
{
	if (SuitAnimGroup(chr, id)) {
		HSD_AnimJoint** anims = chr->mSuitAnim.mAnimJoint;
		return anims != NULL ? anims[(id % 3000) % 1000] : NULL;
	}
	if (AtcAnimGroup(chr, id)) {
		HSD_AnimJoint** anims = chr->mAttachmentAnim.mAnimJoint;
		return anims != NULL ? anims[(id % 2000) % 1000] : NULL;
	}
	if (AnimGroup(chr, id)) {
		HSD_AnimJoint** anims = chr->mAnim.mAnimJoint;
		return anims != NULL ? anims[(id % 1000) % 1000] : NULL;
	}
	HSD_AnimJoint** anims = chr->mArchive.mAnimJoint;
	return anims != NULL ? anims[id % 1000] : NULL;
}
