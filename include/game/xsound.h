#ifndef _GAME_XSOUND_H
#define _GAME_XSOUND_H

#include "types.h"

// one sfx name for the stage bank
struct SfxLoadRequest {
	const char* mName; // _00
	s32 mBank;         // _04
	s32 mResult;       // _08
	u8 mLoaded;        // _0C
};
typedef char SfxLoadRequest_size_check[sizeof(SfxLoadRequest) == 0x10 ? 1 : -1];

// the sound object CGame keeps at +0xE7E24, the CGame ctor stores __vt__6XSound there.
// CmdSetSoundPitch sets +0x2BA to the pitch target in cents and +0x188 to the number of
// frames to get there. sound.cpp has the CSound part spelled out
struct XSound {
	u8 _00[0x4];                      // _00, its vtable
	s32 mLoadState;                   // _04, fn_8001F97C's step
	u8 mFile[0xEC];                   // _08, a CFile, the sample table is read through it
	s32 mSampleBank;                  // _F4
	u8 mSampleReady;                  // _F8
	u8 _F9[0xFC - 0xF9];              // _F9
	s32 mDefaultTrack;                // _FC
	s32 mStageBank;                   // _100, the stage's sfx bank, cleared on every stage change
	u8 _104[0x188 - 0x104];           // _104
	s32 _188;                         // _188
	u8 _18C[0x1A0 - 0x18C];           // _18C
	u8 mQueueRead;                    // _1A0
	u8 mQueueWrite;                   // _1A1
	u8 _1A2[0x2B4 - 0x1A2];           // _1A2
	s32 _2B4;                         // _2B4
	u8 _2B8[0x2BA - 0x2B8];           // _2B8
	s16 _2BA;                         // _2BA
	u8 _2BC[0x2C4 - 0x2BC];           // _2BC
	s32 _2C4;                         // _2C4, fade length
	s32 _2C8;                         // _2C8
	u8 _2CC;                          // _2CC, volume for the all-sfx id
	u8 _2CD[0x2E0 - 0x2CD];           // _2CD
	SfxLoadRequest mLoadRequests[16]; // _2E0, stage sfx names waiting for HSD_SynthSFXLoad
	s32 mLoadRequestCount;            // _3E0
};
typedef char XSound_size_check[sizeof(XSound) == 0x3E4 ? 1 : -1];

#endif
