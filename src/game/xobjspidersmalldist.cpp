#include "game/gmath.h"
#include "Dolphin/mtx.h"

/**
 * @note Address: 0x800F841C
 * @note Size: 0x104
 */
// distance on the floor
extern "C" f32 fn_800F841C(const Vec* a, const Vec* b)
{
	f32 x = a->x - b->x;
	f32 y = 0.0f;
	f32 z = a->z - b->z;
	return sqrtf(x * x + y * y + z * z);
}

/**
 * @note Address: 0x800F8520
 * @note Size: 0x114
 */
extern "C" f32 fn_800F8520(const Vec* a, const Vec* b)
{
	f32 x = a->x - b->x;
	f32 y = a->y - b->y;
	f32 z = a->z - b->z;
	return sqrtf(x * x + y * y + z * z);
}
