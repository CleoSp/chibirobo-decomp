#ifndef _GAME_STG_H
#define _GAME_STG_H

#include "game/obja.h"
#include "game/xobjs.h"
#include "game/ctpl.h"

// the debug and pause menus, game/stg.cpp. class names from RTTI and the vtable symbols,
// the layout from the methods and the static objects' destructor chain. offsets are usa

class MenuCursor;

// the frame a menu is drawn in, one tile of the menu texture stretched over a rect
struct MenuWindow {
	BOOL mVisible;       // _00
	f32 mX;              // _04, center
	f32 mY;              // _08
	f32 mWidth;          // _0C
	f32 mHeight;         // _10
	CTexObj mTex;        // _14
	f32 mTransX;         // _114, texture matrix translation
	f32 mTransY;         // _118
	f32 mScaleX;         // _11C, texture matrix scale
	f32 mScaleY;         // _120
	f32 mAlpha;          // _124
	MenuCursor* mCursor; // _128, the hand MenuEx shares between its menus

	// sets the frame to its default spot
	void Init()
	{
		mVisible = FALSE;
		mX       = 320.0f;
		mY       = 224.0f;
		mWidth   = 100.0f;
		mHeight  = 100.0f;
		mAlpha   = 0.0f;
		mCursor  = NULL;
	}
	void SetSize(f32 w, f32 h)
	{
		mWidth  = w;
		mHeight = h;
	}
	void SetPos(f32 x, f32 y)
	{
		mX = x;
		mY = y;
	}
};

// the base of every menu. mState runs 0 closed, 1 opening, 2 to 4 open, 5 closing, 6 closed
class CMenu {
public:
	s32 mState;      // _00
	f32 mAlpha;      // _04
	s32 mSelect;     // _08, the line the cursor is on
	MenuWindow mWin; // _0C
	BOOL mFading;    // _138
	s32 mFadeDir;    // _13C
	f32 mFade;       // _140
	f32 mFadeAlpha;  // _144
	// _148 vtable

	virtual void Init();             // _08
	virtual BOOL Load();             // _0C
	virtual void Reset();            // _10
	virtual BOOL Update(u32 frames); // _14
	virtual BOOL Draw();             // _18
	virtual void Open();             // _1C
	virtual void Close();            // _20
	virtual BOOL IsOpen();           // _24
	virtual void StartFade(s32 dir); // _28
	virtual void StopFade();         // _2C

	bool IsActive() const { return mState >= 1 && mState < 6; }
	inline BOOL DrawMenu();
	void ResetMenu()
	{
		mState     = 0;
		mAlpha     = 0.0f;
		mSelect    = 0;
		mFading    = FALSE;
		mFadeDir   = 0;
		mFade      = 0.0f;
		mFadeAlpha = 1.0f;
	}
	void InitMenu()
	{
		mWin.Init();
		Reset();
	}
};

// a menu of text lines
class Menu : public CMenu {
public:
	f32 mLineHeight; // _14C
	f32 mMaxWidth;   // _150, of the widest line
	f32 mLines;      // _154
};

// a line of a fixed menu table
struct MenuLine {
	char* mText; // _00
	s32 mId;     // _04, -1 for a separator
};

// the pause menu, its last lines are added by whoever opens it
class MenuMain : public Menu {
public:
	s32 mCount;         // _158, fixed lines
	char* mText[13];    // _15C
	f32 mTextWidth[13]; // _190
	s32 mSlot;          // _1C4, memory card slot
	MenuLine mItems[5]; // _1C8

	inline void StopMainFade();
};

// one letter of a line that flies in
struct MenuGlyph {
	const char* mChar; // _00
	f32 mX;            // _04
	f32 mY;            // _08
	f32 mRotX;         // _0C
	f32 mRotY;         // _10
};

// a line of up to 20 letters that fly in one after the other
struct MenuGlyphText {
	BOOL mActive;          // _00
	s32 mCount;            // _04
	u32 mColor;            // _08
	MenuGlyph mGlyphs[20]; // _0C

	inline void Set(const char* text, u32 color);
};

// one of the status menu's tabs
struct MenuStatusTab {
	f32 _00; // _00
	f32 _04; // _04
	f32 mX;  // _08
	f32 _0C; // _0C
};

class MenuStatus : public Menu {
public:
	u8 _158[0x510 - 0x158]; // _158
	s32 mStep;              // _510
	s32 mLastStep;          // _514
	s32 mNextStep;          // _518, where a wait step goes after
	s32 _51C;               // _51C
	s32 _520;               // _520, the entry the status shows
	u8 _524[0x538 - 0x524]; // _524
	f32 _538;               // _538
	f32 _53C;               // _53C
	u8 _540[0x554 - 0x540]; // _540
	f32 _554;               // _554
	u8 _558[0x598 - 0x558]; // _558
	MenuStatusTab mTabs[4]; // _598
	f32 _5D8;               // _5D8
	f32 _5DC;               // _5DC
	u8 _5E0[0x610 - 0x5E0]; // _5E0
	f32 _610;               // _610
	u8 _614[0x65C - 0x614]; // _614
	s32 mStepTime;          // _65C
	s32 mWait;              // _660
	s32 _664;               // _664
	u8 _668;                // _668
	u8 _669[0x66C - 0x669]; // _669
	f32 _66C;               // _66C
	u8 _670[0x674 - 0x670]; // _670
	u8 _674;                // _674
	u8 _675[0x684 - 0x675]; // _675
	s32 _684;               // _684
	u8 _688[0x690 - 0x688]; // _688
	char* _690;             // _690
	f32 _694;               // _694
	MenuGlyphText mGlyphs;  // _698
	u8 _834[0x940 - 0x834]; // _834

