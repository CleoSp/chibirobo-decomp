#ifndef _GAME_OBJA_CHR_SHARED_H
#define _GAME_OBJA_CHR_SHARED_H

#include "game/hsd_jobj.h"
#include "game/cparticle.h"
#include "game/cobj.h"
#include "game/coordinfo.h"
#include "game/obja.h"
#include "game/hsd_cobj.h"
#include "game/hsd_dobj.h"
#include "game/cshadow.h"
#include "game/ctrigon.h"
#include "game/gmath.h"
extern "C" {
#include <dolphin/mtx/vec.h>
}
#include "game/cvec.h"
#include "game/cparticle.h"
#include <math.h>

// XObjChr, the robot, and the menus it opens (DrsMenu, AtcMenu, OctaMenu). nothing here has
// a name in the dol, so everything is extern "C" under its address. most of the small
// functions after the huge update functions are out of line copies of inlines they stopped
// inlining once they got too big

extern "C" {
s32 fn_8006E9AC(s32 port);
s32 fn_8006EAF0(s32 port);
BOOL fn_800D4FE8(const struct UnkChrSub* timer);
u32 CheckStageFlag(s32 stage, u32 mask);
}

// the objects the robot works with keep 64 bits of flags
struct UnkActorId {
	u8 _00[0x14]; // _00
	s16 mId;      // _14
};

struct UnkChrSub {
	u8 _00[4]; // _00
};

struct UnkActor {
	u8 _000[0x228];     // _000
	u64 mFlags;         // _228
	u8 _230[0x8];       // _230
	UnkChrSub mSub238;  // _238
	u8 _23C[0x4];       // _23C
	UnkActorId* mIdObj; // _240
};

static inline s16 GetActorId(UnkActorId* obj)
{
	if (obj != NULL) {
		return obj->mId;
	}
	return 0;
}

// the id of what it was placed from, -1 when it wasn't
static inline s16 GetId(const UnkActor* obj)
{
	if (obj->mIdObj != NULL) {
		return GetActorId(obj->mIdObj);
	}
	return -1;
}

// puts a CObjVec at a value and its target there too, the speed stays
static inline void SetNow(CObjVec* vec, f32 x, f32 y, f32 z)
{
	vec->x         = x;
	vec->y         = y;
	vec->z         = z;
	vec->mTarget.x = x;
	vec->mTarget.y = y;
	vec->mTarget.z = z;
}

// the y angle a direction faces
static inline f32 GetYAngle(const Vec& dir)
{
	return atan2(dir.x, dir.z);
}

// the y angle that faces from one point to another
static inline f32 GetAngle(const Vec& from, const Vec& to)
{
	return atan2(to.x - from.x, to.z - from.z);
}

// a placement table, 0x20 byte entries in whole hundredths
struct UnkPlaceEntry {
	s32 _00;      // _00
	s32 mPos[3];  // _04
	s32 mRotY;    // _10, degrees
	s32 mPos2[3]; // _14
};

struct UnkPlaceTable {
	UnkPlaceEntry* mEntries; // _00
};

// the robot's state machine, a state and its previous one
struct UnkState {
	s32 _00;    // _00
	s32 mState; // _04

	BOOL Is(s32 value) const { return value == mState; }
};

struct UnkHolder {
	u8 _000[0x24C];     // _000
	XObjPoint* mPoints; // _24C
	u8 _250[0x534];     // _250
	s32 mId;            // _784
};

struct UnkLink {
	u8 _00[0xC];      // _00
	s32 mKind;        // _0C
	u8 _10[0x10];     // _10
	UnkHolder* mHold; // _20
	s32 _24;          // _24
	s32 mHoldId;      // _28
	u8 _2C[0x10];     // _2C
	UnkChrSub mSub3C; // _3C
	u8 _40[0x8];      // _40
	f32 _48;          // _48
};

struct UnkTimer {
	u8 _00[0x24]; // _00
	f32 mTime;    // _24
};

