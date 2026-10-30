#ifndef _GAME_OBJB_H
#define _GAME_OBJB_H

#include "game/cobj.h"
#include "game/xobjs.h"
#include "game/cparticle.h"
extern "C" {
#include <dolphin/mtx/mtx.h>
}
extern "C" {
#include <dolphin/mtx/vec.h>
}
#include "game/cvec.h"

// the second half of the XObj classes, game/objb*.cpp: the robot's own XObjChr code, the
// attachments it carries (XObjAttachment, XObjCopter, XObjPopper, XObjRadar, XObjOX), the
// plug and its wire, and the free, dust, hit box, coin and item objects. class names from
// RTTI and vtable symbols, nothing else has a name, so the functions are extern "C" under
// their address. offsets are usa.
//
// XObj is game/obja.h's, copied here with more of the padding named. don't include both.

extern "C" {
void HSD_JObjSetFlagsAll(HSD_JObj* jobj, u32 flags);
void HSD_JObjClearFlagsAll(HSD_JObj* jobj, u32 flags);
}

// what the object was placed from
struct XObjSource {
	u8 _00[0x4];           // _00
	CObjArchive* mArchive; // _04
};

struct XObjInfo {
	XObjSource* mSource; // _00
	u8 _04[0x10];        // _04
	s16 mId;             // _14
	s16 mKind;           // _16

	static s16 GetId(const XObjInfo* info)
	{
		if (info != NULL) {
			return info->mId;
		}
		return 0;
	}

	static s16 GetKind(const XObjInfo* info)
	{
		if (info != NULL) {
			return info->mKind;
		}
		return 0;
	}
};

// what an object is spawned from, the CoordInfo points at it
struct XObjSpawn {
	u8 _00[0x2C]; // _00
	s32 mCount;   // _2C
};

// what kind of object it is
struct XObjIdent {
	s32 mId; // _00
};

// what the object plays its anims from
struct XObjAnimSet {
	u8 _00[0x30]; // _00
	s32 mIdle;    // _30
};

struct XObjEvt {
	u8 _00[0x4];  // _00
	void* mEvt;   // _04
	u8 _08[0x60]; // _08
};

struct XObjMsg {
	s32 mId; // _00
	s32 _04; // _04
};

struct XObjColor {
	Vec mColor;  // _00
	Vec mTarget; // _0C
	Vec mSpeed;  // _18
};

// one of the object's points, the robot walks to them
struct XObjSpot {
	Vec mPos;     // _00
	u8 _0C[0x18]; // _0C
};

// a CJArchive that counts the reads it has going, __vt__6CJAnim
class CJAnim : public CJArchive {
public:
	s32 mReading; // _80

	virtual void Clear();   // _0C
	virtual void Release(); // _18
};

// __vt__4XObj
class XObj : public CXObjIntp {
public:
	u64 mFlags;            // _228
	u8 _230[0x8];          // _230
	XObjAnimSet* mAnimSet; // _238
	u8 _23C[0x4];          // _23C
	XObjInfo* mInfo;       // _240
	XObjIdent* mIdent;     // _244
	u8 _248[0x18];         // _248
	s32 mCount;            // _260
	Vec mLastPos;          // _264
	u8 _270[0x8];          // _270
	XObjSpot mSpots[32];   // _278
	u8 _6F8[0x4];          // _6F8
	f32 mPointDist[32];    // _6FC
	f32 _77C;              // _77C
	f32 mNearDist;         // _780
	s32 mNearPoint;        // _784
	s32 _788;              // _788
	s32 _78C;              // _78C
	f32 _790;              // _790
	f32 _794;              // _794
	f32 _798;              // _798
	f32 _79C;              // _79C
	u8 _7A0[0x4];          // _7A0
	XObjEvt mEvt;          // _7A4
	s32 _80C;              // _80C
	s32 mMsgCount;         // _810
	XObjMsg mMsgs[16];     // _814
	f32 _894;              // _894
	f32 _898;              // _898
	f32 _89C;              // _89C
	u8 _8A0[0x10];         // _8A0
	u32 _8B0;              // _8B0
	XObjColor mColors[2];  // _8B4
	CJAnim mAnim;          // _8FC

