#ifndef _axdriver_h_
#define _axdriver_h_

#include <dolphin/types.h>

// the sound effect calls the game makes from units with the hsd include path,
// src/sysdolphin/baselib/axdriver.c has the rest
BOOL HSD_AudioSFXKeyOff(s32 vid);
BOOL HSD_AudioSFXCheck(s32 vid);

#endif
