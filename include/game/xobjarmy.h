#ifndef _GAME_XOBJARMY_H
#define _GAME_XOBJARMY_H

#include "game/xobj.h"
#include "game/csound.h"

// one of the army's shots, it flies until mLife runs out or it hits chibi robo
struct XObjArmyShot {
	s32 mActive; // _00
	u32 mLife;   // _04
	CVec mPos;   // _08
	CVec mVel;   // _18
	f32 mSpeed;  // _28
	s32 mPhase;  // _2C, 0 flying, 1 bouncing off, 2 lying on the floor

	void Move()
	{
		mPos.x += mSpeed * mVel.x;
		mPos.y += mSpeed * mVel.y;
		mPos.z += mSpeed * mVel.z;
	}
};
STATIC_ASSERT(sizeof(XObjArmyShot) == 0x30);

class XObjArmy : public XObj {
public:
	typedef void (XObjArmy::*Func)(u32 frames);

	// _00-_980 = XObj
	Func mFunc;             // _980
	s32 mStep;              // _98C
	s32 mTimer;             // _990
	f32 _994;               // _994
	u8 _998[0x9A0 - 0x998]; // _998
	CStatus mFlags;         // _9A0, 1 ignores hits, 0x80 shoots quietly, 0x100 plays its sounds at full volume
	CVec _9A8;              // _9A8
	s32 mNext;              // _9B8, what to do once the wait is over
	XObjArmyShot mShots[5]; // _9BC
	AnimSfx mSfx;           // _AAC
	u8 _ABC[0xAC0 - 0xABC]; // _ABC

	XObjArmy();
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(XObjArmy) == 0xAB0);
#else
STATIC_ASSERT(sizeof(XObjArmy) == 0xAC0);
#endif

extern "C" {
// the army's ptmf constants
extern XObjArmy::Func lbl_8022A6A4;
extern XObjArmy::Func lbl_8022A6B0[5];
}

#endif
