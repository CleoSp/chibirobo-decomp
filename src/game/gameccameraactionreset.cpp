#include "game/ccameraaction.h"

void operator delete(void* object);

extern "C" void* __vt__18CCameraActionReset[];

/**
 * @note Address: 0x8000A260
 * @note Size: 0x258
 */
// todo: weak copy
extern "C" CCameraActionReset* __dt__18CCameraActionResetFv(CCameraActionReset* reset, s32 flags)
{
	if (reset != NULL) {
		*(void***)((u8*)reset + 0x3C) = __vt__18CCameraActionReset;
		reset->mFovNone.CInterpolationNone::~CInterpolationNone();
		reset->mFovEaseOut.CInterpolationEaseOut::~CInterpolationEaseOut();
		reset->mFovEaseInOut.CInterpolationEaseInOut::~CInterpolationEaseInOut();
		reset->mFovLinear.CInterpolationLinear::~CInterpolationLinear();
		reset->mEyeNone.CVectorNoneInterpolation::~CVectorNoneInterpolation();
		reset->mEyeEaseOut.CVectorEaseOutInterpolation::~CVectorEaseOutInterpolation();
		reset->mEyeEaseInOut.CVectorEaseInOutInterpolation::~CVectorEaseInOutInterpolation();
		reset->mEyeLinear.CVectorLinearInterpolation::~CVectorLinearInterpolation();
		reset->CCameraActionAreaExM::~CCameraActionAreaExM();
		if ((s16)flags > 0)
			::operator delete(reset);
	}
	return reset;
}
