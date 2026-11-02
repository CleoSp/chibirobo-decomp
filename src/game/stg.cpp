#include "game/hsd_jobj.h"
#include "game/hsd_cobj.h"
#include "game/stg.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/fbtexeff.h"
#include "game/cfont.h"
#include "game/xchr.h"
#include "game/video.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" {
void* GetSpawnData(s32 index);
char* fn_8006DBC8(s32 id);
char* fn_8006DBF0(s32 id);
char* fn_8006DC8C(s32 id);
char* fn_8006DDB8(s32 id);
char* fn_8006E510(s16 id);
s32 fn_8006DCFC(s32 id);
void fn_80061CD0(MenuCursor* cursor, f32 x, f32 y, f32 w, f32 h, f32 alpha, f32 alpha2);
void vfA4__6CursorFUl(MenuCursor* cursor, s32 a);
void State14__8MenuSaveFUl(MenuSave* menu);
void fn_80033A68(MenuStageSelect* menu);
void fn_80057318(u32 color, f32 x, f32 y, f32 z, f32 w, f32 h, f32 alpha);
void fn_80066630(char* text);
void fn_80054314(f32 alpha);
void fn_80056608(MenuStatus* menu);
void fn_8005D148(s32 id, f32 x, f32 y, f32 alpha, f32 scale, f32 a);
void fn_8005CD3C(s32 id, f32 x, f32 y, f32 alpha, f32 a);
u16 fn_8006E44C(s16 id, s32 kind);
void fn_8004EE0C(MenuStatus* menu);
void fn_800559C4(MenuStatus* menu);
void fn_800554A8(MenuStatus* menu);
void fn_8004F930(MenuStatus* menu);
void fn_80053714(MenuStatus* menu);
void fn_80053D58(MenuStatus* menu);
void GXSetScissor(u32 left, u32 top, u32 width, u32 height);
void Open__10MenuStatusFv(MenuStatus* menu);
void Open__8MenuItemFv(MenuItem* menu);
void Open__6MenuPCFv(MenuPC* menu);
void Open__8MenuSaveFv(MenuSave* menu);
s32 fn_80087A1C(void* save, s32 slot, s32* size, s32* sector);
void fn_80061480(MenuMain* menu);
BOOL Update__5CMenuFUl(CMenu* menu, u32 frames);
void fn_800623F8(MenuWindow* win);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
u16 GXGetTexObjWidth(const GXTexObj* obj);
u16 GXGetTexObjHeight(const GXTexObj* obj);
CTexObj* fn_8019E8F4(CFont* font, const char* str, s32* width);
}
extern "C" u32 gEvtFlags[];

// japan has no language setting, its text is always japanese
static inline bool IsJapanese()
{
#if defined(VERSION_JP)
	return true;
#else
	return lbl_8025C400.mLanguage == 6;
#endif
}

// the fixed lines of the pause menu, MemCard's ? becomes the slot letter
static MenuMainItem sMainLines[] = {
	{ "StatusMenu", 1 }, { "\x01", -1 }, { "MemCard(?)", 6 }, { "SelectStage", 5 }, { NULL, 0 },
};

// the stages the stage select offers, in its order
static s32 sStageOrder[] = { 0, 5, 7, 1, 2, 4, 6, 0x12, 3, 9, 0x10, 0xB, 0x16, 0xD, 0xE, 0xA, -1 };

// the card test popup's rows
static MenuMCTestAction sMCTestSubLines[] = {
	{ "load", 4 },
	{ "save", 2 },
	{ "\x01", -1 },
	{ "delete", 6 },
};

// the file's statics in .bss order, what the first ones hold isn't known yet
u8 lbl_80361F68[0xD4];
Vec lbl_8036203C;
Vec lbl_80362048[8];
u8 lbl_803620A8[0x200]; // MenuPC's input buffer

static MenuMain sMenuMain;
static MenuStatus sMenuStatus;
static MenuItem sMenuItem;
static MenuPC sMenuPC;
static MenuSave sMenuSave;
static MenuStageSelect sMenuStageSelect;
static MenuMCTest sMenuMCTest;
static MenuMCTestSub sMenuMCTestSub;

// the items the status and item menus page through
static MenuStatusItems lbl_80365E08;

// the message each status tab opens with
static s32 sStatusTabMsg[] = { 1, 3, 2, 4 };

inline void MenuMain::Open()
{
	CMenu::Open();
	fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
	lbl_8025C400.mStatus.mFlags |= 2;
}

inline void MenuMain::Close()
{
	CMenu::Close();
	fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
	lbl_8025C400.mStatus.mFlags &= ~2ULL;
}

inline void MenuMain::Resume()
{
	if (mSuspended) {
		lbl_8025C400.mStatus.mFlags |= 2;
	}
	CMenu::Resume();
}

inline void MenuMCTest::Open()
{
	mState = 1;
	Resume();
	mWindow.mOpen = TRUE;
	for (int i = 0; i < mFileCount; i++) {
		strcpy(mText[i], "");
	}
	sprintf(mText[mFileCount + 1], "SLOT: %c", sMenuMain.mChannel + 'A');
	fn_80089890(&lbl_8025C400.mMemcard.mCard, sMenuMain.mChannel, 1);
	mOperation = 1;
}

inline void MenuStageSelect::Open()
{
	mState = 1;
	Resume();
	mWindow.mOpen = TRUE;
	if (lbl_8025C400.mStatus.mFlags & 0x0008000000000000ULL) {
		mSelection = -1;
		int i      = 0;
		for (s32* stage = sStageOrder; *stage >= 0; stage++, i++) {
			if (*stage == lbl_8025C400.mStage) {
				mSelection = i;
				break;
			}
		}
		if (mSelection < 0) {
			mSelection = 0;
		}
	} else {
		mSelection = lbl_8025C400.mStage;
	}
	fn_80033A68(this);
}

inline void MenuTitle::Set(const char* text, u32 color)
{
	const char* p = text;
	mActive       = TRUE;
	mCount        = 0;
	if (text) {
		mColor = color;
		int i;
		for (i = 0; i < 20 && *p; i++, p++) {
			mChars[i].mText   = p;
			mChars[i].mFade   = -0.2f * i;
			mChars[i].mHold   = 60.0f;
			mChars[i].mAngle0 = 0.017453292f * (rand() % 360);
			mChars[i].mAngle1 = 0.017453292f * (rand() % 360);
			mCount++;
			if (IsJapanese() && (IsSjisLead(*p) & 1)) {
				p++;
			}
		}
		for (i = mCount; i < 20; i++) {
			mChars[i].mText = NULL;
			mChars[i].mFade = 0.0f;
			mChars[i].mHold = 0.0f;
		}
	}
}

/**
 * @note Address: 0x8003135C
 * @note Size: 0x48
 */
extern "C" void Close__5CMenuFv(CMenu* menu)
{
	menu->mState = 5;
	menu->Resume();
	menu->mWindow.mOpen = FALSE;
}

/**
 * @note Address: 0x800313A4
 * @note Size: 0x24
 */
extern "C" BOOL IsOpen__5CMenuFv(CMenu* menu)
{
	return menu->CMenu::IsOpen();
}

/**
 * @note Address: 0x800313C8
 * @note Size: 0x10
 */
extern "C" void Suspend__5CMenuFi(CMenu* menu, s32 dir)
{
	menu->mSuspended   = TRUE;
	menu->mSuspendFade = dir;
}

/**
 * @note Address: 0x800313D8
 * @note Size: 0x88
 */
