#include "game/xobjspider.h"

/**
 * @note Address: 0x800F3180
 * @note Size: 0x4
 */
extern "C" void vfB8__10XObjSpiderFv(XObjSpider* spider)
{
}

/**
 * @note Address: 0x800F3184
 * @note Size: 0x5C
 */
extern "C" void vfAC__10XObjSpiderFv(XObjSpider* spider)
{
	spider->vfB0(NULL);
	bool sideways = false;
	if (spider->mCarryMode == 3)
		sideways = true;
	fn_800EAB58(&spider->mCarry, spider->mSlot, sideways);
}
