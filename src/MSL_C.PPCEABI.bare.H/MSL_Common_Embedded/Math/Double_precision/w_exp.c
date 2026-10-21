#include "fdlibm.h"

/**
 * @note Address: 0x8015CB04
 * @note Size: 0x20
 */
f64 exp(f64 x)
{
	return __ieee754_exp(x);
}