extern "C" void fn_800313D8(MenuEx* ex)
{
	if (ex->mActive) {
		for (int i = 0; i < (int)ARRAY_SIZE(ex->mMenus); i++) {
#if defined(VERSION_JP)
			if (ex->mMenus[i]->IsOpen()) {
#else
			if (ex->mMenus[i] && ex->mMenus[i]->IsOpen()) {
#endif
				ex->mMenus[i]->Draw();
			}
		}
	}
}

/**
 * @note Address: 0x80031460
 * @note Size: 0xB0
 */
extern "C" void fn_80031460(MenuEx* ex, u32 frames)
{
	ex->mActive = FALSE;
	for (int i = 0; i < (int)ARRAY_SIZE(ex->mMenus); i++) {
#if defined(VERSION_JP)
		if (ex->mMenus[i]->IsOpen() && ex->mMenus[i]->Update(frames)) {
#else
		if (ex->mMenus[i] && ex->mMenus[i]->IsOpen() && ex->mMenus[i]->Update(frames)) {
#endif
			ex->mActive = TRUE;
		}
	}
	ex->mCursor.vfA4(frames);
}

/**
 * @note Address: 0x80031510
 * @note Size: 0x74
 */
extern "C" void fn_80031510(MenuEx* ex)
{
	ex->mCursor.vf98();
	for (int i = 0; i < (int)ARRAY_SIZE(ex->mMenus); i++) {
#if !defined(VERSION_JP)
		if (ex->mMenus[i])
#endif
		{
			ex->mMenus[i]->Reset();
		}
	}
}

/**
 * @note Address: 0x80031584
 * @note Size: 0xC8
 */
extern "C" BOOL fn_80031584(MenuEx* ex)
{
	if (ex->mCursor.vf9C(CoordInfo(GetSpawnData(12)))) {
		for (int i = 0; i < (int)ARRAY_SIZE(ex->mMenus); i++) {
#if !defined(VERSION_JP)
			if (ex->mMenus[i])
#endif
			{
				ex->mMenus[i]->LoadTexture();
				ex->mMenus[i]->mCursor = &ex->mCursor;
			}
		}
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x80031740
 * @note Size: 0x398
 */
extern "C" BOOL Draw__13MenuMCTestSubFv(MenuMCTestSub* menu)
{
	if (menu->CMenu::Draw()) {
		f32 x, y, width, step, height, maxW, lines, lineH;
		maxW   = menu->mMaxWidth;
		width  = 30.0f + maxW;
		lineH  = menu->mFontSize;
		step   = 2.0f + lineH;
		lines  = menu->mRowCount;
		height = step * lines;
		x      = menu->mWindow.mX;
		y      = menu->mWindow.mY;
		for (int i = 0; i < lines; i++) {
			u32 color;
			if (i == menu->mSelection) {
				color = (u32)(255.0f * menu->mAlpha) | 0x20202000;
			} else {
				color = (u32)(255.0f * menu->mAlpha) | 0x64646400;
			}
			if (*menu->mText[i] == *"\x01") {
				f32 lineY  = y - height / 2 + step * (0.5f + i);
				f32 left   = x - width / 2;
				u32 shadow = (u32)(255.0f * menu->mAlpha) | 0x64646400;
				fn_801992BC(&lbl_8025C400.mGs, NULL, left, lineY, 0.0f, shadow, left + width, lineY, 0.0f, shadow);
				u32 light = (u32)(255.0f * menu->mAlpha) | 0xC8C8C800;
				fn_801992BC(&lbl_8025C400.mGs, NULL, 1.0f + left, 1.0f + lineY, 0.0f, light, 1.0f + (left + width), 1.0f + lineY, 0.0f,
				            light);
			} else {
				fn_80196518(&lbl_8025C400, color, menu->mText[i], x - menu->mTextWidth[i] / 2, y - height / 2 + step * i, 0.0f, lineH,
				            lineH);
			}
		}
		fn_80061CD0(menu->mCursor, x - maxW / 2, y - height / 2 + step * menu->mSelection, maxW, lineH, menu->mAlpha, menu->mAlpha);
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x8003164C
 * @note Size: 0xF4
 */
// NONMATCHING: japan has five more menus, which ones isn't known
extern "C" void fn_8003164C(MenuEx* ex)
{
	ex->mActive = FALSE;
	for (int i = 0; i < (int)ARRAY_SIZE(ex->mMenus); i++) {
		ex->mMenus[i] = NULL;
	}
	ex->mMenus[0] = &sMenuMain;
	ex->mMenus[1] = &sMenuStatus;
	ex->mMenus[2] = &sMenuItem;
	ex->mMenus[3] = &sMenuPC;
	ex->mMenus[4] = &sMenuSave;
	ex->mMenus[5] = &sMenuStageSelect;
	ex->mMenus[6] = &sMenuMCTest;
	ex->mMenus[7] = &sMenuMCTestSub;
	for (int j = 0; j < (int)ARRAY_SIZE(ex->mMenus); j++) {
		if (ex->mMenus[j]) {
			ex->mMenus[j]->Init();
		}
	}
	ex->mCursor.vf90(0);
}

/**
 * @note Address: 0x80031AD8
 * @note Size: 0x3F4
 */
// NONMATCHING: string pool offsets, the strings before these aren't in yet
extern "C" BOOL Update__13MenuMCTestSubFUl(MenuMCTestSub* menu, u32 frames)
{
	if (Update__5CMenuFUl(menu, frames) == 4) {
		CPad pad(0);
		if (pad.mPad->mRepeat & 0x10008) {
			do {
				if (--menu->mSelection < 0) {
					menu->mSelection = 3;
				}
			} while (*menu->mText[menu->mSelection] == *"\x01");
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0x20004) {
			do {
				if (++menu->mSelection > 3) {
					menu->mSelection = 0;
				}
			} while (*menu->mText[menu->mSelection] == *"\x01");
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mTrigger & 0x200) {
			menu->Close();
			sMenuMCTest.mSuspended   = FALSE;
			sMenuMCTest.mSuspendFade = 0;
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
		} else if (pad.mPad->mTrigger & 0x100) {
			s32 id                  = sMCTestSubLines[menu->mSelection].mAction;
			Memcard* card           = &lbl_8025C400.mMemcard.mCard;
			SavePreviewRecord* save = &card->mSaves[0][sMenuMCTest.mSelection];
			if ((id == 4 || id == 6) && save->mState == 0) {
				fn_8001E904(&lbl_8025C400.mSound, 0x85, 0xFF);
			} else if (id == 4 && save->mState != 0 && (save->mStatus != 0 || !fn_80086F24(save))) {
				fn_8001E904(&lbl_8025C400.mSound, 0x85, 0xFF);
			} else {
				menu->Close();
				sMenuMCTest.mSuspended   = FALSE;
				sMenuMCTest.mSuspendFade = 0;
				switch (id) {
				case 0:
					sMenuMCTest.mOperation = 0;
					break;
				case 1:
					fn_80089890(card, sMenuMain.mChannel, 1);
					sMenuMCTest.mOperation = 1;
					break;
				case 2:
					sprintf(sMenuMCTest.mText[sMenuMCTest.mSelection], "(%d) saving...", sMenuMCTest.mSelection);
					fn_8008A1F4(card, sMenuMain.mChannel, sMenuMCTest.mSelection, 1);
					sMenuMCTest.mOperation = 3;
					break;
				case 4:
					sprintf(sMenuMCTest.mText[sMenuMCTest.mSelection], "(%d) loading...", sMenuMCTest.mSelection);
					fn_80089C90(card, sMenuMain.mChannel, sMenuMCTest.mSelection, 1);
					sMenuMCTest.mOperation = 5;
					break;
				case 6:
					sprintf(sMenuMCTest.mText[sMenuMCTest.mSelection], "(%d) deleting...", sMenuMCTest.mSelection);
					fn_80088B4C(card, sMenuMain.mChannel, sMenuMCTest.mSelection, 1);
					sMenuMCTest.mOperation = 7;
					break;
				}
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			}
		}
	}
	return menu->IsOpen();
}

/**
 * @note Address: 0x80031ECC
 * @note Size: 0x50
 */
extern "C" void Open__13MenuMCTestSubFv(Menu* menu)
{
	menu->mState = 1;
	menu->Resume();
	menu->mWindow.mOpen = TRUE;
	menu->mSelection    = 0;
}

/**
 * @note Address: 0x800341EC
 * @note Size: 0x5C
 */
extern "C" void Init__15MenuStageSelectFv(Menu* menu)
{
	menu->CMenu::Init();
}

/**
 * @note Address: 0x80031F1C
 * @note Size: 0x17C
 */
extern "C" void Init__13MenuMCTestSubFv(MenuMCTestSub* menu)
{
	menu->CMenu::Init();
	menu->mText[0]  = sMCTestSubLines[0].mText;
	menu->mText[1]  = sMCTestSubLines[1].mText;
	menu->mText[2]  = sMCTestSubLines[2].mText;
	menu->mText[3]  = sMCTestSubLines[3].mText;
	menu->mRowCount = 4.0f;
	menu->mMaxWidth = 0.0f;
	menu->mFontSize = 16.0f;
	for (int i = 0; i < menu->mRowCount; i++) {
		menu->mTextWidth[i] = fn_801968BC(&lbl_8025C400.mGs, menu->mText[i], 1, menu->mFontSize);
		if (menu->mTextWidth[i] > menu->mMaxWidth) {
			menu->mMaxWidth = menu->mTextWidth[i];
		}
	}
	menu->mWindow.SetSize(60.0f + menu->mMaxWidth, menu->mRowCount * (2.0f + menu->mFontSize) + 24.0f);
	menu->mWindow.SetPos(48.0f + (menu->mWindow.mWidth / 2 + 16.0f), 48.0f + (menu->mWindow.mHeight / 2 + 30.0f));
}

/**
 * @note Address: 0x80032098
 * @note Size: 0x4C4
 */
extern "C" BOOL Draw__10MenuMCTestFv(MenuMCTest* menu)
{
	if (menu->CMenu::Draw()) {
		f32 x, y, width, step, height, maxW, lines, lineH;
		maxW       = menu->mMaxWidth;
		width      = 30.0f + maxW;
		lineH      = menu->mFontSize;
		step       = 2.0f + lineH;
		lines      = menu->mRowCount;
		height     = step * lines;
		x          = menu->mWindow.mX;
		y          = menu->mWindow.mY;
		s32 result = lbl_8025C400.mMemcard.mCard.mCard.mResult;
		for (int i = 0; i < lines; i++) {
			u32 color;
			if (i == menu->mSelection) {
				color = (u32)(255.0f * menu->mAlpha) | 0x20202000;
			} else {
				color = (u32)(255.0f * menu->mAlpha) | 0x64646400;
			}
			if (*menu->mText[i] == *"\x01") {
				f32 lineY  = y - height / 2 + step * (0.5f + i);
				f32 left   = x - width / 2;
				u32 shadow = (u32)(255.0f * menu->mAlpha) | 0x64646400;
				fn_801992BC(&lbl_8025C400.mGs, NULL, left, lineY, 0.0f, shadow, left + width, lineY, 0.0f, shadow);
				u32 light = (u32)(255.0f * menu->mAlpha) | 0xC8C8C800;
				fn_801992BC(&lbl_8025C400.mGs, NULL, 1.0f + left, 1.0f + lineY, 0.0f, light, 1.0f + (left + width), 1.0f + lineY, 0.0f,
				            light);
			} else if (result == 0 || result == -1 || result == -4 || result == -7 || result == -128 || i >= menu->mFileCount) {
				fn_80196518(&lbl_8025C400, color, menu->mText[i], x - menu->mMaxWidth / 2, y - height / 2 + step * i, 0.0f, lineH, lineH);
			} else {
				f32 textW = fn_801968BC(&lbl_8025C400.mGs, menu->mText[i], 0, menu->mFontSize);
				fn_80196518(&lbl_8025C400, (u32)(255.0f * menu->mAlpha) | 0x20202000, menu->mText[i], x - textW / 2,
				            y - height / 2 + step * i, 0.0f, lineH, lineH);
			}
		}
		if (result == 0 || result == -1 || result == -4 || result == -7 || result == -128) {
			f32 cx = x - maxW / 2;
			f32 cy = y - height / 2 + step * menu->mSelection;
			f32 alpha;
			if (sMenuMCTestSub.IsOpen()) {
				if (menu->mState == 4) {
					alpha = 1.0f - sMenuMCTestSub.mDrawAlpha;
				} else {
					alpha = 0.0f;
				}
			} else {
				alpha = menu->mAlpha;
			}
			fn_80061CD0(menu->mCursor, cx, cy, maxW, lineH, menu->mAlpha, alpha);
		}
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x80032CAC
 * @note Size: 0x178
 */
// NONMATCHING: string pool offsets, the strings before these aren't in yet
extern "C" void fn_80032CAC(MenuMCTest* menu, s32 id)
{
	switch (id) {
	case 0:
		menu->mOperation = 0;
		break;
	case 1:
		fn_80089890(&lbl_8025C400.mMemcard.mCard, sMenuMain.mChannel, 1);
		menu->mOperation = 1;
		break;
	case 2:
		sprintf(menu->mText[menu->mSelection], "(%d) saving...", menu->mSelection);
		fn_8008A1F4(&lbl_8025C400.mMemcard.mCard, sMenuMain.mChannel, menu->mSelection, 1);
		menu->mOperation = 3;
		break;
	case 4:
		sprintf(menu->mText[menu->mSelection], "(%d) loading...", menu->mSelection);
		fn_80089C90(&lbl_8025C400.mMemcard.mCard, sMenuMain.mChannel, menu->mSelection, 1);
		menu->mOperation = 5;
		break;
	case 6:
		sprintf(menu->mText[menu->mSelection], "(%d) deleting...", menu->mSelection);
		fn_80088B4C(&lbl_8025C400.mMemcard.mCard, sMenuMain.mChannel, menu->mSelection, 1);
		menu->mOperation = 7;
		break;
	}
}

/**
 * @note Address: 0x80032E24
 * @note Size: 0xD8
 */
extern "C" void Open__10MenuMCTestFv(MenuMCTest* menu)
{
	menu->mState = 1;
	menu->Resume();
	menu->mWindow.mOpen = TRUE;
	for (int i = 0; i < menu->mFileCount; i++) {
		strcpy(menu->mText[i], "");
	}
	sprintf(menu->mText[menu->mFileCount + 1], "SLOT: %c", sMenuMain.mChannel + 'A');
	fn_80089890(&lbl_8025C400.mMemcard.mCard, sMenuMain.mChannel, 1);
	menu->mOperation = 1;
}

/**
 * @note Address: 0x80032EFC
 * @note Size: 0x1E8
 */
extern "C" void Init__10MenuMCTestFv(MenuMCTest* menu)
{
	menu->CMenu::Init();
	menu->mFileCount = 10;
	for (int i = 0; i < menu->mFileCount; i++) {
		strcpy(menu->mText[i],
		       "(0) 00/00/00 00:00 [ 0:00] (000)\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40");
	}
	strcpy(menu->mText[menu->mFileCount], "\x01");
	strcpy(menu->mText[menu->mFileCount + 1], "");
	menu->mRowCount = menu->mFileCount + 2;
	menu->mMaxWidth = 0.0f;
	menu->mFontSize = 16.0f;
	for (int i = 0; i < menu->mRowCount; i++) {
		menu->mTextWidth[i] = fn_801968BC(&lbl_8025C400.mGs, menu->mText[i], 0, menu->mFontSize);
		if (menu->mTextWidth[i] > menu->mMaxWidth) {
			menu->mMaxWidth = menu->mTextWidth[i];
		}
	}
	menu->mWindow.SetSize(60.0f + menu->mMaxWidth, menu->mRowCount * (2.0f + menu->mFontSize) + 24.0f);
	menu->mWindow.SetPos(24.0f + (menu->mWindow.mWidth / 2 + 16.0f), 24.0f + (menu->mWindow.mHeight / 2 + 30.0f));
	menu->mOperation = 0;
}

/**
 * @note Address: 0x800336DC
 * @note Size: 0x38C
 */
extern "C" BOOL Update__15MenuStageSelectFUl(MenuStageSelect* menu, u32 frames)
{
	if (Update__5CMenuFUl(menu, frames) == 4) {
		CPad pad(0);
		if (pad.mPad->mRepeat & 0x10008) {
			do {
				if (--menu->mSelection < 0) {
					menu->mSelection = menu->mCount - 1;
				}
			} while (*menu->mLabels[menu->mSelection] == *"\x01");
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0x20004) {
			do {
				if (++menu->mSelection > menu->mCount - 1) {
					menu->mSelection = 0;
				}
			} while (*menu->mLabels[menu->mSelection] == *"\x01");
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0x40001) {
			if (menu->mSelection - 20 >= 0 && *menu->mLabels[menu->mSelection - 20] != *"\x01") {
				menu->mSelection -= 20;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			}
		} else if (pad.mPad->mRepeat & 0x80002) {
			if (menu->mSelection + 20 < menu->mCount && *menu->mLabels[menu->mSelection + 20] != *"\x01") {
				menu->mSelection += 20;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			}
		} else if (pad.mPad->mTrigger & 0x200) {
			menu->Close();
			sMenuMain.Resume();
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
		} else if (pad.mPad->mTrigger & 0x100) {
			menu->Close();
			sMenuMain.Close();
			if (lbl_8025C400.mStatus.mFlags & 0x0008000000000000ULL) {
				fn_8000B394(&lbl_8025C400, sStageOrder[menu->mSelection], 1);
			} else {
				fn_8000B394(&lbl_8025C400, menu->mSelection, 1);
			}
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
		}
	}
	return menu->IsOpen();
}

/**
 * @note Address: 0x80034108
 * @note Size: 0xE4
 */
extern "C" void Open__15MenuStageSelectFv(MenuStageSelect* menu)
{
	menu->mState = 1;
	menu->Resume();
	menu->mWindow.mOpen = TRUE;
	if (lbl_8025C400.mStatus.mFlags & 0x0008000000000000ULL) {
		menu->mSelection = -1;
		s32* stages      = sStageOrder;
		s32 stage;
		s32 current = lbl_8025C400.mStage;
		for (int i = 0; stages[i] >= 0; i++) {
			stage = stages[i];
			if (stage == current) {
				menu->mSelection = i;
				break;
			}
		}
		if (menu->mSelection < 0) {
			menu->mSelection = 0;
		}
	} else {
		menu->mSelection = lbl_8025C400.mStage;
	}
	fn_80033A68(menu);
}

/**
 * @note Address: 0x8003D350
 * @note Size: 0x3AC
 */
extern "C" void Open__8MenuSaveFv(MenuSave* menu)
{
	menu->mState = 1;
	menu->Resume();
	menu->mWindow.mOpen = TRUE;
	lbl_8025C400.mStatus.mFlags |= 2;
	menu->mWindow.SetSize(0.0f, 0.0f);
	menu->mWindow.SetPos(0.0f, 0.0f);
	menu->_158         = -10.0f;
	menu->_15C         = -200.0f;
	menu->_160         = -10.0f;
	menu->_164         = 0.0f;
	menu->_168         = 0.0f;
	menu->_16C         = 0.0f;
	menu->_170         = 0.0f;
	menu->mOffset      = 0.0f;
	menu->_264         = 0.0f;
	menu->mTargetAlpha = 1.0f;
	menu->mMode        = 0;
	menu->mPrevMode    = menu->mMode;
	menu->mPhase       = 0;
	menu->mTime        = 0.0f;
	menu->mDelay       = 0;
	if (menu->mLastFile == -1) {
		menu->mFile = lbl_8025C400.mInfoScrn._1D0;
	} else {
		menu->mFile = menu->mLastFile;
	}
	menu->mChoice      = 0;
	menu->mResult      = 0;
	menu->mMessage     = 0;
	menu->mReady       = 0;
	menu->mShowMessage = 0;
	menu->_262         = 0;
	menu->mSaved       = 0;
	menu->mChannel     = 0;
	menu->_2A4         = 0.0f;
#if !defined(VERSION_JP)
#if !defined(VERSION_JP)
	menu->_538 = 0;
#endif
#endif
	fn_8001E904(&lbl_8025C400.mSound, 0x14B, 0xFF);
	u32 color = (u32)(255.0f * menu->mTargetAlpha) | 0xFFFFFF00;
	menu->mTitle.Set(fn_8006DBF0(0x62), color);
}

/**
 * @note Address: 0x8003D6FC
 * @note Size: 0x34
 */
extern "C" void Reset__8MenuSaveFv(MenuSave* menu)
{
	menu->CMenu::Reset();
	menu->mLastFile = -1;
}

/**
 * @note Address: 0x8003D730
 * @note Size: 0x70
 */
extern "C" void Init__8MenuSaveFv(MenuSave* menu)
{
	menu->CMenu::Init();
	menu->mLastFile = -1;
}

/**
 * @note Address: 0x8004ED74
 * @note Size: 0x74
 */
extern "C" void Init__6MenuPCFv(MenuPC* menu)
{
	menu->CMenu::Init();
	menu->_384 = 0;
	menu->_3A4 = 0;
}

/**
 * @note Address: 0x8005C728
 * @note Size: 0x70
 */
extern "C" void Init__10MenuStatusFv(MenuStatus* menu)
{
	menu->CMenu::Init();
	menu->mSelection = 0;
}

/**
 * @note Address: 0x80060768
 * @note Size: 0x70
 */
extern "C" void Init__8MenuItemFv(MenuItem* menu)
{
	menu->CMenu::Init();
	menu->mSelection = 0;
}

/**
 * @note Address: 0x800611D4
 * @note Size: 0x30
 */
extern "C" void Resume__8MenuMainFv(MenuMain* menu)
{
	menu->MenuMain::Resume();
}

/**
 * @note Address: 0x80061204
 * @note Size: 0x8C
 */
extern "C" void Close__8MenuMainFv(MenuMain* menu)
{
	menu->MenuMain::Close();
}

/**
 * @note Address: 0x80061290
 * @note Size: 0x78
 */
extern "C" void Open__8MenuMainFv(MenuMain* menu)
{
	menu->MenuMain::Open();
}

/**
 * @note Address: 0x80061308
 * @note Size: 0x5C
 */
extern "C" void fn_80061308(MenuMain* menu, char* text, s32 id)
{
	for (int i = 0; i < 5; i++) {
		if (menu->mExtraRows[i].mText == NULL) {
			menu->mExtraRows[i].mText   = text;
			menu->mExtraRows[i].mAction = id;
			break;
		}
	}
	fn_80061480(menu);
}

/**
 * @note Address: 0x80061364
 * @note Size: 0x4C
 */
extern "C" void fn_80061364(MenuMain* menu)
{
	for (int i = 0; i < 5; i++) {
		menu->mExtraRows[i].mText   = NULL;
		menu->mExtraRows[i].mAction = 0;
	}
	fn_80061480(menu);
}

/**
 * @note Address: 0x800613B0
 * @note Size: 0x74
 */
extern "C" void Reset__8MenuMainFv(MenuMain* menu)
{
	menu->CMenu::Reset();
	menu->mChannel = 0;
	for (int i = 0; i < 5; i++) {
		menu->mExtraRows[i].mText   = NULL;
		menu->mExtraRows[i].mAction = 0;
	}
	fn_80061480(menu);
}

/**
 * @note Address: 0x800607D8
 * @note Size: 0x504
 */
extern "C" BOOL Draw__8MenuMainFv(MenuMain* menu)
{
	if (menu->CMenu::Draw()) {
		f32 x, y, width, step, height, maxW, lines, lineH;
		maxW   = menu->mMaxWidth;
		width  = 30.0f + maxW;
		lineH  = menu->mFontSize;
		step   = 2.0f + lineH;
		lines  = menu->mRowCount;
		height = step * lines;
		x      = menu->mWindow.mX;
		y      = menu->mWindow.mY;
		for (int i = 0; i < lines; i++) {
			u32 color;
			if (i == menu->mSelection) {
				color = (u32)(255.0f * menu->mAlpha) | 0x20202000;
			} else {
				color = (u32)(255.0f * menu->mAlpha) | 0x64646400;
			}
			if (*menu->mText[i] == *"\x01") {
				f32 lineY  = y - height / 2 + step * (0.5f + i);
				f32 left   = x - width / 2;
				u32 shadow = (u32)(255.0f * menu->mAlpha) | 0x64646400;
				fn_801992BC(&lbl_8025C400.mGs, NULL, left, lineY, 0.0f, shadow, left + width, lineY, 0.0f, shadow);
				u32 light = (u32)(255.0f * menu->mAlpha) | 0xC8C8C800;
				fn_801992BC(&lbl_8025C400.mGs, NULL, 1.0f + left, 1.0f + lineY, 0.0f, light, 1.0f + (left + width), 1.0f + lineY, 0.0f,
				            light);
			} else {
				fn_80196518(&lbl_8025C400, color, menu->mText[i], x - menu->mMaxWidth / 2, y - height / 2 + step * i, 0.0f, lineH, lineH);
			}
		}
		f32 cx = x - maxW / 2;
		f32 cy = y - height / 2 + step * menu->mSelection;
		f32 alpha;
		if (sMenuStatus.IsOpen()) {
			alpha = menu->mState == 4 ? 1.0f - sMenuStatus.mDrawAlpha : 0.0f;
		} else if (sMenuMCTest.IsOpen()) {
			alpha = menu->mState == 4 ? 1.0f - sMenuMCTest.mDrawAlpha : 0.0f;
		} else if (sMenuStageSelect.IsOpen()) {
			alpha = menu->mState == 4 ? 1.0f - sMenuStageSelect.mDrawAlpha : 0.0f;
		} else if (sMenuPC.IsOpen()) {
			alpha = menu->mState == 4 ? 1.0f - sMenuPC.mDrawAlpha : 0.0f;
		} else if (sMenuSave.IsOpen()) {
			alpha = menu->mState == 4 ? 1.0f - sMenuSave.mDrawAlpha : 0.0f;
		} else {
			alpha = menu->mAlpha;
		}
		fn_80061CD0(menu->mCursor, cx, cy, maxW, lineH, menu->mAlpha, alpha);
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x80060CDC
 * @note Size: 0x4F8
 */
extern "C" BOOL Update__8MenuMainFUl(MenuMain* menu, u32 frames)
{
	if (Update__5CMenuFUl(menu, frames) == 4) {
		CPad pad(0);
		s32 id = -1;
		if (pad.mPad->mRepeat & 0x10008) {
			do {
				if (--menu->mSelection < 0) {
					menu->mSelection = menu->mRowCount - 1.0f;
				}
			} while (*menu->mText[menu->mSelection] == *"\x01");
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0x20004) {
			do {
				if (++menu->mSelection > menu->mRowCount - 1.0f) {
					menu->mSelection = 0;
				}
			} while (*menu->mText[menu->mSelection] == *"\x01");
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0xC0003) {
			if (sMainLines[menu->mSelection].mAction == 6) {
				menu->mChannel = (menu->mChannel + 1) & 1;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
				menu->mText[menu->mSelection][8] = menu->mChannel + 'A';
			}
		} else if (pad.mPad->mTrigger & 0x1200) {
			menu->Close();
		} else if (pad.mPad->mTrigger & 0x100) {
			if (menu->mSelection < menu->mBaseRows) {
				id = sMainLines[menu->mSelection].mAction;
			} else {
				menu->Close();
				fn_8007EEA4(&lbl_8025C400.mEvt, (u8*)menu->mExtraRows[menu->mSelection - (menu->mBaseRows + 1)].mAction, 0, -1, -1);
			}
		}
		if (id >= 0) {
			BOOL fade = TRUE;
			switch (id) {
			case 1:
				Open__10MenuStatusFv(&sMenuStatus);
				break;
			case 2:
				Open__8MenuItemFv(&sMenuItem);
				break;
			case 6:
				fade = FALSE;
				sMenuMCTest.Open();
				break;
			case 5:
				fade = FALSE;
				sMenuStageSelect.Open();
				break;
			case 3:
				Open__6MenuPCFv(&sMenuPC);
				break;
			case 4:
				Open__8MenuSaveFv(&sMenuSave);
				break;
			}
			if (id >= 0) {
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				menu->Suspend(fade);
			} else {
				fn_8001E904(&lbl_8025C400.mSound, 0x85, 0xFF);
			}
		}
	}
	return menu->IsOpen();
}

/**
 * @note Address: 0x80061424
 * @note Size: 0x5C
 */
extern "C" void Init__8MenuMainFv(MenuMain* menu)
{
	menu->CMenu::Init();
}

/**
 * @note Address: 0x80061480
 * @note Size: 0x2B8
 */
extern "C" void fn_80061480(MenuMain* menu)
{
	menu->mFontSize = 16.0f;
	menu->mBaseRows = 4;
	menu->mRowCount = menu->mBaseRows;
	menu->mMaxWidth = 0.0f;
	for (int i = 0; i < menu->mRowCount; i++) {
		menu->mText[i] = sMainLines[i].mText;
		if (sMainLines[i].mAction == 6) {
			menu->mText[i][8] = menu->mChannel + 'A';
		}
		menu->mTextWidth[i] = fn_801968BC(&lbl_8025C400.mGs, menu->mText[i], 0, menu->mFontSize);
		if (menu->mTextWidth[i] > menu->mMaxWidth) {
			menu->mMaxWidth = menu->mTextWidth[i];
		}
	}
	int items = 0;
	for (int k = 0; k < 5; k++) {
		if (menu->mExtraRows[k].mText) {
			items++;
		}
	}
	int line = menu->mRowCount;
	if (items > 0) {
		menu->mText[line]      = "\x01";
		menu->mTextWidth[line] = 1.0f;
		line++;
		for (int j = 0; j < items; j++) {
			if (menu->mExtraRows[j].mText) {
				menu->mText[line]      = menu->mExtraRows[j].mText;
				menu->mTextWidth[line] = fn_801968BC(&lbl_8025C400.mGs, menu->mText[line], 0, menu->mFontSize);
				if (menu->mTextWidth[line] > menu->mMaxWidth) {
					menu->mMaxWidth = menu->mTextWidth[line];
				}
				line++;
			}
		}
		menu->mRowCount = line;
	}
	if (menu->mSelection >= menu->mRowCount) {
		menu->mSelection = menu->mRowCount - 1.0f;
	}
	menu->mWindow.SetSize(60.0f + menu->mMaxWidth, menu->mRowCount * (2.0f + menu->mFontSize) + 24.0f);
	menu->mWindow.SetPos(menu->mWindow.mWidth / 2 + 16.0f, menu->mWindow.mHeight / 2 + 30.0f);
}

/**
 * @note Address: 0x80061738
 * @note Size: 0xA4
 */
extern "C" BOOL Draw__5CMenuFv(CMenu* menu)
{
	return menu->CMenu::Draw();
}

/**
 * @note Address: 0x800617DC
 * @note Size: 0x2E0
 */
extern "C" BOOL Update__5CMenuFUl(CMenu* menu, u32 frames)
{
	if (menu->mSuspended) {
		if (menu->mSuspendFade != 0 && menu->mSuspendAlpha > 0.0f) {
			if ((menu->mSuspendAlpha -= 0.083333336f * frames) < 0.0f) {
				menu->mSuspendAlpha = 0.0f;
			}
		}
	} else if (menu->mSuspendAlpha < 1.0f) {
		if ((menu->mSuspendAlpha += 0.083333336f * frames) > 1.0f) {
			menu->mSuspendAlpha = 1.0f;
		}
	}
	if (!menu->IsOpen()) {
		return FALSE;
	}
	if (menu->mSuspended) {
		menu->mAlpha     = menu->mFade * menu->mSuspendAlpha;
		menu->mDrawAlpha = menu->mAlpha;
		return FALSE;
	}
	CPad pad(0);
	switch (menu->mState) {
	case 0:
		break;
	case 1:
		menu->mState = 2;
	case 2:
		menu->mState = 3;
		break;
	case 3:
		if (menu->mFade < 1.0f) {
			if ((menu->mFade += 0.083333336f * frames) > 1.0f) {
				menu->mFade = 1.0f;
			}
		}
		if (menu->mFade >= 1.0f) {
			menu->mState = 4;
		}
		break;
	case 4:
		break;
	case 5:
		if (menu->mFade > 0.0f) {
			if ((menu->mFade -= 0.083333336f * frames) < 0.0f) {
				menu->mFade = 0.0f;
			}
		}
		if (0.0f == menu->mFade) {
			menu->mState = 6;
		}
		menu->mWindow.mOpen = FALSE;
		break;
	case 6:
		break;
	}
	menu->mAlpha     = menu->mFade * menu->mSuspendAlpha;
	menu->mDrawAlpha = menu->mAlpha;
	if (menu->mWindow.mOpen) {
		menu->mWindow.mTex.mX += 0.002f * frames;
		menu->mWindow.mTex.mY += 0.002f * frames;
		PSMTXTrans(menu->mWindow.mTex.mTex.mMtx[0], menu->mWindow.mTex.mX, menu->mWindow.mTex.mY, 0.0f);
		menu->mWindow.mTex.mTex.mMtxDirty = TRUE;
		PSMTXScale(menu->mWindow.mTex.mTex.mMtx[1], menu->mWindow.mTex.mScaleX, menu->mWindow.mTex.mScaleY, 1.0f);
		menu->mWindow.mTex.mTex.mMtxDirty = TRUE;
		CPad pad2(0);
	}
	return menu->mState;
}

/**
 * @note Address: 0x80061AE8
 * @note Size: 0x18C
 */
extern "C" BOOL LoadTexture__5CMenuFv(CMenu* menu)
{
	MenuWindow* win   = &menu->mWindow;
	win->mTex.mTex    = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[138], 0);
	CTexObj* tex      = &win->mTex.mTex;
	u16 w             = GXGetTexObjWidth(tex);
	win->mTex.mScaleX = win->mWidth / w;
	u16 h             = GXGetTexObjHeight(tex);
	win->mTex.mScaleY = win->mHeight / h;
	PSMTXTrans(tex->mMtx[0], win->mTex.mX, win->mTex.mY, 0.0f);
	win->mTex.mTex.mMtxDirty = TRUE;
	PSMTXScale(tex->mMtx[1], win->mTex.mScaleX, win->mTex.mScaleY, 1.0f);
	win->mTex.mTex.mMtxDirty = TRUE;
	return TRUE;
}

/**
 * @note Address: 0x80061ABC
 * @note Size: 0x2C
 */
extern "C" void Reset__5CMenuFv(CMenu* menu)
{
	menu->CMenu::Reset();
}

/**
 * @note Address: 0x80061C74
 * @note Size: 0x5C
 */
extern "C" void Init__5CMenuFv(CMenu* menu)
{
	menu->CMenu::Init();
}

/**
 * @note Address: 0x8003AC48
 * @note Size: 0xE0
 */
extern "C" void State17__8MenuSaveFUl(MenuSave* menu)
{
	if (menu->_264 <= 0.0f) {
		fn_8001E660(&lbl_8025C400.mSound, 0x14B);
		fn_8001E904(&lbl_8025C400.mSound, 0x185, 0xFF);
		lbl_8025C400.mStatus.mFlags &= ~2ULL;
		menu->Close();
		sMenuMain.Resume();
	}
}

/**
 * @note Address: 0x8003AD28
 * @note Size: 0x80
 */
// NONMATCHING: regalloc
extern "C" void State16__8MenuSaveFUl(MenuSave* menu)
{
	for (int i = 0; i < 20; i++) {
		menu->mTitle.mChars[i].mHold = 0.0f;
	}
	menu->mTargetAlpha = 0.0f;
	menu->_164         = -200.0f;
	menu->_16C         = 0.0f;
	menu->SetMode(0x11);
}

/**
 * @note Address: 0x8003ADA8
 * @note Size: 0x20
 */
extern "C" void State15__8MenuSaveFUl(MenuSave* menu)
{
	State14__8MenuSaveFUl(menu);
}

/**
 * @note Address: 0x8003BE90
 * @note Size: 0x4
 */
extern "C" void State7__8MenuSaveFUl(MenuSave* menu)
{
}

/**
 * @note Address: 0x8003D7A0
 * @note Size: 0x24
 */
extern "C" char* fn_8003D7A0(s32 id)
{
#if defined(VERSION_JP)
	return fn_8006DBF0(id + 13);
#else
	return fn_8006DBF0(id + 94);
#endif
}

/**
 * @note Address: 0x8003D7C4
 * @note Size: 0x18
 */
extern "C" BOOL fn_8003D7C4(MenuSaveSlot* slot)
{
	return slot->mState != 1;
}

/**
 * @note Address: 0x8003D7DC
 * @note Size: 0x44
 */
extern "C" void fn_8003D7DC(MenuSaveSlot* slot)
{
	ClearSaveSlot(slot);
}

/**
 * @note Address: 0x80048EB0
 * @note Size: 0x8
 */
extern "C" s32 fn_80048EB0(Params* params)
{
	return params->mLevel;
}

/**
 * @note Address: 0x80048EB8
 * @note Size: 0x20
 */
extern "C" Params* fn_80048EB8(CGame* game, s32 i)
{
	return &((XObjChr*)game->mXObjs.mPools[2].Get(i))->mParams;
}

/**
 * @note Address: 0x80048ED8
 * @note Size: 0x2C
 */
extern "C" void fn_80048ED8(CGs* gs, u32 color, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1)
{
	fn_801992BC(gs, NULL, x0, y0, z0, color, x1, y1, z1, color);
}

/**
 * @note Address: 0x80048F04
 * @note Size: 0x1E0
 */
extern "C" void fn_80048F04(f32 x, f32 y, f32 alpha)
{
	CTexObj icon = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[119], 0);
	PSMTXScale(icon.mMtx[1], 0.05882353f, 1.0f, 1.0f);
	icon.mMtxDirty = TRUE;
	PSMTXTrans(icon.mMtx[0], 0.11764706f, 0.0f, 0.0f);
	icon.mMtxDirty = TRUE;
	fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
	u32 color = (u32)(255.0f * alpha) | 0xFFFFFF00;
	fn_80197800(&lbl_8025C400.mGs, &icon, color, color, color, color, x, y, 0.0f, 18.0f, 18.0f, 0.0f, 0.0f, 0.0f);
	fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
}

/**
 * @note Address: 0x800490E4
 * @note Size: 0x8
 */
extern "C" CTexObj* fn_800490E4(CGame* game)
{
	return &game->mSubwin.mTexture.mTex;
}

/**
 * @note Address: 0x800490EC
 * @note Size: 0x8
 */
extern "C" s32 fn_800490EC(s32* p)
{
	return *p;
}

/**
 * @note Address: 0x800490F4
 * @note Size: 0x8
 */
extern "C" s32 fn_800490F4(s32* p)
{
	return *p;
}

/**
 * @note Address: 0x800490FC
 * @note Size: 0x3C
 */
extern "C" void fn_800490FC(CTexObj* tex, f32 x, f32 y)
{
	PSMTXTrans(tex->mMtx[0], x, y, 0.0f);
	tex->mMtxDirty = TRUE;
}

/**
 * @note Address: 0x80049138
 * @note Size: 0x3C
 */
extern "C" void fn_80049138(CTexObj* tex, f32 x, f32 y)
{
	PSMTXScale(tex->mMtx[1], x, y, 1.0f);
	tex->mMtxDirty = TRUE;
}

/**
 * @note Address: 0x80049174
 * @note Size: 0x6C
 */
// NONMATCHING: scheduling
extern "C" void fn_80049174(CGs* gs, Vec* quad, u32 color)
{
	fn_80198614(gs, quad[0].x, quad[0].y, quad[0].z, color, quad[1].x, quad[1].y, quad[1].z, color, quad[2].x, quad[2].y, quad[2].z, color,
	            quad[3].x, quad[3].y, quad[3].z, color);
}

/**
 * @note Address: 0x800491E0
 * @note Size: 0x24
 */
extern "C" void fn_800491E0(CGs* gs, u8 width, s32 texOffs)
{
	fn_8019B34C(gs->mCurRenderMode, width, texOffs);
}

/**
 * @note Address: 0x80049204
 * @note Size: 0x24
 */
extern "C" void fn_80049204(CGs* gs, s32 type, s32 src, s32 dst, s32 op)
{
	fn_8019B56C(gs->mCurRenderMode, type, src, dst, op);
}

#if !defined(VERSION_JP)
/**
 * @note Address: 0x80049228
 * @note Size: 0x8
 */
extern "C" u8 fn_80049228(CBase* base)
{
	return base->mLanguage;
}
#endif

/**
 * @note Address: 0x80049230
 * @note Size: 0x2C
 */
extern "C" void fn_80049230(CGs* gs, CTexObj* tex, u32 color, f32 x, f32 y, f32 z, f32 w, f32 h, f32 a, f32 b, f32 c)
{
	fn_80197800(gs, tex, color, color, color, color, x, y, z, w, h, a, b, c);
}

/**
 * @note Address: 0x8004925C
 * @note Size: 0x20
 */
extern "C" u16 fn_8004925C(CTexObj* tex)
{
	return GXGetTexObjHeight(tex);
}

/**
 * @note Address: 0x8004927C
 * @note Size: 0x20
 */
extern "C" u16 fn_8004927C(CTexObj* tex)
{
	return GXGetTexObjWidth(tex);
}

/**
 * @note Address: 0x8004929C
 * @note Size: 0x24
 */
extern "C" void fn_8004929C(CGs* gs, s32 mode)
{
	fn_8019AFA0(gs->mCurTevStage, mode);
}

/**
 * @note Address: 0x800492C0
 * @note Size: 0xA0
 */
extern "C" CTexObj* fn_800492C0(CTexObj* dst, const CTexObj* src)
{
	*dst = *src;
	return dst;
}

/**
 * @note Address: 0x80049360
 * @note Size: 0x38
 */
extern "C" CTexObj* fn_80049360(CGame* game, s32 i)
{
	return &(&game->mTpl)->mTex.Init(&((TPLPalette*)(&game->mTpl)->mBuffer)->descriptorArray[i], 0);
}

/**
 * @note Address: 0x80049398
 * @note Size: 0x38
 */
extern "C" void fn_80049398(CGs* gs, Vec* pos, u32 color, u16 count)
{
	fn_80198D08(gs, GX_TRIANGLEFAN, gs->mCamera->mViewMtx, pos, color, count);
}

/**
 * @note Address: 0x800493D0
 * @note Size: 0x24
 */
extern "C" void fn_800493D0(CGs* gs, u8 enable, s32 func, u8 update)
{
	fn_8019B464(gs->mCurRenderMode, enable, func, update);
}

/**
 * @note Address: 0x800493F4
 * @note Size: 0x4
 */
extern "C" void fn_800493F4(MenuPC* menu)
{
}

/**
 * @note Address: 0x800493F8
 * @note Size: 0x20
 */
extern "C" char* fn_800493F8(s32 id)
{
	return fn_8006DBC8(id);
}

/**
 * @note Address: 0x8004AB3C
 * @note Size: 0x4
 */
extern "C" void State8__6MenuPCFUl(MenuPC* menu)
{
}

/**
 * @note Address: 0x8004B928
 * @note Size: 0x4
 */
extern "C" void State6__6MenuPCFUl(MenuPC* menu)
{
}

/**
 * @note Address: 0x80051C60
 * @note Size: 0x5C
 */
// NONMATCHING: scheduling
extern "C" BOOL fn_80051C60()
{
	BOOL found = FALSE;
	u32 id     = fn_8006DCFC(3);
	if (gEvtFlags[id / 32] & (1 << (id % 32))) {
		found = TRUE;
	}
	return found;
}

/**
 * @note Address: 0x8005875C
 * @note Size: 0x4
 */
extern "C" void State7__10MenuStatusFUl(MenuStatus* menu)
{
}

/**
 * @note Address: 0x80058760
 * @note Size: 0x4
 */
extern "C" void State6__10MenuStatusFUl(MenuStatus* menu)
{
}

/**
 * @note Address: 0x80062390
 * @note Size: 0x48
 */
extern "C" void vfA4__10MenuCursorFUl(MenuCursor* cursor, s32 a)
{
	if (cursor->mStatus.mFlags & 1) {
		vfA4__6CursorFUl(cursor, a);
	}
}

/**
 * @note Address: 0x800623D8
 * @note Size: 0x20
 */
extern "C" void vf98__10MenuCursorFv(MenuCursor* cursor)
{
	vf98__4XObjFv(cursor);
}

/**
 * @note Address: 0x8003CD68
 * @note Size: 0xB0
 */
extern "C" void State0__8MenuSaveFUl(MenuSave* menu)
{
	for (int i = 0; i < 3; i++) {
		ClearSaveSlot(&menu->mSlots[i]);
	}
	fn_80089890(&lbl_8025C400.mMemcard.mCard, menu->mChannel, 1);
	menu->SetMode(1);
#if !defined(VERSION_JP)
#if !defined(VERSION_JP)
	menu->_538 = 0;
#endif
#endif
}

/**
 * @note Address: 0x8004EDE8
 * @note Size: 0x24
 */
extern "C" char* fn_8004EDE8(s32 id)
{
#if defined(VERSION_JP)
	return fn_8006DBF0(id + 102);
#else
	return fn_8006DBF0(id + 45);
#endif
}

/**
 * @note Address: 0x80034E4C
 * @note Size: 0x60
 */
// NONMATCHING: regalloc
extern "C" s16 fn_80034E4C(s16 id)
{
	s16 num         = 0;
	XObjChr* player = lbl_8025C400.GetPlayer();
	for (int i = 255; i >= 0; i--) {
		if (id == player->mParams.mInventory.mItems[i].mId) {
			num = player->mParams.mInventory.mItems[i].mCount;
			break;
		}
	}
	return num;
}

/**
 * @note Address: 0x80035184
 * @note Size: 0x70
 */
extern "C" s32 fn_80035184(s64 a, s64 b, s64 c)
{
	if (a >= b) {
		if (a >= c) {
			return 0;
		}
		return 2;
	}
	if (b >= c) {
		return 1;
	}
	return 2;
}

// the chibi-pc pages and the lists they show
MenuPCPage lbl_80366A6C;
MenuPCPage lbl_80366B70;
u8 lbl_80366C74[24];
u8 lbl_80366C8C[16];
static MenuPCItem lbl_80366C9C[20];
u8 lbl_80366D3C[0x78];
static MenuPCItem lbl_80366DB4[20];

/**
 * @note Address: 0x800345A8
 * @note Size: 0xC4
 */
// NONMATCHING: regalloc
extern "C" s32 fn_800345A8()
{
	s32 count = 0;
	s16 x;
	s16 y;
	MenuPCItem* table = lbl_80366DB4;
	int i             = 0;
	for (; i < 20; table++, i++) {
		x = table->mItem;
		y = table->mCount;
		if (x >= 0 && y >= 0)
			count++;
	}
	return count;
}

/**
 * @note Address: 0x800349A8
 * @note Size: 0xC4
 */
// NONMATCHING: regalloc
extern "C" s32 fn_800349A8()
{
	s16 x;
	s16 y;
	s32 count = 0;
	for (int i = 0; i < 20; i++) {
		x = lbl_80366C9C[i].mItem;
		y = lbl_80366C9C[i].mCount;
		if (x >= 0 && y >= 0) {
			count++;
		}
	}
	return count;
}

/**
 * @note Address: 0x8003466C
 * @note Size: 0x33C
 */
// the shop's page of 9 from the catalog, and the items just before and after it
// NONMATCHING: regalloc
extern "C" void fn_8003466C(MenuPCPage* list)
{
	s16 x;
	s16 y;
	s32 count;
	s32 n;
	s16 price;
	int i;
	for (i = 0; i < 20; i++) {
		list->mRows[i].mItem  = -1;
		list->mRows[i].mCount = -1;
		list->mRows[i].mPrice = -1;
		list->mIndex[i]       = -1;
	}
	count = 0;
	n     = 0;
	for (i = 0; i < 20; i++) {
		x     = lbl_80366DB4[i].mItem;
		y     = lbl_80366DB4[i].mCount;
		price = lbl_80366DB4[i].mPrice;
		if (x >= 0 && y >= 0) {
			if (count >= list->mStart && count < list->mStart + 9 && n < 9) {
				list->mRows[n].mItem  = x;
				list->mRows[n].mCount = y;
				list->mRows[n].mPrice = price;
				list->mIndex[n]       = i;
				n++;
			}
			count++;
		}
	}
	list->mPrev[0].mItem  = -1;
	list->mPrev[0].mCount = -1;
	list->mPrev[0].mPrice = -1;
	list->mNext[0].mItem  = -1;
	list->mNext[0].mCount = -1;
	list->mNext[0].mPrice = -1;
	count                 = 0;
	n                     = 0;
	for (i = 0; i < 20; i++) {
		x     = lbl_80366DB4[i].mItem;
		y     = lbl_80366DB4[i].mCount;
		price = lbl_80366DB4[i].mPrice;
		if (x >= 0 && y >= 0) {
			if (count >= list->mStart + 9 && count < list->mStart + 10) {
				list->mNext[n].mItem  = x;
				list->mNext[n].mCount = y;
				list->mNext[n].mPrice = price;
				if (++n >= 1) {
					break;
				}
			}
			count++;
		}
	}
	count = 0;
	n     = 0;
	for (i = 0; i < 20; i++) {
		x     = lbl_80366DB4[i].mItem;
		y     = lbl_80366DB4[i].mCount;
		price = lbl_80366DB4[i].mPrice;
		if (x >= 0 && y >= 0) {
			if (count >= list->mStart - 1 && count < list->mStart) {
				list->mPrev[n].mItem  = x;
				list->mPrev[n].mCount = y;
				list->mPrev[n].mPrice = price;
				if (++n >= 1) {
					break;
				}
			}
			count++;
		}
	}
}

/**
 * @note Address: 0x80034EAC
 * @note Size: 0x2D8
 */
// the status menu's page of 8 owned items, and the 4 before and after it
// NONMATCHING: regalloc
extern "C" void fn_80034EAC(MenuStatusItems* table)
{
	s32 count;
	s16 num;
	int i;
	InventoryItem* pairs;
	s16 id;
	s32 n;
	for (i = 0; i < 256; i++) {
		table->mItems[i].mItem          = -1;
		table->mItems[i].mCount         = -1;
		table->mItems[i].mStatus.mFlags = 0;
	}
	n     = 0;
	count = 0;
	for (i = 255; i >= 0; i--) {
		pairs = lbl_8025C400.GetPlayer()->mParams.mInventory.mItems;
		id    = pairs[i].mId;
		num   = pairs[i].mCount;
		if (fn_8006E44C(id, 4) && id >= 0 && num > 0) {
			if (count >= table->mPage * 4 && count < (table->mPage + 2) * 4) {
				table->mItems[n].mItem  = id;
				table->mItems[n].mCount = num;
				if (++n >= 8) {
					break;
				}
			}
			count++;
		}
	}
	n     = 0;
	count = 0;
	for (i = 0; i < 4; i++) {
		table->mPrev[i].mItem          = -1;
		table->mPrev[i].mCount         = -1;
		table->mPrev[i].mStatus.mFlags = 0;
		table->mNext[i].mItem          = -1;
		table->mNext[i].mCount         = -1;
		table->mNext[i].mStatus.mFlags = 0;
	}
	for (i = 255; i >= 0; i--) {
		pairs = lbl_8025C400.GetPlayer()->mParams.mInventory.mItems;
		id    = pairs[i].mId;
		num   = pairs[i].mCount;
		if (fn_8006E44C(id, 4) && id >= 0 && num > 0) {
			if (count >= (table->mPage + 2) * 4 && count < (table->mPage + 4) * 4) {
				table->mNext[n].mItem  = id;
				table->mNext[n].mCount = num;
				if (++n >= 4) {
					break;
				}
			}
			count++;
		}
	}
	n     = 0;
	count = 0;
	for (i = 255; i >= 0; i--) {
		pairs = lbl_8025C400.GetPlayer()->mParams.mInventory.mItems;
		id    = pairs[i].mId;
		num   = pairs[i].mCount;
		if (fn_8006E44C(id, 4) && id >= 0 && num > 0) {
			if (count >= (table->mPage - 1) * 4 && count < (table->mPage + 1) * 4) {
				table->mPrev[n].mItem  = id;
				table->mPrev[n].mCount = num;
				if (++n >= 4) {
					break;
				}
			}
			count++;
		}
	}
}

// how many kinds of things the player has
static inline s32 CountMenuItems()
{
	s16 num;
	s16 id;
	s32 count;
	int i;
	InventoryItem* pairs;
	count = 0;
	for (i = 255; i >= 0; i--) {
		pairs = lbl_8025C400.GetPlayer()->mParams.mInventory.mItems;
		id    = pairs[i].mId;
		num   = pairs[i].mCount;
		if (fn_8006E44C(id, 4) && id >= 0 && num > 0) {
			count++;
		}
	}
	return count;
}

/**
 * @note Address: 0x80058C7C
 * @note Size: 0x6AC
 */
// throwing things away: pick the thing, then how many, then confirm
// NONMATCHING: regalloc
extern "C" void State3__10MenuStatusFUl(MenuStatus* menu)
{
	CPad pad(0);
	s32 count = CountMenuItems();
	switch (menu->mPhase) {
	case 0:
		if (count > 1) {
			if (pad.mPad->mRepeat & 0x40001) {
				if (--menu->mItem < 0) {
					menu->mItem = count - 1;
				}
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			} else if (pad.mPad->mRepeat & 0x80002) {
				if (++menu->mItem > count - 1) {
					menu->mItem = 0;
				}
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			}
		}
		if (pad.mPad->mTrigger & 0x200) {
			menu->mPrevMode = menu->mMode;
			menu->mMode     = 4;
			menu->mNextMode = 2;
			menu->mPhase    = 0;
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
			for (int i = 0; i < 4; i++) {
				menu->mTabs2[i]._00 = 0.0f;
				if (menu->_674) {
					menu->mTabs2[i].mX = 198.0f + i * 108 - 8.0f;
				} else {
					menu->mTabs2[i].mX = 252.0f + i * 108;
				}
			}
			menu->_5E0 = 758.0f;
		} else if ((pad.mPad->mTrigger & 0x100) && lbl_80365E08.mItems[menu->mItem].mCount >= 1) {
			menu->_570   = 336.0f;
			menu->_580   = 336.0f;
			menu->mCount = 1;
			menu->mPhase++;
		}
		break;
	case 1: {
		s32 num = lbl_80365E08.mItems[menu->mItem].mCount;
		if (num >= 1) {
			if (pad.mPad->mRepeat & 0x40001) {
				if (--menu->mCount < 1) {
					menu->mCount = num;
				}
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			} else if (pad.mPad->mRepeat & 0x80002) {
				if (++menu->mCount > num) {
					menu->mCount = 1;
				}
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			}
			if (pad.mPad->mTrigger & 0x100) {
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				menu->mPhase++;
			} else if (pad.mPad->mTrigger & 0x200) {
				fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
				menu->_570   = 725.0f;
				menu->_580   = 937.0f;
				menu->mPhase = 0;
			}
		}
		break;
	}
	case 2:
		if (pad.mPad->mRepeat & 0x40001) {
			if (menu->mChoice > 0) {
				menu->mChoice--;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			}
		} else if (pad.mPad->mRepeat & 0x80002) {
			if (menu->mChoice < 1) {
				menu->mChoice++;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			}
		}
		if (pad.mPad->mTrigger & 0x200) {
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
			menu->mPhase = 1;
		} else if (pad.mPad->mTrigger & 0x100) {
			if (menu->mChoice == 0) {
				MenuStatusItem* entry = &lbl_80365E08.mItems[menu->mItem];
				fn_800DC754(&lbl_8025C400.GetPlayer()->mParams.mInventory, entry->mItem, menu->mCount);
				menu->_570   = 725.0f;
				menu->_580   = 937.0f;
				menu->mPhase = 0;
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				menu->mItem = 0;
				fn_80034EAC(&lbl_80365E08);
				menu->_5E0 = 360.0f - (CountMenuItems() - 1) * 54;
				if (CountMenuItems() == 0) {
					menu->_5E0 = 360.0f;
				}
			} else {
				fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
				menu->mPhase = 1;
			}
		}
		break;
	}
}

/**
 * @note Address: 0x80058BBC
 * @note Size: 0xC0
 */
extern "C" void State4__10MenuStatusFUl(MenuStatus* menu, s32 wait)
{
	CPad pad(0);
	switch (menu->mPhase) {
	case 0:
		menu->mSlide = -200.0f;
		menu->mDelay = wait * 4;
		menu->mPhase++;
	case 1:
		if (--menu->mDelay < 0) {
			if (menu->mNextMode != 5) {
				menu->mSlide = 0.0f;
			}
			menu->SetMode(menu->mNextMode);
		}
		break;
	}
}

/**
 * @note Address: 0x8005A290
 * @note Size: 0x110C
 */
// the status menu's tabs: pick one and open its page, or close the menu
// NONMATCHING: regalloc
extern "C" void State1__10MenuStatusFUl(MenuStatus* menu, u32 frames)
{
	CPad pad(0);
	switch (menu->mPhase) {
	case 0:
		if (pad.mPad->mRepeat & 0x40001) {
			if (--menu->mSelection < 0) {
				if (menu->_674) {
					menu->mSelection = 3;
				} else {
					menu->mSelection = 2;
				}
			}
			char* msg       = fn_8006DBF0((menu->mSelection >= 4 ? 0 : sStatusTabMsg[menu->mSelection]) + 0x17);
			menu->mShowHelp = 1;
			menu->mHelp     = msg;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0x80002) {
			if (menu->_674) {
				if (++menu->mSelection > 3) {
					menu->mSelection = 0;
				}
			} else if (++menu->mSelection > 2) {
				menu->mSelection = 0;
			}
			char* msg       = fn_8006DBF0((menu->mSelection >= 4 ? 0 : sStatusTabMsg[menu->mSelection]) + 0x17);
			menu->mShowHelp = 1;
			menu->mHelp     = msg;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		}
		if (pad.mPad->mTrigger & 0x1200) {
			menu->mSlide = -200.0f;
			for (int i = 0; i < 4; i++) {
				menu->mTabs[i].mX = -102.0f;
			}
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
			menu->mPhase++;
			menu->mDelay = frames * 4;
			for (int i = 0; i < 20; i++) {
				menu->mTitle.mChars[i].mHold = 0.0f;
			}
		} else if (pad.mPad->mTrigger & 0x100) {
			switch (menu->mSelection) {
			case 0: {
				menu->mPrevMode = menu->mMode;
				menu->mMode     = 4;
				menu->mNextMode = 2;
				menu->mPhase    = 0;
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				for (int i = 0; i < 4; i++) {
					menu->mTabs[i].mX = -102.0f;
				}
				for (int i = 0; i < 4; i++) {
					menu->mTabs2[i]._00 = 758.0f;
					if (menu->_674) {
						menu->mTabs2[i].mX = 198.0f + i * 108 - 8.0f;
					} else {
						menu->mTabs2[i].mX = 252.0f + i * 108;
					}
				}
				s32 count = CountMenuItems();
				for (int i = 0; i < 4; i++) {
					menu->mTabs2[i].mX = 0.0f;
				}
				menu->_5E0 = 360.0f - (count - 1) * 51;
				if (count == 0) {
					menu->_5E0 = 360.0f;
				}
				menu->_5E0 = 207.0f;
				u32 color  = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
				menu->mTitle.Set(fn_8006DBF0(0x18), color);
				s16 id     = lbl_80365E08.mItems[menu->mItem].mItem;
				menu->_524 = menu->mItem;
				if (id >= 0 && CountMenuItems() > 0) {
					char* msg       = fn_8006E510(id);
					menu->mShowHelp = 1;
					menu->mHelp     = msg;
				} else {
					menu->mShowHelp = 1;
					menu->mHelp     = NULL;
				}
				break;
			}
			case 1: {
				menu->_5F0      = 360.0f;
				menu->mPrevMode = menu->mMode;
				menu->mMode     = 4;
				menu->mNextMode = 9;
				menu->mPhase    = 0;
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				for (int i = 0; i < 4; i++) {
					menu->mTabs[i].mX = -102.0f;
				}
				u32 color = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
				menu->mTitle.Set(fn_8006DBF0(0x1A), color);
				if (lbl_80366C8C[menu->_528]) {
					char* msg       = fn_8006DC8C(menu->_528);
					menu->mShowHelp = 1;
					menu->mHelp     = msg;
				} else {
					char* msg       = fn_8006DBF0(0xE);
					menu->mShowHelp = 1;
					menu->mHelp     = msg;
				}
				break;
			}
			case 2: {
				menu->_610      = 360.0f;
				menu->mPrevMode = menu->mMode;
				menu->mMode     = 4;
				menu->mNextMode = 5;
				menu->mPhase    = 0;
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				for (int i = 0; i < 4; i++) {
					menu->mTabs[i].mX = -102.0f;
				}
				u32 color = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
				menu->mTitle.Set(fn_8006DBF0(0x19), color);
				menu->mShowHelp = 1;
				menu->mHelp     = NULL;
				break;
			}
			case 3: {
				menu->_600      = 360.0f;
				menu->mPrevMode = menu->mMode;
				menu->mMode     = 4;
				menu->mNextMode = 8;
				menu->mPhase    = 0;
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				for (int i = 0; i < 4; i++) {
					menu->mTabs[i].mX = -102.0f;
				}
				u32 color = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
				menu->mTitle.Set(fn_8006DBF0(0x1B), color);
				if (lbl_80366C74[menu->_52C]) {
					char* msg       = fn_8006DDB8(menu->_52C);
					menu->mShowHelp = 1;
					menu->mHelp     = msg;
				} else {
					char* msg       = fn_8006DBF0(0xE);
					menu->mShowHelp = 1;
					menu->mHelp     = msg;
				}
				break;
			}
			default:
				fn_8001E904(&lbl_8025C400.mSound, 0x147, 0xFF);
				break;
			}
		}
		break;
	case 2:
		break;
	case 1:
		if (--menu->mDelay < 0) {
			lbl_8025C400.mStatus.mFlags &= ~2ULL;
			menu->Close();
			sMenuMain.Resume();
			fn_8001E660(&lbl_8025C400.mSound, 0xFFFF0004);
			if (menu->mSavedBgm) {
				fn_8001E904(&lbl_8025C400.mSound, menu->mSavedBgm, menu->mSavedBgmVolume);
			}
			menu->mPhase++;
		}
		break;
	}
}

/**
 * @note Address: 0x8005B39C
 * @note Size: 0x9C
 */
extern "C" void State0__10MenuStatusFUl(MenuStatus* menu)
{
	menu->mSavedBgm       = lbl_8025C400.mSound._2C4 <= 0 ? lbl_8025C400.mSound._2B4 : 0;
	menu->mSavedBgmVolume = lbl_8025C400.mSound._2CC;
	fn_8001E904(&lbl_8025C400.mSound, 0xFFFF0004, 0xFF);
	menu->mPrevMode = menu->mMode;
	menu->mMode     = 4;
	menu->mNextMode = 1;
	menu->mPhase    = 0;
}

/**
 * @note Address: 0x8003B9B8
 * @note Size: 0x160
 */
extern "C" void State9__8MenuSaveFUl(MenuSave* menu, s32 frames)
{
	menu->mDelay -= frames;
	if (menu->mDelay < 0) {
		menu->mDelay = 0;
	}
	switch (lbl_8025C400.mMemcard.mCard.mState) {
	case 0:
		menu->_250 = lbl_8025C400.mMemcard.mCard._16A04;
		menu->_254 = lbl_8025C400.mMemcard.mCard._16A00;
		menu->SetMode(10);
		break;
	case 1:
	case 2:
	case 3:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		s32 result    = lbl_8025C400.mMemcard.mCard.mCard.mResult;
		menu->mResult = result;
		switch (result) {
		case 0:
		case -1:
		case -2:
		case -3:
		case -4:
		case -5:
		case -6:
		case -7:
		case -8:
		case -9:
		case -10:
		case -11:
		case -12:
		case -13:
		case -14:
		case -128:
			if (result == -6 || result == -13) {
				menu->SetMode(13);
				menu->mShowMessage = TRUE;
			} else {
				menu->SetMode(15);
				menu->mMessage     = 8;
				menu->mShowMessage = TRUE;
			}
			break;
		default:
			menu->SetMode(11);
			menu->mOffset = -32.0f;
			break;
		}
		break;
	default:
		menu->SetMode(11);
		menu->mOffset = -32.0f;
		break;
	}
}

/**
 * @note Address: 0x8003CA2C
 * @note Size: 0x33C
 */
// waits for the card to list its files, then fills in the three file headers
// NONMATCHING: regalloc
extern "C" void State1__8MenuSaveFUl(MenuSave* menu, s32 frames)
{
	SavePreviewRecord* save;
	MenuSaveSlot* header;
	Memcard* card = &lbl_8025C400.mMemcard.mCard;
	s32 state     = card->mState;
	menu->mDelay -= frames;
	if (menu->mDelay < 0) {
		menu->mDelay = 0;
	}
	switch (state) {
	case 0:
		for (int i = 0; i < 3; i++) {
			save = &card->mSaves[0][i];
			if (save->mState != 0) {
				header = &menu->mSlots[i];
				OSCalendarTime cal;
				OSTicksToCalendarTime((OSTime)save->mStat.time * OS_TIMER_CLOCK, &cal);
				s32 stage           = save->mHeader.mStage;
				s32 exp             = save->mHeader.mExp;
				s32 level           = save->mHeader.mLevel;
				f32 battery         = save->mHeader.mBattery;
				f32 maxBattery      = save->mHeader.mMaxBattery;
				s32 money           = save->mHeader.mMoney;
				s32 scrap           = save->mHeader.mScrap;
				s32 rank            = save->mHeader.mRank;
				s32 hours           = save->mHeader.mFrames / 216000;
				s32 minutes         = (save->mHeader.mFrames / 3600) % 60;
				s32 timeMode        = save->mHeader.mTimeMode;
				header->mUsed       = 1;
				header->mLevel      = level;
				header->mBattery    = battery;
				header->mMaxBattery = maxBattery;
				header->mMoney      = money;
				header->mExp        = exp;
				header->mScrap      = scrap;
				header->mRank       = rank;
				header->mHours      = hours;
				header->mMinutes    = minutes;
				header->mStage      = stage;
				header->mTimeMode   = timeMode;
				if (save->mHeader._34 > 0) {
					header->mActive = 1;
				} else {
					header->mActive = 0;
				}
				OSCalendarTimeToTicks(&cal);
				if (save->mStatus != 0 || !fn_80086F24(save)) {
					header->mState = 1;
				} else {
					header->mState = 0;
				}
			}
		}
		menu->_250 = lbl_8025C400.mMemcard.mCard._16A04;
		menu->_254 = lbl_8025C400.mMemcard.mCard._16A00;
		if (menu->mSaved == TRUE) {
			menu->SetMode(4);
			menu->mTime  = 30.0f;
			menu->mSaved = 0;
		} else {
			menu->SetMode(2);
		}
		break;
	case 1:
	case 2:
	case 3:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		s32 result    = lbl_8025C400.mMemcard.mCard.mCard.mResult;
		menu->mResult = result;
		switch (result) {
		case 0:
		case -1:
		case -2:
		case -3:
		case -4:
		case -5:
		case -6:
		case -7:
		case -8:
		case -9:
		case -10:
		case -11:
		case -12:
		case -13:
		case -14:
		case -128:
			if (result == -6 || result == -13) {
				menu->SetMode(8);
				menu->mChoice = 1;
			} else if (result == -5) {
				menu->SetMode(13);
				menu->mShowMessage = TRUE;
			} else {
				menu->SetMode(11);
				menu->mOffset = -32.0f;
			}
			menu->mTargetAlpha = 1.0f;
			break;
		default:
			menu->SetMode(11);
			menu->mTargetAlpha = 1.0f;
			menu->mOffset      = -32.0f;
			break;
		}
		break;
	}
}

/**
 * @note Address: 0x8003C0A8
 * @note Size: 0x3C0
 */
// waits for the card to save, then reads back the header of the file it wrote
extern "C" void State5__8MenuSaveFUl(MenuSave* menu, s32 frames)
{
	SavePreviewRecord* save;
	Memcard* card = &lbl_8025C400.mMemcard.mCard;
	s32 state     = card->mState;
	menu->mSaved  = 0;
	menu->mDelay -= frames;
	if (menu->mDelay < 0) {
		menu->mDelay = 0;
	}
	switch (state) {
	case 0: {
		save = &card->mSaves[0][menu->mFile];
		if (save->mState != 0) {
			OSCalendarTime cal;
			OSTicksToCalendarTime((OSTime)save->mStat.time * OS_TIMER_CLOCK, &cal);
			s32 stage                             = save->mHeader.mStage;
			s32 exp                               = save->mHeader.mExp;
			s32 level                             = save->mHeader.mLevel;
			f32 battery                           = save->mHeader.mBattery;
			f32 maxBattery                        = save->mHeader.mMaxBattery;
			s32 money                             = save->mHeader.mMoney;
			s32 scrap                             = save->mHeader.mScrap;
			s32 rank                              = save->mHeader.mRank;
			s32 hours                             = save->mHeader.mFrames / 216000;
			s32 minutes                           = (save->mHeader.mFrames / 3600) % 60;
			s32 timeMode                          = save->mHeader.mTimeMode;
			menu->mSlots[menu->mFile].mUsed       = 1;
			menu->mSlots[menu->mFile].mLevel      = level;
			menu->mSlots[menu->mFile].mBattery    = battery;
			menu->mSlots[menu->mFile].mMaxBattery = maxBattery;
			menu->mSlots[menu->mFile].mMoney      = money;
			menu->mSlots[menu->mFile].mExp        = exp;
			menu->mSlots[menu->mFile].mScrap      = scrap;
			menu->mSlots[menu->mFile].mRank       = rank;
			menu->mSlots[menu->mFile].mHours      = hours;
			menu->mSlots[menu->mFile].mMinutes    = minutes;
			menu->mSlots[menu->mFile].mTimeMode   = timeMode;
			menu->mSlots[menu->mFile].mStage      = stage;
			menu->mSlots[menu->mFile].mState      = 0;
			if (save->mHeader._34 > 0) {
				menu->mSlots[menu->mFile].mActive = 1;
			} else {
				menu->mSlots[menu->mFile].mActive = 0;
			}
		}
		menu->_250 = lbl_8025C400.mMemcard.mCard._16A04;
		menu->_254 = lbl_8025C400.mMemcard.mCard._16A00;
		fn_80089890(card, menu->mChannel, 1);
		menu->SetMode(1);
		menu->mLastFile = menu->mFile;
		menu->mSaved    = 1;
		menu->mOffset   = 0.0f;
		break;
	}
	case 1:
	case 2:
	case 3:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		s32 result    = lbl_8025C400.mMemcard.mCard.mCard.mResult;
		menu->mResult = result;
		switch (result) {
		case 0:
		case -1:
		case -2:
		case -3:
		case -4:
		case -5:
		case -6:
		case -7:
		case -8:
		case -9:
		case -10:
		case -11:
		case -12:
		case -13:
		case -14:
		case -128:
			if (state == 2 && (result == -6 || result == -13)) {
				menu->SetMode(8);
				menu->mChoice      = 1;
				menu->mShowMessage = TRUE;
			} else if (result == -3 || result == -5) {
				menu->SetMode(14);
				menu->mMessage     = 3;
				menu->mShowMessage = TRUE;
			} else {
				menu->SetMode(11);
				menu->mOffset = -32.0f;
			}
			menu->mTargetAlpha = 1.0f;
			break;
		default:
			menu->SetMode(11);
			menu->mTargetAlpha = 1.0f;
			menu->mOffset      = -32.0f;
			break;
		}
		break;
	}
}

/**
 * @note Address: 0x8003C468
 * @note Size: 0x130
 */
extern "C" void State4__8MenuSaveFUl(MenuSave* menu, u32 frames)
{
	CPad pad(0);
	switch (menu->mPhase) {
	case 0:
		menu->mDelay -= frames;
		if (menu->mDelay < 0) {
			menu->mDelay = 0;
			menu->mPhase++;
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
		}
		break;
	case 1:
		if ((pad.mPad->mTrigger & 0x100) || (menu->mTime -= frames) < 0.0f) {
			menu->mTime = 0.0f;
			menu->SetMode(16);
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
		}
		break;
	}
}

/**
 * @note Address: 0x8003B05C
 * @note Size: 0x1F8
 */
extern "C" void State13__8MenuSaveFUl(MenuSave* menu)
{
	CPad pad(0);
	if (menu->mResult == -3) {
		if (menu->mReady == TRUE) {
			for (int i = 0; i < 3; i++) {
				ClearSaveSlot(&menu->mSlots[i]);
			}
			fn_80089890(&lbl_8025C400.mMemcard.mCard, menu->mChannel, 1);
			menu->SetMode(1);
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
#if !defined(VERSION_JP)
#if !defined(VERSION_JP)
			menu->_538 = 0;
#endif
#endif
			return;
		}
	} else if (!menu->mReady) {
		menu->ResetResults();
#if !defined(VERSION_JP)
#if !defined(VERSION_JP)
		menu->_538 = 0;
#endif
#endif
		return;
	}
	if (pad.mPad->mTrigger & 0x100) {
		menu->SetMode(16);
		fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
#if !defined(VERSION_JP)
#if !defined(VERSION_JP)
		menu->_538 = 0;
#endif
#endif
	}
}

/**
 * @note Address: 0x8003B254
 * @note Size: 0x1B8
 */
extern "C" void State12__8MenuSaveFUl(MenuSave* menu)
{
	CPad pad(0);
	if (menu->mResult == -2) {
		if (!menu->_262) {
			menu->ResetResults();
			return;
		}
	} else if (!menu->mReady) {
		menu->ResetResults();
		return;
	}
	if (pad.mPad->mTrigger & 0x100) {
		menu->SetMode(16);
		fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
	}
}

/**
 * @note Address: 0x8003B7AC
 * @note Size: 0x20C
 */
extern "C" void State10__8MenuSaveFUl(MenuSave* menu, u32 frames)
{
	CPad pad(0);
	if (!menu->mReady) {
		menu->ResetResults();
		return;
	}
	switch (menu->mPhase) {
	case 0:
		menu->mDelay -= frames;
		if (menu->mDelay < 0) {
			menu->mDelay = 0;
			menu->mPhase++;
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
		}
		break;
	case 1:
		if (pad.mPad->mTrigger & 0x100) {
			for (int i = 0; i < 3; i++) {
				ClearSaveSlot(&menu->mSlots[i]);
			}
			fn_80089890(&lbl_8025C400.mMemcard.mCard, menu->mChannel, 1);
			menu->SetMode(1);
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			return;
		}
		break;
	}
}

/**
 * @note Address: 0x8003C598
 * @note Size: 0x200
 */
extern "C" void State3__8MenuSaveFUl(MenuSave* menu)
{
	if (!menu->mReady) {
		menu->ResetResults();
		return;
	}
	CPad pad(0);
	if (pad.mPad->mRepeat & 0x40001) {
		if (menu->mChoice > 0) {
			menu->mChoice--;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		}
	} else if (pad.mPad->mRepeat & 0x80002) {
		if (menu->mChoice < 1) {
			menu->mChoice++;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		}
	}
	if (pad.mPad->mTrigger & 0x100) {
		switch (menu->mChoice) {
		case 0:
			fn_8008A1F4(&lbl_8025C400.mMemcard.mCard, menu->mChannel, menu->mFile, 1);
			menu->SetMode(5);
			menu->mOffset = -32.0f;
			menu->mDelay  = 1.5f * FRAME_RATE;
			break;
		case 1:
			menu->SetMode(2);
			break;
		}
		fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
	}
}

/**
 * @note Address: 0x8003ADC8
 * @note Size: 0x294
 */
extern "C" void State14__8MenuSaveFUl(MenuSave* menu)
{
	CPad pad(0);
	if (menu->mResult == -3) {
		if (menu->mReady == TRUE) {
			for (int i = 0; i < 3; i++) {
				ClearSaveSlot(&menu->mSlots[i]);
			}
			fn_80089890(&lbl_8025C400.mMemcard.mCard, menu->mChannel, 1);
			menu->SetMode(1);
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			return;
		}
	} else if (!menu->mReady) {
		menu->ResetResults();
		return;
	}
	if (pad.mPad->mTrigger & 0x100) {
		if (menu->mResult == -3) {
			menu->ResetResults();
		} else if (menu->mResult == -5) {
			menu->SetMode(13);
			menu->mShowMessage = TRUE;
		} else {
			menu->SetMode(13);
		}
		fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
	}
}

/**
 * @note Address: 0x8003BE94
 * @note Size: 0x214
 */
extern "C" void State6__8MenuSaveFUl(MenuSave* menu)
{
	CPad pad(0);
	if (menu->_262 == TRUE) {
		for (int i = 0; i < 3; i++) {
			ClearSaveSlot(&menu->mSlots[i]);
		}
		fn_80089890(&lbl_8025C400.mMemcard.mCard, menu->mChannel, 1);
		menu->SetMode(1);
		fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
		return;
	}
	if (menu->mReady == TRUE) {
		for (int i = 0; i < 3; i++) {
			ClearSaveSlot(&menu->mSlots[i]);
		}
		fn_80089890(&lbl_8025C400.mMemcard.mCard, menu->mChannel, 1);
		menu->SetMode(1);
		fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
		return;
	}
	if (pad.mPad->mTrigger & 0x100) {
		menu->SetMode(16);
		fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
		return;
	}
}

/**
 * @note Address: 0x8003C798
 * @note Size: 0x294
 */
extern "C" void State2__8MenuSaveFUl(MenuSave* menu)
{
	if (!menu->mReady) {
		menu->ResetResults();
		return;
	}
	CPad pad(0);
	if (pad.mPad->mRepeat & 0x40001) {
		menu->mFile--;
		if (menu->mFile < 0) {
			menu->mFile = 2;
		}
		fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
	} else if (pad.mPad->mRepeat & 0x80002) {
		menu->mFile++;
		if (menu->mFile > 2) {
			menu->mFile = 0;
		}
		fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
	}
	if (pad.mPad->mTrigger & 0x100) {
		if (menu->mSlots[menu->mFile].mUsed == 1) {
			menu->mChoice = 1;
			menu->SetMode(3);
		} else if (menu->_250 >= 6 && menu->_254 > 0) {
			fn_8008A1F4(&lbl_8025C400.mMemcard.mCard, menu->mChannel, menu->mFile, 1);
			menu->SetMode(5);
			menu->mDelay = 1.5f * FRAME_RATE;
		} else {
			menu->mResult = -9;
			menu->SetMode(11);
			menu->mOffset = -32.0f;
		}
		fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
	} else if (pad.mPad->mTrigger & 0x200) {
		menu->SetMode(16);
		fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
	}
}

/**
 * @note Address: 0x8003BB18
 * @note Size: 0x378
 */
extern "C" void State8__8MenuSaveFUl(MenuSave* menu)
{
	CPad pad(0);
	if (!menu->mReady) {
		menu->ResetResults();
		return;
	}
	switch (menu->mPhase) {
	case 0:
		if (pad.mPad->mRepeat & 0x40001) {
			if (menu->mChoice > 0) {
				menu->mChoice--;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			}
		} else if (pad.mPad->mRepeat & 0x80002) {
			if (menu->mChoice < 1) {
				menu->mChoice++;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			}
		}
		if (pad.mPad->mTrigger & 0x100) {
			if (menu->mChoice == 0) {
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				menu->mChoice = 1;
				menu->mPhase++;
			} else {
				menu->SetMode(13);
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
#if !defined(VERSION_JP)
#if !defined(VERSION_JP)
				menu->_538 = 1;
#endif
#endif
			}
			return;
		}
		return;
	case 1:
		if (pad.mPad->mRepeat & 0x40001) {
			if (menu->mChoice > 0) {
				menu->mChoice--;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			}
		} else if (pad.mPad->mRepeat & 0x80002) {
			if (menu->mChoice < 1) {
				menu->mChoice++;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			}
		}
		if (pad.mPad->mTrigger & 0x100) {
			if (menu->mChoice == 0) {
				fn_80088E78(&lbl_8025C400.mMemcard.mCard, menu->mChannel, 1);
				menu->SetMode(9);
				menu->mDelay = 1.5f * FRAME_RATE;
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			} else {
				menu->SetMode(13);
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
#if !defined(VERSION_JP)
#if !defined(VERSION_JP)
				menu->_538 = 1;
#endif
#endif
			}
			return;
		}
		break;
	}
}

/**
 * @note Address: 0x8003B40C
 * @note Size: 0x3A0
 */
extern "C" void State11__8MenuSaveFUl(MenuSave* menu)
{
	CPad pad(0);
	if (menu->mResult == -3) {
		if (menu->_262 == TRUE) {
			for (int i = 0; i < 3; i++) {
				ClearSaveSlot(&menu->mSlots[i]);
			}
			fn_80089890(&lbl_8025C400.mMemcard.mCard, menu->mChannel, 1);
			menu->SetMode(1);
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			menu->mOffset = 0.0f;
			return;
		}
		if (menu->mReady == TRUE) {
			for (int i = 0; i < 3; i++) {
				ClearSaveSlot(&menu->mSlots[i]);
			}
			fn_80089890(&lbl_8025C400.mMemcard.mCard, menu->mChannel, 1);
			menu->SetMode(1);
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			menu->mOffset = 0.0f;
			return;
		}
	} else if (menu->mResult == -2) {
		if (!menu->_262) {
			menu->ResetResults();
			return;
		}
	} else if (!menu->mReady) {
		menu->ResetResults();
		return;
	}
	if (pad.mPad->mTrigger & 0x100) {
		if (menu->mResult == -9) {
			menu->SetMode(12);
			menu->mMessage = 2;
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			menu->mOffset = 0.0f;
		} else {
			menu->SetMode(16);
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
		}
	}
}

// the outline offsets of the flying letters, each menu has its own copy
extern "C" f32 lbl_8021238C[4];
extern "C" f32 lbl_8021239C[4];
extern "C" f32 lbl_80211E50[4];
extern "C" f32 lbl_80211E60[4];

// the letters of a line flying in, each with a blob behind it and a dark outline
/**
 * @note Address: 0x800373D4
 * @note Size: 0x5A4
 */
// NONMATCHING: regalloc
extern "C" void fn_800373D4(MenuSave* menu)
{
	if (menu->mTitle.mActive) {
		fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 1, 4, 5, 15);
		fn_80199AB0(&lbl_8025C400.mGs, 0, 0);
		fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 7, 0);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
		CTexObj blob = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[108], 0);
		u32 alpha;
		f32 t;
		f32 s0;
		f32 bob;
		f32 rot;
		f32 s1;
		f32 s2;
		f32 scale;
		f32 x;
		f32 y;
		f32 pad;
		u32 color = menu->mTitle.mColor & 0xFFFFFF00;
		f32 zero  = 0.0f;
		for (int i = 0; i < menu->mTitle.mCount; i++) {
			MenuTitleChar* glyph = &menu->mTitle.mChars[i];
			t                    = glyph->mFade;
			if (!(t <= 0.0f)) {
				CFont font;
				s32 width;
				CTexObj tex = *fn_8019E8F4(&font, glyph->mText, &width);
				s0          = sin(glyph->mAngle0);
				bob         = 4.0f * s0;
				rot         = sin(0.7f * glyph->mAngle1);
				s1          = sin(3.1415927f * t);
				s2          = sin(1.5707964f * t);
				scale       = 0.2f * s1 + s2;
				x           = 24.0f + (48.0f * i + (320.0f - 48.0f * menu->mTitle.mCount * 0.5f));
				alpha       = 255.0f * t;
				rot         = -(0.017453292f * (10.0f * rot));
				y           = 64.0f + bob;
				u32 c       = alpha | 0xFFFFFF00;
				fn_80197800(&lbl_8025C400.mGs, &blob, c, c, c, c, x, y, 0.0f, 20.0f * (2.0f * scale), 20.0f * (2.0f * scale), 0.0f, -zero,
				            rot);
				scale = 20.0f * (1.7f * scale);
				pad   = (width / 24.0f) * scale;
				pad   = scale - pad;
				for (int j = 0; j < 4; j++) {
					fn_80197800(&lbl_8025C400.mGs, &tex, alpha, alpha, alpha, alpha, pad / 2 + (x + lbl_8021238C[j]),
					            bob + (64.0f + lbl_8021239C[j]), 0.0f, scale, scale, 0.0f, -zero, rot);
				}
				c = color | alpha;
				fn_80197800(&lbl_8025C400.mGs, &tex, c, c, c, c, pad / 2 + x, y, 0.0f, scale, scale, 0.0f, -zero, rot);
			}
		}
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
		fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 0, 7, 0);
		fn_80199AB0(&lbl_8025C400.mGs, 0, 0);
	}
}

// the same for the item menu
/**
 * @note Address: 0x8005C798
 * @note Size: 0x5A4
 */
// NONMATCHING: regalloc
extern "C" void fn_8005C798(MenuItem* menu)
{
	if (menu->mTitle.mActive) {
		fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 1, 4, 5, 15);
		fn_80199AB0(&lbl_8025C400.mGs, 0, 0);
		fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 7, 0);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
		CTexObj blob = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[108], 0);
		u32 alpha;
		f32 t;
		f32 s0;
		f32 bob;
		f32 rot;
		f32 s1;
		f32 s2;
		f32 scale;
		f32 x;
		f32 y;
		f32 pad;
		u32 color = menu->mTitle.mColor & 0xFFFFFF00;
		f32 zero  = 0.0f;
		for (int i = 0; i < menu->mTitle.mCount; i++) {
			MenuTitleChar* glyph = &menu->mTitle.mChars[i];
			t                    = glyph->mFade;
			if (!(t <= 0.0f)) {
				CFont font;
				s32 width;
				CTexObj tex = *fn_8019E8F4(&font, glyph->mText, &width);
				s0          = sin(glyph->mAngle0);
				bob         = 4.0f * s0;
				rot         = sin(0.7f * glyph->mAngle1);
				s1          = sin(3.1415927f * t);
				s2          = sin(1.5707964f * t);
				scale       = 0.2f * s1 + s2;
				x           = 24.0f + (48.0f * i + (320.0f - 48.0f * menu->mTitle.mCount * 0.5f));
				alpha       = 255.0f * t;
				rot         = -(0.017453292f * (10.0f * rot));
				y           = 64.0f + bob;
				u32 c       = alpha | 0xFFFFFF00;
				fn_80197800(&lbl_8025C400.mGs, &blob, c, c, c, c, x, y, 0.0f, 20.0f * (2.0f * scale), 20.0f * (2.0f * scale), 0.0f, -zero,
				            rot);
				scale = 20.0f * (1.7f * scale);
				pad   = (width / 24.0f) * scale;
				pad   = scale - pad;
				for (int j = 0; j < 4; j++) {
					fn_80197800(&lbl_8025C400.mGs, &tex, alpha, alpha, alpha, alpha, pad / 2 + (x + lbl_80211E50[j]),
					            bob + (64.0f + lbl_80211E60[j]), 0.0f, scale, scale, 0.0f, -zero, rot);
				}
				c = color | alpha;
				fn_80197800(&lbl_8025C400.mGs, &tex, c, c, c, c, pad / 2 + x, y, 0.0f, scale, scale, 0.0f, -zero, rot);
			}
		}
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
		fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 0, 7, 0);
		fn_80199AB0(&lbl_8025C400.mGs, 0, 0);
	}
}

