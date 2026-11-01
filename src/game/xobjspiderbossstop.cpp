#include "game/xobjspiderboss.h"

/**
 * @note Address: 0x80103B28
 * @note Size: 0x58
 */
extern "C" void fn_80103B28(XObjSpiderBoss* spider)
{
	switch (spider->mStep) {
	case 0:
		spider->mSpiderStatus.mFlags |= 0x80000;
		spider->mSpiderStatus.mFlags |= 0x10;
		spider->mSpiderStatus.mFlags &= ~0x200U;
		spider->mSpiderStatus.mFlags &= ~0x100000U;
		++spider->mStep;
		break;
	case 1:
		break;
	}
}
