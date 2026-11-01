#include "game/menu.h"
#include "game/cgame.h"
#include "game/csound.h"

extern "C" void fn_80061480(MenuMain* menu);

/**
 * @note Address: 0x800611D4
 * @note Size: 0x30
 */
extern "C" void fn_800611D4(MenuMain* menu)
{
	if (menu->mSuspended)
		lbl_8025C400.mStatus.mFlags |= 2ULL;
	menu->CMenu::Resume();
}

/**
 * @note Address: 0x80061204
 * @note Size: 0x8C
 */
extern "C" void fn_80061204(MenuMain* menu)
{
	menu->CMenu::Close();
	fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x86, 255);
	lbl_8025C400.mStatus.mFlags &= ~2ULL;
}

/**
 * @note Address: 0x80061290
 * @note Size: 0x78
 */
extern "C" void fn_80061290(MenuMain* menu)
{
	menu->CMenu::Open();
	fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x86, 255);
	lbl_8025C400.mStatus.mFlags |= 2ULL;
}

// adds a row under the menu's own, the first free slot of five
/**
 * @note Address: 0x80061308
 * @note Size: 0x5C
 */
extern "C" void fn_80061308(MenuMain* menu, char* text, s32 action)
{
	for (s32 i = 0; i < 5; ++i) {
		if (menu->mExtraRows[i].mText == NULL) {
			menu->mExtraRows[i].mText   = text;
			menu->mExtraRows[i].mAction = action;
			break;
		}
	}
	fn_80061480(menu);
}

static inline void ClearExtraRows(MenuMain* menu)
{
	for (s32 i = 0; i < 5; ++i) {
		menu->mExtraRows[i].mText   = NULL;
		menu->mExtraRows[i].mAction = 0;
	}
}

/**
 * @note Address: 0x80061364
 * @note Size: 0x4C
 */
extern "C" void fn_80061364(MenuMain* menu)
{
	ClearExtraRows(menu);
	fn_80061480(menu);
}

/**
 * @note Address: 0x800613B0
 * @note Size: 0x74
 */
extern "C" void fn_800613B0(MenuMain* menu)
{
	menu->CMenu::Reset();
	menu->mChannel = 0;
	ClearExtraRows(menu);
	fn_80061480(menu);
}

/**
 * @note Address: 0x80061424
 * @note Size: 0x5C
 */
extern "C" void fn_80061424(MenuMain* menu)
{
	menu->CMenu::Init();
}