/**
 * @note Address: 0x8003EF10
 * @note Size: 0x56C
 */
// a number of up to 7 digits, right aligned, and the money icon in front
extern "C" void fn_8003EF10(bool icon, f32 x, f32 y, f32 alpha, f32 scale, f32 value)
{
	CTexObj tex;
	if (icon) {
		tex = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[153], 0);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
		u16 w     = GXGetTexObjWidth(&tex);
		u16 h     = GXGetTexObjHeight(&tex);
		u32 color = (u32)(255.0f * alpha) | 0xC8C8C800;
		fn_80197800(&lbl_8025C400.mGs, &tex, color, color, color, color, x - 16.0f, y, 0.0f, 0.65f * (scale * w), 0.65f * (scale * h), 0.0f,
		            0.0f, 0.0f);
	}
	tex   = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[125], 0);
	f32 w = scale * GXGetTexObjWidth(&tex);
	f32 h = scale * (GXGetTexObjHeight(&tex) / 10);
	PSMTXScale(tex.mMtx[1], 1.0f, 0.1f, 1.0f);
	tex.mMtxDirty = TRUE;
	u8 a          = 255.0f * alpha;
	for (int i = 0; i < 7; i++) {
		u32 top    = a | 0xCECEFF00;
		u32 bottom = a | 0x5959FF00;
		f32 div    = 1.0f;
		if (i > 0) {
			for (int j = 0; j < i; j++) {
				div *= 10.0f;
			}
		}
		s32 n = value / div;
		if (i == 0 || n != 0) {
			n %= 10;
			PSMTXTrans(tex.mMtx[0], 0.0f, 0.1f * n, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
			fn_80197800(&lbl_8025C400.mGs, &tex, top, top, bottom, bottom, 18.0f * scale + (7.0f * w + (x - i * w) - 8.0f * scale), y, 0.0f,
			            w, h, 0.0f, 0.0f, 0.0f);
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
		}
	}
}

