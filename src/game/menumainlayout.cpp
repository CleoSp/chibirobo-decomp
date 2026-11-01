#include "game/cgame.h"
#include "game/cgs.h"
#include "game/menu.h"

#if defined(VERSION_JP)
#define ROW_CHANNEL 10
#else
#define ROW_CHANNEL 6
#endif

extern "C" {
// the menu's own rows, "card slot ?" fills in the channel letter
extern MenuMainItem lbl_80211E08[4];
}

/**
 * @note Address: 0x80061480
 * @note Size: 0x2B8
 */
extern "C" void fn_80061480(MenuMain* menu)
{
	menu->mTextSize     = 16.0f;
	menu->mBaseRows     = 4;
	menu->mRowCount     = (f32)menu->mBaseRows;
	menu->mTextWidthMax = 0.0f;
	MenuMainItem* item  = lbl_80211E08;
	for (s32 i = 0; (f32)i < menu->mRowCount; ++i, ++item) {
		menu->mText[i] = item->mText;
		if (item->mAction == ROW_CHANNEL)
			menu->mText[i][8] = menu->mChannel + 'A';
		menu->mTextWidth[i] = fn_801968BC((CGs*)lbl_8025C400.mGs, menu->mText[i], FALSE, menu->mTextSize);
		if (menu->mTextWidth[i] > menu->mTextWidthMax)
			menu->mTextWidthMax = menu->mTextWidth[i];
	}
	s32 extra = 0;
	if (menu->mExtraRows[0].mText != NULL)
		++extra;
	if (menu->mExtraRows[1].mText != NULL)
		++extra;
	if (menu->mExtraRows[2].mText != NULL)
		++extra;
	if (menu->mExtraRows[3].mText != NULL)
		++extra;
	if (menu->mExtraRows[4].mText != NULL)
		++extra;
	s32 count = (s32)menu->mRowCount;
	if (extra > 0) {
		// a divider row first
		menu->mText[count]      = "\x01";
		menu->mTextWidth[count] = 1.0f;
		++count;
		for (s32 i = 0; i < extra; ++i) {
			if (menu->mExtraRows[i].mText != NULL) {
				menu->mText[count]      = menu->mExtraRows[i].mText;
				menu->mTextWidth[count] = fn_801968BC((CGs*)lbl_8025C400.mGs, menu->mText[count], FALSE, menu->mTextSize);
				if (menu->mTextWidth[count] > menu->mTextWidthMax)
					menu->mTextWidthMax = menu->mTextWidth[count];
				++count;
			}
		}
		menu->mRowCount = (f32)count;
	}
	if ((f32)menu->mSelection >= menu->mRowCount)
		menu->mSelection = (s32)(menu->mRowCount - 1.0f);
	f32 height            = menu->mRowCount * (2.0f + menu->mTextSize) + 24.0f;
	f32 width             = 60.0f + menu->mTextWidthMax;
	menu->mWindow.mWidth  = width;
	menu->mWindow.mHeight = height;
	f32 x                 = menu->mWindow.mWidth * 0.5f + 16.0f;
	f32 y                 = menu->mWindow.mHeight * 0.5f + 30.0f;
	menu->mWindow.mX      = x;
	menu->mWindow.mY      = y;
}

/**
 * @note Address: 0x80061738
 * @note Size: 0xA4
 */
// todo: weak copy
extern "C" BOOL fn_80061738(CMenu* menu)
{
	return menu->CMenu::Draw();
}
