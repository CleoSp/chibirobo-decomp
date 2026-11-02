#include "game/xobjattachment.h"
#include <math.h>

// XObjRadar::vf90
/**
 * @note Address: 0x800D6F98
 * @note Size: 0x50
 */
extern "C" void vf90__9XObjRadarFUl(XObjRadar* object, u32 id)
{
	vf90__4XObjFUl(object, id);
	object->vf94();
	object->mYaw = object->mPitch = NAN;
}
