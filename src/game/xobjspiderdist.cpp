#include "game/xobjspider.h"
#include "game/gmath.h"

/**
 * @note Address: 0x800EF854
 * @note Size: 0x104
 */
extern "C" f32 fn_800EF854(const Vec* first, const Vec* second)
{
	f32 x = first->x - second->x;
	f32 z = first->z - second->z;
	// todo: cleanup, summing from zero is what matches
	f32 squared = 0.0f;
	squared += x * x;
	return sqrtf(z * z + squared);
}
