#include "game/hsd_jobj.h"
#include "game/ut.h"
extern "C" {
#include <dolphin/mtx/vec.h>
}

/**
 * @note Address: 0x8000F93C
 * @note Size: 0x84
 */
CVec CVec::operator+(Vec other) const
{
	CVec result = *this;
	PSVECAdd(&result, &other, &result);
	return result;
}

/**
 * @note Address: 0x8000F9C0
 * @note Size: 0x80
 */
CVec CVec::operator*(f32 scale) const
{
	CVec result = *this;
	PSVECScale(&result, &result, scale);
	return result;
}