/**
 * @note Address: 0x8003F47C
 * @note Size: 0x56C
 */
// the same with the other icon and colors
extern "C" void fn_8003F47C(bool icon, f32 x, f32 y, f32 alpha, f32 scale, f32 value)
{
	CTexObj tex;
	if (icon) {
		tex = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[152], 0);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
		u16 w     = GXGetTexObjWidth(&tex);
		u16 h     = GXGetTexObjHeight(&tex);
		u32 color = (u32)(255.0f * alpha) | 0xC8C8C800;
		fn_80197800(&lbl_8025C400.mGs, &tex, color, color, color, color, x - 16.0f, y, 0.0f, 0.8f * (scale * w), 0.8f * (scale * h), 0.0f,
		            0.0f, 0.0f);
	}
	tex   = (&lbl_8025C400.mTpl)->mTex.Init(&((TPLPalette*)(&lbl_8025C400.mTpl)->mBuffer)->descriptorArray[125], 0);
	f32 w = scale * GXGetTexObjWidth(&tex);
	f32 h = scale * (GXGetTexObjHeight(&tex) / 10);
	PSMTXScale(tex.mMtx[1], 1.0f, 0.1f, 1.0f);
	tex.mMtxDirty = TRUE;
	u8 a          = 255.0f * alpha;
	for (int i = 0; i < 7; i++) {
		u32 top    = a | 0xEFEBC500;
		u32 bottom = a | 0xEDA83600;
		f32 div    = 1.0f;
		if (i > 0) {
			for (int j = 0; j < i; j++) {
				div *= 10.0f;
			}
		}
		s32 n = value / div;
		if (i == 0 || n != 0) {
			n %= 10;
			PSMTXTrans(tex.mMtx[0], 0.0f, 0.1f * n, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
			fn_80197800(&lbl_8025C400.mGs, &tex, top, top, bottom, bottom, 18.0f * scale + (7.0f * w + (x - i * w) - 8.0f * scale), y, 0.0f,
			            w, h, 0.0f, 0.0f, 0.0f);
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
		}
	}
}

