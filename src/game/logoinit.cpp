#include "game/xobjdemo.h"
#include "game/cjobjset.h"
#include "game/cgame.h"
#include "game/logo.h"

/**
 * @note Address: 0x80027A7C
 * @note Size: 0x1C
 */
// Logo's init
extern "C" void fn_80027A7C(Logo* logo)
{
	logo->mState = 0;
	logo->_1F4.x = 0.0f;
	logo->_1F4.y = 0.0f;
	logo->_1F4.z = 0.0f;
}

/**
 * @note Address: 0x80027A98
 * @note Size: 0xD8
 */
// CXObj's SetScaleY
extern "C" void fn_80027A98(CXObj* object, f32 value, s32 force)
{
	object->mMoveScale.y         = value;
	object->mMoveScale.mTarget.y = value;
	if (object->mJObj != NULL) {
		object->CJObj::SetScaleY(value, force);
	}
}

/**
 * @note Address: 0x80027B70
 * @note Size: 0x18C
 */
// CXObj's SetScale(scale)
extern "C" void fn_80027B70(CXObj* object, f32 value, s32 force)
{
	object->mMoveScale.x         = value;
	object->mMoveScale.y         = value;
	object->mMoveScale.z         = value;
	object->mMoveScale.mTarget.x = value;
	object->mMoveScale.mTarget.y = value;
	object->mMoveScale.mTarget.z = value;
	if (object->mJObj != NULL) {
		object->CJObj::SetScale(value, value, value, force);
	}
}

/**
 * @note Address: 0x80027CFC
 * @note Size: 0x18C
 */
// CXObj's SetScale(x, y, z)
extern "C" void fn_80027CFC(CXObj* object, f32 x, f32 y, f32 z, s32 force)
{
	object->mMoveScale.x         = x;
	object->mMoveScale.y         = y;
	object->mMoveScale.z         = z;
	object->mMoveScale.mTarget.x = x;
	object->mMoveScale.mTarget.y = y;
	object->mMoveScale.mTarget.z = z;
	if (object->mJObj != NULL) {
		object->CJObj::SetScale(x, y, z, force);
	}
}

/**
 * @note Address: 0x80027E88
 * @note Size: 0x178
 */
// CXObj's SetScale(v)
extern "C" void fn_80027E88(CXObj* object, const Vec& scale, s32 force)
{
	object->mMoveScale.x         = scale.x;
	object->mMoveScale.y         = scale.y;
	object->mMoveScale.z         = scale.z;
	object->mMoveScale.mTarget.x = scale.x;
	object->mMoveScale.mTarget.y = scale.y;
	object->mMoveScale.mTarget.z = scale.z;
	if (object->mJObj != NULL) {
		object->CJObj::SetScale(scale, force);
	}
}

/**
 * @note Address: 0x80028000
 * @note Size: 0xB4
 */
// XObjDemo's vfAC, draws through its own camera and lights when it has them
extern "C" void fn_80028000(XObjDemo* demo)
{
	if (demo->mState != 0) {
		CCamera* camera = demo->mArchive.mDesc[0] != NULL ? &demo->mCamera : (CCamera*)lbl_8025C400.mCameras[0];
		CLight* light   = demo->mArchive.mDesc[1] != NULL ? &demo->mLights[0] : (CLight*)lbl_8025C400.mLights[0];
		light->SetCurrent(camera);
		if (camera->SetCurrent() != NULL) {
			demo->vfB0(NULL);
			camera->EndCurrent();
		}
	}
}
