#include "game/menu.h"
#include "game/menusave.h"
#include "game/globals.h"

/**
 * @note Address: 0x8003D6FC
 * @note Size: 0x34
 */
extern "C" void fn_8003D6FC(MenuSave* menu)
{
	menu->CMenu::Reset();
	menu->mLastFile = -1;
}

/**
 * @note Address: 0x8003D730
 * @note Size: 0x70
 */
extern "C" void fn_8003D730(MenuSave* menu)
{
	menu->CMenu::Init();
	menu->mLastFile = -1;
}

// the save menu's strings
/**
 * @note Address: 0x8003D7A0
 * @note Size: 0x24
 */
extern "C" const char* fn_8003D7A0(s32 index)
{
#if defined(VERSION_JP)
	return fn_8006DBF0(index + 13);
#else
	return fn_8006DBF0(index + 94);
#endif
}

/**
 * @note Address: 0x8003D7C4
 * @note Size: 0x18
 */
extern "C" bool fn_8003D7C4(const MenuSaveSlot* slot)
{
	return slot->mState != 1;
}

/**
 * @note Address: 0x8003D7DC
 * @note Size: 0x44
 */
extern "C" void fn_8003D7DC(MenuSaveSlot* slot)
{
	ClearSaveSlot(slot);
}
