#include "fdlibm.h"

/**
 * @note Address: 0x8015CB64
 * @note Size: 0x20
 */
f64 pow(f64 x, f64 y)
{
	return __ieee754_pow(x, y);
}
