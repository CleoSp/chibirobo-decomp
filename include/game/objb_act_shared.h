#ifndef _GAME_OBJB_ACT_SHARED_H
#define _GAME_OBJB_ACT_SHARED_H

#include "game/hsd_jobj.h"
#include "game/cparticle.h"
#include "game/objb.h"
#include "game/hsd_dobj.h"
#include "game/gmath.h"
extern "C" {
#include <sysdolphin/mtx_transform.h>
}

// the robot's moves, the XAct classes on XAction, and the game helpers they use

class XActHang;
class XActLift;
class XActCopter;
class XActLadder;
class XActJump;

extern "C" {
void* fn_8006EDB4(s32 id);
void fn_80023EE0(void* obj, s32 a, s32 b, s32 c, s32 d, s32 r, s32 g, s32 bl, s32 e, f32 f);
BOOL fn_8009FEDC(XObjChr* chr);
void fn_800D7AC8(XObjCopter* obj);
void fn_800D7A78(XObjCopter* obj);
void fn_800D7B68(XObjCopter* obj);
void fn_800D7BF0(XObjCopter* obj);
s32 fn_8006EAF0(s32 id); // another of the game's numbers
CVec fn_800C35F0(XObjChr* chr);
CVec fn_8007E7C8(XObj* obj, s32 depth, s32* path);    // where a joint of the object is
CVec fn_800BAD80(XObjChr* chr, s32 depth, s32* path); // game/xobjchrstop.cpp
f32 fn_800A2CE4(const CJObj* obj);                    // the end frame of the anim
HSD_JObj* fn_800B2764(CXObj* obj, s32 depth, s32* path);
void fn_800BB3D0(CJObj* obj, s32 depth, s32* path, f32 x);
void fn_800B2BD0(XObjChrPart* part, BOOL reset);
u32 CheckStageFlag(s32 stage, u32 mask);
Vec fn_800D5190(XAction* act); // where the held point of the target is
void fn_800C7810(CJObj* obj, s32 depth, s32* path, const Vec& pos);
void fn_800C9B74(HSD_JObj* jobj, f32 y);
void fn_800C9C34(XObj* obj);
void fn_800C2DB0(XActCopter* act, u32 frames, f32 speed);
void fn_800C9D54(XActJump* act, f32 fall);
void fn_800CDBA8(XActHang* act);
BOOL fn_800DC754(UnkSlotTable* table, s16 id, s16 n);
BOOL fn_800DC9B0(UnkSlotTable* table, s16 id, s16 n, BOOL save);
void fn_800136B4(void* obj, f32 a, f32 b);
void fn_80024CA0(CPtclSet* ptcl);
void fn_80024E0C(CPtclSet* ptcl);
void fn_80024FAC(CPtclSet* ptcl, CPtclOne* ones, s32 size, s32 count);
void fn_801988EC(void* gs, s32 type, MtxPtr view, void* vtx, u16 count);
void PSMTXRotRad(Mtx mtx, char axis, f32 angle);
s32 fn_8006EBC8(s32 id); // one of the game's numbers
CVec fn_8008C274(XObj* obj, s32 point, Vec offset);
f32 fn_8008DF34(XObj* obj, const Vec* pos, s32 point, s32 a, s32 b, s32 c); // the angle to face a point of obj from pos
void fn_800C21A0(XActPopperShot* shot);
void fn_80015300(UnkGame* game, const Vec& pos, f32* x, f32* y, f32* z, void* camera); // where pos is on the screen
void fn_8019B56C(void* mode, s32 type, s32 src, s32 dst, s32 op);
void fn_80198258(void* gs, u32 segments, u32 centerColor, u32 edgeColor, f32 x, f32 y, f32 z, f32 w, f32 h, f32 rx, f32 ry, f32 rz);
void fn_8009F4F8(CJObj* obj, s32 depth, const s32* path, f32 x, f32 y, f32 z); // puts a joint at a translation
}

// __vt__9XObjFlags, keeps 4096 bits of flags, game/xobjflagsclear.cpp has its methods
class XObjFlags : public XObj {
public:
	u32* mBits; // _980

	virtual s32 GetFlagNo(); // _148

	void SetBit(u32 i) { mBits[i >> 5] |= 1 << (i & 31); }

