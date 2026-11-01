#include "game/menu.h"

/**
 * @note Address: 0x80060768
 * @note Size: 0x70
 */
extern "C" void fn_80060768(MenuItem* menu)
{
	menu->CMenu::Init();
	menu->mSelection = 0;
}
