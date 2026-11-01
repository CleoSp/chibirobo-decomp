#include "game/hsd_jobj.h"
#include "game/ccamera.h"

void operator delete(void* object);

extern "C" void* __vt__7CCamera[];

/**
 * @note Address: 0x8000841C
 * @note Size: 0xB0
 */
// todo: weak copy
extern "C" CCamera* __dt__7CCameraFv(CCamera* camera, s32 flags)
{
	if (camera != NULL) {
		*(void***)((u8*)camera + 0x190) = __vt__7CCamera;
		camera->mScale.CVec::~CVec();
		camera->mUp.CVec::~CVec();
		camera->mAngle.CVec::~CVec();
		camera->mInterest.CVec::~CVec();
		camera->mEye.CVec::~CVec();
		if ((s16)flags > 0)
			::operator delete(camera);
	}
	return camera;
}
