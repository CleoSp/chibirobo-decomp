#include "game/cgame.h"
#include "game/csound.h"
#include "game/csound.h"

// todo: cleanup
static inline bool IsTrack(s32 sfx)
{
	if (sfx == -1) {
		return false;
	}
	return (sfx & 0xFFFF0000) == 0xFFFF0000;
}

/**
 * @note Address: 0x8001D9B0
 * @note Size: 0x194
 */
// AnimSfx::Update, stops the old anim's sounds and starts the ones the frame just passed
extern "C" void fn_8001D9B0(AnimSfx* sfx, s32 anim, f32 frame, u8 vol)
{
	s32 prevAnim = sfx->mAnimId;
	if (prevAnim != anim) {
		sfx->mAnimId = anim;
		sfx->mFrame  = 0.0f;
	}
	XSound* sound = &lbl_8025C400.mSound;
	AnimSfxEvent* event;
	for (u32 i = 0; sfx->mEvents != NULL && (event = &sfx->mEvents[i])->mAnim >= 0; ++i) {
		if (event->mAnim == anim || event->mAnim == prevAnim) {
			if (prevAnim != anim && event->mAnim == prevAnim && sfx->CheckStop(event)) {
				s32 id = sfx->GetSfx(event);
				if (IsTrack(id)) {
					sound->_2B4 = 0;
					sound->_2C8 = 0;
					fn_801E9FA4();
				} else {
					fn_801E62A8(id);
				}
			}
			if (event->mAnim == anim && sfx->CheckStart(event, sfx->mFrame, frame)) {
				fn_8001E904((CSound*)sound, sfx->GetSfx(event), vol);
			}
		}
	}
	sfx->mFrame = frame;
}

/**
 * @note Address: 0x8001DB44
 * @note Size: 0x4
 */
extern "C" void fn_8001DB44()
{
}

/**
 * @note Address: 0x8001DB48
 * @note Size: 0x48
 */
extern "C" s32 fn_8001DB48(AnimSfx* sfx, AnimSfxEvent* event)
{
	if (event->mCount == 0) {
		return event->mSfx;
	}
	++event->mIndex;
	if (event->mCount <= event->mIndex) {
		event->mIndex = 0;
	}
	return (s32)((u32)event->mSfx + event->mIndex);
}

/**
 * @note Address: 0x8001DB90
 * @note Size: 0x8
 */
extern "C" BOOL fn_8001DB90(AnimSfx* sfx, AnimSfxEvent* event)
{
	return FALSE;
}

/**
 * @note Address: 0x8001DB98
 * @note Size: 0x78
 */
extern "C" BOOL fn_8001DB98(AnimSfx* sfx, AnimSfxEvent* event, f32 prev, f32 frame)
{
	f32 start = event->mStart;
	f32 end   = event->mEnd;
	switch (event->mKind) {
	case 1:
		if (0.0f <= start && prev <= start && start < frame) {
			return TRUE;
		}
		if (0.0f <= end && prev <= end && end < frame) {
			return TRUE;
		}
		break;
	}
	return FALSE;
}

#if defined(VERSION_JP)
// japan only, plays one of a group's random picks and remembers it

extern "C" void fn_8001DB48_GGTJ01_01(CSound* sound, s32 group, s32 pick)
{
	if (group < 0 || (u32)group >= 5) {
		return;
	}
	if (pick < 0 || (u32)pick >= 10) {
		return;
	}
	s32* effects = lbl_80210C10.effects;
	fn_8001E904(sound, effects[pick], 255);
	s32* selections   = lbl_80210C10.selections;
	selections[group] = pick;
}

extern "C" s32 fn_8001DBC8(CSound* sound, s32 group)
{
	if (group < 0 || (u32)group >= 5) {
		return -1;
	}
	s32* selections = lbl_80210C10.selections;
	return selections[group];
}
#endif

/**
 * @note Address: 0x8001DC10
 * @note Size: 0x10
 */
extern "C" void fn_8001DC10(XSound* sound)
{
	sound->mQueueWrite = 0;
	sound->mQueueRead  = 0;
}
