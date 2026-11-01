#include "game/xact.h"
#include <math.h>

// todo: weak copy
/**
 * @note Address: 0x800A2BD0
 * @note Size: 0x64
 */
extern "C" s32 fn_800A2BD0(f32 value)
{
	s32 bits = *(s32*)&value;
	switch (bits & 0x7F800000) {
	case 0x7F800000:
		return (bits & 0x007FFFFF) ? FP_NAN : FP_INFINITE;
	case 0:
		return (bits & 0x007FFFFF) ? FP_SUBNORMAL : FP_ZERO;
	}
	return FP_NORMAL;
}

// todo: weak copy
/**
 * @note Address: 0x800A2C34
 * @note Size: 0x14
 */
extern "C" bool fn_800A2C34(const XAction* action, u32 state)
{
	return state == action->mState;
}

// todo: weak copy
/**
 * @note Address: 0x800A2C48
 * @note Size: 0x28
 */
extern "C" CVec fn_800A2C48(const Vec& value)
{
	return CVec(value);
}
