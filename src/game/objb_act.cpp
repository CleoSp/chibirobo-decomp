#include "game/objb.h"
extern "C" {
#include <sysdolphin/mtx_transform.h>
}

// the robot's moves, the XAct classes on XAction, and the game helpers they use

extern "C" {
void* fn_8006EDB4(s32 id);
void fn_80023EE0(void* obj, s32 a, s32 b, s32 c, s32 d, s32 r, s32 g, s32 bl, s32 e, f32 f);
BOOL fn_8009FEDC(XObjChr* chr);
void fn_800D7AC8(XObjCopter* obj);
BOOL fn_800DC754(UnkSlotTable* table, s16 id, s16 n);
void fn_800136B4(void* obj, f32 a, f32 b);
void fn_80024CA0(CPtclSet* ptcl);
void fn_80024E0C(CPtclSet* ptcl);
}

// a value that fades in and out
struct UnkFade {
	s32 mOn;     // _00
	u8 _04[0xC]; // _04
	f32 mValue;  // _10
};

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
};

// the popper's shots, ten of them
class XActPopper : public XAction {
public:
	u8 _28[0x8];               // _28
	XActPopperShot mShots[10]; // _30
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

// ---- functions ----

extern "C" {

/**
 * @note Address: 0x800BE648
 * @note Size: 0xF0
 */
void fn_800BE648(XActE* act, u32 frames)
{
	if (act->mState == 0 && act->_2C != 0 && act->mChr->_C44 == 0) {
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

/**
 * @note Address: 0x800C2D18
 * @note Size: 0x98
 */
void fn_800C2D18(XActPopperShot* shot)
{
	shot->_000 = 0;
	shot->_038 = 0;
	fn_80024E0C(&shot->mPtcl);
	shot->_0C0 = 0.0f;
	shot->_0D8 = 0.0f;
	KillGen(shot->mPtcl.mGens[0]);
	KillGen(shot->mPtcl.mGens[1]);
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
 * @note Address: 0x800C490C
 * @note Size: 0x40
 */
void fn_800C490C(XActB* act)
{
	act->Reset();
	act->_44 = 0;
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
 * @note Address: 0x800CE334
 * @note Size: 0x20
 */
void fn_800CE334(XActC* act, s32 value, BOOL apply)
{
	act->_54 = value;
	if (apply) {
		act->mChr->_A80C = value | 0x10000;
	}
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
// NONMATCHING: the two vec initializers load into each other's registers
HSD_Generator* fn_800D3A5C(UnkGame* game, s32 bank, s32 id, s32 status, const Vec& pos, BOOL keep)
{
	Vec rot   = { 0.0f, 0.0f, 0.0f };
	Vec scale = { 1.0f, 1.0f, 1.0f };
	return game->mParticle.CreateSRT(id | (bank << 16), status, pos, scale, rot, keep);
}

/**
 * @note Address: 0x800D3BA4
 * @note Size: 0x5C
 */
HSD_Generator* fn_800D3BA4(UnkGame* game, s32 bank, s32 id, s32 status, const Vec& pos, const Vec& scale, BOOL keep)
{
	Vec rot = { 0.0f, 0.0f, 0.0f };
	return game->mParticle.CreateSRT(id | (bank << 16), status, pos, scale, rot, keep);
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
Vec fn_800D3C00(CJObj* obj)
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
Vec fn_800D3DF8(CJObj* obj)
{
	HSD_JObj* jobj = obj->mJObj;
	Vec pos;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(jobj), &pos);
	return pos;
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

} // extern "C"
