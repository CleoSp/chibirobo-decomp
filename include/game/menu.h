#ifndef _GAME_MENU_H
#define _GAME_MENU_H

#include "types.h"
#include "Version.h"
#include "game/cgs.h"
#include "game/cgame.h"
#include "game/ut.h"
#include "game/title.h"
#include "game/menusave.h"

// the 2d menus. class names from RTTI, the menu globals' __sinit builds one of each
// (lbl_80361F68 on) and stores CMenu's, Menu's and then their own vtable

// a menu window's scrolling texture, name is ours
struct MenuTex {
	CTexObj mTex; // _00
	f32 mX;       // _100, scrolls while the window is open
	f32 mY;       // _104
	f32 mScaleX;  // _108
	f32 mScaleY;  // _10C
};
STATIC_ASSERT(sizeof(MenuTex) == 0x110);

// the frame a menu draws behind itself, name is ours
struct MenuWindow {
	BOOL mOpen;   // _00
	f32 mX;       // _04
	f32 mY;       // _08
	f32 mWidth;   // _0C
	f32 mHeight;  // _10
	MenuTex mTex; // _14
};
STATIC_ASSERT(sizeof(MenuWindow) == 0x124);

extern "C" {
// draws a menu's window
void fn_800623F8(MenuWindow* window);
}

class CMenu {
public:
	s32 mState;         // _00, 1 opening, 4 open, 5 closing, 6 closed
	f32 mAlpha;         // _04
	s32 mSelection;     // _08
	MenuWindow mWindow; // _0C
	f32 mDrawAlpha;     // _130
	XObj* mCursor;      // _134, MenuEx's cursor
	BOOL mSuspended;    // _138, another menu is on top
	BOOL mSuspendFade;  // _13C, fade out while suspended
	f32 mFade;          // _140
	f32 mSuspendAlpha;  // _144
	// _148 vtable

	~CMenu() { }

	virtual void Init() // _08 (weak)
	{
		mWindow.mOpen   = FALSE;
		mWindow.mX      = 320.0f;
		mWindow.mY      = 224.0f;
		mWindow.mWidth  = 100.0f;
		mWindow.mHeight = 100.0f;
		mDrawAlpha      = 0.0f;
		mCursor         = NULL;
		Reset();
	}
	virtual BOOL LoadTexture(); // _0C (weak)
	virtual void Reset()        // _10 (weak)
	{
		mState        = 0;
		mAlpha        = 0.0f;
		mSelection    = 0;
		mSuspended    = FALSE;
		mSuspendFade  = FALSE;
		mFade         = 0.0f;
		mSuspendAlpha = 1.0f;
	}
	virtual BOOL Update(u32 frames); // _14 (weak), returns mState
	virtual BOOL Draw()              // _18 (weak)
	{
		if (!IsOpen())
			return FALSE;
		fn_80199AB0((CGs*)lbl_8025C400.mGs, 0, 0);
		fn_8019B56C(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 4, 5, 15);
		fn_8019B464(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 0, 3, 0);
		fn_800623F8(&mWindow);
		return TRUE;
	}
	virtual void Open() // _1C (weak)
	{
		mState = 1;
		Resume();
		mWindow.mOpen = TRUE;
	}
	virtual void Close() // _20 (weak)
	{
		mState = 5;
		Resume();
		mWindow.mOpen = FALSE;
	}
	virtual BOOL IsOpen() // _24 (weak)
	{
		bool open = false;
		if (mState >= 1 && mState < 6)
			open = true;
		return open;
	}
	virtual void Suspend(BOOL fade) // _28 (weak)
	{
		mSuspended   = TRUE;
		mSuspendFade = fade;
	}
	virtual void Resume() // _2C (weak)
	{
		mSuspended   = FALSE;
		mSuspendFade = FALSE;
	}
};
STATIC_ASSERT(sizeof(CMenu) == 0x14C);

class Menu : public CMenu {
public:
	~Menu() { }
};

