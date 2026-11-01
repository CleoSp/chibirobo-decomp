#include "game/xact.h"
#include "game/xchr.h"
#include "game/cgame.h"
extern "C" {
#include <dolphin/mtx/vec.h>
}
#include <math.h>

/**
 * @note Address: 0x800BE738
 * @note Size: 0x1F8
 */
// the radar's pick, the nearest drawn object that isn't chibi robo himself
extern "C" XObj* fn_800BE738(XActRadar* action)
{
	CVec position = action->mOwner->mMoveTranslate;
	f32 nearest   = NAN;
	XObj* found   = NULL;
	for (s32 i = 0; i < lbl_8025C400.mObjectCount; ++i) {
		XObj* object = lbl_8025C400.mObjects[i];
		if ((object->mStatus.mFlags & 1ULL) && (object->mStatus.mFlags & 0x10000ULL) && !(object->mStatus.mFlags & 0x4000000000000000ULL)
		    && (object->mStatus.mFlags & 0x80000ULL) && (s32)action->mOwner->mId != (s32)object->mId && object->GetKind() < 10000) {
			f32 distance;
			if (found == NULL) {
				distance = PSVECDistance(&position, &object->mMoveTranslate);
			} else {
				distance = PSVECDistance(&position, &object->mMoveTranslate);
				if (!(distance < nearest))
					distance = NAN;
			}
			if (__fpclassifyf(distance) != FP_NAN) {
				found   = object;
				nearest = distance;
			}
		}
	}
	return found;
}
