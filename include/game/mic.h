#ifndef _GAME_MIC_H
#define _GAME_MIC_H

#include "types.h"

// the mic on the memory card slots and the m2s stream it feeds
struct MicChannel {
	s32 mResult;   // _00, set by the mount callback, -3 once the mic is pulled out
	u32 mDeviceID; // _04
	s32 mState;    // _08, 0 probe, 1 mount, 2 wait, 3 ready
	s16* mBuffer;  // _0C, the mount buffer
};
STATIC_ASSERT(sizeof(MicChannel) == 0x10);

extern "C" {
// one per slot
extern MicChannel lbl_8020FC3C[2];
// the slot in use, -1 for none
extern s32 lbl_80659ECC;
}

#endif