// one of MenuMain's rows
struct MenuMainItem {
	char* mText; // _00
	s32 mAction; // _04
};

// the in-game menu, lbl_803622B4
class MenuMain : public Menu {
public:
	~MenuMain() { }

	virtual void Init();   // _08
	virtual void Reset();  // _10
	virtual void Open();   // _1C
	virtual void Close();  // _20
	virtual void Resume(); // _2C, puts the game's pause flag back

#if defined(VERSION_JP)
	enum { ROW_MAX = 18 };
#else
	enum { ROW_MAX = 13 };
#endif

	f32 mTextSize;              // _14C
	f32 mTextWidthMax;          // _150
	f32 mRowCount;              // _154
	s32 mBaseRows;              // _158
	char* mText[ROW_MAX];       // _15C
	f32 mTextWidth[ROW_MAX];    // _190, _1A4 in japan
	s32 mChannel;               // _1C4, _1EC in japan, the memory card slot the save menus use
	MenuMainItem mExtraRows[5]; // _1C8, _1F0 in japan, rows other menus add under the line
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(MenuMain) == 0x218);
#else
STATIC_ASSERT(sizeof(MenuMain) == 0x1F0);
#endif

// the status screen
class MenuStatus : public Menu {
public:
	~MenuStatus() { }

	virtual void Init();             // _08
	virtual void Reset();            // _10
	virtual BOOL Update(u32 frames); // _14
	virtual BOOL Draw();             // _18
	virtual void Open();             // _1C

	u8 _14C[0x510 - 0x14C]; // _14C
	s32 mMode;              // _510
	s32 mPrevMode;          // _514
	s32 mNextMode;          // _518
	u8 _51C[0x520 - 0x51C]; // _51C
	s32 mItem;              // _520, the one shown in the middle
	u8 _524[0x538 - 0x524]; // _524
	f32 mOffsetX;           // _538
	f32 mOffsetY;           // _53C
	u8 _540[0x554 - 0x540]; // _540
	f32 mSlide;             // _554
	u8 _558[0x5D8 - 0x558]; // _558
	f32 mX;                 // _5D8
	f32 mY;                 // _5DC
	u8 _5E0[0x65C - 0x5E0]; // _5E0
	s32 mPhase;             // _65C
	s32 mDelay;             // _660
	s32 mSavedBgm;          // _664, put back on the way out
	u8 mSavedBgmVolume;     // _668
	u8 _669[0x66C - 0x669]; // _669
	f32 _66C;               // _66C
	u8 _670[0x694 - 0x670]; // _670
	f32 _694;               // _694
	u8 _698[0x940 - 0x698]; // _698
};
#if !defined(VERSION_JP)
STATIC_ASSERT(sizeof(MenuStatus) == 0x940);
#endif

// one of the status screen's item slots
struct MenuStatusItem {
	s16 mItem;       // _00
	s16 mCount;      // _02
	CStatus mStatus; // _04
};
STATIC_ASSERT(sizeof(MenuStatusItem) == 0xC);

// lbl_80365E08, right after the menus
struct MenuStatusItems {
	s32 mPage;                  // _00, in fours
	MenuStatusItem mItems[256]; // _04, the eight on screen
	MenuStatusItem mPrev[4];    // _C04, the four before them
	MenuStatusItem mNext[4];    // _C34, the four after
};

class MenuItem : public Menu {
public:
	~MenuItem() { }

	virtual void Init();             // _08
	virtual BOOL Update(u32 frames); // _14
	virtual BOOL Draw();             // _18
	virtual void Open();             // _1C

#if defined(VERSION_PAL)
	u8 _14C[0x45C - 0x14C]; // _14C
#else
	u8 _14C[0x444 - 0x14C]; // _14C
#endif
};

// the chibi-pc shop
class MenuPC : public Menu {
public:
	~MenuPC() { }

