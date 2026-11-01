#include "game/menu.h"

// waits duration * 4 frames, then switches to mNextMode
/**
 * @note Address: 0x80058BBC
 * @note Size: 0xC0
 */
extern "C" void fn_80058BBC(MenuStatus* menu, s32 duration)
{
	CPad pad(0);
	switch (menu->mPhase) {
	case 0:
		menu->mSlide = -200.0f;
		menu->mDelay = duration * 4;
		++menu->mPhase;
		// falls through, the first frame counts too
	case 1:
		if (--menu->mDelay < 0) {
			if (menu->mNextMode != 5)
				menu->mSlide = 0.0f;
			s32 next        = menu->mNextMode;
			menu->mPrevMode = menu->mMode;
			menu->mMode     = next;
			menu->mPhase    = 0;
		}
		break;
	}
}
