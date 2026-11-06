#define DEFER_VECTOR_INTERPOLATION_DESTRUCTORS
#include "game/cgame.h"

#pragma section RX ".text.vectorinterpolation"

/**
 * @note Address: 0x80015F08
 * @note Size: 0xCC
 */
__declspec(section ".text.vectorinterpolation") bool CVectorLinearInterpolation::Start(Vec from, Vec to, f32 time)
{
	if (time < 0.0f) {
		return false;
	}
	mAxes[0].Start(from.x, to.x, time);
	mAxes[1].Start(from.y, to.y, time);
	mAxes[2].Start(from.z, to.z, time);
	return true;
}

/**
 * @note Address: 0x80015FD4
 * @note Size: 0x30
 */
__declspec(section ".text.vectorinterpolation") bool CInterpolationLinear::Start(f32 from, f32 to, f32 time)
{
	if (time < 0.0f) {
		return false;
	}
	mTime  = time;
	mValue = from;
	mSpeed = (to - from) / time;
	return true;
}

/**
 * @note Address: 0x80016004
 * @note Size: 0xB4
 */
__declspec(section ".text.vectorinterpolation") bool CVectorLinearInterpolation::Step(f32 dt)
{
	bool x = mAxes[0].Step(dt);
	bool y = mAxes[1].Step(dt);
	bool z = mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x800160B8
 * @note Size: 0x38
 */
__declspec(section ".text.vectorinterpolation") bool CInterpolationLinear::Step(f32 dt)
{
	mTime -= dt;
	mValue += mSpeed * dt;
	return mTime <= 0.0f;
}

/**
 * @note Address: 0x800160F0
 * @note Size: 0x2C
 */
__declspec(section ".text.vectorinterpolation") bool CVectorLinearInterpolation::Update(u32 step, f32 dt)
{
	return Step(dt);
}

/**
 * @note Address: 0x8001611C
 * @note Size: 0xBC
 */
__declspec(section ".text.vectorinterpolation") Vec CVectorLinearInterpolation::Get()
{
	return CVec(mAxes[0].Get(), mAxes[1].Get(), mAxes[2].Get());
}

/**
 * @note Address: 0x800161D8
 * @note Size: 0x8
 */
__declspec(section ".text.vectorinterpolation") f32 CInterpolationLinear::Get()
{
	return mValue;
}

__declspec(section ".text.vectorinterpolation") CVectorLinearInterpolation::~CVectorLinearInterpolation()
{
}

/**
 * @note Address: 0x80016264
 * @note Size: 0xD8
 */
__declspec(section ".text.vectorinterpolation") bool CVectorEaseInOutInterpolation::Start(Vec from, Vec to, f32 time)
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
 * @note Address: 0x8001633C
 * @note Size: 0xE0
 */
__declspec(section ".text.vectorinterpolation") bool CVectorEaseInOutInterpolation::Update(u32 step, f32 dt)
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
 * @note Address: 0x8001641C
 * @note Size: 0xB4
 */
__declspec(section ".text.vectorinterpolation") bool CVectorEaseInOutInterpolation::Step(f32 dt)
{
	bool x = mAxes[0].Step(dt);
	bool y = mAxes[1].Step(dt);
	bool z = mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x800164D0
 * @note Size: 0xBC
 */
__declspec(section ".text.vectorinterpolation") Vec CVectorEaseInOutInterpolation::Get()
{
	return CVec(mAxes[0].Get(), mAxes[1].Get(), mAxes[2].Get());
}

__declspec(section ".text.vectorinterpolation") CVectorEaseInOutInterpolation::~CVectorEaseInOutInterpolation()
{
}

/**
 * @note Address: 0x80016610
 * @note Size: 0xD0
 */
__declspec(section ".text.vectorinterpolation") bool CVectorEaseOutInterpolation::Start(Vec from, Vec to, f32 time)
{
	if (time <= 0.0f) {
		return false;
	}
	mAxes[0].Start(from.x, to.x, time);
	mAxes[1].Start(from.y, to.y, time);
	mAxes[2].Start(from.z, to.z, time);
	return true;
}

/**
 * @note Address: 0x800166E0
 * @note Size: 0xC8
 */
__declspec(section ".text.vectorinterpolation") bool CVectorEaseOutInterpolation::Update(u32 step, f32 dt)
{
	bool x = mAxes[0].Update(step, dt);
	bool y = mAxes[1].Update(step, dt);
	bool z = mAxes[2].Update(step, dt);
	return x && y && z;
}

/**
 * @note Address: 0x800167A8
 * @note Size: 0xB4
 */
__declspec(section ".text.vectorinterpolation") bool CVectorEaseOutInterpolation::Step(f32 dt)
{
	bool x = mAxes[0].Step(dt);
	bool y = mAxes[1].Step(dt);
	bool z = mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x8001685C
 * @note Size: 0xBC
 */
__declspec(section ".text.vectorinterpolation") Vec CVectorEaseOutInterpolation::Get()
{
	return CVec(mAxes[0].Get(), mAxes[1].Get(), mAxes[2].Get());
}
__declspec(section ".text.vectorinterpolation") CVectorEaseOutInterpolation::~CVectorEaseOutInterpolation()
{
}