	virtual void Init();             // _08
	virtual BOOL Update(u32 frames); // _14
	virtual BOOL Draw();             // _18
	virtual void Open();             // _1C

	inline void ResetText(); // game/menupc.h

	u8 _14C[0x158 - 0x14C]; // _14C
	s32 mMode;              // _158
	s32 mPrevMode;          // _15C
	u32 _160;               // _160
	s32 mPhase;             // _164
	f32 mSpeed;             // _168
	s32 _16C;               // _16C
	u8 _170[0x190 - 0x170]; // _170
	f32 mProgress;          // _190
	f32 mTargetProgress;    // _194
	u8 _198[0x19C - 0x198]; // _198
	f32 mFade;              // _19C
	u8 _1A0[0x1B8 - 0x1A0]; // _1A0
	s32 mCharIndex;         // _1B8
	u32 _1BC;               // _1BC
	f32 mTextX;             // _1C0
	f32 mTextY;             // _1C4
	f32 mTargetX;           // _1C8
	f32 mTargetY;           // _1CC
	u16 mChars[40];         // _1D0, two byte sjis
	u8 mTerminator;         // _220
	u8 _221[0x224 - 0x221]; // _221
	f32 mCharX[40];         // _224
	u8 mCharFlags[40];      // _2C4
	u8 mCharCopies[40][3];  // _2EC
	u32 _364;               // _364
	u32 _368;               // _368
	u32 _36C;               // _36C
	u32 _370;               // _370
	u8 _374;                // _374
	u8 _375[0x378 - 0x375]; // _375
	u32 _378;               // _378
	u8 _37C;                // _37C
	u8 _37D[0x384 - 0x37D]; // _37D
	u32 _384;               // _384
	u8 _388[0x3A4 - 0x388]; // _388
	u32 _3A4;               // _3A4
	u8 _3A8[0x430 - 0x3A8]; // _3A8
	u32 mExitState;         // _430, the game state it hands back
	u8 _434[0x450 - 0x434]; // _434
};
#if !defined(VERSION_JP)
STATIC_ASSERT(sizeof(MenuPC) == 0x450);
#endif

// a letter of the save menu's title, they fly in one by one
struct MenuTitleChar {
	const char* mText; // _00
	f32 mDelay;        // _04
	f32 mTime;         // _08
	f32 mAngle0;       // _0C
	f32 mAngle1;       // _10
};
STATIC_ASSERT(sizeof(MenuTitleChar) == 0x14);

// the save menu, its state handlers sit in the table at 0x802121B0
class MenuSave : public Menu {
public:
	~MenuSave() { }

	virtual void Init();             // _08
	virtual void Reset();            // _10
	virtual BOOL Update(u32 frames); // _14
	virtual BOOL Draw();             // _18
	virtual void Open();             // _1C

	u8 _14C[0x158 - 0x14C];   // _14C
	f32 _158;                 // _158
	f32 _15C;                 // _15C
	f32 _160;                 // _160
	f32 _164;                 // _164
	f32 _168;                 // _168
	f32 _16C;                 // _16C
	f32 _170;                 // _170
	f32 mOffset;              // _174
	MenuSaveSlot mSlots[3];   // _178, the three save files
	s32 _250;                 // _250, the card's _16A04
	s32 _254;                 // _254, the card's _16A00
	s32 mResult;              // _258, the card result
	s32 mMessage;             // _25C
	u8 mReady;                // _260
	u8 mShowMessage;          // _261
	u8 _262;                  // _262, checked instead of mReady after a -2
	u8 mSaved;                // _263
	f32 _264;                 // _264
	f32 mAlpha;               // _268
	s32 mMode;                // _26C
	s32 mPrevMode;            // _270
	u32 _274;                 // _274
	s32 mPhase;               // _278
	f32 mTime;                // _27C
	s32 mDelay;               // _280
	s32 mFile;                // _284
	s32 mChoice;              // _288
	s32 mLastFile;            // _28C, -1 until something's saved
	s32 mChannel;             // _290
	u8 _294[0x2A4 - 0x294];   // _294
	f32 _2A4;                 // _2A4
	s32 mTitleState;          // _2A8
	s32 mTitleCount;          // _2AC
	u32 mTitleColor;          // _2B0
	MenuTitleChar mTitle[20]; // _2B4
#if !defined(VERSION_JP)
	u8 _444[0x538 - 0x444]; // _444
	BOOL _538;              // _538
#endif

