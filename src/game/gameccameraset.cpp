#include "game/hsd_jobj.h"
#include "game/ccamera.h"

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
