#include "game/xobjhitevent.h"
#include "game/globals.h"

// XObjDust::OnHitFromPosition
/**
 * @note Address: 0x800DE424
 * @note Size: 0x1E4
 */
extern "C" void OnHitFromPosition__8XObjDustFRC3Vecf(XObjDust* object, const Vec&, f32 strength)
{
	if (object->GetObjFlags() & 1) {
		object->mStatus.mFlags |= 0x4000000000000000ULL;
		if (strength >= (s32)GetPlayerActionParameter(31) / 100.0f)
			object->mEventArg = 2001;
		else
			object->mEventArg = 2000;
	} else if (object->mStatus.mFlags & 0x0000080000000000ULL) {
		if (strength >= (s32)GetPlayerActionParameter(31) / 100.0f)
			StartObjectHitEvent(object, -1, 2001);
		else
			StartObjectHitEvent(object, -1, 2000);
	}
}
