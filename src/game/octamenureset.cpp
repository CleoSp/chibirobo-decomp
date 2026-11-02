#include "game/octamenu.h"

// todo: weak copy
/**
 * @note Address: 0x80093BD8
 * @note Size: 0x30
 */
extern "C" void vf14__8OctaMenuFv(OctaMenu* menu)
{
	menu->Open(0);
}

// todo: weak copy
/**
 * @note Address: 0x80093C08
 * @note Size: 0xC
 */
extern "C" void vf18__8OctaMenuFv(OctaMenu* menu)
{
	menu->mOpen = 0;
}
