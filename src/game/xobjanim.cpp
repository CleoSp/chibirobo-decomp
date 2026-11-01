#include "game/xobjs.h"
#include "game/video.h"

extern "C" void C_VECReflect(const Vec* source, const Vec* normal, Vec* result);

#if defined(VERSION_PAL)
#define ANIMATION_RATE(rate) ((rate) * (60.0f / (f32)lbl_80660D4C))
#else
#define ANIMATION_RATE(rate) (rate)
#endif

static inline CJArchive* ExtraArchive(XObj* object)
{
	return &object->mAnim;
}

static inline s32 AnimationGroup(XObj* object, s32 id)
{
	bool extra = (id >= 1000 && ExtraArchive(object)->mArchive != NULL) ? true : false;
	return extra ? id / 1000 : 0;
}

/**
 * @note Address: 0x800901E8
 * @note Size: 0x6C
 */
extern "C" CVec fn_800901E8(const Vec* source, const Vec* normal)
{
	CVec result;
	C_VECReflect(source, normal, &result);
	return result;
}

/**
 * @note Address: 0x80090254
 * @note Size: 0x50
 */
extern "C" void fn_80090254(XObj* object)
{
	object->CXObj::ClearAnim();
	object->mStatus.mFlags &= ~0x0008000000000000ULL;
}

/**
 * @note Address: 0x800902A4
 * @note Size: 0x1D8
 */
extern "C" void fn_800902A4(XObj* object, s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate,
                            f32 blend, f32 blendRate)
{
	if (object->mStatus.mFlags & 0x0020000000000000ULL)
		blend = 1.0f;
	if (AnimationGroup(object, id)) {
		s32 index = id % 1000;
		if (jointAnim == NULL) {
			HSD_AnimJoint** animations = ExtraArchive(object)->mAnimJoint;
			jointAnim                  = animations != NULL ? animations[index % 1000] : NULL;
		}
		if (matAnim == NULL) {
			HSD_MatAnimJoint** animations = ExtraArchive(object)->mMatAnimJoint;
			matAnim                       = animations != NULL ? animations[index % 1000] : NULL;
		}
		if (shapeAnim == NULL) {
			HSD_ShapeAnimJoint** animations = ExtraArchive(object)->mShapeAnimJoint;
			shapeAnim                       = animations != NULL ? animations[index % 1000] : NULL;
		}
		object->CXObjIntp::SetAnim(id, force, jointAnim, matAnim, shapeAnim, frame, ANIMATION_RATE(rate), blend, blendRate);
		object->mStatus.mFlags |= 0x0008000000000000ULL;
	} else {
		object->CXObjIntp::SetAnim(id, force, jointAnim, matAnim, shapeAnim, frame, ANIMATION_RATE(rate), blend, blendRate);
		object->mStatus.mFlags &= ~0x0008000000000000ULL;
	}
}

/**
 * @note Address: 0x8009047C
 * @note Size: 0xEC
 */
extern "C" void fn_8009047C(XObj* object)
{
	if ((object->mStatus.mFlags & 0x800ULL) && (object->mStatus.mFlags & 0x10000ULL)) {
		HSD_JObjSetFlags(object->mJObj, CLASSICAL_SCALE);
		Vec scale = object->mMoveScale;
		object->SetScale(scale.x, -scale.y, scale.z, 0);
		if (object->mJObj != NULL)
			HSD_JObjDispAll(object->mJObj, NULL, 7, 0);
		object->SetScale(scale, 0);
	}
}

/**
 * @note Address: 0x80090568
 * @note Size: 0x4
 */
extern "C" void fn_80090568()
{
}
