#include "game/cobj.h"
#include "game/xobjs.h"
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
#include <math.h>

// XObjChr, the robot, and the menus it opens (DrsMenu, AtcMenu, OctaMenu). nothing here has
// a name in the dol, so everything is extern "C" under its address. most of the small
// functions after the huge update functions are out of line copies of inlines they stopped
// inlining once they got too big

extern "C" {
void PSVECScale(Vec* src, Vec* dst, f32 scale);
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
	u8 _0000[0x3444]; // _0000
	s32 mStage;       // _3444
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
	u8 _00[0xC];  // _00
	s32 mId;      // _0C
	s32 mCount;   // _10
	f32 mTime;    // _14
	u8 _18[0x10]; // _18
};

// name from __vt__8OctaMenu, DrsMenu and AtcMenu are on it
class OctaMenu {
public:
	s32 mOpen;                // _000
	s32 mCursor;              // _004
	UnkMenuEntry mEntries[8]; // _008
	u8 _148[0x20];            // _148
	UnkMenuOwner* mOwner;     // _168
	f32 _16C;                 // _16C
	u8 _170[0x10C];           // _170
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
};

// name from __vt__7DrsMenu
class DrsMenu : public OctaMenu {
public:
	s32 _280;      // _280
	u8 _284[0x8];  // _284
	s32 _28C;      // _28C
	s32 mSelected; // _290
	u8 _294[0x4];  // _294

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

	bool IsActive() const
	{
		bool ret = false;
		if (mMenu0.mOpen != 0 || mMenu1.mOpen != 0) {
			ret = true;
		}
		return ret;
	}

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
	u8 _000[0x8];   // _000
	Vec mAngle;     // _008
	u8 _014[0x97C]; // _014
};