/**
 * @note Address: 0x80049418
 * @note Size: 0xF0
 */
extern "C" void State11__6MenuPCFUl(MenuPC* menu)
{
	if (menu->mFade <= 2.0f) {
		lbl_8025C400.mEvt._10 = menu->mExitState;
		menu->Close();
		sMenuMain.Resume();
		fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
		lbl_8025C400.mStatus.mFlags &= ~2ULL;
		fn_8001E660(&lbl_8025C400.mSound, 0x30008);
	}
}

// the status screen's item strip, europe scales it to the screen
static inline void SetScissor(u32 top, u32 height)
{
#if defined(VERSION_PAL)
	f32 scale     = lbl_8025C400.mGs.mScreen._40;
	u32 scaledH   = (u32)((f32)height * scale);
	u32 scaledTop = (u32)((f32)top * scale);
	GXSetScissor(0, scaledTop, 640, scaledH);
#else
	GXSetScissor(0, top, 640, height);
#endif
}

/**
 * @note Address: 0x8005773C
 * @note Size: 0x1F8
 */
// NONMATCHING: the item table address is formed differently
extern "C" BOOL Draw__10MenuStatusFv(MenuStatus* menu)
{
	if (menu->CMenu::Draw()) {
		fn_80054314(menu->mAlpha);
		fn_80056608(menu);
		if (menu->mMode == 2 || menu->mMode == 4) {
			SetScissor(104, 248);
			f32 x    = (menu->mX - 42.0f) + menu->mOffsetX;
			f32 y    = (180.0f + menu->mY) + menu->mOffsetY;
			s32 item = menu->mPhase == 100 ? -1 : menu->mItem;
			fn_8005D148(item, x, y, menu->mAlpha, 1.0f, menu->_694);
			fn_8005CD3C(lbl_80365E08.mItems[menu->mItem].mItem, 320.0f, 224.0f, menu->mAlpha, menu->_66C);
			for (s32 i = 255; i >= 0; --i) {
				s16 id = lbl_8025C400.GetPlayer()->mParams.mInventory.mItems[i].mId;
				fn_8006E44C(id, 4);
			}
			fn_8004EE0C(menu);
			SetScissor(0, 448);
		}
		fn_800559C4(menu);
		fn_800554A8(menu);
		fn_8004F930(menu);
		fn_80053714(menu);
		fn_80053D58(menu);
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x80057084
 * @note Size: 0x294
 */
// two byte text a character at a time, digits and narrow letters moved to the middle of their cell
// NONMATCHING: regalloc
extern "C" void fn_80057084(const char* text, f32 x, f32 y, f32 z, f32 w, f32 h, f32 alpha)
{
	const char* p;
	u32 shadow;
	u32 color;
	u32 count;
	int i;
	p      = text;
	count  = strlen(text) / 2;
	shadow = (u32)(128.0f * alpha) | 0xFFFFFF00;
	color  = (u32)(255.0f * alpha) | 0x20202000;
	for (i = 0; i < count; i++, p += 2) {
		char c[3];
		char buf[4];
		c[0]     = p[0];
		c[1]     = p[1];
		c[2]     = 0;
		f32 left = i * w + x;
		fn_80057318(shadow, left, y, z, w, h, alpha);
		f32 shift = 0.0f;
		for (int digit = 0; digit < 10; digit++) {
			sprintf(buf, "%d", digit);
			fn_80066630(buf);
			if (strcmp(c, buf) == 0) {
				if (digit == 1) {
					shift = 6.0f;
				} else {
					shift = 3.0f;
				}
				break;
			}
		}
		for (int letter = 0; letter < 26; letter++) {
			buf[0] = 'A' + letter;
			buf[1] = 0;
			fn_80066630(buf);
			if (strcmp(c, buf) == 0) {
				switch (letter) {
				case 'B' - 'A':
				case 'E' - 'A':
				case 'F' - 'A':
				case 'J' - 'A':
				case 'K' - 'A':
				case 'L' - 'A':
				case 'P' - 'A':
				case 'R' - 'A':
				case 'S' - 'A':
				case 'T' - 'A':
					shift = 6.0f;
					break;
				case 'I' - 'A':
					shift = 12.0f;
					break;
				case 'C' - 'A':
				case 'H' - 'A':
				case 'U' - 'A':
				case 'X' - 'A':
					shift = 4.5f;
					break;
				default:
					shift = 3.0f;
					break;
				}
				break;
			}
		}
		fn_80196610(&lbl_8025C400, color, c, shift + left - w / 2, y - h / 2, 0.0f, w, h);
	}
}

// empties one entry of the status table
static inline void ClearStatusEntry(MenuStatusItem* e)
{
	e->mItem          = -1;
	e->mCount         = -1;
	e->mStatus.mFlags = 0;
}

static inline void ClearStatusTable(MenuStatusItems* table)
{
	table->mPage = 0;
	for (int i = 0; i < 256; i++) {
		ClearStatusEntry(&table->mItems[i]);
	}
	for (int i = 0; i < 4; i++) {
		ClearStatusEntry(&table->mPrev[i]);
		ClearStatusEntry(&table->mNext[i]);
	}
}

/**
 * @note Address: 0x80060118
 * @note Size: 0x650
 */
// NONMATCHING: regalloc
extern "C" void Open__8MenuItemFv(MenuItem* menu)
{
	menu->mState = 1;
	menu->Resume();
	menu->mWindow.mOpen = TRUE;
	lbl_8025C400.mStatus.mFlags |= 2;
	menu->mWindow.SetSize(0.0f, 0.0f);
	menu->mWindow.SetPos(0.0f, 0.0f);
	ClearStatusTable(&lbl_80365E08);
	fn_80034EAC(&lbl_80365E08);
	s32 count  = CountMenuItems();
	menu->_160 = 758.0f;
	menu->_164 = 36.0f;
	if (count == 0) {
		menu->_168 = 360.0f;
	} else {
		menu->_168 = 360.0f - (count - 1) * 51;
	}
	menu->_168       = 207.0f;
	menu->_16C       = 36.0f;
	menu->_170       = -10.0f;
	menu->_174       = -200.0f;
	menu->_178       = -10.0f;
	menu->_17C       = 0.0f;
	menu->_158       = 0;
	menu->_15C       = 0;
	menu->mSelection = 0;
	menu->_19C       = menu->mSelection;
	menu->_190       = 0.0f;
	menu->_194       = 0.0f;
	u32 color        = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
	menu->mTitle.Set(fn_8006DBF0(0x18), color);
	menu->mShowHelp = 0;
	menu->_1A4      = 0.0f;
	s16 id          = lbl_80365E08.mItems[menu->mSelection].mItem;
	if (id >= 0 && CountMenuItems() > 0) {
		menu->mName     = fn_8006E510(id);
		char* msg       = fn_8006E510(id);
		menu->mShowHelp = 1;
		menu->mHelp     = msg;
	} else {
		menu->mShowHelp = 1;
		menu->mHelp     = NULL;
	}
	menu->_350 = 0;
	for (int i = 0; i < 20; i++) {
		menu->mBg[i].mAngle0 = 0.017453292f * (rand() % 360);
		menu->mBg[i].mAngle1 = 0.017453292f * (rand() % 360);
	}
}

/**
 * @note Address: 0x80057934
 * @note Size: 0x700
 */
// moving the cursor over the map grid, each cell names its place once it's been visited
extern "C" void State9__10MenuStatusFUl(MenuStatus* menu)
{
	CPad pad(0);
	s32 grid[13][4] = {
		{ 4, 2, 9, 5 },  { 0, 2, 10, 6 }, { 0, 3, 10, 6 },  { 2, 4, 11, 7 }, { 3, 0, 12, 8 },  { 8, 6, 0, 9 },  { 5, 7, 2, 10 },
		{ 6, 8, 3, 11 }, { 7, 5, 4, 12 }, { 12, 10, 5, 0 }, { 9, 11, 6, 2 }, { 10, 12, 7, 3 }, { 11, 9, 8, 4 },
	};
	switch (menu->mPhase) {
	case 0:
		if (pad.mPad->mRepeat & 0x40001) {
			menu->_528 = grid[menu->_528][2];
			if (lbl_80366C8C[menu->_528]) {
				char* msg       = fn_8006DC8C(menu->_528);
				menu->mShowHelp = 1;
				menu->mHelp     = msg;
			} else {
				char* msg       = fn_8006DBF0(0xE);
				menu->mShowHelp = 1;
				menu->mHelp     = msg;
			}
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0x80002) {
			menu->_528 = grid[menu->_528][3];
			if (lbl_80366C8C[menu->_528]) {
				char* msg       = fn_8006DC8C(menu->_528);
				menu->mShowHelp = 1;
				menu->mHelp     = msg;
			} else {
				char* msg       = fn_8006DBF0(0xE);
				menu->mShowHelp = 1;
				menu->mHelp     = msg;
			}
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		}
		if (pad.mPad->mRepeat & 0x10008) {
			menu->_528 = grid[menu->_528][0];
			if (lbl_80366C8C[menu->_528]) {
				char* msg       = fn_8006DC8C(menu->_528);
				menu->mShowHelp = 1;
				menu->mHelp     = msg;
			} else {
				char* msg       = fn_8006DBF0(0xE);
				menu->mShowHelp = 1;
				menu->mHelp     = msg;
			}
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0x20004) {
			menu->_528 = grid[menu->_528][1];
			if (lbl_80366C8C[menu->_528]) {
				char* msg       = fn_8006DC8C(menu->_528);
				menu->mShowHelp = 1;
				menu->mHelp     = msg;
			} else {
				char* msg       = fn_8006DBF0(0xE);
				menu->mShowHelp = 1;
				menu->mHelp     = msg;
			}
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		}
		if (pad.mPad->mTrigger & 0x200) {
			menu->_5F0 = 1086.0f;
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
			for (int i = 0; i < 4; i++) {
				menu->mTabs[i]._00 = 0.0f;
				if (menu->_674) {
					menu->mTabs[i].mX = 198.0f + i * 108 - 8.0f;
				} else {
					menu->mTabs[i].mX = 252.0f + i * 108;
				}
			}
			menu->_658      = 0.0f;
			menu->mPrevMode = menu->mMode;
			menu->mMode     = 4;
			menu->mNextMode = 1;
			menu->mPhase    = 0;
			u32 color       = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
			menu->mTitle.Set(fn_8006DBF0(0x17), color);
			char* msg       = fn_8006DBF0((menu->mSelection >= 4 ? 0 : sStatusTabMsg[menu->mSelection]) + 0x17);
			menu->mShowHelp = 1;
			menu->mHelp     = msg;
			menu->_51C      = 0;
		}
		break;
	case 100:
		if (pad.mPad->mTrigger & 0x300) {
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			menu->mPhase = 0;
			menu->_670   = 0.0f;
		}
		break;
	}
}

/**
 * @note Address: 0x80058034
 * @note Size: 0x728
 */
// the same on the 6 by 4 grid, it wraps around
extern "C" void State8__10MenuStatusFUl(MenuStatus* menu)
{
	CPad pad(0);
	switch (menu->mPhase) {
	case 0:
		if (pad.mPad->mRepeat & 0x40001) {
			if (--menu->_52C < 0) {
				menu->_52C = 23;
			}
			if (lbl_80366C74[menu->_52C]) {
				char* msg       = fn_8006DDB8(menu->_52C);
				menu->mShowHelp = 1;
				menu->mHelp     = msg;
			} else {
				char* msg       = fn_8006DBF0(0xE);
				menu->mShowHelp = 1;
				menu->mHelp     = msg;
			}
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0x80002) {
			menu->_52C++;
			if (menu->_52C > 23) {
				menu->_52C = 0;
			}
			if (lbl_80366C74[menu->_52C]) {
				char* msg       = fn_8006DDB8(menu->_52C);
				menu->mShowHelp = 1;
				menu->mHelp     = msg;
			} else {
				char* msg       = fn_8006DBF0(0xE);
				menu->mShowHelp = 1;
				menu->mHelp     = msg;
			}
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		}
		if (pad.mPad->mRepeat & 0x10008) {
			s32 row = menu->_52C / 6;
			s32 col = menu->_52C % 6;
			if (--row < 0) {
				row = 3;
			}
			menu->_52C = col + row * 6;
			if (lbl_80366C74[menu->_52C]) {
				char* msg       = fn_8006DDB8(menu->_52C);
				menu->mShowHelp = 1;
				menu->mHelp     = msg;
			} else {
				char* msg       = fn_8006DBF0(0xE);
				menu->mShowHelp = 1;
				menu->mHelp     = msg;
			}
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0x20004) {
			s32 row = menu->_52C / 6;
			s32 col = menu->_52C % 6;
			if (++row > 3) {
				row = 0;
			}
			menu->_52C = col + row * 6;
			if (lbl_80366C74[menu->_52C]) {
				char* msg       = fn_8006DDB8(menu->_52C);
				menu->mShowHelp = 1;
				menu->mHelp     = msg;
			} else {
				char* msg       = fn_8006DBF0(0xE);
				menu->mShowHelp = 1;
				menu->mHelp     = msg;
			}
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		}
		if (pad.mPad->mTrigger & 0x200) {
			menu->_600 = 959.0f;
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
			for (int i = 0; i < 4; i++) {
				menu->mTabs[i]._00 = 0.0f;
				if (menu->_674) {
					menu->mTabs[i].mX = 198.0f + i * 108 - 8.0f;
				} else {
					menu->mTabs[i].mX = 252.0f + i * 108;
				}
			}
			menu->mPrevMode = menu->mMode;
			menu->mMode     = 4;
			menu->mNextMode = 1;
			menu->mPhase    = 0;
			u32 color       = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
			menu->mTitle.Set(fn_8006DBF0(0x17), color);
			char* msg       = fn_8006DBF0((menu->mSelection >= 4 ? 0 : sStatusTabMsg[menu->mSelection]) + 0x17);
			menu->mShowHelp = 1;
			menu->mHelp     = msg;
			menu->_51C      = 0;
		}
		break;
	case 100:
		if (pad.mPad->mTrigger & 0x300) {
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			menu->mPhase = 0;
			menu->_670   = 0.0f;
		}
		break;
	}
}

/**
 * @note Address: 0x80058764
 * @note Size: 0x458
 */
extern "C" void State5__10MenuStatusFUl(MenuStatus* menu)
{
	CPad pad(0);
	if (pad.mPad->mTrigger & 0x200) {
		menu->_610 = 1086.0f;
		fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
		for (int i = 0; i < 4; i++) {
			menu->mTabs[i]._00 = 0.0f;
			if (menu->_674) {
				menu->mTabs[i].mX = 198.0f + i * 108 - 8.0f;
			} else {
				menu->mTabs[i].mX = 252.0f + i * 108;
			}
		}
		menu->mPrevMode = menu->mMode;
		menu->mMode     = 4;
		menu->mNextMode = 1;
		menu->mPhase    = 0;
		u32 color       = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
		menu->mTitle.Set(fn_8006DBF0(0x17), color);
		char* msg       = fn_8006DBF0((menu->mSelection >= 4 ? 0 : sStatusTabMsg[menu->mSelection]) + 0x17);
		menu->mShowHelp = 1;
		menu->mHelp     = msg;
		menu->_51C      = 0;
	}
}

extern "C" f32 lbl_8065B1A0;
extern "C" void fn_80068C8C(f32*, f32*, f32);

static inline void ResetPCText(MenuPC* menu, s32 step)
{
	menu->mPrevMode  = menu->mMode;
	menu->mMode      = step;
	menu->mPhase     = 0;
	menu->mTextPhase = 0;
	menu->mTextX     = 210.0f;
	menu->mTextY     = 129.0f;
	menu->mTargetX   = menu->mTextX;
	menu->mTargetY   = menu->mTextY;
	switch (menu->mMode) {
	case 3:
	case 7:
	case 10:
		menu->mTextY   = 121.0f;
		menu->mTargetY = menu->mTextY;
		break;
	}
	menu->mCharIndex = 0;
	menu->_378       = 0;
	menu->_37C       = 0;
	char space[3]    = "\x81\x40";
	for (int i = 0; i < 40; i++)
		menu->mChars[i] = *(u16*)space;
	menu->mTerminator = 0;
	for (int i = 0; i < 40; i++) {
		menu->mCharX[i]             = 640.0f;
		menu->mCharFlags[i]         = 0;
		*(u16*)menu->mCharCopies[i] = *(u16*)space;
		menu->mCharCopies[i][2]     = 0;
	}
	menu->_364   = 0;
	menu->_368   = 0;
	menu->_36C   = 0;
	menu->_370   = 0;
	menu->_374   = 0;
	lbl_8065B1A0 = 640.0f;
	memset(lbl_803620A8, 0, 0x200);
}

/**
 * @note Address: 0x8004BBC0
 * @note Size: 0x288
 */
extern "C" void State4__6MenuPCFUl(MenuPC* menu)
{
	CPad pad(0);
	if (pad.mPad->mTrigger & 0x200) {
		ResetPCText(menu, 2);
		fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
	}
}

/**
 * @note Address: 0x8004B92C
 * @note Size: 0x294
 */
extern "C" void State5__6MenuPCFUl(MenuPC* menu)
{
	CPad pad(0);
	if (pad.mPad->mTrigger & 0x200) {
		lbl_8025C400.mSubwin.mEnabled = 0;
		ResetPCText(menu, 2);
		fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
	}
}

/**
 * @note Address: 0x8004D584
 * @note Size: 0x2D8
 */
extern "C" void State1__6MenuPCFUl(MenuPC* menu, u32 frames)
{
	switch (menu->mPhase) {
	case 0:
		fn_8001E904(&lbl_8025C400.mSound, 0x30008, 0xFF);
		menu->mPhase++;
		break;
	case 1:
		break;
	}
	fn_80068C8C(&menu->mProgress, &menu->mTargetProgress, menu->mSpeed * frames);
	if (menu->mProgress >= 1.0f)
		ResetPCText(menu, 2);
}

/**
 * @note Address: 0x8004D85C
 * @note Size: 0x4C0
 */
extern "C" void State0__6MenuPCFUl(MenuPC* menu)
{
	if (menu->_3A0) {
		ResetPCText(menu, 3);
	} else {
		ResetPCText(menu, 1);
		menu->mProgress       = 0.0f;
		menu->mTargetProgress = 1.0f;
		switch (rand() % 2) {
		case 0:
			menu->mSpeed = 0.04f;
			break;
		case 1:
			menu->mSpeed = 0.08f;
			break;
		}
	}
}

extern "C" s32 gEvtVars[];

static inline s16 MenuHistoryValue(int i)
{
	return lbl_80366C9C[i].mCount;
}

static inline void RestorePCHistory(MenuPC* menu)
{
	if (menu->mHistoryCount > 0) {
		for (int i = 0; i < menu->mHistoryCount; i++) {
			gEvtVars[menu->mHistoryStart + i + menu->mHistoryCount] = MenuHistoryValue(i);
		}
		gEvtVars[196] = menu->_384;
	}
}

/**
 * @note Address: 0x8004CD38
 * @note Size: 0x84C
 */
extern "C" void State2__6MenuPCFUl(MenuPC* menu)
{
	CPad pad(0);
	if (pad.mPad->mRepeat & 0x10008) {
		menu->mChoice--;
		if (menu->mChoice < 0)
			menu->mChoice = menu->mChoiceCount - 1;
		fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
	} else if (pad.mPad->mRepeat & 0x20004) {
		menu->mChoice++;
		if (menu->mChoice > menu->mChoiceCount - 1)
			menu->mChoice = 0;
		fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
	} else if (pad.mPad->mTrigger & 0x100) {
		s32 step = menu->mChoices[menu->mChoice];
		if (step == 7) {
			if (menu->mCanSelect) {
				ResetPCText(menu, 7);
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			} else {
				fn_8001E904(&lbl_8025C400.mSound, 0x85, 0xFF);
			}
		} else if (step >= 0) {
			ResetPCText(menu, step);
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
		}
	} else if (pad.mPad->mTrigger & 0x200) {
		ResetPCText(menu, 11);
		menu->_1A4 = 0.0f;
		RestorePCHistory(menu);
	}
}

static inline s32 MenuPower10(s32 exponent)
{
	s32 result = 1;
	for (s32 i = 0; i < exponent; i++)
		result *= 10;
	return result;
}
static inline s32 MenuDigit(s32 value, s32 digit)
{
	if (digit == 0)
		return value % 10;
	return (value / MenuPower10(digit)) % 10;
}
// chibi robo's money and scrap
static inline s32 GetMoney()
{
	return lbl_8025C400.GetPlayer()->mParams.mMoney;
}
static inline s32 GetScrap()
{
	return lbl_8025C400.GetPlayer()->mParams.mScrap;
}

static inline void DepositPCAmount(MenuPC* menu)
{
	s32 deposit = menu->mAmount;
	s32 money   = GetMoney();
	money -= deposit;
	menu->_384 += deposit;
	lbl_8025C400.GetPlayer()->mParams.SetMoney(money);
	menu->mDisplayMoney = money;
}

static inline void RecyclePCAmount(MenuPC* menu)
{
	s32 scrap = GetScrap() - menu->mAmount;
	lbl_8025C400.GetPlayer()->mParams.SetScrap(scrap);
	s32 money = GetMoney() + menu->mAmount * menu->mScrapRate;
	lbl_8025C400.GetPlayer()->mParams.SetMoney(money);
	menu->mDisplayScrap = scrap;
	menu->mDisplayMoney = money;
}

/**
 * @note Address: 0x8004A188
 * @note Size: 0x9B4
 */
// NONMATCHING: regalloc
extern "C" void State9__6MenuPCFUl(MenuPC* menu)
{
	CPad pad(0);
	s32 digits[7] = { 0, 0, 0, 0, 0, 0, 0 };
	switch (menu->mPhase) {
	case 0:
		if (pad.mPad->mRepeat & 0x80002) {
			menu->mMoneyDigit--;
			if (menu->mMoneyDigit < 0)
				menu->mMoneyDigit = 4;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0x40001) {
			menu->mMoneyDigit++;
			if (menu->mMoneyDigit > 4)
				menu->mMoneyDigit = 0;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0x20004) {
			s32 value = 0;
			for (s32 i = 0; i < 7; i++) {
				if (i == 0)
					digits[i] = menu->mAmount % 10;
				else {
					s32 power = 1;
					for (s32 j = 0; j < i; j++)
						power *= 10;
					digits[i] = (menu->mAmount / power) % 10;
				}
			}
			digits[menu->mMoneyDigit]--;
			if (digits[menu->mMoneyDigit] < 0)
				digits[menu->mMoneyDigit] = 9;
			for (s32 i = 0; i < 7; i++) {
				s32 power = 1;
				if (i > 0) {
					for (s32 j = 0; j < i; j++)
						power *= 10;
				}
				value += power * digits[i];
			}
			menu->mAmount = value;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0x10008) {
			s32 value = 0;
			for (s32 i = 0; i < 7; i++) {
				if (i == 0)
					digits[i] = menu->mAmount % 10;
				else {
					s32 power = 1;
					for (s32 j = 0; j < i; j++)
						power *= 10;
					digits[i] = (menu->mAmount / power) % 10;
				}
			}
			digits[menu->mMoneyDigit]++;
			if (digits[menu->mMoneyDigit] > 9)
				digits[menu->mMoneyDigit] = 0;
			for (s32 i = 0; i < 7; i++) {
				s32 power = 1;
				if (i > 0) {
					for (s32 j = 0; j < i; j++)
						power *= 10;
				}
				value += power * digits[i];
			}
			menu->mAmount = value;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mTrigger & 0x200) {
			ResetPCText(menu, 2);
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
		} else if (pad.mPad->mTrigger & 0x100) {
			if (menu->mAmount > GetMoney()) {
				menu->mAmount = GetMoney();
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				return;
			}
			if (menu->mAmount == 0) {
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				return;
			}
			menu->mPhase = 1;
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
		}
		break;
	case 1:
		if (pad.mPad->mRepeat & 0x40001) {
			if (menu->mYesNo > 0) {
				menu->mYesNo--;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			}
		} else if (pad.mPad->mRepeat & 0x80002) {
			if (menu->mYesNo < 1) {
				menu->mYesNo++;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			}
		}
		if (pad.mPad->mTrigger & 0x100) {
			switch (menu->mYesNo) {
			case 0: {
				DepositPCAmount(menu);
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				menu->mPhase = 2;
				break;
			}
			case 1:
				menu->mPhase = 0;
				fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
				break;
			}
		} else if (pad.mPad->mTrigger & 0x200) {
			menu->mPhase = 0;
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
		}
		break;
	case 2:
		if (pad.mPad->mTrigger & 0x300) {
			menu->mPhase = 0;
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
		}
		break;
	}
}

/**
 * @note Address: 0x80049508
 * @note Size: 0xC80
 */
// NONMATCHING: regalloc
extern "C" void State10__6MenuPCFUl(MenuPC* menu)
{
	CPad pad(0);
	s32 digits[7] = { 0, 0, 0, 0, 0, 0, 0 };
	switch (menu->mPhase) {
	case 0:
		menu->mYesNo = -1;
		if (pad.mPad->mRepeat & 0x80002) {
			menu->mScrapDigit--;
			if (menu->mScrapDigit < 0)
				menu->mScrapDigit = 4;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0x40001) {
			menu->mScrapDigit++;
			if (menu->mScrapDigit > 4)
				menu->mScrapDigit = 0;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0x20004) {
			s32 value = 0;
			for (s32 i = 0; i < 5; i++) {
				if (i == 0)
					digits[i] = menu->mAmount % 10;
				else {
					s32 power = 1;
					for (s32 j = 0; j < i; j++)
						power *= 10;
					digits[i] = (menu->mAmount / power) % 10;
				}
			}
			digits[menu->mScrapDigit]--;
			if (digits[menu->mScrapDigit] < 0)
				digits[menu->mScrapDigit] = 9;
			for (s32 i = 0; i < 5; i++) {
				s32 power = 1;
				if (i > 0) {
					for (s32 j = 0; j < i; j++)
						power *= 10;
				}
				value += power * digits[i];
			}
			menu->mAmount = value;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0x10008) {
			s32 value = 0;
			for (s32 i = 0; i < 5; i++) {
				if (i == 0)
					digits[i] = menu->mAmount % 10;
				else {
					s32 power = 1;
					for (s32 j = 0; j < i; j++)
						power *= 10;
					digits[i] = (menu->mAmount / power) % 10;
				}
			}
			digits[menu->mScrapDigit]++;
			if (digits[menu->mScrapDigit] > 9)
				digits[menu->mScrapDigit] = 0;
			for (s32 i = 0; i < 5; i++) {
				s32 power = 1;
				if (i > 0) {
					for (s32 j = 0; j < i; j++)
						power *= 10;
				}
				value += power * digits[i];
			}
			menu->mAmount = value;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mTrigger & 0x200) {
			ResetPCText(menu, 2);
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
		} else if (pad.mPad->mTrigger & 0x100) {
			if (menu->mAmount > GetScrap()) {
				menu->mAmount = GetScrap();
				fn_8001E904(&lbl_8025C400.mSound, 0x85, 0xFF);
				return;
			}
			if (menu->mAmount == 0) {
				fn_8001E904(&lbl_8025C400.mSound, 0x85, 0xFF);
				return;
			}
			menu->mPhase = 1;
			menu->mYesNo = 0;
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
		}
		break;
	case 1:
		if (pad.mPad->mRepeat & 0x10008) {
			if (menu->mYesNo > 0) {
				menu->mYesNo--;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			}
		} else if (pad.mPad->mRepeat & 0x20004) {
			if (menu->mYesNo < 1) {
				menu->mYesNo++;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			}
		}
		if (pad.mPad->mTrigger & 0x100) {
			switch (menu->mYesNo) {
			case 0: {
				RecyclePCAmount(menu);
				menu->mYesNo = 2;
				fn_8001E904(&lbl_8025C400.mSound, 0x30009, 0xFF);
				menu->mPhase = 2;
				break;
			}
			case 1:
				menu->mPhase = 0;
				fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
				break;
			}
		} else if (pad.mPad->mTrigger & 0x200) {
			menu->mPhase = 0;
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
		}
		break;
	case 2:
		if (pad.mPad->mTrigger & 0x300) {
			menu->mPhase = 0;
			ResetPCText(menu, 2);
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
		}
		break;
	}
}
// the placement entry's scale, in percent there
static inline Vec EntryScale(XObj* obj)
{
	UnkStageObj* entry = obj->mCoord._00;
	Vec scale          = lbl_80362048[0];
	scale.x            = entry->mScale[0] / 100.0f;
	scale.y            = entry->mScale[1] / 100.0f;
	scale.z            = entry->mScale[2] / 100.0f;
	return scale;
}

/**
 * @note Address: 0x800621E0
 * @note Size: 0xD8
 */
extern "C" CVec fn_800621E0(XObj* obj)
{
	return EntryScale(obj);
}

// and its position, in centimeters
static inline Vec EntryPos(XObj* obj)
{
	UnkStageObj* entry = obj->mCoord._00;
	Vec pos            = lbl_8036203C;
	pos.x              = entry->mPos[0] / 100.0f;
	pos.y              = entry->mPos[1] / 100.0f;
	pos.z              = entry->mPos[2] / 100.0f;
	return pos;
}

/**
 * @note Address: 0x800622B8
 * @note Size: 0xD8
 */
extern "C" CVec fn_800622B8(XObj* obj)
{
	return EntryPos(obj);
}

extern "C" void fn_8003466C(MenuPCPage*);

static inline s32 CountPCShopItems(MenuPCItem* table)
{
	s16 x;
	s16 y;
	int i;
	s32 count;
	count = 0;
	for (i = 0; i < 20; i++) {
		x = table[i].mItem;
		y = table[i].mCount;
		if (x >= 0 && y >= 0)
			count++;
	}
	return count;
}
static inline s32 PCShopMaxPage(MenuPCItem*& table)
{
	s16 x, y;
	int i;
	s32 pages = 0;
	for (i = 0; i < 20; table++, i++) {
		x = table->mItem;
		y = table->mCount;
		if (x >= 0 && y >= 0)
			pages++;
	}
	pages -= 8;
	if (pages < 1)
		pages = 1;
	return pages;
}
static inline s32 ResetPCShopPage(MenuPC* menu, MenuPCItem*& table)
{
	s16 x, y;
	int i;
	s32 pages    = 0;
	i            = 0;
	menu->mPhase = 0;
	for (; i < 20; table++, i++) {
		x = table->mItem;
		y = table->mCount;
		if (x >= 0 && y >= 0)
			pages++;
	}
	pages -= 8;
	if (pages < 1)
		pages = 1;
	return pages;
}
static inline s16 PCShopEntryId(s32 index)
{
	return lbl_80366B70.mRows[index].mItem;
}

/**
 * @note Address: 0x8004AB40
 * @note Size: 0xDE8
 */
extern "C" void State7__6MenuPCFUl(MenuPC* menu, u32 frames)
{
	s32 count;
	CPad pad(0);
	MenuPCItem* table = lbl_80366DB4;
	count             = CountPCShopItems(table);
	fn_80068C8C(&menu->mPanelAlpha, &menu->mPanelTarget, 0.08f * frames);
	switch (menu->mPhase) {
	case 0:
		if (count > 1) {
			if (menu->mShopSelection == 0) {
				if (pad.mPad->mRepeat & 0x20004) {
					s32 next = menu->mShopSelection + 1;
					if (next <= 19) {
						menu->mShopSelection = next;
						fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
					}
				}
				if (pad.mPad->mRepeat & 0x10008) {
					CountPCShopItems(table);
					if (lbl_80366B70.mStart > 0) {
						menu->mScrollTarget = 17.0f;
						menu->mPhase        = 20;
						break;
					}
				}
			} else if (menu->mShopSelection == 8) {
				if (pad.mPad->mRepeat & 0x10008) {
					if (--menu->mShopSelection < 0)
						menu->mShopSelection = 19;
					fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
				}
				if (pad.mPad->mRepeat & 0x20004) {
					if (lbl_80366B70.mStart < PCShopMaxPage(table) - 1) {
						menu->mScrollTarget = -17.0f;
						menu->mPhase        = 10;
						break;
					}
				}
			} else {
				if (pad.mPad->mRepeat & 0x10008) {
					if (--menu->mShopSelection < 0)
						menu->mShopSelection = 19;
					fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
				}
				if (pad.mPad->mRepeat & 0x20004) {
					s32 next = menu->mShopSelection + 1;
					if (next <= 19) {
						menu->mShopSelection = next;
						s16 id               = lbl_80366B70.mRows[menu->mShopSelection].mItem;
						if (id >= 0)
							fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
					}
				}
			}
		}
		if (PCShopEntryId(menu->mShopSelection) < 0) {
			do {
				menu->mShopSelection--;
				s16 id = lbl_80366B70.mRows[menu->mShopSelection].mItem;
				if (id >= 0)
					break;
			} while (menu->mShopSelection != 0);
		}
		if (pad.mPad->mTrigger & 0x100) {
			if (PCShopEntryId(menu->mShopSelection) >= 0 && lbl_80366B70.mRows[menu->mShopSelection].mCount > 0) {
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				menu->mPanelTarget = 1.0f;
				menu->mPhase++;
				menu->mYesNo = 0;
				break;
			}
			fn_8001E904(&lbl_8025C400.mSound, 0x85, 0xFF);
			break;
		}
		if (pad.mPad->mTrigger & 0x200) {
			ResetPCText(menu, 2);
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
			menu->mPanelTarget = 0.0f;
		}
		break;
	case 1:
		if (pad.mPad->mRepeat & 0x10008) {
			menu->mYesNo--;
			if (menu->mYesNo < 0)
				menu->mYesNo = 1;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mRepeat & 0x20004) {
			menu->mYesNo++;
			if (menu->mYesNo > 1)
				menu->mYesNo = 0;
			fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
		} else if (pad.mPad->mTrigger & 0x100) {
			if (menu->mYesNo == 0) {
				MenuPCItem* entry;
				s32 item  = lbl_80366B70.mIndex[menu->mShopSelection];
				s32 scrap = GetScrap();
				entry     = &lbl_80366DB4[item];
				s16 price = entry->mPrice;
				if (price <= scrap) {
					scrap -= price;
					lbl_8025C400.GetPlayer()->mParams.SetScrap(scrap);
					menu->mDisplayScrap = scrap;
					menu->mYesNo        = 2;
					fn_8001E904(&lbl_8025C400.mSound, 0x30009, 0xFF);
					gEvtVars[menu->mCatalogBase + entry->mItem] = 1;
					menu->mExitState                            = item;
					s16& quantity                               = entry->mCount;
					s16 value                                   = quantity;
					s32 remaining                               = value;
					if (value > 0)
						remaining = value - 1;
					quantity = remaining;
					menu->mPhase++;
					return;
				}
				fn_8001E904(&lbl_8025C400.mSound, 0x85, 0xFF);
			} else {
				menu->mPhase       = 0;
				menu->mPanelTarget = 0.0f;
				fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
				menu->_39C = 1.0f;
			}
		} else if (pad.mPad->mTrigger & 0x200) {
			fn_8001E904(&lbl_8025C400.mSound, 0x86, 0xFF);
			menu->mPhase       = 0;
			menu->mPanelTarget = 0.0f;
		}
		break;
	case 2:
		if (pad.mPad->mTrigger & 0x300) {
			menu->mPhase       = 0;
			menu->mPanelTarget = 0.0f;
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 0xFF);
			fn_8003466C(&lbl_80366B70);
			ResetPCText(menu, 11);
			menu->_1A4 = 0.0f;
			RestorePCHistory(menu);
		}
		break;
	case 10:
		if (menu->mScroll == menu->mScrollTarget) {
			if (lbl_80366B70.mStart < ResetPCShopPage(menu, table) - 1) {
				lbl_80366B70.mStart++;
				fn_8003466C(&lbl_80366B70);
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			}
			menu->mScroll       = 0.0f;
			menu->mScrollTarget = 0.0f;
		}
		break;
	case 20:
		if (menu->mScroll == menu->mScrollTarget) {
			menu->mPhase = 0;
			CountPCShopItems(table);
			if (lbl_80366B70.mStart > 0) {
				lbl_80366B70.mStart--;
				fn_8003466C(&lbl_80366B70);
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 0xFF);
			}
			menu->mScroll       = 0.0f;
			menu->mScrollTarget = 0.0f;
		}
		break;
	}
}

