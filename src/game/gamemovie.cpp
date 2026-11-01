#include "Version.h"
#include "game/cmovie.h"
#include "game/cgame.h"
#include "game/cthread.h"
#include "Dolphin/os.h"
#include "Dolphin/vi.h"
#include <math.h>

extern "C" {
// the movie's thread and its stack
extern CThread lbl_80360AE8;
extern u8 lbl_80360E10[0x1000];
extern const char lbl_801FDA80[]; // "game.cpp"
extern const char lbl_801FDA8C[];
extern const char lbl_8020FC30[]; // "UpdateMovie"
char* fn_801911B4(const char* format, ...);
void* fn_8000AC64(void* game);
}

// the panic's game.cpp line
#if defined(VERSION_JP)
static const s32 kUpdateMovieLine = 3780;
#elif defined(VERSION_PAL)
static const s32 kUpdateMovieLine = 3917;
#else
static const s32 kUpdateMovieLine = 3832;
#endif

/**
 * @note Address: 0x8000AB8C
 * @note Size: 0x4
 */
// XObj's vf144
extern "C" void fn_8000AB8C(XObj* object)
{
}

/**
 * @note Address: 0x8000AB90
 * @note Size: 0x28
 */
// stops the movie
extern "C" void fn_8000AB90(CGame* game)
{
	((CMovie*)game->mMovie)->Stop();
}

/**
 * @note Address: 0x8000ABB8
 * @note Size: 0xAC
 */
// starts a movie, the thread below keeps it going
extern "C" void fn_8000ABB8(CGame* game, const char* path, BOOL loop, u32 flags)
{
	BOOL alpha        = flags & 1;
	game->mMovieFlags = flags;
	((CMovie*)game->mMovie)->Start(path, loop, alpha, TRUE);
	game->mMovieRect[0] = game->mMovieRect[1] = game->mMovieRect[2] = game->mMovieRect[3] = NAN;
	lbl_80360AE8.Start(fn_8000AC64, game, lbl_80360E10, sizeof(lbl_80360E10));
}

/**
 * @note Address: 0x8000AC64
 * @note Size: 0xD0
 */
// the movie thread, updates the movie once a retrace until it stops
extern "C" void* fn_8000AC64(void* game)
{
	u32 retrace   = VIGetRetraceCount();
	CMovie* movie = (CMovie*)((CGame*)game)->mMovie;
	do {
		while (retrace == VIGetRetraceCount())
			VIWaitForRetrace();
		retrace = VIGetRetraceCount();
		if (movie->mState != CMOVIE_STATE_IDLE) {
			s32 result = movie->Update();
			if (result < 0) {
				movie->Stop();
				OSPanic(lbl_801FDA80, kUpdateMovieLine, "%s%s", fn_801911B4(lbl_801FDA8C, result), lbl_8020FC30);
			}
		}
	} while (movie->mState != CMOVIE_STATE_IDLE);
	return NULL;
}
