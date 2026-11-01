#include "game/xobjspidersmall.h"

/**
 * @note Address: 0x800FC930
 * @note Size: 0x40
 */
extern "C" void fn_800FC930(XObjSpiderSmall* spider)
{
	spider->mStatus.mFlags &= ~0x10000ULL;
	spider->mSpiderStatus.mFlags |= 0x80000;
	spider->mSpiderStatus.mFlags &= ~0x100000U;
}
