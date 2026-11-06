#include "game/xobj.h"
#include "game/globals.h"

/**
 * @note Address: 0x800928E8
 * @note Size: 0xDC
 */
void XObj::ResetObjectState()
{
	_80C                = 0;
	mFloorHit.mTriangle = -1;
	mFloorHit.mTrigon   = 0;
	mFloorHit.mHeight   = 0.0f;
	_794.x              = 0.0f;
	_794.y              = 0.0f;
	_794.z              = 0.0f;
	mAnmQueue.Clear();
	mAnchorX = mAnchorZ = NAN;
	_8B0                = 0;
	_8B0 |= 0x80000000;
	mColorStep[0].mValue.x = mColorStep[0].mValue.y = mColorStep[0].mValue.z = 255.0f;
	mColorStep[0].mTarget.x = mColorStep[0].mTarget.y = mColorStep[0].mTarget.z = 255.0f;
	mColorStep[0].mSpeed.x = mColorStep[0].mSpeed.y = mColorStep[0].mSpeed.z = 0.0f;
	mColorStep[1].mValue.x = mColorStep[1].mValue.y = mColorStep[1].mValue.z = 0.0f;
	mColorStep[1].mTarget.x = mColorStep[1].mTarget.y = mColorStep[1].mTarget.z = 0.0f;
	mColorStep[1].mSpeed.x = mColorStep[1].mSpeed.y = mColorStep[1].mSpeed.z = 0.0f;
}