	// the robot takes it, it goes in the robot's counts and is marked as taken
	void Take(XObjChr* chr)
	{
		fn_800DC9B0(&chr->mStats.mTable, GetKind(), 1, TRUE);
		SetBit(GetFlagNo());
	}
};

class XActLift : public XAction {
public:
	s32 mPoint; // _28, the point of the carried thing the robot holds it by
	f32 _2C;    // _2C
};

class XActJump : public XAction {
public:
	u8 _28[0x20]; // _28
	f32 _48;      // _48
	f32 mFall;    // _4C, how far the robot has fallen
};

class XActPush : public XAction {
public:
	s32 mPoint; // _28, the point of the pushed thing the robot pushes at
};

class XActLadder : public XAction {
public:
	s32 mPoint; // _28
	BOOL _2C;   // _2C, let go at the next rung
	Vec mPos;   // _30, where the robot hangs, the ladder's place
	f32 mDist;  // _3C, how far the robot still has to go to get there
	f32 mAngle; // _40
};

// a += b, b by value like the other CVec operators
static inline void AddVec(CVec& a, Vec b)
{
	PSVECAdd(&a, &b, &a);
}

class XActCopter : public XAction {
public:
	u8 _28[0x4]; // _28
	f32 mTime;   // _2C, how long the robot can stay up
	f32 mPhase;  // _30
	f32 mBob;    // _34
};

class XActRope : public XAction {
public:
};

// how far the stick is pushed
static inline f32 StickLength(CPadOne* pad)
{
	f32 x = pad->mStickX;
	f32 y = pad->mStickY;
	return sqrtf(x * x + y * y);
}

class XActHang : public XAction {
public:
	s32 mPoint; // _28
	BOOL mDrop; // _2C, the stick points away from the wall
};

// a value that fades in and out
struct UnkFade {
	s32 mOn;     // _00
	Vec mOffset; // _04
	f32 mValue;  // _10
};

// one corner of a drawn primitive, game/cprimitive.h has it
struct PrimVertex {
	Vec pos;   // _00
	u32 color; // _0C
};

// a shadow sits just above the floor
static inline Vec ShadowPos(const Vec& pos, const Vec& offset)
{
	Vec v = { pos.x + offset.x, 0.01f + pos.y, pos.z + offset.z };
	return v;
}

// the move whose functions start at fn_800BE648
class XActE : public XAction {
public:
	u8 _28[0x4];  // _28
	void* _2C;    // _2C
	Vec _30;      // _30
	Vec _3C;      // _3C
	u8 _48[0x34]; // _48
	u8 _7C[0x34]; // _7C
	f32 _B0;      // _B0
	f32 _B4;      // _B4
	f32 _B8;      // _B8
	f32 _BC;      // _BC
};

// a value with an upper limit
struct UnkLimited {
	u8 _00[0xC]; // _00
	f32 mValue;  // _0C
	f32 mMax;    // _10
};

// three colour bytes
struct UnkColorOut {
	u8 _00[0x12]; // _00
	u8 mR;        // _12
	u8 mG;        // _13
	u8 mB;        // _14
};

// the popper's particles
struct UnkPopperFx {
	s32 _00;              // _00
	f32 _04;              // _04
	u8 _08[0xC];          // _08
	HSD_Generator* mGen0; // _14
	HSD_Generator* mGen1; // _18
	HSD_JObj* mJObj;      // _1C
};

// the popper's shots, ten of them
class XActPopper : public XAction {
public:
	u8 _28[0x8];               // _28
	XActPopperShot mShots[10]; // _30
	UnkPopperFx mFx;           // _8758
};

class XActB : public XAction {
public:
	u8 _28[0x1C]; // _28
	s32 _44;      // _44
};

class XActC : public XAction {
public:
	u8 _28[0x4];  // _28
	s32 _2C;      // _2C
	u8 _30[0x24]; // _30
	s32 _54;      // _54
};

class XActD : public XAction {
public:
	u8 _28[0x4];  // _28
	f32 _2C;      // _2C
	u8 _30[0x18]; // _30
	s16 _48;      // _48
	u8 _4A[0x16]; // _4A
	s32 _60;      // _60
	u8 _64[0x8];  // _64
	s32 _6C;      // _6C
	u8 _70[0x4];  // _70
	s32 _74;      // _74
};

