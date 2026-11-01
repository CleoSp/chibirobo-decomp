#include "game/hsd_jobj.h"
#include "game/cscrn.h"

/**
 * @note Address: 0x80021474
 * @note Size: 0x8C
 */
extern "C" void fn_80021474(CScrnRoboeyes* eyes, u32 frames)
{
	fn_80023030(eyes, frames);
	if (eyes->mActive) {
		eyes->mAngle += 0.02617994f * frames;
		eyes->mPhase += 0.10471976f * frames;
	}
}

/**
 * @note Address: 0x80021500
 * @note Size: 0x70
 */
extern "C" void fn_80021500(CScrnRoboeyes* eyes)
{
	eyes->mActive = FALSE;
	eyes->mX = eyes->mY = 0.0f;
	eyes->mWidth = eyes->mHeight = 0.0f;
	eyes->mTargetWidth = eyes->mTargetHeight = 0.0f;
	eyes->mSpeedWidth = eyes->mSpeedHeight = 0.0f;
	eyes->mColor.r                         = 0.0f;
	eyes->mColor.g                         = 0.0f;
	eyes->mColor.b                         = 0.0f;
	eyes->mColor.a                         = 0.0f;
	eyes->mTarget.r                        = 0.0f;
	eyes->mTarget.g                        = 0.0f;
	eyes->mTarget.b                        = 0.0f;
	eyes->mTarget.a                        = 0.0f;
	eyes->mSpeed.r                         = 0.0f;
	eyes->mSpeed.g                         = 0.0f;
	eyes->mSpeed.b                         = 0.0f;
	eyes->mSpeed.a                         = 0.0f;
	eyes->_5C = eyes->_60 = 0.0f;
	eyes->mAngle          = 0.0f;
	eyes->mPhase          = 0.0f;
}
