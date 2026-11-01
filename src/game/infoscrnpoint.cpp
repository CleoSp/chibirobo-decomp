#include "game/infopoint.h"
#include "game/cgame.h"

static inline void SetPointFlags(u8 value)
{
	InfoPointPool* pool = &lbl_80366E58;
	InfoPoint* point1   = &pool->mPoint1;
	InfoPoint* point0   = &pool->mPoint0;
	InfoPoint* point2   = &pool->mPoint2;
	point1->_75         = value;
	point0->_75         = value;
	point2->_75         = value;
}

/**
 * @note Address: 0x80068B48
 * @note Size: 0x28
 */
extern "C" void fn_80068B48(void*)
{
	SetPointFlags(0);
}
/**
 * @note Address: 0x80068B70
 * @note Size: 0x28
 */
extern "C" void fn_80068B70(void*)
{
	SetPointFlags(1);
}
/**
 * @note Address: 0x80068B98
 * @note Size: 0x14
 */
extern "C" void fn_80068B98(void*)
{
	lbl_803670CC.mState = 2;
}
/**
 * @note Address: 0x80068BAC
 * @note Size: 0x14
 */
extern "C" void fn_80068BAC(void*)
{
	lbl_803670CC.mVisible = 0;
}
/**
 * @note Address: 0x80068BC0
 * @note Size: 0x14
 */
extern "C" void fn_80068BC0(void*)
{
	lbl_803670CC.mVisible = 1;
}
/**
 * @note Address: 0x80068BD4
 * @note Size: 0x14
 */
extern "C" void fn_80068BD4(void*)
{
	lbl_80366FA4.mVisible = 0;
}
/**
 * @note Address: 0x80068BE8
 * @note Size: 0x14
 */
extern "C" void fn_80068BE8(void*)
{
	lbl_80366FA4.mVisible = 1;
}
/**
 * @note Address: 0x80068BFC
 * @note Size: 0x14
 */
extern "C" void fn_80068BFC(void*)
{
	lbl_80367038.mVisible = 0;
}
/**
 * @note Address: 0x80068C10
 * @note Size: 0x14
 */
extern "C" void fn_80068C10(void*)
{
	lbl_80367038.mVisible = 1;
}

// where the top counter sits
/**
 * @note Address: 0x80068C24
 * @note Size: 0x68
 */
extern "C" void fn_80068C24(void*, f32* x, f32* y)
{
	f32 height = lbl_8025C400._417C * 0.5f + lbl_8025C400._4174;
	f32 limit  = height >= 418.0f ? 418.0f : height;
	f32 top    = limit - 64.0f - 40.0f - 9.0f;
	if (x != NULL)
		*x = 60.0f;
	if (y != NULL)
		*y = top;
}

// steps value toward target, frozen while the game is paused
/**
 * @note Address: 0x80068C8C
 * @note Size: 0x84
 */
extern "C" void fn_80068C8C(f32* value, f32* target, f32 step)
{
	if (lbl_8025C400.mStatus.mFlags & 0x0000200000000000ULL)
		return;
	if (*value > *target) {
		f32 next = *value - step;
		*value   = next;
		if (next < *target)
			*value = *target;
	}
	if (*value < *target) {
		f32 next = *value + step;
		*value   = next;
		if (next > *target)
			*value = *target;
	}
}
