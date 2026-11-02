#include "game/xobj.h"
#include "game/cjobjset.h"
#include "game/xobjs.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/cobj.h"
#include "game/cfile.h"
#include "game/ccamera.h"
#include "game/ccameraaction.h"
#include "game/hsd_jobj.h"
#include "game/hsd_dobj.h"
#include "game/globals.h"
#include "game/evt.h"
#include "game/xobjchr.h"
#include "game/video.h"
#include "game/ut.h"
#include "Version.h"
#include <math.h>
extern "C" {
#include <dolphin/mtx/mtx.h>
#include <sysdolphin/mtx.h>
#include <sysdolphin/class_alloc.h>
#include "game/gmath.h"
}

extern "C" {
void HSD_JObjModifyFlags(HSD_JObj* jobj, u32 mask, u32 flags);
void HSD_DObjModifyFlags(HSD_DObj* dobj, u32 mask, u32 flags);
void HSD_MObjSetAlpha(HSD_MObj* mobj, f32 alpha);
void JObjSetPObjFlags(HSD_JObj* jobj, void* flags, u32 type);
void JObjClearPObjFlags(HSD_JObj* jobj, void* flags, u32 type);
}

// XObj, the CXObjIntp every placed object in the house is built on

extern "C" {
CObjArchive* fn_80132620(CObjArchive* archives);
void PSMTXRotRad(Mtx matrix, char axis, f32 angle);
void GXProject(f32 x, f32 y, f32 z, const Mtx view, const f32* projection, const f32* viewport, f32* screenX, f32* screenY, f32* screenZ);
void C_VECReflect(const Vec* source, const Vec* normal, Vec* result);
void fn_8007F194(void* event, void* context, void* owner);
}

#if defined(VERSION_PAL)
#define ANIMATION_RATE(rate) ((rate) * (60.0f / (f32)lbl_80660D4C))
#else
#define ANIMATION_RATE(rate) (rate)
#endif

// the pe presets sit 0x5C into this unit's rodata, which isn't split out yet
struct XObjPEPresets {
	u8 _00[0x5C];          // _00
	HSD_PEDesc mReset;     // _5C
	HSD_PEDesc mPreset[3]; // _68
};

extern "C" {
extern const XObjPEPresets lbl_802091E8;
}

static inline const void* ObjectScript(CXObj* object)
{
	return ((XObj*)object)->vf128();
}

static inline void TriggerObjectEvent(CXObj* object, u32 first, u32 second)
{
	const void* script = ObjectScript(object);
	if (!fn_8009FDE4(lbl_8025C400.GetPlayer())) {
		fn_8007EEA4(&lbl_8025C400.mEvt, (u8*)script, (u32)object, first, second);
	}
}

static inline u32 ShadowSize(XObj* object)
{
	return object->mAttr._00 != NULL ? object->mAttr._00->_0C : 0;
}

static inline void StepColor(f32& current, f32& target, f32& speed, u32 frames)
{
	if (current != target) {
		current += speed * frames;
		if (speed > 0.0f && current >= target) {
			current = target;
			speed   = 0.0f;
		}
		if (speed < 0.0f && current <= target) {
			current = target;
			speed   = 0.0f;
		}
	}
}

/**
 * @note Address: 0x8008A360
 * @note Size: 0x2C
 */
// CJAnim::Release
extern "C" void fn_8008A360(CJAnim* anim)
{
	if (anim->mArchive != NULL)
		anim->CJArchive::Release();
}

/**
 * @note Address: 0x8008A38C
 * @note Size: 0x170
 */
// starts the read, then sets it up once it's in, TRUE when done
extern "C" BOOL fn_8008A38C(CJAnim* anim, const char* path)
{
	if (anim->mLoading == 0) {
		anim->Release();
		if (path == NULL || *path == '\0')
			return TRUE;
		CFile file;
		if (file.ConvertPathToEntrynum(path) < 0)
			return TRUE;
		CObjArchive* storage = fn_80132620(lbl_8025C400.mObjArchives.mArchives);
		if (storage != NULL) {
			anim->ReadAsync(storage, path, 0, 0, 0);
			anim->mLoading++;
		} else {
			return TRUE;
		}
	} else {
		if (anim->ReadCheck()) {
			anim->SetupScene(0, 0);
			anim->mLoading = 0;
		}
	}
	return anim->mLoading == 0;
}

/**
 * @note Address: 0x8008A4FC
 * @note Size: 0x34
 */
// CJAnim::Clear
extern "C" void fn_8008A4FC(CJAnim* anim)
{
	anim->CJArchive::Clear();
	anim->mLoading = 0;
}

