#include "fdlibm.h"

/**
 * @note Address: 0x8015CAC4
 * @note Size: 0x20
 */
f64 asin(f64 x)
{
	return __ieee754_asin(x);
}
