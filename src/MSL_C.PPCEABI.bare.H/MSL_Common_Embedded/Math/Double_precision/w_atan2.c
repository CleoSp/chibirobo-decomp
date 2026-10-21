#include "fdlibm.h"

/**
 * @note Address: 0x8015CAE4
 * @note Size: 0x20
 */
f64 atan2(f64 y, f64 x)
{
	return __ieee754_atan2(y, x);
}
