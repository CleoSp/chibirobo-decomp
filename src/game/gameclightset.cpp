#include "game/hsd_jobj.h"
#include "game/clight.h"

/**
 * @note Address: 0x8000DF78
 * @note Size: 0x30
 */
// todo: weak copy
extern "C" void SetPosition__6CLightFfff(CLight* light, f32 x, f32 y, f32 z)
{
	light->mPosition.x = x;
	light->mPosition.y = y;
	light->mPosition.z = z;
	HSD_LObjSetPosition(light, &light->mPosition);
}
