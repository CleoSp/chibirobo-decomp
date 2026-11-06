#include "game/csound.h"

void AnimSfx::vf08()
{
}

s32 AnimSfx::GetSfx(AnimSfxEvent* event)
{
	if (event->mCount == 0) {
		return event->mSfx;
	}
	event->mIndex++;
	if (event->mCount <= event->mIndex) {
		event->mIndex = 0;
	}
	return event->mSfx + event->mIndex;
}

BOOL AnimSfx::CheckStop(AnimSfxEvent* event)
{
	return FALSE;
}
