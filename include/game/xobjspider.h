#ifndef _GAME_XOBJSPIDER_H
#define _GAME_XOBJSPIDER_H

#include "game/xobj.h"
#include "game/csound.h"

// a carried model, drawn with the object 0x13C model the spider claimed a slot for
struct CarryInfo {
	CVec mPos;           // _00
	CVec _10;            // _10
	f32 mAngle;          // _20
	u8 _24[0x30 - 0x24]; // _24
	CStatus mFlags;      // _30, 1 drawn
};
STATIC_ASSERT(sizeof(CarryInfo) == 0x38);

// the spiders, offsets are usa, pal has 4 more bytes before _9C8 and 4 after the sfx
class XObjSpider : public XObj {
public:
	typedef void (XObjSpider::*Func)(u32 frames);

	// _00-_980 = XObj
	u8 _980[0x984 - 0x980]; // _980
	f32 mSpeed;             // _984
	u8 _988[0x99C - 0x988]; // _988
	f32 mRadius;            // _99C, shadow size, also kept off the area edges
	u8 _9A0[0x9A4 - 0x9A0]; // _9A0
	s32 mState;             // _9A4
	s32 mPhase;             // _9A8
	s32 mCounter;           // _9AC
	CStatus mFlags;         // _9B0
	s32 mPrevState;         // _9B8
#if defined(VERSION_PAL)
	u8 _9BC[0x9CC - 0x9BC]; // _9BC
#else
	u8 _9BC[0x9C8 - 0x9BC]; // _9BC
#endif
	Vec mHitPos;            // _9C8, where it was hit from
	Vec mHome;              // _9D4
	f32 mHitStrength;       // _9E0
	s32 mCarryMode;         // _9E4, 3 carries it on its side
	s32 mHits;              // _9E8, hits left
	u8 _9EC[0xA4C - 0x9EC]; // _9EC
	Vec mAreaPos;           // _A4C, corner of the area it stays in
	f32 mAreaWidth;         // _A58
	f32 mAreaDepth;         // _A5C
	Vec mStartPos;          // _A60
	u32 _A6C;               // _A6C
	s32 mCommand;           // _A70, from the event scripts, 0 once handled
	s32 mArgs[5];           // _A74
	u32 _A88;               // _A88
	CarryInfo mCarry;       // _A8C
	u32 _AC4;               // _AC4
	CarryInfo _AC8;         // _AC8
	XObj* mSlotObj;         // _B00
	u8 mSlot;               // _B04, 255 for none
	u8 _B05[0xB58 - 0xB05]; // _B05
	s16 mEffectId;          // _B58
	u16 _B5A;               // _B5A
	XObj* mEffect;          // _B5C
	s32 mEffectOn;          // _B60
	u8 _B64[0xBA8 - 0xB64]; // _B64
	AnimSfx mSfx;           // _BA8
#if defined(VERSION_PAL)
	u8 _BB8[0xD7C - 0xBB8]; // _BB8
#else
	u8 _BB8[0xD78 - 0xBB8]; // _BB8
#endif

	void SetState(s32 state)
	{
		mPhase     = 0;
		mCounter   = 0;
		mPrevState = mState;
		mState     = state;
	}

	XObjSpider();
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(XObjSpider) == 0xD68);
#elif defined(VERSION_PAL)
STATIC_ASSERT(sizeof(XObjSpider) == 0xD80);
#else
STATIC_ASSERT(sizeof(XObjSpider) == 0xD78);
#endif

extern "C" {
// draws a CarryInfo with the claimed slot's model
void fn_800EAB58(CarryInfo* carry, u8 slot, bool sideways);
}

#endif