// the robot as the menus see it
struct UnkMenuOwner : public XObj {
	u8 _980[0x8];                 // _980
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
	u8 _A68[0xAC4 - 0xA68];       // _A68
	s32 _AC4;                     // _AC4
	u8 _AC8[0xB14 - 0xAC8];       // _AC8
	s32 _B14;                     // _B14
	u8 _B18[0xB68 - 0xB18];       // _B18
	s32 _B68;                     // _B68
	u8 _B6C[0xBAC - 0xB6C];       // _B6C
	s32 _BAC;                     // _BAC
	u8 _BB0[0xBE8 - 0xBB0];       // _BB0
	UnkChrSub _BE8;               // _BE8
	s32 _BEC;                     // _BEC
	u8 _BF0[0x4];                 // _BF0
	s32 _BF4;                     // _BF4
	u8 _BF8[0xC44 - 0xBF8];       // _BF8
	s32 _C44;                     // _C44
	u8 _C48[0xC8C - 0xC48];       // _C48
	s32 _C8C;                     // _C8C
	u8 _C90[0xCC4 - 0xC90];       // _C90
	s32 _CC4;                     // _CC4
	u8 _CC8[0x943C - 0xCC8];      // _CC8
	s32 _943C;                    // _943C
	u8 _9440[0x955C - 0x9440];    // _9440
	CJointRot mJointRots[2];      // _955C
	u8 _959C[0x95D0 - 0x959C];    // _959C
	s32 _95D0;                    // _95D0
	s32 _95D4;                    // _95D4
	s32 _95D8;                    // _95D8
	u8 _95DC[0x9658 - 0x95DC];    // _95DC
	f32 _9658;                    // _9658
	u8 _965C[0x9AD0 - 0x965C];    // _965C
	XObj mSub9AD0;                // _9AD0
	u8 _A450[0xB1B0 - 0xA450];    // _A450
	XObj mSubB1B0;                // _B1B0
	u8 _BB30[0x8];                // _BB30
	XObj mSubBB38;                // _BB38
	u8 _C4B8[0x8];                // _C4B8
	XObj mSubC4C0;                // _C4C0
	u8 _CE40[0x18];               // _CE40
	XObj mSubCE58;                // _CE58
	u8 _D7D8[0xC];                // _D7D8
	UnkReq mReq;                  // _D7E4
	u8 _D7F0[0x8];                // _D7F0
	XObj mSubs[9];                // _D7F8
	u8 _12D78[0x12DF8 - 0x12D78]; // _12D78
	f32 mMoveAngle;               // _12DF8
	u8 _12DFC[0x12E44 - 0x12DFC]; // _12DFC
	UnkMenuPair mPair;            // _12E44
	u8 _13390[0x4];               // _13390
	s32 mSpinning;                // _13394
	s32 _13398;                   // _13398
	s32 _1339C;                   // _1339C
	UnkSpin mSpins[2];            // _133A0
	u8 _146C0[0x8];               // _146C0
	XObj* mSpinObj;               // _146C8
	f32 mSpinRotY;                // _146CC
	u8 _146D0[0x158A4 - 0x146D0]; // _146D0
	s32 _158A4;                   // _158A4
	u8 _158A8[0x8];               // _158A8
	f32 _158B0;                   // _158B0
	s32 _158B4;                   // _158B4
	u8 _158B8[0x1C];              // _158B8
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

// one of the ten things the robot can have going at once
struct UnkPoolObj {
	u8 _000[0xC];   // _000
	s32 _00C;       // _00C
	u8 _010[0x64];  // _010
	s32 _074;       // _074
	u8 _078[0x5A4]; // _078

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
void fn_80095AD8(UnkPoolObj* obj);
}

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

extern "C" {
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
	u8 _00[0xB4]; // _00
	f32 mPitch;   // _B4

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

static const GXColor lbl_8065D4AC = { 0x00, 0x30, 0x30, 0x30 };

// ---- functions ----

extern "C" {

void fn_80093BD8(OctaMenu* menu)
{
	menu->vf0C(0);
}

void fn_80093C08(s32* value)
{
	*value = 0;
}

const GXColor* fn_8009AFD4(void)
{
	return &lbl_8065D4AC;
}

BOOL fn_800984CC(UnkMenu* menu, s32 i)
{
	return menu->mOwner->mReq.Is(menu->mEntries[i].mId);
}

void fn_800970E4(HSD_JObj* jobj, UnkColorArgs* args, u32 type)
{
	GXColor diffuse  = *args->mDiffuse;
	GXColor specular = *args->mSpecular;
	f32 alpha        = *args->mAlpha;
	for (HSD_DObj* dobj = HSD_JObjGetDObj(jobj); dobj != NULL; dobj = HSD_DObjGetNext(dobj)) {
		HSD_MObj* mobj = HSD_DObjGetMObj(dobj);
		if (mobj != NULL) {
			HSD_MObjSetDiffuseColor(mobj, diffuse.r, diffuse.g, diffuse.b);
			fn_801BA3AC(mobj, specular.r, specular.g, specular.b);
			HSD_MObjSetAlpha(mobj, alpha);
		}
	}
}

void fn_800971C4(HSD_JObj* jobj)
{
	HSD_JObjModifyFlags(jobj, 0x20080000, 0x701C0000);
	for (HSD_DObj* dobj = HSD_JObjGetDObj(jobj); dobj != NULL; dobj = HSD_DObjGetNext(dobj)) {
		HSD_DObjModifyFlags(dobj, 4, 0xE);
		HSD_MObj* mobj = HSD_DObjGetMObj(dobj);
		if (mobj != NULL) {
			HSD_MObjSetFlags(mobj, 0x60000000);
			HSD_MObjClearFlags(mobj, 8);
		}
		if (dobj != NULL) {
			HSD_MObjCompileTev(dobj->mobj);
		}
	}
}

void fn_80097284(UnkMenuPair* pair, s32 dir)
{
	if (dir == 0) {
		fn_8001E904(GAME_E7E24, 0x86, 0xFF);
		if (pair->mMenu0._280 != 3) {
			pair->_544 = pair->_548;
		}
	} else if (dir > 0) {
		fn_8001E904(GAME_E7E24, 0x88, 0xFF);
	} else if (dir < 0) {
		fn_8001E904(GAME_E7E24, 0x86, 0xFF);
		if (pair->mMenu0._280 != 3) {
			pair->_544 = pair->_548;
		}
	}
	pair->mMenu0.vf10(dir);
	pair->mMenu1.vf10(dir);
}

const GXColor* fn_800983B8(UnkMenu* menu, s32 i)
{
	if (menu->mEntries[i].mCount > 0) {
		if (fn_800984CC(menu, i)) {
			return fn_8006DBF0(13);
		} else {
			return fn_8006E034(menu->mEntries[i].mId);
		}
	}
	return fn_8006DBF0(14);
}

s32 fn_8009841C(UnkMenu* menu, s32 i)
{
	s32 ids[9] = { 0x7E, 0x62, 0x64, 0x63, 0x65, 0x7E, 0x66, 0x67, 0x68 };
	if (i == -1) {
		return 0x6B;
	}
	if (i == -2) {
		return 0x6D;
	}
	if (menu->mEntries[i].mCount > 0) {
		return ids[menu->mEntries[i].mId];
	}
	return 0x69;
}

void DrsMenu::vf1C()
{
	char usable[9] = { 0, 1, 1, 1, 1, 0, 1, 1, 1 };
	s32 id         = mEntries[mCursor].mId;
	if (usable[id]) {
		if (mOwner->mReq.Is(id)) {
			mOwner->mReq.Request(0);
		} else {
			mOwner->mReq.Request(id);
		}
	} else {
		mOwner->mReq.Request(0);
	}
}

BOOL fn_80098D9C(UnkMenu* menu, s32 i)
{
	return menu->mOwner->IsRoom(menu->mEntries[i].mId);
}

const GXColor* fn_80098C20(UnkMenu* menu, s32 i)
{
	static const GXColor* color = fn_8009AFD4();
	if (menu->mEntries[i].mCount > 0) {
		if (menu->mOwner->_95D4 < 0 && menu->mOwner->_A48 != 0x14 && menu->mOwner->_A48 != 0x15 && menu->mOwner->_A48 != 0x16) {
			if (fn_80098D9C(menu, i)) {
				color = fn_8006DBF0(12);
			} else {
				color = fn_8006E160(menu->mEntries[i].mId);
			}
		}
	} else {
		color = fn_8006DBF0(14);
	}
	return color;
}

s32 fn_80098CEC(UnkMenu* menu, s32 i)
{
	s32 ids[9] = { 0x7E, 0x5B, 0x5C, 0x5D, 0x5E, 0x60, 0x5F, 0x61, 0x7E };
	if (i == -1) {
		return 0x6A;
	}
	if (i == -2) {
		return 0x6C;
	}
	if (menu->mEntries[i].mCount > 0) {
		return ids[menu->mEntries[i].mId];
	}
	return 0x69;
}

void fn_80098DD4(OctaMenu* menu)
{
	s32 room = menu->mOwner->GetRoom();
	s32 id   = menu->mEntries[menu->mCursor].mId;
	if (id == room) {
		menu->mOwner->RequestRoom(0);
	} else {
		menu->mOwner->RequestRoom(id);
	}
}

s32 fn_800991FC(u8* menu, s32 id)
{
	return fn_8009C1BC((UnkSlots*)(menu + 0x12DFC), id);
}

BOOL fn_8009AFDC(void)
{
	return FALSE;
}

BOOL fn_8009AFE4(void)
{
	return FALSE;
}

void fn_8009BE18(void)
{
}

// turns the joint to an angle and lets go of what it looked at
static inline void TurnTo(CJointRot* rot, f32 angle)
{
	rot->mMode   = 2;
	rot->mTarget = angle;
	rot->mLookAt = NULL;
}

u32 fn_8009AFEC(DrsMenu* menu, s32 i)
{
	s32 r, g, b;
	if (menu->mEntries[i].mCount == 0) {
		b = 0x20;
		g = 0x20;
		r = 0x20;
	} else if (menu->_280 == 3) {
		r = 0x32;
		g = 0x32;
		b = 0xC8;
	} else if (i == menu->mCursor) {
		r = 0xC8;
		g = 0x64;
		b = 0x64;
	} else {
		r = 0x32;
		g = 0x32;
		b = 0xC8;
	}
	if (i == menu->mCursor) {
		r += (s32)(25.0f * (f32)sin(1.2f * menu->mEntries[i].mTime) + 25.0f);
		g += (s32)(25.0f * (f32)sin(1.2f * menu->mEntries[i].mTime) + 25.0f);
		b += (s32)(25.0f * (f32)sin(1.2f * menu->mEntries[i].mTime) + 25.0f);
	}
	return (r << 24) | ((u8)g << 16) | ((u8)b << 8);
}

void fn_8009BE1C(DrsMenu* menu, s32 dir)
{
	if (dir > 0 && menu->mSelected == 0) {
		menu->mOpen = -1;
	} else {
		menu->mOpen = -2;
	}
	menu->_28C      = -1;
	menu->mSelected = 0;
	if (menu->mOwner->_C44 == 0) {
		menu->mOwner->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		menu->mOwner->SetRotateY(menu->_16C, 0.20943952f);
		TurnTo(&menu->mOwner->mJointRots[0], 0.0f);
		TurnTo(&menu->mOwner->mJointRots[1], -1.52f);
	}
}

s32 fn_8009C1BC(UnkSlots* slots, s32 id)
{
	s32 i = slots->Find(id);
	if (i >= 0) {
		fn_8009C398(slots);
		return slots->mSlots[i].mValue;
	}
	return -1;
}

void fn_8009C2C8(UnkSlots* slots, s32 id, s32 value)
{
	s32 i = slots->Find(id);
	if (i >= 0) {
		slots->mSlots[i].mValue = value;
	}
}

void fn_8009CEA0(UnkSlots* slots);

void fn_8009CED4(UnkSlots* slots)
{
	slots->mSlots[0].mId = 1;
	slots->mSlots[1].mId = 2;
	slots->mSlots[2].mId = 3;
	slots->mSlots[3].mId = 4;
	slots->mSlots[4].mId = 5;
	slots->mSlots[5].mId = 6;
	slots->mSlots[6].mId = 7;
	slots->mSlots[7].mId = 8;
	fn_8009CEA0(slots);
}

// after its caller, which doesn't inline it
void fn_8009CEA0(UnkSlots* slots)
{
	fn_8009C8E0(slots);
	fn_8009C398(slots);
}

// the nearest point of a held object still under the robot's reach, want stops the search
void fn_8009D600(UnkGlow* glow, u32 frames)
{
	if (glow->mOn) {
		if (glow->mAlpha < 1.0f) {
			if ((glow->mAlpha += 0.055555556f * frames) >= 1.0f) {
				glow->mAlpha = 1.0f;
			}
			glow->mPulse = glow->mAlpha;
		}
	} else {
		if (glow->mAlpha > 0.0f) {
			if ((glow->mAlpha -= 0.055555556f * frames) <= 0.0f) {
				glow->mAlpha = 0.0f;
			}
		}
	}
	if (glow->mAlpha > 0.0f) {
		glow->mAngle = fmod(glow->mAngle + frames * 6, 360.0);
		if (glow->mAlpha >= 1.0f) {
			if ((glow->mPulse += 0.055555556f * frames) >= 2.0f) {
				glow->mPulse = fmod(glow->mPulse, 2.0);
			}
		}
	}
}

s32 fn_8009DC84(UnkMenuOwner* chr, s32 i, s32 want)
{
	s32 found = -1;
	if (chr->mHeld[i] >= 0) {
		XObj* obj = GAME_OBJS[chr->mHeld[i]];
		f32 best  = NAN;
		for (int j = 0; obj->mPoints[j].mKind != -1; j++) {
			if (obj->mPointMasks[i] & (1 << j)) {
				if ((fpclassify(best) == FP_NAN || obj->mPointDist[j] >= best) && obj->mPointDist[j] <= 1.0f + chr->mMoveTranslate.y) {
					best  = obj->mPointDist[j];
					found = j;
					if (j == want) {
						break;
					}
				}
			}
		}
	}
	return found;
}

BOOL fn_8009FC60(const CXObj* self, const UnkActor* obj)
{
	if (GetId(obj) == 9) {
		return fn_800E4AAC((CXObj*)obj, &self->mMoveTranslate);
	}
	return TRUE;
}

BOOL fn_8009FEDC(const UnkMenuOwner* obj)
{
	bool ret = false;
	if (obj->_9658 <= 0.0f && fn_800665E0(GAME_LIST)) {
		ret = true;
	}
	return ret;
}

// the robot is in a pose where it talks over everything
static inline bool IsChrTalking(const UnkMenuOwner* chr)
{
	bool ret = false;
	if (fn_8009FEDC(chr)) {
		if (chr->_158A4 != 0) {
			ret = true;
		}
	}
	return ret;
}

static inline bool IsChrTalkingNow(const UnkMenuOwner* chr)
{
	bool ret = false;
	if ((GAME_FLAGS & 0x40000000) && IsChrTalking(chr)) {
		ret = true;
	}
	return ret;
}

static inline bool IsChrSpeaking(const UnkMenuOwner* chr)
{
	bool ret = false;
	if (chr->_C44 == 4 || chr->_C44 == 5 || chr->_C44 == 6) {
		ret = true;
	}
	return ret;
}

static inline bool IsMuted(const UnkMenuOwner* chr)
{
	bool ret = false;
	if (IsChrTalkingNow(chr) && IsChrSpeaking(chr)) {
		ret = true;
	}
	return ret;
}

BOOL fn_8009FDE4(const UnkMenuOwner* chr)
{
	return IsMuted(chr);
}

void fn_8009FF40(UnkMenuOwner* chr, f32 t)
{
	if (fn_800D4D54(&chr->mTimer)) {
		chr->mTarget->SetColor(1, 0x96, 0x96, 0, 0.0f);
		chr->mTarget->SetColor(1, 0, 0, 0, 0.2f);
	} else if (chr->_A34 <= 0) {
		fn_800A0014(chr, 1, 1, t);
	}
}

BOOL fn_800A0000(UnkChr* chr)
{
	return chr->_A34 > 0;
}

CVec fn_800A0894(const UnkChr* chr, s32 depth, s32* path)
{
	Vec v;
	HSD_JObj* jobj = GetJointByPath(chr->mJObj, path, depth);
	HSD_JObjGetTranslation(jobj, &v);
	return CVec(v);
}

// stops walking and turning and drops what it was carrying around
static inline void StopMove(UnkMenuOwner* owner)
{
	owner->mSpeeds[0].Set(0.0f, 0.0f, 0.0f);
	owner->mSpeeds[1].Set(0.0f, 0.0f, 0.0f);
	owner->mSpeeds[2].Set(0.0f, 0.0f, 0.0f);
	owner->mMoveRotate.Set(owner->mMoveRotate.mTarget);
	if (owner->_BEC != 0) {
		fn_800C5A00(&owner->_BE8);
		owner->_BEC = 0;
		owner->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		owner->_A2C = 3;
	}
}

void fn_800A0694(UnkMenuOwner* chr, const Vec* dir, f32 speed)
{
	StopMove(chr);
	chr->CXObj::SetMoveDir(dir, speed);
	CVec move = chr->mMoveTranslate.mSpeed;
	move.y    = 0.0f;
	if (move.Length() > 0.0f) {
		chr->mMoveAngle = WrapAngle(GetYAngle(chr->mMoveTranslate.mTarget - chr->mMoveTranslate));
	}
}

void fn_800A0014(UnkMenuOwner* chr, BOOL react, BOOL stop, f32 time)
{
	if (!(GAME_FLAGS & 0xC00) && chr->_95D4 < 0 && GAME_BIG->_5D080 == NULL) {
		BOOL idle;
		if (react && ((idle = chr->_A48 == 0) || chr->_A48 == 4) && chr->_AC4 == 0 && chr->_B14 == 0 && chr->_B68 == 0 && chr->_BAC == 0
		    && chr->_BEC == 0 && chr->_C44 == 0 && chr->_C8C == 0 && (chr->_CC4 == 0 || chr->_CC4 == 4 || chr->_CC4 == 8)
		    && chr->_943C == 0) {
			if (idle) {
				chr->SetAnim(0xD4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else {
				chr->SetAnim(0x94, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
			chr->_C44 = 8;
		} else if (stop) {
			if (chr->_AC4 == 5) {
				chr->_AC4 = 8;
			} else if (chr->_B68 == 5) {
				chr->_B68 = 8;
			} else if (chr->_BAC == 5) {
				chr->_BAC = 7;
			}
		}
	}
	if (0.0f != time) {
		chr->_A34 = 60;
		chr->_A38 = 0;
		chr->_A3C = time;
	}
}

void fn_800A0234(UnkMenuOwner* chr, s32 i, u8 r, u8 g, u8 b, f32 seconds)
{
	if (chr->mReq.mBusy != 0) {
		if (i == 2) {
			i = 1;
		} else if (i == 1) {
			return;
		}
	}
	fn_8008A820(chr, i, r, g, b, seconds);
	chr->mSub9AD0.SetColor(i, r, g, b, seconds);
	chr->mSubB1B0.SetColor(i, r, g, b, seconds);
	chr->mSubBB38.SetColor(i, r, g, b, seconds);
	chr->mSubC4C0.SetColor(i, r, g, b, seconds);
	chr->mSubCE58.SetColor(i, r, g, b, seconds);
	for (int j = 0; j < 9; j++) {
		chr->mSubs[j].SetColor(i, r, g, b, seconds);
	}
	if (fn_800D4D54(&chr->mTimer)) {
		chr->mTarget->SetColor(i, r, g, b, seconds);
	}
}

CVec fn_800A0850(UnkChr* chr)
{
	s32 path[2] = { 0, 0 };
	return fn_800A0894(chr, 2, path);
}

CVec fn_800A0978(UnkChr* chr)
{
	s32 path[2] = { 0, 0 };
	return fn_8007E7F4(chr, 2, path);
}

// a point between the robot's hands, a bit in front of it
Vec fn_800A09BC(UnkChr* chr)
{
	s32 left[8]  = { 0, 1, 0, 1, 2, 0, 0, 0 };
	CVec a       = fn_8007E7F4(chr, 8, left);
	s32 right[8] = { 0, 1, 0, 1, 2, 0, 0, 2 };
	CVec b       = fn_8007E7F4(chr, 8, right);
	Vec mid      = a + (b - a) / 2.0f;
	mid.x -= 1.5f * (f32)sin(((CXObj*)chr)->mMoveRotate.y);
	mid.z -= 1.5f * (f32)cos(((CXObj*)chr)->mMoveRotate.y);
	return mid;
}

int fn_800A2BD0(f32 x)
{
	switch ((*(s32*)&x) & 0x7F800000) {
	case 0x7F800000:
		return ((*(s32*)&x) & 0x7FFFFF) ? FP_NAN : FP_INFINITE;
	case 0:
		return ((*(s32*)&x) & 0x7FFFFF) ? FP_SUBNORMAL : FP_ZERO;
	}
	return FP_NORMAL;
}

BOOL fn_800A2C34(UnkState* state, s32 value)
{
	return value == state->mState;
}

BOOL fn_800A2C70(UnkState* state, s32 value)
{
	return value == state->mState;
}

BOOL fn_800A2C84(UnkState* state, s32 value)
{
	return value == state->mState;
}

void fn_800A2C98(CVec* vec, const Vec& v)
{
	vec->Set(v);
}

BOOL fn_800A2CB4(UnkState* state, s32 value)
{
	return value == state->mState;
}

void fn_800A2CC8(CVec* vec, const Vec& v)
{
	vec->Set(v);
}

CVec fn_800A2E2C(const CVec& v)
{
	return v;
}

BOOL fn_800A2E54(UnkActor* obj)
{
	return (obj->mFlags & 1) != 0;
}

f32 fn_800A4574(UnkChr* chr)
{
	return GetAnimScale(chr);
}

f32 fn_800A2CE4(const CJObj* obj)
{
	HSD_AObj* aobj = GetAObj(obj->mJObj);
	return aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
}

f32 fn_800A2D88(const CJObj* obj)
{
	HSD_AObj* aobj = GetAObj(obj->mJObj);
	return aobj != NULL ? HSD_AObjGetCurrFrame(aobj) : 0.0f;
}

void fn_800A36E8(UnkMenuOwner* chr, f32 rate, u32 mask)
{
	chr->CXObj::SetAnimRate(rate * GetAnimScale((UnkChr*)chr), mask);
	for (UnkJointAnim* anim = lbl_80226130; anim->mId >= 0; anim++) {
		if (anim->mId == chr->mReq.mState) {
			HSD_JObj* jobj = GetJointByPath(chr->mJObj, anim->mPath, anim->mDepth);
			if (GetAObj(jobj) != NULL) {
				chr->Push(jobj, 0x200, 1.0f);
			}
		}
	}
}

void* fn_800A388C(UnkMenuOwner* chr, s32 id)
{
	bool ret = false;
	if (id >= 3000 && chr->mAnimSets[1].mArchive != NULL) {
		ret = true;
	}
	if ((ret ? id / 3000 : 0) != 0) {
		return chr->mAnimSets[1].GetShapeAnimJoint(id % 3000);
	}
	ret = false;
	if (id >= 2000 && chr->mAnimSets[0].mArchive != NULL) {
		ret = true;
	}
	if ((ret ? id / 2000 : 0) != 0) {
		return chr->mAnimSets[0].GetShapeAnimJoint(id % 2000);
	}
	ret = id >= 1000;
	if (ret) {
		ret = chr->mAnim.mArchive != NULL;
	}
	if ((ret ? id / 1000 : 0) != 0) {
		return chr->mAnim.GetShapeAnimJoint(id % 1000);
	}
	return chr->mArchive.mShapeAnimJoint != NULL ? chr->mArchive.mShapeAnimJoint[id % 1000] : NULL;
}

void* fn_800A3AE0(UnkMenuOwner* chr, s32 id)
{
	bool ret = false;
	if (id >= 3000 && chr->mAnimSets[1].mArchive != NULL) {
		ret = true;
	}
	if ((ret ? id / 3000 : 0) != 0) {
		return chr->mAnimSets[1].GetMatAnimJoint(id % 3000);
	}
	ret = false;
	if (id >= 2000 && chr->mAnimSets[0].mArchive != NULL) {
		ret = true;
	}
	if ((ret ? id / 2000 : 0) != 0) {
		return chr->mAnimSets[0].GetMatAnimJoint(id % 2000);
	}
	ret = id >= 1000;
	if (ret) {
		ret = chr->mAnim.mArchive != NULL;
	}
	if ((ret ? id / 1000 : 0) != 0) {
		return chr->mAnim.GetMatAnimJoint(id % 1000);
	}
	return chr->mArchive.mMatAnimJoint != NULL ? chr->mArchive.mMatAnimJoint[id % 1000] : NULL;
}

void* fn_800A3D34(UnkMenuOwner* chr, s32 id)
{
	bool ret = false;
	if (id >= 3000 && chr->mAnimSets[1].mArchive != NULL) {
		ret = true;
	}
	if ((ret ? id / 3000 : 0) != 0) {
		return chr->mAnimSets[1].GetAnimJoint(id % 3000);
	}
	ret = false;
	if (id >= 2000 && chr->mAnimSets[0].mArchive != NULL) {
		ret = true;
	}
	if ((ret ? id / 2000 : 0) != 0) {
		return chr->mAnimSets[0].GetAnimJoint(id % 2000);
	}
	ret = id >= 1000;
	if (ret) {
		ret = chr->mAnim.mArchive != NULL;
	}
	if ((ret ? id / 1000 : 0) != 0) {
		return chr->mAnim.GetAnimJoint(id % 1000);
	}
	return chr->mArchive.mAnimJoint != NULL ? chr->mArchive.mAnimJoint[id % 1000] : NULL;
}

void fn_800A45B8(UnkMenuOwner* chr, CShadow* shadow)
{
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mJObj);
	if (GAME_FLAGS & 0x0040000000000000ULL) {
		HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mSub9AD0.mJObj);
	}
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mSubB1B0.mJObj);
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mSubBB38.mJObj);
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mSubC4C0.mJObj);
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)chr->mSubCE58.mJObj);
}

