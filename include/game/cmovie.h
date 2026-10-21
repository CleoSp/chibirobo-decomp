#ifndef _GAME_CMOVIE_H
#define _GAME_CMOVIE_H

#include "types.h"
#include "game/THPSimple.h"

// plays one THP movie through the THPSimple player. the class name is RTTI, the methods and
// CGame holds one, the vtable and dtor live with it

// mState steps, errors get stored as plain negative numbers
enum {
	CMOVIE_STATE_IDLE,
	CMOVIE_STATE_OPEN,
	CMOVIE_STATE_INFO,
	CMOVIE_STATE_PRELOAD,
	CMOVIE_STATE_READY,
	CMOVIE_STATE_START,
	CMOVIE_STATE_PLAY,
};

class CMovie {
public:
	u32 mState;              // _00, unsigned, so the negative errors pass every >= START check, oops
	char mPath[0x80];        // _04
	BOOL mLoop;              // _84
	BOOL mAlpha;             // _88, keep the movie's alpha when drawing
	THPVideoInfo mVideoInfo; // _8C
	THPAudioInfo mAudioInfo; // _98
	void* mBuffer;           // _A8
	s32 mFrame;              // _AC
	s32 mLastFrame;          // _B0
	// _B4 vtable

	virtual ~CMovie(); // _08

	void EndDraw();
	s32 BeginDraw();
	void Draw(f32 x, f32 y, f32 w, f32 h);
	s32 DrawFrame(f32 x, f32 y, f32 w, f32 h);
	s32 Update();
	void Stop();
	void Start(const char* path, BOOL loop, BOOL alpha, BOOL sound);
	void Init();
};
STATIC_ASSERT(sizeof(CMovie) == 0xB8);

#endif
