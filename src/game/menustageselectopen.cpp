#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/menu.h"

/**
 * @note Address: 0x80034108
 * @note Size: 0xE4
 */
extern "C" void fn_80034108(MenuStageSelect* menu)
{
	menu->mState = 1;
	menu->Resume();
	menu->mWindow.mOpen = TRUE;
	if (lbl_8025C400.mStatus.mFlags & 0x0008000000000000ULL) {
		menu->mSelection = -1;
		s32 stage        = lbl_8025C400.mStage;
		for (s32 i = 0; lbl_8021240C[i] >= 0; ++i) {
			if (lbl_8021240C[i] == stage) {
				menu->mSelection = i;
				break;
			}
		}
		if (menu->mSelection < 0)
			menu->mSelection = 0;
	} else {
		menu->mSelection = lbl_8025C400.mStage;
	}
	fn_80033A68(menu);
}

/**
 * @note Address: 0x800341EC
 * @note Size: 0x5C
 */
extern "C" void fn_800341EC(MenuStageSelect* menu)
{
	menu->mWindow.mOpen   = FALSE;
	menu->mWindow.mX      = 320.0f;
	menu->mWindow.mY      = 224.0f;
	menu->mWindow.mWidth  = 100.0f;
	menu->mWindow.mHeight = 100.0f;
	menu->mDrawAlpha      = 0.0f;
	menu->mCursor         = NULL;
	menu->Reset();
}
