#include "game/xchr.h"
#include <math.h>
#include "game/globals.h"

// the globals label and icon ids are 2 lower in japan
#if defined(VERSION_JP)
#define DRS_LABEL(id) ((id) - 2)
#else
#define DRS_LABEL(id) (id)
#endif

// DrsMenu::vf28, the item's color, pulsing while selected
/**
 * @note Address: 0x80098204
 * @note Size: 0x1B4
 */
extern "C" u32 fn_80098204(DrsMenu* menu, s32 index)
{
	s32 red, green, blue;
	if (menu->mItems[index].mAvailable == 0) {
		blue  = 32;
		green = 32;
		red   = 32;
	} else if (menu->_280 == 3) {
		if (menu->mChr->mSuit == menu->mItems[index].mType) {
			red   = 50;
			green = 200;
			blue  = 200;
		} else {
			red   = 50;
			green = 150;
			blue  = 50;
		}
	} else if (index == menu->_04 && menu->mChr->mSuit == menu->mItems[index].mType) {
		red   = 200;
		green = 100;
		blue  = 150;
	} else if (index == menu->_04) {
		red   = 200;
		green = 100;
		blue  = 100;
	} else if (menu->mChr->mSuit == menu->mItems[index].mType) {
		red   = 50;
		green = 200;
		blue  = 200;
	} else {
		red   = 50;
		green = 150;
		blue  = 50;
	}
	if (index == menu->_04) {
		red += (s32)(25.0f * (f32)sin(1.2f * menu->mItems[index]._08) + 25.0f);
		green += (s32)(25.0f * (f32)sin(1.2f * menu->mItems[index]._08) + 25.0f);
		blue += (s32)(25.0f * (f32)sin(1.2f * menu->mItems[index]._08) + 25.0f);
	}
	return ((u32)(u8)red << 24) | ((u32)(u8)green << 16) | ((u32)(u8)blue << 8);
}

// DrsMenu::vf24, the item's label
/**
 * @note Address: 0x800983B8
 * @note Size: 0x64
 */
extern "C" const char* fn_800983B8(DrsMenu* menu, s32 index)
{
	if (menu->mItems[index].mAvailable > 0) {
		if (menu->mChr->mSuit == menu->mItems[index].mType)
			return fn_8006DBF0(DRS_LABEL(13));
		return (const char*)fn_8006E034(menu->mItems[index].mType);
	}
	return fn_8006DBF0(DRS_LABEL(14));
}

// DrsMenu::vf20, the item's icon
/**
 * @note Address: 0x8009841C
 * @note Size: 0xB0
 */
extern "C" s32 fn_8009841C(DrsMenu* menu, s32 index)
{
	s32 icons[9] = { DRS_LABEL(126), DRS_LABEL(98),  DRS_LABEL(100), DRS_LABEL(99), DRS_LABEL(101),
		             DRS_LABEL(126), DRS_LABEL(102), DRS_LABEL(103), DRS_LABEL(104) };
	if (index == -1)
		return DRS_LABEL(107);
	if (index == -2)
		return DRS_LABEL(109);
	if (menu->mItems[index].mAvailable > 0)
		return icons[menu->mItems[index].mType];
	return DRS_LABEL(105);
}

// DrsMenu::vf2C
/**
 * @note Address: 0x800984CC
 * @note Size: 0x28
 */
extern "C" u8 fn_800984CC(DrsMenu* menu, s32 index)
{
	return menu->mChr->mSuit == menu->mItems[index].mType;
}

// DrsMenu::vf1C, puts the selected costume on, or takes the worn one off
/**
 * @note Address: 0x800984F4
 * @note Size: 0xC0
 */
extern "C" void fn_800984F4(DrsMenu* menu)
{
	s8 selectable[9] = { 0, 1, 1, 1, 1, 0, 1, 1, 1 };
	s32 type         = menu->mItems[menu->_04].mType;
	if (selectable[type]) {
		if (type == menu->mChr->mSuit) {
			XObjChr* chr = menu->mChr;
			if (chr->mSuitChanged == 0) {
				chr->mSuitRequest = 0;
				chr->mSuitChanged = 1;
			}
		} else {
			XObjChr* chr = menu->mChr;
			if (chr->mSuitChanged == 0) {
				chr->mSuitRequest = type;
				chr->mSuitChanged = 1;
			}
		}
	} else {
		XObjChr* chr = menu->mChr;
		if (chr->mSuitChanged == 0) {
			chr->mSuitRequest = 0;
			chr->mSuitChanged = 1;
		}
	}
}