struct UnkPair {
	s32 _000;       // _000
	u8 _004[0x294]; // _004
	s32 _298;       // _298
};

struct UnkChrState {
	u8 _000[0x980]; // _000
	u32 mFlags;     // _980
};

extern "C" s32 fn_8001E6C8(void* list, s32 id);

struct UnkMenuOwner;

struct UnkBig {
	u8 _00000[0x96BC];           // _00000
	UnkMenuOwner* mChr;          // _096BC
	u8 _096C0[0x5D07C - 0x96C0]; // _096C0
	u8 mSound[0x4];              // _5D07C
	void* _5D080;                // _5D080
	u8 _5D084[0x8ADA0];          // _5D084
	u8 _E7E24[0x4];              // _E7E24

	s32 Check(s32 id) { return fn_8001E6C8(_E7E24, id); }
	BOOL IsOff(s32 id) { return !fn_8001E6C8(_E7E24, id); }
};

struct UnkChrSlot {
	u8 _00[0x54]; // _00
};

// the robot, a CXObjIntp underneath. offsets are usa
struct UnkChr {
	HSD_JObj* mJObj;           // _000
	u8 _004[0xB0];             // _004
	UnkChrSub mSubB4;          // _0B4
	u8 _0B8[0x194];            // _0B8
	UnkChrSub mSub24C;         // _24C
	u8 _250[0xC];              // _250
	u32 _25C[2];               // _25C
	UnkChrSub mSub264;         // _264
	u8 _268[0xC];              // _268
	s32 mIndex;                // _274
	u8 _278[0x480];            // _278
	f32 _6F8;                  // _6F8
	f32 _6FC[2];               // _6FC
	u8 _704[0x78];             // _704
	f32 _77C;                  // _77C
	f32 _780;                  // _780
	s32 mId;                   // _784
	u8 _788[0x128];            // _788
	u32 mFlags;                // _8B0
	u8 _8B4[0xD0];             // _8B4
	s32 _984;                  // _984
	u8 _988[0x64];             // _988
	UnkChrSub mSub9EC;         // _9EC
	u8 _9F0[0x44];             // _9F0
	s32 _A34;                  // _A34
	u8 _A38[0x8];              // _A38
	f32 _A40;                  // _A40
	UnkChrSub mTimer;          // _A44
	s32 mTimerState;           // _A48
	u8 _A4C[0x18];             // _A4C
	UnkActor* mTarget;         // _A64
	u8 _A68[0x5C];             // _A68
	s32 mStateAC4;             // _AC4
	u8 _AC8[0x18];             // _AC8
	UnkHolder* mHoldAE0;       // _AE0
	u8 _AE4[0x4];              // _AE4
	s32 mHoldIdAE8;            // _AE8
	u8 _AEC[0x7C];             // _AEC
	s32 mStateB68;             // _B68
	u8 _B6C[0x18];             // _B6C
	UnkHolder* mHoldB84;       // _B84
	u8 _B88[0x4];              // _B88
	s32 mHoldIdB8C;            // _B8C
	u8 _B90[0x1C];             // _B90
	s32 mStateBAC;             // _BAC
	u8 _BB0[0x18];             // _BB0
	UnkHolder* mHoldBC8;       // _BC8
	u8 _BCC[0x4];              // _BCC
	s32 mHoldIdBD0;            // _BD0
	u8 _BD4[0x70];             // _BD4
	s32 _C44;                  // _C44
	u8 _C48[0x24];             // _C48
	s32 _C6C;                  // _C6C
	u8 _C70[0xC];              // _C70
	f32 _C7C;                  // _C7C
	u8 _C80[0xBC];             // _C80
	s32 _D3C;                  // _D3C
	u8 _D40[0xC74];            // _D40
	UnkChrSlot mSlots[4];      // _19B4
	u8 _1B04[0xC];             // _1B04
	u64 mFlags2;               // _1B10
	u8 _1B18[0x331C - 0x1B18]; // _1B18
	s32 _331C;                 // _331C
	s32 _3320;                 // _3320
	s32 _3324;                 // _3324