void fn_800A54FC(CXObjIntp* obj, u32 speed)
{
	obj->CXObjIntp::UpdateAnim(speed);
}

void fn_800A77D4(UnkMenuOwner* chr)
{
	if (GAME_CAMERA->GetMapDistance() < 1.0f) {
		return;
	}
	if (chr->mFlags & 4) {
		if (GAME_FLAGS & 0x0040000000000000ULL) {
			chr->mSub9AD0.vfB8();
		}
	} else {
		fn_80090864(chr);
	}
}

void fn_800A7894(UnkMenuOwner* chr, MtxPtr vmtx)
{
	if (GAME_CAMERA->GetMapDistance() < 1.0f) {
		return;
	}
	if (GAME_FLAGS & 0x100) {
		fn_800A30EC(chr, 0);
	}
	chr->Draw(vmtx);
	if (GAME_FLAGS & 0x100) {
		fn_800A30EC(chr, 1);
	}
	if (GAME_FLAGS & 0x0040000000000000ULL) {
		chr->mSub9AD0.DrawSub(vmtx);
	}
	chr->mSubB1B0.DrawSub(vmtx);
	chr->mSubBB38.DrawSub(vmtx);
	chr->mSubC4C0.DrawSub(vmtx);
	chr->mSubCE58.DrawSub(vmtx);
}

