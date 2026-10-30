#include "game/stg.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" u8 lbl_8025C400[];
#define GAME_FLAGS  (*(u64*)(lbl_8025C400 + 0x1B10))
#define GAME_STAGE  (*(s32*)(lbl_8025C400 + 0x3444))         // the stage that's loaded
#define GAME_TPL    (*(TPLPalette**)(lbl_8025C400 + 0x3D4C)) // the system textures
#define GAME_TEX    ((CTexObj*)(lbl_8025C400 + 0x3D6C))
#define GAME_SOUND  (lbl_8025C400 + 0xE7E24)
#define GAME_SAVE   (lbl_8025C400 + 0xE8400)
#define SAVE_STATE  (*(s32*)(GAME_SAVE))
#define SAVE_RESULT (*(s32*)(GAME_SAVE + 0x38)) // the last card result
#define GS_RENDER   (*(void**)(lbl_8025C400 + 0x290))

#define GS_TEV (*(void**)(lbl_8025C400 + 0x294))

extern "C" {
void* GetSpawnData(s32 index);
void fn_8001E660(void* sound, s32 sfx);
char* fn_8006DBC8(s32 id);
char* fn_8006DBF0(s32 id);
s32 fn_8006DCFC(s32 id);
void fn_8019B34C(void* mode, s32 a);
void fn_8019AFA0(void* tev, s32 mode);
void fn_801992BC(void* gs, s32 mode, u32 c0, u32 c1, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1);
void fn_80196518(void* gs, u32 color, const char* fmt, f32 x, f32 y, f32 z, f32 sx, f32 sy, ...);
void fn_80061CD0(MenuCursor* cursor, f32 x, f32 y, f32 w, f32 h, f32 alpha, f32 alpha2);
void fn_80197800(void* gs, CTexObj* tex, u32 c0, u32 c1, u32 c2, u32 c3, f32 x, f32 y, f32 z, f32 w, f32 h, f32 a, f32 b, f32 c);
void fn_80198614(void* gs, u32 c0, u32 c1, u32 c2, u32 c3, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3,
                 f32 y3, f32 z3);
void fn_80198D08(void* gs, s32 a, void* mtx, s32 b, s32 c, s32 d);
void fn_80026B9C(MenuCursor* cursor, s32 a);
void fn_800928E8(MenuCursor* cursor);
void fn_8003ADC8(MenuSave* menu);
BOOL fn_8001E904(void* sound, s32 sfx, u8 vol);
f32 fn_801968BC(void* gs, const char* text, s32 a, f32 size);
void fn_80199AB0(void* gs, s32 a, s32 b);
void fn_8019B56C(void* mode, s32 type, s32 src, s32 dst, s32 op);
void fn_8019B464(void* mode, s32 enable, s32 func, s32 update);
void fn_80089890(void* save, s32 slot, s32 a);
void fn_8008A1F4(void* save, s32 slot, s32 file, s32 a);
void fn_80089C90(void* save, s32 slot, s32 file, s32 a);
void fn_80088B4C(void* save, s32 slot, s32 file, s32 a);
void fn_80088E78(void* save, s32 slot, s32 a);
void fn_80033A68(MenuStageSelect* menu);
void fn_80057318(u32 color, f32 x, f32 y, f32 z, f32 w, f32 h, f32 alpha);
void fn_80066630(char* text);
void fn_80196610(void* gs, u32 color, const char* fmt, f32 x, f32 y, f32 z, f32 sx, f32 sy, ...);
void fn_80054314(f32 alpha);
void fn_80056608(MenuStatus* menu);
void fn_8005D148(s32 id, f32 x, f32 y, f32 alpha, f32 scale, f32 a);
void fn_8005CD3C(s32 id, f32 x, f32 y, f32 alpha, f32 a);
void fn_8006E44C(s32 id, s32 a);
void fn_8004EE0C(MenuStatus* menu);
void fn_800559C4(MenuStatus* menu);
void fn_800554A8(MenuStatus* menu);
void fn_8004F930(MenuStatus* menu);
void fn_80053714(MenuStatus* menu);
void fn_80053D58(MenuStatus* menu);
void GXSetScissor(u32 left, u32 top, u32 width, u32 height);
BOOL fn_80086F24(void* file);
void fn_8000B394(void* game, s32 stage, s32 a);
void fn_8005BE74(MenuStatus* menu);
void fn_80060118(MenuItem* menu);
void fn_8004E54C(MenuPC* menu);
void fn_8003D350(MenuSave* menu);
void fn_8007EEA4(void* evt, s32 id, s32 a, s32 b, s32 c);
void fn_80061480(MenuMain* menu);
BOOL fn_800617DC(CMenu* menu, u32 frames);
void fn_800623F8(MenuWindow* win);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
u16 GXGetTexObjWidth(const GXTexObj* obj);
u16 GXGetTexObjHeight(const GXTexObj* obj);
}
extern "C" u32 lbl_8036781C[];

// a key and a value, 256 of them in the player's data
struct UnkIdPair {
	s16 mId;    // _00
	s16 mValue; // _02
};

struct UnkPlayerIds {
	u8 _00[0x9688];        // _00
	UnkIdPair mPairs[256]; // _9688
};
#define GAME_PLAYER_IDS (*(UnkPlayerIds**)(lbl_8025C400 + 0x96BC))

// what a CPad points at, game/cpad.h has it as CPadOne
struct MenuPadStatus {
	u8 _00[0x14];    // _00
	u32 mTrigger;    // _14
	u32 mDirTrigger; // _18
	u32 mRepeat;     // _1C, the dpad and stick bits repeat while held
};
#define PAD_REPEAT(pad) (((MenuPadStatus*)(pad).mStatus)->mRepeat)

// the fixed lines of the pause menu, MemCard's ? becomes the slot letter
static MenuLine sMainLines[] = {
	{ "StatusMenu", 1 }, { "\x01", -1 }, { "MemCard(?)", 6 }, { "SelectStage", 5 }, { NULL, 0 },
};

// the stages the stage select offers, in its order
static s32 sStageOrder[] = { 0, 5, 7, 1, 2, 4, 6, 0x12, 3, 9, 0x10, 0xB, 0x16, 0xD, 0xE, 0xA, -1 };

static MenuLine sMCTestSubLines[] = {
	{ "load", 4 },
	{ "save", 2 },
	{ "\x01", -1 },
	{ "delete", 6 },
};

inline BOOL CMenu::DrawMenu()
{
	if (!IsOpen()) {
		return FALSE;
	}
	fn_80199AB0(lbl_8025C400, 0, 0);
	fn_8019B56C(GS_RENDER, 1, 4, 5, 15);
	fn_8019B464(GS_RENDER, 0, 3, 0);
	fn_800623F8(&mWin);
	return TRUE;
}

// the file's other statics come first in .bss, the menus are reached at fixed offsets from them
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

// one entry of the status table, the CStatus holds its flags
struct UnkStatusEntry {
	s16 mId;         // _00
	s16 _02;         // _02
	CStatus mStatus; // _04
};

struct UnkStatusTable {
	s32 _00;                      // _00
	UnkStatusEntry mEntries[256]; // _04
	UnkStatusEntry _C04[4];       // _C04
	UnkStatusEntry _C34[4];       // _C34
};

static UnkStatusTable lbl_80365E08;

inline void MenuMain::StopMainFade()
{
	if (mFading) {
		GAME_FLAGS |= 2;
	}
	mFading  = FALSE;
	mFadeDir = 0;
}

// a shift-jis lead byte
static inline bool IsSjisLead(u8 c)
{
	return (c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xFC);
}

inline void MenuGlyphText::Set(const char* text, u32 color)
{
	const char* p = text;
	mActive       = TRUE;
	mCount        = 0;
	if (text) {
		mColor = color;
		int i;
		for (i = 0; i < 20 && *p; i++, p++) {
			mGlyphs[i].mChar = p;
			mGlyphs[i].mX    = -0.2f * i;
			mGlyphs[i].mY    = 60.0f;
			mGlyphs[i].mRotX = 0.017453292f * (rand() % 360);
			mGlyphs[i].mRotY = 0.017453292f * (rand() % 360);
			mCount++;
			if (lbl_8025C400[0x3318] == 6 && IsSjisLead(*p)) {
				p++;
			}
		}
		for (i = mCount; i < 20; i++) {
			mGlyphs[i].mChar = NULL;
			mGlyphs[i].mX    = 0.0f;
			mGlyphs[i].mY    = 0.0f;
		}
	}
}

