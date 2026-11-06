#include "game/stg.h"

#pragma section RX ".text.menulifecycle"

/**
 * @note Address: 0x8003135C
 * @note Size: 0x48
 */
extern "C" __declspec(section ".text.menulifecycle") void Close__5CMenuFv(CMenu* menu)
{
	menu->mState = 5;
	menu->Resume();
	menu->mWindow.mOpen = FALSE;
}

/**
 * @note Address: 0x800313A4
 * @note Size: 0x24
 */
extern "C" __declspec(section ".text.menulifecycle") BOOL IsOpen__5CMenuFv(CMenu* menu)
{
	return menu->CMenu::IsOpen();
}

/**
 * @note Address: 0x800313C8
 * @note Size: 0x10
 */
extern "C" __declspec(section ".text.menulifecycle") void Suspend__5CMenuFi(CMenu* menu, s32 dir)
{
	menu->mSuspended   = TRUE;
	menu->mSuspendFade = dir;
}
