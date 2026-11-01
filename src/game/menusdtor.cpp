#include "game/menu.h"

// the menus' dtors, the menu globals' __sinit registers them

/**
 * @note Address: 0x8006341C
 * @note Size: 0x90
 */
// todo: weak copy
extern "C" MenuMCTestSub* fn_8006341C(MenuMCTestSub* menu, s16 flags)
{
	if (menu != NULL) {
		menu->~MenuMCTestSub();
		if (flags > 0)
			::operator delete(menu);
	}
	return menu;
}

/**
 * @note Address: 0x800634AC
 * @note Size: 0x90
 */
// todo: weak copy
extern "C" MenuMCTest* fn_800634AC(MenuMCTest* menu, s16 flags)
{
	if (menu != NULL) {
		menu->~MenuMCTest();
		if (flags > 0)
			::operator delete(menu);
	}
	return menu;
}

/**
 * @note Address: 0x8006353C
 * @note Size: 0x90
 */
// todo: weak copy
extern "C" MenuStageSelect* fn_8006353C(MenuStageSelect* menu, s16 flags)
{
	if (menu != NULL) {
		menu->~MenuStageSelect();
		if (flags > 0)
			::operator delete(menu);
	}
	return menu;
}

/**
 * @note Address: 0x800635CC
 * @note Size: 0x90
 */
// todo: weak copy
extern "C" MenuSave* fn_800635CC(MenuSave* menu, s16 flags)
{
	if (menu != NULL) {
		menu->~MenuSave();
		if (flags > 0)
			::operator delete(menu);
	}
	return menu;
}

/**
 * @note Address: 0x8006365C
 * @note Size: 0x90
 */
// todo: weak copy
extern "C" MenuPC* fn_8006365C(MenuPC* menu, s16 flags)
{
	if (menu != NULL) {
		menu->~MenuPC();
		if (flags > 0)
			::operator delete(menu);
	}
	return menu;
}

/**
 * @note Address: 0x800636EC
 * @note Size: 0x90
 */
// todo: weak copy
extern "C" MenuItem* fn_800636EC(MenuItem* menu, s16 flags)
{
	if (menu != NULL) {
		menu->~MenuItem();
		if (flags > 0)
			::operator delete(menu);
	}
	return menu;
}

/**
 * @note Address: 0x8006377C
 * @note Size: 0x90
 */
// todo: weak copy
extern "C" MenuStatus* fn_8006377C(MenuStatus* menu, s16 flags)
{
	if (menu != NULL) {
		menu->~MenuStatus();
		if (flags > 0)
			::operator delete(menu);
	}
	return menu;
}

/**
 * @note Address: 0x8006380C
 * @note Size: 0x90
 */
// todo: weak copy
extern "C" MenuMain* fn_8006380C(MenuMain* menu, s16 flags)
{
	if (menu != NULL) {
		menu->~MenuMain();
		if (flags > 0)
			::operator delete(menu);
	}
	return menu;
}
