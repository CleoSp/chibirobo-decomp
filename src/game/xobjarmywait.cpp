#include "game/xobjarmy.h"

/**
 * @note Address: 0x800E8564
 * @note Size: 0x194
 */
extern "C" void fn_800E8564(XObjArmy* army)
{
	switch (army->mStep) {
	case 0:
		army->mTimer = 30;
		army->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		army->SetMoveDir(army->mMoveTranslate, 1.0f);
		army->SetRotateY(HSD_JObjGetRotationY(army->mJObj), 1.0f);
		++army->mStep;
		break;
	case 1:
		if (--army->mTimer == 0) {
			army->mStep = 0;
			switch (army->mNext) {
			case 1:
				army->mStatus.mFlags |= 8ULL;
				army->mFunc = lbl_8022A6A4;
				break;
			case 2:
				army->mFunc = lbl_8022A6B0[0];
				break;
			default:
				army->mFunc = NULL;
				break;
			}
		}
		break;
	}
}
