#include "game/cgs.h"
#include "game/menu.h"
#include "game/video.h"

#if defined(VERSION_PAL)
#define MENU_FADE_RATE (1.0f / (0.2f * lbl_80660D4C))
#else
#define MENU_FADE_RATE (1.0f / 12.0f)
#endif

static inline void FadeIn(f32& value, u32 frames)
{
	if (value < 1.0f) {
		f32 next = value + MENU_FADE_RATE * frames;
		value    = next;
		if (next > 1.0f)
			value = 1.0f;
	}
}

static inline void FadeOut(f32& value, u32 frames)
{
	if (value > 0.0f) {
		f32 next = value - MENU_FADE_RATE * frames;
		value    = next;
		if (next < 0.0f)
			value = 0.0f;
	}
}

/**
 * @note Address: 0x800617DC
 * @note Size: 0x2E0
 */
// todo: weak copy
extern "C" BOOL fn_800617DC(CMenu* menu, u32 frames)
{
	if (menu->mSuspended) {
		if (menu->mSuspendFade)
			FadeOut(menu->mSuspendAlpha, frames);
	} else {
		FadeIn(menu->mSuspendAlpha, frames);
	}
	if (!menu->IsOpen())
		return 0;
	if (menu->mSuspended) {
		menu->mAlpha     = menu->mFade * menu->mSuspendAlpha;
		menu->mDrawAlpha = menu->mAlpha;
		return 0;
	}
	CPad pad(0);
	switch (menu->mState) {
	case 0:
		break;
	case 1:
		menu->mState = 2;
	case 2:
		menu->mState = 3;
		break;
	case 3:
		FadeIn(menu->mFade, frames);
		if (menu->mFade >= 1.0f)
			menu->mState = 4;
		break;
	case 4:
		break;
	case 5:
		FadeOut(menu->mFade, frames);
		if (0.0f == menu->mFade)
			menu->mState = 6;
		menu->mWindow.mOpen = FALSE;
		break;
	case 6:
		break;
	}
	menu->mAlpha     = menu->mFade * menu->mSuspendAlpha;
	menu->mDrawAlpha = menu->mAlpha;
	if (menu->mWindow.mOpen) {
		f32 step = 0.002f * frames;
		menu->mWindow.mTex.mX += step;
		menu->mWindow.mTex.mY += step;
		PSMTXTrans(menu->mWindow.mTex.mTex.mMtx[0], menu->mWindow.mTex.mX, menu->mWindow.mTex.mY, 0.0f);
		menu->mWindow.mTex.mTex.mMtxDirty = TRUE;
		PSMTXScale(menu->mWindow.mTex.mTex.mMtx[1], menu->mWindow.mTex.mScaleX, menu->mWindow.mTex.mScaleY, 1.0f);
		menu->mWindow.mTex.mTex.mMtxDirty = TRUE;
		// retail builds a second pad here and drops it
		CPad scrollPad(0);
	}
	return menu->mState;
}
