#include "game/hsd_jobj.h"
#include "game/menu.h"

// todo: weak copy
/**
 * @note Address: 0x8003135C
 * @note Size: 0x48
 */
extern "C" void fn_8003135C(CMenu* menu)
{
	menu->mState = 5;
	menu->Resume();
	menu->mWindow.mOpen = FALSE;
}

// todo: weak copy
/**
 * @note Address: 0x800313A4
 * @note Size: 0x24
 */
extern "C" BOOL fn_800313A4(CMenu* menu)
{
	bool open = false;
	if (menu->mState >= 1 && menu->mState < 6)
		open = true;
	return open;
}

// todo: weak copy
/**
 * @note Address: 0x800313C8
 * @note Size: 0x10
 */
extern "C" void fn_800313C8(CMenu* menu, BOOL arg)
{
	menu->mSuspended   = TRUE;
	menu->mSuspendFade = arg;
}
