#ifndef _GAME_STG_SHARED_H
#define _GAME_STG_SHARED_H

#include "game/hsd_jobj.h"
#include "game/cparticle.h"
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

#endif
