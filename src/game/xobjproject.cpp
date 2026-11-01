#include "game/cobj.h"
#include "game/ccamera.h"
#include "game/cgame.h"
#include <math.h>

extern "C" {
#include <sysdolphin/mtx.h>
#include "game/gmath.h"
void HSD_JObjWalkTree(HSD_JObj* joint, HSD_JObjWalkTreeCallback callback, f32** arguments);
void JObjClearPObjFlags(HSD_JObj* joint, u32 flags);
void JObjSetPObjFlags(HSD_JObj* joint, u32 flags);
void GXProject(f32 x, f32 y, f32 z, const Mtx view, const f32* projection, const f32* viewport, f32* screenX, f32* screenY, f32* screenZ);
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
extern "C" BOOL fn_8008B224(CJObj* object, CCamera* camera)
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
	camera          = camera != NULL ? camera : (CCamera*)lbl_8025C400.mCamera;
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
	f32 scale = 448.0f / ((GXRenderModeObj*)lbl_8025C400.mGs)->xfbHeight;
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
	camera = camera != NULL ? camera : (CCamera*)lbl_8025C400.mCamera;
	f32 screenX, screenY, screenZ;
	GXProject(object->mMoveTranslate.x + offset->x, object->mMoveTranslate.y + offset->y, object->mMoveTranslate.z + offset->z,
	          camera->mViewMtx, camera->mProjection, camera->mViewport, &screenX, &screenY, &screenZ);
	if (x != NULL)
		*x = screenX;
	if (y != NULL) {
#if defined(VERSION_PAL)
		*y = 448.0f * screenY / ((GXRenderModeObj*)lbl_8025C400.mGs)->xfbHeight;
#else
		*y = screenY;
#endif
	}
	if (z != NULL)
		*z = screenZ;
}