static inline void PCSlide(f32& value, f32& target, f32 speed)
{
	if (value > target) {
		if ((value -= speed) < target)
			value = target;
	}
	if (value < target) {
		if ((value += speed) > target)
			value = target;
	}
}
static inline void PCEase(f32& value, f32& target)
{
	f32 speed = target - value;
	f32 rate  = 0.25f;
	speed     = speed * rate;
	if (speed < 0.0f)
		speed = -speed;
	if (speed < 1.0f)
		speed = 1.0f;
	PCSlide(value, target, speed);
}
static inline void PCEaseCall(f32& value, f32& target)
{
	f32 speed = target - value;
	f32 rate  = 0.25f;
	rate      = speed * rate;
	if (rate < 0.0f)
		rate = -rate;
	if (rate <= 1.0f)
		rate = 1.0f;
	fn_80068C8C(&value, &target, rate);
}
static inline XObj* PCWorldObject(s32 index)
{
	return lbl_8025C400.mXObjs.mPools[22].Get(index);
}
// where the object's first anim is and where it ends
static inline f32 PCAnimEnd(XObj* obj)
{
	HSD_AObj* aobj = GetAObj(obj->mJObj);
	return aobj ? HSD_AObjGetEndFrame(aobj) : 0.0f;
}
static inline f32 PCAnimFrame(XObj* obj)
{
	HSD_AObj* aobj = GetAObj(obj->mJObj);
	return aobj ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
}
typedef void (MenuPC::*PCStepHandler)(u32);
extern "C" PCStepHandler lbl_80212120[];

