#ifndef _GAME_PROGRESSIVE_H
#define _GAME_PROGRESSIVE_H

#include "types.h"

// the progressive scan question at boot, kept between resets. class unknown
struct ResetState {
	s32 _00; // _00, the step
	s32 _04; // _04, the scan mode to switch to, -1 for none
	f32 _08; // _08, frames left
	f32 _0C; // _0C, the question's alpha
	s32 _10; // _10, it was asked before the reset
	s32 _14; // _14, interlaced was picked
};
STATIC_ASSERT(sizeof(ResetState) == 0x18);

extern "C" ResetState lbl_80210730;

#if defined(VERSION_PAL)
extern "C" f32 gProgressiveFadeFrames;
#endif

#endif