	virtual void Init(u32 id);                                   // _90
	virtual void vf94();                                         // _94
	virtual void Reset();                                        // _98
	virtual BOOL vf9C(const CoordInfo& info);                    // _9C
	virtual void vfA0();                                         // _A0
	virtual void Update(u32 frames);                             // _A4
	virtual void vfA8();                                         // _A8
	virtual void vfAC();                                         // _AC
	virtual void vfB0(u32 frames);                               // _B0
	virtual void vfB4(u32 frames);                               // _B4
	virtual void vfB8();                                         // _B8
	virtual void vfBC(Vec* pos, f32 scale);                      // _BC
	virtual void vfC0(f32 frames);                               // _C0
	virtual void vfC4();                                         // _C4
	virtual void vfC8();                                         // _C8
	virtual void vfCC();                                         // _CC
	virtual void vfD0();                                         // _D0
	virtual void vfD4();                                         // _D4
	virtual void vfD8(void* args);                               // _D8
	virtual void vfDC(s32 on, u8 mode);                          // _DC
	virtual void vfE0(f32 alpha);                                // _E0
	virtual void vfE4(f32 alpha);                                // _E4
	virtual void vfE8();                                         // _E8
	virtual void vfEC();                                         // _EC
	virtual void vfF0();                                         // _F0
	virtual void vfF4();                                         // _F4
	virtual void vfF8();                                         // _F8
	virtual void vfFC();                                         // _FC
	virtual void vf100();                                        // _100
	virtual void vf104();                                        // _104
	virtual void vf108();                                        // _108
	virtual void vf10C();                                        // _10C
	virtual void vf110();                                        // _110
	virtual void vf114();                                        // _114
	virtual void vf118();                                        // _118
	virtual void vf11C();                                        // _11C
	virtual void vf120(s32 mode);                                // _120
	virtual void vf124();                                        // _124
	virtual s32 GetSoundPos();                                   // _128
	virtual void SetColor(s32 i, u8 r, u8 g, u8 b, f32 seconds); // _12C
	virtual void vf130();                                        // _130
	virtual BOOL vf134();                                        // _134
	virtual void vf138(f32 dist);                                // _138
	virtual BOOL vf13C(s32 a, s32 b);                            // _13C
	virtual BOOL vf140(s32 a, s32 b);                            // _140
	virtual void vf144();                                        // _144

	BOOL IsFlag(u64 mask) const { return mFlags & mask; }
	void SetFlag(u64 mask) { mFlags |= mask; }

	s32 GetObjId() const
	{
		if (mIdent != NULL) {
			return mIdent->mId;
		}
		return -1;
	}

	s16 GetId() const
	{
		if (mInfo != NULL) {
			return XObjInfo::GetId(mInfo);
		}
		return -1;
	}

	s16 GetKind() const
	{
		if (mInfo != NULL) {
			return XObjInfo::GetKind(mInfo);
		}
		return -1;
	}
};
typedef char XObj_size_check[sizeof(XObj) == 0x980 ? 1 : -1];

// the XObj methods the classes on it call directly, the XObj vtable has them
extern "C" {
XObj* fn_8000877C(XObj* obj, s16 del);              // ~XObj
void fn_800911C0(XObj* obj);                        // _54 Release
void fn_800929C4(XObj* obj, u32 id);                // _90 Init
void fn_800928E8(XObj* obj);                        // _98 Reset
BOOL fn_80091C80(XObj* obj, const CoordInfo& info); // _9C, loads the model
void fn_80090ED4(XObj* obj, u32 frames);            // _A4 Update
void fn_800908C0(XObj* obj, u32 frames);            // _B0
void fn_8009056C(XObj* obj, Vec* pos, f32 scale);   // _BC
void fn_8008A6C0(XObj* obj, f32 dist);              // _138
void fn_80091834(XObj* obj, s32 on, u8 mode);       // _DC
}

extern "C" void* GetSpawnData(s32 index);

// ---- the classes that the robot holds, their destructors are in game/objb.cpp ----

// __vt__14XObjAttachment, a thing the robot carries
class XObjAttachment : public XObj {
public:
	HSD_JObj* mOwner; // _980, the joint it hangs from
	s32 mShown;       // _984

	virtual void Show();                    // _148
	virtual void Hide();                    // _14C
	virtual void SetOwner(HSD_JObj* owner); // _150

	void PlayAnim(s32 id, BOOL force) { SetAnim(id, force, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f); }
};

// __vt__6XObjOX
class XObjOX : public XObjAttachment {
public:
};

// __vt__9XObjRadar
class XObjRadar : public XObjAttachment {
public:
	s32 mState; // _988
	f32 _98C;   // _98C
	f32 _990;   // _990
};

// __vt__10XObjPopper
class XObjPopper : public XObjAttachment {
public:
};