	bool IsBusy() const
	{
		bool ret = false;
		if ((_331C != 0 || _3320 != 0) && _3324 == 0) {
			ret = true;
		}
		return ret;
	}
};

// the robot's anims run a bit faster while it is in a hurry
static inline f32 GetAnimScale(const UnkChr* chr)
{
	if (chr->mTimerState == 4 && fn_800D4FE8(&chr->mTimer)) {
		return 1.135f;
	}
	return 1.0f;
}

static inline BOOL IsTimerDone(const UnkChr* chr)
{
	return chr->mTimerState == 4 && fn_800D4FE8(&chr->mTimer);
}

// the game object behind lbl_8025C400, the CGs is only its start
struct UnkGame {
	u8 _0000[0x331C]; // _0000
	s32 _331C;        // _331C
	s32 _3320;        // _3320
	s32 _3324;        // _3324
	u8 _3328[0x11C];  // _3328
	s32 mStage;       // _3444

	// an event or a talk has the robot
	bool IsBusy() const
	{
		bool ret = false;
		if ((_331C != 0 || _3320 != 0) && _3324 == 0) {
			ret = true;
		}
		return ret;
	}
};

// a table of rooms, each with a list of points and a grid
struct UnkRoomList {
	u8* mData;   // _00
	s32 mCount;  // _04
	u8 _08[0x8]; // _08
	s32 mStride; // _10
};

struct UnkRoom {
	s32 _0000[0x400]; // _0000
	s16 _1000[0x400]; // _1000
	s32 _1800;        // _1800
};

struct UnkMap {
	u8 _0000[0x8580];      // _0000
	s32 _8580;             // _8580
	u8 _8584[0x104C];      // _8584
	s32 _95D0;             // _95D0
	s32 _95D4;             // _95D4
	u8 _95D8[0xBC];        // _95D8
	UnkRoomList mLists[4]; // _9694
	u8 _96E4[0x17C];       // _96E4
	u32 _9860[0x400];      // _9860
	s32 _A860;             // _A860
	u8 _A864[0xC];         // _A864
	UnkRoom mRooms[1];     // _A870
};

struct UnkStageTable {
	UnkStageObj* mObjs; // _00
};

// a value that waits to be taken, mNext is only set while nothing waits
struct UnkReq {
	s32 mBusy;  // _00
	s32 mState; // _04
	s32 mNext;  // _08

	BOOL Is(s32 value) const { return value == mState; }
	void Request(s32 value)
	{
		if (mBusy == 0) {
			mNext = value;
			mBusy = 1;
		}
	}
};

// one 0x28 byte slot of a menu
struct UnkMenuEntry {
	u8 _00[0x4];  // _00
	s32 mId;      // _04
	s32 mCount;   // _08
	f32 mTime;    // _0C
	f32 mSpin;    // _10
	f32 mScale;   // _14
	u8 _18[0x10]; // _18
};

// one of the sparks that fly around an open menu
struct UnkMenuSpark {
	u8 _00[0x4]; // _00
	f32 mAngle;  // _04
	f32 mSpin;   // _08
};

// name from __vt__8OctaMenu, DrsMenu and AtcMenu are on it
class OctaMenu {
public:
	s32 mOpen;                // _000
	s32 mCursor;              // _004
	f32 mFade;                // _008, 1 once it is all in
	f32 _00C;                 // _00C
	UnkMenuEntry mEntries[8]; // _010
	u8 _150[0x8];             // _150
	f32 mScreenX;             // _158
	f32 mScreenY;             // _15C
	u8 _160[0x8];             // _160
	UnkMenuOwner* mOwner;     // _168
	f32 _16C;                 // _16C
	s32 _170;                 // _170
	UnkMenuSpark mSparks[22]; // _174
	// _27C vtable

