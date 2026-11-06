#include "game/xobjhitevent.h"
#include "game/globals.h"

// XObjDust::vf138
/**
 * @note Address: 0x800DE424
 * @note Size: 0x1E4
 */
extern "C" void vf138__8XObjDustFRC3Vecf(XObjDust* object, f32 strength)
{
	if (object->GetObjFlags() & 1) {
		object->mStatus.mFlags |= 0x4000000000000000ULL;
		if (strength >= (s32)fn_8006EBC8(31) / 100.0f)
			object->mEventArg = 2001;
		else
			object->mEventArg = 2000;
	} else if (object->mStatus.mFlags & 0x0000080000000000ULL) {
		if (strength >= (s32)fn_8006EBC8(31) / 100.0f)
			StartObjectHitEvent(object, -1, 2001);
		else
			StartObjectHitEvent(object, -1, 2000);
	}
}