/**
 * @note Address: 0x8004DD1C
 * @note Size: 0x830
 */
extern "C" BOOL Update__6MenuPCFUl(MenuPC* menu, u32 frames)
{
	if (Update__5CMenuFUl(menu, frames) == 4) {
		PCEase(menu->_198, menu->_1A0);
		PCEase(menu->mFade, menu->_1A4);
		PCSlide(menu->mTextX, menu->mTargetX, 6.0f * frames);
		PCSlide(menu->mTextY, menu->mTargetY, 6.0f * frames);
		fn_80068C8C(&menu->mScroll, &menu->mScrollTarget, 17.0f);
		PCEaseCall(menu->_40C, menu->mDisplayMoney);
		PCEaseCall(menu->_414, menu->mDisplayScrap);
		PCEaseCall(menu->_424, menu->_428);
		menu->_44C -= 0.008f * frames;
		if (menu->mMode > 1) {
			switch (menu->mTextPhase) {
			case 0:
				switch (menu->mMode) {
				case 3:
				case 7:
				case 10:
					menu->mTargetX = 140.0f;
					break;
				case 4:
					menu->mTargetX = 160.0f;
					break;
				default:
					menu->mTargetX = 180.0f;
					break;
				}
				menu->mTextPhase++;
				break;
			case 1:
				if (menu->mTextX == menu->mTargetX) {
					menu->mTextY   = 129.0f;
					menu->mTargetY = 301.0f;
					switch (menu->mMode) {
					case 3:
					case 7:
					case 10:
						menu->mTextY -= 8.0f;
						menu->mTargetY -= 8.0f;
						break;
					}
					menu->mTextPhase++;
				}
				break;
			case 2:
				if (menu->mTextY == menu->mTargetY) {
					switch (menu->mMode) {
					case 3:
					case 7:
					case 10:
						menu->mTextX   = 140.0f;
						menu->mTargetX = 469.0f;
						break;
					case 4:
						menu->mTextX   = 160.0f;
						menu->mTargetX = 449.0f;
						break;
					default:
						menu->mTextX   = 210.0f;
						menu->mTargetX = 429.0f;
						break;
					}
					menu->mTextPhase++;
				}
				break;
			case 3:
				if (menu->mTextX == menu->mTargetX)
					menu->mTextPhase++;
				break;
			case 4:
				break;
			}
		}
		switch (menu->_36C) {
		case 0:
			menu->_370 = 0;
			break;
		case 1:
			menu->_370 = 1;
			break;
		case 2:
			menu->_370 = 2;
			break;
		case 3:
			menu->_370 = 6;
			break;
		case 4:
			menu->_370 = 7;
			break;
		case 5:
			menu->_370 = 8;
			break;
		case 6:
			menu->_370 = 9;
			break;
		case 7:
			menu->_370 = 2;
			break;
		case 8:
			menu->_370 = 10;
			break;
		default:
			menu->_370 = 0;
			break;
		}
		if (menu->_374 == 1) {
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = PCWorldObject(i);
				switch (obj->GetKind()) {
				case 120:
					obj->SetAnim(menu->_370, 0, 0, 0, 0, 0.0f, frames, 1.0f, 10.0f);
					break;
				}
			}
			menu->_374 = 0;
		}
		switch (menu->_36C) {
		case 1:
		case 2:
		case 6:
		case 8:
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = PCWorldObject(i);
				switch (obj->GetKind()) {
				case 120: {
					f32 end     = PCAnimEnd(obj);
					f32 current = PCAnimFrame(obj);
					if (end - 2.0f <= current) {
						menu->_36C++;
						menu->_374 = 1;
					}
				} break;
				}
			}
			break;
		}
		(menu->*lbl_80212120[menu->mMode])(frames);
	}
	return menu->IsOpen();
}

