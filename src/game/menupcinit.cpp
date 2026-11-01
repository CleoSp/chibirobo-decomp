#include "game/menu.h"
#include "game/globals.h"

/**
 * @note Address: 0x8004ED74
 * @note Size: 0x74
 */
extern "C" void fn_8004ED74(MenuPC* menu)
{
	menu->CMenu::Init();
	menu->_384 = 0;
	menu->_3A4 = 0;
}

// the chibi-pc's strings
/**
 * @note Address: 0x8004EDE8
 * @note Size: 0x24
 */
extern "C" const char* fn_8004EDE8(s32 index)
{
#if defined(VERSION_JP)
	return fn_8006DBF0(index + 102);
#else
	return fn_8006DBF0(index + 45);
#endif
}
