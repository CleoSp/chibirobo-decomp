#include "game/cgame.h"

/**
 * @note Address: 0x8000F8F4
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" void SetInterest__7CCameraFRC3Vec(CCamera* camera, const Vec* interest)
{
	camera->SetInterest(interest->x, interest->y, interest->z);
}

/**
 * @note Address: 0x8000F92C
 * @note Size: 0x10
 */
// todo: weak copy
extern "C" void SetInterest__7CCameraFfff(CCamera* camera, f32 x, f32 y, f32 z)
{
	camera->mInterest.x = x;
	camera->mInterest.y = y;
	camera->mInterest.z = z;
}

/**
 * @note Address: 0x8000F93C
 * @note Size: 0x84
 */
CVec CVec::operator+(Vec v) const
{
	CVec sum = *this;
	PSVECAdd(&sum, &v, &sum);
	return sum;
}

/**
 * @note Address: 0x8000F9C0
 * @note Size: 0x80
 */
CVec CVec::operator*(f32 s) const
{
	CVec product = *this;
	PSVECScale(&product, &product, s);
	return product;
}