// the message each status tab opens with
static s32 sStatusTabMsg[] = { 1, 3, 2, 4 };

// MenuMain's close, the stage select inlines it
static inline void CloseMain(MenuMain* menu)
{
	menu->mState = 5;
	menu->StopFade();
	menu->mWin.mVisible = FALSE;
	fn_8001E904(GAME_SOUND, 0x86, 0xFF);
	GAME_FLAGS &= ~2ULL;
}

// the memory card test's open, MenuMain inlines it
static inline void OpenMCTest(MenuMCTest* menu)
{
	menu->mState = 1;
	menu->StopFade();
	menu->mWin.mVisible = TRUE;
	for (int i = 0; i < menu->mCount; i++) {
		strcpy(menu->mText[i], "");
	}
	sprintf(menu->mText[menu->mCount + 1], "SLOT: %c", sMenuMain.mSlot + 'A');
	fn_80089890(GAME_SAVE, sMenuMain.mSlot, 1);
	menu->mMode = 1;
}

// the stage select's open, MenuMain inlines it
static inline void OpenStageSelect(MenuStageSelect* menu)
{
	menu->mState = 1;
	menu->StopFade();
	menu->mWin.mVisible = TRUE;
	if (GAME_FLAGS & 0x0008000000000000ULL) {
		menu->mSelect = -1;
		int i         = 0;
		for (s32* stage = sStageOrder; *stage >= 0; stage++, i++) {
			if (*stage == GAME_STAGE) {
				menu->mSelect = i;
				break;
			}
		}
		if (menu->mSelect < 0) {
			menu->mSelect = 0;
		}
	} else {
		menu->mSelect = GAME_STAGE;
	}
	fn_80033A68(menu);
}

/**
 * @note Address: 0x8003135C
 * @note Size: 0x48
 */
extern "C" void fn_8003135C(CMenu* menu)
{
	menu->mState = 5;
	menu->StopFade();
	menu->mWin.mVisible = FALSE;
}

/**
 * @note Address: 0x800313A4
 * @note Size: 0x24
 */
extern "C" BOOL fn_800313A4(CMenu* menu)
{
	return menu->IsActive();
}

/**
 * @note Address: 0x800313C8
 * @note Size: 0x10
 */
extern "C" void fn_800313C8(CMenu* menu, s32 dir)
{
	menu->mFading  = TRUE;
	menu->mFadeDir = dir;
}

/**
 * @note Address: 0x800313D8
 * @note Size: 0x88
 */
