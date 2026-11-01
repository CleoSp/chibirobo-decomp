#include "game/hsd_jobj.h"
#include "game/cscrn.h"
#include "game/video.h"

// todo: cleanup
static inline bool SizeStopped(CScrnWipe* wipe)
{
	return 0.0f == wipe->mSpeedWidth && 0.0f == wipe->mSpeedHeight;
}

/**
 * @note Address: 0x80023438
 * @note Size: 0x2B4
 */
// starts a wipe, the color and the hole size both go from the first values to the second
extern "C" void fn_80023438(CScrnWipe* wipe, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1, u8 g1, u8 b1, f32 x, f32 y, f32 width0, f32 height0,
                            f32 width1, f32 height1, f32 time, u8 a1, BOOL hideWhenDone)
{
	wipe->mActive       = TRUE;
	wipe->mHideWhenDone = hideWhenDone;
	wipe->mX            = x;
	wipe->mY            = y;
	wipe->mWidth        = width0;
	wipe->mHeight       = height0;
	wipe->mTargetWidth  = width1;
	wipe->mTargetHeight = height1;
	if (0.0f == time) {
		wipe->mWidth  = width1;
		wipe->mHeight = height1;
	} else {
		wipe->mSpeedWidth  = (width1 - width0) / (FRAME_RATE * time);
		wipe->mSpeedHeight = (height1 - height0) / (FRAME_RATE * time);
	}
	wipe->mColor.r  = r0;
	wipe->mColor.g  = g0;
	wipe->mColor.b  = b0;
	wipe->mColor.a  = a0;
	wipe->mTarget.r = r1;
	wipe->mTarget.g = g1;
	wipe->mTarget.b = b1;
	wipe->mTarget.a = a1;
	if (0.0f == time) {
		wipe->mColor.r = r1;
		wipe->mColor.g = g1;
		wipe->mColor.b = b1;
		wipe->mColor.a = a1;
		wipe->mSpeed.r = 0.0f;
		wipe->mSpeed.g = 0.0f;
		wipe->mSpeed.b = 0.0f;
		wipe->mSpeed.a = 0.0f;
	} else {
		time           = FRAME_RATE * time;
		wipe->mSpeed.r = ((f32)r1 - (f32)r0) / time;
		wipe->mSpeed.g = ((f32)g1 - (f32)g0) / time;
		wipe->mSpeed.b = ((f32)b1 - (f32)b0) / time;
		wipe->mSpeed.a = ((f32)a1 - (f32)a0) / time;
	}
	if (wipe->mHideWhenDone && SizeStopped(wipe))
		wipe->mActive = FALSE;
}

/**
 * @note Address: 0x800236EC
 * @note Size: 0x1B8
 */
// the same with one color
extern "C" void fn_800236EC(CScrnWipe* wipe, u8 r, u8 g, u8 b, u8 a, BOOL hideWhenDone, f32 x, f32 y, f32 width0, f32 height0, f32 width1,
                            f32 height1, f32 time)
{
	wipe->mActive       = TRUE;
	wipe->mHideWhenDone = hideWhenDone;
	wipe->mX            = x;
	wipe->mY            = y;
	wipe->mWidth        = width0;
	wipe->mHeight       = height0;
	wipe->mTargetWidth  = width1;
	wipe->mTargetHeight = height1;
	if (0.0f == time) {
		wipe->mWidth      = width1;
		wipe->mHeight     = height1;
		wipe->mSpeedWidth = wipe->mSpeedHeight = 0.0f;
	} else {
		wipe->mSpeedWidth  = (width1 - width0) / (FRAME_RATE * time);
		wipe->mSpeedHeight = (height1 - height0) / (FRAME_RATE * time);
	}
	wipe->mColor.r  = r;
	wipe->mColor.g  = g;
	wipe->mColor.b  = b;
	wipe->mColor.a  = a;
	wipe->mTarget.r = r;
	wipe->mTarget.g = g;
	wipe->mTarget.b = b;
	wipe->mTarget.a = a;
	wipe->mSpeed.r  = r;
	wipe->mSpeed.g  = g;
	wipe->mSpeed.b  = b;
	wipe->mSpeed.a  = a;
	if (wipe->mHideWhenDone && SizeStopped(wipe))
		wipe->mActive = FALSE;
}

/**
 * @note Address: 0x800238A4
 * @note Size: 0x68
 */
extern "C" void fn_800238A4(CScrnWipe* wipe)
{
	wipe->mActive = FALSE;
	wipe->mX = wipe->mY = 0.0f;
	wipe->mWidth = wipe->mHeight = 0.0f;
	wipe->mTargetWidth = wipe->mTargetHeight = 0.0f;
	wipe->mSpeedWidth = wipe->mSpeedHeight = 0.0f;
	wipe->mColor.r                         = 0.0f;
	wipe->mColor.g                         = 0.0f;
	wipe->mColor.b                         = 0.0f;
	wipe->mColor.a                         = 0.0f;
	wipe->mTarget.r                        = 0.0f;
	wipe->mTarget.g                        = 0.0f;
	wipe->mTarget.b                        = 0.0f;
	wipe->mTarget.a                        = 0.0f;
	wipe->mSpeed.r                         = 0.0f;
	wipe->mSpeed.g                         = 0.0f;
	wipe->mSpeed.b                         = 0.0f;
	wipe->mSpeed.a                         = 0.0f;
	wipe->_5C = wipe->_60 = 0.0f;
}