void fn_800A7A2C(UnkMenuOwner* chr, MtxPtr vmtx)
{
	CMotionControlCamera* cam = GAME_CAMERA;
	if (cam->GetMapDistance() < 1.0f) {
		return;
	}
	fn_800908C0(chr, vmtx);
	if (!((GAME_FLAGS & 0x100) && chr->_A48 == 4 && fn_800D4FE8((UnkChrSub*)&chr->mTimer)) && (GAME_FLAGS & 0x0040000000000000ULL)) {
		chr->mSub9AD0.Draw(vmtx);
	}
	chr->mSubB1B0.Draw(vmtx);
	if (GAME_FLAGS & 0x100) {
		Mtx rot;
		Mtx m;
		PSMTXRotRad(rot, 'x', -cam->mPitch);
		if (vmtx != NULL) {
			PSMTXConcat(rot, vmtx, m);
		} else {
			PSMTXConcat(rot, GAME_VIEW->GetCamera()->mViewMtx, m);
		}
		chr->mSubBB38.Draw(m);
	} else {
		chr->mSubBB38.Draw(vmtx);
	}
	chr->mSubC4C0.Draw(vmtx);
	chr->mSubCE58.Draw(vmtx);
}

void fn_800A7C44(UnkChr* chr, MtxPtr vmtx)
{
	if (chr->_984 != 0) {
		fn_800908C0(chr, vmtx);
	}
}

