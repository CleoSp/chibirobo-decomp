#include "game/xobj.h"
#include "game/cjobjset.h"
#include "game/cgame.h"
#include "game/hsd_dobj.h"
#include "game/globals.h"
#include "game/xobjchr.h"

extern "C" {
void JObjSetPObjFlags(HSD_JObj* jobj, void* flags, u32 type);
void JObjClearPObjFlags(HSD_JObj* jobj, void* flags, u32 type);
}

extern "C" {
CObjArchive* fn_80132620(CObjArchive* archives);
void GXProject(f32 x, f32 y, f32 z, const Mtx view, const f32* projection, const f32* viewport, f32* screenX, f32* screenY, f32* screenZ);
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

/**
 * @note Address: 0x8008A360
 * @note Size: 0x2C
 */
// CJAnim::Release
extern "C" void Release__6CJAnimFv(CJAnim* anim)
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
extern "C" void Clear__6CJAnimFv(CJAnim* anim)
{
	anim->CJArchive::Clear();
	anim->mLoading = 0;
}

/**
 * @note Address: 0x8008A530
 * @note Size: 0xD4
 */
extern "C" BOOL vf140__4XObjFll(XObj* object, s32 unused, s32 value)
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
extern "C" void vf13C__4XObjFll(XObj* object, s32 unused, s32 value)
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
extern "C" void vf138__4XObjFRC3Vecf(XObj* object, const Vec* position, f32 strength)
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
extern "C" BOOL vf134__4XObjFv(void* object)
{
	return TRUE;
}

/**
 * @note Address: 0x8008A820
 * @note Size: 0x124
 */
extern "C" void vf12C__4XObjFlUcUcUcf(XObj* object, s32 index, u8 red, u8 green, u8 blue, f32 time)
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
extern "C" void vf120__4XObjFl(CJObj* object, s32 mode)
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
extern "C" BOOL vf11C__4XObjFP7CCamera(const XObj* object, CCamera* camera)
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
extern "C" BOOL vf118__4XObjFPfPfP7CCameraff(CXObj* object, f32* width, f32* height, CCamera* camera, f32 radius, f32 objectHeight)
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
extern "C" void vf10C__4XObjFPC3VecPfPfPfP7CCamera(CXObj* object, const Vec* offset, f32* x, f32* y, f32* z, CCamera* camera)
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
