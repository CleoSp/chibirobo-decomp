#include "game/xchr.h"

/**
 * @note Address: 0x800BAD80
 * @note Size: 0x2C
 */
extern "C" CVec fn_800BAD80(CJObj* object, s32 depth, s32* path)
{
	return fn_800A0894(object, depth, path);
}

/**
 * @note Address: 0x800BADAC
 * @note Size: 0xC4
 */
// stops him where he stands
extern "C" void fn_800BADAC(XObjChr* chr)
{
	chr->_988.x         = 0.0f;
	chr->_988.y         = 0.0f;
	chr->_988.z         = 0.0f;
	chr->_988.mTarget.x = 0.0f;
	chr->_988.mTarget.y = 0.0f;
	chr->_988.mTarget.z = 0.0f;
	chr->_988.mSpeed.x  = 0.0f;
	chr->_988.mSpeed.y  = 0.0f;
	chr->_988.mSpeed.z  = 0.0f;
	f32 x               = chr->mMoveRotate.mTarget.x;
	f32 y               = chr->mMoveRotate.mTarget.y;
	f32 z               = chr->mMoveRotate.mTarget.z;
	chr->mMoveRotate.x  = x;
	chr->mMoveRotate.y  = y;
	chr->mMoveRotate.z  = z;
	if (chr->mPush.mState != 0) {
		fn_800C5A00(&chr->mPush);
		chr->mPush.mState = 0;
		chr->SetAnim(17, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		chr->_A2C = 3;
	}
}