	virtual ~OctaMenu() { }             // _08
	virtual void vf0C(s32 a);           // _0C
	virtual void vf10(s32 dir);         // _10
	virtual void vf14();                // _14
	virtual void vf18();                // _18
	virtual void vf1C();                // _1C
	virtual s32 vf20(s32 i);            // _20
	virtual const GXColor* vf24(s32 i); // _24
	virtual u32 vf28(s32 i);            // _28
	virtual BOOL vf2C(s32 i);           // _2C

	// after the virtuals, so the vtable comes before them
	s32 _280;      // _280
	u8 _284[0x4];  // _284
	s32 _288;      // _288
	s32 _28C;      // _28C
	s32 mSelected; // _290
	f32 mHeadTurn; // _294, nan to turn the head with the cursor
};
typedef char OctaMenu_size_check[sizeof(OctaMenu) == 0x298 ? 1 : -1];

// name from __vt__7DrsMenu
class DrsMenu : public OctaMenu {
public:
	virtual void vf1C();                // _1C
	virtual void vf0C(s32 a);           // _0C
	virtual s32 vf20(s32 i);            // _20
	virtual const GXColor* vf24(s32 i); // _24
	virtual u32 vf28(s32 i);            // _28
	virtual BOOL vf2C(s32 i);           // _2C

	void Select(BOOL on)
	{
		if (on) {
			mSelected = 1;
			mCursor   = -1;
		} else {
			mSelected = 0;
		}
	}
};

// the two menus that open together, the second one's fields are the pair's
struct UnkMenuPair {
	DrsMenu mMenu0; // _000
	DrsMenu mMenu1; // _298
	u8 _530[0x14];  // _530
	s32 _544;       // _544
	s32 _548;       // _548

	bool IsActive() const { return mMenu0.mOpen != 0 || mMenu1.mOpen != 0; }
	bool IsClosing() const
	{
		bool ret = false;
		if (mMenu0.mOpen < 0 || mMenu1.mOpen < 0) {
			ret = true;
		}
		return ret;
	}

	bool IsOpen() const
	{
		bool ret = false;
		if (mMenu0.mOpen > 0 || mMenu1.mOpen > 0) {
			ret = true;
		}
		return ret;
	}
};

typedef OctaMenu UnkMenu;
struct UnkMenuOwner;
static inline void StopMove(UnkMenuOwner* owner);

// one of the two things that spin around the robot
struct UnkSpin {
	Vec mAngle;   // _000, the turn, the spin and the scale
	u8 _00C[0x4]; // _00C
	XObj mObj;    // _010
};

// the two things that spin around the robot while it picks one
struct UnkSpinner {
	u8 _0000[0x4];        // _0000
	s32 mOn;              // _0004, 1 opening, -1 closing
	s32 _0008;            // _0008
	s32 mSelected;        // _000C
	f32 mAlpha;           // _0010
	u8 _0014[0x4];        // _0014
	UnkSpin mSpins[2];    // _0018
	UnkMenuOwner* mOwner; // _1338
	f32 mRotY;            // _133C, the turn it had before
	s32 mPicked;          // _1340
};
typedef char UnkSpinner_size_check[sizeof(UnkSpinner) == 0x1348 ? 1 : -1];

// the CPadOne a CPad points at, as far as this file reads it
struct UnkPadOne {
	u8 _00[0x4];    // _00
	u32 mButton;    // _04
	u8 _08[0xC];    // _08
	u32 mTrigger;   // _14
	u8 _18[0x4];    // _18
	u32 mRepeat;    // _1C
	u32 mDirRepeat; // _20
};

struct UnkPadRef {
	UnkPadOne* mPad; // _00
};

// the robot's pad, name from __vt__4XPad, a CPadOne
class XPad {
public:
	u8 _00[0x50]; // _00
	// _50 vtable

