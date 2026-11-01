#include "game/hsd_jobj.h"
#include "game/ccamera.h"

/**
 * @note Address: 0x80013784
 * @note Size: 0x24
 */
// todo: weak copy
extern "C" void SetFov__7CCameraFf(CCamera* camera, f32 fov)
{
	camera->mFov = fov;
	HSD_CObjSetFov(camera, fov);
}

/**
 * @note Address: 0x800137A8
 * @note Size: 0x8
 */
// todo: weak copy
extern "C" void SetDistance__7CCameraFf(CCamera* camera, f32 distance)
{
	camera->mDistance = distance;
}

/**
 * @note Address: 0x800137B0
 * @note Size: 0x8
 */
// todo: weak copy
extern "C" void SetAngleX__7CCameraFf(CCamera* camera, f32 x)
{
	camera->mAngle.x = x;
}

/**
 * @note Address: 0x800137B8
 * @note Size: 0x8
 */
// todo: weak copy
extern "C" void SetAngleY__7CCameraFf(CCamera* camera, f32 y)
{
	camera->mAngle.y = y;
}
