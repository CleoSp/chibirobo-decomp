#include "game/hsd_jobj.h"
#include "game/cmotioncontrolcamera.h"

void operator delete(void* object);

extern "C" void* __vt__20CMotionControlCamera[];

/**
 * @note Address: 0x8000902C
 * @note Size: 0x1A0
 */
// todo: weak copy
extern "C" CMotionControlCamera* fn_8000902C(CMotionControlCamera* camera, s32 flags)
{
	if (camera != NULL) {
		*(void***)((u8*)camera + 0x1F4) = __vt__20CMotionControlCamera;
		camera->mMemento.CCameraMemento::~CCameraMemento();
		camera->mFollow.mStep.CVec::~CVec();
		camera->mFollow.mValue.CVec::~CVec();
		camera->mFollow.mTarget.CVec::~CVec();
		camera->mFlags.CStatus::~CStatus();
		camera->mAngle.CVec::~CVec();
		camera->mInterest.CVec::~CVec();
		camera->mEye.CVec::~CVec();
		camera->mCamera.CCamera::~CCamera();
		if ((s16)flags > 0)
			::operator delete(camera);
	}
	return camera;
}
