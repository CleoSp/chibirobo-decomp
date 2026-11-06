#include "game/cgame.h"

#pragma section RX ".text.divide"

/**
 * @note Address: 0x800169E4
 * @note Size: 0xD8
 */
__declspec(section ".text.divide") bool CVectorEaseOutDivideInterpolation::Start(Vec from, Vec to, f32 time)
{
	if (time <= 0.0f) {
		return false;
	}
	mAxes[0].Start(from.x, to.x, time);
	mAxes[1].Start(from.y, to.y, time);
	mAxes[2].Start(from.z, to.z, time);
	mXZDone = FALSE;
	return true;
}

/**
 * @note Address: 0x80016ABC
 * @note Size: 0x2C
 */
__declspec(section ".text.divide") bool CInterpolationEaseOutDivide::Start(f32 from, f32 to, f32 time)
{
	if (time <= 0.0f) {
		return false;
	}
	mValue   = from;
	mTarget  = to;
	mDivisor = time;
	return true;
}

/**
 * @note Address: 0x80016AE8
 * @note Size: 0xE0
 */
__declspec(section ".text.divide") bool CVectorEaseOutDivideInterpolation::Update(u32 step, f32 dt)
{
	bool x = mAxes[0].Update(step, dt);
	bool y = mAxes[1].Update(step, dt);
	bool z = mAxes[2].Update(step, dt);
	if (x && z) {
		mXZDone = TRUE;
	}
	return x && y && z;
}

/**
 * @note Address: 0x80016BC8
 * @note Size: 0x84
 */
__declspec(section ".text.divide") bool CInterpolationEaseOutDivide::Update(u32 step, f32 dt)
{
	bool done = false;
	for (u32 i = 0; i < step; i++) {
		done = Step(dt);
		if (done == true) {
			break;
		}
	}
	return done;
}

/**
 * @note Address: 0x80016C4C
 * @note Size: 0x58
 */
__declspec(section ".text.divide") bool CInterpolationEaseOutDivide::Step(f32 dt)
{
	f32 last = mValue;
	if (mDivisor > 0.0f) {
		mValue = (last * (mDivisor - 1.0f) + mTarget) / mDivisor;
	}
	if ((f32)fabs(mValue - last) < 0.001f) {
		return true;
	}
	return false;
}

/**
 * @note Address: 0x80016CA4
 * @note Size: 0xB4
 */
__declspec(section ".text.divide") bool CVectorEaseOutDivideInterpolation::Step(f32 dt)
{
	bool x = mAxes[0].Step(dt);
	bool y = mAxes[1].Step(dt);
	bool z = mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x80016D58
 * @note Size: 0xBC
 */
__declspec(section ".text.divide") Vec CVectorEaseOutDivideInterpolation::Get()
{
	return CVec(mAxes[0].Get(), mAxes[1].Get(), mAxes[2].Get());
}

/**
 * @note Address: 0x80016E14
 * @note Size: 0x8
 */
__declspec(section ".text.divide") f32 CInterpolationEaseOutDivide::Get()
{
	return mValue;
}
