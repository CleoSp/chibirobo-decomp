#include "game/particle_srt.h"
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

// ---- functions ----

extern "C" {

// the lift drop, defined early so the jump can inline it
/**
 * @note Address: 0x800D4998
 * @note Size: 0x1C8
 */
// NONMATCHING: the dol keeps the action in r30 and the target in r31
void fn_800D4998(XActLift* act)
{
	XObjFlags* target;
	XObjChr* chr = act->mChr;
	if (chr->mAnimId == 0x8B1 || chr->mAnimId == 0x8B3) {
		chr->SetAnim(0x8B2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	} else if (chr->mAnimId == 0xB) {
		chr->SetAnim(8, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}
	act->mState      = 0;
	act->mChr->_95D0 = 0;
	act->mTarget->mFlags &= ~0x0200000000000000ULL;
	act->mTarget->SetAnim(7, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
	act->_24 = 0x14;
	target   = (XObjFlags*)act->mTarget;
	fn_800DC9B0(&act->mChr->mStats.mTable, target->GetKind(), 1, TRUE);
	target->SetBit(target->GetFlagNo());
	fn_8001E904(GAME_SOUND, 0x39, 0xFF);
}

/**
 * @note Address: 0x800BE648
 * @note Size: 0xF0
 */
void fn_800BE648(XActE* act, u32 frames)
{
	if (act->mState == 0 && act->_2C != 0 && act->mChr->mActTumble.mState == 0) {
		if (act->_B0 < 1.0f) {
			if ((act->_B0 += 0.05f * frames) > 1.0f) {
				act->_B0 = 1.0f;
			}
		}
		act->_B4 -= 0.08726646f * frames;
		act->_B8 -= 0.08726646f * frames;
	} else if (act->_B0 > 0.0f) {
		if ((act->_B0 -= 0.05f * frames) < 0.0f) {
			act->_B0 = 0.0f;
		}
	}
}

/**
 * @note Address: 0x800BE738
 * @note Size: 0x1F8
 */
// NONMATCHING: the dol keeps the object in r31 and the table pointers in r29 and r30
XObj* fn_800BE738(XActE* act)
{
	XObj* obj;
	CVec pos        = act->mChr->mMoveTranslate;
	XObj* nearest   = NULL;
	f32 nearestDist = NAN;
	for (int i = 0; i < lbl_8025C400.mObjectCount; i++) {
		obj = lbl_8025C400.mObjects[i];
		if ((obj->mFlags & 1) && (obj->mFlags & 0x10000) && !(obj->mFlags & 0x4000000000000000ULL) && (obj->mFlags & 0x80000)
		    && (s32)act->mChr->mId != (s32)obj->mId && obj->GetObjId() < 10000) {
			f32 dist;
			if (nearest == NULL) {
				dist = PSVECDistance(&pos, &obj->mMoveTranslate);
			} else {
				dist = PSVECDistance(&pos, &obj->mMoveTranslate);
				if (!(dist < nearestDist)) {
					dist = NAN;
				}
			}
			if (fpclassify(dist) != FP_NAN) {
				nearest     = obj;
				nearestDist = dist;
			}
		}
	}
	return nearest;
}

/**
 * @note Address: 0x800BFC04
 * @note Size: 0x68
 */
// NONMATCHING: the state goes into r3, the dol keeps it in r0 and tests 3, 4 and 5 one by one
void fn_800BFC04(XAction* act, s32 unused, f32* out)
{
	if (act->mChr->GetMode() == 3) {
		if (act->mState == 1 || act->mState == 2) {
			*out = 0.0f;
		} else if (act->mState == 3 || act->mState == 4 || act->mState == 5) {
			*out = 0.0f;
		}
	}
}

/**
 * @note Address: 0x800BFC6C
 * @note Size: 0xCC
 */
void fn_800BFC6C(XActE* act)
{
	act->Reset();
	act->_2C   = NULL;
	act->_30.x = act->_30.y = act->_30.z = 0.0f;
	act->_3C.x = act->_3C.y = act->_3C.z = 0.0f;
	act->_BC                             = NAN;
	fn_80023EE0(act->_48, 0, 0, 0, 0, 0xFF, 0x19, 0xFF, 0, 0.0f);
	fn_80023EE0(act->_7C, 0, 0, 0, 0, 0xFF, 0x19, 0xFF, 0, 0.0f);
	act->_B0 = 0.0f;
}

/**
 * @note Address: 0x800BFD38
 * @note Size: 0xE0
 */
void fn_800BFD38(XActE* act, XObjChr* chr)
{
	act->Init(chr);
	fn_800BFC6C(act);
}

/**
 * @note Address: 0x800BFE18
 * @note Size: 0x22C
 */
// NONMATCHING: register allocation, the dol puts the loop counters above the pointers
void fn_800BFE18(XActPopper* act)
{
	int i;
	XActPopperShot* shot;
	for (i = 0, shot = act->mShots; i < 10; i++, shot++) {
		for (int j = 0; j < 2; j++) {
			if (shot->mGlows[j].mFade > 0.0f) {
				f32 y;
				f32 x;
				fn_80015300(&lbl_8025C400, shot->mGlows[j].mPos, &x, &y, NULL, NULL);
				u32 color  = shot->mGlows[j].mColor;
				u32 edge   = color & 0xFFFFFF00;
				u32 center = (color & 0xFFFFFF00) | (u8)((color & 0xFF) * shot->mGlows[j].mFade);
				f32 size   = shot->mGlows[j].mSize * shot->mGlows[j].mFade;
				f32 ring   = shot->mGlows[j].mSize * (1.0f - shot->mGlows[j].mFade);
				f32 w;
				fn_8019B56C(lbl_8025C400._290, 1, 2, 4, 15);
				w = 2.0f * size;
				fn_80198258(&lbl_8025C400, 16, (u32)(255.0f * shot->mGlows[j].mFade) | 0xFFFFFF00, 0xFFFFFF00, x, y, 0.0f, w, w, 0.0f, 0.0f,
				            0.0f);
				w = 1.5f * size;
				fn_80198258(&lbl_8025C400, 16, (u32)(128.0f * shot->mGlows[j].mFade) | 0xFFFFFF00, 0xFFFFFF00, x, y, 0.0f, w, w, 0.0f, 0.0f,
				            0.0f);
				fn_8019B56C(lbl_8025C400._290, 1, 4, 5, 15);
				fn_80198258(&lbl_8025C400, 16, edge, center, x, y, 0.0f, ring, ring, 0.0f, 0.0f, 0.0f);
			}
		}
	}
}

/**
 * @note Address: 0x800C0044
 * @note Size: 0x4C
 */
void fn_800C0044(XActPopper* act)
{
	int i;
	XActPopperShot* shot;
	for (i = 0, shot = act->mShots; i < 10; i++, shot++) {
		fn_80024CA0(&shot->mPtcl);
	}
}

/**
 * @note Address: 0x800C159C
 * @note Size: 0x74
 */
CVec fn_800C159C(const CVec& v, f32 d)
{
	CVec t(v);
	t.x += d;
	t.y += d;
	t.z += d;
	return t;
}

/**
 * @note Address: 0x800C1610
 * @note Size: 0x64
 */
// NONMATCHING: same as fn_800BFC04
void fn_800C1610(XAction* act, s32 unused, f32* out)
{
	s32 state = act->mState;
	if (state == 4) {
		*out = 1.0f;
	} else if (state == 6) {
		*out = 1.0f;
	} else if (state == 7 || state == 8 || state == 5) {
		*out = 1.0f;
	} else if (state != 0) {
		*out = 0.0f;
	}
}

/**
 * @note Address: 0x800C2A14
 * @note Size: 0x304
 */
// NONMATCHING: the dol reads the position again for the glow
void fn_800C2A14(XActPopperShot* shot, const Vec& pos, s32 kind, XObj* target, f32 a, f32 b, f32 c, f32 d, f32 e, f32 charge)
{
	shot->_000 = 1;
	shot->_038 = 0;
	shot->_004 = pos;
	shot->_014 = a;
	shot->_018 = b;
	shot->_01C = d;
	shot->_020 = 0.0f;
	shot->_024 = e;
	shot->_028 = c;
	for (int i = 0; i < 10; i++) {
		shot->mTrail[i].x = shot->_004.x;
		shot->mTrail[i].y = shot->_004.y;
		shot->mTrail[i].z = shot->_004.z;
	}
	shot->mGlows[0].Set(pos, 0.9f, 100.0f, 0x00FF0096);
	shot->mCharge = charge;
	shot->mKind   = kind;
	if (target != NULL) {
		shot->mTarget = target;
	} else {
		shot->mTarget = NULL;
	}
	shot->mPtcl.mGens[0] = lbl_8025C400.mParticle.Create((kind == 1 ? 0x4E : 0x4F) | 0x10000, 0, shot->mPtcl.mJObj, TRUE);
	if (charge >= fn_8006EBC8(0x1F) / 100.0f) {
		shot->mPtcl.mGens[1] = lbl_8025C400.mParticle.Create(0x10050, 0, shot->mPtcl.mJObj, TRUE);
	}
	f32 full = fn_8006EBC8(0x1F) / 100.0f;
	fn_8001E904(GAME_SOUND, charge >= full ? (kind == 1 ? 0x193 : 0x196) : (kind == 1 ? 0xB : 0x195), 0xFF);
	fn_800C21A0(shot);
}

/**
 * @note Address: 0x800C2D18
 * @note Size: 0x98
 */
void fn_800C2D18(XActPopperShot* shot)
{
	ResetShot(shot);
}

/**
 * @note Address: 0x800C1674
 * @note Size: 0x148
 */
// NONMATCHING: the dol keeps the particle manager in r29 and the counter in r31
void fn_800C1674(XActPopper* act, XObjChr* chr)
{
	act->Init(chr);
	XActPopperShot* shot;
	int i;
	for (i = 0, shot = act->mShots; i < 10; shot++, i++) {
		fn_800C2D18(shot);
		fn_80024FAC(&shot->mPtcl, shot->mPtcl.mOnes, 0x50, 0x28);
		shot->mGlows[0].mFade = 0.0f;
		shot->mGlows[1].mFade = 0.0f;
		shot->mPtcl.mGens[0]  = NULL;
		shot->mPtcl.mGens[1]  = NULL;
		if (shot->mPtcl.mJObj == NULL) {
			shot->mPtcl.mJObj = HSD_JObjAlloc();
		}
	}
	act->mFx._00   = 0;
	act->mFx.mGen1 = NULL;
	act->mFx.mGen0 = NULL;
	if (act->mFx.mJObj == NULL) {
		act->mFx.mJObj = HSD_JObjAlloc();
	}
}

/**
 * @note Address: 0x800C17BC
 * @note Size: 0xB0
 */
void fn_800C17BC(UnkPopperFx* fx)
{
	fx->_00 = 0;
	fx->_04 = 0.0f;
	KillGen(fx->mGen0);
	KillGen(fx->mGen1);
	fn_8001E660(GAME_SOUND, 0x186);
	fn_8001E660(GAME_SOUND, 0x18B);
}

// the copter update, defined early so that it calls the bob instead of inlining it
/**
 * @note Address: 0x800C2EB8
 * @note Size: 0x738
 */
void fn_800C2EB8(XActCopter* act, s32 anim, HSD_AObj* aobj, u32 frames, f32 frame, f32 end)
{
	if (act->mState == 0) {
		act->mTime = 60.0f * (fn_8006EAF0(0) / 100.0f);
	} else if (act->mState == 3) {
		if ((act->mTime -= frames) < 0.0f) {
			act->mTime  = 0.0f;
			act->mState = 4;
		}
	}
	switch (act->mState) {
	case 1:
		if (anim != 0x814) {
			act->mChr->SetAnim(0x814, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			fn_800D7BF0(&act->mChr->mCopter);
		} else if (aobj->flags & 0x44000000) {
			act->mChr->SetAnim(0x4A, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			act->mState = 0;
		}
		break;
	case 2:
		if (anim != 0x817) {
			act->mChr->SetAnim(0x817, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			fn_800D7B68(&act->mChr->mCopter);
		} else if (aobj->flags & 0x44000000) {
			XObjChr* chr = act->mChr;
			chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			chr->_A2C   = 3;
			act->mState = 0;
		}
		break;
	case 3:
		fn_800C2DB0(act, frames, 0.08726646f);
		break;
	case 4:
		fn_800C2DB0(act, frames, -1.0f);
		if (anim != 0x28) {
			act->mChr->SetAnim(0x28, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			fn_800D7AC8(&act->mChr->mCopter);
		}
		if (act->mChr->GetHeight() <= 0.1f) {
			act->mChr->SetAnim(0x816, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			fn_800D7A78(&act->mChr->mCopter);
			act->mState = 6;
		}
		break;
	case 5:
		fn_800C2DB0(act, frames, -1.0f);
		if (anim != 0x28) {
			act->mChr->SetAnim(0x28, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			act->mChr->mCopter.SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
		if (act->mChr->GetHeight() <= 0.0f || (act->mChr->GetHeight() <= 1.0f && fn_800C35F0(act->mChr).Length() > 0.5f)) {
			act->mChr->SetAnim(0x816, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			fn_800D7A78(&act->mChr->mCopter);
			act->mState = 6;
		}
		break;
	case 6:
		fn_800C2DB0(act, frames, -1.0f);
		if (aobj->flags & 0x44000000) {
			XObjChr* chr = act->mChr;
			chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			chr->_A2C   = 3;
			act->mState = 0;
		}
		break;
	case 7:
		if (anim != 0x38) {
			act->mChr->SetAnim(0x38, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			fn_800D7AC8(&act->mChr->mCopter);
		}
		if (act->mChr->GetHeight() <= 0.1f) {
			act->mChr->SetAnim(0x59, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			fn_800D7A78(&act->mChr->mCopter);
			act->mState = 9;
		}
		break;
	case 8:
		if (anim != 0x1A) {
			act->mChr->SetAnim(0x1A, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			fn_800D7AC8(&act->mChr->mCopter);
		}
		if (act->mChr->GetHeight() <= 0.1f) {
			act->mChr->SetAnim(0x1B, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			fn_800D7A78(&act->mChr->mCopter);
			act->mState = 9;
		}
		break;
	case 9:
		if (aobj->flags & 0x44000000) {
			XObjChr* chr = act->mChr;
			chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			chr->_A2C   = 3;
			act->mState = 0;
		}
		break;
	}
}

/**
 * @note Address: 0x800C2DB0
 * @note Size: 0x108
 */
void fn_800C2DB0(XActCopter* act, u32 frames, f32 speed)
{
	if (speed > 0.0f) {
		act->mPhase = fmod(speed * frames + act->mPhase, 6.2831855f);
		act->mBob   = 0.4f * (f32)sin(act->mPhase) + 0.4f;
		s32 path[]  = { 0 };
		fn_8009F4F8(act->mChr, 1, path, 0.0f, act->mBob, 0.0f);
	} else {
		if (act->mBob > 0.0f) {
			if ((act->mBob -= 0.2f * frames) <= 0.0f) {
				act->mBob = 0.0f;
			}
		}
		s32 path[] = { 0 };
		fn_8009F4F8(act->mChr, 1, path, 0.0f, act->mBob, 0.0f);
	}
}

/**
 * @note Address: 0x800C35F0
 * @note Size: 0x2C
 */
CVec fn_800C35F0(XObjChr* chr)
{
	return chr->_963C;
}

/**
 * @note Address: 0x800C361C
 * @note Size: 0xC8
 */
void fn_800C361C(XAction* act, CPadOne* pad, f32* out)
{
	if (act->mState == 3) {
		if ((pad->mTrigger & 0x200) || fn_8009FEDC(act->mChr)) {
			act->mState = 4;
			fn_800D7AC8(&act->mChr->mCopter);
		}
		*out = 1.0f;
	} else if (act->mState == 4 || act->mState == 7 || act->mState == 8) {
		if (!(pad->mTrigger & 0x200)) {
			fn_8009FEDC(act->mChr);
		}
		*out = 1.0f;
	} else if (act->mState != 0) {
		*out = 0.0f;
	}
}

/**
 * @note Address: 0x800C36E4
 * @note Size: 0x2F0
 */
// NONMATCHING: the float registers of the ring loop are numbered the other way, and the two vectors swap stack slots
void fn_800C36E4(UnkFade* fade, const Vec& pos, f32 angle)
{
	if (fade->mValue > 0.0f) {
		CVec offset(fade->mOffset);
		Mtx m;
		PSMTXRotRad(m, 'y', angle);
		PSMTXMultVec(m, &offset, &offset);
		Vec center = ShadowPos(pos, offset);
		f32 size   = 6.0f * fade->mValue;
		u32 color  = (u32)(160.0f * fade->mValue) | 0x641E00;
		Vec rot    = { -HALF_PI, 0.0f, 0.0f };
		fn_80198258(&lbl_8025C400, 0x18, color, color, center.x, center.y, center.z, size, size, rot.x, rot.y, rot.z);
		PrimVertex verts[50];
		f32 outer = size * 0.5f + 1.0f;
		for (int i = 0; i < 25; i++) {
			f32 a                  = 0.2617994f * i;
			verts[i * 2].pos.x     = center.x - size * (f32)sin(a) * 0.5f;
			verts[i * 2].pos.z     = center.z + size * (f32)cos(a) * 0.5f;
			verts[i * 2].pos.y     = center.y;
			verts[i * 2].color     = color;
			verts[i * 2 + 1].pos.x = center.x - outer * (f32)sin(a);
			verts[i * 2 + 1].pos.z = center.z + outer * (f32)cos(a);
			verts[i * 2 + 1].pos.y = center.y;
			verts[i * 2 + 1].color = 0;
		}
		fn_801988EC(&lbl_8025C400, 0x98, (MtxPtr)(lbl_8025C400._44 + 0xE8), verts, 50);
	}
}

/**
 * @note Address: 0x800C39D4
 * @note Size: 0xAC
 */
void fn_800C39D4(UnkFade* fade, BOOL in, u32 frames)
{
	if (fade->mOn) {
		if (in) {
			if (fade->mValue < 1.0f) {
				if ((fade->mValue += 0.01f * frames) > 1.0f) {
					fade->mValue = 1.0f;
				}
			}
		} else if (fade->mValue > 0.0f) {
			if ((fade->mValue -= 0.05f * frames) < 0.0f) {
				fade->mValue = 0.0f;
				fade->mOn    = FALSE;
			}
		}
	}
}

/**
 * @note Address: 0x800C4468
 * @note Size: 0x4A4
 */
void fn_800C4468(XAction* act, CPadOne* pad, f32* out)
{
	if (act->mState == 2) {
		if (CanGameAct() && lbl_8025C400._5D080 == NULL) {
			if ((pad->mTrigger & 0x100) || (pad->mTrigger & 0x200) || StickLength(pad) > 0.0f) {
				act->mState = 3;
			}
		}
		*out = 0.0f;
	} else if (act->mState == 0x10) {
		if (CanGameAct() && lbl_8025C400._5D080 == NULL) {
			if ((pad->mTrigger & 0x100) || (pad->mTrigger & 0x200) || StickLength(pad) > 0.0f) {
				act->mState = 0x11;
			}
		}
		*out = 0.0f;
	} else if (act->mState == 5) {
		if (CanGameAct() && lbl_8025C400._5D080 == NULL) {
			if ((pad->mTrigger & 0x100) || (pad->mTrigger & 0x200)) {
				act->mState = 6;
			}
		}
		*out = 0.0f;
	} else if (act->mState == 7) {
		*out = 0.8f;
	} else if (act->mState != 0) {
		*out = 0.0f;
	}
}

/**
 * @note Address: 0x800C490C
 * @note Size: 0x40
 */
void fn_800C490C(XActB* act)
{
	act->Reset();
	act->_44 = 0;
}

/**
 * @note Address: 0x800C5270
 * @note Size: 0x464
 */
void fn_800C5270(XActPush* act, s32 anim, HSD_AObj* aobj, u32 frames, f32 frame, f32 end)
{
	switch (act->mState) {
	case 1:
		if (anim != 0x3E) {
			if (act->mTarget != NULL) {
				f32 angle = fn_8008DF34(act->mTarget, &act->mChr->mMoveTranslate, act->mPoint, 1, 0, 0);
				if (fpclassify(angle) != FP_NAN) {
					act->mChr->SetAnim(0x3E, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					act->mChr->SetRotationY(angle, 0);
				} else {
					StopAct(act);
					act->mState = 0;
				}
			}
		} else if (aobj->flags & 0x44000000) {
			act->mChr->SetAnim(0x3F, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			act->mState = 2;
			act->_24    = 1;
		}
		break;
	case 2:
		if (act->_0C == 1) {
			if (anim == 0x4B) {
				if (aobj->flags & 0x44000000) {
					act->mChr->SetAnim(0x40, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					act->_24 = 2;
				}
			} else if (anim != 0x40) {
				act->mChr->SetAnim(0x4B, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				act->_24 = 0;
			}
		} else if (act->_0C == -1) {
			if (anim == 0x4C) {
				if (aobj->flags & 0x44000000) {
					act->mChr->SetAnim(0x41, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					act->_24 = 2;
				}
			} else if (anim != 0x41) {
				act->mChr->SetAnim(0x4C, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				act->_24 = 0;
			}
		} else if (anim != 0x3F) {
			act->mChr->SetAnim(0x3F, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			act->_24 = 0;
		}
		break;
	case 3:
		if (anim != 0x42) {
			act->mChr->SetAnim(0x42, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		} else if (aobj->flags & 0x44000000) {
			XObjChr* chr = act->mChr;
			chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			chr->_A2C   = 3;
			act->mState = 0;
		}
		break;
	}
}

/**
 * @note Address: 0x800C56D4
 * @note Size: 0x248
 */
void fn_800C56D4(XAction* act, CPadOne* pad, f32* out)
{
	if (act->mState == 2) {
		if (CanGameAct() && lbl_8025C400._5D080 == NULL && !(pad->mButton & 0x100)) {
			StopAct(act);
			act->mState = 3;
			*out        = 0.0f;
		}
	} else if (act->mState != 0) {
		if (act->mState != 3 && !(pad->mButton & 0x100)) {
			StopAct(act);
			XObjChr* chr = act->mChr;
			chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			chr->_A2C   = 3;
			act->mState = 0;
		}
		*out = 0.0f;
	}
}

/**
 * @note Address: 0x800C591C
 * @note Size: 0xE4
 */
void fn_800C591C(XAction* act)
{
	if (act->mState != 0) {
		StopAct(act);
		act->mState  = 0;
		XObjChr* chr = act->mChr;
		chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
		chr->_A2C = 3;
	}
}

/**
 * @note Address: 0x800C5A00
 * @note Size: 0x7C
 */
void fn_800C5A00(XAction* act)
{
	StopAct(act);
}

/**
 * @note Address: 0x800C5A7C
 * @note Size: 0x11C
 */
// NONMATCHING: the dol puts the flat vector below the CVec on the stack
void fn_800C5A7C(XActRope* act, f32 angle)
{
	if (act->mTarget != NULL) {
		Vec flat = { act->_10.x, 0.0f, act->_10.z };
		CVec dir(flat);
		f32 dist = dir.Length();
		act->mChr->SetTranslateX(act->mTarget->mMoveTranslate.x - dist * (f32)sin(angle), 0);
		act->mChr->SetTranslateZ(act->mTarget->mMoveTranslate.z - dist * (f32)cos(angle), 0);
	}
}

// the ladder update, defined early so that it calls the joint translate instead of inlining it
/**
 * @note Address: 0x800C7BC4
 * @note Size: 0x1FB0
 */
// NONMATCHING: for the ladder's move the dol reads the target again after copying its last position
void fn_800C7BC4(XActLadder* act, s32 anim, HSD_AObj* aobj, u32 frames, f32 frame, f32 end)
{
	switch (act->mState) {
	case 1:
		if (anim != 0x66) {
			act->mChr->SetAnim(0x66, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			if (act->mTarget != NULL) {
				act->mChr->SetRotationY(WrapAngle(PI + act->mTarget->mMoveRotate.y), 0);
				CVec chrPos(act->mChr->mMoveTranslate);
				chrPos.y = 0.0f;
				CVec ladPos(act->mTarget->mMoveTranslate);
				ladPos.y = 0.0f;
				CVec off(0.0f, 0.0f, 4.0f);
				Mtx m;
				PSMTXRotRad(m, 'y', act->mTarget->mMoveRotate.y);
				PSMTXMultVec(m, &off, &off);
				AddVec(ladPos, off);
				act->mPos.x = ladPos.x;
				act->mPos.y = ladPos.y;
				act->mPos.z = ladPos.z;
				act->mDist  = PSVECDistance(&ladPos, &chrPos);
				act->mAngle = atan2(chrPos.x - ladPos.x, chrPos.z - ladPos.z);
			}
		} else if (aobj->flags & 0x44000000) {
			s32 path[] = { 0 };
			act->mChr->SetPos(fn_8007E7C8(act->mChr, 1, path), 0);
			fn_800C9C34(act->mChr);
			s32 path2[] = { 0 };
			fn_8009F4F8(act->mChr, 1, path2, 0.0f, 0.0f, 0.0f);
			act->mChr->SetAnim(0x67, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			act->mState = 5;
		} else {
			if (act->mTarget != NULL) {
				Vec move = act->mTarget->mMoveTranslate - act->mTarget->mLastPos;
				act->mPos.x += move.x;
				act->mPos.z += move.z;
				if (act->mDist > 0.0f) {
					if ((act->mDist -= 0.2f * frames) < 0.0f) {
						act->mDist = 0.0f;
					}
					act->mChr->SetTranslateX(act->mDist * (f32)sin(act->mAngle) + act->mPos.x, 0);
					act->mChr->SetTranslateZ(act->mDist * (f32)cos(act->mAngle) + act->mPos.z, 0);
				} else {
					act->mChr->SetTranslateX(act->mPos.x, 0);
					act->mChr->SetTranslateZ(act->mPos.z, 0);
				}
			}
			if (CheckStageFlag(lbl_8025C400.mStage, 1)) {
				XObjChrPart* part = &act->mChr->mPart;
				if (part->mOn != 0 && frame >= 25.0f) {
					fn_800B2BD0(part, TRUE);
					fn_8001E904(GAME_SOUND, 0x37, 0xFF);
				}
			}
		}
		break;
	case 2:
		if (anim != 0x6D) {
			act->mChr->SetAnim(0x6D, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			if (act->mTarget != NULL) {
				act->mChr->SetRotationY(WrapAngle(act->mTarget->mMoveRotate.y), 0);
				CVec chrPos(act->mChr->mMoveTranslate);
				chrPos.y = 0.0f;
				CVec ladPos(act->mTarget->mMoveTranslate);
				ladPos.y = 0.0f;
				CVec off(0.0f, 0.0f, -1.5f);
				Mtx m;
				PSMTXRotRad(m, 'y', act->mTarget->mMoveRotate.y);
				PSMTXMultVec(m, &off, &off);
				AddVec(ladPos, off);
				act->mPos.x = ladPos.x;
				act->mPos.y = ladPos.y;
				act->mPos.z = ladPos.z;
				act->mDist  = PSVECDistance(&ladPos, &chrPos);
				act->mAngle = atan2(chrPos.x - ladPos.x, chrPos.z - ladPos.z);
			}
		} else if (aobj->flags & 0x44000000) {
			s32 path[] = { 0 };
			act->mChr->SetPos(fn_8007E7C8(act->mChr, 1, path), 0);
			fn_800C9C34(act->mChr);
			s32 path2[] = { 0 };
			fn_8009F4F8(act->mChr, 1, path2, 0.0f, 0.0f, 0.0f);
			act->mChr->SetRotationY(PI + act->mChr->mMoveRotate.y, 0);
			s32 path3[] = { 0 };
			fn_800BB3D0(act->mChr, 1, path3, 0.0f);
			act->mChr->SetAnim(0x68, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			act->mState = 5;
		} else {
			if (act->mTarget != NULL) {
				Vec move = act->mTarget->mMoveTranslate - act->mTarget->mLastPos;
				act->mPos.x += move.x;
				act->mPos.z += move.z;
				if (act->mDist > 0.0f && frame > 20.0f) {
					if ((act->mDist -= 0.2f * frames) < 0.0f) {
						act->mDist = 0.0f;
					}
					act->mChr->SetTranslateX(act->mDist * (f32)sin(act->mAngle) + act->mPos.x, 0);
					act->mChr->SetTranslateZ(act->mDist * (f32)cos(act->mAngle) + act->mPos.z, 0);
				}
			}
			if (CheckStageFlag(lbl_8025C400.mStage, 1)) {
				XObjChrPart* part = &act->mChr->mPart;
				if (part->mOn != 0 && frame >= 50.0f) {
					fn_800B2BD0(part, TRUE);
					fn_8001E904(GAME_SOUND, 0x37, 0xFF);
				}
			}
		}
		break;
	case 3:
		if (anim != 0x87) {
			if (act->mTarget != NULL) {
				if (act->mTarget->mPoints[act->mPoint].mKind == 0) {
					CVec chrPos(act->mChr->mMoveTranslate);
					chrPos.y = 0.0f;
					CVec pt(fn_800D5190(act));
					pt.y = 0.0f;
					act->mChr->SetRotationY(PI + (f32)atan2(pt.x - chrPos.x, pt.z - chrPos.z), 0);
				} else {
					act->mChr->SetRotationY(act->mTarget->mMoveRotate.y, 0);
				}
				act->mChr->SetAnim(0x87, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				act->mChr->SetAnim(0x87, TRUE, NULL, NULL, NULL, fn_800A2CE4(act->mChr), 1.0f, 1.0f, 10.0f);
				act->mChr->UpdateAnim(0);
				s32 path[] = { 0 };
				CVec d     = fn_8007E7C8(act->mChr, 1, path) - act->mChr->mMoveTranslate;
				act->mChr->SetTranslateX(act->mChr->mMoveTranslate.x - d.x, 0);
				act->mChr->SetTranslateZ(act->mChr->mMoveTranslate.z - d.z, 0);
				act->mChr->SetTranslateY(act->mTarget->_77C, 0);
				act->mChr->SetAnim(0x87, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				CVec chrPos(act->mChr->mMoveTranslate);
				chrPos.y = 0.0f;
				CVec ladPos(act->mTarget->mMoveTranslate);
				ladPos.y = 0.0f;
				CVec off(0.0f, 0.0f, -1.0f);
				Mtx m;
				PSMTXRotRad(m, 'y', act->mTarget->mMoveRotate.y);
				PSMTXMultVec(m, &off, &off);
				AddVec(ladPos, off);
				act->mPos.x = ladPos.x;
				act->mPos.y = ladPos.y;
				act->mPos.z = ladPos.z;
				act->mDist  = PSVECDistance(&ladPos, &chrPos);
				act->mAngle = atan2(chrPos.x - ladPos.x, chrPos.z - ladPos.z);
			}
			if (CheckStageFlag(lbl_8025C400.mStage, 1)) {
				XObjChrPart* part = &act->mChr->mPart;
				if (part->mOn != 0) {
					fn_800B2BD0(part, TRUE);
					fn_8001E904(GAME_SOUND, 0x37, 0xFF);
				}
			}
		} else if (aobj->flags & 0x44000000) {
			s32 path[]      = { 0 };
			HSD_JObj* joint = fn_800B2764(act->mChr, 1, path);
			f32 turn        = HSD_JObjGetRotationY(joint);
			f32 facing      = HSD_JObjGetRotationY(act->mChr->mJObj);
			act->mChr->SetRotationY(facing + turn, 0);
			fn_800C9B74(joint, 0.0f);
			s32 path2[] = { 0 };
			CVec d      = fn_8007E7C8(act->mChr, 1, path2) - act->mChr->mMoveTranslate;
			f32 angle   = HSD_JObjGetRotationY(act->mChr->mJObj);
			d.x         = -d.x;
			d.z         = -d.z;
			act->mChr->SetPos((act->mChr->mMoveTranslate + d), 0);
			fn_800C9C34(act->mChr);
			s32 path3[] = { 0 };
			fn_8009F4F8(act->mChr, 1, path3, 0.0f, 0.0f, 0.0f);
			act->mChr->SetAnim(0x68, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			act->mState = 5;
			act->_0C    = 0;
		} else if (act->mTarget != NULL) {
			Vec move = act->mTarget->mMoveTranslate - act->mTarget->mLastPos;
			act->mPos.x += move.x;
			act->mPos.z += move.z;
			if (act->mDist > 0.0f) {
				if ((act->mDist -= 0.4f * frames) < 0.0f) {
					act->mDist = 0.0f;
				}
				act->mChr->SetTranslateX(act->mDist * (f32)sin(act->mAngle) + act->mPos.x, 0);
				act->mChr->SetTranslateZ(act->mDist * (f32)cos(act->mAngle) + act->mPos.z, 0);
			} else {
				act->mChr->SetTranslateX(act->mPos.x, 0);
				act->mChr->SetTranslateZ(act->mPos.z, 0);
			}
			act->mChr->SetTranslateY(act->mTarget->_77C, 0);
		}
		break;
	case 4:
		act->mChr->SetAnim(0x68, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
		act->mState = 5;
		break;
	case 5:
		if (anim == 0x69 || anim == 0x6A || anim == 0x6B || anim == 0x6C) {
			if (aobj->flags & 0x44000000) {
				s32 path[] = { 0 };
				CVec hand  = fn_800BAD80(act->mChr, 1, path);
				if (hand.y < 0.0f) {
					hand.y = (s32)(hand.y - 0.5f);
				}
				s32 path2[] = { 0 };
				fn_800C7810(act->mChr, 1, path2, hand);
				s32 path3[] = { 0 };
				act->mChr->SetPos(fn_8007E7C8(act->mChr, 1, path3), 0);
				fn_800C9C34(act->mChr);
				s32 path4[] = { 0 };
				fn_8009F4F8(act->mChr, 1, path4, 0.0f, 0.0f, 0.0f);
				if (act->_0C == 1) {
					if (act->mTarget != NULL) {
						s32 path5[] = { 0 };
						if (14.0f + fn_8007E7C8(act->mChr, 1, path5).y >= act->mTarget->mPointDist[0]) {
							act->mState = 6;
							goto top;
						}
					}
					if (anim == 0x69 || anim == 0x6B) {
						act->mChr->SetAnim(0x6A, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					} else if (anim == 0x6A || anim == 0x6C) {
						act->mChr->SetAnim(0x69, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					}
				} else if (act->_0C == -1) {
					if (act->mChr->GetHeight() <= 2.0f) {
						act->mState = 7;
					} else if (anim == 0x69 || anim == 0x6B) {
						act->mChr->SetAnim(0x6C, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					} else if (anim == 0x6A || anim == 0x6C) {
						act->mChr->SetAnim(0x6B, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					}
				} else if (anim == 0x69 || anim == 0x6B) {
					act->mChr->SetAnim(0x68, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				} else if (anim == 0x6A || anim == 0x6C) {
					act->mChr->SetAnim(0x67, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				}
			top:
				if (act->_2C && act->mState == 5) {
					act->mState = 9;
				}
			}
		} else {
			if (act->_0C == 1) {
				if (act->mTarget != NULL) {
					s32 path[] = { 0 };
					if (14.0f + fn_8007E7C8(act->mChr, 1, path).y >= act->mTarget->mPointDist[0]) {
						act->mState = 6;
						goto top2;
					}
				}
				if (anim == 0x67) {
					act->mChr->SetAnim(0x69, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				} else if (anim == 0x68) {
					act->mChr->SetAnim(0x6A, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				}
			} else if (act->_0C == -1) {
				if (act->mChr->GetHeight() <= 2.0f) {
					act->mState = 7;
				} else if (anim == 0x67) {
					act->mChr->SetAnim(0x6B, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				} else if (anim == 0x68) {
					act->mChr->SetAnim(0x6C, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				}
			}
		top2:
			if (act->_2C && act->mState == 5) {
				act->mState = 9;
			}
		}
		break;
	case 6:
		if (!(anim == 0x6E || anim == 0x6F)) {
			if (anim == 0x69 || anim == 0x6B) {
				act->mChr->SetAnim(0x6F, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x6A || anim == 0x6C) {
				act->mChr->SetAnim(0x6E, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x67) {
				act->mChr->SetAnim(0x6E, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x68) {
				act->mChr->SetAnim(0x6F, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
		} else if (aobj->flags & 0x44000000) {
			s32 path[] = { 0 };
			act->mChr->SetPos(fn_8007E7C8(act->mChr, 1, path), 0);
			fn_800C9C34(act->mChr);
			s32 path2[] = { 0 };
			fn_8009F4F8(act->mChr, 1, path2, 0.0f, 0.0f, 0.0f);
			XObjChr* chr = act->mChr;
			chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			chr->_A2C    = 3;
			act->mState  = 0;
			act->mTarget = NULL;
		}
		break;
	case 7:
		if (!(anim == 0x70 || anim == 0x71)) {
			if (anim == 0x69 || anim == 0x6B) {
				act->mChr->SetAnim(0x71, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x6A || anim == 0x6C) {
				act->mChr->SetAnim(0x70, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x67) {
				act->mChr->SetAnim(0x70, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x68) {
				act->mChr->SetAnim(0x71, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
		} else if (aobj->flags & 0x44000000) {
			s32 path[] = { 0 };
			act->mChr->SetPos(fn_8007E7C8(act->mChr, 1, path), 0);
			fn_800C9C34(act->mChr);
			s32 path2[] = { 0 };
			fn_8009F4F8(act->mChr, 1, path2, 0.0f, 0.0f, 0.0f);
			XObjChr* chr = act->mChr;
			chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			chr->_A2C    = 3;
			act->mState  = 0;
			act->mTarget = NULL;
		}
		break;
	case 8:
		if (anim == 0x69 || anim == 0x6B) {
			act->mChr->SetAnim(0x73, TRUE, NULL, NULL, NULL, 10.0f, 1.0f, 1.0f, 10.0f);
		} else if (anim == 0x6A || anim == 0x6C) {
			act->mChr->SetAnim(0x72, TRUE, NULL, NULL, NULL, 10.0f, 1.0f, 1.0f, 10.0f);
		} else if (anim == 0x67) {
			act->mChr->SetAnim(0x72, TRUE, NULL, NULL, NULL, 10.0f, 1.0f, 1.0f, 10.0f);
		} else if (anim == 0x68) {
			act->mChr->SetAnim(0x73, TRUE, NULL, NULL, NULL, 10.0f, 1.0f, 1.0f, 10.0f);
		}
		act->mState = 9;
	case 9:
		if (!(anim == 0x72 || anim == 0x73)) {
			if (anim == 0x69 || anim == 0x6B) {
				act->mChr->SetAnim(0x73, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x6A || anim == 0x6C) {
				act->mChr->SetAnim(0x72, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x67) {
				act->mChr->SetAnim(0x72, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x68) {
				act->mChr->SetAnim(0x73, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
		} else if (aobj->flags & 0x44000000) {
			s32 path[] = { 0 };
			act->mChr->SetPos(fn_8007E7C8(act->mChr, 1, path), 0);
			fn_800C9C34(act->mChr);
			s32 path2[] = { 0 };
			fn_8009F4F8(act->mChr, 1, path2, 0.0f, 0.0f, 0.0f);
			if (act->mChr->GetMode() == 1) {
				act->mState                  = 0;
				act->mChr->mActCopter.mState = 4;
			} else {
				act->mChr->SetAnim(0x74, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				act->mState = 10;
			}
			act->mTarget = NULL;
		}
		break;
	case 10:
		if (act->mChr->GetHeight() <= 0.0f) {
			if (act->mChr->_9A90 > 30.0f) {
				act->mState = 0;
				act->mChr->SetAnim(0x8E, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				act->mChr->mActJump.mState = 5;
				fn_800C9D54((XActJump*)&act->mChr->mActJump, act->mChr->_9A90);
				act->mChr->SetColor(1, 0xFF, 0, 0, 0.0f);
				act->mChr->SetColor(1, 0, 0, 0, 0.3f);
				fn_80023EE0(lbl_8025C400._4588, 0xFF, 0, 0, 0, 100, 100, 200, 0, 0.3f);
			} else {
				act->mChr->SetAnim(0x75, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				act->mState = 11;
			}
		}
		break;
	case 11:
		if (aobj->flags & 0x44000000) {
			XObjChr* chr = act->mChr;
			chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			chr->_A2C   = 3;
			act->mState = 0;
		}
		break;
	}
}

/**
 * @note Address: 0x800C7810
 * @note Size: 0x2B4
 */
void fn_800C7810(CJObj* obj, s32 depth, s32* path, const Vec& pos)
{
	CJObj joint(GetJointByPath(obj->mJObj, path, depth));
	joint.SetTranslate(pos.x, pos.y, pos.z, TRUE);
}

/**
 * @note Address: 0x800C9C34
 * @note Size: 0x3C
 */
void fn_800C9C34(XObj* obj)
{
	if (obj->mMoveTranslate.y != obj->mLastPos.y) {
		obj->mFlags |= 0x0010000000000000ULL;
	}
	obj->mLastPos.x = obj->mMoveTranslate.x;
	obj->mLastPos.y = obj->mMoveTranslate.y;
	obj->mLastPos.z = obj->mMoveTranslate.z;
}

/**
 * @note Address: 0x800C9C70
 * @note Size: 0xE4
 */
void fn_800C9C70(XActC* act, CPadOne* pad, f32* out)
{
	act->_2C = 0;
	if (act->mState == 5) {
		if (CanGameAct() && lbl_8025C400._5D080 == NULL && (pad->mTrigger & 0x200)) {
			act->_2C = 1;
		}
	} else if (act->mState != 0) {
		*out = 0.0f;
	}
}

/**
 * @note Address: 0x800C9D54
 * @note Size: 0x360
 */
void fn_800C9D54(XActJump* act, f32 fall)
{
	if (lbl_8025C400._5D080 != NULL) {
		act->mFall = 0.0f;
		act->_48   = 0.0f;
	} else {
		act->mFall = fall;
		act->_48   = fall;
		if (act->mFall >= 50.0f) {
			XObjChr* chr = act->mChr;
			if (chr->mActLift.mState != 0 && IsCarrying(&chr->mActLift, 0x16)) {
				fn_800D4998((XActLift*)&chr->mActLift);
				if (act->mFall >= 60.0f) {
					act->mState                  = 0;
					act->mChr->mActTumble.mState = 1;
					act->mChr->SetAnim(0x35, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				}
			}
		}
	}
}

/**
 * @note Address: 0x800CBDB4
 * @note Size: 0x18
 */
void fn_800CBDB4(XAction* act, s32 unused, f32* out)
{
	if (act->mState != 0) {
		*out = 0.0f;
	}
}

/**
 * @note Address: 0x800CE120
 * @note Size: 0x214
 */
void fn_800CE120(XActHang* act, CPadOne* pad, f32* out)
{
	act->mDrop = FALSE;
	if (act->mState == 4) {
		f32 stick = -(f32)atan2(pad->mStickX, pad->mStickY);
		f32 angle = WrapAngle(stick + lbl_8025C400.mCameraAngle - act->mChr->mMoveRotate.y);
		if (!(angle < 0.5235988f) && !(angle > 5.7595863f)) {
			act->mDrop = TRUE;
		}
		*out = 0.0f;
	} else if (act->mState == 5) {
		if (CanGameAct() && lbl_8025C400._5D080 == NULL) {
			if (pad->mTrigger & 0x100) {
				if (act->mTarget == NULL || (act->mTarget->mFlags & 0x400000000ULL)) {
					fn_800CDBA8(act);
					act->_0C = 0;
				}
				*out = 0.0f;
			} else if (pad->mTrigger & 0x200) {
				act->mState = 9;
				act->_0C    = 0;
				*out        = 0.0f;
			}
		}
	} else if (act->mState != 0) {
		*out = 0.0f;
	}
}

/**
 * @note Address: 0x800CE334
 * @note Size: 0x20
 */
void fn_800CE334(XActC* act, s32 value, BOOL apply)
{
	act->_54 = value;
	if (apply) {
		act->mChr->mPart.mOn = value | 0x10000;
	}
}

/**
 * @note Address: 0x800CE9A8
 * @note Size: 0x354
 */
// NONMATCHING: the temporaries take other stack slots
Vec fn_800CE9A8(XActLift* act, XObj* target)
{
	if (target->GetId() == 0x18) {
		s32 path[] = { 0 };
		CJObj joint(GetJointByPath(target->mJObj, path, 1));
		Vec pos = GetJObjPos(joint.mJObj);
		return CVec(pos) - target->mMoveTranslate;
	}
	return fn_8008C274(target, act->mPoint, target->mMoveTranslate * -1.0f);
}

/**
 * @note Address: 0x800D0D3C
 * @note Size: 0x90
 */
void fn_800D0D3C(HSD_JObj* jobj)
{
	HSD_JObjClearUserMtx(jobj);
}

/**
 * @note Address: 0x800D3A5C
 * @note Size: 0x94
 */
HSD_Generator* fn_800D3A5C(UnkGame* game, s32 bank, s32 id, s32 status, const Vec& pos, BOOL keep)
{
	// the dol's is game/particle_srt.h's CreateParticle out of line, its rot is that header's
	// fifth Vec. inlined into here MWCC swaps the two stack slots, so this keeps its own
	Vec rot = { 0.0f, 0.0f, 0.0f };
	return game->mParticle.CreateSRT(id | (bank << 16), status, pos, ParticleUnitScale(), rot, keep);
}

/**
 * @note Address: 0x800D3BA4
 * @note Size: 0x5C
 */
HSD_Generator* fn_800D3BA4(UnkGame* game, s32 bank, s32 id, s32 status, const Vec& pos, const Vec& scale, BOOL keep)
{
	return CreateParticle(game->mParticle, bank, id, status, pos, scale, keep);
}

/**
 * @note Address: 0x800D3CB8
 * @note Size: 0x24
 */
void fn_800D3CB8(UnkStatus* status, s16 id, s16 n)
{
	fn_800DC754(&status->mTable, id, n);
}

/**
 * @note Address: 0x800D3CDC
 * @note Size: 0x30
 */
BOOL fn_800D3CDC(XObj* obj)
{
	return !(obj->mFlags & 0x0040000000000000ULL);
}

/**
 * @note Address: 0x800D3D0C
 * @note Size: 0x64
 */
UnkXObjReadObj* fn_800D3D0C(UnkGame* game, s16 id)
{
	return fn_80132698((UnkXObjList*)game->mObjList, &CoordInfo(fn_8006EDB4(id)), 0, TRUE);
}

/**
 * @note Address: 0x800D3D70
 * @note Size: 0x44
 */
void fn_800D3D70(UnkLimited* lim, f32 value)
{
	lim->mValue = value > 0.0f ? value : 0.0f;
	if (lim->mValue < 0.0f) {
		lim->mValue = 0.0f;
	}
	if (lim->mValue > lim->mMax) {
		lim->mValue = lim->mMax;
	}
}

/**
 * @note Address: 0x800D3DB4
 * @note Size: 0x20
 */
void fn_800D3DB4(UnkGame* game, u32 flags)
{
	game->_43D0 = 1;
	game->_43D4 = 0;
	game->_43D4 |= flags;
}

/**
 * @note Address: 0x800D3DD4
 * @note Size: 0x24
 */
void fn_800D3DD4(UnkGame* game, f32 a, f32 b)
{
	fn_800136B4(game->_042AC, a, b);
}

/**
 * @note Address: 0x800D3EB0
 * @note Size: 0x14
 */
void fn_800D3EB0(UnkGame* game, f32 a, f32 b, f32 c, f32 d)
{
	game->_43E0[0] = a;
	game->_43E0[1] = b;
	game->_43E0[2] = c;
	game->_43E0[3] = d;
}

/**
 * @note Address: 0x800D3EC4
 * @note Size: 0x50
 */
void fn_800D3EC4(XObj* obj)
{
	obj->mMsgCount = 0;
	for (int i = 0; i < 16; i++) {
		obj->mMsgs[i].mId = -1;
	}
}

/**
 * @note Address: 0x800D40EC
 * @note Size: 0x164
 */
// NONMATCHING: the dol keeps the three values on the stack and checks the rotation pointer
void fn_800D40EC(CXObj* obj)
{
	Quaternion q;
	HSD_JObjGetTranslation(obj->mJObj, (Vec*)&q);
	SetStill(&obj->mMoveTranslate, q.x, q.y, q.z);
	HSD_JObjGetRotation(obj->mJObj, &q);
	SetStill(&obj->mMoveRotate, q.x, q.y, q.z);
	HSD_JObjGetScale(obj->mJObj, (Vec*)&q);
	SetStill(&obj->mMoveScale, q.x, q.y, q.z);
}

/**
 * @note Address: 0x800D4250
 * @note Size: 0x44
 */
f32 fn_800D4250(CJObj* obj)
{
	return HSD_JObjGetRotationY(obj->mJObj);
}

/**
 * @note Address: 0x800D4294
 * @note Size: 0x60
 */
void fn_800D4294(XObjChr* chr, s32 id, f32 blend)
{
	chr->SetAnim(id, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, blend, 10.0f);
	chr->_A2C = 3;
}

/**
 * @note Address: 0x800D42F4
 * @note Size: 0x3C
 */
HSD_Generator* fn_800D42F4(UnkGame* game, s32 bank, s32 id, u8 type, HSD_JObj* jobj, BOOL keep)
{
	return game->mParticle.Create(id | (bank << 16), type, jobj, keep);
}

/**
 * @note Address: 0x800D4330
 * @note Size: 0x10
 */
s16 fn_800D4330(UnkStatus* status, s32 i)
{
	return status->mTable.mSlots[i].mValue;
}

/**
 * @note Address: 0x800D4340
 * @note Size: 0x28
 */
s16 fn_800D4340(XObj* obj)
{
	return obj->GetKind();
}

/**
 * @note Address: 0x800D4368
 * @note Size: 0x48C
 */
void fn_800D4368(XActLift* act, CPadOne* pad, f32* out)
{
	if (act->mState == 4) {
		XObjChr* chr = act->mChr;
		if (chr->mActJump.mState != 0) {
			return;
		}
		if (chr->mActHang.mState != 0) {
			return;
		}
		if (!CanGameAct()) {
			return;
		}
		if (lbl_8025C400.mFlags & 0x100) {
			return;
		}
		if (lbl_8025C400._5D080 != NULL) {
			return;
		}
		if (chr->mAnimId == 0x895 || chr->mAnimId == 0x8AE || chr->mAnimId == 0x8AF) {
			*out = 0.0f;
			return;
		}
		if (pad->mTrigger & 0x100) {
			if (act->mTarget == NULL) {
				return;
			}
			if (act->mTarget->GetId() == 8) {
				return;
			}
			if (chr->_9E4 >= 0 && (lbl_8025C400.mObjects[chr->_9E4]->mFlags & 0x20)) {
				*out = 0.0f;
				return;
			}
			if (act->mTarget->GetId() == 0x18) {
				return;
			}
			act->mState = 6;
			*out        = 0.0f;
		} else if (pad->mTrigger & 0x200) {
			if (act->mTarget != NULL) {
				if (act->mTarget->GetId() == 8) {
					if (chr->GetMode() == 0) {
						act->mState = 0x13;
					}
				} else {
					act->mState = 6;
				}
			}
			*out = 0.0f;
		}
	} else if (act->mState == 0xC || act->mState == 0xD) {
		if (CanGameAct() && lbl_8025C400._5D080 == NULL && ((pad->mTrigger & 0x100) || (pad->mTrigger & 0x200))) {
			act->mState = 0xE;
			act->_2C    = 0.0f;
		}
		*out = 0.0f;
	} else if (act->mState == 0xF) {
		if (CanGameAct()) {
			if (pad->mStickX > 0.8f) {
				act->mState = 0x10;
				act->_2C    = 0.0f;
			} else if (pad->mTrigger & 0x200) {
				act->mState = 0x11;
				act->_2C    = 0.0f;
			}
		}
		*out = 0.0f;
	} else if (act->mState == 0x1B) {
		// stays as it is while the lift is thrown
	} else if (act->mState == 0x1C) {
	} else if (act->mState == 0x1F || act->mState == 0x20) {
		if (!(pad->mButton & 0x100)) {
			act->mState = 0x21;
		}
		*out = 0.0f;
	} else if (act->mState != 0) {
		*out = 0.0f;
	}
}

/**
 * @note Address: 0x800D47F4
 * @note Size: 0x38
 */
BOOL fn_800D47F4(UnkColorOut* out)
{
	u32 color = lbl_8025C400.mChr->_158AC;
	out->mR   = color >> 24;
	out->mG   = color >> 16;
	out->mB   = color >> 8;
	return FALSE;
}

/**
 * @note Address: 0x800C7AC4
 * @note Size: 0x100
 */
void fn_800C7AC4(XAction* act, CPadOne* pad, f32* out)
{
	if (act->mState == 5) {
		if (CanGameAct() && lbl_8025C400._5D080 == NULL) {
			if (pad->mTrigger & 0x200) {
				act->mState = 8;
			}
			if (act->mChr->mAnimId == 0x8A || act->mChr->mAnimId == 0x8C) {
				*out = 0.0f;
			}
		}
	} else if (act->mState != 0) {
		*out = 0.0f;
	}
}

/**
 * @note Address: 0x800C9B74
 * @note Size: 0xC0
 */
void fn_800C9B74(HSD_JObj* jobj, f32 y)
{
	HSD_JObjSetRotationY(jobj, y);
}

/**
 * @note Address: 0x800D0E8C
 * @note Size: 0xC0
 */
void fn_800D0E8C(HSD_JObj* jobj, Mtx mtx)
{
	HSD_JObjCopyMtx(jobj, mtx);
}

/**
 * @note Address: 0x800D3AF0
 * @note Size: 0xB4
 */
Vec fn_800D3AF0(void* unused, HSD_JObj* jobj)
{
	Vec scale;
	HSD_MtxGetScale(HSD_JObjGetMtxPtr(jobj), &scale);
	return scale;
}

/**
 * @note Address: 0x800D3C00
 * @note Size: 0xB8
 */
// NONMATCHING: the dol loads the jobj before saving r30
Vec fn_800D3C00(const CJObj* obj)
{
	HSD_JObj* jobj = obj->mJObj;
	Vec scale;
	HSD_MtxGetScale(HSD_JObjGetMtxPtr(jobj), &scale);
	return scale;
}

/**
 * @note Address: 0x800D3DF8
 * @note Size: 0xB8
 */
// NONMATCHING: same as fn_800D3C00
Vec fn_800D3DF8(const CJObj* obj)
{
	HSD_JObj* jobj = obj->mJObj;
	Vec pos;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(jobj), &pos);
	return pos;
}

/**
 * @note Address: 0x800D482C
 * @note Size: 0x16C
 */
void fn_800D482C(XAction* act)
{
	s32 path[]     = { 0, 1 };
	HSD_JObj* jobj = FindJoint(act->mTarget->mJObj, 2, path);
	for (HSD_DObj* dobj = HSD_JObjGetDObj(jobj); dobj != NULL; dobj = HSD_DObjGetNext(dobj)) {
		HSD_MObj* mobj = HSD_DObjGetMObj(dobj);
		if (mobj != NULL) {
			mobj->mat->diffuse = act->mChr->mColor;
			for (HSD_TObj* tobj = HSD_MObjGetTObj(mobj); tobj != NULL; tobj = HSD_TObjGetNext(tobj)) {
				tobj->flags = (tobj->flags &= ~0xF0000) | 0x60000;
			}
			HSD_MObjCompileTev(mobj);
		}
	}
}

/**
 * @note Address: 0x800D4B60
 * @note Size: 0x118
 */
BOOL fn_800D4B60(XAction* act)
{
	return IsCarryingStill(act, 0x16);
}

/**
 * @note Address: 0x800D4C78
 * @note Size: 0xDC
 */
BOOL fn_800D4C78(XAction* act)
{
	return IsCarrying(act, 0x38);
}

/**
 * @note Address: 0x800D4D54
 * @note Size: 0xDC
 */
BOOL fn_800D4D54(XAction* act)
{
	return IsCarrying(act, 0x16);
}

/**
 * @note Address: 0x800D4E30
 * @note Size: 0xDC
 */
BOOL fn_800D4E30(XAction* act)
{
	return IsCarrying(act, 0x15);
}

/**
 * @note Address: 0x800D4F0C
 * @note Size: 0xDC
 */
BOOL fn_800D4F0C(XAction* act)
{
	return IsCarrying(act, 0x10);
}

/**
 * @note Address: 0x800D4FE8
 * @note Size: 0x4C
 */
BOOL fn_800D4FE8(XAction* act)
{
	return IsTargetId(act, 0x18);
}

/**
 * @note Address: 0x800D5034
 * @note Size: 0xA0
 */
void fn_800D5034(XActD* act)
{
	if (act->mTarget != NULL) {
		act->mTarget->mFlags &= ~0x10000ULL;
		act->mTarget->mFlags &= ~0x0400000000000000ULL;
	}
	act->Reset();
	act->_60 = 0;
	act->_6C = 0;
	act->_74 = 0;
}

/**
 * @note Address: 0x800D50D4
 * @note Size: 0xBC
 */
void fn_800D50D4(XActD* act, XObjChr* chr)
{
	act->Init(chr);
	act->_2C = 0.0f;
	act->_48 = -1;
	fn_800D5034(act);
}

CVec fn_800A0978(XObjChr* chr);

// Place the carried object relative to the robot's attachment point.
void fn_800CE904(XActLift* act, XObj* target)
{
	target->SetPos((fn_800A0978(act->mChr) - fn_800CE9A8(act, target)), 0);
}
} // extern "C"
