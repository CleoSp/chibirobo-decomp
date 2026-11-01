#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include <stdio.h>
#include <string.h>
#include "game/cgs.h"
#include "game/memcard.h"
#include "game/menu.h"

/**
 * @note Address: 0x80032CAC
 * @note Size: 0x178
 */
extern "C" void fn_80032CAC(MenuMCTest* menu, s32 action)
{
	switch (action) {
	case 0:
		menu->mOperation = 0;
		break;
	case 1:
		fn_80089890(&lbl_8025C400.mMemcard, lbl_803622B4.mChannel, TRUE);
		menu->mOperation = 1;
		break;
	case 2:
		sprintf(menu->mText[menu->mSelection], "(%d) saving...", menu->mSelection);
		fn_8008A1F4(&lbl_8025C400.mMemcard, lbl_803622B4.mChannel, menu->mSelection, TRUE);
		menu->mOperation = 3;
		break;
	case 4:
		sprintf(menu->mText[menu->mSelection], "(%d) loading...", menu->mSelection);
		fn_80089C90(&lbl_8025C400.mMemcard, lbl_803622B4.mChannel, menu->mSelection, TRUE);
		menu->mOperation = 5;
		break;
	case 6:
		sprintf(menu->mText[menu->mSelection], "(%d) deleting...", menu->mSelection);
		fn_80088B4C(&lbl_8025C400.mMemcard, lbl_803622B4.mChannel, menu->mSelection, TRUE);
		menu->mOperation = 7;
		break;
	}
}

/**
 * @note Address: 0x80032E24
 * @note Size: 0xD8
 */
extern "C" void fn_80032E24(MenuMCTest* menu)
{
	menu->mState = 1;
	menu->Resume();
	menu->mWindow.mOpen = TRUE;
	for (s32 i = 0; i < menu->mFileCount; ++i)
		strcpy(menu->mText[i], "");
	sprintf(menu->mText[menu->mFileCount + 1], lbl_8020144C, lbl_803622B4.mChannel + 'A');
	fn_80089890(&lbl_8025C400.mMemcard, lbl_803622B4.mChannel, TRUE);
	menu->mOperation = 1;
}

/**
 * @note Address: 0x80032EFC
 * @note Size: 0x1E8
 */
extern "C" void fn_80032EFC(MenuMCTest* menu)
{
	menu->mWindow.mOpen   = FALSE;
	menu->mWindow.mX      = 320.0f;
	menu->mWindow.mY      = 224.0f;
	menu->mWindow.mWidth  = 100.0f;
	menu->mWindow.mHeight = 100.0f;
	menu->mDrawAlpha      = 0.0f;
	menu->mCursor         = NULL;
	menu->Reset();
	menu->mFileCount = 10;
	for (s32 i = 0; i < menu->mFileCount; ++i)
		strcpy(menu->mText[i], lbl_80201458);
	strcpy(menu->mText[menu->mFileCount], "\x01");
	strcpy(menu->mText[menu->mFileCount + 1], "");
	menu->mRowCount = (f32)(menu->mFileCount + 2);
	menu->mMaxWidth = 0.0f;
	menu->mFontSize = 16.0f;
	for (s32 i = 0; (f32)i < menu->mRowCount; ++i) {
		menu->mTextWidth[i] = fn_801968BC((CGs*)lbl_8025C400.mGs, menu->mText[i], FALSE, menu->mFontSize);
		if (menu->mTextWidth[i] > menu->mMaxWidth)
			menu->mMaxWidth = menu->mTextWidth[i];
	}
	f32 height            = menu->mRowCount * (2.0f + menu->mFontSize) + 24.0f;
	f32 width             = 60.0f + menu->mMaxWidth;
	menu->mWindow.mWidth  = width;
	menu->mWindow.mHeight = height;
	f32 x                 = 24.0f + (menu->mWindow.mWidth * 0.5f + 16.0f);
	f32 y                 = 24.0f + (menu->mWindow.mHeight * 0.5f + 30.0f);
	menu->mWindow.mX      = x;
	menu->mWindow.mY      = y;
	menu->mOperation      = 0;
}
