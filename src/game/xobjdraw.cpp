#include "game/xobj.h"
#include "Version.h"

/**
 * @note Address: 0x80090864
 * @note Size: 0x5C
 */
extern "C" void fn_80090864(XObj* object)
{
	if (object->mStatus.mFlags & 8ULL) {
		object->vfBC(&object->mMoveTranslate, 1.0f);
	}
}
