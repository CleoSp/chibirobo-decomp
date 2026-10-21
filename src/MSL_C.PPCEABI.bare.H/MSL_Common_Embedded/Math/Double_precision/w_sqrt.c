#include "fdlibm.h"

/**
 * @note Address: 0x8015CF24
 * @note Size: 0x20
 */
f64 sqrt(f64 x)
{
	return __ieee754_sqrt(x);
}
