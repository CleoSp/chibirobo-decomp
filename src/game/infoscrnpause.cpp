#include "game/cgs.h"
#include "game/cgame.h"

extern "C" {
extern const char lbl_80201A98[];
}

// the caption while the game is paused
/**
 * @note Address: 0x8006CA90
 * @note Size: 0xD8
 */
extern "C" void fn_8006CA90()
{
	if (lbl_8025C400.mStatus.mFlags & 0x200000000000ULL) {
		fn_8019B464(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 0, 3, 0);
		const char* text = lbl_80201A98;
		f32 width        = fn_801968BC((CGs*)lbl_8025C400.mGs, text, TRUE, 24.0f);
		fn_80196610(&lbl_8025C400, 0xFFFFFFFF, text, 320.0f - width * 0.5f, 200.0f, 0.0f, 24.0f, 24.0f);
		fn_8019B464(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 3, 0);
	}
}
