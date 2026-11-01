#ifndef _GAME_XOBJLETICKER_H
#define _GAME_XOBJLETICKER_H

#include "game/xobj.h"
#include "game/csound.h"

// carries up to two other objects along with it, its camera is CCameraActionLeticker
class XObjLeticker : public XObj {
public:
	typedef void (XObjLeticker::*Func)(u32 frames);

	// _00-_980 = XObj
	Func mFunc;             // _980, null while idle
	s32 mMode;              // _98C, 10 holds chibi robo on joint 9
	s32 mLinked[2];         // _990, object table indices that move with it
	s32 mLinkedCount;       // _998
	s32 _99C;               // _99C
	s32 _9A0;               // _9A0
	s32 _9A4[4];            // _9A4, picked by which way it faces
	s32 _9B4;               // _9B4
	s16 _9B8;               // _9B8
	u8 _9BA[0x9D8 - 0x9BA]; // _9BA
	s16 _9D8;               // _9D8
	u8 _9DA[0x9F8 - 0x9DA]; // _9DA
	f32 _9F8;               // _9F8, _99C in tenths
	f32 _9FC;               // _9FC, _9A0 in tenths
	f32 mHeight;            // _A00, the stacked joint is drawn every 24 under it
	u8 _A04[0xA28 - 0xA04]; // _A04
	f32 mHeading;           // _A28
	u8 mPtcl;               // _A2C, particle 165 is running
	u8 _A2D;                // _A2D
	u8 _A2E[0xA30 - 0xA2E]; // _A2E
	s32 _A30;               // _A30
	u8 _A34[0xA38 - 0xA34]; // _A34
	s32 mState;             // _A38, 1 draws the stack, 2 and 3 skip the camera pop
	AnimSfx mSfx;           // _A3C
	u8 _A4C[0xA50 - 0xA4C]; // _A4C

	XObjLeticker();
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(XObjLeticker) == 0xA40);
#else
STATIC_ASSERT(sizeof(XObjLeticker) == 0xA50);
#endif

#endif
