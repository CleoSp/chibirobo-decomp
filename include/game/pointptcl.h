#ifndef _GAME_POINTPTCL_H
#define _GAME_POINTPTCL_H

#include "types.h"

struct PointPtclOne {
	s32 mState;   // _00
	u8 _04[0x20]; // _04
};
STATIC_ASSERT(sizeof(PointPtclOne) == 0x24);

// the sparks that fly in when points change, a pool of 10 at lbl_80378300. type 0, 2 and 3
// have their own colors, a negative amount draws them black
struct PointPtcl {
	u8 _00[0x8];             // _00
	s32 mType;               // _08
	s32 mState;              // _0C
	s32 mAmount;             // _10
	u8 _14[0x54];            // _14
	u32 mColor;              // _68
	u32 mColor2;             // _6C
	u8 _70[0x4];             // _70
	s32 _74;                 // _74
	s32 mNum;                // _78
	PointPtclOne mPtcls[40]; // _7C

	u8 IsFree() const { return mState == 0 && _74 == 0; }
};
STATIC_ASSERT(sizeof(PointPtcl) == 0x61C);

#endif
