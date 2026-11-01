#include "game/menu.h"

/**
 * @note Address: 0x80010CD0
 * @note Size: 0x48
 */
// todo: weak copy
// CMenu's Open
extern "C" void fn_80010CD0(CMenu* menu)
{
	menu->mState = 1;
	menu->Resume();
	menu->mWindow.mOpen = TRUE;
}

/**
 * @note Address: 0x80010D18
 * @note Size: 0x10
 */
// todo: weak copy
// CMenu's Resume
extern "C" void fn_80010D18(CMenu* menu)
{
	menu->mSuspended   = FALSE;
	menu->mSuspendFade = FALSE;
}
