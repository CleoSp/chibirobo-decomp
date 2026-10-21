#include "fdlibm.h"

/**
 * @note Address: 0x8015CB44
 * @note Size: 0x20
 */
f64 log(f64 x)
{
	return __ieee754_log(x);
}