// __vt__10XObjCopter
class XObjCopter : public XObjAttachment {
public:
};

// __vt__11XObjWireEff, the sparks along the plug's wire
class XObjWireEff : public XObj {
public:
	s32 mAnim;           // _980
	f32 mRate;           // _984
	s32 mEffect;         // _988
	HSD_Generator* mGen; // _98C
};

// the interpolations, game/cgame.cpp has their methods
class CInterfaceInterpolation {
public:
	// _00 vtable

	virtual ~CInterfaceInterpolation() { }              // _08
	virtual bool Start(f32 from, f32 to, f32 time) = 0; // _0C
	virtual bool Step(f32 dt)                      = 0; // _10
	virtual bool Update(u32 step, f32 dt)          = 0; // _14
	virtual f32 Get()                              = 0; // _18
};

class CInterpolationEaseInOut : public CInterfaceInterpolation {
public:
	u8 _04[0x18]; // _04

	virtual bool Start(f32 from, f32 to, f32 time);
	virtual bool Step(f32 dt);
	virtual bool Update(u32 step, f32 dt);
	virtual f32 Get();
};

// one point of the plug's wire
struct XObjPlugNode {
	u8 _00[0x4];                 // _00
	CVec _04;                    // _04
	CInterpolationEaseInOut _14; // _14
	u8 _30[0x4];                 // _30
	CVec _34;                    // _34
	u8 _44[0x8];                 // _44
};

struct XObjPlugNodes {
	XObjPlugNode mNodes[10]; // _00
};

// one point of a wire path, the paths share a static pool of them
struct XObjPathPoint {
	CVec mPos;   // _00
	CVec mSpeed; // _10
	CVec _20;    // _20
};

// the plug's wire, a chain of points from the plug to the robot
struct XObjPlugWire {
	s32 mCount;             // _00
	XObjPathPoint* mPoints; // _04
	f32 _08;                // _08
	Vec mEnds[3];           // _0C
	Vec* mTargets[3];       // _30
	f32 _3C;                // _3C
	f32 _40;                // _40
	u8 _44[0x4];            // _44
	s32 _48;                // _48
	s32 _4C;                // _4C
	u8 _50[0x4];            // _50
	u8 _54;                 // _54
	f32 _58;                // _58
	u32 mColors[2];         // _5C
	u8 mMode;               // _64
	u32 mFlags;             // _68
	CVec _6C;               // _6C
	u8 _7C[0x10];           // _7C
	XObjPlugNodes mNodes;   // _8C
	u8 _384[0x3C];          // _384
};

// __vt__8XObjPlug
class XObjPlug : public XObj {
public:
	XObjPlugWire mWire;   // _980
	s32 mLoadState;       // _D40
	XObjWireEff mWireEff; // _D48
	s32 _16D8;            // _16D8
	s32 _16DC;            // _16DC
};

class XObjChr;

// one controller port, game/cpad.h has it but clashes with game/ut.h's CPad
class CPadOne {
public:
	u8 mPort;            // _00
	u32 mButton;         // _04
	u32 mDirButton;      // _08
	u32 mLastButton;     // _0C
	u32 mLastDirButton;  // _10
	u32 mTrigger;        // _14
	u32 mDirTrigger;     // _18
	u32 mRepeat;         // _1C
	u32 mDirRepeat;      // _20
	s32 mRepeatTimer[4]; // _24
	f32 mStickX;         // _34
	f32 mStickY;         // _38
	f32 mSubStickX;      // _3C
	f32 mSubStickY;      // _40
	f32 mAnalogL;        // _44
	f32 mAnalogR;        // _48
	u32 mConnected;      // _4C
	// _50 vtable

	virtual ~CPadOne() { } // _08
};

// one move the robot can make, the XAct classes are on it. Init and Reset are in
// game/objb.cpp
class XAction {
public:
	XObjChr* mChr; // _00
	s32 mState;    // _04
	// _08 vtable

	virtual void Init(XObjChr* chr); // _08
	virtual void Reset();            // _0C

	s32 _0C;       // _0C
	u8 _10[0x10];  // _10
	XObj* mTarget; // _20
	s32 _24;       // _24
};