extern "C" void fn_800313D8(MenuEx* ex)
{
	if (ex->mActive) {
		for (int i = 0; i < 8; i++) {
			if (ex->mMenus[i] && ex->mMenus[i]->IsOpen()) {
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
	for (int i = 0; i < 8; i++) {
		if (ex->mMenus[i] && ex->mMenus[i]->IsOpen() && ex->mMenus[i]->Update(frames)) {
			ex->mActive = TRUE;
		}
	}
	ex->mCursor.Update(frames);
}

/**
 * @note Address: 0x80031510
 * @note Size: 0x74
 */
extern "C" void fn_80031510(MenuEx* ex)
{
	ex->mCursor.Reset();
	for (int i = 0; i < 8; i++) {
		if (ex->mMenus[i]) {
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
	if (ex->mCursor.Load(CoordInfo(GetSpawnData(12)))) {
		for (int i = 0; i < 8; i++) {
			if (ex->mMenus[i]) {
				ex->mMenus[i]->Load();
				ex->mMenus[i]->mWin.mCursor = &ex->mCursor;
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
extern "C" BOOL fn_80031740(MenuMCTestSub* menu)
{
	if (menu->DrawMenu()) {
		f32 x, y, width, step, height, maxW, lines, lineH;
		maxW   = menu->mMaxWidth;
		width  = 30.0f + maxW;
		lineH  = menu->mLineHeight;
		step   = 2.0f + lineH;
		lines  = menu->mLines;
		height = step * lines;
		x      = menu->mWin.mX;
		y      = menu->mWin.mY;
		for (int i = 0; i < lines; i++) {
			u32 color;
			if (i == menu->mSelect) {
				color = (u32)(255.0f * menu->mAlpha) | 0x20202000;
			} else {
				color = (u32)(255.0f * menu->mAlpha) | 0x64646400;
			}
			if (*menu->mText[i] == *"\x01") {
				f32 lineY  = y - height / 2 + step * (0.5f + i);
				f32 left   = x - width / 2;
				u32 shadow = (u32)(255.0f * menu->mAlpha) | 0x64646400;
				fn_801992BC(lbl_8025C400, 0, shadow, shadow, left, lineY, 0.0f, left + width, lineY, 0.0f);
				u32 light = (u32)(255.0f * menu->mAlpha) | 0xC8C8C800;
				fn_801992BC(lbl_8025C400, 0, light, light, 1.0f + left, 1.0f + lineY, 0.0f, 1.0f + (left + width), 1.0f + lineY, 0.0f);
			} else {
				fn_80196518(lbl_8025C400, color, menu->mText[i], x - menu->mTextWidth[i] / 2, y - height / 2 + step * i, 0.0f, lineH,
				            lineH);
			}
		}
		fn_80061CD0(menu->mWin.mCursor, x - maxW / 2, y - height / 2 + step * menu->mSelect, maxW, lineH, menu->mAlpha, menu->mAlpha);
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x8003164C
 * @note Size: 0xF4
 */
extern "C" void fn_8003164C(MenuEx* ex)
{
	ex->mActive = FALSE;
	for (int i = 0; i < 8; i++) {
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
	for (int j = 0; j < 8; j++) {
		if (ex->mMenus[j]) {
			ex->mMenus[j]->Init();
		}
	}
	ex->mCursor.Init(0);
}

// one of the card's file slots as the save code keeps them
struct UnkCardFile {
	s32 mUsed;    // _00
	s32 mBroken;  // _04
	u8 _08[0xC4]; // _08
};

// the memory card code's state, GAME_SAVE
struct UnkSaveData {
	u8 _00[0x16200];       // _00
	UnkCardFile mFiles[3]; // _16200
};

/**
 * @note Address: 0x80031AD8
 * @note Size: 0x3F4
 */
// NONMATCHING: the three formats come off the file's string pool at other offsets
extern "C" BOOL fn_80031AD8(MenuMCTestSub* menu, u32 frames)
{
	if (fn_800617DC(menu, frames) == 4) {
		CPad pad(0);
		if (PAD_REPEAT(pad) & 0x10008) {
			do {
				if (--menu->mSelect < 0) {
					menu->mSelect = 3;
				}
			} while (*menu->mText[menu->mSelect] == *"\x01");
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x20004) {
			do {
				if (++menu->mSelect > 3) {
					menu->mSelect = 0;
				}
			} while (*menu->mText[menu->mSelect] == *"\x01");
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (pad.mStatus->_14 & 0x200) {
			menu->Close();
			sMenuMCTest.mFading  = FALSE;
			sMenuMCTest.mFadeDir = 0;
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		} else if (pad.mStatus->_14 & 0x100) {
			s32 id            = sMCTestSubLines[menu->mSelect].mId;
			UnkSaveData* save = (UnkSaveData*)GAME_SAVE;
			UnkCardFile* file = &save->mFiles[sMenuMCTest.mSelect];
			if ((id == 4 || id == 6) && file->mUsed == 0) {
				fn_8001E904(GAME_SOUND, 0x85, 0xFF);
			} else if (id == 4 && file->mUsed != 0 && (file->mBroken != 0 || !fn_80086F24(file))) {
				fn_8001E904(GAME_SOUND, 0x85, 0xFF);
			} else {
				menu->Close();
				sMenuMCTest.mFading  = FALSE;
				sMenuMCTest.mFadeDir = 0;
				switch (id) {
				case 0:
					sMenuMCTest.mMode = 0;
					break;
				case 1:
					fn_80089890(save, sMenuMain.mSlot, 1);
					sMenuMCTest.mMode = 1;
					break;
				case 2:
					sprintf(sMenuMCTest.mText[sMenuMCTest.mSelect], "(%d) saving...", sMenuMCTest.mSelect);
					fn_8008A1F4(save, sMenuMain.mSlot, sMenuMCTest.mSelect, 1);
					sMenuMCTest.mMode = 3;
					break;
				case 4:
					sprintf(sMenuMCTest.mText[sMenuMCTest.mSelect], "(%d) loading...", sMenuMCTest.mSelect);
					fn_80089C90(save, sMenuMain.mSlot, sMenuMCTest.mSelect, 1);
					sMenuMCTest.mMode = 5;
					break;
				case 6:
					sprintf(sMenuMCTest.mText[sMenuMCTest.mSelect], "(%d) deleting...", sMenuMCTest.mSelect);
					fn_80088B4C(save, sMenuMain.mSlot, sMenuMCTest.mSelect, 1);
					sMenuMCTest.mMode = 7;
					break;
				}
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
			}
		}
	}
	return menu->IsOpen();
}

/**
 * @note Address: 0x80031ECC
 * @note Size: 0x50
 */
extern "C" void fn_80031ECC(Menu* menu)
{
	menu->mState = 1;
	menu->StopFade();
	menu->mWin.mVisible = TRUE;
	menu->mSelect       = 0;
}

/**
 * @note Address: 0x800341EC
 * @note Size: 0x5C
 */
extern "C" void fn_800341EC(Menu* menu)
{
	menu->InitMenu();
}

/**
 * @note Address: 0x80031F1C
 * @note Size: 0x17C
 */
extern "C" void fn_80031F1C(MenuMCTestSub* menu)
{
	menu->InitMenu();
	menu->mText[0]    = sMCTestSubLines[0].mText;
	menu->mText[1]    = sMCTestSubLines[1].mText;
	menu->mText[2]    = sMCTestSubLines[2].mText;
	menu->mText[3]    = sMCTestSubLines[3].mText;
	menu->mLines      = 4.0f;
	menu->mMaxWidth   = 0.0f;
	menu->mLineHeight = 16.0f;
	for (int i = 0; i < menu->mLines; i++) {
		menu->mTextWidth[i] = fn_801968BC(lbl_8025C400, menu->mText[i], 1, menu->mLineHeight);
		if (menu->mTextWidth[i] > menu->mMaxWidth) {
			menu->mMaxWidth = menu->mTextWidth[i];
		}
	}
	menu->mWin.SetSize(60.0f + menu->mMaxWidth, menu->mLines * (2.0f + menu->mLineHeight) + 24.0f);
	menu->mWin.SetPos(48.0f + (menu->mWin.mWidth / 2 + 16.0f), 48.0f + (menu->mWin.mHeight / 2 + 30.0f));
}

/**
 * @note Address: 0x80032098
 * @note Size: 0x4C4
 */
extern "C" BOOL fn_80032098(MenuMCTest* menu)
{
	if (menu->DrawMenu()) {
		f32 x, y, width, step, height, maxW, lines, lineH;
		maxW       = menu->mMaxWidth;
		width      = 30.0f + maxW;
		lineH      = menu->mLineHeight;
		step       = 2.0f + lineH;
		lines      = menu->mLines;
		height     = step * lines;
		x          = menu->mWin.mX;
		y          = menu->mWin.mY;
		s32 result = SAVE_RESULT;
		for (int i = 0; i < lines; i++) {
			u32 color;
			if (i == menu->mSelect) {
				color = (u32)(255.0f * menu->mAlpha) | 0x20202000;
			} else {
				color = (u32)(255.0f * menu->mAlpha) | 0x64646400;
			}
			if (*menu->mText[i] == *"\x01") {
				f32 lineY  = y - height / 2 + step * (0.5f + i);
				f32 left   = x - width / 2;
				u32 shadow = (u32)(255.0f * menu->mAlpha) | 0x64646400;
				fn_801992BC(lbl_8025C400, 0, shadow, shadow, left, lineY, 0.0f, left + width, lineY, 0.0f);
				u32 light = (u32)(255.0f * menu->mAlpha) | 0xC8C8C800;
				fn_801992BC(lbl_8025C400, 0, light, light, 1.0f + left, 1.0f + lineY, 0.0f, 1.0f + (left + width), 1.0f + lineY, 0.0f);
			} else if (result == 0 || result == -1 || result == -4 || result == -7 || result == -128 || i >= menu->mCount) {
				fn_80196518(lbl_8025C400, color, menu->mText[i], x - menu->mMaxWidth / 2, y - height / 2 + step * i, 0.0f, lineH, lineH);
			} else {
				f32 textW = fn_801968BC(lbl_8025C400, menu->mText[i], 0, menu->mLineHeight);
				fn_80196518(lbl_8025C400, (u32)(255.0f * menu->mAlpha) | 0x20202000, menu->mText[i], x - textW / 2,
				            y - height / 2 + step * i, 0.0f, lineH, lineH);
			}
		}
		if (result == 0 || result == -1 || result == -4 || result == -7 || result == -128) {
			f32 cx = x - maxW / 2;
			f32 cy = y - height / 2 + step * menu->mSelect;
			f32 alpha;
			if (sMenuMCTestSub.IsActive()) {
				if (menu->mState == 4) {
					alpha = 1.0f - sMenuMCTestSub.mWin.mAlpha;
				} else {
					alpha = 0.0f;
				}
			} else {
				alpha = menu->mAlpha;
			}
			fn_80061CD0(menu->mWin.mCursor, cx, cy, maxW, lineH, menu->mAlpha, alpha);
		}
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x80032CAC
 * @note Size: 0x178
 */
// NONMATCHING: the formats come off the file's string pool at other offsets
extern "C" void fn_80032CAC(MenuMCTest* menu, s32 id)
{
	switch (id) {
	case 0:
		menu->mMode = 0;
		break;
	case 1:
		fn_80089890(GAME_SAVE, sMenuMain.mSlot, 1);
		menu->mMode = 1;
		break;
	case 2:
		sprintf(menu->mText[menu->mSelect], "(%d) saving...", menu->mSelect);
		fn_8008A1F4(GAME_SAVE, sMenuMain.mSlot, menu->mSelect, 1);
		menu->mMode = 3;
		break;
	case 4:
		sprintf(menu->mText[menu->mSelect], "(%d) loading...", menu->mSelect);
		fn_80089C90(GAME_SAVE, sMenuMain.mSlot, menu->mSelect, 1);
		menu->mMode = 5;
		break;
	case 6:
		sprintf(menu->mText[menu->mSelect], "(%d) deleting...", menu->mSelect);
		fn_80088B4C(GAME_SAVE, sMenuMain.mSlot, menu->mSelect, 1);
		menu->mMode = 7;
		break;
	}
}

/**
 * @note Address: 0x80032E24
 * @note Size: 0xD8
 */
// the out of line copy of OpenMCTest
extern "C" void fn_80032E24(MenuMCTest* menu)
{
	menu->mState = 1;
	menu->StopFade();
	menu->mWin.mVisible = TRUE;
	for (int i = 0; i < menu->mCount; i++) {
		strcpy(menu->mText[i], "");
	}
	sprintf(menu->mText[menu->mCount + 1], "SLOT: %c", sMenuMain.mSlot + 'A');
	fn_80089890(GAME_SAVE, sMenuMain.mSlot, 1);
	menu->mMode = 1;
}

/**
 * @note Address: 0x80032EFC
 * @note Size: 0x1E8
 */
extern "C" void fn_80032EFC(MenuMCTest* menu)
{
	menu->InitMenu();
	menu->mCount = 10;
	for (int i = 0; i < menu->mCount; i++) {
		strcpy(menu->mText[i],
		       "(0) 00/00/00 00:00 [ 0:00] (000)\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40");
	}
	strcpy(menu->mText[menu->mCount], "\x01");
	strcpy(menu->mText[menu->mCount + 1], "");
	menu->mLines      = menu->mCount + 2;
	menu->mMaxWidth   = 0.0f;
	menu->mLineHeight = 16.0f;
	for (int i = 0; i < menu->mLines; i++) {
		menu->mTextWidth[i] = fn_801968BC(lbl_8025C400, menu->mText[i], 0, menu->mLineHeight);
		if (menu->mTextWidth[i] > menu->mMaxWidth) {
			menu->mMaxWidth = menu->mTextWidth[i];
		}
	}
	menu->mWin.SetSize(60.0f + menu->mMaxWidth, menu->mLines * (2.0f + menu->mLineHeight) + 24.0f);
	menu->mWin.SetPos(24.0f + (menu->mWin.mWidth / 2 + 16.0f), 24.0f + (menu->mWin.mHeight / 2 + 30.0f));
	menu->mMode = 0;
}

/**
 * @note Address: 0x800336DC
 * @note Size: 0x38C
 */
extern "C" BOOL fn_800336DC(MenuStageSelect* menu, u32 frames)
{
	if (fn_800617DC(menu, frames) == 4) {
		CPad pad(0);
		if (PAD_REPEAT(pad) & 0x10008) {
			do {
				if (--menu->mSelect < 0) {
					menu->mSelect = menu->mCount - 1;
				}
			} while (*menu->mText[menu->mSelect] == *"\x01");
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x20004) {
			do {
				if (++menu->mSelect > menu->mCount - 1) {
					menu->mSelect = 0;
				}
			} while (*menu->mText[menu->mSelect] == *"\x01");
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x40001) {
			if (menu->mSelect - 20 >= 0 && *menu->mText[menu->mSelect - 20] != *"\x01") {
				menu->mSelect -= 20;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		} else if (PAD_REPEAT(pad) & 0x80002) {
			if (menu->mSelect + 20 < menu->mCount && *menu->mText[menu->mSelect + 20] != *"\x01") {
				menu->mSelect += 20;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		} else if (pad.mStatus->_14 & 0x200) {
			menu->Close();
			sMenuMain.StopMainFade();
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		} else if (pad.mStatus->_14 & 0x100) {
			menu->Close();
			CloseMain(&sMenuMain);
			if (GAME_FLAGS & 0x0008000000000000ULL) {
				fn_8000B394(lbl_8025C400, sStageOrder[menu->mSelect], 1);
			} else {
				fn_8000B394(lbl_8025C400, menu->mSelect, 1);
			}
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
		}
	}
	return menu->IsOpen();
}

/**
 * @note Address: 0x80034108
 * @note Size: 0xE4
 */
// the out of line copy of OpenStageSelect
// NONMATCHING: the stage table pointer and the stage swap registers
extern "C" void fn_80034108(MenuStageSelect* menu)
{
	menu->mState = 1;
	menu->StopFade();
	menu->mWin.mVisible = TRUE;
	if (GAME_FLAGS & 0x0008000000000000ULL) {
		menu->mSelect = -1;
		for (int i = 0; sStageOrder[i] >= 0; i++) {
			if (sStageOrder[i] == GAME_STAGE) {
				menu->mSelect = i;
				break;
			}
		}
		if (menu->mSelect < 0) {
			menu->mSelect = 0;
		}
	} else {
		menu->mSelect = GAME_STAGE;
	}
	fn_80033A68(menu);
}

/**
 * @note Address: 0x8003D6FC
 * @note Size: 0x34
 */
extern "C" void fn_8003D6FC(MenuSave* menu)
{
	menu->ResetMenu();
	menu->_28C = -1;
}

/**
 * @note Address: 0x8003D730
 * @note Size: 0x70
 */
extern "C" void fn_8003D730(MenuSave* menu)
{
	menu->InitMenu();
	menu->_28C = -1;
}

/**
 * @note Address: 0x8004ED74
 * @note Size: 0x74
 */
extern "C" void fn_8004ED74(MenuPC* menu)
{
	menu->InitMenu();
	menu->_384 = 0;
	menu->_3A4 = 0;
}

/**
 * @note Address: 0x8005C728
 * @note Size: 0x70
 */
extern "C" void fn_8005C728(MenuStatus* menu)
{
	menu->InitMenu();
	menu->mSelect = 0;
}

/**
 * @note Address: 0x80060768
 * @note Size: 0x70
 */
extern "C" void fn_80060768(MenuItem* menu)
{
	menu->InitMenu();
	menu->mSelect = 0;
}

/**
 * @note Address: 0x800611D4
 * @note Size: 0x30
 */
extern "C" void fn_800611D4(MenuMain* menu)
{
	menu->StopMainFade();
}

/**
 * @note Address: 0x80061204
 * @note Size: 0x8C
 */
extern "C" void fn_80061204(MenuMain* menu)
{
	menu->mState = 5;
	menu->StopFade();
	menu->mWin.mVisible = FALSE;
	fn_8001E904(GAME_SOUND, 0x86, 0xFF);
	GAME_FLAGS &= ~2ULL;
}

/**
 * @note Address: 0x80061290
 * @note Size: 0x78
 */
extern "C" void fn_80061290(MenuMain* menu)
{
	menu->mState = 1;
	menu->StopFade();
	menu->mWin.mVisible = TRUE;
	fn_8001E904(GAME_SOUND, 0x86, 0xFF);
	GAME_FLAGS |= 2;
}

/**
 * @note Address: 0x80061308
 * @note Size: 0x5C
 */
extern "C" void fn_80061308(MenuMain* menu, char* text, s32 id)
{
	for (int i = 0; i < 5; i++) {
		if (menu->mItems[i].mText == NULL) {
			menu->mItems[i].mText = text;
			menu->mItems[i].mId   = id;
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
		menu->mItems[i].mText = NULL;
		menu->mItems[i].mId   = 0;
	}
	fn_80061480(menu);
}

/**
 * @note Address: 0x800613B0
 * @note Size: 0x74
 */
extern "C" void fn_800613B0(MenuMain* menu)
{
	menu->ResetMenu();
	menu->mSlot = 0;
	for (int i = 0; i < 5; i++) {
		menu->mItems[i].mText = NULL;
		menu->mItems[i].mId   = 0;
	}
	fn_80061480(menu);
}

/**
 * @note Address: 0x800607D8
 * @note Size: 0x504
 */
extern "C" BOOL fn_800607D8(MenuMain* menu)
{
	if (menu->DrawMenu()) {
		f32 x, y, width, step, height, maxW, lines, lineH;
		maxW   = menu->mMaxWidth;
		width  = 30.0f + maxW;
		lineH  = menu->mLineHeight;
		step   = 2.0f + lineH;
		lines  = menu->mLines;
		height = step * lines;
		x      = menu->mWin.mX;
		y      = menu->mWin.mY;
		for (int i = 0; i < lines; i++) {
			u32 color;
			if (i == menu->mSelect) {
				color = (u32)(255.0f * menu->mAlpha) | 0x20202000;
			} else {
				color = (u32)(255.0f * menu->mAlpha) | 0x64646400;
			}
			if (*menu->mText[i] == *"\x01") {
				f32 lineY  = y - height / 2 + step * (0.5f + i);
				f32 left   = x - width / 2;
				u32 shadow = (u32)(255.0f * menu->mAlpha) | 0x64646400;
				fn_801992BC(lbl_8025C400, 0, shadow, shadow, left, lineY, 0.0f, left + width, lineY, 0.0f);
				u32 light = (u32)(255.0f * menu->mAlpha) | 0xC8C8C800;
				fn_801992BC(lbl_8025C400, 0, light, light, 1.0f + left, 1.0f + lineY, 0.0f, 1.0f + (left + width), 1.0f + lineY, 0.0f);
			} else {
				fn_80196518(lbl_8025C400, color, menu->mText[i], x - menu->mMaxWidth / 2, y - height / 2 + step * i, 0.0f, lineH, lineH);
			}
		}
		f32 cx = x - maxW / 2;
		f32 cy = y - height / 2 + step * menu->mSelect;
		f32 alpha;
		if (sMenuStatus.IsActive()) {
			alpha = menu->mState == 4 ? 1.0f - sMenuStatus.mWin.mAlpha : 0.0f;
		} else if (sMenuMCTest.IsActive()) {
			alpha = menu->mState == 4 ? 1.0f - sMenuMCTest.mWin.mAlpha : 0.0f;
		} else if (sMenuStageSelect.IsActive()) {
			alpha = menu->mState == 4 ? 1.0f - sMenuStageSelect.mWin.mAlpha : 0.0f;
		} else if (sMenuPC.IsActive()) {
			alpha = menu->mState == 4 ? 1.0f - sMenuPC.mWin.mAlpha : 0.0f;
		} else if (sMenuSave.IsActive()) {
			alpha = menu->mState == 4 ? 1.0f - sMenuSave.mWin.mAlpha : 0.0f;
		} else {
			alpha = menu->mAlpha;
		}
		fn_80061CD0(menu->mWin.mCursor, cx, cy, maxW, lineH, menu->mAlpha, alpha);
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x80060CDC
 * @note Size: 0x4F8
 */
// NONMATCHING: the inlined stage select loop swaps registers
extern "C" BOOL fn_80060CDC(MenuMain* menu, u32 frames)
{
	if (fn_800617DC(menu, frames) == 4) {
		CPad pad(0);
		s32 id = -1;
		if (PAD_REPEAT(pad) & 0x10008) {
			do {
				if (--menu->mSelect < 0) {
					menu->mSelect = menu->mLines - 1.0f;
				}
			} while (*menu->mText[menu->mSelect] == *"\x01");
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x20004) {
			do {
				if (++menu->mSelect > menu->mLines - 1.0f) {
					menu->mSelect = 0;
				}
			} while (*menu->mText[menu->mSelect] == *"\x01");
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0xC0003) {
			if (sMainLines[menu->mSelect].mId == 6) {
				menu->mSlot = (menu->mSlot + 1) & 1;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
				menu->mText[menu->mSelect][8] = menu->mSlot + 'A';
			}
		} else if (pad.mStatus->_14 & 0x1200) {
			menu->Close();
		} else if (pad.mStatus->_14 & 0x100) {
			if (menu->mSelect < menu->mCount) {
				id = sMainLines[menu->mSelect].mId;
			} else {
				menu->Close();
				fn_8007EEA4(lbl_8025C400 + 0x5D07C, menu->mItems[menu->mSelect - (menu->mCount + 1)].mId, 0, -1, -1);
			}
		}
		if (id >= 0) {
			BOOL fade = TRUE;
			switch (id) {
			case 1:
				fn_8005BE74(&sMenuStatus);
				break;
			case 2:
				fn_80060118(&sMenuItem);
				break;
			case 6:
				fade = FALSE;
				OpenMCTest(&sMenuMCTest);
				break;
			case 5:
				fade = FALSE;
				OpenStageSelect(&sMenuStageSelect);
				break;
			case 3:
				fn_8004E54C(&sMenuPC);
				break;
			case 4:
				fn_8003D350(&sMenuSave);
				break;
			}
			if (id >= 0) {
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				menu->StartFade(fade);
			} else {
				fn_8001E904(GAME_SOUND, 0x85, 0xFF);
			}
		}
	}
	return menu->IsOpen();
}

/**
 * @note Address: 0x80061424
 * @note Size: 0x5C
 */
extern "C" void fn_80061424(MenuMain* menu)
{
	menu->InitMenu();
}

/**
 * @note Address: 0x80061480
 * @note Size: 0x2B8
 */
extern "C" void fn_80061480(MenuMain* menu)
{
	menu->mLineHeight = 16.0f;
	menu->mCount      = 4;
	menu->mLines      = menu->mCount;
	menu->mMaxWidth   = 0.0f;
	for (int i = 0; i < menu->mLines; i++) {
		menu->mText[i] = sMainLines[i].mText;
		if (sMainLines[i].mId == 6) {
			menu->mText[i][8] = menu->mSlot + 'A';
		}
		menu->mTextWidth[i] = fn_801968BC(lbl_8025C400, menu->mText[i], 0, menu->mLineHeight);
		if (menu->mTextWidth[i] > menu->mMaxWidth) {
			menu->mMaxWidth = menu->mTextWidth[i];
		}
	}
	int items = 0;
	for (int k = 0; k < 5; k++) {
		if (menu->mItems[k].mText) {
			items++;
		}
	}
	int line = menu->mLines;
	if (items > 0) {
		menu->mText[line]      = "\x01";
		menu->mTextWidth[line] = 1.0f;
		line++;
		for (int j = 0; j < items; j++) {
			if (menu->mItems[j].mText) {
				menu->mText[line]      = menu->mItems[j].mText;
				menu->mTextWidth[line] = fn_801968BC(lbl_8025C400, menu->mText[line], 0, menu->mLineHeight);
				if (menu->mTextWidth[line] > menu->mMaxWidth) {
					menu->mMaxWidth = menu->mTextWidth[line];
				}
				line++;
			}
		}
		menu->mLines = line;
	}
	if (menu->mSelect >= menu->mLines) {
		menu->mSelect = menu->mLines - 1.0f;
	}
	menu->mWin.SetSize(60.0f + menu->mMaxWidth, menu->mLines * (2.0f + menu->mLineHeight) + 24.0f);
	menu->mWin.SetPos(menu->mWin.mWidth / 2 + 16.0f, menu->mWin.mHeight / 2 + 30.0f);
}

/**
 * @note Address: 0x80061738
 * @note Size: 0xA4
 */
extern "C" BOOL fn_80061738(CMenu* menu)
{
	return menu->DrawMenu();
}

/**
 * @note Address: 0x800617DC
 * @note Size: 0x2E0
 */
extern "C" BOOL fn_800617DC(CMenu* menu, u32 frames)
{
	if (menu->mFading) {
		if (menu->mFadeDir != 0 && menu->mFadeAlpha > 0.0f) {
			if ((menu->mFadeAlpha -= 0.083333336f * frames) < 0.0f) {
				menu->mFadeAlpha = 0.0f;
			}
		}
	} else if (menu->mFadeAlpha < 1.0f) {
		if ((menu->mFadeAlpha += 0.083333336f * frames) > 1.0f) {
			menu->mFadeAlpha = 1.0f;
		}
	}
	if (!menu->IsOpen()) {
		return FALSE;
	}
	if (menu->mFading) {
		menu->mAlpha      = menu->mFade * menu->mFadeAlpha;
		menu->mWin.mAlpha = menu->mAlpha;
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
		menu->mWin.mVisible = FALSE;
		break;
	case 6:
		break;
	}
	menu->mAlpha      = menu->mFade * menu->mFadeAlpha;
	menu->mWin.mAlpha = menu->mAlpha;
	if (menu->mWin.mVisible) {
		menu->mWin.mTransX += 0.002f * frames;
		menu->mWin.mTransY += 0.002f * frames;
		PSMTXTrans(menu->mWin.mTex.mMtx[0], menu->mWin.mTransX, menu->mWin.mTransY, 0.0f);
		menu->mWin.mTex.mMtxDirty = TRUE;
		PSMTXScale(menu->mWin.mTex.mMtx[1], menu->mWin.mScaleX, menu->mWin.mScaleY, 1.0f);
		menu->mWin.mTex.mMtxDirty = TRUE;
		CPad pad2(0);
	}
	return menu->mState;
}

/**
 * @note Address: 0x80061AE8
 * @note Size: 0x18C
 */
// NONMATCHING: the u16 to float conversions use other float registers
extern "C" BOOL fn_80061AE8(CMenu* menu)
{
	MenuWindow* win = &menu->mWin;
	win->mTex       = GAME_TEX->Init(&GAME_TPL->descriptorArray[138], 0);
	CTexObj* tex    = &win->mTex;
	f32 w           = GXGetTexObjWidth(tex);
	win->mScaleX    = win->mWidth / w;
	f32 h           = GXGetTexObjHeight(tex);
	win->mScaleY    = win->mHeight / h;
	PSMTXTrans(tex->mMtx[0], win->mTransX, win->mTransY, 0.0f);
	win->mTex.mMtxDirty = TRUE;
	PSMTXScale(tex->mMtx[1], win->mScaleX, win->mScaleY, 1.0f);
	win->mTex.mMtxDirty = TRUE;
	return TRUE;
}

/**
 * @note Address: 0x80061ABC
 * @note Size: 0x2C
 */
extern "C" void fn_80061ABC(CMenu* menu)
{
	menu->ResetMenu();
}

/**
 * @note Address: 0x80061C74
 * @note Size: 0x5C
 */
extern "C" void fn_80061C74(CMenu* menu)
{
	menu->InitMenu();
}

/**
 * @note Address: 0x8003AC48
 * @note Size: 0xE0
 */
extern "C" void fn_8003AC48(MenuSave* menu)
{
	if (menu->_264 <= 0.0f) {
		fn_8001E660(GAME_SOUND, 0x14B);
		fn_8001E904(GAME_SOUND, 0x185, 0xFF);
		GAME_FLAGS &= ~2ULL;
		menu->Close();
		sMenuMain.StopMainFade();
	}
}

/**
 * @note Address: 0x8003AD28
 * @note Size: 0x80
 */
// NONMATCHING: the two constants swap registers
extern "C" void fn_8003AD28(MenuSave* menu)
{
	for (int i = 0; i < 20; i++) {
		menu->mLines2[i]._0C = 0.0f;
	}
	menu->_268 = 0.0f;
	menu->_164 = -200.0f;
	menu->_16C = 0.0f;
	menu->SetStep(0x11);
}

/**
 * @note Address: 0x8003ADA8
 * @note Size: 0x20
 */
extern "C" void fn_8003ADA8(MenuSave* menu)
{
	fn_8003ADC8(menu);
}

/**
 * @note Address: 0x8003BE90
 * @note Size: 0x4
 */
extern "C" void fn_8003BE90(MenuSave* menu)
{
}

/**
 * @note Address: 0x8003D7A0
 * @note Size: 0x24
 */
extern "C" char* fn_8003D7A0(s32 id)
{
	return fn_8006DBF0(id + 0x5E);
}

/**
 * @note Address: 0x8003D7C4
 * @note Size: 0x18
 */
extern "C" BOOL fn_8003D7C4(UnkSaveHeader* header)
{
	return header->_40 != 1;
}

/**
 * @note Address: 0x8003D7DC
 * @note Size: 0x44
 */
extern "C" void fn_8003D7DC(UnkSaveHeader* header)
{
	header->Clear();
}

// the out of line copies of inlines MenuPC's draw was too big to take

struct UnkPlayerData {
	u8 _00[0x4]; // _00
	s32 _04;     // _04
};

/**
 * @note Address: 0x80048EB0
 * @note Size: 0x8
 */
extern "C" s32 fn_80048EB0(UnkPlayerData* data)
{
	return data->_04;
}

/**
 * @note Address: 0x80048EB8
 * @note Size: 0x20
 */
extern "C" UnkPlayerData* fn_80048EB8(u8* game, s32 i)
{
	return (UnkPlayerData*)(*(u8**)(game + 0x96BC) + i * *(s32*)(game + 0x96CC) + 0x964C);
}

/**
 * @note Address: 0x80048ED8
 * @note Size: 0x2C
 */
extern "C" void fn_80048ED8(void* gs, u32 color, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1)
{
	fn_801992BC(gs, 0, color, color, x0, y0, z0, x1, y1, z1);
}

/**
 * @note Address: 0x80048F04
 * @note Size: 0x1E0
 */
extern "C" void fn_80048F04(f32 x, f32 y, f32 alpha)
{
	CTexObj icon = GAME_TEX->Init(&GAME_TPL->descriptorArray[119], 0);
	PSMTXScale(icon.mMtx[1], 0.05882353f, 1.0f, 1.0f);
	icon.mMtxDirty = TRUE;
	PSMTXTrans(icon.mMtx[0], 0.11764706f, 0.0f, 0.0f);
	icon.mMtxDirty = TRUE;
	fn_8019AFA0(GS_TEV, 0);
	u32 color = (u32)(255.0f * alpha) | 0xFFFFFF00;
	fn_80197800(lbl_8025C400, &icon, color, color, color, color, x, y, 0.0f, 18.0f, 18.0f, 0.0f, 0.0f, 0.0f);
	fn_8019AFA0(GS_TEV, 4);
}

/**
 * @note Address: 0x800490E4
 * @note Size: 0x8
 */
extern "C" u8* fn_800490E4(u8* game)
{
	return game + 0x42C8;
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
// NONMATCHING: the quad pointer is moved out of r4 too early
extern "C" void fn_80049174(void* gs, Vec* quad, u32 color)
{
	fn_80198614(gs, color, color, color, color, quad[0].x, quad[0].y, quad[0].z, quad[1].x, quad[1].y, quad[1].z, quad[2].x, quad[2].y,
	            quad[2].z, quad[3].x, quad[3].y, quad[3].z);
}

/**
 * @note Address: 0x800491E0
 * @note Size: 0x24
 */
extern "C" void fn_800491E0(u8* gs, s32 a)
{
	fn_8019B34C(*(void**)(gs + 0x290), a);
}

/**
 * @note Address: 0x80049204
 * @note Size: 0x24
 */
extern "C" void fn_80049204(u8* gs, s32 type, s32 src, s32 dst, s32 op)
{
	fn_8019B56C(*(void**)(gs + 0x290), type, src, dst, op);
}

/**
 * @note Address: 0x80049228
 * @note Size: 0x8
 */
extern "C" u8 fn_80049228(u8* game)
{
	return game[0x3318];
}

/**
 * @note Address: 0x80049230
 * @note Size: 0x2C
 */
extern "C" void fn_80049230(void* gs, CTexObj* tex, u32 color, f32 x, f32 y, f32 z, f32 w, f32 h, f32 a, f32 b, f32 c)
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
extern "C" void fn_8004929C(u8* gs, s32 mode)
{
	fn_8019AFA0(*(void**)(gs + 0x294), mode);
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
extern "C" CTexObj* fn_80049360(u8* game, s32 i)
{
	return &((CTexObj*)(game + 0x3D6C))->Init(&(*(TPLPalette**)(game + 0x3D4C))->descriptorArray[i], 0);
}

/**
 * @note Address: 0x80049398
 * @note Size: 0x38
 */
extern "C" void fn_80049398(u8* gs, s32 a, s32 b, s32 c)
{
	fn_80198D08(gs, 0xA0, *(u8**)(gs + 0x44) + 0xE8, a, b, c);
}

/**
 * @note Address: 0x800493D0
 * @note Size: 0x24
 */
extern "C" void fn_800493D0(u8* gs, s32 enable, s32 func, s32 update)
{
	fn_8019B464(*(void**)(gs + 0x290), enable, func, update);
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
extern "C" void fn_8004AB3C(MenuPC* menu)
{
}

/**
 * @note Address: 0x8004B928
 * @note Size: 0x4
 */
extern "C" void fn_8004B928(MenuPC* menu)
{
}

/**
 * @note Address: 0x80051C60
 * @note Size: 0x5C
 */
// NONMATCHING: the clrlwi is scheduled before the li
extern "C" BOOL fn_80051C60()
{
	BOOL found = FALSE;
	u32 id     = fn_8006DCFC(3);
	if (lbl_8036781C[id / 32] & (1 << (id % 32))) {
		found = TRUE;
	}
	return found;
}

/**
 * @note Address: 0x8005875C
 * @note Size: 0x4
 */
extern "C" void fn_8005875C(MenuStatus* menu)
{
}

/**
 * @note Address: 0x80058760
 * @note Size: 0x4
 */
extern "C" void fn_80058760(MenuStatus* menu)
{
}

/**
 * @note Address: 0x80062390
 * @note Size: 0x48
 */
extern "C" void fn_80062390(MenuCursor* cursor, s32 a)
{
	if (cursor->mFlags & 1) {
		fn_80026B9C(cursor, a);
	}
}

/**
 * @note Address: 0x800623D8
 * @note Size: 0x20
 */
extern "C" void fn_800623D8(MenuCursor* cursor)
{
	fn_800928E8(cursor);
}

/**
 * @note Address: 0x8003CD68
 * @note Size: 0xB0
 */
extern "C" void fn_8003CD68(MenuSave* menu)
{
	for (int i = 0; i < 3; i++) {
		menu->mFiles[i].Clear();
	}
	fn_80089890(GAME_SAVE, menu->mSlot, 1);
	menu->SetStep(1);
	menu->_538 = 0;
}

/**
 * @note Address: 0x8004EDE8
 * @note Size: 0x24
 */
extern "C" char* fn_8004EDE8(s32 id)
{
	return fn_8006DBF0(id + 0x2D);
}

/**
 * @note Address: 0x80034E4C
 * @note Size: 0x60
 */
// NONMATCHING: the original indexes off the data pointer plus 0x10000
extern "C" s16 fn_80034E4C(s16 id)
{
	UnkPlayerIds* data = *(UnkPlayerIds**)(lbl_8025C400 + 0x96BC);
	for (int i = 255; i >= 0; i--) {
		if (id == data->mPairs[i].mId) {
			return data->mPairs[i].mValue;
		}
	}
	return 0;
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

// a placed thing on the stage select's map, negative when the slot is free
struct UnkStagePos {
	s16 mX;      // _00
	s16 mY;      // _02
	u8 _04[0x4]; // _04
};

static UnkStagePos lbl_80366C9C[20];
static UnkStagePos lbl_80366DB4[20];

/**
 * @note Address: 0x800345A8
 * @note Size: 0xC4
 */
// NONMATCHING: the table pointer and the counter swap registers
extern "C" s32 fn_800345A8()
{
	s32 count        = 0;
	UnkStagePos* pos = lbl_80366DB4;
	for (int i = 0; i < 20; i++, pos++) {
		s16 x = pos->mX;
		s16 y = pos->mY;
		if (x >= 0 && y >= 0) {
			count++;
		}
	}
	return count;
}

/**
 * @note Address: 0x800349A8
 * @note Size: 0xC4
 */
// NONMATCHING: the table pointer and the counter swap registers
extern "C" s32 fn_800349A8()
{
	s32 count = 0;
	for (int i = 0; i < 20; i++) {
		s16 x = lbl_80366C9C[i].mX;
		s16 y = lbl_80366C9C[i].mY;
		if (x >= 0 && y >= 0) {
			count++;
		}
	}
	return count;
}

/**
 * @note Address: 0x80058BBC
 * @note Size: 0xC0
 */
extern "C" void fn_80058BBC(MenuStatus* menu, s32 wait)
{
	CPad pad(0);
	switch (menu->mStepTime) {
	case 0:
		menu->_554  = -200.0f;
		menu->mWait = wait * 4;
		menu->mStepTime++;
	case 1:
		if (--menu->mWait < 0) {
			if (menu->mNextStep != 5) {
				menu->_554 = 0.0f;
			}
			menu->SetStep(menu->mNextStep);
		}
		break;
	}
}

/**
 * @note Address: 0x8005B39C
 * @note Size: 0x9C
 */
extern "C" void fn_8005B39C(MenuStatus* menu)
{
	menu->_664 = *(s32*)(lbl_8025C400 + 0xE80E8) <= 0 ? *(s32*)(lbl_8025C400 + 0xE80D8) : 0;
	menu->_668 = lbl_8025C400[0xE80F0];
	fn_8001E904(GAME_SOUND, 0xFFFF0004, 0xFF);
	menu->mLastStep = menu->mStep;
	menu->mStep     = 4;
	menu->mNextStep = 1;
	menu->mStepTime = 0;
}

/**
 * @note Address: 0x8003B9B8
 * @note Size: 0x160
 */
extern "C" void fn_8003B9B8(MenuSave* menu, s32 frames)
{
	menu->mTimer -= frames;
	if (menu->mTimer < 0) {
		menu->mTimer = 0;
	}
	switch (SAVE_STATE) {
	case 0:
		menu->_250 = *(s32*)(lbl_8025C400 + 0xFEE04);
		menu->_254 = *(s32*)(lbl_8025C400 + 0xFEE00);
		menu->SetStep(10);
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
		s32 result    = SAVE_RESULT;
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
				menu->SetStep(13);
				menu->_261 = TRUE;
			} else {
				menu->SetStep(15);
				menu->_25C = 8;
				menu->_261 = TRUE;
			}
			break;
		default:
			menu->SetStep(11);
			menu->_174 = -32.0f;
			break;
		}
		break;
	default:
		menu->SetStep(11);
		menu->_174 = -32.0f;
		break;
	}
}

/**
 * @note Address: 0x8003C468
 * @note Size: 0x130
 */
extern "C" void fn_8003C468(MenuSave* menu, u32 frames)
{
	CPad pad(0);
	switch (menu->mStepTime) {
	case 0:
		menu->mTimer -= frames;
		if (menu->mTimer < 0) {
			menu->mTimer = 0;
			menu->mStepTime++;
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
		}
		break;
	case 1:
		if ((pad.mStatus->_14 & 0x100) || (menu->_27C -= frames) < 0.0f) {
			menu->_27C = 0.0f;
			menu->SetStep(16);
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
		}
		break;
	}
}

/**
 * @note Address: 0x8003B05C
 * @note Size: 0x1F8
 */
extern "C" void fn_8003B05C(MenuSave* menu)
{
	CPad pad(0);
	if (menu->mResult == -3) {
		if (menu->_260 == TRUE) {
			for (int i = 0; i < 3; i++) {
				menu->mFiles[i].Clear();
			}
			fn_80089890(GAME_SAVE, menu->mSlot, 1);
			menu->SetStep(1);
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
			menu->_538 = 0;
			return;
		}
	} else if (!menu->_260) {
		menu->NoCard();
		menu->_538 = 0;
		return;
	}
	if (pad.mStatus->_14 & 0x100) {
		menu->SetStep(16);
		fn_8001E904(GAME_SOUND, 0x88, 0xFF);
		menu->_538 = 0;
	}
}

/**
 * @note Address: 0x8003B254
 * @note Size: 0x1B8
 */
extern "C" void fn_8003B254(MenuSave* menu)
{
	CPad pad(0);
	if (menu->mResult == -2) {
		if (!menu->_262) {
			menu->NoCard();
			return;
		}
	} else if (!menu->_260) {
		menu->NoCard();
		return;
	}
	if (pad.mStatus->_14 & 0x100) {
		menu->SetStep(16);
		fn_8001E904(GAME_SOUND, 0x88, 0xFF);
	}
}

/**
 * @note Address: 0x8003B7AC
 * @note Size: 0x20C
 */
extern "C" void fn_8003B7AC(MenuSave* menu, u32 frames)
{
	CPad pad(0);
	if (!menu->_260) {
		menu->NoCard();
		return;
	}
	switch (menu->mStepTime) {
	case 0:
		menu->mTimer -= frames;
		if (menu->mTimer < 0) {
			menu->mTimer = 0;
			menu->mStepTime++;
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
		}
		break;
	case 1:
		if (pad.mStatus->_14 & 0x100) {
			for (int i = 0; i < 3; i++) {
				menu->mFiles[i].Clear();
			}
			fn_80089890(GAME_SAVE, menu->mSlot, 1);
			menu->SetStep(1);
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
			return;
		}
		break;
	}
}

/**
 * @note Address: 0x8003C598
 * @note Size: 0x200
 */
extern "C" void fn_8003C598(MenuSave* menu)
{
	if (!menu->_260) {
		menu->NoCard();
		return;
	}
	CPad pad(0);
	if (PAD_REPEAT(pad) & 0x40001) {
		if (menu->mYesNo > 0) {
			menu->mYesNo--;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		}
	} else if (PAD_REPEAT(pad) & 0x80002) {
		if (menu->mYesNo < 1) {
			menu->mYesNo++;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		}
	}
	if (pad.mStatus->_14 & 0x100) {
		switch (menu->mYesNo) {
		case 0:
			fn_8008A1F4(GAME_SAVE, menu->mSlot, menu->_284, 1);
			menu->SetStep(5);
			menu->_174   = -32.0f;
			menu->mTimer = 90;
			break;
		case 1:
			menu->SetStep(2);
			break;
		}
		fn_8001E904(GAME_SOUND, 0x88, 0xFF);
	}
}

/**
 * @note Address: 0x8003ADC8
 * @note Size: 0x294
 */
extern "C" void fn_8003ADC8(MenuSave* menu)
{
	CPad pad(0);
	if (menu->mResult == -3) {
		if (menu->_260 == TRUE) {
			for (int i = 0; i < 3; i++) {
				menu->mFiles[i].Clear();
			}
			fn_80089890(GAME_SAVE, menu->mSlot, 1);
			menu->SetStep(1);
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
			return;
		}
	} else if (!menu->_260) {
		menu->NoCard();
		return;
	}
	if (pad.mStatus->_14 & 0x100) {
		if (menu->mResult == -3) {
			menu->NoCard();
		} else if (menu->mResult == -5) {
			menu->SetStep(13);
			menu->_261 = TRUE;
		} else {
			menu->SetStep(13);
		}
		fn_8001E904(GAME_SOUND, 0x88, 0xFF);
	}
}

/**
 * @note Address: 0x8003BE94
 * @note Size: 0x214
 */
extern "C" void fn_8003BE94(MenuSave* menu)
{
	CPad pad(0);
	if (menu->_262 == TRUE) {
		for (int i = 0; i < 3; i++) {
			menu->mFiles[i].Clear();
		}
		fn_80089890(GAME_SAVE, menu->mSlot, 1);
		menu->SetStep(1);
		fn_8001E904(GAME_SOUND, 0x88, 0xFF);
		return;
	}
	if (menu->_260 == TRUE) {
		for (int i = 0; i < 3; i++) {
			menu->mFiles[i].Clear();
		}
		fn_80089890(GAME_SAVE, menu->mSlot, 1);
		menu->SetStep(1);
		fn_8001E904(GAME_SOUND, 0x88, 0xFF);
		return;
	}
	if (pad.mStatus->_14 & 0x100) {
		menu->SetStep(16);
		fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		return;
	}
}

/**
 * @note Address: 0x8003C798
 * @note Size: 0x294
 */
extern "C" void fn_8003C798(MenuSave* menu)
{
	if (!menu->_260) {
		menu->NoCard();
		return;
	}
	CPad pad(0);
	if (PAD_REPEAT(pad) & 0x40001) {
		menu->_284--;
		if (menu->_284 < 0) {
			menu->_284 = 2;
		}
		fn_8001E904(GAME_SOUND, 0x87, 0xFF);
	} else if (PAD_REPEAT(pad) & 0x80002) {
		menu->_284++;
		if (menu->_284 > 2) {
			menu->_284 = 0;
		}
		fn_8001E904(GAME_SOUND, 0x87, 0xFF);
	}
	if (pad.mStatus->_14 & 0x100) {
		if (menu->mFiles[menu->_284]._00 == 1) {
			menu->mYesNo = 1;
			menu->SetStep(3);
		} else if (menu->_250 >= 6 && menu->_254 > 0) {
			fn_8008A1F4(GAME_SAVE, menu->mSlot, menu->_284, 1);
			menu->SetStep(5);
			menu->mTimer = 90;
		} else {
			menu->mResult = -9;
			menu->SetStep(11);
			menu->_174 = -32.0f;
		}
		fn_8001E904(GAME_SOUND, 0x88, 0xFF);
	} else if (pad.mStatus->_14 & 0x200) {
		menu->SetStep(16);
		fn_8001E904(GAME_SOUND, 0x86, 0xFF);
	}
}

/**
 * @note Address: 0x8003BB18
 * @note Size: 0x378
 */
extern "C" void fn_8003BB18(MenuSave* menu)
{
	CPad pad(0);
	if (!menu->_260) {
		menu->NoCard();
		return;
	}
	switch (menu->mStepTime) {
	case 0:
		if (PAD_REPEAT(pad) & 0x40001) {
			if (menu->mYesNo > 0) {
				menu->mYesNo--;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		} else if (PAD_REPEAT(pad) & 0x80002) {
			if (menu->mYesNo < 1) {
				menu->mYesNo++;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		}
		if (pad.mStatus->_14 & 0x100) {
			if (menu->mYesNo == 0) {
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				menu->mYesNo = 1;
				menu->mStepTime++;
			} else {
				menu->SetStep(13);
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				menu->_538 = 1;
			}
			return;
		}
		return;
	case 1:
		if (PAD_REPEAT(pad) & 0x40001) {
			if (menu->mYesNo > 0) {
				menu->mYesNo--;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		} else if (PAD_REPEAT(pad) & 0x80002) {
			if (menu->mYesNo < 1) {
				menu->mYesNo++;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		}
		if (pad.mStatus->_14 & 0x100) {
			if (menu->mYesNo == 0) {
				fn_80088E78(GAME_SAVE, menu->mSlot, 1);
				menu->SetStep(9);
				menu->mTimer = 90;
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
			} else {
				menu->SetStep(13);
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				menu->_538 = 1;
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
extern "C" void fn_8003B40C(MenuSave* menu)
{
	CPad pad(0);
	if (menu->mResult == -3) {
		if (menu->_262 == TRUE) {
			for (int i = 0; i < 3; i++) {
				menu->mFiles[i].Clear();
			}
			fn_80089890(GAME_SAVE, menu->mSlot, 1);
			menu->SetStep(1);
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
			menu->_174 = 0.0f;
			return;
		}
		if (menu->_260 == TRUE) {
			for (int i = 0; i < 3; i++) {
				menu->mFiles[i].Clear();
			}
			fn_80089890(GAME_SAVE, menu->mSlot, 1);
			menu->SetStep(1);
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
			menu->_174 = 0.0f;
			return;
		}
	} else if (menu->mResult == -2) {
		if (!menu->_262) {
			menu->NoCard();
			return;
		}
	} else if (!menu->_260) {
		menu->NoCard();
		return;
	}
	if (pad.mStatus->_14 & 0x100) {
		if (menu->mResult == -9) {
			menu->SetStep(12);
			menu->_25C = 2;
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
			menu->_174 = 0.0f;
		} else {
			menu->SetStep(16);
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
		}
	}
}

/**
 * @note Address: 0x80049418
 * @note Size: 0xF0
 */
extern "C" void fn_80049418(MenuPC* menu)
{
	if (menu->_19C <= 2.0f) {
		*(s32*)(lbl_8025C400 + 0x5D08C) = menu->_430;
		menu->Close();
		sMenuMain.StopMainFade();
		fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		GAME_FLAGS &= ~2ULL;
		fn_8001E660(GAME_SOUND, 0x30008);
	}
}

/**
 * @note Address: 0x8005773C
 * @note Size: 0x1F8
 */
// NONMATCHING: the id loop counters get other registers
extern "C" BOOL fn_8005773C(MenuStatus* menu)
{
	if (menu->DrawMenu()) {
		fn_80054314(menu->mAlpha);
		fn_80056608(menu);
		if (menu->mStep == 2 || menu->mStep == 4) {
			GXSetScissor(0, 0x68, 0x280, 0xF8);
			f32 x = menu->_5D8 - 42.0f + menu->_538;
			f32 y = 180.0f + menu->_5DC + menu->_53C;
			fn_8005D148(menu->mStepTime == 100 ? -1 : menu->_520, x, y, menu->mAlpha, 1.0f, menu->_694);
			fn_8005CD3C(lbl_80365E08.mEntries[menu->_520].mId, 320.0f, 224.0f, menu->mAlpha, menu->_66C);
			for (int i = 255; i >= 0; i--) {
				fn_8006E44C(((UnkIdPair*)((u8*)GAME_PLAYER_IDS + 0x9688))[i].mId, 4);
			}
			fn_8004EE0C(menu);
			GXSetScissor(0, 0, 0x280, 0x1C0);
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
// NONMATCHING: the locals get other registers
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
		fn_80196610(lbl_8025C400, color, c, shift + left - w / 2, y - h / 2, 0.0f, w, h);
	}
}

/**
 * @note Address: 0x80058764
 * @note Size: 0x458
 */
// NONMATCHING: the shift-jis test materializes its bool differently
extern "C" void fn_80058764(MenuStatus* menu)
{
	CPad pad(0);
	if (pad.mStatus->_14 & 0x200) {
		menu->_610 = 1086.0f;
		fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		for (int i = 0; i < 4; i++) {
			menu->mTabs[i]._00 = 0.0f;
			if (menu->_674) {
				menu->mTabs[i].mX = 198.0f + i * 108 - 8.0f;
			} else {
				menu->mTabs[i].mX = 252.0f + i * 108;
			}
		}
		menu->mLastStep = menu->mStep;
		menu->mStep     = 4;
		menu->mNextStep = 1;
		menu->mStepTime = 0;
		u32 color       = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
		menu->mGlyphs.Set(fn_8006DBF0(0x17), color);
		char* msg  = fn_8006DBF0((menu->mSelect >= 4 ? 0 : sStatusTabMsg[menu->mSelect]) + 0x17);
		menu->_684 = 1;
		menu->_690 = msg;
		menu->_51C = 0;
	}
}
