#ifndef _GAME_XOBJSPIDER_H
#define _GAME_XOBJSPIDER_H

#include "game/scr.h"

// XObjSpider, game/xobjspider.cpp, the small spiders that walk the floor and the walls.
// the class name is from its vtable, nothing else has a name in the dol. offsets are usa

// __vt__7AnimSfx, plays the sounds an animation has at its frames. game/sound.cpp has the
// methods, and no destructor, so the objects that hold one don't destroy it
// one entry of an AnimSfx table, the sound an animation plays at a frame
struct AnimSfxEntry {
	s32 mAnim;    // _00, -1 ends the table
	f32 mFrame;   // _04
	u8 _08[0x10]; // _08
};

class AnimSfx {
public:
	AnimSfxEntry* mTable; // _00
	s32 mAnim;            // _04
	f32 mFrame;           // _08
	// _0C vtable

	virtual void vf08();                           // _08
	virtual void Play(s32 id, s32 vol, f32 frame); // _0C
};

// what an object is read from, game/xobjs.h has it but needs game/cobj.h, __vt__9CoordInfo
class CoordInfo {
public:
	void* _00; // _00
	// _04 vtable

	CoordInfo(void* value)
	    : _00(value)
	{
	}
	virtual ~CoordInfo() { } // _08
};

// another object the spider holds on one of its joints
struct UnkSpiderTie {
	HSD_JObj* mJoint; // _00
	XObj* mObj;       // _04
	Mtx mMtx;         // _08
};

// one thing the spider moves and draws through another object's model, 0x3C bytes
struct UnkSpiderPart {
	CVec mPos;       // _00
	CVec _10;        // _10
	f32 mRotY;       // _20
	f32 _24;         // _24
	s32 mState;      // _28
	s32 _2C;         // _2C
	CStatus mStatus; // _30, 1 while it's shown
	XObj* mObj;      // _38
};

class XObjSpider : public XObj {
public:
	f32 mDir;              // _980, the way it walks
	f32 mSpeed;            // _984
	f32 _988;              // _988
	f32 _98C;              // _98C
	f32 mTurnSpeed;        // _990
	u8 _994[0x4];          // _994
	f32 mThreadAlpha;      // _998, 0 to 1
	f32 mRadius;           // _99C
	f32 mFallSpeed;        // _9A0
	s32 mState;            // _9A4
	s32 mStep;             // _9A8
	s32 mTimer;            // _9AC
	CStatus mStatus;       // _9B0
	s32 mPrevState;        // _9B8
	s32 mWait;             // _9BC
	f32 _9C0;              // _9C0
	f32 _9C4;              // _9C4
	Vec mHitPos;           // _9C8
	Vec mHome;             // _9D4, y is the floor
	f32 mHitDist;          // _9E0
	s32 mWall;             // _9E4, 2 to 5 while it's on a wall
	s32 mLife;             // _9E8
	s32 mMaxLife;          // _9EC
	Vec mWallPos;          // _9F0
	Vec _9FC;              // _9FC
	Vec _A08;              // _A08
	Vec _A14;              // _A14
	u8 _A20[0xC];          // _A20
	Vec mShotTo;           // _A2C, where the part is shot at
	f32 _A38;              // _A38
	u8 _A3C[0x4];          // _A3C
	f32 _A40;              // _A40
	s32 _A44;              // _A44
	s32 _A48;              // _A48
	Vec mArea;             // _A4C, the corner of the floor it stays on
	f32 mAreaW;            // _A58
	f32 mAreaD;            // _A5C
	Vec _A60;              // _A60
	f32 _A6C;              // _A6C
	s32 mArgs[6];          // _A70, what the last event command said
	s32 _A88;              // _A88
	UnkSpiderPart mPart;   // _A8C
	UnkSpiderPart mPart2;  // _AC8
	u8 mCarry;             // _B04, which of the carried objects, 0xFF for none
	u8 _B05[0xB];          // _B05
	s16 _B10;              // _B10
	s32 _B14[4];           // _B14
	s32 _B24[3];           // _B24
	u8 _B30[0x20];         // _B30
	s16 _B50;              // _B50
	s32 _B54;              // _B54
	s16 _B58;              // _B58
	XObj* _B5C;            // _B5C
	s32 _B60;              // _B60
	Mtx _B64;              // _B64
	s32 _B94;              // _B94
	s32 _B98;              // _B98
	s32 _B9C;              // _B9C
	s32 mRumble;           // _BA0
	s32 mRumbleTimer;      // _BA4
	AnimSfx mSe;           // _BA8
	UnkSpiderTie mTies[8]; // _BB8

	virtual void Init(u32 id);                 // _90
	virtual void vf94();                       // _94
	virtual void Reset();                      // _98
	virtual BOOL vf9C(void* info);             // _9C
	virtual void Update(u32 frames);           // _A4
	virtual void vfAC();                       // _AC
	virtual void vfB8();                       // _B8
	virtual void vfBC(Vec* pos, f32 scale);    // _BC
	virtual void vfC0();                       // _C0
	virtual void vfC8();                       // _C8
	virtual void vfCC();                       // _CC
	virtual s32 vf100(s32 a, s32 b);           // _100
	virtual void vf138(const Vec* pos, f32 f); // _138
	virtual void vf148();                      // _148, plays the sounds along the anim

	// the states, sStates has them by number
	void State0(u32 frames);
	void State1(u32 frames);
	void State2(u32 frames);
	void State3(u32 frames);
	void State4(u32 frames);
	void State5(u32 frames);
	void State6(u32 frames);
	void State7(u32 frames);
	void State8(u32 frames);
	void State9(u32 frames);

	// XObj's _25C, scr.h has it in its padding
	s32 Get25C() const { return *(s32*)&_25C[0]; }

	void SetState(s32 state)
	{
		mStep      = 0;
		mTimer     = 0;
		mPrevState = mState;
		mState     = state;
	}
};
typedef char XObjSpider_size_check[sizeof(XObjSpider) == 0xD78 ? 1 : -1];

#endif
