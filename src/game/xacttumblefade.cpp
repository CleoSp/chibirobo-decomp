#include "game/xact.h"

/**
 * @note Address: 0x800C39D4
 * @note Size: 0xAC
 */
extern "C" void fn_800C39D4(XActTumbleFade* fade, BOOL increase, u32 frames)
{
	if (fade->mActive) {
		if (increase) {
			if (fade->mAmount < 1.0f) {
				f32 amount    = fade->mAmount + 0.01f * frames;
				fade->mAmount = amount;
				if (amount > 1.0f)
					fade->mAmount = 1.0f;
			}
		} else {
			if (fade->mAmount > 0.0f) {
				f32 amount    = fade->mAmount - 0.05f * frames;
				fade->mAmount = amount;
				if (amount < 0.0f) {
					fade->mAmount = 0.0f;
					fade->mActive = FALSE;
				}
			}
		}
	}
}
