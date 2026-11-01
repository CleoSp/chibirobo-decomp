#ifndef _GAME_OBJB_LETICKER_SHARED_H
#define _GAME_OBJB_LETICKER_SHARED_H

#include "game/objb_leticker.h"

// XObjLeticker, and the camera that turns around it in steps, CCameraActionLeticker

class CCamera;

// __vt__12XObjLeticker
class XObjLeticker : public XObj {
public:
	typedef void (XObjLeticker::*StateFunc)();

	StateFunc mState;  // _980
	s32 mStep;         // _98C
	s32 mParts[2];     // _990, indices in the game's object table
	s32 mPartCount;    // _998
	s32 _99C;          // _99C
	s32 _9A0;          // _9A0
	s32 _9A4[4];       // _9A4
	s32 mStateTime;    // _9B4
	XObjPoint mBox[2]; // _9B8, the point the parts stand on
	f32 _9F8;          // _9F8
	f32 _9FC;          // _9FC
	f32 _A00;          // _A00
	Vec _A04;          // _A04
	u8 _A10[0x14];     // _A10
	u8 _A24;           // _A24
	u8 _A25[0x3];      // _A25
	f32 _A28;          // _A28
	u8 mParticleOn;    // _A2C
	u8 _A2D;           // _A2D
	u8 _A2E[0x2];      // _A2E
	s32 _A30;          // _A30
	f32 _A34;          // _A34
	s32 mMode;         // _A38
	AnimSfx mSfx;      // _A3C
	u8 _A4C[0x4];      // _A4C

	virtual void Reset();                      // _98
	virtual void Update(u32 frames);           // _A4
	virtual void vfAC();                       // _AC
	virtual void vfC0();                       // _C0
	virtual void vf138(const Vec* pos, f32 f); // _138
	virtual void vf148();                      // _148

	BOOL TurnParts(f32 angle);

	void StateLift();
	void StateCount();
	void StateRide();
};
typedef char XObjLeticker_size_check[sizeof(XObjLeticker) == 0xA50 ? 1 : -1];

// __vt__21CCameraActionLeticker, turns around the leticker a quarter at a time
class CCameraActionLeticker : public CCameraActionLadr {
public:
	Vec* mTarget;   // _44
	f32 mTurnTo;    // _48
	f32 mTurnSpeed; // _4C
	s32 mTurnTime;  // _50
	u8 mBounded;    // _54
	f32 mMinX;      // _58
	f32 mMaxX;      // _5C
	f32 mMinZ;      // _60
	f32 mMaxZ;      // _64
	u8 _68[0x4];    // _68

	CCameraActionLeticker() { mTarget = NULL; }

	virtual void Update(u32 step);      // _10
	virtual void vf14();                // _14
	virtual u32 vf50() { return 0; }    // _50
	virtual u32 Get_38() { return 11; } // _54
	virtual f32 vf58() { return 0.1f; } // _58
	virtual Vec GetCameraTarget();      // _64

	// whether the eye at that angle stays inside the room
	bool CanTurn(f32 angle)
	{
		if (!mBounded) {
			return true;
		}
		Vec eye;
		CalcEye(&eye, angle);
		OSReport("%4.2f %4.2f\n", eye.x, eye.z);
		if (eye.x < mMinX || mMaxX < eye.x || eye.z < mMinZ || mMaxZ < eye.z) {
			OSReport("CCameraActionLeticker::Update %4.2f %4.2f %d\n", eye.x, eye.z, (s32)(57.295776f * mTurnTo));
			return false;
		}
		return true;
	}
};
typedef char CCameraActionLeticker_size_check[sizeof(CCameraActionLeticker) == 0x6C ? 1 : -1];

static inline f32 Cos(f32 x)
{
	return cos(x);
}

static inline f32 Sin(f32 x)
{
	return sin(x);
}

// which quarter an angle faces, 0 front, 1 left, 2 back, 3 right
static inline s32 GetSide(f32 angle)
{
	s32 side;
	if (Cos(0.7853982f) <= Cos(angle)) {
		side = 0;
	} else if (Cos(angle) <= Cos(3.9269907f)) {
		side = 2;
	} else if (0.0f < Sin(angle)) {
		side = 1;
	} else {
		side = 3;
	}
	return side;
}

static AnimSfxEntry sSfx[] = {
	{ 7, 10.0f, -1.0f, 1, 0x190, 0xFF, 0, 0, 0 },
	{ -1 },
};

extern "C" XObjPoint* lbl_8022A008[];
extern "C" bool fn_800E4790(XObj* obj, HSD_JObj* jobj);
extern "C" XObjPoint* lbl_8065A6D0[2];

#endif
