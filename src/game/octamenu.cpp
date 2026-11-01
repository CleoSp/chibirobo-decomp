#include "game/octamenu.h"
#include <math.h>

extern "C" const char lbl_8065D4AC[1]; // ""

// OctaMenu::vf24, the base menu has no labels
/**
 * @note Address: 0x8009AFD4
 * @note Size: 0x8
 */
extern "C" const char* fn_8009AFD4(OctaMenu* menu, s32 index)
{
	return lbl_8065D4AC;
}

// OctaMenu::vf2C
/**
 * @note Address: 0x8009AFDC
 * @note Size: 0x8
 */
extern "C" u8 fn_8009AFDC(OctaMenu* menu, s32 index)
{
	return 0;
}

// OctaMenu::vf20
/**
 * @note Address: 0x8009AFE4
 * @note Size: 0x8
 */
extern "C" s32 fn_8009AFE4(OctaMenu* menu, s32 index)
{
	return 0;
}

// OctaMenu::vf28, the item's color, pulsing while selected
/**
 * @note Address: 0x8009AFEC
 * @note Size: 0x130
 */
extern "C" u32 fn_8009AFEC(OctaMenu* menu, s32 index)
{
	OctaMenuItem* item = &menu->mItems[index];
	s32 red, green, blue;
	if (item->mAvailable == 0) {
		blue  = 32;
		green = 32;
		red   = 32;
	} else if (menu->_280 == 3) {
		red   = 50;
		green = 50;
		blue  = 200;
	} else if (index == menu->_04) {
		red   = 200;
		green = 100;
		blue  = 100;
	} else {
		red   = 50;
		green = 50;
		blue  = 200;
	}
	if (index == menu->_04) {
		red += (s32)(25.0f * (f32)sin(1.2f * item->_08) + 25.0f);
		green += (s32)(25.0f * (f32)sin(1.2f * item->_08) + 25.0f);
		blue += (s32)(25.0f * (f32)sin(1.2f * item->_08) + 25.0f);
	}
	return ((u32)(u8)red << 24) | ((u32)(u8)green << 16) | ((u32)(u8)blue << 8);
}
