#include "game/hsd_jobj.h"
#include "game/ccamera.h"
#include "game/clight.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__7CCamera[];
extern void* __vt__6CLight[];
}

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

/**
 * @note Address: 0x800084CC
 * @note Size: 0x13C
 */
// todo: weak copy
extern "C" CLight* __dt__6CLightFv(CLight* light, s32 flags)
{
	if (light != NULL) {
		*(void***)((u8*)light + 0x354) = __vt__6CLight;
		light->mShadow.CShadow::~CShadow();
		light->mSpecColor.CColor::~CColor();
		light->mColor.CColor::~CColor();
		light->_F8.CVec::~CVec();
		light->mInterest.CVec::~CVec();
		light->mPosition.CVec::~CVec();
		if ((s16)flags > 0)
			::operator delete(light);
	}
	return light;
}
