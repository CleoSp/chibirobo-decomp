#ifndef _GAME_STG_H
#define _GAME_STG_H

#include "game/obja.h"
#include "game/coordinfo.h"
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
	u8 _158[0x510 - 0x158];  // _158
	s32 mStep;               // _510
	s32 mLastStep;           // _514
	s32 mNextStep;           // _518, where a wait step goes after
	s32 _51C;                // _51C
	s32 _520;                // _520, the entry the status shows
	s32 _524;                // _524
	s32 _528;                // _528, the cursor on the map grid
	s32 _52C;                // _52C, the cursor on the other grid
	s32 _530;                // _530, 0 yes, 1 no
	s32 _534;                // _534, how many of the entry
	f32 _538;                // _538
	f32 _53C;                // _53C
	u8 _540[0x554 - 0x540];  // _540
	f32 _554;                // _554
	u8 _558[0x570 - 0x558];  // _558
	f32 _570;                // _570
	u8 _574[0x580 - 0x574];  // _574
	f32 _580;                // _580
	u8 _584[0x598 - 0x584];  // _584
	MenuStatusTab mTabs[4];  // _598
	f32 _5D8;                // _5D8
	f32 _5DC;                // _5DC
	f32 _5E0;                // _5E0
	u8 _5E4[0x5F0 - 0x5E4];  // _5E4
	f32 _5F0;                // _5F0
	u8 _5F4[0x600 - 0x5F4];  // _5F4
	f32 _600;                // _600
	u8 _604[0x610 - 0x604];  // _604
	f32 _610;                // _610
	u8 _614[0x618 - 0x614];  // _614
	MenuStatusTab mTabs2[4]; // _618
	f32 _658;                // _658
	s32 mStepTime;           // _65C
	s32 mWait;               // _660
	s32 _664;                // _664
	u8 _668;                 // _668
	u8 _669[0x66C - 0x669];  // _669
	f32 _66C;                // _66C
	f32 _670;                // _670
	u8 _674;                 // _674
	u8 _675[0x684 - 0x675];  // _675
	s32 _684;                // _684
	u8 _688[0x690 - 0x688];  // _688
	char* _690;              // _690
	f32 _694;                // _694
	MenuGlyphText mGlyphs;   // _698
	u8 _834[0x940 - 0x834];  // _834

	void SetStep(s32 step)
	{
		mLastStep = mStep;
		mStep     = step;
		mStepTime = 0;
	}
};

// one of the shapes turning behind the save menu
struct MenuSaveBg {
	f32 _00;   // _00
	f32 mRotX; // _04
	f32 mRotY; // _08
};

class MenuItem : public Menu {
public:
	s32 _158;               // _158
	s32 _15C;               // _15C
	f32 _160;               // _160
	f32 _164;               // _164
	f32 _168;               // _168
	f32 _16C;               // _16C
	f32 _170;               // _170
	f32 _174;               // _174
	f32 _178;               // _178
	f32 _17C;               // _17C
	u8 _180[0x190 - 0x180]; // _180
	f32 _190;               // _190
	f32 _194;               // _194
	u8 _198[0x4];           // _198
	s32 _19C;               // _19C
	s32 _1A0;               // _1A0
	f32 _1A4;               // _1A4
	char* _1A8;             // _1A8
	char* _1AC;             // _1AC
	u8 _1B0[0x4];           // _1B0
	MenuGlyphText mGlyphs;  // _1B4
	s32 _350;               // _350
	MenuSaveBg mBg[20];     // _354
};

class MenuPC : public Menu {
public:
	s32 mStep;     // _158
	s32 mLastStep; // _15C
	u8 _160[4];
	s32 mSubStep;   // _164
	f32 mStepSpeed; // _168
	s32 mStepTime;  // _16C
	s32 mChoice;    // _170
	u8 _174[0xC];
	s32 mYesNo;          // _180
	s32 mMoneyDigit;     // _184
	s32 mShopSelection;  // _188
	s32 mScrapDigit;     // _18C
	f32 mProgress;       // _190
	f32 mProgressTarget; // _194
	u8 _198[4];
	f32 _19C;
	u8 _1A0[0x18];
	s32 mTextIndex; // _1B8
	u8 _1BC[4];
	f32 mTextX;       // _1C0
	f32 mTextY;       // _1C4
	f32 mTextTargetX; // _1C8
	f32 mTextTargetY; // _1CC
	u16 mText[40];    // _1D0
	u8 mTextEnd;      // _220
	u8 _221[3];
	f32 mGlyphX[40];        // _224
	u8 mGlyphFlags[40];     // _2C4
	char mGlyphText[40][3]; // _2EC
	s32 _364;
	s32 _368;
	s32 _36C;
	s32 _370;
	u8 _374;
	u8 _375[3];
	s32 _378;
	u8 _37C;
	u8 _37D[3];
	s32 mAmount;       // _380
	s32 _384;          // _384
	s32 mHistoryStart; // _388
	s32 mHistoryCount; // _38C
	f32 mPanelAlpha;   // _390
	f32 mPanelTarget;  // _394
	f32 _398;
	f32 _39C;
	u8 _3A0;
	u8 _3A1[3];
	s32 _3A4; // _3A4
	u8 _3A8[0x54];
	f32 mScroll;       // _3FC
	f32 mScrollTarget; // _400
	s32 mCatalogBase;  // _404
	s32 mCatalogCount; // _408
	f32 _40C;
	f32 mDisplayMoney; // _410
	f32 _414;
	f32 mDisplayScrap; // _418
	f32 _41C;
	f32 _420;
	f32 _424;
	f32 _428;
	f32 mScrapRate; // _42C
	s32 _430;       // _430
	u8 mCanSelect;  // _434
	u8 _435[3];
	s32 mChoices[4];  // _438
	s32 mChoiceCount; // _448
	u8 _44C[4];
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

class MenuSave : public Menu {
public:
	f32 _158;                // _158
	f32 _15C;                // _15C
	f32 _160;                // _160
	f32 _164;                // _164
	f32 _168;                // _168
	f32 _16C;                // _16C
	f32 _170;                // _170
	f32 _174;                // _174
	UnkSaveHeader mFiles[3]; // _178
	s32 _250;                // _250
	s32 _254;                // _254
	s32 mResult;             // _258, the card result the error step shows
	s32 _25C;                // _25C
	u8 _260;                 // _260
	u8 _261;                 // _261
	u8 _262;                 // _262
	u8 _263;                 // _263
	f32 _264;                // _264
	f32 _268;                // _268
	s32 mStep;               // _26C
	s32 mLastStep;           // _270
	u8 _274[0x4];            // _274
	s32 mStepTime;           // _278
	f32 _27C;                // _27C
	s32 mTimer;              // _280
	s32 _284;                // _284, the file to work on
	s32 mYesNo;              // _288, 0 yes, 1 no
	s32 _28C;                // _28C
	s32 mSlot;               // _290
	s32 _294;                // _294
	f32 _298;                // _298
	s32 _29C;                // _29C
	s32 _2A0;                // _2A0
	f32 _2A4;                // _2A4
	MenuGlyphText mGlyphs;   // _2A8
	u8 _444[0x4];            // _444
	MenuSaveBg mBg[20];      // _448
	s32 _538;                // _538

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
