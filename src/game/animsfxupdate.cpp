#include "game/cgame.h"
#include "game/soundplayback.h"

/**
 * @note Address: 0x8001D9A8
 * @note Size: 0x8
 */
// CCameraActionMap's GetTarget
extern "C" XObj* GetTarget__16CCameraActionMapFv(CCameraActionMap* map)
{
	return map->_44;
}

/**
 * @note Address: 0x8001D9B0
 * @note Size: 0x194
 */
// NONMATCHING: the stop's sound fields are addressed off the game object, not the sound
#pragma push
#pragma opt_strength_reduction off
void AnimSfx::Update(s32 anim, f32 frame, u8 vol)
{
	s32 prev = mAnimId;

	if (prev != anim) {
		mAnimId = anim;
		mFrame  = 0.0f;
	}
	AnimSfxEvent* event;
	for (s32 i = 0; mEvents != NULL && (event = &mEvents[i])->mAnim >= 0; i++) {
		if (event->mAnim == anim || event->mAnim == prev) {
			if (prev != anim && event->mAnim == prev && CheckStop(event)) {
				stopSfx(&gGame.mSound, GetSfx(event));
			}
			if (event->mAnim == anim && CheckStart(event, mFrame, frame)) {
				PlayGameSound(&gGame.mSound, GetSfx(event), vol);
			}
		}
	}
	mFrame = frame;
}
#pragma pop