// __vt__7XObjChr, the robot
class XObjChr : public XObj {
public:
	// FIELDS chr BEGIN
	u32 mStatus;        // _00980
	u8 _00984[0x4];     // _00984
	CVec _988;          // _00988
	CVec _998;          // _00998
	CVec _9A8;          // _009A8
	u8 _009B8[0x74];    // _009B8
	s32 _A2C;           // _00A2C
	u8 _00A30[0x14];    // _00A30
	XAction mActLift;   // _00A44
	u8 _00A6C[0x17C];   // _00A6C
	XAction mActBE8;    // _00BE8
	u8 _00C10[0x34];    // _00C10
	s32 _C44;           // _00C44
	u8 _00C48[0x8988];  // _00C48
	s32 _95D0;          // _095D0
	s32 _95D4;          // _095D4
	u8 _095D8[0x64];    // _095D8
	CVec _963C;         // _0963C
	u8 _0964C[0x444];   // _0964C
	f32 _9A90;          // _09A90
	u8 _09A94[0xD78];   // _09A94
	u32 _A80C;          // _0A80C
	u8 _0A810[0x9A0];   // _0A810
	XObjCopter mCopter; // _0B1B0
	u8 _0BB38[0x72BC];  // _0BB38
	s32 _12DF4;         // _12DF4
	u8 _12DF8[0x2A9C];  // _12DF8
	s16 _15894;         // _15894
	u8 _15896[0x2];     // _15896
	s32 _15898;         // _15898
	s32 _1589C;         // _1589C
	u8 _158A0[0xC];     // _158A0
	u32 _158AC;         // _158AC
	u8 _158B0[0x24];    // _158B0
	CJAnim mAnims[2];   // _158D4
	// FIELDS chr END

	s32 GetMode() const { return _95D4 >= 0 ? _95D4 : _95D0; }
};

// one particle a CPtcl keeps, its destructor is in another file
class CPtclOne {
public:
	u8 _00[0x50]; // _00

	~CPtclOne();
};

// __vt__5CPtcl
class CPtcl {
public:
	u8 _00[0x10]; // _00
	// _10 vtable

	virtual ~CPtcl() { } // _08
};

struct CPtclSet {
	CPtcl mPtcl;             // _000
	CPtclOne mOnes[0x28];    // _014
	HSD_Generator* mGens[2]; // _C94
	u8 _C9C[0x4];            // _C9C
};

// one shot of the popper
struct XActPopperShot {
	s32 _000;       // _000
	CVec _004;      // _004
	u8 _014[0x24];  // _014
	s32 _038;       // _038
	u8 _03C[0x84];  // _03C
	f32 _0C0;       // _0C0
	u8 _0C4[0x14];  // _0C4
	f32 _0D8;       // _0D8
	u8 _0DC[0x8];   // _0DC
	CPtclSet mPtcl; // _0E4
};

// one slot of the status's id table
struct UnkStatusSlot {
	s16 mId;    // _00
	s16 mValue; // _02
};

// counts of things by id, a slot that runs out is taken out when mCompact is set
struct UnkSlotTable {
	UnkStatusSlot mSlots[256]; // _000
	BOOL mCompact;             // _400

	s32 Find(s16 id)
	{
		for (int i = 0; i < 256; i++) {
			if (id == mSlots[i].mId && mSlots[i].mValue > 0) {
				return i;
			}
		}
		return -1;
	}

	void Remove(int i)
	{
		if (mCompact) {
			for (; i < 255; i++) {
				mSlots[i].mId    = mSlots[i + 1].mId;
				mSlots[i].mValue = mSlots[i + 1].mValue;
			}
			mSlots[i].mId    = -1;
			mSlots[i].mValue = 0;
		}
	}
};

// the robot's numbers: happy points, its level, the moolah and so on
struct UnkStatus {
	s32 mHappy;          // _000
	s32 mLevel;          // _004, 0 to 30
	s32 _008;            // _008
	f32 _00C;            // _00C
	f32 _010;            // _010
	s32 _014;            // _014
	s32 _018;            // _018
	s32 _01C[8];         // _01C
	UnkSlotTable mTable; // _03C
};

// the three render objects the game keeps at _290, an id count of 0 means not set
struct UnkRenderA {
	u8 _00[0xC]; // _00
	s32 _0C;     // _0C
	s32 _10;     // _10
	s32 _14;     // _14
	s32 _18;     // _18
};

struct UnkRenderB {
	u8 _00[0x74]; // _00
	s32 _74;      // _74
	s32 _78;      // _78
	s32 _7C;      // _7C
};

struct UnkRenderC {
	u8 _00[0x30]; // _00
	s32 _30;      // _30
};

