#ifndef _GAME_XOBJHBOX_H
#define _GAME_XOBJHBOX_H

#include "game/xobj.h"

// the plain object an XObjHBox carries
struct XObjHBoxPart {
	XObj mObj;              // _00
	f32 mHit;               // _980, set to 1 when chibi robo hits the box
	u8 _984[0x988 - 0x984]; // _984
	f32 mRange;             // _988
	s32 mSfx;               // _98C, keyed off on a hit

	XObjHBoxPart();
};

// a box that reads a second object (placement 36) and draws it inside itself
class XObjHBox : public XObj {
public:
	// _00-_980 = XObj
	s32 mLoadState;     // _980
	f32 mRing;          // _984, draws the two rings while above 0
	XObjHBoxPart mPart; // _988

	XObjHBox();
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(XObjHBox) == 0x12F8);
#else
STATIC_ASSERT(sizeof(XObjHBox) == 0x1318);
#endif

#endif
