#include "fdlibm.h"

/**
 * @note Address: 0x8015CAA4
 * @note Size: 0x20
 */
f64 acos(f64 x)
{
	return __ieee754_acos(x);
}
