#include "game/octamenu.h"

// todo: weak copy
/**
 * @note Address: 0x80093BD8
 * @note Size: 0x30
 */
extern "C" void fn_80093BD8(OctaMenu* menu)
{
	menu->Open(0);
}

// todo: weak copy
/**
 * @note Address: 0x80093C08
 * @note Size: 0xC
 */
extern "C" void fn_80093C08(OctaMenu* menu)
{
	menu->mOpen = 0;
}
