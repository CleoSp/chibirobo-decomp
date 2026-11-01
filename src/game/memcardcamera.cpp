#include "game/hsd_jobj.h"
#include "game/ccamera.h"

// todo: weak copy
/**
 * @note Address: 0x80087C88
 * @note Size: 0x38
 */
// CCamera::SetAngle(const Vec&)
extern "C" void SetAngle__7CCameraFRC3Vec(CCamera* camera, const Vec& angle)
{
	camera->SetAngle(angle.x, angle.y, angle.z);
}
