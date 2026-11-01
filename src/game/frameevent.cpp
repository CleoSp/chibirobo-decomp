#include "game/cgame.h"
#include "game/csound.h"

struct FrameEvent;

// the frame pair fn_800A7E1C steps, XObjChr _94F8
struct FrameEventTime {
	f32 mFrame;     // _00
	f32 mLastFrame; // _04
};

// a frame window check, 1 starts the event, -1 stops it, 0 leaves it
typedef s32 (*FrameEventFunc)(const u32* aobjFlags, const FrameEventTime* time, const FrameEvent* event);

// one entry of an anim's frame event table, like lbl_80226324
struct FrameEvent {
	s32 mAnim;          // _00
	f32 mStart;         // _04
	f32 mEnd;           // _08
	FrameEventFunc _0C; // _0C
	FrameEventFunc _10; // _10
	s32 mId;            // _14, a sound or particle
};
STATIC_ASSERT(sizeof(FrameEvent) == 0x18);

// stop once the sound is playing
/**
 * @note Address: 0x800A8454
 * @note Size: 0x40
 */
extern "C" s32 fn_800A8454(const u32* aobjFlags, const FrameEventTime* time, const FrameEvent* event)
{
	return fn_8001E6C8((CSound*)&lbl_8025C400.mSound, event->mId) ? -1 : 0;
}

// start while the sound isn't playing
/**
 * @note Address: 0x800A8494
 * @note Size: 0x3C
 */
extern "C" s32 fn_800A8494(const u32* aobjFlags, const FrameEventTime* time, const FrameEvent* event)
{
	return !fn_8001E6C8((CSound*)&lbl_8025C400.mSound, event->mId);
}

// keep the sound playing inside the window only
/**
 * @note Address: 0x800A84D0
 * @note Size: 0x90
 */
extern "C" s32 fn_800A84D0(const u32* aobjFlags, const FrameEventTime* time, const FrameEvent* event)
{
	if (time->mFrame >= event->mStart && time->mFrame < event->mEnd) {
		if (!fn_8001E6C8((CSound*)&lbl_8025C400.mSound, event->mId))
			return 1;
	} else {
		if (fn_8001E6C8((CSound*)&lbl_8025C400.mSound, event->mId))
			return -1;
	}
	return 0;
}

// keep the sound playing until the anim ends
/**
 * @note Address: 0x800A8560
 * @note Size: 0x7C
 */
extern "C" s32 fn_800A8560(const u32* aobjFlags, const FrameEventTime* time, const FrameEvent* event)
{
	if (*aobjFlags & 0x44000000) {
		if (fn_8001E6C8((CSound*)&lbl_8025C400.mSound, event->mId))
			return -1;
	} else {
		if (!fn_8001E6C8((CSound*)&lbl_8025C400.mSound, event->mId))
			return 1;
	}
	return 0;
}

// fires on the frame it crosses mStart or mEnd, a loop back counts as crossing
/**
 * @note Address: 0x800A85DC
 * @note Size: 0x64
 */
extern "C" s32 fn_800A85DC(const u32* aobjFlags, const FrameEventTime* time, const FrameEvent* event)
{
	if (((time->mLastFrame < event->mStart || time->mLastFrame > time->mFrame) && time->mFrame >= event->mStart)
	    || ((time->mLastFrame < event->mEnd || time->mLastFrame > time->mFrame) && time->mFrame >= event->mEnd))
		return 1;
	return 0;
}

// fires on the frame it crosses mStart
/**
 * @note Address: 0x800A8640
 * @note Size: 0x3C
 */
extern "C" s32 fn_800A8640(const u32* aobjFlags, const FrameEventTime* time, const FrameEvent* event)
{
	if ((time->mLastFrame < event->mStart || time->mLastFrame > time->mFrame) && time->mFrame >= event->mStart)
		return 1;
	return 0;
}