	virtual ~XPad();                 // _08
	virtual void Init(u8 port);      // _0C
	virtual void Clear();            // _10
	virtual void Update(s32 frames); // _14
};

// the popper's effect, UnkPopperFx in objb_act.cpp
struct UnkFx {
	u8 _00[0x1C]; // _00
};

extern "C" void fn_800C17BC(UnkFx* fx);

// one of the robot's popper shots, XActPopperShot in objb.h
struct UnkShot {
	u8 _000[0xD84]; // _000
};

extern "C" void fn_800C2D18(UnkShot* shot);

// a joint that blinks, its anim is added the first time
struct UnkBlink {
	HSD_JObj* mJObj; // _00
	void* mAnim;     // _04
	u8* mData;       // _08, the anim data it plays, what tells it from the other anims
};

// the robot's item list, objb_path.cpp
struct UnkItemTable {
	u8 _00[0x4]; // _00
};

extern "C" s32 fn_800DCD30(UnkItemTable* table, s16 id);

// the robot as the menus see it
struct UnkMenuOwner : public XObj {
	u32 mChrFlags;                // _980
	u8 _984[0x4];                 // _984
	CVec mSpeeds[3];              // _988
	u8 _9B8[0x1C];                // _9B8
	s32 mHeld[2];                 // _9D4, the objects it holds, by game object index
	u8 _9DC[0xA2C - 0x9DC];       // _9DC
	s32 _A2C;                     // _A2C
	u8 _A30[0x4];                 // _A30
	s32 _A34;                     // _A34
	s32 _A38;                     // _A38
	f32 _A3C;                     // _A3C
	u8 _A40[0x4];                 // _A40
	UnkChrSub mTimer;             // _A44
	s32 _A48;                     // _A48
	u8 _A4C[0x18];                // _A4C
	XObj* mTarget;                // _A64
	u8 _A68[0xA8C - 0xA68];       // _A68
	s16 _A8C;                     // _A8C
	u8 _A8E[0xAC4 - 0xA8E];       // _A8E
	s32 _AC4;                     // _AC4
	u8 _AC8[0xAE0 - 0xAC8];       // _AC8
	XObj* mHoldAE0;               // _AE0
	u8 _AE4[0xB14 - 0xAE4];       // _AE4
	s32 _B14;                     // _B14
	u8 _B18[0xB68 - 0xB18];       // _B18
	s32 _B68;                     // _B68
	u8 _B6C[0xB84 - 0xB6C];       // _B6C
	XObj* mHoldB84;               // _B84
	u8 _B88[0xBAC - 0xB88];       // _B88
	s32 _BAC;                     // _BAC
	u8 _BB0[0xBC8 - 0xBB0];       // _BB0
	XObj* mHoldBC8;               // _BC8
	u8 _BCC[0xBE8 - 0xBCC];       // _BCC
	UnkChrSub _BE8;               // _BE8
	s32 _BEC;                     // _BEC
	u8 _BF0[0x4];                 // _BF0
	s32 _BF4;                     // _BF4
	u8 _BF8[0xC44 - 0xBF8];       // _BF8
	s32 _C44;                     // _C44
	u8 _C48[0xC6C - 0xC48];       // _C48
	s32 _C6C;                     // _C6C
	u8 _C70[0xC7C - 0xC70];       // _C70
	f32 _C7C;                     // _C7C
	u8 _C80[0xC8C - 0xC80];       // _C80
	s32 _C8C;                     // _C8C
	u8 _C90[0xCC4 - 0xC90];       // _C90
	s32 _CC4;                     // _CC4
	u8 _CC8[0xCF0 - 0xCC8];       // _CC8
	UnkShot mShots[10];           // _CF0
	UnkFx mPopperFx;              // _9418
	u8 _9434[0x943C - 0x9434];    // _9434
	s32 _943C;                    // _943C
	u8 _9440[0x94F8 - 0x9440];    // _9440
	f32 mFrame;                   // _94F8, the anim frame the particles last ran at
	f32 mLastFrame;               // _94FC
	UnkBlink mBlinks[4];          // _9500
	s32 mBlinkTimer;              // _9530, frames to the next blink
	u8 _9534[0x955C - 0x9534];    // _9534
	CJointRot mJointRots[2];      // _955C
	u8 _959C[0x95D0 - 0x959C];    // _959C
	s32 _95D0;                    // _95D0
	s32 _95D4;                    // _95D4
	s32 _95D8;                    // _95D8, how far the room change is
	s32 _95DC;                    // _95DC
	s32 _95E0;                    // _95E0
	s32 _95E4;                    // _95E4
	u8 _95E8[0x9658 - 0x95E8];    // _95E8
	f32 _9658;                    // _9658
	u8 _965C[0x9688 - 0x965C];    // _965C
	UnkItemTable mItems;          // _9688, what the robot carries
	u8 _968C[0x9AD0 - 0x968C];    // _968C
	XObj mSub9AD0;                // _9AD0
	u8 _A450[0xB1B0 - 0xA450];    // _A450
	XObj mSubB1B0;                // _B1B0
	u8 _BB30[0x4];                // _BB30
	s32 _BB34;                    // _BB34
	XObj mSubBB38;                // _BB38
	u8 _C4B8[0x8];                // _C4B8
	XObj mSubC4C0;                // _C4C0
	u8 _CE40[0x18];               // _CE40
	XObj mSubCE58;                // _CE58
	u8 _D7D8[0xC];                // _D7D8
	UnkReq mReq;                  // _D7E4
	u8 _D7F0[0x8];                // _D7F0
	XObj mSubs[9];                // _D7F8
	u8 _12D78[0x12DA0 - 0x12D78]; // _12D78
	XPad mPad;                    // _12DA0
	u8 _12DF4[0x4];               // _12DF4
	f32 mMoveAngle;               // _12DF8
	u8 _12DFC[0x12E44 - 0x12DFC]; // _12DFC
	UnkMenuPair mPair;            // _12E44
	UnkSpinner mSpinner;          // _13390
	u8 _146D8[0x158A4 - 0x146D8]; // _146D8
	s32 _158A4;                   // _158A4
	u8 _158A8[0x8];               // _158A8
	f32 _158B0;                   // _158B0
	s32 _158B4;                   // _158B4
	s32 mIdleKind;                // _158B8
	s32 mIdleTime;                // _158BC
	u8 _158C0[0x14];              // _158C0
	CJAnim mAnimSets[2];          // _158D4, anims from 2000 and from 3000

