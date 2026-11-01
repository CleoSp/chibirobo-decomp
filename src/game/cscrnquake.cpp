#include "game/hsd_jobj.h"
#include "game/cscrn.h"
extern "C" {
#include <dolphin/mtx/vec.h>
}
#include "game/video.h"

// todo: cleanup
static inline void StepAxis(f32& amount, f32& target, f32& velocity, u32 frames)
{
	if (amount != target) {
		amount += velocity * frames;
		if (velocity > 0.0f && amount >= target) {
			amount   = target;
			velocity = 0.0f;
		}
		if (velocity < 0.0f && amount <= target) {
			amount   = target;
			velocity = 0.0f;
		}
	}
}

/**
 * @note Address: 0x800240EC
 * @note Size: 0x1BC
 */
extern "C" void fn_800240EC(CScrnQuake* quake, u32 frames)
{
	StepAxis(quake->mAmount.x, quake->mTarget.x, quake->mSpeed.x, frames);
	StepAxis(quake->mAmount.y, quake->mTarget.y, quake->mSpeed.y, frames);
	StepAxis(quake->mAmount.z, quake->mTarget.z, quake->mSpeed.z, frames);
	quake->mSign = -quake->mSign;
}

// todo: cleanup
static inline void CopyVector(CVec& destination, const Vec& source)
{
	destination.x = source.x;
	destination.y = source.y;
	destination.z = source.z;
}

/**
 * @note Address: 0x800242A8
 * @note Size: 0x184
 */
extern "C" void fn_800242A8(CScrnQuake* quake, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 time)
{
	quake->mAmount.Set(x0, y0, z0);
	quake->mAmount.x = Abs(quake->mAmount.x);
	quake->mAmount.y = Abs(quake->mAmount.y);
	quake->mAmount.z = Abs(quake->mAmount.z);
	quake->mTarget.Set(x1, y1, z1);
	quake->mTarget.x = Abs(quake->mTarget.x);
	quake->mTarget.y = Abs(quake->mTarget.y);
	quake->mTarget.z = Abs(quake->mTarget.z);
	if (time == 0.0f) {
		CopyVector(quake->mAmount, quake->mTarget);
		quake->mSpeed.Set(0.0f, 0.0f, 0.0f);
	} else {
		CopyVector(quake->mSpeed, (quake->mTarget - quake->mAmount) / (FRAME_RATE * time));
	}
	quake->mSign = 1;
}

/**
 * @note Address: 0x8002442C
 * @note Size: 0x74
 */
CVec CVec::operator/(f32 divisor) const
{
	CVec result = *this;
	result.x /= divisor;
	result.y /= divisor;
	result.z /= divisor;
	return result;
}

/**
 * @note Address: 0x800244A0
 * @note Size: 0x84
 */
CVec CVec::operator-(Vec other) const
{
	CVec result = *this;
	PSVECSubtract(&result, &other, &result);
	return result;
}

/**
 * @note Address: 0x80024524
 * @note Size: 0x34
 */
extern "C" void fn_80024524(CScrnQuake* quake)
{
	quake->mAmount.Set(0.0f, 0.0f, 0.0f);
	quake->mTarget.Set(0.0f, 0.0f, 0.0f);
	quake->mSpeed.Set(0.0f, 0.0f, 0.0f);
	quake->mSign = 1;
}
