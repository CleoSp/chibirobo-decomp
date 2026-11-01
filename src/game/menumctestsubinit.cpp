#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/menu.h"

/**
 * @note Address: 0x80031ECC
 * @note Size: 0x50
 */
extern "C" void fn_80031ECC(MenuMCTestSub* menu)
{
	menu->mState = 1;
	menu->Resume();
	menu->mWindow.mOpen = TRUE;
	menu->mSelection    = 0;
}

/**
 * @note Address: 0x80031F1C
 * @note Size: 0x17C
 */
extern "C" void fn_80031F1C(MenuMCTestSub* menu)
{
	menu->mWindow.mOpen   = FALSE;
	menu->mWindow.mX      = 320.0f;
	menu->mWindow.mY      = 224.0f;
	menu->mWindow.mWidth  = 100.0f;
	menu->mWindow.mHeight = 100.0f;
	menu->mDrawAlpha      = 0.0f;
	menu->mCursor         = NULL;
	menu->Reset();
	menu->mText[0]  = lbl_80212450[0].mText;
	menu->mText[1]  = lbl_80212450[1].mText;
	menu->mText[2]  = lbl_80212450[2].mText;
	menu->mText[3]  = lbl_80212450[3].mText;
	menu->mRowCount = 4.0f;
	menu->mMaxWidth = 0.0f;
	menu->mFontSize = 16.0f;
	for (s32 i = 0; i < menu->mRowCount; ++i) {
		menu->mTextWidth[i] = fn_801968BC((CGs*)lbl_8025C400.mGs, menu->mText[i], TRUE, menu->mFontSize);
		if (menu->mTextWidth[i] > menu->mMaxWidth)
			menu->mMaxWidth = menu->mTextWidth[i];
	}
	f32 height            = 2.0f + menu->mFontSize;
	f32 width             = 60.0f + menu->mMaxWidth;
	height                = menu->mRowCount * height + 24.0f;
	menu->mWindow.mWidth  = width;
	menu->mWindow.mHeight = height;
	f32 x                 = 48.0f + (menu->mWindow.mWidth * 0.5f + 16.0f);
	f32 y                 = 48.0f + (menu->mWindow.mHeight * 0.5f + 30.0f);
	menu->mWindow.mX      = x;
	menu->mWindow.mY      = y;
}