BOOL fn_800A8454(void* self, void* a, const UnkRangeTest* b)
{
	return fn_8001E6C8(GAME_E7E24, b->_14) ? -1 : 0;
}

bool fn_800A8494(void* self, void* a, const UnkRangeTest* b)
{
	return !fn_8001E6C8(GAME_E7E24, b->_14);
}

s32 fn_800A84D0(void* self, Vec* pos, const UnkRangeTest* b)
{
	if (pos->x >= b->mMin && pos->x < b->mMax) {
		if (!fn_8001E6C8(GAME_E7E24, b->_14)) {
			return 1;
		}
	} else {
		if (fn_8001E6C8(GAME_E7E24, b->_14)) {
			return -1;
		}
	}
	return 0;
}

s32 fn_800A8560(const u32* flags, void* a, const UnkRangeTest* b)
{
	if (*flags & 0x44000000) {
		if (fn_8001E6C8(GAME_E7E24, b->_14)) {
			return -1;
		}
	} else {
		if (!fn_8001E6C8(GAME_E7E24, b->_14)) {
			return 1;
		}
	}
	return 0;
}

BOOL fn_800A85DC(void* self, const UnkRange* range, const Vec* pos)
{
	if (((range->mMin < pos->y || range->mMin > range->mMax) && range->mMax >= pos->y)
	    || ((range->mMin < pos->z || range->mMin > range->mMax) && range->mMax >= pos->z)) {
		return TRUE;
	}
	return FALSE;
}

BOOL fn_800A8640(void* self, const UnkRange* range, const Vec* pos)
{
	if ((range->mMin < pos->y || range->mMin > range->mMax) && range->mMax >= pos->y) {
		return TRUE;
	}
	return FALSE;
}

void fn_800B1B0C(UnkLink* link, s32 value)
{
	link->_24 = value;
}

void fn_800B1B14(UnkState* state, s32 value)
{
	state->mState = value;
}

void fn_800B1B1C(UnkState* state, s32 value)
{
	state->mState = value;
}

BOOL fn_800B1B24(const UnkChr* chr)
{
	return IsTimerDone(chr);
}

void fn_800B1B70(UnkLink* link, UnkHolder* hold)
{
	link->mHold   = hold;
	link->mHoldId = link->mHold->mId;
}

f32 fn_800B1B84(void)
{
	return fn_8006E9AC(2) / 100.0f;
}

f32 fn_800B1BCC(void)
{
	return fn_8006E9AC(1) / 100.0f;
}

f32 fn_800B1C14(void)
{
	return fn_8006E9AC(0) / 100.0f;
}

f32 fn_800B1C5C(const UnkPadStatus* pad)
{
	f32 x = pad->_34;
	f32 y = pad->_38;
	return sqrtf(x * x + y * y);
}

BOOL fn_800B1D50(UnkActor* obj)
{
	return (obj->mFlags & 0x400000000ULL) != 0;
}

UnkChrSub* fn_800B1D84(UnkChr* chr)
{
	return &chr->mSubB4;
}

void fn_800B1DDC(UnkState* state, s32 value)
{
	state->mState = value;
}

void fn_800B1DE4(s32* value)
{
	*value = 1;
}

u32 fn_800B1DF0(UnkChr* chr, u32 mask)
{
	return chr->mFlags & mask;
}

u32 fn_800B1DFC(UnkChr* chr)
{
	return chr->mFlags;
}

BOOL fn_800B1E04(void)
{
	return FALSE;
}

BOOL fn_800B1E0C(void* self, const Vec& pos, CXObj* obj)
{
	f32 angle = GetAngle(pos, obj->mMoveTranslate);
	angle     = WrapAngle(angle - obj->mMoveRotate.y);
	return angle >= 2.0943952f && angle <= 4.1887903f;
}

f32 fn_800B1EB8(UnkChr* chr)
{
	return chr->_77C;
}

s32 fn_800B2004(UnkChr* chr)
{
	return chr->mId;
}

BOOL fn_800B200C(void)
{
	return FALSE;
}

BOOL fn_800B2014(UnkActor* obj)
{
	return (obj->mFlags & 0x8000000000ULL) != 0;
}

BOOL fn_800B2048(UnkActor* obj)
{
	return (obj->mFlags & 0x100000000ULL) != 0;
}

void fn_800B207C(s32* value)
{
	*value = 0;
}

BOOL fn_800B2088(UnkTimer* timer)
{
	return timer->mTime > 0.0f;
}

s32 fn_800B20A0(UnkMap* map, s32 list)
{
	return map->mLists[list].mCount;
}

UnkStageObj* fn_800B20B4(UnkStageTable* table, s32 i)
{
	return &table->mObjs[i];
}

UnkChrSub* fn_800B20C4(UnkActor* obj)
{
	return &obj->mSub238;
}

f32 fn_800B20CC(const Vec& from, const Vec& to)
{
	return GetAngle(from, to);
}

void* fn_800B2108(UnkMap* map, s32 list, s32 i)
{
	return map->mLists[list].mData + i * map->mLists[list].mStride;
}

s32 fn_800B2128(UnkMap* map)
{
	return map->_A860;
}

f32 fn_800B2134(UnkChr* chr)
{
	return chr->_780;
}

void fn_800B213C(UnkState* state, s32 value)
{
	state->mState = value;
}

void fn_800B2144(UnkChr* chr, s32 state, UnkHolder* hold)
{
	chr->mStateBAC = state;
	if (hold != NULL) {
		chr->mHoldBC8   = hold;
		chr->mHoldIdBD0 = chr->mHoldBC8->mId;
	}
}

BOOL fn_800B2164(UnkActor* obj)
{
	return (obj->mFlags & 0x1000000000ULL) != 0;
}

void fn_800B2198(UnkChr* chr, s32 state, UnkHolder* hold)
{
	chr->mStateB68 = state;
	if (hold != NULL) {
		chr->mHoldB84   = hold;
		chr->mHoldIdB8C = chr->mHoldB84->mId;
	}
}

BOOL fn_800B21B8(UnkActor* obj)
{
	return (obj->mFlags & 0x800000000ULL) != 0;
}

