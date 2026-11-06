#include "game/cmotioncontrolcamera.h"

void operator delete(void* object);
extern "C" void* __vt__16CCameraActionEvt[];

/**
 * @note Address: 0x800099C8
 * @note Size: 0x4FC
 */
// todo: weak copy
extern "C" CCameraActionEvt* __dt__16CCameraActionEvtFv(CCameraActionEvt* evt, s32 flags)
{
	if (evt != NULL) {
		*(void***)((u8*)evt + 0x3C) = __vt__16CCameraActionEvt;
		evt->mAngleNone.CVectorNoneInterpolation::~CVectorNoneInterpolation();
		evt->mAngleEaseOut.CVectorEaseOutInterpolation::~CVectorEaseOutInterpolation();
		evt->mAngleEaseInOut.CVectorEaseInOutInterpolation::~CVectorEaseInOutInterpolation();
		evt->mAngleLinear.CVectorLinearInterpolation::~CVectorLinearInterpolation();
		evt->mDistanceNone.CInterpolationNone::~CInterpolationNone();
		evt->mDistanceEaseOut.CInterpolationEaseOut::~CInterpolationEaseOut();
		evt->mDistanceEaseInOut.CInterpolationEaseInOut::~CInterpolationEaseInOut();
		evt->mDistanceLinear.CInterpolationLinear::~CInterpolationLinear();
		evt->mFovNone.CInterpolationNone::~CInterpolationNone();
		evt->mFovEaseOut.CInterpolationEaseOut::~CInterpolationEaseOut();
		evt->mFovEaseInOut.CInterpolationEaseInOut::~CInterpolationEaseInOut();
		evt->mFovLinear.CInterpolationLinear::~CInterpolationLinear();
		evt->mInterestNone.CVectorNoneInterpolation::~CVectorNoneInterpolation();
		evt->mInterestEaseOut.CVectorEaseOutInterpolation::~CVectorEaseOutInterpolation();
		evt->mInterestEaseInOut.CVectorEaseInOutInterpolation::~CVectorEaseInOutInterpolation();
		evt->mInterestLinear.CVectorLinearInterpolation::~CVectorLinearInterpolation();
		evt->mEyeNone.CVectorNoneInterpolation::~CVectorNoneInterpolation();
		evt->mEyeEaseOut.CVectorEaseOutInterpolation::~CVectorEaseOutInterpolation();
		evt->mEyeEaseInOut.CVectorEaseInOutInterpolation::~CVectorEaseInOutInterpolation();
		evt->mEyeLinear.CVectorLinearInterpolation::~CVectorLinearInterpolation();
		evt->mAxes.CStatus::~CStatus();
		evt->CInterfaceCameraAction::~CInterfaceCameraAction();
		if ((s16)flags > 0)
			::operator delete(evt);
	}
	return evt;
}
