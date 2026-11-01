#ifndef _GAME_INFOPOINT_H
#define _GAME_INFOPOINT_H

#include "types.h"
#include "Version.h"
#include <dolphin/mtx/mtxtypes.h>

// one of the three counters InfoScrn slides in from the left, class name from RTTI
class InfoPoint {
public:
	s32 mIndex;    // _00
	s32 mResource; // _04
	s32 _08;       // _08
	s32 _0C;       // _0C
	s32 _10;       // _10
	f32 _14;       // _14
	s32 _18;       // _18
	f32 _1C;       // _1C
	f32 _20;       // _20
	f32 _24;       // _24
	f32 _28;       // _28
	Vec mPos[6];   // _2C
	u8 mVisible;   // _74
	u8 _75;        // _75
	s32 mState;    // _78
	f32 _7C;       // _7C
	s32 _80;       // _80
	// _84 vtable

	virtual ~InfoPoint() { } // _08 (weak)
};
STATIC_ASSERT(sizeof(InfoPoint) == 0x88);

// the points' ids, the first is lbl_80366FA4's. japan has two fewer before them
#if defined(VERSION_JP)
#define INFOPOINT_ID 0x96
#else
#define INFOPOINT_ID 0x98
#endif

// infoscrn's bss from lbl_80366E58. the points are statics of that tu, so code that
// touches more than one reaches them from the start of the block
struct InfoPointPool {
#if defined(VERSION_JP)
	u8 _00[0x134]; // _00
#else
	u8 _00[0x140]; // _00
#endif
	u8 _140[0xC];      // _140, _134 in japan, a destructor chain link
	InfoPoint mPoint0; // _14C, lbl_80366FA4
	u8 _1D4[0xC];      // _1D4
	InfoPoint mPoint1; // _1E0, lbl_80367038
	u8 _268[0xC];      // _268
	InfoPoint mPoint2; // _274, lbl_803670CC
};

extern "C" {
extern InfoPointPool lbl_80366E58;
extern InfoPoint lbl_80366FA4;
extern InfoPoint lbl_80367038;
extern InfoPoint lbl_803670CC;

void fn_800650AC(InfoPoint* point, s32 index);
}

#endif