	s32 GetRoom() const { return _95D4 >= 0 ? _95D4 : _95D0; }
	BOOL IsRoom(s32 value) const { return value == (_95D4 >= 0 ? _95D4 : _95D0); }
	void RequestRoom(s32 room)
	{
		if (room != _95D0) {
			_95D4 = room;
			_95D8 = 0;
		}
	}
};

struct UnkPoolItem {
	s32 mState;   // _00
	u8 _04[0x20]; // _04
};

// one of the ten things the robot can have going at once
struct UnkPoolObj {
	u8 _000[0x8];           // _000
	s32 mType;              // _008
	s32 _00C;               // _00C
	s32 mValue;             // _010
	u8 _014[0x54];          // _014
	u32 mColors[2];         // _068
	u8 _070[0x4];           // _070
	s32 _074;               // _074
	s32 mCount;             // _078
	UnkPoolItem mItems[40]; // _07C

	bool IsFree() const
	{
		bool ret = false;
		if (_00C == 0 && _074 == 0) {
			ret = true;
		}
		return ret;
	}
};

extern "C" {
extern UnkPoolObj lbl_80378300[10];
extern UnkPoolObj* lbl_8037C018[10];
void fn_80095AD8(UnkPoolObj* obj, s32 type, s32 value);
}

// one point of a trail that curls up from the robot
struct UnkTrailPoint {
	Vec mPos;   // _00
	f32 mAlpha; // _0C
	f32 _10;    // _10
	f32 mAngle; // _14
};

