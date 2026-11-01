#include "game/xobjspider.h"
#include "game/cgame.h"

/**
 * @note Address: 0x800EDEE4
 * @note Size: 0x16C
 */
extern "C" void fn_800EDEE4(XObjSpider* spider)
{
	switch (spider->mPhase) {
	default:
		break;
	case 0:
		spider->mFlags.mFlags |= 0x40;
		spider->mFlags.mFlags |= 0x400;
		spider->mFlags.mFlags |= 0x8000;
		spider->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		++spider->mPhase;
		break;
	case 1:
		if (lbl_8025C400._5D080 != NULL)
			break;
		spider->mFlags.mFlags &= ~0x40U;
		spider->mFlags.mFlags &= ~0x80U;
		spider->mFlags.mFlags &= ~0x100U;
		spider->mFlags.mFlags &= ~0x8000U;
		for (s32 i = 0; i < 16; ++i) {
			if (lbl_8037E600[i] == -1) {
				spider->mSlotObj = lbl_8037E5C0[i];
				spider->mSlot    = i;
				lbl_8037E600[i]  = spider->_258;
				break;
			}
		}
		spider->mPhase     = 0;
		spider->mCounter   = 0;
		spider->mPrevState = spider->mState;
		spider->mState     = 4;
		break;
	}
}
