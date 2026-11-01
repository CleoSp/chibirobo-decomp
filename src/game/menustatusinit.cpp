#include "game/menu.h"

/**
 * @note Address: 0x8005C728
 * @note Size: 0x70
 */
extern "C" void fn_8005C728(MenuStatus* menu)
{
	menu->CMenu::Init();
	menu->mSelection = 0;
}