// the game object behind lbl_8025C400, only the parts these files reach
struct UnkGame {
	// FIELDS game BEGIN
	u8 _00000[0x44];     // _00000
	u8* _44;             // _00044
	u8 _00048[0x248];    // _00048
	UnkRenderA* _290;    // _00290
	UnkRenderB* _294;    // _00294
	UnkRenderC* _298;    // _00298
	u8 _0029C[0x1874];   // _0029C
	u64 mFlags;          // _01B10
	u8 _01B18[0x1804];   // _01B18
	s32 _331C;           // _0331C
	s32 _3320;           // _03320
	s32 _3324;           // _03324
	u8 _03328[0xF84];    // _03328
	u8 _042AC[0x124];    // _042AC
	s32 _43D0;           // _043D0
	u32 _43D4;           // _043D4
	u8 _043D8[0x8];      // _043D8
	f32 _43E0[4];        // _043E0
	u8 _043F0[0x4B48];   // _043F0
	u8 _08F38[0x4];      // _08F38
	u8 _08F3C[0x1D8];    // _08F3C
	CParticle mParticle; // _09114
	u8 _0965C[0x38];     // _0965C
	u8 mObjList[0x28];   // _09694
	XObjChr* mChr;       // _096BC
	u8 _096C0[0x539BC];  // _096C0
	u8 mEvt[0x4];        // _5D07C
	void* _5D080;        // _5D080
	u8 _5D084[0x8ADA0];  // _5D084
	u8 mSound[0x4];      // _E7E24
	                     // FIELDS game END
};
extern "C" UnkGame lbl_8025C400;
#define GAME_SOUND (lbl_8025C400.mSound)

extern "C" {
void fn_8001E660(void* sound, s32 id);             // stops a sound
BOOL fn_8001E6C8(void* sound, s32 id);             // is it playing
void fn_8001E904(void* sound, s32 id, s32 volume); // plays a sound
}

// todo: copied inline, clean up
inline f32 HSD_AObjGetCurrentFrame(HSD_AObj* aobj)
{
	((aobj) ? ((void)0) : __assert("aobj.h", 149, "aobj"));
	return aobj->curr_frame;
}

extern "C" {
HSD_AObj* fn_801908F8(HSD_JObj* jobj);
HSD_AObj* fn_801905D4(HSD_JObj* jobj);
HSD_AObj* fn_801902C4(HSD_JObj* jobj);
}

// frame of the first aobj on the jobj or below it
static inline f32 GetAnimFrame(HSD_JObj* jobj)
{
	HSD_AObj* aobj;

	if (jobj == NULL) {
		aobj = NULL;
	} else if ((aobj = fn_801908F8(jobj)) != NULL) {
	} else if ((aobj = fn_801905D4(jobj)) != NULL) {
	} else if ((aobj = fn_801902C4(jobj)) != NULL) {
	} else {
		aobj = NULL;
	}
	if (aobj != NULL) {
		return HSD_AObjGetCurrentFrame(aobj);
	}
	return 0.0f;
}

// first anim object on the jobj, whichever kind it is
static inline HSD_AObj* GetAObj(HSD_JObj* jobj)
{
	HSD_AObj* aobj;
	if (jobj != NULL) {
		if ((aobj = fn_801908F8(jobj)) != NULL) {
			return aobj;
		}
		if ((aobj = fn_801905D4(jobj)) != NULL) {
			return aobj;
		}
		if ((aobj = fn_801902C4(jobj)) != NULL) {
			return aobj;
		}
	}
	return NULL;
}

// todo: copied inline, clean up
inline void HSD_JObjCopyMtx(HSD_JObj* jobj, Mtx mtx)
{
	JOBJ_H_ASSERT(1173, jobj);
	JOBJ_H_ASSERT(1174, mtx);
	PSMTXCopy(mtx, jobj->mtx);
	jobj->flags |= 0x03800000;
	HSD_JObjSetMtxDirty(jobj);
}

// todo: copied inline, clean up
inline f32 HSD_AObjGetEndFrame(HSD_AObj* aobj)
{
	((aobj) ? ((void)0) : __assert("aobj.h", 171, "aobj"));
	return aobj->end_frame;
}

// todo: copied inline, clean up. hands the matrix back to the srt values
inline void HSD_JObjClearUserMtx(HSD_JObj* jobj)
{
	JOBJ_H_ASSERT(1199, jobj);
	jobj->flags &= ~0x03800000;
	HSD_JObjSetMtxDirty(jobj);
}

// the destructors are weak copies without names, the stand-ins set the vtables by hand
typedef void* (*Dtor)(void* obj, s16 del);
extern "C" void __destroy_arr(void* block, Dtor dtor, u32 size, u32 count);

#endif
