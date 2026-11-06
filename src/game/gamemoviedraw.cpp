#include "game/cgame.h"
#include "game/cgameutil.h"

/**
 * @note Address: 0x8000BE38
 * @note Size: 0x560
 */
extern "C" void DrawGameMovie(CGame* game)
{
	if (game->mMovie.mMovie.mState != 0) {
		f32 w   = fpclassify(game->mMovieRect[2]) == FP_NAN ? (f32)game->mMovie.mMovie.mVideoInfo.xSize : game->mMovieRect[2];
		f32 h   = fpclassify(game->mMovieRect[3]) == FP_NAN ? (f32)game->mMovie.mMovie.mVideoInfo.ySize : game->mMovieRect[3];
		f32 x   = fpclassify(game->mMovieRect[0]) == FP_NAN ? (640.0f - w) / 2 : game->mMovieRect[0];
		f32 y   = fpclassify(game->mMovieRect[1]) == FP_NAN ? (448.0f - h) / 2 : game->mMovieRect[1];
		CGs* gs = &gGame.mGs;
		SetGsColorUpdate(gs->mCurRenderMode, 1);
		SetGsAlphaUpdate(gs->mCurRenderMode, 0);
		SetGsDepthMode(gs->mCurRenderMode, 1, 7, 0);
		if (game->mMovie.mMovie.mAlpha) {
			SetGsBlendMode(gs->mCurRenderMode, 1, 4, 5, 15);
			SetGsAlphaCompare(gs->mCurRenderMode, 4, 0x20, 0, 3, 255);
			SetupAll(FALSE);
			if (game->mMovie.mMovie.BeginDraw() >= 0) {
				for (s32 i = 0; i < 3; i++) {
					game->mMovie.mMovie.Draw(x, y, w, h);
				}
				game->mMovie.mMovie.EndDraw();
			}
		} else {
			SetGsBlendMode(gs->mCurRenderMode, 0, 4, 5, 15);
			SetGsAlphaCompare(gs->mCurRenderMode, 7, 0, 0, 7, 255);
			SetupAll(FALSE);
			game->mMovie.mMovie.DrawFrame(x, y, w, h);
		}
		SetGsAlphaCompare(gs->mCurRenderMode, 4, 0, 0, 3, 255);
		InvalidateAll();
	}
}
