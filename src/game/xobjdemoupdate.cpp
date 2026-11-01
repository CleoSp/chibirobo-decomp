#include "game/cgame.h"
#include "game/xobjdemo.h"
#include "game/xobjs.h"

// todo: cleanup
static inline bool FadeDone()
{
	return 0.0f == lbl_8025C400._3E90[0] && 0.0f == lbl_8025C400._3E90[1] && 0.0f == lbl_8025C400._3E90[2] && 0.0f == lbl_8025C400._3E90[3];
}

/**
 * @note Address: 0x800280B4
 * @note Size: 0x244
 */
// XObjDemo's update, steps the camera and light anims with the model
extern "C" void fn_800280B4(XObjDemo* demo, u32 frames)
{
	if (demo->mState == 0) {
		return;
	}
	if (demo->mState == 2 && FadeDone()) {
		demo->mState = 0;
		return;
	}
	fn_80090ED4(demo, frames);
	if (demo->mArchive.mDesc[0] != NULL && demo->mCameraAnim >= 0) {
		demo->mCamera.SetAnimRate((f32)frames);
		demo->mCamera.Anim();
	}
	for (s32 i = 0; i < demo->mLightCount; ++i) {
		if (demo->mArchive.mDesc[i + 1] != NULL && demo->mLightAnims[i] >= 0) {
			demo->mLights[i].SetAnimRate((f32)frames);
			demo->mLights[i].Anim();
		}
	}
	if (demo->mLoop) {
		// no null check on the aobj, retail does this
		if (GetAObj(demo->mJObj)->flags & 0x44000000) {
			demo->mState = 1;
			demo->SetPos(demo->mMoveTranslate, 0);
			demo->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
	}
}