// a held thing of the given id and kind
static inline bool IsHolding(XAction* act)
{
	bool result = false;
	if (act->mTarget != NULL && (act->mTarget->mFlags & 0x400000000000ULL)) {
		result = true;
	}
	return result;
}

static inline bool IsHoldingId(XAction* act, s16 id)
{
	bool result = false;
	if (IsHolding(act) && act->mTarget->GetId() == id) {
		result = true;
	}
	return result;
}

static inline bool IsHoldingKind(XAction* act, s16 id, s16 kind)
{
	bool result = false;
	if (IsHoldingId(act, id) && act->mTarget->GetKind() == kind) {
		result = true;
	}
	return result;
}

// holding the thing and not throwing it
static inline bool IsCarrying(XAction* act, s16 kind)
{
	bool result = false;
	if (IsHoldingKind(act, 8, kind) && act->_24 != 15) {
		result = true;
	}
	return result;
}

// lets go of a held thing and stops
static inline void StopAct(XAction* act)
{
	XObj* target = act->mTarget;
	if (target != NULL && (target->mFlags & 0x0800000000000000ULL)) {
		target->mFlags &= ~0x0800000000000000ULL;
	}
	act->Reset();
}

static inline bool IsGameBusy()
{
	bool result = false;
	if ((lbl_8025C400._331C != 0 || lbl_8025C400._3320 != 0) && lbl_8025C400._3324 == 0) {
		result = true;
	}
	return result;
}

static inline bool CanGameAct()
{
	bool result = false;
	if ((lbl_8025C400.mFlags & 1) && !(lbl_8025C400.mFlags & 0x22) && !IsGameBusy()) {
		result = true;
	}
	return result;
}

static inline bool IsTargetId(XAction* act, s16 id)
{
	bool result = false;
	if (act->mTarget != NULL && act->mTarget->GetId() == id) {
		result = true;
	}
	return result;
}

static inline bool IsStill(XAction* act)
{
	bool result = false;
	if (act->mState == 0x1F || act->mState == 0x20 || act->mState == 0x21) {
		result = true;
	}
	return result;
}

static inline bool IsCarryingStill(XAction* act, s16 kind)
{
	bool result = false;
	if (IsCarrying(act, kind) && IsStill(act)) {
		result = true;
	}
	return result;
}

// puts a CObjVec at a value with its target there too, and stops it
static inline void SetStill(CObjVec* vec, f32 x, f32 y, f32 z)
{
	vec->x         = x;
	vec->y         = y;
	vec->z         = z;
	vec->mTarget.x = x;
	vec->mTarget.y = y;
	vec->mTarget.z = z;
	vec->mSpeed.x  = 0.0f;
	vec->mSpeed.y  = 0.0f;
	vec->mSpeed.z  = 0.0f;
}

static inline void KillGen(HSD_Generator*& gen)
{
	if (gen != NULL) {
		lbl_8025C400.mParticle.Kill(gen);
		gen = NULL;
	}
}

static inline void ResetShot(XActPopperShot* shot)
{
	shot->_000 = 0;
	shot->_038 = 0;
	fn_80024E0C(&shot->mPtcl);
	shot->mGlows[0].mFade = 0.0f;
	shot->mGlows[1].mFade = 0.0f;
	KillGen(shot->mPtcl.mGens[0]);
	KillGen(shot->mPtcl.mGens[1]);
}

// game/msg.cpp has the out of line copy, here it is inlined
inline CJObj::CJObj(HSD_JObj* jobj)
{
	Attach(jobj);
}

// the jobj's place in the world
static inline Vec GetJObjPos(HSD_JObj* jobj)
{
	Vec v;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(jobj), &v);
	return v;
}

// the joint at the end of the path, one child step then path[i] sibling steps per level
static inline HSD_JObj* FindJoint(HSD_JObj* jobj, s32 depth, s32* path)
{
	s32 i;
	s32 j;

	if (jobj == NULL) {
		return NULL;
	}
	for (i = 0; i < depth; i++, path++) {
		jobj = jobj == NULL ? NULL : jobj->child;
		for (j = 0; j < *path; j++) {
			jobj = jobj == NULL ? NULL : jobj->next;
			if (jobj == NULL) {
				return NULL;
			}
		}
	}
	return jobj;
}

#endif
