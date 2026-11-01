#include "game/particle_srt.h"
#include "game/stg.h"
#include "game/cfont.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern "C" u8 lbl_8025C400[];
// Formats in the original string pool; retained until the remaining menus are reconstructed.
extern "C" char lbl_801FF4B8[];
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
char* fn_8006DC8C(s32 id);
char* fn_8006DDB8(s32 id);
char* fn_8006E510(s16 id);
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
u16 fn_8006E44C(s16 id, s32 kind);
void fn_8004EE0C(MenuStatus* menu);
void fn_800559C4(MenuStatus* menu);
void fn_800554A8(MenuStatus* menu);
void fn_8004F930(MenuStatus* menu);
void fn_80053714(MenuStatus* menu);
void fn_80053D58(MenuStatus* menu);
void GXSetScissor(u32 left, u32 top, u32 width, u32 height);
BOOL fn_80086F24(void* file);
void fn_8000B394(void* game, s32 stage, s32 a);
BOOL fn_800DC754(struct UnkIdPair* items, s16 id, s16 n);
void fn_8005BE74(MenuStatus* menu);
void fn_80060118(MenuItem* menu);
void fn_8004E54C(MenuPC* menu);
void fn_8003D350(MenuSave* menu);
s32 fn_80087A1C(void* save, s32 slot, s32* size, s32* sector);
void fn_8007EEA4(void* evt, s32 id, s32 a, s32 b, s32 c);
void fn_80061480(MenuMain* menu);
BOOL fn_800617DC(CMenu* menu, u32 frames);
void fn_800623F8(MenuWindow* win);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
u16 GXGetTexObjWidth(const GXTexObj* obj);
u16 GXGetTexObjHeight(const GXTexObj* obj);
CTexObj* fn_8019E8F4(CFont* font, const char* str, s32* width);
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
#define PAD_REPEAT(pad) (((MenuPadStatus*)(pad).mPad)->mRepeat)

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
			if (lbl_8025C400[0x3318] == 6 && (IsSjisLead(*p) & 1)) {
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
static inline void SelectCurrentMenuStage(s32* stage, s32 current, MenuStageSelect* menu)
{
	int i = 0;
	for (; *stage >= 0; stage++, i++) {
		if (*stage == current) {
			menu->mSelect = i;
			break;
		}
	}
}
static inline void OpenStageSelect(MenuStageSelect* menu)
{
	menu->mState = 1;
	menu->StopFade();
	menu->mWin.mVisible = TRUE;
	if (GAME_FLAGS & 0x0008000000000000ULL) {
		menu->mSelect = -1;
		SelectCurrentMenuStage(sStageOrder, GAME_STAGE, menu);
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
	s32 mUsed;           // _00
	s32 mBroken;         // _04
	u8 _08[0x2C - 0x8];  // _08
	u32 mSaveTime;       // _2C, in seconds
	u8 _30[0x7C - 0x30]; // _30
	s32 _7C;             // _7C
	s32 _80;             // _80
	u8 _84[0x8C - 0x84]; // _84
	s32 _8C;             // _8C
	s32 _90;             // _90
	s32 _94;             // _94
	f32 _98;             // _98
	f32 _9C;             // _9C
	s32 _A0;             // _A0
	s32 _A4;             // _A4
	s32 _A8;             // _A8
	u8 _AC[0xC8 - 0xAC]; // _AC
	s32 mPlayTime;       // _C8, in frames
};

// the memory card code's state, GAME_SAVE
struct UnkSaveData {
	s32 mState;                   // _00
	u8 _04[0x16200 - 0x4];        // _04
	UnkCardFile mFiles[3];        // _16200
	u8 _16464[0x16A08 - 0x16464]; // _16464
	s32 _16A08;                   // _16A08, a card job is running
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
		} else if (pad.mPad->mTrigger & 0x200) {
			menu->Close();
			sMenuMCTest.mFading  = FALSE;
			sMenuMCTest.mFadeDir = 0;
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		} else if (pad.mPad->mTrigger & 0x100) {
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
extern "C" void fn_80032CAC(MenuMCTest* menu, s32 id)
{
	char* formats = lbl_801FF4B8;
	switch (id) {
	case 0:
		menu->mMode = 0;
		break;
	case 1:
		fn_80089890(GAME_SAVE, sMenuMain.mSlot, 1);
		menu->mMode = 1;
		break;
	case 2:
		sprintf(menu->mText[menu->mSelect], (formats + 0x1ee0), menu->mSelect);
		fn_8008A1F4(GAME_SAVE, sMenuMain.mSlot, menu->mSelect, 1);
		menu->mMode = 3;
		break;
	case 4:
		sprintf(menu->mText[menu->mSelect], (formats + 0x1ef0), menu->mSelect);
		fn_80089C90(GAME_SAVE, sMenuMain.mSlot, menu->mSelect, 1);
		menu->mMode = 5;
		break;
	case 6:
		sprintf(menu->mText[menu->mSelect], (formats + 0x1f00), menu->mSelect);
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
		} else if (pad.mPad->mTrigger & 0x200) {
			menu->Close();
			sMenuMain.StopMainFade();
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		} else if (pad.mPad->mTrigger & 0x100) {
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
		s32* stages   = sStageOrder;
		s32 stage;
		s32 current = GAME_STAGE;
		for (int i = 0; stages[i] >= 0; i++) {
			stage = stages[i];
			if (stage == current) {
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
 * @note Address: 0x8003D350
 * @note Size: 0x3AC
 */
extern "C" void fn_8003D350(MenuSave* menu)
{
	menu->mState = 1;
	menu->StopFade();
	menu->mWin.mVisible = TRUE;
	GAME_FLAGS |= 2;
	menu->mWin.SetSize(0.0f, 0.0f);
	menu->mWin.SetPos(0.0f, 0.0f);
	menu->_158      = -10.0f;
	menu->_15C      = -200.0f;
	menu->_160      = -10.0f;
	menu->_164      = 0.0f;
	menu->_168      = 0.0f;
	menu->_16C      = 0.0f;
	menu->_170      = 0.0f;
	menu->_174      = 0.0f;
	menu->_264      = 0.0f;
	menu->_268      = 1.0f;
	menu->mStep     = 0;
	menu->mLastStep = menu->mStep;
	menu->mStepTime = 0;
	menu->_27C      = 0.0f;
	menu->mTimer    = 0;
	if (menu->_28C == -1) {
		menu->_284 = *(s32*)(lbl_8025C400 + 0x9108);
	} else {
		menu->_284 = menu->_28C;
	}
	menu->mYesNo  = 0;
	menu->mResult = 0;
	menu->_25C    = 0;
	menu->_260    = 0;
	menu->_261    = 0;
	menu->_262    = 0;
	menu->_263    = 0;
	menu->mSlot   = 0;
	menu->_2A4    = 0.0f;
	menu->_538    = 0;
	fn_8001E904(GAME_SOUND, 0x14B, 0xFF);
	u32 color = (u32)(255.0f * menu->_268) | 0xFFFFFF00;
	menu->mGlyphs.Set(fn_8006DBF0(0x62), color);
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
		} else if (pad.mPad->mTrigger & 0x1200) {
			menu->Close();
		} else if (pad.mPad->mTrigger & 0x100) {
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
extern "C" BOOL fn_80061AE8(CMenu* menu)
{
	MenuWindow* win = &menu->mWin;
	win->mTex       = GAME_TEX->Init(&GAME_TPL->descriptorArray[138], 0);
	CTexObj* tex    = &win->mTex;
	u16 w           = GXGetTexObjWidth(tex);
	win->mScaleX    = win->mWidth / w;
	u16 h           = GXGetTexObjHeight(tex);
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
		menu->mGlyphs.mGlyphs[i].mY = 0.0f;
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
	s16 num            = 0;
	UnkPlayerIds* data = GAME_PLAYER_IDS;
	for (int i = 255; i >= 0; i--) {
		if (id == data->mPairs[i].mId) {
			num = data->mPairs[i].mValue;
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

// a placed thing on the stage select's map, negative when the slot is free
struct UnkStagePos {
	s16 mX;     // _00
	s16 mY;     // _02
	s16 mPrice; // _04
	s16 _06;    // _06
};

struct MenuShopList {
	s32 first;
	UnkStagePos entries[20];
	s32 indices[20];
	UnkStagePos previous;
	UnkStagePos next;
};
// Visible PC inventory pages, status availability, and reward rows.
MenuShopList lbl_80366A6C;
MenuShopList lbl_80366B70;
u8 lbl_80366C74[24];
u8 lbl_80366C8C[16];
static UnkStagePos lbl_80366C9C[20];
struct MenuRewardRow {
	s32 id;
	s32 count;
	s32 value;
};
MenuRewardRow lbl_80366D3C[10];
static UnkStagePos lbl_80366DB4[20];

/**
 * @note Address: 0x800345A8
 * @note Size: 0xC4
 */
// NONMATCHING: the table pointer and the counter swap registers
extern "C" s32 fn_800345A8()
{
	s32 count = 0;
	s16 x;
	s16 y;
	UnkStagePos* table = lbl_80366DB4;
	int i              = 0;
	for (; i < 20; table++, i++) {
		x = table->mX;
		y = table->mY;
		if (x >= 0 && y >= 0)
			count++;
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
	s16 x;
	s16 y;
	s32 count = 0;
	for (int i = 0; i < 20; i++) {
		x = lbl_80366C9C[i].mX;
		y = lbl_80366C9C[i].mY;
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
// NONMATCHING: the counters and the copied values get other registers
extern "C" void fn_8003466C(MenuShopList* list)
{
	s16 x;
	s16 y;
	s32 count;
	s32 n;
	s16 price;
	int i;
	for (i = 0; i < 20; i++) {
		list->entries[i].mX     = -1;
		list->entries[i].mY     = -1;
		list->entries[i].mPrice = -1;
		list->indices[i]        = -1;
	}
	count = 0;
	n     = 0;
	for (i = 0; i < 20; i++) {
		x     = lbl_80366DB4[i].mX;
		y     = lbl_80366DB4[i].mY;
		price = lbl_80366DB4[i].mPrice;
		if (x >= 0 && y >= 0) {
			if (count >= list->first && count < list->first + 9 && n < 9) {
				list->entries[n].mX     = x;
				list->entries[n].mY     = y;
				list->entries[n].mPrice = price;
				list->indices[n]        = i;
				n++;
			}
			count++;
		}
	}
	list->previous.mX     = -1;
	list->previous.mY     = -1;
	list->previous.mPrice = -1;
	list->next.mX         = -1;
	list->next.mY         = -1;
	list->next.mPrice     = -1;
	count                 = 0;
	n                     = 0;
	for (i = 0; i < 20; i++) {
		x     = lbl_80366DB4[i].mX;
		y     = lbl_80366DB4[i].mY;
		price = lbl_80366DB4[i].mPrice;
		if (x >= 0 && y >= 0) {
			if (count >= list->first + 9 && count < list->first + 10) {
				(&list->next)[n].mX     = x;
				(&list->next)[n].mY     = y;
				(&list->next)[n].mPrice = price;
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
		x     = lbl_80366DB4[i].mX;
		y     = lbl_80366DB4[i].mY;
		price = lbl_80366DB4[i].mPrice;
		if (x >= 0 && y >= 0) {
			if (count >= list->first - 1 && count < list->first) {
				(&list->previous)[n].mX     = x;
				(&list->previous)[n].mY     = y;
				(&list->previous)[n].mPrice = price;
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
// NONMATCHING: the counters and the table pointer get other registers
extern "C" void fn_80034EAC(UnkStatusTable* table)
{
	s32 count;
	s16 num;
	int i;
	UnkIdPair* pairs;
	s16 id;
	s32 n;
	for (i = 0; i < 256; i++) {
		table->mEntries[i].mId            = -1;
		table->mEntries[i]._02            = -1;
		table->mEntries[i].mStatus.mFlags = 0;
	}
	n     = 0;
	count = 0;
	for (i = 255; i >= 0; i--) {
		pairs = GAME_PLAYER_IDS->mPairs;
		id    = pairs[i].mId;
		num   = pairs[i].mValue;
		if (fn_8006E44C(id, 4) && id >= 0 && num > 0) {
			if (count >= table->_00 * 4 && count < (table->_00 + 2) * 4) {
				table->mEntries[n].mId = id;
				table->mEntries[n]._02 = num;
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
		table->_C04[i].mId            = -1;
		table->_C04[i]._02            = -1;
		table->_C04[i].mStatus.mFlags = 0;
		table->_C34[i].mId            = -1;
		table->_C34[i]._02            = -1;
		table->_C34[i].mStatus.mFlags = 0;
	}
	for (i = 255; i >= 0; i--) {
		pairs = GAME_PLAYER_IDS->mPairs;
		id    = pairs[i].mId;
		num   = pairs[i].mValue;
		if (fn_8006E44C(id, 4) && id >= 0 && num > 0) {
			if (count >= (table->_00 + 2) * 4 && count < (table->_00 + 4) * 4) {
				table->_C34[n].mId = id;
				table->_C34[n]._02 = num;
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
		pairs = GAME_PLAYER_IDS->mPairs;
		id    = pairs[i].mId;
		num   = pairs[i].mValue;
		if (fn_8006E44C(id, 4) && id >= 0 && num > 0) {
			if (count >= (table->_00 - 1) * 4 && count < (table->_00 + 1) * 4) {
				table->_C04[n].mId = id;
				table->_C04[n]._02 = num;
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
	UnkIdPair* pairs;
	count = 0;
	for (i = 255; i >= 0; i--) {
		pairs = GAME_PLAYER_IDS->mPairs;
		id    = pairs[i].mId;
		num   = pairs[i].mValue;
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
// NONMATCHING: the item counts and the drop call arguments take other registers
extern "C" void fn_80058C7C(MenuStatus* menu)
{
	CPad pad(0);
	s32 count = CountMenuItems();
	switch (menu->mStepTime) {
	case 0:
		if (count > 1) {
			if (PAD_REPEAT(pad) & 0x40001) {
				if (--menu->_520 < 0) {
					menu->_520 = count - 1;
				}
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			} else if (PAD_REPEAT(pad) & 0x80002) {
				if (++menu->_520 > count - 1) {
					menu->_520 = 0;
				}
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		}
		if (pad.mPad->mTrigger & 0x200) {
			menu->mLastStep = menu->mStep;
			menu->mStep     = 4;
			menu->mNextStep = 2;
			menu->mStepTime = 0;
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
			for (int i = 0; i < 4; i++) {
				menu->mTabs2[i]._00 = 0.0f;
				if (menu->_674) {
					menu->mTabs2[i].mX = 198.0f + i * 108 - 8.0f;
				} else {
					menu->mTabs2[i].mX = 252.0f + i * 108;
				}
			}
			menu->_5E0 = 758.0f;
		} else if ((pad.mPad->mTrigger & 0x100) && lbl_80365E08.mEntries[menu->_520]._02 >= 1) {
			menu->_570 = 336.0f;
			menu->_580 = 336.0f;
			menu->_534 = 1;
			menu->mStepTime++;
		}
		break;
	case 1: {
		s32 num = lbl_80365E08.mEntries[menu->_520]._02;
		if (num >= 1) {
			if (PAD_REPEAT(pad) & 0x40001) {
				if (--menu->_534 < 1) {
					menu->_534 = num;
				}
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			} else if (PAD_REPEAT(pad) & 0x80002) {
				if (++menu->_534 > num) {
					menu->_534 = 1;
				}
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
			if (pad.mPad->mTrigger & 0x100) {
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				menu->mStepTime++;
			} else if (pad.mPad->mTrigger & 0x200) {
				fn_8001E904(GAME_SOUND, 0x86, 0xFF);
				menu->_570      = 725.0f;
				menu->_580      = 937.0f;
				menu->mStepTime = 0;
			}
		}
		break;
	}
	case 2:
		if (PAD_REPEAT(pad) & 0x40001) {
			if (menu->_530 > 0) {
				menu->_530--;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		} else if (PAD_REPEAT(pad) & 0x80002) {
			if (menu->_530 < 1) {
				menu->_530++;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		}
		if (pad.mPad->mTrigger & 0x200) {
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
			menu->mStepTime = 1;
		} else if (pad.mPad->mTrigger & 0x100) {
			switch (menu->_530 == 0) {
			default:
				UnkStatusEntry* entry = &lbl_80365E08.mEntries[menu->_520];
				fn_800DC754(GAME_PLAYER_IDS->mPairs, entry->mId, menu->_534);
				menu->_570      = 725.0f;
				menu->_580      = 937.0f;
				menu->mStepTime = 0;
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				menu->_520 = 0;
				fn_80034EAC(&lbl_80365E08);
				menu->_5E0 = 360.0f - (CountMenuItems() - 1) * 54;
				if (CountMenuItems() == 0) {
					menu->_5E0 = 360.0f;
				}
				break;
			case false:
				fn_8001E904(GAME_SOUND, 0x86, 0xFF);
				menu->mStepTime = 1;
				break;
			}
		}
		break;
	}
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
 * @note Address: 0x8005A290
 * @note Size: 0x110C
 */
// the status menu's tabs: pick one and open its page, or close the menu
// NONMATCHING: the item counts take other saved registers
extern "C" void fn_8005A290(MenuStatus* menu, u32 frames)
{
	CPad pad(0);
	switch (menu->mStepTime) {
	case 0:
		if (PAD_REPEAT(pad) & 0x40001) {
			if (--menu->mSelect < 0) {
				if (menu->_674) {
					menu->mSelect = 3;
				} else {
					menu->mSelect = 2;
				}
			}
			char* msg  = fn_8006DBF0((menu->mSelect >= 4 ? 0 : sStatusTabMsg[menu->mSelect]) + 0x17);
			menu->_684 = 1;
			menu->_690 = msg;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x80002) {
			if (menu->_674) {
				if (++menu->mSelect > 3) {
					menu->mSelect = 0;
				}
			} else if (++menu->mSelect > 2) {
				menu->mSelect = 0;
			}
			char* msg  = fn_8006DBF0((menu->mSelect >= 4 ? 0 : sStatusTabMsg[menu->mSelect]) + 0x17);
			menu->_684 = 1;
			menu->_690 = msg;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		}
		if (pad.mPad->mTrigger & 0x1200) {
			menu->_554 = -200.0f;
			for (int i = 0; i < 4; i++) {
				menu->mTabs[i].mX = -102.0f;
			}
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
			menu->mStepTime++;
			menu->mWait = frames * 4;
			for (int i = 0; i < 20; i++) {
				menu->mGlyphs.mGlyphs[i].mY = 0.0f;
			}
		} else if (pad.mPad->mTrigger & 0x100) {
			switch (menu->mSelect) {
			case 0: {
				menu->mLastStep = menu->mStep;
				menu->mStep     = 4;
				menu->mNextStep = 2;
				menu->mStepTime = 0;
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
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
				menu->mGlyphs.Set(fn_8006DBF0(0x18), color);
				s16 id     = lbl_80365E08.mEntries[menu->_520].mId;
				menu->_524 = menu->_520;
				if (id >= 0 && CountMenuItems() > 0) {
					char* msg  = fn_8006E510(id);
					menu->_684 = 1;
					menu->_690 = msg;
				} else {
					menu->_684 = 1;
					menu->_690 = NULL;
				}
				break;
			}
			case 1: {
				menu->_5F0      = 360.0f;
				menu->mLastStep = menu->mStep;
				menu->mStep     = 4;
				menu->mNextStep = 9;
				menu->mStepTime = 0;
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				for (int i = 0; i < 4; i++) {
					menu->mTabs[i].mX = -102.0f;
				}
				u32 color = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
				menu->mGlyphs.Set(fn_8006DBF0(0x1A), color);
				if (lbl_80366C8C[menu->_528]) {
					char* msg  = fn_8006DC8C(menu->_528);
					menu->_684 = 1;
					menu->_690 = msg;
				} else {
					char* msg  = fn_8006DBF0(0xE);
					menu->_684 = 1;
					menu->_690 = msg;
				}
				break;
			}
			case 2: {
				menu->_610      = 360.0f;
				menu->mLastStep = menu->mStep;
				menu->mStep     = 4;
				menu->mNextStep = 5;
				menu->mStepTime = 0;
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				for (int i = 0; i < 4; i++) {
					menu->mTabs[i].mX = -102.0f;
				}
				u32 color = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
				menu->mGlyphs.Set(fn_8006DBF0(0x19), color);
				menu->_684 = 1;
				menu->_690 = NULL;
				break;
			}
			case 3: {
				menu->_600      = 360.0f;
				menu->mLastStep = menu->mStep;
				menu->mStep     = 4;
				menu->mNextStep = 8;
				menu->mStepTime = 0;
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				for (int i = 0; i < 4; i++) {
					menu->mTabs[i].mX = -102.0f;
				}
				u32 color = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
				menu->mGlyphs.Set(fn_8006DBF0(0x1B), color);
				if (lbl_80366C74[menu->_52C]) {
					char* msg  = fn_8006DDB8(menu->_52C);
					menu->_684 = 1;
					menu->_690 = msg;
				} else {
					char* msg  = fn_8006DBF0(0xE);
					menu->_684 = 1;
					menu->_690 = msg;
				}
				break;
			}
			default:
				fn_8001E904(GAME_SOUND, 0x147, 0xFF);
				break;
			}
		}
		break;
	case 2:
		break;
	case 1:
		if (--menu->mWait < 0) {
			GAME_FLAGS &= ~2ULL;
			menu->Close();
			sMenuMain.StopMainFade();
			fn_8001E660(GAME_SOUND, 0xFFFF0004);
			if (menu->_664) {
				fn_8001E904(GAME_SOUND, menu->_664, menu->_668);
			}
			menu->mStepTime++;
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

// the os time functions, game/ has no header for them
typedef s64 OSTime;
struct OSCalendarTime {
	s32 sec;  // _00
	s32 min;  // _04
	s32 hour; // _08
	s32 mday; // _0C
	s32 mon;  // _10
	s32 year; // _14
	s32 wday; // _18
	s32 yday; // _1C
	s32 msec; // _20
	s32 usec; // _24
};

extern "C" {
void OSTicksToCalendarTime(OSTime ticks, OSCalendarTime* td);
OSTime OSCalendarTimeToTicks(OSCalendarTime* td);
}

#define OS_TIMER_CLOCK (*(u32*)0x800000F8 / 4)

/**
 * @note Address: 0x8003CA2C
 * @note Size: 0x33C
 */
// waits for the card to list its files, then fills in the three file headers
// NONMATCHING: the file, header and card pointers rotate through other saved registers
extern "C" void fn_8003CA2C(MenuSave* menu, s32 frames)
{
	UnkCardFile* file;
	UnkSaveHeader* header;
	s32 state = SAVE_STATE;
	menu->mTimer -= frames;
	if (menu->mTimer < 0) {
		menu->mTimer = 0;
	}
	switch (state) {
	case 0:
		for (int i = 0; i < 3; i++) {
			file = &((UnkSaveData*)GAME_SAVE)->mFiles[i];
			if (file->mUsed != 0) {
				header = &menu->mFiles[i];
				OSCalendarTime cal;
				OSTicksToCalendarTime((OSTime)file->mSaveTime * OS_TIMER_CLOCK, &cal);
				s32 v28        = file->_7C;
				s32 v14        = file->_8C;
				s32 v04        = file->_90;
				f32 v08        = file->_98;
				f32 v0C        = file->_9C;
				s32 v10        = file->_A0;
				s32 v18        = file->_A4;
				s32 v1C        = file->_94;
				s32 hours      = file->mPlayTime / 216000;
				s32 minutes    = (file->mPlayTime / 3600) % 60;
				s32 v2C        = file->_80;
				header->_00    = 1;
				header->_04    = v04;
				header->_08    = v08;
				header->_0C    = v0C;
				header->_10[0] = v10;
				header->_10[1] = v14;
				header->_10[2] = v18;
				header->_10[3] = v1C;
				header->_10[4] = hours;
				header->_10[5] = minutes;
				header->_10[6] = v28;
				header->_10[7] = v2C;
				if (file->_A8 > 0) {
					header->_44 = 1;
				} else {
					header->_44 = 0;
				}
				OSCalendarTimeToTicks(&cal);
				if (file->mBroken != 0 || !fn_80086F24(file)) {
					header->_40 = 1;
				} else {
					header->_40 = 0;
				}
			}
		}
		menu->_250 = *(s32*)(lbl_8025C400 + 0xFEE04);
		menu->_254 = *(s32*)(lbl_8025C400 + 0xFEE00);
		if (menu->_263 == TRUE) {
			menu->SetStep(4);
			menu->_27C = 30.0f;
			menu->_263 = 0;
		} else {
			menu->SetStep(2);
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
				menu->SetStep(8);
				menu->mYesNo = 1;
			} else if (result == -5) {
				menu->SetStep(13);
				menu->_261 = TRUE;
			} else {
				menu->SetStep(11);
				menu->_174 = -32.0f;
			}
			menu->_268 = 1.0f;
			break;
		default:
			menu->SetStep(11);
			menu->_268 = 1.0f;
			menu->_174 = -32.0f;
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
extern "C" void fn_8003C0A8(MenuSave* menu, s32 frames)
{
	UnkCardFile* file;
	UnkSaveData* save = (UnkSaveData*)GAME_SAVE;
	s32 state         = save->mState;
	menu->_263        = 0;
	menu->mTimer -= frames;
	if (menu->mTimer < 0) {
		menu->mTimer = 0;
	}
	switch (state) {
	case 0: {
		file = &save->mFiles[menu->_284];
		if (file->mUsed != 0) {
			OSCalendarTime cal;
			OSTicksToCalendarTime((OSTime)file->mSaveTime * OS_TIMER_CLOCK, &cal);
			s32 v28                         = file->_7C;
			s32 v14                         = file->_8C;
			s32 v04                         = file->_90;
			f32 v08                         = file->_98;
			f32 v0C                         = file->_9C;
			s32 v10                         = file->_A0;
			s32 v18                         = file->_A4;
			s32 v1C                         = file->_94;
			s32 hours                       = file->mPlayTime / 216000;
			s32 minutes                     = (file->mPlayTime / 3600) % 60;
			s32 v2C                         = file->_80;
			menu->mFiles[menu->_284]._00    = 1;
			menu->mFiles[menu->_284]._04    = v04;
			menu->mFiles[menu->_284]._08    = v08;
			menu->mFiles[menu->_284]._0C    = v0C;
			menu->mFiles[menu->_284]._10[0] = v10;
			menu->mFiles[menu->_284]._10[1] = v14;
			menu->mFiles[menu->_284]._10[2] = v18;
			menu->mFiles[menu->_284]._10[3] = v1C;
			menu->mFiles[menu->_284]._10[4] = hours;
			menu->mFiles[menu->_284]._10[5] = minutes;
			menu->mFiles[menu->_284]._10[7] = v2C;
			menu->mFiles[menu->_284]._10[6] = v28;
			menu->mFiles[menu->_284]._40    = 0;
			if (file->_A8 > 0) {
				menu->mFiles[menu->_284]._44 = 1;
			} else {
				menu->mFiles[menu->_284]._44 = 0;
			}
		}
		menu->_250 = *(s32*)(lbl_8025C400 + 0xFEE04);
		menu->_254 = *(s32*)(lbl_8025C400 + 0xFEE00);
		fn_80089890(save, menu->mSlot, 1);
		menu->SetStep(1);
		menu->_28C = menu->_284;
		menu->_263 = 1;
		menu->_174 = 0.0f;
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
			if (state == 2 && (result == -6 || result == -13)) {
				menu->SetStep(8);
				menu->mYesNo = 1;
				menu->_261   = TRUE;
			} else if (result == -3 || result == -5) {
				menu->SetStep(14);
				menu->_25C = 3;
				menu->_261 = TRUE;
			} else {
				menu->SetStep(11);
				menu->_174 = -32.0f;
			}
			menu->_268 = 1.0f;
			break;
		default:
			menu->SetStep(11);
			menu->_268 = 1.0f;
			menu->_174 = -32.0f;
			break;
		}
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
		if ((pad.mPad->mTrigger & 0x100) || (menu->_27C -= frames) < 0.0f) {
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
	if (pad.mPad->mTrigger & 0x100) {
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
	if (pad.mPad->mTrigger & 0x100) {
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
		if (pad.mPad->mTrigger & 0x100) {
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
	if (pad.mPad->mTrigger & 0x100) {
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
	if (pad.mPad->mTrigger & 0x100) {
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
	if (pad.mPad->mTrigger & 0x100) {
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
	if (pad.mPad->mTrigger & 0x100) {
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
	} else if (pad.mPad->mTrigger & 0x200) {
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
		if (pad.mPad->mTrigger & 0x100) {
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
		if (pad.mPad->mTrigger & 0x100) {
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
	if (pad.mPad->mTrigger & 0x100) {
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
// NONMATCHING: the float temporaries take other saved registers
extern "C" void fn_800373D4(MenuSave* menu)
{
	if (menu->mGlyphs.mActive) {
		fn_8019B56C(GS_RENDER, 1, 4, 5, 15);
		fn_80199AB0(lbl_8025C400, 0, 0);
		fn_8019B464(GS_RENDER, 1, 7, 0);
		fn_8019AFA0(GS_TEV, 0);
		CTexObj blob = GAME_TEX->Init(&GAME_TPL->descriptorArray[108], 0);
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
		u32 color = menu->mGlyphs.mColor & 0xFFFFFF00;
		f32 zero  = 0.0f;
		for (int i = 0; i < menu->mGlyphs.mCount; i++) {
			MenuGlyph* glyph = &menu->mGlyphs.mGlyphs[i];
			t                = glyph->mX;
			if (!(t <= 0.0f)) {
				CFont font;
				s32 width;
				CTexObj tex = *fn_8019E8F4(&font, glyph->mChar, &width);
				s0          = sin(glyph->mRotX);
				bob         = 4.0f * s0;
				rot         = sin(0.7f * glyph->mRotY);
				s1          = sin(3.1415927f * t);
				s2          = sin(1.5707964f * t);
				scale       = 0.2f * s1 + s2;
				x           = 24.0f + (48.0f * i + (320.0f - 48.0f * menu->mGlyphs.mCount * 0.5f));
				alpha       = 255.0f * t;
				rot         = -(0.017453292f * (10.0f * rot));
				y           = 64.0f + bob;
				u32 c       = alpha | 0xFFFFFF00;
				fn_80197800(lbl_8025C400, &blob, c, c, c, c, x, y, 0.0f, 20.0f * (2.0f * scale), 20.0f * (2.0f * scale), 0.0f, -zero, rot);
				scale = 20.0f * (1.7f * scale);
				pad   = (width / 24.0f) * scale;
				pad   = scale - pad;
				for (int j = 0; j < 4; j++) {
					fn_80197800(lbl_8025C400, &tex, alpha, alpha, alpha, alpha, pad / 2 + (x + lbl_8021238C[j]),
					            bob + (64.0f + lbl_8021239C[j]), 0.0f, scale, scale, 0.0f, -zero, rot);
				}
				c = color | alpha;
				fn_80197800(lbl_8025C400, &tex, c, c, c, c, pad / 2 + x, y, 0.0f, scale, scale, 0.0f, -zero, rot);
			}
		}
		fn_8019AFA0(GS_TEV, 4);
		fn_8019B464(GS_RENDER, 0, 7, 0);
		fn_80199AB0(lbl_8025C400, 0, 0);
	}
}

// the same for the item menu
/**
 * @note Address: 0x8005C798
 * @note Size: 0x5A4
 */
// NONMATCHING: the float temporaries take other saved registers
extern "C" void fn_8005C798(MenuItem* menu)
{
	if (menu->mGlyphs.mActive) {
		fn_8019B56C(GS_RENDER, 1, 4, 5, 15);
		fn_80199AB0(lbl_8025C400, 0, 0);
		fn_8019B464(GS_RENDER, 1, 7, 0);
		fn_8019AFA0(GS_TEV, 0);
		CTexObj blob = GAME_TEX->Init(&GAME_TPL->descriptorArray[108], 0);
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
		u32 color = menu->mGlyphs.mColor & 0xFFFFFF00;
		f32 zero  = 0.0f;
		for (int i = 0; i < menu->mGlyphs.mCount; i++) {
			MenuGlyph* glyph = &menu->mGlyphs.mGlyphs[i];
			t                = glyph->mX;
			if (!(t <= 0.0f)) {
				CFont font;
				s32 width;
				CTexObj tex = *fn_8019E8F4(&font, glyph->mChar, &width);
				s0          = sin(glyph->mRotX);
				bob         = 4.0f * s0;
				rot         = sin(0.7f * glyph->mRotY);
				s1          = sin(3.1415927f * t);
				s2          = sin(1.5707964f * t);
				scale       = 0.2f * s1 + s2;
				x           = 24.0f + (48.0f * i + (320.0f - 48.0f * menu->mGlyphs.mCount * 0.5f));
				alpha       = 255.0f * t;
				rot         = -(0.017453292f * (10.0f * rot));
				y           = 64.0f + bob;
				u32 c       = alpha | 0xFFFFFF00;
				fn_80197800(lbl_8025C400, &blob, c, c, c, c, x, y, 0.0f, 20.0f * (2.0f * scale), 20.0f * (2.0f * scale), 0.0f, -zero, rot);
				scale = 20.0f * (1.7f * scale);
				pad   = (width / 24.0f) * scale;
				pad   = scale - pad;
				for (int j = 0; j < 4; j++) {
					fn_80197800(lbl_8025C400, &tex, alpha, alpha, alpha, alpha, pad / 2 + (x + lbl_80211E50[j]),
					            bob + (64.0f + lbl_80211E60[j]), 0.0f, scale, scale, 0.0f, -zero, rot);
				}
				c = color | alpha;
				fn_80197800(lbl_8025C400, &tex, c, c, c, c, pad / 2 + x, y, 0.0f, scale, scale, 0.0f, -zero, rot);
			}
		}
		fn_8019AFA0(GS_TEV, 4);
		fn_8019B464(GS_RENDER, 0, 7, 0);
		fn_80199AB0(lbl_8025C400, 0, 0);
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
		tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[153], 0);
		fn_8019AFA0(GS_TEV, 0);
		u16 w     = GXGetTexObjWidth(&tex);
		u16 h     = GXGetTexObjHeight(&tex);
		u32 color = (u32)(255.0f * alpha) | 0xC8C8C800;
		fn_80197800(lbl_8025C400, &tex, color, color, color, color, x - 16.0f, y, 0.0f, 0.65f * (scale * w), 0.65f * (scale * h), 0.0f,
		            0.0f, 0.0f);
	}
	tex   = GAME_TEX->Init(&GAME_TPL->descriptorArray[125], 0);
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
			fn_8019AFA0(GS_TEV, 0);
			fn_80197800(lbl_8025C400, &tex, top, top, bottom, bottom, 18.0f * scale + (7.0f * w + (x - i * w) - 8.0f * scale), y, 0.0f, w,
			            h, 0.0f, 0.0f, 0.0f);
			fn_8019AFA0(GS_TEV, 4);
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
		tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[152], 0);
		fn_8019AFA0(GS_TEV, 0);
		u16 w     = GXGetTexObjWidth(&tex);
		u16 h     = GXGetTexObjHeight(&tex);
		u32 color = (u32)(255.0f * alpha) | 0xC8C8C800;
		fn_80197800(lbl_8025C400, &tex, color, color, color, color, x - 16.0f, y, 0.0f, 0.8f * (scale * w), 0.8f * (scale * h), 0.0f, 0.0f,
		            0.0f);
	}
	tex   = GAME_TEX->Init(&GAME_TPL->descriptorArray[125], 0);
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
			fn_8019AFA0(GS_TEV, 0);
			fn_80197800(lbl_8025C400, &tex, top, top, bottom, bottom, 18.0f * scale + (7.0f * w + (x - i * w) - 8.0f * scale), y, 0.0f, w,
			            h, 0.0f, 0.0f, 0.0f);
			fn_8019AFA0(GS_TEV, 4);
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
static inline s16 MenuInventoryId(s32 index)
{
	return ((UnkIdPair*)((u8*)GAME_PLAYER_IDS + 0x9688))[index].mId;
}
static inline void MarkMenuInventory()
{
	for (int i = 255; i >= 0; i--) {
		fn_8006E44C(MenuInventoryId(i), 4);
	}
}
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
			MarkMenuInventory();
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

// empties one entry of the status table
static inline void ClearStatusEntry(UnkStatusEntry* e)
{
	e->mId            = -1;
	e->_02            = -1;
	e->mStatus.mFlags = 0;
}

static inline void ClearStatusTable(UnkStatusTable* table)
{
	table->_00 = 0;
	for (int i = 0; i < 256; i++) {
		ClearStatusEntry(&table->mEntries[i]);
	}
	for (int i = 0; i < 4; i++) {
		ClearStatusEntry(&table->_C04[i]);
		ClearStatusEntry(&table->_C34[i]);
	}
}

/**
 * @note Address: 0x80060118
 * @note Size: 0x650
 */
// NONMATCHING: the name lookup after the second count takes another saved register
extern "C" void fn_80060118(MenuItem* menu)
{
	menu->mState = 1;
	menu->StopFade();
	menu->mWin.mVisible = TRUE;
	GAME_FLAGS |= 2;
	menu->mWin.SetSize(0.0f, 0.0f);
	menu->mWin.SetPos(0.0f, 0.0f);
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
	menu->_168    = 207.0f;
	menu->_16C    = 36.0f;
	menu->_170    = -10.0f;
	menu->_174    = -200.0f;
	menu->_178    = -10.0f;
	menu->_17C    = 0.0f;
	menu->_158    = 0;
	menu->_15C    = 0;
	menu->mSelect = 0;
	menu->_19C    = menu->mSelect;
	menu->_190    = 0.0f;
	menu->_194    = 0.0f;
	u32 color     = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
	menu->mGlyphs.Set(fn_8006DBF0(0x18), color);
	menu->_1A0 = 0;
	menu->_1A4 = 0.0f;
	s16 id     = lbl_80365E08.mEntries[menu->mSelect].mId;
	if (id >= 0 && CountMenuItems() > 0) {
		menu->_1A8 = fn_8006E510(id);
		char* msg  = fn_8006E510(id);
		menu->_1A0 = 1;
		menu->_1AC = msg;
	} else {
		menu->_1A0 = 1;
		menu->_1AC = NULL;
	}
	menu->_350 = 0;
	for (int i = 0; i < 20; i++) {
		menu->mBg[i].mRotX = 0.017453292f * (rand() % 360);
		menu->mBg[i].mRotY = 0.017453292f * (rand() % 360);
	}
}

/**
 * @note Address: 0x80057934
 * @note Size: 0x700
 */
// moving the cursor over the map grid, each cell names its place once it's been visited
extern "C" void fn_80057934(MenuStatus* menu)
{
	CPad pad(0);
	s32 grid[13][4] = {
		{ 4, 2, 9, 5 },  { 0, 2, 10, 6 }, { 0, 3, 10, 6 },  { 2, 4, 11, 7 }, { 3, 0, 12, 8 },  { 8, 6, 0, 9 },  { 5, 7, 2, 10 },
		{ 6, 8, 3, 11 }, { 7, 5, 4, 12 }, { 12, 10, 5, 0 }, { 9, 11, 6, 2 }, { 10, 12, 7, 3 }, { 11, 9, 8, 4 },
	};
	switch (menu->mStepTime) {
	case 0:
		if (PAD_REPEAT(pad) & 0x40001) {
			menu->_528 = grid[menu->_528][2];
			if (lbl_80366C8C[menu->_528]) {
				char* msg  = fn_8006DC8C(menu->_528);
				menu->_684 = 1;
				menu->_690 = msg;
			} else {
				char* msg  = fn_8006DBF0(0xE);
				menu->_684 = 1;
				menu->_690 = msg;
			}
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x80002) {
			menu->_528 = grid[menu->_528][3];
			if (lbl_80366C8C[menu->_528]) {
				char* msg  = fn_8006DC8C(menu->_528);
				menu->_684 = 1;
				menu->_690 = msg;
			} else {
				char* msg  = fn_8006DBF0(0xE);
				menu->_684 = 1;
				menu->_690 = msg;
			}
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		}
		if (PAD_REPEAT(pad) & 0x10008) {
			menu->_528 = grid[menu->_528][0];
			if (lbl_80366C8C[menu->_528]) {
				char* msg  = fn_8006DC8C(menu->_528);
				menu->_684 = 1;
				menu->_690 = msg;
			} else {
				char* msg  = fn_8006DBF0(0xE);
				menu->_684 = 1;
				menu->_690 = msg;
			}
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x20004) {
			menu->_528 = grid[menu->_528][1];
			if (lbl_80366C8C[menu->_528]) {
				char* msg  = fn_8006DC8C(menu->_528);
				menu->_684 = 1;
				menu->_690 = msg;
			} else {
				char* msg  = fn_8006DBF0(0xE);
				menu->_684 = 1;
				menu->_690 = msg;
			}
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		}
		if (pad.mPad->mTrigger & 0x200) {
			menu->_5F0 = 1086.0f;
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
			for (int i = 0; i < 4; i++) {
				menu->mTabs[i]._00 = 0.0f;
				if (menu->_674) {
					menu->mTabs[i].mX = 198.0f + i * 108 - 8.0f;
				} else {
					menu->mTabs[i].mX = 252.0f + i * 108;
				}
			}
			menu->_658      = 0.0f;
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
		break;
	case 100:
		if (pad.mPad->mTrigger & 0x300) {
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
			menu->mStepTime = 0;
			menu->_670      = 0.0f;
		}
		break;
	}
}

/**
 * @note Address: 0x80058034
 * @note Size: 0x728
 */
// the same on the 6 by 4 grid, it wraps around
extern "C" void fn_80058034(MenuStatus* menu)
{
	CPad pad(0);
	switch (menu->mStepTime) {
	case 0:
		if (PAD_REPEAT(pad) & 0x40001) {
			if (--menu->_52C < 0) {
				menu->_52C = 23;
			}
			if (lbl_80366C74[menu->_52C]) {
				char* msg  = fn_8006DDB8(menu->_52C);
				menu->_684 = 1;
				menu->_690 = msg;
			} else {
				char* msg  = fn_8006DBF0(0xE);
				menu->_684 = 1;
				menu->_690 = msg;
			}
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x80002) {
			menu->_52C++;
			if (menu->_52C > 23) {
				menu->_52C = 0;
			}
			if (lbl_80366C74[menu->_52C]) {
				char* msg  = fn_8006DDB8(menu->_52C);
				menu->_684 = 1;
				menu->_690 = msg;
			} else {
				char* msg  = fn_8006DBF0(0xE);
				menu->_684 = 1;
				menu->_690 = msg;
			}
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		}
		if (PAD_REPEAT(pad) & 0x10008) {
			s32 row = menu->_52C / 6;
			s32 col = menu->_52C % 6;
			if (--row < 0) {
				row = 3;
			}
			menu->_52C = col + row * 6;
			if (lbl_80366C74[menu->_52C]) {
				char* msg  = fn_8006DDB8(menu->_52C);
				menu->_684 = 1;
				menu->_690 = msg;
			} else {
				char* msg  = fn_8006DBF0(0xE);
				menu->_684 = 1;
				menu->_690 = msg;
			}
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x20004) {
			s32 row = menu->_52C / 6;
			s32 col = menu->_52C % 6;
			if (++row > 3) {
				row = 0;
			}
			menu->_52C = col + row * 6;
			if (lbl_80366C74[menu->_52C]) {
				char* msg  = fn_8006DDB8(menu->_52C);
				menu->_684 = 1;
				menu->_690 = msg;
			} else {
				char* msg  = fn_8006DBF0(0xE);
				menu->_684 = 1;
				menu->_690 = msg;
			}
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		}
		if (pad.mPad->mTrigger & 0x200) {
			menu->_600 = 959.0f;
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
		break;
	case 100:
		if (pad.mPad->mTrigger & 0x300) {
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
			menu->mStepTime = 0;
			menu->_670      = 0.0f;
		}
		break;
	}
}

/**
 * @note Address: 0x80058764
 * @note Size: 0x458
 */
extern "C" void fn_80058764(MenuStatus* menu)
{
	CPad pad(0);
	if (pad.mPad->mTrigger & 0x200) {
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

extern "C" f32 lbl_8065B1A0;
extern "C" void fn_80068C8C(f32*, f32*, f32);

static inline void ResetPCText(MenuPC* menu, s32 step)
{
	menu->mLastStep    = menu->mStep;
	menu->mStep        = step;
	menu->mSubStep     = 0;
	menu->mStepTime    = 0;
	menu->mTextX       = 210.0f;
	menu->mTextY       = 129.0f;
	menu->mTextTargetX = menu->mTextX;
	menu->mTextTargetY = menu->mTextY;
	switch (menu->mStep) {
	case 3:
	case 7:
	case 10:
		menu->mTextY       = 121.0f;
		menu->mTextTargetY = menu->mTextY;
		break;
	}
	menu->mTextIndex = 0;
	menu->_378       = 0;
	menu->_37C       = 0;
	char space[3]    = "\x81\x40";
	for (int i = 0; i < 40; i++)
		menu->mText[i] = *(u16*)space;
	menu->mTextEnd = 0;
	for (int i = 0; i < 40; i++) {
		menu->mGlyphX[i]           = 640.0f;
		menu->mGlyphFlags[i]       = 0;
		*(u16*)menu->mGlyphText[i] = *(u16*)space;
		menu->mGlyphText[i][2]     = 0;
	}
	menu->_364   = 0;
	menu->_368   = 0;
	menu->_36C   = 0;
	menu->_370   = 0;
	menu->_374   = 0;
	lbl_8065B1A0 = 640.0f;
	memset(lbl_803620A8, 0, 0x200);
}

extern "C" void fn_8004BBC0(MenuPC* menu)
{
	CPad pad(0);
	if (pad.mPad->mTrigger & 0x200) {
		ResetPCText(menu, 2);
		fn_8001E904(GAME_SOUND, 0x86, 0xFF);
	}
}
extern "C" void fn_8004B92C(MenuPC* menu)
{
	CPad pad(0);
	if (pad.mPad->mTrigger & 0x200) {
		*(s32*)(lbl_8025C400 + 0x43D0) = 0;
		ResetPCText(menu, 2);
		fn_8001E904(GAME_SOUND, 0x86, 0xFF);
	}
}
extern "C" void fn_8004D584(MenuPC* menu, u32 frames)
{
	switch (menu->mSubStep) {
	case 0:
		fn_8001E904(GAME_SOUND, 0x30008, 0xFF);
		menu->mSubStep++;
		break;
	case 1:
		break;
	}
	fn_80068C8C(&menu->mProgress, &menu->mProgressTarget, menu->mStepSpeed * frames);
	if (menu->mProgress >= 1.0f)
		ResetPCText(menu, 2);
}

extern "C" void fn_8004D85C(MenuPC* menu)
{
	if (*((u8*)menu + 0x3A0)) {
		ResetPCText(menu, 3);
	} else {
		ResetPCText(menu, 1);
		menu->mProgress       = 0.0f;
		menu->mProgressTarget = 1.0f;
		switch (rand() % 2) {
		case 0:
			menu->mStepSpeed = 0.04f;
			break;
		case 1:
			menu->mStepSpeed = 0.08f;
			break;
		}
	}
}

extern "C" s32 lbl_80367A2C[];

static inline s16 MenuHistoryValue(int i)
{
	return lbl_80366C9C[i].mY;
}

static inline void RestorePCHistory(MenuPC* menu)
{
	if (menu->mHistoryCount > 0) {
		for (int i = 0; i < menu->mHistoryCount; i++) {
			lbl_80367A2C[menu->mHistoryStart + i + menu->mHistoryCount] = MenuHistoryValue(i);
		}
		lbl_80367A2C[196] = menu->_384;
	}
}

extern "C" void fn_8004CD38(MenuPC* menu)
{
	CPad pad(0);
	if (PAD_REPEAT(pad) & 0x10008) {
		menu->mChoice--;
		if (menu->mChoice < 0)
			menu->mChoice = menu->mChoiceCount - 1;
		fn_8001E904(GAME_SOUND, 0x87, 0xFF);
	} else if (PAD_REPEAT(pad) & 0x20004) {
		menu->mChoice++;
		if (menu->mChoice > menu->mChoiceCount - 1)
			menu->mChoice = 0;
		fn_8001E904(GAME_SOUND, 0x87, 0xFF);
	} else if (pad.mPad->mTrigger & 0x100) {
		s32 step = menu->mChoices[menu->mChoice];
		if (step == 7) {
			if (menu->mCanSelect) {
				ResetPCText(menu, 7);
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
			} else {
				fn_8001E904(GAME_SOUND, 0x85, 0xFF);
			}
		} else if (step >= 0) {
			ResetPCText(menu, step);
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
		}
	} else if (pad.mPad->mTrigger & 0x200) {
		ResetPCText(menu, 11);
		*(f32*)((u8*)menu + 0x1A4) = 0.0f;
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
struct MenuPlayerBalances {
	u8 _00[0x9660];
	s32 money;
	s32 scrap;
	void SetMoney(s32 value)
	{
		s32& balance = money;
		balance      = value > 0 ? value : 0;
		if (balance < 0)
			balance = 0;
		if (balance > 99999)
			balance = 99999;
	}
	void SetScrap(s32 value)
	{
		scrap        = value > 0 ? value : 0;
		s32& balance = scrap;
		if (balance < 0)
			balance = 0;
		if (balance > 99999)
			balance = 99999;
	}
};
static inline s32 MenuGetMoney()
{
	return ((MenuPlayerBalances*)GAME_PLAYER_IDS)->money;
}
static inline s32 MenuGetScrap()
{
	return ((MenuPlayerBalances*)GAME_PLAYER_IDS)->scrap;
}
static inline void SetPCBalance(s32& balance, s32 value)
{
	balance = value > 0 ? value : 0;
	if (balance < 0)
		balance = 0;
	if (balance > 99999)
		balance = 99999;
}
static inline void MenuSetMoney(s32 value)
{
	SetPCBalance(((MenuPlayerBalances*)GAME_PLAYER_IDS)->money, value);
}

static inline void DepositPCAmount(MenuPC* menu)
{
	s32 deposit = menu->mAmount;
	s32 money   = MenuGetMoney();
	money -= deposit;
	menu->_384 += deposit;
	((MenuPlayerBalances*)GAME_PLAYER_IDS)->SetMoney(money);
	*(f32*)((u8*)menu + 0x410) = money;
}

static inline void RecyclePCAmount(MenuPC* menu)
{
	s32 scrap = MenuGetScrap() - menu->mAmount;
	((MenuPlayerBalances*)GAME_PLAYER_IDS)->SetScrap(scrap);
	s32 money = MenuGetMoney() + menu->mAmount * *(f32*)((u8*)menu + 0x42C);
	((MenuPlayerBalances*)GAME_PLAYER_IDS)->SetMoney(money);
	*(f32*)((u8*)menu + 0x418) = scrap;
	*(f32*)((u8*)menu + 0x410) = money;
}
// NONMATCHING: transaction balance registers differ.
extern "C" void fn_8004A188(MenuPC* menu)
{
	CPad pad(0);
	s32 digits[7] = { 0, 0, 0, 0, 0, 0, 0 };
	switch (menu->mSubStep) {
	case 0:
		if (PAD_REPEAT(pad) & 0x80002) {
			menu->mMoneyDigit--;
			if (menu->mMoneyDigit < 0)
				menu->mMoneyDigit = 4;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x40001) {
			menu->mMoneyDigit++;
			if (menu->mMoneyDigit > 4)
				menu->mMoneyDigit = 0;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x20004) {
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
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x10008) {
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
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (pad.mPad->mTrigger & 0x200) {
			ResetPCText(menu, 2);
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		} else if (pad.mPad->mTrigger & 0x100) {
			if (menu->mAmount > MenuGetMoney()) {
				menu->mAmount = MenuGetMoney();
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				return;
			}
			if (menu->mAmount == 0) {
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				return;
			}
			menu->mSubStep = 1;
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
		}
		break;
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
		if (pad.mPad->mTrigger & 0x100) {
			switch (menu->mYesNo) {
			case 0: {
				DepositPCAmount(menu);
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				menu->mSubStep = 2;
				break;
			}
			case 1:
				menu->mSubStep = 0;
				fn_8001E904(GAME_SOUND, 0x86, 0xFF);
				break;
			}
		} else if (pad.mPad->mTrigger & 0x200) {
			menu->mSubStep = 0;
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		}
		break;
	case 2:
		if (pad.mPad->mTrigger & 0x300) {
			menu->mSubStep = 0;
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		}
		break;
	}
}

static inline void MenuSetScrap(s32 value)
{
	MenuPlayerBalances* player = (MenuPlayerBalances*)GAME_PLAYER_IDS;
	s32& balance               = player->scrap;
	balance                    = value > 0 ? value : 0;
	if (balance < 0)
		balance = 0;
	if (balance > 99999)
		balance = 99999;
}
// NONMATCHING: transaction balance registers differ.
extern "C" void fn_80049508(MenuPC* menu)
{
	CPad pad(0);
	s32 digits[7] = { 0, 0, 0, 0, 0, 0, 0 };
	switch (menu->mSubStep) {
	case 0:
		menu->mYesNo = -1;
		if (PAD_REPEAT(pad) & 0x80002) {
			menu->mScrapDigit--;
			if (menu->mScrapDigit < 0)
				menu->mScrapDigit = 4;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x40001) {
			menu->mScrapDigit++;
			if (menu->mScrapDigit > 4)
				menu->mScrapDigit = 0;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x20004) {
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
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x10008) {
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
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (pad.mPad->mTrigger & 0x200) {
			ResetPCText(menu, 2);
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		} else if (pad.mPad->mTrigger & 0x100) {
			if (menu->mAmount > MenuGetScrap()) {
				menu->mAmount = MenuGetScrap();
				fn_8001E904(GAME_SOUND, 0x85, 0xFF);
				return;
			}
			if (menu->mAmount == 0) {
				fn_8001E904(GAME_SOUND, 0x85, 0xFF);
				return;
			}
			menu->mSubStep = 1;
			menu->mYesNo   = 0;
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
		}
		break;
	case 1:
		if (PAD_REPEAT(pad) & 0x10008) {
			if (menu->mYesNo > 0) {
				menu->mYesNo--;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		} else if (PAD_REPEAT(pad) & 0x20004) {
			if (menu->mYesNo < 1) {
				menu->mYesNo++;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		}
		if (pad.mPad->mTrigger & 0x100) {
			switch (menu->mYesNo) {
			case 0: {
				RecyclePCAmount(menu);
				menu->mYesNo = 2;
				fn_8001E904(GAME_SOUND, 0x30009, 0xFF);
				menu->mSubStep = 2;
				break;
			}
			case 1:
				menu->mSubStep = 0;
				fn_8001E904(GAME_SOUND, 0x86, 0xFF);
				break;
			}
		} else if (pad.mPad->mTrigger & 0x200) {
			menu->mSubStep = 0;
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		}
		break;
	case 2:
		if (pad.mPad->mTrigger & 0x300) {
			menu->mSubStep = 0;
			ResetPCText(menu, 2);
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		}
		break;
	}
}
static inline Vec MenuObjectVector(XObj* obj, int first)
{
	s32* params = (s32*)obj->_238;
	Vec value   = lbl_80362048[0];
	value.x     = params[first] / 100.0f;
	value.y     = params[first + 1] / 100.0f;
	value.z     = params[first + 2] / 100.0f;
	return value;
}
extern "C" CVec fn_800621E0(XObj* obj)
{
	return CVec(MenuObjectVector(obj, 7));
}
extern "C" CVec fn_800622B8(XObj* obj)
{
	return CVec(MenuObjectVector(obj, 1));
}

extern "C" void fn_8003466C(MenuShopList*);

static inline s32 CountPCShopItems(UnkStagePos* table)
{
	s16 x;
	s16 y;
	int i;
	s32 count;
	count = 0;
	for (i = 0; i < 20; i++) {
		x = table[i].mX;
		y = table[i].mY;
		if (x >= 0 && y >= 0)
			count++;
	}
	return count;
}
static inline s32 PCShopMaxPage(UnkStagePos*& table)
{
	s16 x, y;
	int i;
	s32 pages = 0;
	for (i = 0; i < 20; table++, i++) {
		x = table->mX;
		y = table->mY;
		if (x >= 0 && y >= 0)
			pages++;
	}
	pages -= 8;
	if (pages < 1)
		pages = 1;
	return pages;
}
static inline s32 ResetPCShopPage(MenuPC* menu, UnkStagePos*& table)
{
	s16 x, y;
	int i;
	s32 pages      = 0;
	i              = 0;
	menu->mSubStep = 0;
	for (; i < 20; table++, i++) {
		x = table->mX;
		y = table->mY;
		if (x >= 0 && y >= 0)
			pages++;
	}
	pages -= 8;
	if (pages < 1)
		pages = 1;
	return pages;
}
#define PC_FLOAT(menu, off) (*(f32*)((u8*)(menu) + (off)))
#define PC_INT(menu, off)   (*(s32*)((u8*)(menu) + (off)))
static inline s16 PCShopEntryId(s32 index)
{
	return lbl_80366B70.entries[index].mX;
}
extern "C" void fn_8004AB40(MenuPC* menu, u32 frames)
{
	s32 count;
	CPad pad(0);
	UnkStagePos* table = lbl_80366DB4;
	count              = CountPCShopItems(table);
	fn_80068C8C(&menu->mPanelAlpha, &menu->mPanelTarget, 0.08f * frames);
	switch (menu->mSubStep) {
	case 0:
		if (count > 1) {
			if (menu->mShopSelection == 0) {
				if (PAD_REPEAT(pad) & 0x20004) {
					s32 next = menu->mShopSelection + 1;
					if (next <= 19) {
						menu->mShopSelection = next;
						fn_8001E904(GAME_SOUND, 0x87, 0xFF);
					}
				}
				if (PAD_REPEAT(pad) & 0x10008) {
					CountPCShopItems(table);
					if (lbl_80366B70.first > 0) {
						menu->mScrollTarget = 17.0f;
						menu->mSubStep      = 20;
						break;
					}
				}
			} else if (menu->mShopSelection == 8) {
				if (PAD_REPEAT(pad) & 0x10008) {
					if (--menu->mShopSelection < 0)
						menu->mShopSelection = 19;
					fn_8001E904(GAME_SOUND, 0x87, 0xFF);
				}
				if (PAD_REPEAT(pad) & 0x20004) {
					if (lbl_80366B70.first < PCShopMaxPage(table) - 1) {
						menu->mScrollTarget = -17.0f;
						menu->mSubStep      = 10;
						break;
					}
				}
			} else {
				if (PAD_REPEAT(pad) & 0x10008) {
					if (--menu->mShopSelection < 0)
						menu->mShopSelection = 19;
					fn_8001E904(GAME_SOUND, 0x87, 0xFF);
				}
				if (PAD_REPEAT(pad) & 0x20004) {
					s32 next = menu->mShopSelection + 1;
					if (next <= 19) {
						menu->mShopSelection = next;
						s16 id               = lbl_80366B70.entries[menu->mShopSelection].mX;
						if (id >= 0)
							fn_8001E904(GAME_SOUND, 0x87, 0xFF);
					}
				}
			}
		}
		if (PCShopEntryId(menu->mShopSelection) < 0) {
			do {
				menu->mShopSelection--;
				s16 id = lbl_80366B70.entries[menu->mShopSelection].mX;
				if (id >= 0)
					break;
			} while (menu->mShopSelection != 0);
		}
		if (pad.mPad->mTrigger & 0x100) {
			if (PCShopEntryId(menu->mShopSelection) >= 0 && lbl_80366B70.entries[menu->mShopSelection].mY > 0) {
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				menu->mPanelTarget = 1.0f;
				menu->mSubStep++;
				menu->mYesNo = 0;
				break;
			}
			fn_8001E904(GAME_SOUND, 0x85, 0xFF);
			break;
		}
		if (pad.mPad->mTrigger & 0x200) {
			ResetPCText(menu, 2);
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
			menu->mPanelTarget = 0.0f;
		}
		break;
	case 1:
		if (PAD_REPEAT(pad) & 0x10008) {
			menu->mYesNo--;
			if (menu->mYesNo < 0)
				menu->mYesNo = 1;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x20004) {
			menu->mYesNo++;
			if (menu->mYesNo > 1)
				menu->mYesNo = 0;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (pad.mPad->mTrigger & 0x100) {
			if (menu->mYesNo == 0) {
				UnkStagePos* entry;
				s32 item  = lbl_80366B70.indices[menu->mShopSelection];
				s32 scrap = MenuGetScrap();
				entry     = &lbl_80366DB4[item];
				s16 price = entry->mPrice;
				if (price <= scrap) {
					scrap -= price;
					((MenuPlayerBalances*)GAME_PLAYER_IDS)->SetScrap(scrap);
					menu->mDisplayScrap = scrap;
					menu->mYesNo        = 2;
					fn_8001E904(GAME_SOUND, 0x30009, 0xFF);
					lbl_80367A2C[menu->mCatalogBase + entry->mX] = 1;
					menu->_430                                   = item;
					s16& quantity                                = entry->mY;
					s16 value                                    = quantity;
					s32 remaining                                = value;
					if (value > 0)
						remaining = value - 1;
					quantity = remaining;
					menu->mSubStep++;
					return;
				}
				fn_8001E904(GAME_SOUND, 0x85, 0xFF);
			} else {
				menu->mSubStep     = 0;
				menu->mPanelTarget = 0.0f;
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				menu->_39C = 1.0f;
			}
		} else if (pad.mPad->mTrigger & 0x200) {
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
			menu->mSubStep     = 0;
			menu->mPanelTarget = 0.0f;
		}
		break;
	case 2:
		if (pad.mPad->mTrigger & 0x300) {
			menu->mSubStep     = 0;
			menu->mPanelTarget = 0.0f;
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
			fn_8003466C(&lbl_80366B70);
			ResetPCText(menu, 11);
			PC_FLOAT(menu, 0x1A4) = 0.0f;
			RestorePCHistory(menu);
		}
		break;
	case 10:
		if (menu->mScroll == menu->mScrollTarget) {
			if (lbl_80366B70.first < ResetPCShopPage(menu, table) - 1) {
				lbl_80366B70.first++;
				fn_8003466C(&lbl_80366B70);
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
			menu->mScroll       = 0.0f;
			menu->mScrollTarget = 0.0f;
		}
		break;
	case 20:
		if (menu->mScroll == menu->mScrollTarget) {
			menu->mSubStep = 0;
			CountPCShopItems(table);
			if (lbl_80366B70.first > 0) {
				lbl_80366B70.first--;
				fn_8003466C(&lbl_80366B70);
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
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
	return (XObj*)(*(u8**)(lbl_8025C400 + 0x984C) + index * (*(s32*)(lbl_8025C400 + 0x985C)));
}
static inline s32 PCWorldObjectId(XObj* obj)
{
	s32* ident = (s32*)obj->_244;
	return ident ? *ident : -1;
}
static inline f32 PCCheckedEnd(HSD_AObj* aobj)
{
	if (!aobj)
		__assert("aobj.h", 0xAB, "aobj");
	return *(f32*)((u8*)aobj + 0xC);
}
static inline f32 PCCheckedFrame(HSD_AObj* aobj)
{
	if (!aobj)
		__assert("aobj.h", 0x95, "aobj");
	return *(f32*)((u8*)aobj + 4);
}
static inline f32 PCAnimEnd(XObj* obj)
{
	HSD_AObj* a = GetAObj(obj->mJObj);
	return a ? PCCheckedEnd(a) : 0.0f;
}
static inline f32 PCAnimFrame(XObj* obj)
{
	HSD_AObj* a = GetAObj(obj->mJObj);
	return a ? PCCheckedFrame(a) : 0.0f;
}
typedef void (MenuPC::*PCStepHandler)(u32);
extern "C" PCStepHandler lbl_80212120[];
extern "C" BOOL fn_8004DD1C(MenuPC* menu, u32 frames)
{
	if (fn_800617DC(menu, frames) == 4) {
		PCEase(PC_FLOAT(menu, 0x198), PC_FLOAT(menu, 0x1A0));
		PCEase(menu->_19C, PC_FLOAT(menu, 0x1A4));
		PCSlide(menu->mTextX, menu->mTextTargetX, 6.0f * frames);
		PCSlide(menu->mTextY, menu->mTextTargetY, 6.0f * frames);
		fn_80068C8C(&menu->mScroll, &menu->mScrollTarget, 17.0f);
		PCEaseCall(menu->_40C, menu->mDisplayMoney);
		PCEaseCall(menu->_414, menu->mDisplayScrap);
		PCEaseCall(menu->_424, menu->_428);
		PC_FLOAT(menu, 0x44C) -= 0.008f * frames;
		if (menu->mStep > 1) {
			switch (menu->mStepTime) {
			case 0:
				switch (menu->mStep) {
				case 3:
				case 7:
				case 10:
					menu->mTextTargetX = 140.0f;
					break;
				case 4:
					menu->mTextTargetX = 160.0f;
					break;
				default:
					menu->mTextTargetX = 180.0f;
					break;
				}
				menu->mStepTime++;
				break;
			case 1:
				if (menu->mTextX == menu->mTextTargetX) {
					menu->mTextY       = 129.0f;
					menu->mTextTargetY = 301.0f;
					switch (menu->mStep) {
					case 3:
					case 7:
					case 10:
						menu->mTextY -= 8.0f;
						menu->mTextTargetY -= 8.0f;
						break;
					}
					menu->mStepTime++;
				}
				break;
			case 2:
				if (menu->mTextY == menu->mTextTargetY) {
					switch (menu->mStep) {
					case 3:
					case 7:
					case 10:
						menu->mTextX       = 140.0f;
						menu->mTextTargetX = 469.0f;
						break;
					case 4:
						menu->mTextX       = 160.0f;
						menu->mTextTargetX = 449.0f;
						break;
					default:
						menu->mTextX       = 210.0f;
						menu->mTextTargetX = 429.0f;
						break;
					}
					menu->mStepTime++;
				}
				break;
			case 3:
				if (menu->mTextX == menu->mTextTargetX)
					menu->mStepTime++;
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
				switch (PCWorldObjectId(obj)) {
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
				switch (PCWorldObjectId(obj)) {
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
		(menu->*lbl_80212120[menu->mStep])(frames);
	}
	return menu->IsOpen();
}

typedef void (MenuSave::*SaveStepHandler)(u32);
extern "C" SaveStepHandler lbl_802122B4[];

/**
 * @note Address: 0x8003CE18
 * @note Size: 0x538
 */
// NONMATCHING: the glyph loops take other volatile registers
extern "C" BOOL fn_8003CE18(MenuSave* menu, u32 frames)
{
	if (fn_800617DC(menu, frames) == 4) {
		PCEaseCall(menu->_15C, menu->_164);
		fn_80068C8C(&menu->_264, &menu->_268, 0.08f * frames);
		PCEaseCall(menu->_16C, menu->_174);
		menu->_2A4 -= 0.008f * frames;
		if (menu->mGlyphs.mActive && menu->mGlyphs.mActive > 0) {
			f32 fade = 0.055555556f * frames;
			for (int i = 0; i < menu->mGlyphs.mCount; i++) {
				MenuGlyph* glyph = &menu->mGlyphs.mGlyphs[i];
				if (glyph->mY > 0.0f) {
					if (glyph->mX < 1.0f) {
						if ((glyph->mX += fade) > 1.0f) {
							glyph->mX = 1.0f;
						}
					}
				} else if (glyph->mX > 0.0f) {
					if ((glyph->mX -= fade) < 0.0f) {
						glyph->mX = 0.0f;
					}
				}
			}
			s32 shown = 0;
			f32 spinX = 0.08726646f * frames;
			f32 spinY = 0.05235988f * frames;
			for (int i = 0; i < menu->mGlyphs.mCount; i++) {
				MenuGlyph* glyph = &menu->mGlyphs.mGlyphs[i];
				if (glyph->mX != 0.0f) {
					shown++;
					glyph->mRotX += spinX;
					glyph->mRotY += spinY;
				}
			}
			if (shown == 0) {
				menu->mGlyphs.mActive = FALSE;
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
				menu->mBg[i].mRotX += spinX;
				menu->mBg[i].mRotY += spinY;
			}
		}
		if (menu->mStep != 1 && menu->mStep != 5 && menu->mStep != 9) {
			s32 size;
			s32 sector;
			menu->_260 = FALSE;
			menu->_262 = FALSE;
			switch (fn_80087A1C(GAME_SAVE, menu->mSlot, &size, &sector)) {
			case -2:
				menu->_262 = TRUE;
				break;
			case 0:
				if (sector == 0x2000) {
					menu->_260 = TRUE;
				}
				break;
			case -128:
				break;
			}
			if (!menu->_260) {
				menu->_261 = FALSE;
			}
		}
		if (((UnkSaveData*)GAME_SAVE)->_16A08 != 0) {
			return menu->IsOpen();
		}
		(menu->*lbl_802122B4[menu->mStep])(frames);
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
	f32 width  = fn_801968BC(lbl_8025C400, text, 1, size);
	fn_80196610(lbl_8025C400, color, text, MenuTextCenter(width, 0.5f, 320.0f), y, 0.0f, size, size);
}
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
		if (lbl_8025C400[0x3318] == 6u) {
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
		if (lbl_8025C400[0x3318] == 6u) {
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
		if (lbl_8025C400[0x3318] == 6u) {
			DrawMenuHelpLine(0x8a, y, size, color);
			lines = 1;
		} else {
			DrawMenuHelpLine(0x8a, y, size, color);
			DrawMenuHelpLine(0x8b, (y + size), size, color);
			lines = 2;
		}
		break;
	case 10:
		if (lbl_8025C400[0x3318] == 6u) {
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
		if (lbl_8025C400[0x3318] == 6u) {
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
		if (lbl_8025C400[0x3318] == 6u) {
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

			if (lbl_8025C400[0x3318] == 6u) {
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
