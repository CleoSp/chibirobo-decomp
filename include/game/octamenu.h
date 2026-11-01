#ifndef _GAME_OCTAMENU_H
#define _GAME_OCTAMENU_H

#include "game/xobj.h"
#include "game/cptcl.h"

// chibi robo's ring menus, class names from RTTI. AtcMenu picks the attachment, DrsMenu the
// costume, OXMenu is the yes/no pick with the two spinning marks. all of them live in XObjChr

class XObjChr;

// a joint of the menu model bobbing up and down, with its sparkles
struct OctaMenuBob {
	CJObj* mObj;          // _00
	u8 _04[0x30 - 0x4];   // _04
	f32 mPhase;           // _30
	f32 mHeight;          // _34
	u32 _38;              // _38
	u8 _3C[0xC0 - 0x3C];  // _3C
	f32 _C0;              // _C0
	u8 _C4[0xD8 - 0xC4];  // _C4
	f32 _D8;              // _D8
	u8 _DC[0xE4 - 0xDC];  // _DC
	CPtcl mPtcl;          // _E4
	u8 _F8[0xD78 - 0xF8]; // _F8
	HSD_Generator* mGen;  // _D78
	HSD_Generator* mGen2; // _D7C
};
STATIC_ASSERT(sizeof(OctaMenuBob) == 0xD80);

// one item on the ring
struct OctaMenuItem {
	s32 mType;           // _00, attachment or costume id
	s32 mAvailable;      // _04
	f32 _08;             // _08, spin, the pulse phase while it's selected
	f32 _0C;             // _0C
	f32 _10;             // _10
	u8 _14[0x28 - 0x14]; // _14
};
STATIC_ASSERT(sizeof(OctaMenuItem) == 0x28);

// one of the sparks around the ring
struct OctaMenuSpark {
	u32 _00; // _00
	f32 _04; // _04
	f32 _08; // _08
};
STATIC_ASSERT(sizeof(OctaMenuSpark) == 0xC);

class OctaMenu {
public:
	s32 mOpen;              // _00, above 0 while open, below 0 while it closes
	s32 _04;                // _04
	u32 _08;                // _08
	f32 _0C;                // _0C, glow, 1 when opened from the pad
	u32 _10;                // _10
	OctaMenuItem mItems[8]; // _14
	u32 _154;               // _154
	f32 _158;               // _158, ring center on screen
	f32 _15C;               // _15C
	u8 _160[0x168 - 0x160]; // _160
	XObjChr* mChr;          // _168
	f32 _16C;               // _16C, mChr's heading when it opened
	s32 _170;               // _170
#if defined(VERSION_JP)
	OctaMenuSpark _174[20]; // _174
#else
	OctaMenuSpark _174[22]; // _174
#endif
	// _27C vtable, _264 in japan

	OctaMenu() { mChr = NULL; }

	virtual ~OctaMenu() { }       // _08 (weak)
	virtual void Open(s32 item);  // _0C
	virtual void Select(s32 dir); // _10
	virtual void vf14();          // _14
	virtual void vf18();          // _18
	virtual void vf1C();          // _1C
	virtual void vf20();          // _20
	virtual void vf24();          // _24
	virtual void vf28();          // _28
	virtual void vf2C();          // _2C

	s32 _280;               // _280, _268 in japan
	u32 _284;               // _284
	s32 _288;               // _288, item to start on
	s32 _28C;               // _28C, queued start item, -1 for none
	s32 mFocus;             // _290, set on the menu that has the cursor
	u8 _294[0x298 - 0x294]; // _294
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(OctaMenu) == 0x280);
#else
STATIC_ASSERT(sizeof(OctaMenu) == 0x298);
#endif

class AtcMenu : public OctaMenu {
public:
	virtual ~AtcMenu() { }       // _08 (weak)
	virtual void Open(s32 item); // _0C
	virtual void vf18();         // _18
	virtual void vf1C();         // _1C
	virtual void vf20();         // _20
	virtual void vf24();         // _24
	virtual void vf28();         // _28
	virtual void vf2C();         // _2C
};

class DrsMenu : public OctaMenu {
public:
	virtual ~DrsMenu();          // _08
	virtual void Open(s32 item); // _0C
	virtual void vf18();         // _18
	virtual void vf1C();         // _1C
	virtual void vf20();         // _20
	virtual void vf24();         // _24
	virtual void vf28();         // _28
	virtual void vf2C();         // _2C
};

// one of OXMenu's marks. usa and europe give each mark its own object
struct OXMenuMark {
	f32 mSpin;  // _00
	f32 mTilt;  // _04
	f32 mScale; // _08
#if !defined(VERSION_JP)
	u32 _0C;   // _0C
	XObj mObj; // _10
#endif

	OXMenuMark();
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(OXMenuMark) == 0xC);
#else
STATIC_ASSERT(sizeof(OXMenuMark) == 0x990);
#endif

// offsets are usa, japan has no _00 or _14 and its marks are only 0xC each
class OXMenu {
public:
#if !defined(VERSION_JP)
	u32 _00; // _00
#endif
	BOOL mStarted; // _04
	s32 _08;       // _08
	s32 _0C;       // _0C
	f32 mFade;     // _10
#if !defined(VERSION_JP)
	u32 _14; // _14
#endif
	OXMenuMark mMark[2]; // _18
	XObj* mChr;          // _1338, _28 in japan
	f32 mStartRot;       // _133C, the chr's y rotation when it opened
	u32 _1340;           // _1340
	// _1344 vtable, _34 in japan

	OXMenu() { mChr = NULL; }

	virtual ~OXMenu(); // _08 (weak)
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(OXMenu) == 0x38);
#else
STATIC_ASSERT(sizeof(OXMenu) == 0x1348);
#endif

#endif