	// todo: cleanup
	void SetMode(s32 mode)
	{
		mPrevMode = mMode;
		mMode     = mode;
		mPhase    = 0;
	}

	// todo: cleanup
	void ResetResults()
	{
		mResult = -3;
		SetMode(6);
		mOffset = -32.0f;
		for (s32 i = 0; i < 3; ++i) {
			ClearSaveSlot(&mSlots[i]);
		}
	}
};
#if !defined(VERSION_JP)
STATIC_ASSERT(sizeof(MenuSave) == 0x53C);
#endif

// the debug stage select
class MenuStageSelect : public Menu {
public:
	~MenuStageSelect() { }

	u8 _14C[0x158 - 0x14C]; // _14C
	s32 mCount;             // _158
	char mLabels[100][64];  // _15C, a leading 1 marks a stage that can't be picked
	f32 mLabelWidth[100];   // _1A5C
};

// the memory card test menu, lbl_803657E8
class MenuMCTest : public Menu {
public:
	~MenuMCTest() { }

	f32 mFontSize;      // _14C
	f32 mMaxWidth;      // _150
	f32 mRowCount;      // _154
	s32 mFileCount;     // _158
	char mText[12][64]; // _15C
	f32 mTextWidth[12]; // _45C
	s32 mOperation;     // _48C, the card job running, odd while busy
};

// its popup with the four card actions
class MenuMCTestSub : public Menu {
public:
	~MenuMCTestSub() { }

	f32 mFontSize;        // _14C
	f32 mMaxWidth;        // _150
	f32 mRowCount;        // _154
	const char* mText[4]; // _158
	f32 mTextWidth[4];    // _168
};

// a popup line and what picking it does
struct MenuMCTestAction {
	const char* mText; // _00
	s32 mAction;       // _04
};

extern "C" {
// CMenu's own update, the overrides call it first
BOOL fn_800617DC(CMenu* menu, u32 frames);
// the card test popup's lines
extern MenuMCTestAction lbl_80212450[4];
// a label starting with this can't be picked
extern char lbl_8065C440[1];
// the card test's file name formats
extern const char lbl_8020144C[];
extern const char lbl_80201458[];
// the stages the select lists in the full game, ends at -1
extern s32 lbl_8021240C[];
void fn_80033A68(MenuStageSelect* menu);
}

// the hand over a menu, MenuEx's
class MenuCursor : public Cursor {
public:
	MenuCursor();          // fn_8001B3CC
	virtual ~MenuCursor(); // _08, fn_80008CD8
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(MenuCursor) == 0x978);
#else
STATIC_ASSERT(sizeof(MenuCursor) == 0x988);
#endif

// not a CMenu, it only holds the cursor
class MenuEx {
public:
	BOOL mActive; // _00, a menu is open
#if defined(VERSION_JP)
	CMenu* mMenus[13]; // _04
#else
	CMenu* mMenus[8];    // _04, evt indexes these by id
	u8 _24[0x28 - 0x24]; // _24
#endif
	MenuCursor mCursor; // _28, _38 in japan
	// _9B0 vtable, then 4 bytes of padding

	MenuEx() { }          // fn_8001B38C
	virtual ~MenuEx() { } // _08, fn_80008C74
};
STATIC_ASSERT(sizeof(MenuEx) == 0x9B8);

extern "C" {
extern MenuMain lbl_803622B4;
extern MenuStatusItems lbl_80365E08;
}

#endif
