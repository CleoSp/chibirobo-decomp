#include "game/objb_leticker_shared.h"

static inline void PlayLetickerSound(XObjLeticker& obj)
{
	s32 anim  = obj.mAnimId;
	f32 frame = GetAnimFrame(obj.mJObj);
	obj.mSfx.Play(anim, fn_8001DF00(GAME_SOUND, &obj.mMoveTranslate, 0), frame);
}

/**
 * @note Address: 0x800E64A8
 * @note Size: 0x2E8
 */
void CCameraActionLeticker::Update(u32 step)
{
	CPad pad(0);
	u64 buttons     = GAME_FLAGS & 0x80000000;
	u32 leftButton  = buttons ? HSD_PAD_DPADLEFT : HSD_PAD_CSTICK_LEFT;
	u32 rightButton = buttons ? HSD_PAD_DPADRIGHT : HSD_PAD_CSTICK_RIGHT;

	if (mTurnTime == 0) {
		if (pad.mPad->mTrigger & leftButton) {
			if (CanTurn(mTurnTo - 1.5707964f)) {
				mTurnTo -= 1.5707964f;
				mTurnSpeed = -0.08726646f;
				mTurnTime  = 18;
			}
		} else if (pad.mPad->mTrigger & rightButton) {
			if (CanTurn(1.5707964f + mTurnTo)) {
				mTurnTo += 1.5707964f;
				mTurnSpeed = 0.08726646f;
				mTurnTime  = 18;
			}
		}
	} else {
		mTurnTime -= step;
		if (mTurnTime <= 0) {
			mAngle    = mTurnTo;
			mTurnTime = 0;
		} else {
			mAngle += mTurnSpeed * step;
		}
	}
	CCameraActionLadr::Update(step);
}
