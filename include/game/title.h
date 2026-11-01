#ifndef _GAME_TITLE_H
#define _GAME_TITLE_H

#include "Version.h"
#include "game/xobj.h"
#include "game/xobjdemo.h"
#include "game/ctpl.h"

// the menus' pointer, an XObj with a little on top. class name from RTTI
class Cursor : public XObj {
public:
	// _00-_980 = XObj
	f32 mAngle;      // _980, _970 in japan
	f32 mAngleSpeed; // _984, it spins, the scale setters flip y for screen space

	virtual ~Cursor(); // _08, fn_8000871C
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(Cursor) == 0x978);
#else
STATIC_ASSERT(sizeof(Cursor) == 0x988);
#endif

// a speck floating over the title screen
struct TitleDust {
	u32 mState;  // _00
	u32 mTimer;  // _04
	f32 mX;      // _08
	f32 mY;      // _0C
	f32 mSpeedX; // _10
	f32 mSpeedY; // _14
};
STATIC_ASSERT(sizeof(TitleDust) == 0x18);

// one of the title's texts
struct TitleLabel {
	f32 mAlpha;   // _00
	u8 _04[0x10]; // _04
};
STATIC_ASSERT(sizeof(TitleLabel) == 0x14);

// the title screen, CGame keeps it at _4930. class name from RTTI
class Title {
public:
	s32 mState;                // _00, CGame's FrameInterval runs at 60 while it's 1 to 22
	s32 mMode;                 // _04
	u32 _08;                   // _08
	s32 mTimeout;              // _0C
	f32 _10;                   // _10
	f32 _14;                   // _14
	f32 _18;                   // _18
	u32 _1C;                   // _1C
	u32 _20;                   // _20
	u32 _24;                   // _24
	Cursor mCursor;            // _28
	XObjDemo mDemo;            // _9B0, _9A0 in japan
	CTpl mTpl;                 // _2FB8, _2F98 in japan
	CTexObj mTex[6];           // _31A8
	BOOL _37A8;                // _37A8, a save file was read
	f32 _37AC;                 // _37AC
	f32 _37B0;                 // _37B0
	f32 _37B4;                 // _37B4
	f32 _37B8;                 // _37B8
	f32 _37BC;                 // _37BC
	f32 _37C0;                 // _37C0
	f32 _37C4;                 // _37C4
	f32 _37C8;                 // _37C8
	f32 mOffset;               // _37CC
	s32 _37D0;                 // _37D0, the card's _16A04
	s32 _37D4;                 // _37D4, the card's _16A00
	s32 mResult;               // _37D8, the card result
	u32 _37DC;                 // _37DC
	u8 mReady;                 // _37E0
	u8 mShowMessage;           // _37E1
	u8 _37E2;                  // _37E2
	u8 _37E3;                  // _37E3
	f32 _37E4;                 // _37E4
	f32 mAlpha;                // _37E8
	s32 mPrevMode;             // _37EC
	s32 mPhase;                // _37F0
	s32 mDelay;                // _37F4
	s32 mFile;                 // _37F8, the newest save starts picked
	s32 mChoice;               // _37FC
	u32 _3800;                 // _3800
	s32 mChannel;              // _3804
	u8 _3808;                  // _3808
	u8 _3809;                  // _3809
	u8 _380A[0x380C - 0x380A]; // _380A
	u32 _380C;                 // _380C
	f32 _3810;                 // _3810
	f32 _3814;                 // _3814
	u8 _3818;                  // _3818
	u8 _3819;                  // _3819
	u8 _381A[0x3824 - 0x381A]; // _381A
	TitleDust mDust[16];       // _3824
	u8 _39A4[0x39C8 - 0x39A4]; // _39A4
	TitleLabel mLabels[20];    // _39C8, faded out together when a card job finishes
	u8 _3B58[0x3C44 - 0x3B58]; // _3B58
#if !defined(VERSION_JP)
	u32 _3C44; // _3C44
#endif
	// _3C48 vtable, _3C24 in japan

	virtual ~Title(); // _08, fn_80008650

	// todo: cleanup
	void SetMode(s32 mode)
	{
		mPrevMode = mMode;
		mMode     = mode;
		mPhase    = 0;
	}
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(Title) == 0x3C28);
#else
STATIC_ASSERT(sizeof(Title) == 0x3C50);
#endif

#endif
