#include "game/hsd_jobj.h"
#include "game/cscrn.h"
#include "game/video.h"

// todo: cleanup
static inline void StepChannel(f32& current, f32& target, f32& speed, u32 frames)
{
	if (current != target) {
		current += speed * frames;
		if (speed > 0.0f && current >= target) {
			current = target;
			speed   = 0.0f;
		}
		if (speed < 0.0f && current <= target) {
			current = target;
			speed   = 0.0f;
		}
	}
}

/**
 * @note Address: 0x80023B38
 * @note Size: 0x23C
 */
extern "C" void fn_80023B38(CScrnColor* color, u32 frames)
{
	StepChannel(color->mColor.r, color->mTarget.r, color->mSpeed.r, frames);
	StepChannel(color->mColor.g, color->mTarget.g, color->mSpeed.g, frames);
	StepChannel(color->mColor.b, color->mTarget.b, color->mSpeed.b, frames);
	StepChannel(color->mColor.a, color->mTarget.a, color->mSpeed.a, frames);
}

/**
 * @note Address: 0x80023D74
 * @note Size: 0x16C
 */
extern "C" void fn_80023D74(CScrnColor* color, u8 r, u8 g, u8 b, u8 a, f32 time)
{
	color->mTarget.r = r;
	color->mTarget.g = g;
	color->mTarget.b = b;
	color->mTarget.a = a;
	if (0.0f == time) {
		color->mColor.r = r;
		color->mColor.g = g;
		color->mColor.b = b;
		color->mColor.a = a;
		color->mSpeed.r = 0.0f;
		color->mSpeed.g = 0.0f;
		color->mSpeed.b = 0.0f;
		color->mSpeed.a = 0.0f;
	} else {
		time            = FRAME_RATE * time;
		f32 sr          = ((f32)r - color->mColor.r) / time;
		f32 sg          = ((f32)g - color->mColor.g) / time;
		f32 sb          = ((f32)b - color->mColor.b) / time;
		f32 sa          = ((f32)a - color->mColor.a) / time;
		color->mSpeed.r = sr;
		color->mSpeed.g = sg;
		color->mSpeed.b = sb;
		color->mSpeed.a = sa;
	}
}

/**
 * @note Address: 0x80023EE0
 * @note Size: 0x20C
 */
extern "C" void fn_80023EE0(CScrnColor* color, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1, u8 g1, u8 b1, u8 a1, f32 time)
{
	color->mColor.r  = r0;
	color->mColor.g  = g0;
	color->mColor.b  = b0;
	color->mColor.a  = a0;
	color->mTarget.r = r1;
	color->mTarget.g = g1;
	color->mTarget.b = b1;
	color->mTarget.a = a1;
	if (0.0f == time) {
		color->mColor.r = r1;
		color->mColor.g = g1;
		color->mColor.b = b1;
		color->mColor.a = a1;
		color->mSpeed.r = 0.0f;
		color->mSpeed.g = 0.0f;
		color->mSpeed.b = 0.0f;
		color->mSpeed.a = 0.0f;
	} else {
		time            = FRAME_RATE * time;
		color->mSpeed.r = ((f32)r1 - (f32)r0) / time;
		color->mSpeed.g = ((f32)g1 - (f32)g0) / time;
		color->mSpeed.b = ((f32)b1 - (f32)b0) / time;
		color->mSpeed.a = ((f32)a1 - (f32)a0) / time;
	}
}
