#include "game/xchr.h"
#include <math.h>
#include "game/gmath.h"
#include "game/globals.h"

extern "C" Vec lbl_80378234;

/**
 * @note Address: 0x800B1B0C
 * @note Size: 0x8
 */
extern "C" void fn_800B1B0C(XActPush* act, s32 value)
{
	act->_24 = value;
}

/**
 * @note Address: 0x800B1B14
 * @note Size: 0x8
 */
extern "C" void fn_800B1B14(XActPush* act, s32 state)
{
	act->mState = state;
}

/**
 * @note Address: 0x800B1B1C
 * @note Size: 0x8
 */
extern "C" void fn_800B1B1C(XActHang* act, s32 state)
{
	act->mState = state;
}

/**
 * @note Address: 0x800B1B24
 * @note Size: 0x4C
 */
extern "C" u8 fn_800B1B24(XObjChr* chr)
{
	return (u8)(chr->mLift.mState == 4 && fn_800D4FE8(&chr->mLift));
}

/**
 * @note Address: 0x800B1B70
 * @note Size: 0x14
 */
extern "C" void fn_800B1B70(XAction* action, XObj* target)
{
	action->SetTarget(target);
}

/**
 * @note Address: 0x800B1B84
 * @note Size: 0x48
 */
extern "C" f32 fn_800B1B84(void*)
{
	return (f32)(s32)fn_8006E9AC(2) / 100.0f;
}

/**
 * @note Address: 0x800B1BCC
 * @note Size: 0x48
 */
extern "C" f32 fn_800B1BCC(void*)
{
	return (f32)(s32)fn_8006E9AC(1) / 100.0f;
}

/**
 * @note Address: 0x800B1C14
 * @note Size: 0x48
 */
extern "C" f32 fn_800B1C14(void*)
{
	return (f32)(s32)fn_8006E9AC(0) / 100.0f;
}

/**
 * @note Address: 0x800B1C5C
 * @note Size: 0xF4
 */
extern "C" f32 fn_800B1C5C(const CPadOne* pad)
{
	f32 x = pad->mStickX;
	f32 y = pad->mStickY;
	return sqrtf(x * x + y * y);
}

/**
 * @note Address: 0x800B1D50
 * @note Size: 0x34
 */
extern "C" bool fn_800B1D50(XObj* object)
{
	return (u8)((object->mStatus.mFlags & 0x0000000400000000ULL) != 0);
}

/**
 * @note Address: 0x800B1D84
 * @note Size: 0x8
 */
extern "C" CVec* fn_800B1D84(CCamera* camera)
{
	return &camera->mAngle;
}

/**
 * @note Address: 0x800B1D8C
 * @note Size: 0x50
 */
extern "C" void fn_800B1D8C(XObjChrLook* look, f64 x, f64 y, f64 z)
{
	Vec target    = lbl_80378234;
	target.x      = x;
	target.y      = y;
	target.z      = z;
	look->mTarget = target;
}

/**
 * @note Address: 0x800B1DDC
 * @note Size: 0x8
 */
extern "C" void fn_800B1DDC(XObjChrLook* look, s32 mode)
{
	look->mMode = mode;
}

/**
 * @note Address: 0x800B1DE4
 * @note Size: 0xC
 */
extern "C" void fn_800B1DE4(XObjChrLook* look)
{
	look->mActive = TRUE;
}

/**
 * @note Address: 0x800B1DF0
 * @note Size: 0xC
 */
extern "C" u32 fn_800B1DF0(XObj* object, u32 mask)
{
	return object->_8B0 & mask;
}

/**
 * @note Address: 0x800B1DFC
 * @note Size: 0x8
 */
extern "C" u32 fn_800B1DFC(XObj* object)
{
	return object->_8B0;
}

/**
 * @note Address: 0x800B1E04
 * @note Size: 0x8
 */
extern "C" s32 fn_800B1E04(void*)
{
	return 0;
}

/**
 * @note Address: 0x800B1E0C
 * @note Size: 0xAC
 */
// is position behind the object, within 60 degrees either side
extern "C" u8 fn_800B1E0C(void*, const Vec* pos, CXObj* object)
{
	f32 angle = (f32)atan2(object->mMoveTranslate.x - pos->x, object->mMoveTranslate.z - pos->z);
	angle -= object->mMoveRotate.y;
	while (angle < 0.0f)
		angle += TWO_PI;
	while (angle >= TWO_PI)
		angle -= TWO_PI;
	return (u8)(angle >= 2.0f * PI / 3.0f && angle <= 4.0f * PI / 3.0f);
}

/**
 * @note Address: 0x800B1EB8
 * @note Size: 0x8
 */
extern "C" f32 fn_800B1EB8(XObj* object)
{
	return object->_77C;
}
