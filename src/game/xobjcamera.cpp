#include "game/xobj.h"

// the camera archive's first desc list, the cobj then its anims
static inline HSD_CObjDesc* GetCObjDesc(XObjCamera* object)
{
	return (HSD_CObjDesc*)object->mArchive.mDesc[0]->_00;
}

static inline HSD_CameraAnim** GetCameraAnims(XObjCamera* object)
{
	return (HSD_CameraAnim**)object->mArchive.mDesc[0]->_04;
}

/**
 * @note Address: 0x80092B80
 * @note Size: 0x10
 */
extern "C" bool fn_80092B80(XObjCamera* object)
{
	return object->mCameraAnim >= 0;
}

// XObjCamera::vfA4
/**
 * @note Address: 0x80092B90
 * @note Size: 0xF8
 */
extern "C" void fn_80092B90(XObjCamera* object, u32 frames)
{
	if (object->mCameraAnim >= 0) {
		object->mCamera.SetAnimRate((f32)frames);
		object->mCamera.Anim();
		f32 end     = object->mCamera.mLongestAObj ? HSD_AObjGetEndFrame(object->mCamera.mLongestAObj) : 0.0f;
		f32 current = object->mCamera.mLongestAObj ? HSD_AObjGetCurrentFrame(object->mCamera.mLongestAObj) : 0.0f;
		if (current >= end)
			object->mCameraAnim = -1;
	}
}

/**
 * @note Address: 0x80092C88
 * @note Size: 0x68
 */
extern "C" void fn_80092C88(XObjCamera* object, s32 index, f32 frame)
{
	object->mCameraAnim = index;
	object->mCamera.SetAnim(GetCameraAnims(object)[object->mCameraAnim], frame);
	object->mCamera.Anim();
}

// XObjCamera::Release
/**
 * @note Address: 0x80092CF0
 * @note Size: 0x44
 */
extern "C" void fn_80092CF0(XObjCamera* object)
{
	object->mCamera.Release();
	fn_800911C0(object);
}

// XObjCamera::vf9C
/**
 * @note Address: 0x80092D34
 * @note Size: 0x5C
 */
extern "C" BOOL fn_80092D34(XObjCamera* object, CoordInfo* info)
{
	if (fn_80091C80(object, info)) {
		object->mCamera.Init(GetCObjDesc(object));
		return TRUE;
	}
	return FALSE;
}

// XObjCamera::vf98
/**
 * @note Address: 0x80092D90
 * @note Size: 0xC
 */
extern "C" void fn_80092D90(XObjCamera* object)
{
	object->mCameraAnim = -1;
}

// XObjCamera::vf90
/**
 * @note Address: 0x80092D9C
 * @note Size: 0x20
 */
extern "C" void fn_80092D9C(XObjCamera* object, u32 id)
{
	fn_800929C4(object, id);
}