// two trails of ten points, each point takes the place of the one before it
struct UnkTrails {
	BOOL mOn;                     // _00
	UnkTrailPoint mPoints[2][10]; // _04
};

// a glow that fades in and out and spins while it shows
struct UnkGlow {
	BOOL mOn;     // _00
	u8 _04[0x20]; // _04
	f32 mAlpha;   // _24
	f32 mAngle;   // _28, degrees
	f32 mPulse;   // _2C
};

// what the jobj walk that colors a model gets
struct UnkColorArgs {
	GXColor* mDiffuse;  // _00
	GXColor* mSpecular; // _04
	f32* mAlpha;        // _08
};

struct UnkMenuList {
	s32 mId;    // _00
	s32 mValue; // _04
};

// slots numbered 1 to 8 with a value each
struct UnkSlots {
	UnkMenuList mSlots[9]; // _00

	s32 Find(s32 id) const
	{
		for (int i = 0; i < 9; i++) {
			if (id == mSlots[i].mId) {
				return i;
			}
		}
		return -1;
	}
};

struct UnkRange {
	f32 mMax; // _00
	f32 mMin; // _04
};

struct UnkRangeTest {
	u8 _00[0x4]; // _00
	f32 mMin;    // _04
	f32 mMax;    // _08
	u8 _0C[0x8]; // _0C
	s32 _14;     // _14
};

inline f32 HSD_AObjGetCurrFrame(HSD_AObj* aobj)
{
	AOBJ_H_ASSERT(0x95, aobj);
	return aobj->curr_frame;
}

// msg.cpp has the dol's copy, this file inlines it
inline CJObj::CJObj(HSD_JObj* jobj)
{
	Attach(jobj);
}

// the frame the jobj's first anim is at
static inline f32 GetFrame(const CJObj* obj)
{
	HSD_AObj* aobj = GetAObj(obj->mJObj);
	return aobj != NULL ? HSD_AObjGetCurrFrame(aobj) : 0.0f;
}

// the joint at the end of the path, one child step then path[i] sibling steps per level
static inline HSD_JObj* FindJoint(HSD_JObj* jobj, s32 depth, s32* path)
{
	if (jobj == NULL) {
		return NULL;
	}
	for (int i = 0; i < depth; i++, path++) {
		jobj = jobj == NULL ? NULL : jobj->child;
		for (int j = 0; j < *path; j++) {
			jobj = jobj == NULL ? NULL : jobj->next;
			if (jobj == NULL) {
				return NULL;
			}
		}
	}
	return jobj;
}

extern "C" {
s32 fn_800991FC(u8* menu, s32 id);
const GXColor* fn_8006DBF0(s32 id);
const GXColor* fn_8006E034(s32 id);
const GXColor* fn_8006E160(s32 id);
BOOL fn_800E4AAC(CXObj* obj, const CObjVec* pos);
BOOL fn_800665E0(void* list);
s32 fn_8001E6C8(void* list, s32 id);
s32 fn_8009C1BC(UnkSlots* slots, s32 id);
void fn_8009C398(UnkSlots* slots);
void fn_8009C8E0(UnkSlots* slots);
CVec fn_800A0894(const UnkChr* chr, s32 depth, s32* path);
CVec fn_8007E7F4(UnkChr* chr, s32 depth, s32* path);
void fn_800908C0(void* obj, MtxPtr vmtx);
void fn_8001E904(void* sound, s32 id, s32 volume);
void fn_800C5A00(UnkChrSub* sub);
BOOL fn_800D4D54(UnkChrSub* timer);
void fn_800A0014(UnkMenuOwner* chr, BOOL react, BOOL stop, f32 time);
void fn_80090864(XObj* obj);
void fn_8008B0D8(XObj* obj, s32 mode);
void fn_800A30EC(UnkMenuOwner* chr, s32 pass);
void fn_8008A820(XObj* obj, s32 i, u8 r, u8 g, u8 b, f32 seconds);
int rand(void);
BOOL fn_800D4F0C(UnkChrSub* timer);
BOOL fn_800D4E30(UnkChrSub* timer);
BOOL fn_800D4C78(UnkChrSub* timer);
BOOL fn_8008A38C(CJAnim* anim, const char* path);
void fn_800D5804(XObj* obj);
void fn_800D5874(XObj* obj);
s32 fn_80093210(void* map, s32 trigon, s32 tri);
void PSMTXRotRad(Mtx m, char axis, f32 rad);
}

