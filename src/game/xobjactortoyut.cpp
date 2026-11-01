#include "game/ccameraaction.h"
#include "game/gmath.h"

/**
 * @note Address: 0x801219B0
 * @note Size: 0x104
 */
extern "C" f32 fn_801219B0(const Vec* first, const Vec* second)
{
	f32 x       = first->x - second->x;
	f32 z       = first->z - second->z;
	f32 squared = 0.0f;
	squared += x * x;
	return sqrtf(z * z + squared);
}
