#include "game/title.h"

// todo: cleanup
static inline CJArchive* ExtraArchive(XObj* object)
{
	return &object->mAnim;
}

// todo: cleanup
static inline s32 AnimationGroup(XObj* object, s32 id)
{
	bool extra = id >= 1000;
	if (extra) {
		extra = ExtraArchive(object)->mArchive != NULL;
	}
	return extra ? id / 1000 : 0;
}

/**
 * @note Address: 0x800256E4
 * @note Size: 0x38
 */
// Cursor's SetAnim, keeps the classical scale
extern "C" void fn_800256E4(Cursor* object, s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate,
                            f32 blend, f32 blendRate)
{
	fn_800902A4(object, id, force, jointAnim, matAnim, shapeAnim, frame, rate, blend, blendRate);
	HSD_JObjSetFlags(object->mJObj, CLASSICAL_SCALE);
}

/**
 * @note Address: 0x8002571C
 * @note Size: 0x4
 */
// XObj's vf94
extern "C" void fn_8002571C()
{
}

/**
 * @note Address: 0x80025720
 * @note Size: 0x24
 */
// XObj's vfA0
extern "C" BOOL fn_80025720(XObj* object, const char* path)
{
	return fn_8008A38C(ExtraArchive(object), path);
}

/**
 * @note Address: 0x80025744
 * @note Size: 0x4
 */
// XObj's vfCC
extern "C" void fn_80025744()
{
}

/**
 * @note Address: 0x80025748
 * @note Size: 0x1C
 */
// XObj's GetCameraTarget
extern "C" Vec fn_80025748(CXObj* object)
{
	return object->mMoveTranslate;
}

/**
 * @note Address: 0x80025764
 * @note Size: 0xF4
 */
// XObj's vfF0
extern "C" HSD_AnimJoint* fn_80025764(XObj* object, s32 id)
{
	if (AnimationGroup(object, id)) {
		HSD_AnimJoint** animations = ExtraArchive(object)->mAnimJoint;
		return animations != NULL ? animations[(id % 1000) % 1000] : NULL;
	}
	HSD_AnimJoint** animations = object->mArchive.mAnimJoint;
	return animations != NULL ? animations[id % 1000] : NULL;
}

/**
 * @note Address: 0x80025858
 * @note Size: 0xF4
 */
// XObj's vfF4
extern "C" HSD_MatAnimJoint* fn_80025858(XObj* object, s32 id)
{
	if (AnimationGroup(object, id)) {
		HSD_MatAnimJoint** animations = ExtraArchive(object)->mMatAnimJoint;
		return animations != NULL ? animations[(id % 1000) % 1000] : NULL;
	}
	HSD_MatAnimJoint** animations = object->mArchive.mMatAnimJoint;
	return animations != NULL ? animations[id % 1000] : NULL;
}

/**
 * @note Address: 0x8002594C
 * @note Size: 0xF4
 */
// XObj's vfF8
extern "C" HSD_ShapeAnimJoint* fn_8002594C(XObj* object, s32 id)
{
	if (AnimationGroup(object, id)) {
		HSD_ShapeAnimJoint** animations = ExtraArchive(object)->mShapeAnimJoint;
		return animations != NULL ? animations[(id % 1000) % 1000] : NULL;
	}
	HSD_ShapeAnimJoint** animations = object->mArchive.mShapeAnimJoint;
	return animations != NULL ? animations[id % 1000] : NULL;
}

/**
 * @note Address: 0x80025A40
 * @note Size: 0x8
 */
// XObj's vf100
extern "C" s32 fn_80025A40(void* object, s32 unused, s32 value)
{
	return value;
}

/**
 * @note Address: 0x80025A48
 * @note Size: 0x50
 */
// XObj's vf124
extern "C" s32 fn_80025A48(void* object, s32 count)
{
	// retail does this
	for (s32 i = 0; i < count; ++i) { }
	return 0;
}

/**
 * @note Address: 0x80025A98
 * @note Size: 0x8
 */
// XObj's vf128
extern "C" s32 fn_80025A98(XObj* object)
{
	return object->_80C;
}

/**
 * @note Address: 0x80025AA0
 * @note Size: 0x4
 */
// XObj's vf130
extern "C" void fn_80025AA0()
{
}

/**
 * @note Address: 0x80025AA4
 * @note Size: 0xD8
 */
// CXObj's SetTranslateX
extern "C" void fn_80025AA4(CXObj* object, f32 x, s32 force)
{
	object->mMoveTranslate.x         = x;
	object->mMoveTranslate.mTarget.x = x;
	if (object->mJObj != NULL) {
		object->CJObj::SetTranslateX(x, force);
	}
}

/**
 * @note Address: 0x80025B7C
 * @note Size: 0xD8
 */
// CXObj's SetTranslateY
extern "C" void fn_80025B7C(CXObj* object, f32 y, s32 force)
{
	object->mMoveTranslate.y         = y;
	object->mMoveTranslate.mTarget.y = y;
	if (object->mJObj != NULL) {
		object->CJObj::SetTranslateY(y, force);
	}
}

/**
 * @note Address: 0x80025C54
 * @note Size: 0xD8
 */
// CXObj's SetTranslateZ
extern "C" void fn_80025C54(CXObj* object, f32 z, s32 force)
{
	object->mMoveTranslate.z         = z;
	object->mMoveTranslate.mTarget.z = z;
	if (object->mJObj != NULL) {
		object->CJObj::SetTranslateZ(z, force);
	}
}

/**
 * @note Address: 0x80025D2C
 * @note Size: 0xD8
 */
// CXObj's SetScaleX
extern "C" void fn_80025D2C(CXObj* object, f32 x, s32 force)
{
	object->mMoveScale.x         = x;
	object->mMoveScale.mTarget.x = x;
	if (object->mJObj != NULL) {
		object->CJObj::SetScaleX(x, force);
	}
}

/**
 * @note Address: 0x80025E04
 * @note Size: 0xD8
 */
// CXObj's SetScaleZ
extern "C" void fn_80025E04(CXObj* object, f32 z, s32 force)
{
	object->mMoveScale.z         = z;
	object->mMoveScale.mTarget.z = z;
	if (object->mJObj != NULL) {
		object->CJObj::SetScaleZ(z, force);
	}
}
