#include "game/xobj.h"

// todo: weak copy
/**
 * @note Address: 0x800A2E54
 * @note Size: 0x34
 */
extern "C" BOOL fn_800A2E54(XObj* object)
{
	return (object->mStatus.mFlags & 1ULL) != 0;
}
