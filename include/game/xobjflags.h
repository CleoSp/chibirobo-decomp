#ifndef _GAME_XOBJFLAGS_H
#define _GAME_XOBJFLAGS_H

#include "game/xobj.h"

// objects that remember being collected in a CEvtFlags, a bit per placement id
class XObjFlags : public XObj {
public:
	// _00-_980 = XObj
	CEvtFlags* mFlags; // _980

	XObjFlags();

	virtual void vf94();        // _94 (weak), clears every flag
	virtual s32 GetFlagIndex(); // _148, the bit in mFlags, 0 and below have none

	BOOL IsFlagSet()
	{
		if (GetFlagIndex() > 0) {
			u32 index = GetFlagIndex();
			return mFlags->mFlags[index >> 5] & (1 << (index & 31));
		}
		return FALSE;
	}
	void SetFlag()
	{
		u32 index = GetFlagIndex();
		mFlags->mFlags[index / 32] |= 1 << (index & 31);
	}
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(XObjFlags) == 0x978);
#else
STATIC_ASSERT(sizeof(XObjFlags) == 0x988);
#endif

// the dust piles, swept up by the vacuum or blown off by chibi robo
class XObjDust : public XObjFlags {
public:
	// _00-_988 = XObjFlags
	HSD_MatAnimJoint* mMatAnim; // _988
	s32 mAnimId2;               // _98C, anim it was placed with
	s32 mAnimCount;             // _990
	f32 mBaseY;                 // _994
	f32 mPhase;                 // _998, bobbing
	f32 mPhaseStep;             // _99C
	f32 mBouncePhase;           // _9A0
	f32 mBounceHeight;          // _9A4
	f32 mHeading;               // _9A8, NAN until something pushes it
	f32 mSpeed;                 // _9AC
	f32 mFade;                  // _9B0
	Vec mStartScale;            // _9B4
	Vec mFrom;                  // _9C0
	Vec mTo;                    // _9CC
	s16 mEventId;               // _9D8
	s16 mEventArg;              // _9DA
	u8 _9DC[0x9E0 - 0x9DC];     // _9DC

	XObjDust();
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(XObjDust) == 0x9D0);
#else
STATIC_ASSERT(sizeof(XObjDust) == 0x9E0);
#endif

// coins, they drop out of things and bounce
class XObjCoin : public XObjFlags {
public:
	// _00-_988 = XObjFlags
	f32 mSpin;              // _988
	f32 _98C;               // _98C
	s32 mState;             // _990, 1 while falling
	u32 mParam0;            // _994
	u32 mParam1;            // _998
	u32 mFallFrames;        // _99C
	u32 mParams[8];         // _9A0
	u8 _9C0[0x9E0 - 0x9C0]; // _9C0
	f32 mHeading;           // _9E0
	f32 mFallSpeed;         // _9E4
	u8 mBouncing;           // _9E8, spiders bounce it out

	XObjCoin();
};
#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(XObjCoin) == 0x9E0);
#else
STATIC_ASSERT(sizeof(XObjCoin) == 0x9F0);
#endif

// items lying around, nothing past XObjFlags
class XObjItem : public XObjFlags {
public:
	// _00-_988 = XObjFlags

	XObjItem();
};

extern "C" {
// the flag sets the three kinds keep
extern CEvtFlags lbl_8037DDBC; // items
extern CEvtFlags lbl_8037DFCC; // coins
extern CEvtFlags lbl_8037E1DC; // dust
}

#endif