typedef void (MenuSave::*SaveStepHandler)(u32);
extern "C" SaveStepHandler lbl_802122B4[];

/**
 * @note Address: 0x8003CE18
 * @note Size: 0x538
 */
// NONMATCHING: regalloc
extern "C" BOOL Update__8MenuSaveFUl(MenuSave* menu, u32 frames)
{
	if (Update__5CMenuFUl(menu, frames) == 4) {
		PCEaseCall(menu->_15C, menu->_164);
		fn_80068C8C(&menu->_264, &menu->mTargetAlpha, 0.08f * frames);
		PCEaseCall(menu->_16C, menu->mOffset);
		menu->_2A4 -= 0.008f * frames;
		if (menu->mTitle.mActive && menu->mTitle.mActive > 0) {
			f32 fade = 0.055555556f * frames;
			for (int i = 0; i < menu->mTitle.mCount; i++) {
				MenuTitleChar* glyph = &menu->mTitle.mChars[i];
				if (glyph->mHold > 0.0f) {
					if (glyph->mFade < 1.0f) {
						if ((glyph->mFade += fade) > 1.0f) {
							glyph->mFade = 1.0f;
						}
					}
				} else if (glyph->mFade > 0.0f) {
					if ((glyph->mFade -= fade) < 0.0f) {
						glyph->mFade = 0.0f;
					}
				}
			}
			s32 shown = 0;
			f32 spinX = 0.08726646f * frames;
			f32 spinY = 0.05235988f * frames;
			for (int i = 0; i < menu->mTitle.mCount; i++) {
				MenuTitleChar* glyph = &menu->mTitle.mChars[i];
				if (glyph->mFade != 0.0f) {
					shown++;
					glyph->mAngle0 += spinX;
					glyph->mAngle1 += spinY;
				}
			}
			if (shown == 0) {
				menu->mTitle.mActive = FALSE;
			}
		}
		switch (menu->_294) {
		case 1:
			menu->_298 = 0.0f;
			menu->_294++;
			menu->_29C = menu->_2A0;
			break;
		case 2:
			menu->_298 += 0.04f * frames;
			if (menu->_298 >= 1.0f) {
				menu->_298 = 1.0f;
				menu->_294 = 0;
			}
			break;
		}
		if (menu->mAlpha > 0.0f) {
			f32 spinX = 0.08726646f * frames;
			f32 spinY = 0.05235988f * frames;
			for (int i = 0; i < 20; i++) {
				menu->mBg[i].mAngle0 += spinX;
				menu->mBg[i].mAngle1 += spinY;
			}
		}
		if (menu->mMode != 1 && menu->mMode != 5 && menu->mMode != 9) {
			s32 size;
			s32 sector;
			menu->mReady = FALSE;
			menu->_262   = FALSE;
			switch (fn_80087A1C(&lbl_8025C400.mMemcard.mCard, menu->mChannel, &size, &sector)) {
			case -2:
				menu->_262 = TRUE;
				break;
			case 0:
				if (sector == 0x2000) {
					menu->mReady = TRUE;
				}
				break;
			case -128:
				break;
			}
			if (!menu->mReady) {
				menu->mShowMessage = FALSE;
			}
		}
		if (lbl_8025C400.mMemcard.mCard.mBusy != 0) {
			return menu->IsOpen();
		}
		(menu->*lbl_802122B4[menu->mMode])(frames);
	}
	return menu->IsOpen();
}

static inline f32 MenuTextCenter(f32 w, f32 factor, f32 center)
{
	return center - w * factor;
}
static inline void DrawMenuHelpLine(s32 id, f32 y, f32 size, u32 color)
{
	char* text = fn_8006DBF0(id);
	f32 width  = fn_801968BC(&lbl_8025C400.mGs, text, 1, size);
	fn_80196610(&lbl_8025C400, color, text, MenuTextCenter(width, 0.5f, 320.0f), y, 0.0f, size, size);
}

/**
 * @note Address: 0x800351F4
 * @note Size: 0x10D0
 */
extern "C" s32 fn_800351F4(void*, s32 id, f32 x, f32 y, f32 size, f32 alpha)
{
	s32 lines = 0;
	u32 color = (u32)(255.0f * alpha);
	switch (id) {
	case 1:
		DrawMenuHelpLine(0xad, y, size, color);
		DrawMenuHelpLine(0xae, (y + size), size, color);
		lines = 2;
		break;
	case 2:
		if (IsJapanese()) {
			DrawMenuHelpLine(0xdf, y, size, color);
			lines = 1;
		} else {
			DrawMenuHelpLine(0xdf, y, size, color);
			DrawMenuHelpLine(0xe0, (y + size), size, color);
			lines = 2;
		}
		break;
	case 3:
		DrawMenuHelpLine(0xb7, y, size, color);
		lines = 1;
		break;
	case 8:
		if (IsJapanese()) {
			DrawMenuHelpLine(0x85, y, size, color);
			lines = 1;
		} else {
			DrawMenuHelpLine(0x85, y, size, color);
			DrawMenuHelpLine(0x86, (y + size), size, color);
			lines = 2;
		}
		break;
	case 0xe:
		DrawMenuHelpLine(0xee, y, size, color);
		lines = 1;
		break;
	case 4:
		DrawMenuHelpLine(0xb2, y, size, color);
		DrawMenuHelpLine(0xb3, (y + size), size, color);
		DrawMenuHelpLine(0xb4, (2.0f * size + y), size, color);
		lines = 3;
		break;
	case 5:
		DrawMenuHelpLine(0x76, y, size, color);
		DrawMenuHelpLine(0x77, (y + size), size, color);
		DrawMenuHelpLine(0x78, (2.0f * size + y), size, color);
		lines = 4;
		break;
	case 6:
		DrawMenuHelpLine(0x7b, y, size, color);
		DrawMenuHelpLine(0x7c, (y + size), size, color);
		lines = 3;
		break;
	case 7:
		DrawMenuHelpLine(0x80, y, size, color);
		DrawMenuHelpLine(0x81, (y + size), size, color);
		DrawMenuHelpLine(0x82, (2.0f * size + y), size, color);
		lines = 3;
		break;
	case 9:
		if (IsJapanese()) {
			DrawMenuHelpLine(0x8a, y, size, color);
			lines = 1;
		} else {
			DrawMenuHelpLine(0x8a, y, size, color);
			DrawMenuHelpLine(0x8b, (y + size), size, color);
			lines = 2;
		}
		break;
	case 10:
		if (IsJapanese()) {
			DrawMenuHelpLine(0x8f, y, size, color);
			DrawMenuHelpLine(0x90, (y + size), size, color);
			lines = 2;
		} else {
			DrawMenuHelpLine(0x8f, y, size, color);
			lines = 1;
		}
		lines = lines + 1;
		break;
	case 0xb:
		DrawMenuHelpLine(0xcb, y, size, color);
		DrawMenuHelpLine(0xcc, (y + size), size, color);
		lines = 3;
		break;
	case 0x10:
		DrawMenuHelpLine(0x71, y, size, color);
		DrawMenuHelpLine(0x72, (y + size), size, color);
		lines = 2;
		break;
	case 0xc:
		DrawMenuHelpLine(0xf8, y, size, color);
		DrawMenuHelpLine(0xf9, (y + size), size, color);
		DrawMenuHelpLine(0xfa, (2.0f * size + y), size, color);
		DrawMenuHelpLine(0xfb, (3.0f * size + y), size, color);
		lines = 4;
		break;
	case 0x11:
		DrawMenuHelpLine(0xfd, y, size, color);
		DrawMenuHelpLine(0xfe, (y + size), size, color);
		DrawMenuHelpLine(0xff, (2.0f * size + y), size, color);
		DrawMenuHelpLine(0x100, (3.0f * size + y), size, color);
		lines = 4;
		break;
	case 0xd:
		DrawMenuHelpLine(0xda, y, size, color);
		DrawMenuHelpLine(0xdb, (y + size), size, color);
		DrawMenuHelpLine(0xdc, (2.0f * size + y), size, color);
		DrawMenuHelpLine(0xdd, (3.0f * size + y), size, color);
		DrawMenuHelpLine(0xde, (4.0f * size + y), size, color);
		lines = 5;
	}
	return lines;
}

/**
 * @note Address: 0x800362C4
 * @note Size: 0xAA8
 */
extern "C" s32 fn_800362C4(void*, s32 id, u8 retry, f32 x, f32 y, f32 size, f32 alpha)
{
	s32 lines = 0;
	u32 color = (u32)(255.0f * alpha);
	switch (id) {
	case -128:
	case -14:
	case -12:
	case -11:
	case -10:
	case -8:
	case -7:
	case -4:
	case -1:
	case 0:
		if (IsJapanese()) {
			DrawMenuHelpLine(0xC6, y, size, color);
			lines = 1;
		} else {
			DrawMenuHelpLine(0xC6, y, size, color);
			DrawMenuHelpLine(0xC7, y + size, size, color);
			DrawMenuHelpLine(0xC8, 2.0f * size + y, size, color);
			lines = 3;
		}
		break;
	case -5:
		DrawMenuHelpLine(0xF3, y, size, color);
		DrawMenuHelpLine(0xF4, y + size, size, color);
		lines = 2;
		break;
	case -3: {
		s32 n;
		if (IsJapanese()) {
			DrawMenuHelpLine(0x67, y, size, color);
			DrawMenuHelpLine(0x68, y + (2.0f + size), size, color);
			DrawMenuHelpLine(0x69, 2.0f * (2.0f + size) + y, size, color);
			n = 3;
		} else {
			DrawMenuHelpLine(0x67, y, size, color);
			DrawMenuHelpLine(0x68, y + (2.0f + size), size, color);
			DrawMenuHelpLine(0x69, 2.0f * (2.0f + size) + y, size, color);
			DrawMenuHelpLine(0x6A, 3.0f * (2.0f + size) + y, size, color);
			n = 4;
		}
		lines = n;
		break;
	}
	case -9:
		DrawMenuHelpLine(0xA3, y, size, color);
		DrawMenuHelpLine(0xA4, y + size, size, color);
		DrawMenuHelpLine(0xA5, 2.0f * size + y, size, color);
		DrawMenuHelpLine(0xA6, 3.0f * size + y, size, color);
		lines = 4;
		break;
	case -2:
		if (retry) {

			if (IsJapanese()) {
				DrawMenuHelpLine(0x6C, y, size, color);
				lines = 1;
			} else {
				DrawMenuHelpLine(0x6C, y, size, color);
				DrawMenuHelpLine(0x6D, y + size, size, color);
				DrawMenuHelpLine(0x6E, 2.0f * size + y, size, color);
				DrawMenuHelpLine(0x6F, 3.0f * size + y, size, color);
				lines = 4;
			}
		} else {
			DrawMenuHelpLine(0xD0, y, size, color);
			DrawMenuHelpLine(0xD1, y + size, size, color);
			DrawMenuHelpLine(0xD2, 2.0f * size + y, size, color);
			DrawMenuHelpLine(0xD3, 3.0f * size + y, size, color);
			lines = 4;
		}
		break;

	case -13:
	case -6:
		break;
	}
	return lines;
}
