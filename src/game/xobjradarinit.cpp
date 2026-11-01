#include "game/xobjattachment.h"
#include <math.h>

// XObjRadar::vf90
/**
 * @note Address: 0x800D6F98
 * @note Size: 0x50
 */
extern "C" void fn_800D6F98(XObjRadar* object, u32 id)
{
	fn_800929C4(object, id);
	object->vf94();
	object->mYaw = object->mPitch = NAN;
}
