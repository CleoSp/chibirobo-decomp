#include "game/xobjspiderboss.h"
#include "game/hsd_cobj.h"

/**
 * @note Address: 0x80103B80
 * @note Size: 0x340
 */
// gets back up, blinking
extern "C" void fn_80103B80(XObjSpiderBoss* spider, u32 frames)
{
	switch (spider->mStep) {
	case 0:
		spider->mSpiderStatus.mFlags |= 0x10;
		spider->SetAnim(11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		++spider->mStep;
		spider->mBlinkTimer = 0;
		spider->mSpiderStatus.mFlags &= ~0x200;
		break;
	case 1: {
		if (spider->mBlinkTimer % 30 < 15) {
			spider->vf12C(1, 128, 0, 0, 0.0f);
		} else {
			spider->vf12C(1, 0, 0, 0, 0.0f);
		}
		++spider->mBlinkTimer;
		HSD_AObj* aobj = GetAObj(spider->mJObj);
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		aobj           = GetAObj(spider->mJObj);
		f32 frame      = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
		if (end - frames <= frame) {
			spider->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			spider->mBlinkTimer = 0;
			spider->vf12C(1, 0, 0, 0, 0.0f);
			spider->mStep      = 0;
			spider->mTimer     = 0;
			spider->mPrevState = spider->mState;
			spider->mState     = 0;
			spider->mSpiderStatus.mFlags |= 0x8000;
			spider->mSpiderStatus.mFlags |= 0x02000000;
			spider->mSpiderStatus.mFlags |= 0x40000000;
			spider->mSpiderStatus.mFlags &= ~0x10;
			spider->mSpiderStatus.mFlags |= 0x80000;
			spider->mNextAction = 16;
		}
		break;
	}
	}
}
