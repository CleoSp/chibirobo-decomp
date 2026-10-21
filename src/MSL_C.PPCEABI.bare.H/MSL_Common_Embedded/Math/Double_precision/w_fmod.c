#include "fdlibm.h"

/**
 * @note Address: 0x8015CB24
 * @note Size: 0x20
 */
f64 fmod(f64 x, f64 y)
{
	return __ieee754_fmod(x, y);
}
