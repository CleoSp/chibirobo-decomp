#ifndef _GAME_CSOUND_H
#define _GAME_CSOUND_H

#include "types.h"

// CSound's layout is still in sound.cpp, CGame's XSound sits on top of it
struct CSound;
struct Vec;

// sound.cpp's table block, the distance falloff per sound kind and, japan only, the random
// pick tables
struct SoundTables {
	f32 ranges[101][4]; // _000, falloff distances far to near, one row per kind
	u8 _650[0xC];       // _650
	s32 effects[10];    // _65C
	s32 selections[5];  // _684
};
typedef char SoundTables_size_check[sizeof(SoundTables) == 0x698 ? 1 : -1];

extern "C" {
extern SoundTables lbl_80210C10;

// CSound, sound.cpp. sfx ids from 0xFFFF0000 up are music tracks
void fn_8001DC20(CSound* sound, u32 group, f32 strength, u8 first, u8 second);
u8 fn_8001DF00(CSound* sound, Vec* pos, s32 kind);
BOOL fn_8001E044(CSound* sound, const char* name);
void fn_8001E1A4(CSound* sound, s32 sfx, u16 frames);
void fn_8001E2CC(CSound* sound, s32 sfx, u16 frames);
void fn_8001E468(CSound* sound, s32 sfx, u8 vol, u16 frames);
void fn_8001E4D4(CSound* sound, s32 track);
void fn_8001E504(CSound* sound, s32 sfx);
void fn_8001E590(CSound* sound, s32 track);
void fn_8001E5C0(CSound* sound, s32 sfx, s32 arg2);
void fn_8001E630(CSound* sound, s32 track);
// stop, is it playing, and play, which gives back the voice or stream id
BOOL fn_8001E660(CSound* sound, s32 sfx);
BOOL fn_8001E6C8(CSound* sound, s32 sfx);
s32 fn_8001E904(CSound* sound, s32 sfx, u8 vol);

// hsd axdriver.c: key off an sfx, a pal only one, pause an sfx, stop every stream slot
BOOL fn_801E62A8(s32 sfx);
BOOL fn_801E8CC0(void);
BOOL fn_801E94E4(s32 sfx);
void fn_801E9FA4(void);
}

// one sound an anim plays between two frames, a negative mAnim ends an object's table
struct AnimSfxEvent {
	s32 mAnim;  // _00
	f32 mStart; // _04
	f32 mEnd;   // _08
	s32 mKind;  // _0C, 1 plays when the frame passes mStart or mEnd
	s32 mSfx;   // _10, a track id from 0xFFFF0000 up
	u8 _14;     // _14
	u8 mCount;  // _15, mSfx and the next ones are played in turn
	u8 mIndex;  // _16
	u8 _17;     // _17
};
typedef char AnimSfxEvent_size_check[sizeof(AnimSfxEvent) == 0x18 ? 1 : -1];

// plays an anim's sound events as its frames pass, sound.cpp, vtable at 0x802112A8
class AnimSfx {
public:
	AnimSfxEvent* mEvents; // _00, ends at an anim id below 0
	s32 mAnimId;           // _04
	f32 mFrame;            // _08, the last frame Update saw
	// _0C vtable

	virtual void vf08();                                            // _08, fn_8001DB44
	virtual void Update(s32 id, f32 frame, u8 volume);              // _0C, fn_8001D9B0
	virtual BOOL CheckStart(AnimSfxEvent* event, f32 from, f32 to); // _10, fn_8001DB98, starts the event
	virtual BOOL CheckStop(AnimSfxEvent* event);                    // _14, fn_8001DB90, stops it
	virtual s32 GetSfx(AnimSfxEvent* event);                        // _18, fn_8001DB48, its sfx
};
typedef char AnimSfx_size_check[sizeof(AnimSfx) == 0x10 ? 1 : -1];

#endif