void fn_800B21EC(UnkChr* chr)
{
	if (chr->mTarget != NULL) {
		chr->mTarget->mFlags &= ~0x0400000000000000ULL;
		chr->mTarget = NULL;
	}
	chr->mTimerState = 0;
}

void fn_800B2230(UnkChr* chr, s32 state, UnkHolder* hold)
{
	chr->mStateAC4 = state;
	if (hold != NULL) {
		chr->mHoldAE0   = hold;
		chr->mHoldIdAE8 = chr->mHoldAE0->mId;
	}
}

BOOL fn_800B2250(UnkActor* obj)
{
	return (obj->mFlags & 0x200000000ULL) != 0;
}

u32 fn_800B2284(UnkChr* chr, s32 i)
{
	return chr->_25C[i];
}

BOOL fn_800B2294(UnkActor* obj)
{
	return (obj->mFlags & 0x2000000000ULL) != 0;
}

void fn_800B22C8(UnkState* state, s32 value)
{
	state->mState = value;
}

BOOL fn_800B22D0(UnkPair* pair)
{
	return pair->_000 != 0 || pair->_298 != 0;
}

f32 fn_800B22F8(void)
{
	return fn_8006EAF0(2) / 100.0f;
}

f32 fn_800B2340(void)
{
	return fn_8006EAF0(1) / 100.0f;
}

void fn_800B2388(CObjVec* vec, f32 y)
{
	vec->mTarget.y = y;
}

BOOL fn_800B2390(void)
{
	return TRUE;
}

f32 fn_800B2398(f32 x)
{
	return sqrtf(x);
}

f32 fn_800B247C(const UnkPlaceTable& table, s32 i)
{
	return DEG2RAD * table.mEntries[i].mRotY;
}

s32 fn_800B24BC(UnkLink* link)
{
	return link->mHoldId;
}

UnkChrSub* fn_800B24C4(UnkChr* chr)
{
	return &chr->mSub24C;
}

CObjVec* fn_800B24CC(CXObj* obj)
{
	return &obj->mMoveScale;
}

CObjVec* fn_800B24D4(CXObj* obj)
{
	return &obj->mMoveRotate;
}

s16 fn_800B24DC(UnkActor* obj)
{
	return GetId(obj);
}

s32 fn_800B2504(UnkMap* map, s32 room)
{
	return map->mRooms[room]._1800;
}

void fn_800B2518(UnkActor* obj, u64 mask)
{
	obj->mFlags &= ~mask;
}

u32 fn_800B253C(UnkChrState* state, u32 mask)
{
	return state->mFlags & mask;
}

s16 fn_800B2548(UnkMap* map, s32 room, s32 i)
{
	return map->mRooms[room]._1000[i];
}

void fn_800B2564(UnkActor* obj, u64 mask)
{
	obj->mFlags |= mask;
}

f32 fn_800B2580(UnkChr* chr)
{
	return chr->_6FC[chr->mIndex];
}

f32 fn_800B2594(UnkChr* chr)
{
	return chr->_6F8;
}

f32 fn_800B259C(const Vec& a, const Vec& b)
{
	f32 dx = a.x - b.x;
	f32 dz = a.z - b.z;
	return sqrtf(dx * dx + dz * dz);
}

BOOL fn_800B26A0(UnkLink* link, s32 value)
{
	return link->mKind == value;
}

s32 fn_800B26B4(CXObj* obj)
{
	return obj->mId;
}

u64 fn_800B26BC(UnkActor* obj, u64 mask)
{
	return obj->mFlags & mask;
}

s32 fn_800B26D0(UnkMap* map, s32 room, s32 i)
{
	return map->mRooms[room]._0000[i];
}

void fn_800B26EC(void* self, HSD_JObj* jobj, Vec* scale)
{
	HSD_JObjGetScale(jobj, scale);
}

HSD_JObj* fn_800B2764(CXObj* obj, s32 depth, s32* path)
{
	return GetJointByPath(obj->mJObj, path, depth);
}

Vec fn_800B27E0(const UnkPlaceTable& table, s32 i)
{
	Vec pos = { table.mEntries[i].mPos[0] / 100.0f, table.mEntries[i].mPos[1] / 100.0f, table.mEntries[i].mPos[2] / 100.0f };
	return pos;
}

Vec fn_800B289C(const UnkPlaceTable& table, s32 i)
{
	Vec pos = { table.mEntries[i].mPos2[0] / 100.0f, table.mEntries[i].mPos2[1] / 100.0f, table.mEntries[i].mPos2[2] / 100.0f };
	return pos;
}

void fn_800B2958(CObjVec* vec, const Vec& target)
{
	vec->SetTarget(target);
}

void fn_800B2974(CVec* vec, f32 x, f32 y, f32 z)
{
	vec->Set(x, y, z);
}

void fn_800B2984(CObjVec* vec, f32 x, f32 y, f32 z)
{
	vec->mSpeed.Set(x, y, z);
}

void fn_800B2994(CVec* vec, f32 scale)
{
	PSVECScale(vec, vec, scale);
}

BOOL fn_800B29B8(void)
{
	return TRUE;
}

BOOL fn_800B29C0(void)
{
	return TRUE;
}

BOOL fn_800B29C8(UnkState* state)
{
	return state->mState == 0 || state->mState == 6 || state->mState == 9;
}

BOOL fn_800B29F4(void)
{
	return TRUE;
}

BOOL fn_800B29FC(void)
{
	return TRUE;
}

BOOL fn_800B2A04(UnkState* state)
{
	return state->mState == 0 || state->mState == 10;
}

BOOL fn_800B2A28(UnkState* state)
{
	return state->mState == 0 || state->mState == 11;
}

BOOL fn_800B2A4C(UnkState* state)
{
	return state->mState == 0 || state->mState == 4;
}

BOOL fn_800B2A70(UnkState* state)
{
	return state->mState == 0;
}

BOOL fn_800B2A80(void)
{
	return TRUE;
}

CVec* fn_800B2A88(CVec* vec)
{
	return &vec->Normalize();
}

// what a floor search found
struct UnkFloorHit {
	s32 mTri;    // _00
	s32 mTrigon; // _04
	f32 mY;      // _08
};

// XObjMap as far as the floor search goes, see game/obja_map.cpp
struct UnkMapObj {
	u8 _000[0x980];       // _000
	s32 mTrigonCount;     // _980
	s32 mOrder[16];       // _984
	CTrigon mTrigons[16]; // _9C4

	// the first trigon by height that reaches up to y
	s32 FindStart(f32 y) const
	{
		for (int i = 0; i < mTrigonCount; i++) {
			if (mTrigons[mOrder[i]].mMinY <= y) {
				return i;
			}
		}
		return 0;
	}
};

BOOL fn_800B2ABC(const UnkMapObj* map, const Vec* pos, UnkFloorHit* hit, Vec* tri)
{
	s32 start = map->FindStart(0.2f + pos->y);
	for (int i = 0; i < map->mTrigonCount; i++) {
		s32 trigon = map->mOrder[(start + i) % map->mTrigonCount];
		int found  = ((CTrigon*)&map->mTrigons[trigon])->Check((Vec*)pos, &hit->mY, tri);
		if (found >= 0) {
			hit->mTrigon = trigon;
			hit->mTri    = found;
			return TRUE;
		}
	}
	return FALSE;
}

void fn_800B2BB0(UnkChr* chr, s32 value, BOOL reset)
{
	chr->_C44 = value;
	if (reset) {
		chr->_C6C = 1;
		chr->_C7C = 0.0f;
	}
}