/**
 * @note Address: 0x8008A530
 * @note Size: 0xD4
 */
extern "C" BOOL fn_8008A530(XObj* object, s32 unused, s32 value)
{
	if (object->mStatus.mFlags & 0x0000040000000000ULL) {
		if (value == 56)
			return FALSE;
		const void* script = ObjectScript(object);
		if (!fn_8009FDE4(lbl_8025C400.GetPlayer())) {
			fn_8007EEA4(&lbl_8025C400.mEvt, (u8*)script, (u32)object, (s16)value, -1);
		}
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x8008A604
 * @note Size: 0xBC
 */
extern "C" void fn_8008A604(XObj* object, s32 unused, s32 value)
{
	if (object->mStatus.mFlags & 0x0000080000000000ULL) {
		const void* script = ObjectScript(object);
		if (!fn_8009FDE4(lbl_8025C400.GetPlayer())) {
			fn_8007EEA4(&lbl_8025C400.mEvt, (u8*)script, (u32)object, -1, (s16)((u32)value + 3000));
		}
	}
}

/**
 * @note Address: 0x8008A6C0
 * @note Size: 0x158
 */
extern "C" void fn_8008A6C0(XObj* object, const Vec* position, f32 strength)
{
	if (object->mStatus.mFlags & 0x0000080000000000ULL) {
		if (strength >= (s32)fn_8006EBC8(31) / 100.0f) {
			TriggerObjectEvent(object, -1, 2001);
		} else {
			TriggerObjectEvent(object, -1, 2000);
		}
	}
}

/**
 * @note Address: 0x8008A818
 * @note Size: 0x8
 */
extern "C" BOOL fn_8008A818(void* object)
{
	return TRUE;
}

/**
 * @note Address: 0x8008A820
 * @note Size: 0x124
 */
extern "C" void fn_8008A820(XObj* object, s32 index, u8 red, u8 green, u8 blue, f32 time)
{
	object->mColorStep[index].mTarget.x = red;
	object->mColorStep[index].mTarget.y = green;
	object->mColorStep[index].mTarget.z = blue;
	if (time == 0.0f) {
		object->mColorStep[index].mValue.x = red;
		object->mColorStep[index].mValue.y = green;
		object->mColorStep[index].mValue.z = blue;
		object->mColorStep[index].mSpeed.x = object->mColorStep[index].mSpeed.y = object->mColorStep[index].mSpeed.z = 0.0f;
	} else {
#if defined(VERSION_PAL)
		// retail reads the refresh rate for each one
		object->mColorStep[index].mSpeed.x = ((f32)red - object->mColorStep[index].mValue.x) / (lbl_80660D4C * time);
		object->mColorStep[index].mSpeed.y = ((f32)green - object->mColorStep[index].mValue.y) / (lbl_80660D4C * time);
		object->mColorStep[index].mSpeed.z = ((f32)blue - object->mColorStep[index].mValue.z) / (lbl_80660D4C * time);
#else
		object->mColorStep[index].mSpeed.x = ((f32)red - object->mColorStep[index].mValue.x) / (60.0f * time);
		object->mColorStep[index].mSpeed.y = ((f32)green - object->mColorStep[index].mValue.y) / (60.0f * time);
		object->mColorStep[index].mSpeed.z = ((f32)blue - object->mColorStep[index].mValue.z) / (60.0f * time);
#endif
	}
}

/**
 * @note Address: 0x8008ADA0
 * @note Size: 0x338
 */
extern "C" BOOL fn_8008ADA0(XObj* object, const Vec* direction, u8 axes, f32 x, f32 z)
{
	BOOL changed = FALSE;
	if (__fpclassifyf(object->mAnchorX) == FP_NAN || __fpclassifyf(object->mAnchorZ) == FP_NAN) {
		object->SetTranslateX(x, 0);
		object->SetTranslateZ(z, 0);
	} else if (object->mAnchorX == x && object->mAnchorZ == z) {
		object->SetTranslateX(x, 0);
		object->SetTranslateZ(z, 0);
	} else {
		Mtx rotation;
		Vec displacement;
		PSMTXRotRad(rotation, 'y', -object->mMoveRotate.y);
		displacement.x = object->mAnchorX - x;
		displacement.z = object->mAnchorZ - z;
		displacement.y = 0.0f;
		PSMTXMultVec(rotation, &displacement, &displacement);

		if (axes & 1) {
			if (direction->x > 0.0f && displacement.x < object->mLimitPosX) {
				displacement.x = object->mLimitPosX;
				changed        = TRUE;
			} else if (direction->x < 0.0f && displacement.x > object->mLimitNegX) {
				displacement.x = object->mLimitNegX;
				changed        = TRUE;
			}
		}
		if (axes & 2) {
			if (direction->z > 0.0f && displacement.z < object->mLimitPosZ) {
				displacement.z = object->mLimitPosZ;
				changed        = TRUE;
			} else if (direction->z < 0.0f && displacement.z > object->mLimitNegZ) {
				displacement.z = object->mLimitNegZ;
				changed        = TRUE;
			}
		}

		PSMTXRotRad(rotation, 'y', object->mMoveRotate.y);
		PSMTXMultVec(rotation, &displacement, &displacement);
		object->SetTranslateX(object->mAnchorX - displacement.x, 0);
		object->SetTranslateZ(object->mAnchorZ - displacement.z, 0);
	}
	return changed;
}

/**
 * @note Address: 0x8008B0D8
 * @note Size: 0x14C
 */
extern "C" void fn_8008B0D8(CJObj* object, s32 mode)
{
	switch (mode) {
	case 0:
		if (object->mJObj != NULL)
			HSD_JObjWalkTree(object->mJObj, (HSD_JObjWalkTreeCallback)JObjClearPObjFlags, (f32**)0x4000);
		if (object->mJObj != NULL)
			HSD_JObjWalkTree(object->mJObj, (HSD_JObjWalkTreeCallback)JObjClearPObjFlags, (f32**)0x8000);
		break;
	case 2:
		if (object->mJObj != NULL)
			HSD_JObjWalkTree(object->mJObj, (HSD_JObjWalkTreeCallback)JObjClearPObjFlags, (f32**)0x4000);
		if (object->mJObj != NULL)
			HSD_JObjWalkTree(object->mJObj, (HSD_JObjWalkTreeCallback)JObjSetPObjFlags, (f32**)0x8000);
		break;
	case 1:
		if (object->mJObj != NULL)
			HSD_JObjWalkTree(object->mJObj, (HSD_JObjWalkTreeCallback)JObjSetPObjFlags, (f32**)0x4000);
		if (object->mJObj != NULL)
			HSD_JObjWalkTree(object->mJObj, (HSD_JObjWalkTreeCallback)JObjClearPObjFlags, (f32**)0x8000);
		break;
	case 3:
		if (object->mJObj != NULL)
			HSD_JObjWalkTree(object->mJObj, (HSD_JObjWalkTreeCallback)JObjSetPObjFlags, (f32**)0x4000);
		if (object->mJObj != NULL)
			HSD_JObjWalkTree(object->mJObj, (HSD_JObjWalkTreeCallback)JObjSetPObjFlags, (f32**)0x8000);
		break;
	}
}

/**
 * @note Address: 0x8008B224
 * @note Size: 0x114
 */
extern "C" BOOL fn_8008B224(const XObj* object, CCamera* camera)
{
	HSD_JObj* joint = object->mJObj;
	if (joint != NULL) {
		Vec position = GetJointPosition(joint);
		f32 screenX, screenY, screenZ;
		GXProject(position.x, position.y, position.z, camera->mViewMtx, camera->mProjection, camera->mViewport, &screenX, &screenY,
		          &screenZ);
		if (screenZ > 0.0f && screenZ < 1.0f)
			return FALSE;
		return TRUE;
	}
	return TRUE;
}

/**
 * @note Address: 0x8008B338
 * @note Size: 0x29C
 */
extern "C" BOOL fn_8008B338(CXObj* object, f32* width, f32* height, CCamera* camera, f32 radius, f32 objectHeight)
{
	camera          = camera != NULL ? camera : &lbl_8025C400.mMotionControlCamera.mCamera;
	f32 heading     = camera->mAngle.y;
	MtxPtr view     = camera->mViewMtx;
	f32* projection = camera->mProjection;
	f32* viewport   = camera->mViewport;
	f32 left, right, bottom, top, unusedX, unusedY, depth;
	f32 angle  = (PI + HALF_PI) + heading;
	f32 cosine = (f32)cos(angle);
	f32 sine   = (f32)sin(angle);
	GXProject(radius * sine + object->mMoveTranslate.x, object->mMoveTranslate.y, radius * cosine + object->mMoveTranslate.z, view,
	          projection, viewport, &left, &unusedY, &depth);
	angle  = HALF_PI + heading;
	cosine = (f32)cos(angle);
	sine   = (f32)sin(angle);
	GXProject(radius * sine + object->mMoveTranslate.x, object->mMoveTranslate.y, radius * cosine + object->mMoveTranslate.z, view,
	          projection, viewport, &right, &unusedY, &depth);
	angle  = PI + heading;
	cosine = (f32)cos(angle);
	sine   = (f32)sin(angle);
	GXProject(radius * sine + object->mMoveTranslate.x, object->mMoveTranslate.y, radius * cosine + object->mMoveTranslate.z, view,
	          projection, viewport, &unusedX, &bottom, &depth);
	cosine = (f32)cos(heading);
	sine   = (f32)sin(heading);
	GXProject(radius * sine + object->mMoveTranslate.x, object->mMoveTranslate.y + objectHeight, radius * cosine + object->mMoveTranslate.z,
	          view, projection, viewport, &unusedX, &top, &depth);
#if defined(VERSION_PAL)
	f32 scale = 448.0f / (&lbl_8025C400.mGs.mScreen)->xfbHeight;
	bottom *= scale;
	top *= scale;
#endif
	if (left >= 0.0f && right < 640.0f && bottom >= 0.0f && top < 448.0f && depth > 0.0f && depth < 1.0f) {
		if (width != NULL) {
			f32 value = right - left;
			if (value < 0.0f)
				value = -value;
			*width = value;
		}
		if (height != NULL) {
			f32 value = top - bottom;
			if (value < 0.0f)
				value = -value;
			*height = value;
		}
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x8008B5D4
 * @note Size: 0xCC
 */
extern "C" void fn_8008B5D4(CXObj* object, const Vec* offset, f32* x, f32* y, f32* z, CCamera* camera)
{
	camera = camera != NULL ? camera : &lbl_8025C400.mMotionControlCamera.mCamera;
	f32 screenX, screenY, screenZ;
	GXProject(object->mMoveTranslate.x + offset->x, object->mMoveTranslate.y + offset->y, object->mMoveTranslate.z + offset->z,
	          camera->mViewMtx, camera->mProjection, camera->mViewport, &screenX, &screenY, &screenZ);
	if (x != NULL)
		*x = screenX;
	if (y != NULL) {
#if defined(VERSION_PAL)
		*y = 448.0f * screenY / (&lbl_8025C400.mGs.mScreen)->xfbHeight;
#else
		*y = screenY;
#endif
	}
	if (z != NULL)
		*z = screenZ;
}

/**
 * @note Address: 0x8008C1F4
 * @note Size: 0x80
 */
CVec CVec::operator*(Vec other) const
{
	CVec result = *this;
	result.x *= other.x;
	result.y *= other.y;
	result.z *= other.z;
	return result;
}

/**
 * @note Address: 0x8008C7CC
 * @note Size: 0xD8
 */
extern "C" void fn_8008C7CC(XObj* object)
{
	object->_780 = NAN;
	for (s32 i = 0; object->mFloors[i].mId != -1; ++i) {
		if (object->_250[0] & (1U << i)) {
			if (__fpclassifyf(object->_780) == FP_NAN || object->_6FC[i] > object->_780) {
				object->_784 = i;
				object->_780 = object->_6FC[i];
			}
		}
	}
}

/**
 * @note Address: 0x8008DEC0
 * @note Size: 0x74
 */
extern "C" void fn_8008DEC0(XObj* object)
{
	if (object->mStatus.mFlags & 0x0200000000000000ULL) {
		object->_250[1] = object->_250[0];
		for (int i = 0; object->mFloors[i].mId != -1; i++) {
			if (object->IsFloorOn(i)) {
				object->ClearFloor(i);
			}
		}
	}
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
	if (object->GetAnimSet(id) != 0) {
		s32 index = id % 1000;
		jointAnim = jointAnim != NULL ? jointAnim : object->mAnim.GetAnimJoint(index);
		matAnim   = matAnim != NULL ? matAnim : object->mAnim.GetMatAnimJoint(index);
		shapeAnim = shapeAnim != NULL ? shapeAnim : object->mAnim.GetShapeAnimJoint(index);
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

/**
 * @note Address: 0x8009056C
 * @note Size: 0x2F8
 */
extern "C" void fn_8009056C(XObj* object, const Vec* origin, f32 opacity)
{
	if (!(object->mStatus.mFlags & 8ULL))
		return;
	if (ShadowSize(object) == 0)
		return;
	Vec position = *origin;
	Vec rotation = { -HALF_PI, 0.0f, 0.0f };
	Vec size     = object->mMoveScale * (f32)ShadowSize(object);
	fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 1, 4, 5, 15);
	u32 edge   = (u32)(128.0f * opacity);
	u32 center = (u32)(160.0f * opacity);
	fn_80198258(&lbl_8025C400.mGs, 12, center, edge, position.x, position.y, position.z, size.x, size.z, rotation.x, rotation.y,
	            rotation.z);
	PrimVertex vertices[26];
	f32 halfWidth      = size.x * 0.5f;
	f32 halfHeight     = size.z * 0.5f;
	PrimVertex* vertex = vertices;
	for (s32 i = 0; i < 13; ++i, vertex += 2) {
		f32 angle       = (PI / 6.0f) * i;
		vertex[0].pos.x = __fnmsubs(halfWidth, (f32)sin(angle), position.x);
		vertex[0].pos.z = __fmadds(halfHeight, (f32)cos(angle), position.z);
		vertex[0].pos.y = position.y;
		vertex[0].color = edge;
		// retail reads the scale again here
		f32 sine        = sin(angle);
		f32 outerWidth  = __fmadds(1.5f, object->mMoveScale.x, halfWidth);
		vertex[1].pos.x = __fnmsubs(outerWidth, sine, position.x);
		f32 cosine      = cos(angle);
		f32 outerHeight = __fmadds(1.5f, object->mMoveScale.z, halfHeight);
		vertex[1].pos.z = __fmadds(outerHeight, cosine, position.z);
		vertex[1].pos.y = position.y;
		vertex[1].color = 0;
	}
	fn_801988EC(&lbl_8025C400.mGs, GX_TRIANGLESTRIP, lbl_8025C400.mGs.mCamera->mViewMtx, vertices, 26);
}

/**
 * @note Address: 0x80090864
 * @note Size: 0x5C
 */
extern "C" void fn_80090864(XObj* object)
{
	if (object->mStatus.mFlags & 8ULL) {
		object->vfBC(&object->mMoveTranslate, 1.0f);
	}
}

/**
 * @note Address: 0x800908C0
 * @note Size: 0x3C4
 */
extern "C" void fn_800908C0(XObj* object, Mtx view)
{
	HSD_AObj* animation = GetAObj(object->mJObj);
	if (animation != NULL && (animation->flags & (AOBJ_NO_ANIM | AOBJ_REWINDED)) && object->HasQueuedAnim()) {
		s32 id;
		u16 frame;
		s16 rate;
		object->PopAnim(id, frame, rate);
		if (id & 0xFFFF0000) {
			s32 jointId             = id & 0xFFFF;
			s32 materialId          = id >> 16;
			void* jointAnimation    = object->vfF0(jointId);
			void* materialAnimation = object->vfF4(materialId);
			void* shapeAnimation    = object->vfF8(jointId);
			object->SetAnim(0, TRUE, jointAnimation, materialAnimation, shapeAnimation, (f32)frame, rate / 100.0f, 0.0f, 10.0f);
			if (jointId >= 1000 || materialId >= 1000)
				object->mStatus.mFlags |= 0x0008000000000000ULL;
		} else {
			object->SetAnim(id, FALSE, NULL, NULL, NULL, (f32)frame, rate / 100.0f, 0.0f, 10.0f);
		}
		object->UpdateAnim(1);
	}
	if (!(object->mStatus.mFlags & 0x800ULL)) {
		if (object->mJObj != NULL)
			HSD_JObjDispAll(object->mJObj, view, 7, 0);
	}
}

/**
 * @note Address: 0x80090C84
 * @note Size: 0x250
 */
extern "C" void fn_80090C84(XObj* object, u32 frames)
{
	if (object->mJObj != NULL) {
		for (s32 i = 0; i < 2; ++i) {
			XObjColorStep& color = object->mColorStep[i];
			StepColor(color.mValue.x, color.mTarget.x, color.mSpeed.x, frames);
			StepColor(color.mValue.y, color.mTarget.y, color.mSpeed.y, frames);
			StepColor(color.mValue.z, color.mTarget.z, color.mSpeed.z, frames);
		}
		object->mColors[1] = ((u32)object->mColorStep[0].mValue.x << 24) | ((u32)object->mColorStep[0].mValue.y << 16)
		                   | ((u32)object->mColorStep[0].mValue.z << 8);
		object->mColors[0] = ((u32)object->mColorStep[1].mValue.x << 24) | ((u32)object->mColorStep[1].mValue.y << 16)
		                   | ((u32)object->mColorStep[1].mValue.z << 8);
	}
}

/**
 * @note Address: 0x80090ED4
 * @note Size: 0x2EC
 */
extern "C" void fn_80090ED4(XObj* object, u32 frames)
{
	if (object->mJObj != NULL) {
		object->vfA8(frames);
		if (object->mStatus.mFlags & 0x1000000000000000ULL) {
			if (object->mMoveTranslate.y + object->_794.y > object->_790) {
				object->mMoveTranslate.mTarget.y = object->_790;
				f32 speed                        = 0.15f + object->mMoveTranslate.mSpeed.y;
				if (speed > 10.0f)
					speed = 10.0f;
				object->mMoveTranslate.mSpeed.y = speed;
			} else {
				object->mMoveTranslate.mSpeed.y = 0.0f;
				object->mMoveTranslate.x += object->_794.x;
				object->mMoveTranslate.z += object->_794.z;
				object->mMoveTranslate.y = object->_790;
				object->_794.x           = 0.0f;
				object->_794.y           = 0.0f;
				object->_794.z           = 0.0f;
				object->SetRotationX(0.0f, 0);
				object->SetRotationZ(0.0f, 0);
				object->mStatus.mFlags &= ~0x1000000000000000ULL;
				if ((s16)object->GetPool() == 21) {
					object->mStatus.mFlags |= 0x2000000000000000ULL;
				} else if ((s16)object->GetPool() == 22) {
					if (object->mStatus.mFlags & 0x04000000ULL)
						object->mStatus.mFlags |= 0x2000000000000000ULL;
				}
			}
		}
		object->CXObjIntp::StepRotate(frames);
		object->CXObjIntp::StepTranslate(frames);
		if (object->CXObjIntp::StepScale(frames)) {
			fn_8008C8A4((XObj*)object);
			fn_8008D220((XObj*)object);
		}
		object->UpdateAnim(frames);
		HSD_JObjSetupMatrix(object->mJObj);
	}
	if ((object->mStatus.mFlags & 0x0010000000000000ULL) || object->mMoveTranslate.y != object->_264.y) {
		fn_8008C8A4((XObj*)object);
	}
	if (object->mEvt.mData.mScript != NULL)
		CEvt_Update(&object->mEvt, frames);
}

/**
 * @note Address: 0x800911C0
 * @note Size: 0xF4
 */
extern "C" void fn_800911C0(XObj* object)
{
	CJArchive* archive = &object->mAnim;
	archive->Release();
	if (object->mEvt.mData.mScript != NULL)
		fn_8007F040(&object->mEvt, TRUE, FALSE);
	if (object->mStatus.mFlags & 1) {
		object->CXObjIntp::Release();
		XObjSource** slot = (XObjSource**)object->mAttr._00;
		if (slot != NULL && *slot != NULL) {
			XObjSource* source = *slot;
			if (source->mArchive != NULL && source->mArchive->mRefCount <= 0)
				source->mArchive = NULL;
		}
	}
	object->mAttr._00 = NULL;
	object->mAttr._04 = NULL;
	object->mFloors   = NULL;
	object->mStatus.mFlags &= ~0x10001ULL;
}

/**
 * @note Address: 0x800912B4
 * @note Size: 0xB8
 */
extern "C" void fn_800912B4(HSD_JObj* joint, f32** arguments, s32 type)
{
	HSD_JObjModifyFlags(joint, 0x40000, 0x1C0000);
	for (HSD_DObj* draw = HSD_JObjGetDObj(joint); draw != NULL; draw = HSD_DObjGetNext(draw)) {
		HSD_DObjModifyFlags(draw, 2, 0xE);
		HSD_MObj* material = HSD_DObjGetMObj(draw);
		if (material != NULL) {
			HSD_MObjClearFlags(material, 0x68000000);
			HSD_MObjSetFlags(material, 0x2000);
			HSD_MObjCompileTev(material);
		}
	}
}

/**
 * @note Address: 0x8009136C
 * @note Size: 0x100
 */
extern "C" void fn_8009136C(HSD_JObj* joint, f32** arguments, s32 type)
{
	CXObj* object = (CXObj*)arguments[0];
	f32 alpha     = *arguments[1];
	HSD_JObjModifyFlags(joint, 0x80000, 0x1C0000);
	for (HSD_DObj* draw = HSD_JObjGetDObj(joint); draw != NULL; draw = HSD_DObjGetNext(draw)) {
		HSD_DObjModifyFlags(draw, 4, 0xE);
		HSD_MObj* material = HSD_DObjGetMObj(draw);
		if (material != NULL) {
			HSD_MObjSetFlags(material, 0x60000000);
			HSD_MObjSetFlags(material, 0x2000);
			HSD_MObjCompileTev(material);
			if (material->mat != NULL)
				HSD_MObjSetAlpha(material, alpha * object->GetMObjAlpha(material));
		}
	}
}

/**
 * @note Address: 0x8009146C
 * @note Size: 0x60
 */
extern "C" void fn_8009146C(CXObj* object)
{
	void* arguments[1] = { NULL };
	arguments[0]       = object;
	if (object->mJObj != NULL)
		HSD_JObjWalkTree(object->mJObj, fn_800912B4, (f32**)arguments);
	HSD_JObjModifyFlags(object->mJObj, 0x10000000, 0x70000000);
}

/**
 * @note Address: 0x800914CC
 * @note Size: 0x74
 */
extern "C" void fn_800914CC(CXObj* object, f32 alpha)
{
	void* arguments[2] = { NULL, NULL };
	arguments[0]       = object;
	arguments[1]       = &alpha;
	if (object->mJObj != NULL)
		HSD_JObjWalkTree(object->mJObj, fn_8009136C, (f32**)arguments);
	HSD_JObjModifyFlags(object->mJObj, 0x20000000, 0x70000000);
}

/**
 * @note Address: 0x80091540
 * @note Size: 0x1EC
 */
extern "C" void fn_80091540(HSD_JObj* joint, f32** arguments, s32 type)
{
	// NONMATCHING: the flags are read again between the stores
	f32 factor = *arguments[1];
	HSD_JObjSetFlags(joint, 0x80);
	for (HSD_DObj* draw = HSD_JObjGetDObj(joint); draw != NULL; draw = HSD_DObjGetNext(draw)) {
		HSD_MObj* material = HSD_DObjGetMObj(draw);
		if (material != NULL) {
			HSD_MObjSetFlags(material, 4);
			if (material->mat != NULL && __fpclassifyf(factor) != FP_NAN) {
				material->mat->diffuse.r = (u8)(material->mat->diffuse.r * factor);
				material->mat->diffuse.g = (u8)(material->mat->diffuse.g * factor);
				material->mat->diffuse.b = (u8)(material->mat->diffuse.b * factor);
			}
			for (HSD_TObj* texture = HSD_MObjGetTObj(material); texture != NULL; texture = HSD_TObjGetNext(texture)) {
				texture->flags &= ~0x1F0;
				texture->flags |= 0x50;
				texture->flags &= ~0xF0000;
				texture->flags |= 0x70000;
			}
			HSD_MObjCompileTev(material);
		}
	}
}

/**
 * @note Address: 0x8009172C
 * @note Size: 0x58
 */
extern "C" void fn_8009172C(CXObj* object, f32 factor)
{
	void* arguments[2] = { NULL, NULL };
	arguments[0]       = object;
	arguments[1]       = &factor;
	if (object->mJObj != NULL)
		HSD_JObjWalkTree(object->mJObj, fn_80091540, (f32**)arguments);
}

/**
 * @note Address: 0x80091784
 * @note Size: 0xB0
 */
extern "C" void fn_80091784(HSD_JObj* joint, f32** arguments, s32 type)
{
	BOOL enabled = (BOOL)arguments[0];
	u8 alpha     = (u32)arguments[1];
	for (HSD_DObj* draw = HSD_JObjGetDObj(joint); draw != NULL; draw = HSD_DObjGetNext(draw)) {
		HSD_MObj* material = HSD_DObjGetMObj(draw);
		if (material != NULL && material->pe != NULL) {
			if (enabled)
				material->pe->flags |= 6;
			else
				material->pe->flags &= ~6;
			material->pe->dst_alpha = alpha;
		}
	}
}

/**
 * @note Address: 0x80091834
 * @note Size: 0x54
 */
extern "C" void fn_80091834(CXObj* object, BOOL enabled, u8 value)
{
	void* arguments[2] = { NULL, NULL };
	arguments[0]       = (void*)enabled;
	arguments[1]       = (void*)value;
	if (object->mJObj != NULL)
		HSD_JObjWalkTree(object->mJObj, fn_80091784, (f32**)arguments);
}

// joint walk callback for XObj::vfD8, the args are five on/off flags
/**
 * @note Address: 0x80091888
 * @note Size: 0x3BC
 */
extern "C" void fn_80091888(HSD_JObj* joint, f32** arguments, s32 type)
{
	const XObjPEPresets* presets = &lbl_802091E8;
	u32 reset                    = (u32)arguments[0];
	u32 firstPreset              = (u32)arguments[1];
	u32 secondPreset             = (u32)arguments[2];
	u32 thirdPreset              = (u32)arguments[3];
	u32 toon                     = (u32)arguments[4];
	for (HSD_DObj* draw = HSD_JObjGetDObj(joint); draw != NULL; draw = HSD_DObjGetNext(draw)) {
		HSD_MObj* material = HSD_DObjGetMObj(draw);
		if (material != NULL) {
			if (reset) {
				if (material->pe == NULL)
					material->pe = (HSD_PEDesc*)hsdAllocMemPiece(sizeof(HSD_PEDesc));
				HSD_PEDesc settings = presets->mReset;
				*material->pe       = settings;
				if (material->rendermode & RENDER_XLU)
					material->pe->type = 1;
				if (material->rendermode & RENDER_NO_ZUPDATE)
					material->pe->flags &= ~0x20;
				if (material->rendermode & RENDER_ZMODE_ALWAYS)
					material->pe->flags &= ~0x10;
			}
			if (firstPreset) {
				if (material->pe == NULL)
					material->pe = (HSD_PEDesc*)hsdAllocMemPiece(sizeof(HSD_PEDesc));
				HSD_PEDesc settings = presets->mPreset[0];
				*material->pe       = settings;
			} else if (secondPreset) {
				if (material->pe == NULL)
					material->pe = (HSD_PEDesc*)hsdAllocMemPiece(sizeof(HSD_PEDesc));
				HSD_PEDesc settings = presets->mPreset[1];
				*material->pe       = settings;
			} else if (thirdPreset) {
				if (material->pe == NULL)
					material->pe = (HSD_PEDesc*)hsdAllocMemPiece(sizeof(HSD_PEDesc));
				HSD_PEDesc settings = presets->mPreset[2];
				*material->pe       = settings;
				if (material->rendermode & RENDER_XLU)
					material->pe->type = 1;
			}
			if (toon) {
				if (!(fn_801B8E5C(material) & RENDER_TOON)) {
					HSD_MObjSetFlags(material, RENDER_TOON);
					HSD_MObjCompileTev(material);
				}
			} else {
				if (fn_801B8E5C(material) & RENDER_TOON) {
					HSD_MObjClearFlags(material, RENDER_TOON);
					HSD_MObjCompileTev(material);
				}
			}
		}
	}
}

// XObj::vfD8, the flags go to fn_80091888 on every joint
/**
 * @note Address: 0x80091C44
 * @note Size: 0x3C
 */
extern "C" void fn_80091C44(XObj* object, u32* flags)
{
	if (object->mJObj != NULL)
		HSD_JObjWalkTree(object->mJObj, fn_80091888, (f32**)flags);
}

// XObj::vf98
/**
 * @note Address: 0x800928E8
 * @note Size: 0xDC
 */
extern "C" void fn_800928E8(XObj* object)
{
	object->_80C   = 0;
	object->_788   = -1;
	object->_78C   = 0;
	object->_790   = 0.0f;
	object->_794.x = 0.0f;
	object->_794.y = 0.0f;
	object->_794.z = 0.0f;
	object->mAnmQueue.Clear();
	object->mAnchorX = object->mAnchorZ = NAN;
	object->_8B0                        = 0;
	object->_8B0 |= 0x80000000;
	object->mColorStep[0].mValue.x = object->mColorStep[0].mValue.y = object->mColorStep[0].mValue.z = 255.0f;
	object->mColorStep[0].mTarget.x = object->mColorStep[0].mTarget.y = object->mColorStep[0].mTarget.z = 255.0f;
	object->mColorStep[0].mSpeed.x = object->mColorStep[0].mSpeed.y = object->mColorStep[0].mSpeed.z = 0.0f;
	object->mColorStep[1].mValue.x = object->mColorStep[1].mValue.y = object->mColorStep[1].mValue.z = 0.0f;
	object->mColorStep[1].mTarget.x = object->mColorStep[1].mTarget.y = object->mColorStep[1].mTarget.z = 0.0f;
	object->mColorStep[1].mSpeed.x = object->mColorStep[1].mSpeed.y = object->mColorStep[1].mSpeed.z = 0.0f;
}

// XObj::vf90
/**
 * @note Address: 0x800929C4
 * @note Size: 0xE0
 */
extern "C" void fn_800929C4(XObj* object, u32 id)
{
	// NONMATCHING: the dol adds the event data's offset and its index apart
	object->CXObj::Init(id);
	object->mStatus.mFlags = 0;
	object->mCoord._00     = NULL;
	object->mAttr._00      = NULL;
	object->mAttr._04      = NULL;
	object->mFloors        = NULL;
	object->_258           = 0;
	object->vf98();
	fn_8007F194(&object->mEvt, &lbl_8025C400.mCMsgs[1], object);
	object->mAnmQueue.Clear();
	object->mAnim.Clear();
}