// a joint whose anim runs with the robot's when a menu item is picked
struct UnkJointAnim {
	s32 mId;       // _00, the menu item, negative ends the table
	s32 mDepth;    // _04
	s32 mPath[17]; // _08
};

extern "C" {
extern UnkJointAnim lbl_80226130[];
void fn_80091C44(XObj* obj, void* args);
void fn_80091834(XObj* obj, s32 on, u8 mode);
void fn_8007EEA4(void* sound, s32 pos, void* obj, s16 a, s16 b);
void HSD_MObjSetDiffuseColor(HSD_MObj* mobj, u8 r, u8 g, u8 b);
void fn_801BA3AC(HSD_MObj* mobj, u8 r, u8 g, u8 b);

BOOL fn_800B1B24(const UnkChr* chr);
extern u8 lbl_8025C400[];
}

#define GAME_LIST   (lbl_8025C400 + 0x8F38)
#define GAME_E7E24  (lbl_8025C400 + 0xE7E24)
#define GAME_BIG    ((UnkBig*)lbl_8025C400)
#define GAME_FLAGS  (*(u64*)(lbl_8025C400 + 0x1B10))
#define GAME_OBJS   ((XObj**)(lbl_8025C400 + 0x9860))
#define GAME_CAMERA ((CMotionControlCamera*)(lbl_8025C400 + 0xE5D38))

// game/cmotioncontrolcamera.h can't come in next to game/obja.h, both have an XObj
class CMotionControlCamera {
public:
	u8 _00[0x94]; // _00
	Vec mEye;     // _94
	u8 _A0[0x14]; // _A0
	f32 mPitch;   // _B4
	f32 mYaw;     // _B8

	f32 GetMapDistance();
};

// the game object, only for the camera it hands out
class UnkGameView {
public:
	u8 _0000[0x3334]; // _0000
	// _3334 vtable

	virtual void vf08();          // _08
	virtual void vf0C();          // _0C
	virtual void vf10();          // _10
	virtual void vf14();          // _14
	virtual void vf18();          // _18
	virtual void vf1C();          // _1C
	virtual void vf20();          // _20
	virtual void vf24();          // _24
	virtual void vf28();          // _28
	virtual void vf2C();          // _2C
	virtual void vf30();          // _30
	virtual void vf34();          // _34
	virtual CCamera* GetCamera(); // _38
};
#define GAME_VIEW ((UnkGameView*)lbl_8025C400)

// a particle an anim frame starts, from a table of them
struct UnkPtclSpec {
	s32 mId;    // _00, -1 for none
	Vec mPos;   // _04
	Vec mScale; // _10
	Vec mRot;   // _1C
};

struct UnkPtclTable;

extern "C" {
extern UnkPtclTable lbl_80226324[];
extern UnkPtclTable lbl_802276D4[];
UnkPtclSpec fn_800A7E1C(void* obj, f32* frame, const UnkPtclTable* table);
}

#define GAME_PARTICLE ((CParticle*)(lbl_8025C400 + 0x9114))

static const GXColor lbl_8065D4AC = { 0x00, 0x30, 0x30, 0x30 };

#endif