void fn_800B2BD0(UnkChr* chr, BOOL reset)
{
	chr->_D3C = 0;
	if (reset) {
		s32 path[] = { 0 };
		HSD_JObjSetRotationX(GetJointByPath(chr->mJObj, path, 1), 0.0f);
	}
}

void* fn_800B2CFC(UnkBig* big)
{
	return &big->_E7E24;
}

u32 fn_800B2D08(const UnkGame* game, u32 mask)
{
	return CheckStageFlag(game->mStage, mask);
}

BOOL fn_800B2D2C(UnkState* state, s32 value)
{
	return value == state->mState;
}

BOOL fn_800B2D40(UnkState* state, s32 value)
{
	return value == state->mState;
}

UnkChrSub* fn_800B2D54(UnkChr* chr)
{
	return &chr->mSub9EC;
}

BOOL fn_800B2D5C(void)
{
	return TRUE;
}

BOOL fn_800B2D64(void)
{
	return TRUE;
}

BOOL fn_800B2D6C(UnkState* state)
{
	return state->mState == 0 || state->mState == 3 || state->mState == 4 || state->mState == 7 || state->mState == 8 || state->mState == 5
	    || state->mState == 6 || state->mState == 9;
}

BOOL fn_800B2DC0(UnkState* state)
{
	return state->mState != 8 && state->mState != 9;
}

BOOL fn_800B2DE4(UnkState* state)
{
	return state->mState == 0 || state->mState == 9 || state->mState == 10 || state->mState == 11;
}

BOOL fn_800B2E18(UnkState* state)
{
	return state->mState == 0 || state->mState == 10 || state->mState == 11;
}

BOOL fn_800B2E44(UnkState* state)
{
	return state->mState == 0 || state->mState == 2 || state->mState == 3 || state->mState == 4 || state->mState == 17;
}

BOOL fn_800B2E80(UnkState* state)
{
	return state->mState == 0 || state->mState == 10;
}

s32 fn_800B2EA4(UnkMap* map)
{
	return map->_8580;
}

s32 fn_800B2EB0(UnkChr* chr)
{
	return chr->_D3C;
}

void* fn_800B2EB8(UnkMap* map, s32 i)
{
	return map->mLists[3].mData + i * map->mLists[3].mStride;
}

void fn_800B2ED0(UnkTimer* timer, f32 time)
{
	timer->mTime = time;
}

void fn_800B2ED8(CObjVec* vec, const Vec& v)
{
	SetNow(vec, v.x, v.y, v.z);
}

u32 fn_800B2F00(UnkState* state, u32 mask)
{
	return state->mState & mask;
}

UnkChrSlot* fn_800B2F0C(UnkChr* chr, u8 i)
{
	UnkChrSlot* slots = chr->mSlots;
	return &slots[i];
}

void fn_800B2F20(CObjVec* vec, f32 z)
{
	vec->z         = z;
	vec->mTarget.z = z;
}

void fn_800B2F2C(CObjVec* vec, f32 x)
{
	vec->x         = x;
	vec->mTarget.x = x;
}

BOOL fn_800B2F38(UnkState* state, s32 value)
{
	return value == state->mState;
}

BOOL fn_800B2F4C(UnkMap* map, s32 value)
{
	return value == (map->_95D4 >= 0 ? map->_95D4 : map->_95D0);
}

BOOL fn_800B2F74(UnkBig* big)
{
	return big->_5D080 != 0;
}

BOOL fn_800B2F8C(UnkState* state, s32 value)
{
	return value == state->mState;
}

f32 fn_800B2FA0(Vec* a, Vec* b)
{
	return PSVECDistance(a, b);
}

void fn_800B2FC0(void)
{
}

u64 fn_800B2FC4(UnkChr* chr, u64 mask)
{
	return chr->mFlags2 & mask;
}

void fn_800B2FD8(CObjVec* vec, f32 y)
{
	vec->y         = y;
	vec->mTarget.y = y;
}

s32 fn_800B2FE4(CXObj* obj)
{
	return obj->mAnimId;
}

void fn_800B2FEC(CObjVec* vec, f32 x, f32 y, f32 z)
{
	SetNow(vec, x, y, z);
}

void fn_800B3008(CVec* vec, f32 z)
{
	vec->z = z;
}

void fn_800B3010(CVec* vec, f32 x)
{
	vec->x = x;
}

f32 fn_800B3018(f32 angle)
{
	return WrapAngle(angle);
}

s32 fn_800B304C(UnkLink* link)
{
	return link->mKind;
}

f32 fn_800B3054(UnkLink* link)
{
	return link->_48;
}

UnkChrSub* fn_800B305C(UnkLink* link)
{
	return &link->mSub3C;
}

BOOL fn_800B3064(UnkLink* link, s32 kind)
{
	return link->mHold->mPoints[link->mHoldId].mKind == kind;
}

void fn_800B3088(CObjVec* vec, f32 z)
{
	vec->mSpeed.z = z;
}

void fn_800B3090(CObjVec* vec, f32 x)
{
	vec->mSpeed.x = x;
}

CVec* fn_800B3098(CObjVec* vec)
{
	return &vec->mSpeed;
}

f32 fn_800B30A0(CVec* vec)
{
	return vec->Length();
}

CVec* fn_800B30C0(CObjVec* vec)
{
	return &vec->mTarget;
}

void fn_800B30C8(CObjVec* vec, f32 z)
{
	vec->mTarget.z = z;
}

void fn_800B30D0(CObjVec* vec, f32 x)
{
	vec->mTarget.x = x;
}

f32 fn_800B30D8(f32 x)
{
	return cos(x);
}

f32 fn_800B30FC(UnkChr* chr)
{
	return chr->_A40;
}

f32 fn_800B3104(f32 x)
{
	return sin(x);
}

UnkHolder* fn_800B3128(UnkLink* link)
{
	return link->mHold;
}

BOOL fn_800B3130(UnkState* state, s32 value)
{
	return value == state->mState;
}

UnkChrSub* fn_800B3144(UnkChr* chr)
{
	return &chr->mSub264;
}

CObjVec* fn_800B314C(CXObj* obj)
{
	return &obj->mMoveTranslate;
}

u32 fn_800B3154(UnkMap* map, s32 i)
{
	return map->_9860[i];
}

BOOL fn_800B3168(UnkLink* link)
{
	return link->mHold != NULL;
}

BOOL fn_800B317C(UnkState* state, s32 value)
{
	return value == state->mState;
}

u32 fn_800B31AC(UnkChrState* state, u32 mask)
{
	return state->mFlags & mask;
}

bool fn_800B4F28(const CVec* vec, Vec v)
{
	return vec->x != v.x || vec->y != v.y || vec->z != v.z;
}

Vec fn_800B4F68(void* self, HSD_JObj* jobj)
{
	Vec v;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(jobj), &v);
	return v;
}

void fn_800B501C(UnkState* state, s32 value)
{
	state->mState = value;
}

void fn_800B5024(UnkState* state, s32 value)
{
	state->mState = value;
}

void fn_800B502C(UnkState* state, s32 value)
{
	state->mState = value;
}

void fn_800B5034(UnkState* state, s32 value)
{
	state->mState = value;
}

void fn_800B503C(UnkLink* link, s32 value)
{
	link->_24 = value;
}

void fn_800B5044(UnkState* state, s32 value)
{
	state->mState = value;
}

BOOL fn_800B517C(UnkChr* chr)
{
	bool ret = false;
	if ((chr->mFlags2 & 1) && !(chr->mFlags2 & 0x22) && !chr->IsBusy()) {
		ret = true;
	}
	return ret;
}

