#include "game/ccameraaction.h"
#include "game/cgame.h"

extern "C" const char lbl_8020AAF8[];

/**
 * @note Address: 0x800E6790
 * @note Size: 0x68
 */
extern "C" void fn_800E6790(CCameraActionLeticker* camera)
{
#if !defined(VERSION_PAL)
	OSReport(lbl_8020AAF8);
#endif
	camera->CCameraActionLadr::vf14();
	camera->mTurnTo     = 0.0f;
	camera->mTurnFrames = 0;
	camera->mBounded    = false;
	lbl_8025C400.mStatus.mFlags |= 0x80;
}

/**
 * @note Address: 0x800E67F8
 * @note Size: 0x20
 */
extern "C" Vec fn_800E67F8(CCameraActionLeticker* camera)
{
	return *camera->mTarget;
}