	void SetStep(s32 step)
	{
		mLastStep = mStep;
		mStep     = step;
		mStepTime = 0;
	}
};

class MenuItem : public Menu {
public:
	u8 _158[0x444 - 0x158]; // _158
};

class MenuPC : public Menu {
public:
	u8 _158[0x19C - 0x158]; // _158
	f32 _19C;               // _19C
	u8 _1A0[0x384 - 0x1A0]; // _1A0
	s32 _384;               // _384
	u8 _388[0x3A4 - 0x388]; // _388
	s32 _3A4;               // _3A4
	u8 _3A8[0x430 - 0x3A8]; // _3A8
	s32 _430;               // _430
	u8 _434[0x450 - 0x434]; // _434
};

// a save file's header as the save menu shows it
struct UnkSaveHeader {
	u32 _00;      // _00, 1 when the file is there
	s32 _04;      // _04
	f32 _08;      // _08
	f32 _0C;      // _0C
	s32 _10[8];   // _10
	u8 _30[0x10]; // _30
	s32 _40;      // _40
	u8 _44;       // _44

	void Clear()
	{
		_00 = 0;
		_04 = 0;
		_08 = 0.0f;
		_0C = 0.0f;
		for (int i = 0; i < 8; i++) {
			_10[i] = 0;
		}
		_40 = 0;
		_44 = 0;
	}
};

// one line of the save menu's list
struct MenuSaveLine {
	u8 _00[0xC]; // _00
	f32 _0C;     // _0C
	u8 _10[0x4]; // _10
};

class MenuSave : public Menu {
public:
	u8 _158[0xC];             // _158
	f32 _164;                 // _164
	u8 _168[0x4];             // _168
	f32 _16C;                 // _16C
	u8 _170[0x4];             // _170
	f32 _174;                 // _174
	UnkSaveHeader mFiles[3];  // _178
	s32 _250;                 // _250
	s32 _254;                 // _254
	s32 mResult;              // _258, the card result the error step shows
	s32 _25C;                 // _25C
	u8 _260;                  // _260
	u8 _261;                  // _261
	u8 _262;                  // _262
	u8 _263;                  // _263
	f32 _264;                 // _264
	f32 _268;                 // _268
	s32 mStep;                // _26C
	s32 mLastStep;            // _270
	u8 _274[0x4];             // _274
	s32 mStepTime;            // _278
	f32 _27C;                 // _27C
	s32 mTimer;               // _280
	s32 _284;                 // _284, the file to work on
	s32 mYesNo;               // _288, 0 yes, 1 no
	s32 _28C;                 // _28C
	s32 mSlot;                // _290
	u8 _294[0x2B0 - 0x294];   // _294
	MenuSaveLine mLines2[20]; // _2B0
	u8 _440[0x538 - 0x440];   // _440
	s32 _538;                 // _538

	void SetStep(s32 step)
	{
		mLastStep = mStep;
		mStep     = step;
		mStepTime = 0;
	}
	// the card went away, forget its files
	void NoCard()
	{
		mResult = -3;
		SetStep(6);
		_174 = -32.0f;
		for (int i = 0; i < 3; i++) {
			mFiles[i].Clear();
		}
	}
};

// the stage list, two columns of 20
class MenuStageSelect : public Menu {
public:
	s32 mCount;                // _158
	char mText[100][64];       // _15C
	u8 _1A5C[0x1BEC - 0x1A5C]; // _1A5C
};

// the memory card test
class MenuMCTest : public Menu {
public:
	s32 mCount;         // _158, file lines
	char mText[12][64]; // _15C
	f32 mTextWidth[12]; // _45C
	s32 mMode;          // _48C
};

// load, save or delete
class MenuMCTestSub : public Menu {
public:
	char* mText[4];    // _158
	f32 mTextWidth[4]; // _168
};

// the hand MenuEx shares between its menus, an XObj underneath
class MenuCursor : public CXObjIntp {
public:
	u64 mFlags;             // _228
	u8 _230[0x988 - 0x230]; // _230

	virtual void Init(u32 id);                // _90
	virtual void vf94();                      // _94
	virtual void Reset();                     // _98
	virtual BOOL Load(const CoordInfo& info); // _9C
	virtual void vfA0();                      // _A0
	virtual void Update(u32 frames);          // _A4
};

// owns the menus, updates and draws the open ones
class MenuEx {
public:
	BOOL mActive;       // _00, some menu is open
	CMenu* mMenus[8];   // _04
	u8 _24[0x4];        // _24
	MenuCursor mCursor; // _28
	// _9B0 vtable

	virtual ~MenuEx();
};

#endif
