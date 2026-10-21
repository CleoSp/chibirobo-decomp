#ifndef _GAME_XSOUND_H
#define _GAME_XSOUND_H

#include "types.h"

// the sound object CGame keeps at +0xE7E24, the CGame ctor stores __vt__6XSound there.
// CmdSetSoundPitch sets +0x2BA to the pitch target in cents and +0x188 to the number of
// frames to get there
struct XSound {
	u8 _00[0x188];          // _00
	s32 _188;               // _188
	u8 _18C[0x2BA - 0x18C]; // _18C
	s16 _2BA;               // _2BA
};

#endif