void fn_800B556C(UnkMenuOwner* owner)
{
	owner->RequestRoom(0);
}

void fn_800B504C(UnkBig* game, s32 pos, void* obj, s16 a, s16 b)
{
	if (!IsMuted(game->mChr)) {
		fn_8007EEA4(game->mSound, pos, obj, a, b);
	}
}

void fn_800B51FC(UnkMenuOwner* chr, s32 a, s32 b)
{
	if (chr->mSpinning == 0) {
		chr->mSpinning = 1;
		chr->_13398    = a;
		chr->mSpinRotY = chr->mSpinObj->mMoveRotate.y;
		chr->_1339C    = b;
		for (int i = 0; i < 2; i++) {
			chr->mSpins[i].mAngle.x = DEG2RAD * (rand() % 360);
			chr->mSpins[i].mAngle.y = DEG2RAD * (rand() % 360);
			chr->mSpins[i].mAngle.z = 0.5f;
		}
		fn_8001E904(GAME_E7E24, 0x86, 0xFF);
		chr->mSpinObj->SetAnim(0x5D, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		GAME_FLAGS |= 0x800;
	}
}

void fn_800B53A0(UnkMenuOwner* owner, s32 dir)
{
	fn_80097284(&owner->mPair, dir);
}

void fn_800B558C(UnkMenuOwner* owner);

// opens the two menus, the one picked last gets the cursor
static inline void OpenMenus(UnkMenuOwner* owner)
{
	GAME_FLAGS |= 0x400;
	fn_8001E904(GAME_E7E24, 0x86, 0xFF);
	if (owner->mPair._544 == 0) {
		owner->mPair.mMenu0.vf0C(0);
		owner->mPair.mMenu1.vf0C(1);
	} else {
		owner->mPair.mMenu0.vf0C(1);
		owner->mPair.mMenu1.vf0C(0);
	}
}

void fn_800B54A4(UnkMenuOwner* owner, s32 sel)
{
	if (sel >= 0) {
		owner->mPair._548 = owner->mPair._544;
		owner->mPair._544 = sel;
	}
	if (owner->mPair.IsOpen()) {
		if (owner->mPair._544 == 0) {
			owner->mPair.mMenu0.Select(FALSE);
			owner->mPair.mMenu1.Select(TRUE);
		} else {
			owner->mPair.mMenu0.Select(TRUE);
			owner->mPair.mMenu1.Select(FALSE);
		}
		fn_8001E904(GAME_E7E24, 0x86, 0xFF);
	} else {
		fn_800B558C(owner);
	}
}

void fn_800B558C(UnkMenuOwner* owner)
{
	if (owner->_95D4 != -1) {
		return;
	}
	if (owner->mReq.mBusy != 0) {
		return;
	}
	if (!owner->mPair.IsActive()) {
		StopMove(owner);
		OpenMenus(owner);
	} else if (owner->mPair.IsClosing()) {
		OpenMenus(owner);
	}
}

void fn_800B580C(UnkMenuOwner* owner, s32 on, u8 mode)
{
	fn_80091834(owner, on, mode);
	if (owner->mSub9AD0.mJObj != NULL) {
		owner->mSub9AD0.vfDC(on, mode);
	}
	if (owner->mSubB1B0.mJObj != NULL) {
		owner->mSubB1B0.vfDC(on, mode);
	}
	if (owner->mSubBB38.mJObj != NULL) {
		owner->mSubBB38.vfDC(on, mode);
	}
	if (owner->mSubC4C0.mJObj != NULL) {
		owner->mSubC4C0.vfDC(on, mode);
	}
	if (owner->mSubCE58.mJObj != NULL) {
		owner->mSubCE58.vfDC(on, mode);
	}
	for (int i = 0; i < 9; i++) {
		if (owner->mSubs[i].mJObj != NULL) {
			owner->mSubs[i].vfDC(on, mode);
		}
	}
}

void fn_800B5808(void)
{
}

void fn_800B5974(UnkMenuOwner* owner, void* args)
{
	fn_80091C44(owner, args);
	if (owner->mSub9AD0.mJObj != NULL) {
		owner->mSub9AD0.vfD8(args);
	}
	if (owner->mSubB1B0.mJObj != NULL) {
		owner->mSubB1B0.vfD8(args);
	}
	if (owner->mSubBB38.mJObj != NULL) {
		owner->mSubBB38.vfD8(args);
	}
	if (owner->mSubC4C0.mJObj != NULL) {
		owner->mSubC4C0.vfD8(args);
	}
	if (owner->mSubCE58.mJObj != NULL) {
		owner->mSubCE58.vfD8(args);
	}
	for (int i = 0; i < 9; i++) {
		if (owner->mSubs[i].mJObj != NULL) {
			owner->mSubs[i].vfD8(args);
		}
	}
}

void fn_800B5AB8(UnkMenuOwner* owner, s32 mode)
{
	fn_8008B0D8(owner, mode);
	owner->mSub9AD0.vf120(mode);
	owner->mSubB1B0.vf120(mode);
	owner->mSubBB38.vf120(mode);
	owner->mSubC4C0.vf120(mode);
	owner->mSubCE58.vf120(mode);
}

void fn_800B5B7C(void)
{
	for (int i = 0; i < 10; i++) {
		if (lbl_80378300[i].IsFree()) {
			for (int j = 0; j < 10; j++) {
				if (lbl_8037C018[j] == NULL) {
					lbl_8037C018[j] = &lbl_80378300[i];
					fn_80095AD8(lbl_8037C018[j]);
					return;
				}
			}
			return;
		}
	}
}

// the robot is in one of its work poses
static inline bool IsWorking(const UnkMenuOwner* chr)
{
	return chr->_BAC == 1 || chr->_BAC == 2 || chr->_BAC == 3 || chr->_BAC == 4 || chr->_BAC == 5 || chr->_BAC == 6 || chr->_BAC == 7
	    || chr->_BAC == 8;
}

// eases the robot's lean towards what it is doing
// NONMATCHING: the dol tests the work poses one by one, here they become a range check
void fn_800B5C3C(UnkMenuOwner* chr, u32 frames)
{
	f32 target, speed;
	if (chr->_158B4 == 0 && GAME_BIG->_5D080 == 0) {
		chr->_158B4 = 1;
	}
	if (chr->_BEC == 1 || chr->_BEC == 2) {
		if (chr->_BF4 == 1) {
			target = 8.0f;
		} else if (chr->_BF4 == -1) {
			target = -5.0f;
		} else {
			target = 0.0f;
		}
		speed = 0.4f;
	} else if (GAME_BIG->_5D080 != 0 && chr->_158B4 != 0) {
		target = 0.0f;
		speed  = 0.2f;
	} else if (IsWorking(chr)) {
		target = 0.0f;
		speed  = 0.2f;
	} else {
		target = 5.0f;
		speed  = 0.2f;
	}
	if (fpclassify(target) != FP_NAN) {
		if (chr->_158B0 > target) {
			if ((chr->_158B0 -= speed * frames) < target) {
				chr->_158B0 = target;
			}
		} else if (chr->_158B0 < target) {
			if ((chr->_158B0 += speed * frames) > target) {
				chr->_158B0 = target;
			}
		}
	}
}

void fn_800B5E1C(u8* obj)
{
	*(f32*)(obj + 0x158B0) = 5.0f;
	*(s32*)(obj + 0x158B4) = 0;
}
} // extern "C"
