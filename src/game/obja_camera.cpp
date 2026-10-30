#include "game/obja.h"
#include "game/ccamera.h"
#include "game/gmath.h"
extern "C" {
#include <dolphin/mtx/vec.h>
}
#include "game/cvec.h"

// XObjCamera, an XObj that carries a camera with its own animations

// __vt__10XObjCamera
class XObjCamera : public XObj {
public:
	CCamera mCamera; // _980
	s32 mAnim;       // _B14, the camera anim playing, -1 once it ended
};
typedef char XObjCamera_size_check[sizeof(XObjCamera) == 0xB18 ? 1 : -1];

inline f32 HSD_AObjGetCurrFrame(HSD_AObj* aobj)
{
	AOBJ_H_ASSERT(0x95, aobj);
	return aobj->curr_frame;
}

// ---- functions ----

extern "C" {

bool fn_80092B80(const XObjCamera* obj)
{
	return obj->mAnim >= 0;
}

void fn_80092B90(XObjCamera* obj, u32 frames)
{
	if (obj->mAnim >= 0) {
		obj->mCamera.SetAnimRate(frames);
		obj->mCamera.Anim();
		f32 end = obj->mCamera.mLongestAObj != NULL ? HSD_AObjGetEndFrame(obj->mCamera.mLongestAObj) : 0.0f;
		f32 cur = obj->mCamera.mLongestAObj != NULL ? HSD_AObjGetCurrFrame(obj->mCamera.mLongestAObj) : 0.0f;
		if (cur >= end) {
			obj->mAnim = -1;
		}
	}
}

void fn_80092C88(XObjCamera* obj, s32 anim, f32 frame)
{
	obj->mAnim = anim;
	obj->mCamera.SetAnim((HSD_CameraAnim*)obj->mArchive.mDesc[0]->_04[obj->mAnim], frame);
	obj->mCamera.Anim();
}

void fn_80092CF0(XObjCamera* obj)
{
	obj->mCamera.Release();
	fn_800911C0(obj);
}

BOOL fn_80092D34(XObjCamera* obj, void* info)
{
	if (fn_80091C80(obj, info)) {
		obj->mCamera.Init((HSD_CObjDesc*)obj->mArchive.mDesc[0]->_00);
		return TRUE;
	}
	return FALSE;
}

void fn_80092D90(XObjCamera* obj)
{
	obj->mAnim = -1;
}

void fn_80092D9C(XObjCamera* obj, u32 id)
{
	fn_800929C4(obj, id);
}

} // extern "C"
