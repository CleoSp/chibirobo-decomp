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

	virtual void Release();          // _54
	virtual void Init(u32 id);       // _90
	virtual void Reset();            // _98
	virtual BOOL vf9C(void* info);   // _9C
	virtual void Update(u32 frames); // _A4
};
typedef char XObjCamera_size_check[sizeof(XObjCamera) == 0xB18 ? 1 : -1];

inline f32 HSD_AObjGetCurrFrame(HSD_AObj* aobj)
{
	AOBJ_H_ASSERT(0x95, aobj);
	return aobj->curr_frame;
}

// ---- functions ----

extern "C" bool fn_80092B80(const XObjCamera* obj)
{
	return obj->mAnim >= 0;
}

void XObjCamera::Update(u32 frames)
{
	if (mAnim >= 0) {
		mCamera.SetAnimRate(frames);
		mCamera.Anim();
		f32 end = mCamera.mLongestAObj != NULL ? HSD_AObjGetEndFrame(mCamera.mLongestAObj) : 0.0f;
		f32 cur = mCamera.mLongestAObj != NULL ? HSD_AObjGetCurrFrame(mCamera.mLongestAObj) : 0.0f;
		if (cur >= end) {
			mAnim = -1;
		}
	}
}

extern "C" void fn_80092C88(XObjCamera* obj, s32 anim, f32 frame)
{
	obj->mAnim = anim;
	obj->mCamera.SetAnim((HSD_CameraAnim*)obj->mArchive.mDesc[0]->_04[obj->mAnim], frame);
	obj->mCamera.Anim();
}

void XObjCamera::Release()
{
	mCamera.Release();
	fn_800911C0(this);
}

BOOL XObjCamera::vf9C(void* info)
{
	if (fn_80091C80(this, info)) {
		mCamera.Init((HSD_CObjDesc*)mArchive.mDesc[0]->_00);
		return TRUE;
	}
	return FALSE;
}

void XObjCamera::Reset()
{
	mAnim = -1;
}

void XObjCamera::Init(u32 id)
{
	fn_800929C4(this, id);
}
