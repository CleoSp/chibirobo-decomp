#ifndef _GAME_INFOSCRN_H
#define _GAME_INFOSCRN_H

#include "game/cgs.h"
#include "game/cscrn.h"
#include "game/infopoint.h"

// the hud over the game, CGame's mInfoScrn. class name from RTTI, its vtable only has the dtor
class InfoScrn {
public:
	CScrnFade mFade;        // _00
	CScrnWipe mWipe;        // _34
	u32 _98;                // _98
	u32 _9C;                // _9C
	u32 _A0;                // _A0
	u32 _A4;                // _A4
	f32 _A8;                // _A8
	f32 _AC;                // _AC
	f32 _B0;                // _B0
	u32 _B4;                // _B4
	u32 _B8;                // _B8
	s32 _BC;                // _BC
	f32 _C0;                // _C0
	f32 _C4;                // _C4
	f32 _C8;                // _C8
	f32 _CC;                // _CC
	f32 _D0;                // _D0
	u32 _D4;                // _D4
	u32 _D8;                // _D8
	f32 _DC;                // _DC
	f32 _E0[4];             // _E0
	u32 _F0[4];             // _F0
	u8 _100[4];             // _100
	f32 _104[4];            // _104
	f32 _114[4];            // _114
	u8 _124;                // _124
	u8 _125[0x128 - 0x125]; // _125
	Vec _128;               // _128
	u8 _134;                // _134
	u8 _135[0x138 - 0x135]; // _135
	f32 _138;               // _138
	f32 _13C;               // _13C
	f32 _140;               // _140
	f32 _144;               // _144
	s32 _148;               // _148
	f32 _14C;               // _14C
	u32 _150;               // _150
	u32 _154;               // _154
	u8 _158;                // _158
	u8 _159[0x164 - 0x159]; // _159
	s32 mItemMax;           // _164, 5, 10 or 15 by the upgrades in chibi robo's items
	s32 mNextItemMax;       // _168
	f32 _16C;               // _16C
	Vec _170;               // _170
	u32 _17C;               // _17C
	u32 _180;               // _180
	u32 _184;               // _184
	s32 _188;               // _188
	f32 _18C;               // _18C
	Vec _190;               // _190
	s32 _19C;               // _19C
	s32 _1A0;               // _1A0
	f32 _1A4;               // _1A4
	Vec _1A8;               // _1A8
	s32 _1B4;               // _1B4
	f32 _1B8;               // _1B8
	f32 mTimer;             // _1BC, frames
	u8 mTimerShown;         // _1C0
	u8 _1C1[0x1C4 - 0x1C1]; // _1C1
	s32 _1C4;               // _1C4
	f32 mScanlineAlpha;     // _1C8
	f32 _1CC;               // _1CC
	u32 _1D0;               // _1D0, CGame's _9108, the game state menus put back
	f32 _1D4;               // _1D4, the battery gauge, 10 a watt
	// _1D8 vtable

	virtual ~InfoScrn() { } // _08 (weak)
};
STATIC_ASSERT(sizeof(InfoScrn) == 0x1DC);

extern "C" {
void fn_800660E0(InfoScrn* scrn, s32 level);
void fn_800660E4(InfoScrn* scrn, s32 exp);
void fn_80066190(InfoScrn* scrn, u8 show, s32 mode);
void fn_800661E4(InfoScrn* scrn, s32 value, u8 show, s32 mode);
void fn_80066250(InfoScrn* scrn, u8 show, s32 mode, s32 flag);
BOOL fn_800665E0(InfoScrn* scrn);
}

#endif
