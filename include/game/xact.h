#ifndef _GAME_XACT_H
#define _GAME_XACT_H

#include "game/xobj.h"
#include "game/cptcl.h"
#include "game/cscrn.h"
#include "game/xaction.h"

// chibi robo's actions, XAction and its XAct* kinds from RTTI. every one of them sits in
// XObjChr and shares XAction's two virtuals. offsets are the same in every region

class XObjChr;
struct HSD_Generator;

class XAction {
public:
	XObjChr* mOwner; // _00
	s32 mState;      // _04, 0 while idle
	// _08 vtable

	virtual void Init(XObjChr* owner); // _08, 0x800BBD7C
	virtual void Reset();              // _0C, 0x800BB708

	s32 _0C;          // _0C
	CVec _10;         // _10, push keeps the offset to its target here
	XObj* mTarget;    // _20
	s32 _24;          // _24
	s32 mTargetFloor; // _28, the target's _784 when it was picked

	// todo: cleanup
	void SetTarget(XObj* target)
	{
		mTarget      = target;
		mTargetFloor = mTarget->_784;
	}
	void SetState(s32 state, XObj* target)
	{
		mState = state;
		if (target != NULL) {
			SetTarget(target);
		}
	}
};
STATIC_ASSERT(sizeof(XAction) == 0x2C);

// picking things up and carrying them
class XActLift : public XAction {
public:
	f32 _2C;             // _2C
	CVec _30;            // _30
	u8 _40[0x48 - 0x40]; // _40
	s16 _48;             // _48, -1 when there's none
	u8 _4A[0x54 - 0x4A]; // _4A
	u32 _54;             // _54, copied to XObjPlug's _D3C
	u8 _58[0x60 - 0x58]; // _58
	u32 _60;             // _60
	u8 _64[0x6C - 0x64]; // _64
	u32 _6C;             // _6C
	u32 _70;             // _70
	u32 _74;             // _74
	u8 _78[0x7C - 0x78]; // _78
};
STATIC_ASSERT(sizeof(XActLift) == 0x7C);

class XActHang : public XAction {
public:
	BOOL _2C;            // _2C, set when the stick points away
	u8 _30[0x3C - 0x30]; // _30
	Vec _3C;             // _3C
	f32 _48;             // _48
	u8 _4C[0x50 - 0x4C]; // _4C
};
STATIC_ASSERT(sizeof(XActHang) == 0x50);

class XActJump : public XAction {
public:
	u8 _2C[0x48 - 0x2C]; // _2C
	f32 mStrength;       // _48
	f32 mTargetStrength; // _4C
	u8 _50[0x54 - 0x50]; // _50
};
STATIC_ASSERT(sizeof(XActJump) == 0x54);

class XActLadder : public XAction {
public:
	BOOL _2C;            // _2C, set when b is pressed on it
	u8 _30[0x44 - 0x30]; // _30
};
STATIC_ASSERT(sizeof(XActLadder) == 0x44);

class XActRope : public XAction {
public:
	u8 _2C[0x40 - 0x2C]; // _2C
};
STATIC_ASSERT(sizeof(XActRope) == 0x40);

class XActPush : public XAction {
public:
	u8 _2C[0x34 - 0x2C]; // _2C
	CVec _34;            // _34
	CVec _44;            // _44
	u8 _54[0x58 - 0x54]; // _54
};
STATIC_ASSERT(sizeof(XActPush) == 0x58);

// the ring on the floor around him while he tumbles
struct XActTumbleFade {
	BOOL mActive; // _00
	Vec mOffset;  // _04, from his feet, turned with him
	f32 mAmount;  // _10
};
STATIC_ASSERT(sizeof(XActTumbleFade) == 0x14);

class XActTumble : public XAction {
public:
	XActTumbleFade mFade; // _2C
	u8 _40[0x44 - 0x40];  // _40
	s32 _44;              // _44
};
STATIC_ASSERT(sizeof(XActTumble) == 0x48);

class XActCopter : public XAction {
public:
	u8 _2C[0x38 - 0x2C]; // _2C
};
STATIC_ASSERT(sizeof(XActCopter) == 0x38);

// one of the popper's rings, drawn as two glow sprites and a soft disc
struct XActPopperRing {
	Vec mPos;    // _00
	f32 mAmount; // _0C, 0 when it's gone
	f32 mRadius; // _10
	u32 mColor;  // _14
};
STATIC_ASSERT(sizeof(XActPopperRing) == 0x18);

// one popper shot, it keeps its last ten positions for the trail
struct XActPopperShot {
	BOOL mActive;              // _00
	CVec mPos;                 // _04
	f32 _14;                   // _14
	f32 _18;                   // _18
	f32 _1C;                   // _1C
	f32 mPhase;                // _20
	f32 _24;                   // _24
	f32 _28;                   // _28
	f32 mStrength;             // _2C
	s32 mKind;                 // _30, 1 for the small shot
	void* mAttachment;         // _34
	s32 mState;                // _38
	Vec mHistory[10];          // _3C
	XActPopperRing mRing[2];   // _B4
	CPtcl mPtcl;               // _E4
	CPtclDust2 mPtcls[40];     // _F8
	HSD_Generator* mGen;       // _D78
	HSD_Generator* mChargeGen; // _D7C
	HSD_JObj* mJoint;          // _D80

	XActPopperShot();
};
STATIC_ASSERT(sizeof(XActPopperShot) == 0xD84);

// the popper's charge effect
struct XActPopperCharge {
	u32 mState;                // _00
	f32 mTimer;                // _04
	u8 _08[0x14 - 0x8];        // _08
	HSD_Generator* mGen;       // _14
	HSD_Generator* mChargeGen; // _18
	HSD_JObj* mJoint;          // _1C
};
STATIC_ASSERT(sizeof(XActPopperCharge) == 0x20);

// the chibi-blaster
class XActPopper : public XAction {
public:
	u32 _2C;                  // _2C
	XActPopperShot mShot[10]; // _30
	XActPopperCharge mCharge; // _8758
};
STATIC_ASSERT(sizeof(XActPopper) == 0x8778);

class XActRadar : public XAction {
public:
	XObj* _2C;            // _2C
	Vec _30;              // _30
	Vec _3C;              // _3C
	CScrnColor mColor[2]; // _48
	f32 mFade;            // _B0
	f32 mAngle[2];        // _B4
	f32 _BC;              // _BC, NAN until it's placed
};
STATIC_ASSERT(sizeof(XActRadar) == 0xC0);

#endif
