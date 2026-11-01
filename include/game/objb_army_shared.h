#ifndef _GAME_OBJB_ARMY_SHARED_H
#define _GAME_OBJB_ARMY_SHARED_H

#include "game/objb_leticker.h"

// XObjArmy, the toy soldiers, and the shots they fire

// the robot's fields these read, game/objb.h has the robot
struct UnkChrMode {
	u8 _00[0x95D0]; // _00
	s32 _95D0;      // _95D0
	s32 _95D4;      // _95D4

	s32 GetMode() const { return _95D4 >= 0 ? _95D4 : _95D0; }
};

// one shot a soldier fires, it flies at the robot or the leader and drops when it hits
class ArmyShot {
public:
	s32 mActive; // _00
	s32 mLife;   // _04, frames left
	CVec mPos;   // _08
	CVec mVel;   // _18
	f32 mSpeed;  // _28
	s32 mPhase;  // _2C, 0 flying, 1 bouncing off, 2 on the floor

	void Update(u32 frames);

	void Move()
	{
		mPos.x += mSpeed * mVel.x;
		mPos.y += mSpeed * mVel.y;
		mPos.z += mSpeed * mVel.z;
	}
};

// nothing reads it
static s32 sUnused[26] = { 0, 3, 0, 0, 3, 0, 0, 2, 0, 0, 2, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 3, 0, 0, 3, 0 };

// the points the soldier stands on, upright, falling over and lying down
static XObjPoint sPointsStand[2] = {
	{ 0, 0, { 0, 0x226, 0 }, 0, { 0x200, 0x200, 0x200 } },
	{ -1 },
};
static XObjPoint sPointsFall[2] = {
	{ 0, 0, { 0, 0x100, -0xD4 }, 0, { 0x200, 0x100, 0x200 } },
	{ -1 },
};
static XObjPoint sPointsDown[2] = {
	{ 0, 0, { 0, 0x100, 0 }, 0, { 0x200, 0x100, 0x200 } },
	{ -1 },
};
static XObjPoint* sPoints[3] = { sPointsStand, sPointsFall, sPointsDown };

static AnimSfxEntry sSfxF7[] = {
	{ 0x02, 18.0f, 36.0f, 1, 0x11000E, 0xFF, 4, 0, 0 }, { 0x03, 14.0f, 27.0f, 1, 0x11000E, 0xFF, 4, 0, 0 },
	{ 0x11, 11.0f, 22.0f, 1, 0x11000E, 0xFF, 4, 0, 0 }, { 0x12, 5.0f, -1.0f, 1, 0x11000D, 0xFF, 4, 0, 0 },
	{ 0x0F, 0.0f, -1.0f, 1, 0x11001C, 0xFF, 0, 0, 0 },  { 0x07, 28.0f, -1.0f, 1, 0x110028, 0xFF, 0, 0, 0 },
	{ 0x28, 18.0f, 36.0f, 1, 0x11000E, 0xFF, 0, 0, 0 }, { -1 },
};
static AnimSfxEntry sSfxF8[] = {
	{ 0x02, 18.0f, 36.0f, 1, 0x11000E, 0xFF, 4, 0, 0 },
	{ 0x03, 14.0f, 27.0f, 1, 0x11000E, 0xFF, 4, 0, 0 },
	{ 0x11, 11.0f, 22.0f, 1, 0x11000E, 0xFF, 4, 0, 0 },
	{ 0x12, 5.0f, -1.0f, 1, 0x11000D, 0xFF, 4, 0, 0 },
	{ 0x0F, 0.0f, -1.0f, 1, 0x11001C, 0xFF, 0, 0, 0 },
	{ 0x07, 28.0f, -1.0f, 1, 0x110028, 0xFF, 0, 0, 0 },
	{ -1 },
};

// the y angle that looks from to at from, 0 when they're on top of each other
static inline f32 GetAngleY(const Vec& from, const Vec& to)
{
	f32 dx = to.x - from.x;
	f32 dz = to.z - from.z;
	if (sqrtf(dx * dx + dz * dz) == 0.0f) {
		return 0.0f;
	}
	f32 angle = atan2(from.x - to.x, from.z - to.z);
	while (angle < 0.0f) {
		angle += 6.2831855f;
	}
	while (6.2831855f <= angle) {
		angle -= 6.2831855f;
	}
	return angle;
}

// __vt__8XObjArmy
class XObjArmy : public XObj {
public:
	typedef void (XObjArmy::*StateFunc)();

	StateFunc mState;   // _980
	s32 mStep;          // _98C
	s32 mTimer;         // _990
	f32 _994;           // _994
	u8 _998[0x8];       // _998
	CStatus mStatus;    // _9A0
	CVec mGuardPos;     // _9A8
	s32 mMode;          // _9B8
	ArmyShot mShots[5]; // _9BC
	AnimSfx mSfx;       // _AAC
	u8 _ABC[0x4];       // _ABC

	virtual void Reset();                      // _98
	virtual void Update(u32 frames);           // _A4
	virtual void vfC0();                       // _C0
	virtual void vf138(const Vec* pos, f32 f); // _138
	virtual void vf148();                      // _148

	void Attack()
	{
		mStep  = 0;
		mState = &XObjArmy::State81A0;
	}

	// what the soldier does, 1 marches, 2 stands guard
	void SetMode(s32 mode)
	{
		mMode = mode;
		RestartMode();
	}

	void RestartMode()
	{
		mStep = 0;
		switch (mMode) {
		case 1:
			mFlags |= 8;
			mState = &XObjArmy::State86F8;
			break;
		case 2:
			mState = &XObjArmy::State77F4;
			break;
		default:
			mState = NULL;
			break;
		}
	}

	void Shoot();

	void State77F4();
	void State7DDC();
	void State81A0();
	void State8564();
	void State86F8();
};
typedef char XObjArmy_size_check[sizeof(XObjArmy) == 0xAC0 ? 1 : -1];

static Vec sMin;
static Vec sMax;
static XObj* sLeader;
static s32 sB2A4;
static s32 sB2A8;
static XObj* sTarget;
static CStatus sStatus;

extern "C" {
s32 fn_8006EBC8(s32 id);
f32 HSD_Randf(void);
}

#endif
