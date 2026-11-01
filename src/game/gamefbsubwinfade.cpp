#include "game/fbtexeff.h"
#include "game/video.h"

// todo: cleanup
static inline f32 FadeStep()
{
#if defined(VERSION_PAL)
	return 1.0f / (0.3f * lbl_80660D4C);
#else
	return 1.0f / 18.0f;
#endif
}

/**
 * @note Address: 0x800148C0
 * @note Size: 0xA4
 */
// FbSubwin's Update, fades the window in or out over 0.3 seconds
extern "C" void fn_800148C0(FbSubwin* subwin, u32 frames)
{
	if (subwin->mEnabled) {
		if (subwin->mAmount < 1.0f) {
			f32 amount      = subwin->mAmount + FadeStep() * frames;
			subwin->mAmount = amount;
			if (amount >= 1.0f)
				subwin->mAmount = 1.0f;
		}
	} else {
		if (subwin->mAmount > 0.0f) {
			f32 amount      = subwin->mAmount - FadeStep() * frames;
			subwin->mAmount = amount;
			if (amount <= 0.0f)
				subwin->mAmount = 0.0f;
		}
	}
}

/**
 * @note Address: 0x80014964
 * @note Size: 0x14
 */
// FbSubwin's Reset
extern "C" void fn_80014964(FbSubwin* subwin)
{
	subwin->mEnabled = FALSE;
	subwin->mAmount  = 0.0f;
}
