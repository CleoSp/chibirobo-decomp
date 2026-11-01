#ifndef _GAME_VIDEO_H
#define _GAME_VIDEO_H

#include "types.h"
#include "Version.h"

extern "C" {
// frames per second, 50 or 60 by the video mode the player picked. europe only
extern s32 lbl_80660D4C;
}

// what the frame based timers count in a second
#if defined(VERSION_PAL)
#define FRAME_RATE ((f32)lbl_80660D4C)
#else
#define FRAME_RATE 60.0f
#endif

#endif
