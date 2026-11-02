#include "game/xchr.h"
#include "game/xact.h"
#include "game/xobjflags.h"
#include "game/xobjs.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/ccamera.h"
#include "game/cjobjset.h"
#include "game/cjobjinlineattach.h"
#include "game/cparticle.h"
#include "game/cscrn.h"
#include "game/csound.h"
#include "game/fbtexeff.h"
#include "game/globals.h"
#include "game/hsd_dobj.h"
#include "game/stagedata.h"
#include "game/video.h"
#include "game/gmath.h"
#include <math.h>
extern "C" {
#include <dolphin/mtx/mtx.h>
#include <dolphin/mtx/vec.h>
#include <sysdolphin/mtx_transform.h>
}

// chibi robo's actions, the XAct kinds of XAction, and the game helpers they use

extern "C" {
void fn_80015300(CGame* game, const Vec& pos, f32* x, f32* y, f32* z, CCamera* camera); // where pos is on the screen
void fn_800136B4(FbSubwin* subwin, const Vec& interest, f32 angleY, f32 angleX, f32 distance, f32 fov);
f32 fn_8008DF34(XObj* obj, const Vec* pos, s32 floor, s32 a, s32 b, s32 c);    // the angle to face a floor of obj from pos
void fn_8009F4F8(CXObj* obj, s32 depth, const s32* path, f32 x, f32 y, f32 z); // puts a joint at a translation
BOOL fn_8009FEDC(XObjChr* chr);
f32 fn_800A2CE4(const CJObj* obj); // the end frame of the anim
HSD_JObj* fn_800B2764(CXObj* obj, s32 depth, s32* path);
void fn_800B2BD0(XObjPlug* plug, BOOL reset);
CVec fn_800BAD80(CJObj* obj, s32 depth, s32* path);
void fn_800BB3D0(CXObj* obj, s32 depth, s32* path, f32 angle);
void fn_800C21A0(XActPopperShot* shot);
void fn_800C2DB0(XActCopter* act, u32 frames, f32 speed);
CVec fn_800C35F0(XObjChr* chr);
void fn_800C7810(CJObj* obj, s32 depth, s32* path, const Vec& pos);
void fn_800C9B74(HSD_JObj* jobj, f32 angle);
void fn_800C9C34(XObj* obj);
void fn_800C9D54(XActJump* act, f32 strength);
void fn_800CDBA8(XActHang* act);
Vec fn_800CE9A8(XActLift* act, XObj* target);
Vec fn_800D5190(XAction* act); // where the held floor of the target is
u32 CheckStageFlag(s32 stage, u32 mask);
}

// the part of a particle the item tint callback writes
struct ItemPtclColor {
	u8 _00[0x12]; // _00
	u8 mRed;      // _12
	u8 mGreen;    // _13
	u8 mBlue;     // _14
};

// how far the stick is pushed
static inline f32 StickLength(const CPadOne* pad)
{
	f32 x = pad->mStickX;
	f32 y = pad->mStickY;
	return sqrtf(x * x + y * y);
}

// a += b, b by value like the other CVec operators
static inline void AddVec(CVec& a, Vec b)
{
	PSVECAdd(&a, &b, &a);
}

// the tumble ring sits just above the floor
static inline Vec RingPos(const Vec& pos, const Vec& offset)
{
	Vec v = { pos.x + offset.x, 0.01f + pos.y, pos.z + offset.z };
	return v;
}

// a held object of the given pool and kind
static inline bool IsHolding(XAction* act)
{
	bool result = false;
	if (act->mTarget != NULL && (act->mTarget->mStatus.mFlags & 0x400000000000ULL)) {
		result = true;
	}
	return result;
}

static inline bool IsHoldingPool(XAction* act, s16 pool)
{
	bool result = false;
	if (IsHolding(act) && act->mTarget->GetCategory() == pool) {
		result = true;
	}
	return result;
}

static inline bool IsHoldingKind(XAction* act, s16 pool, s16 kind)
{
	bool result = false;
	if (IsHoldingPool(act, pool) && act->mTarget->GetItem() == kind) {
		result = true;
	}
	return result;
}

// holding the item and not throwing it
static inline bool IsCarrying(XAction* act, s16 kind)
{
	bool result = false;
	if (IsHoldingKind(act, 8, kind) && act->_24 != 15) {
		result = true;
	}
	return result;
}

static inline bool IsTargetPool(XAction* act, s16 pool)
{
	bool result = false;
	if (act->mTarget != NULL && act->mTarget->GetCategory() == pool) {
		result = true;
	}
	return result;
}

static inline bool IsStill(XAction* act)
{
	bool result = false;
	if (act->mState == 31 || act->mState == 32 || act->mState == 33) {
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

// lets go of a pushed object and resets
static inline void Release(XAction* act)
{
	XObj* target = act->mTarget;
	if (target != NULL && (target->mStatus.mFlags & 0x0800000000000000ULL)) {
		target->mStatus.mFlags &= ~0x0800000000000000ULL;
	}
	act->Reset();
}

static inline bool IsDiskBusy()
{
	bool result = false;
	if ((lbl_8025C400.mDvdStatus != 0 || lbl_8025C400.mPrevDvdStatus != 0) && lbl_8025C400.mDiskOk == 0) {
		result = true;
	}
	return result;
}

static inline bool IsGameRunning()
{
	bool result = false;
	if ((lbl_8025C400.mStatus.mFlags & 1) && !(lbl_8025C400.mStatus.mFlags & 0x22) && !IsDiskBusy()) {
		result = true;
	}
	return result;
}

static inline void KillGen(HSD_Generator*& gen)
{
	if (gen != NULL) {
		(&lbl_8025C400.mParticle)->Kill(gen);
		gen = NULL;
	}
}

static inline void ResetShot(XActPopperShot* shot)
{
	shot->mActive = FALSE;
	shot->mState  = 0;
	fn_80024E0C(&shot->mPtcl);
	shot->mRing[0].mAmount = 0.0f;
	shot->mRing[1].mAmount = 0.0f;
	KillGen(shot->mGen);
	KillGen(shot->mChargeGen);
}

static inline Vec UnitScale()
{
	Vec scale = { 1.0f, 1.0f, 1.0f };
	return scale;
}

// whether his hands are up at the top of the ladder
static inline bool AtLadderTop(XActLadder* act)
{
	if (act->mTarget != NULL) {
		s32 path[] = { 0 };
		if (14.0f + fn_8007E7C8(act->mOwner, 1, path).y >= act->mTarget->_6FC[0]) {
			return true;
		}
	}
	return false;
}

// drops the item he's carrying, it goes in his inventory
static inline void PutAway(XActLift* act)
{
	XObjChr* chr = act->mOwner;
	if (chr->mAnimId == 0x8B1 || chr->mAnimId == 0x8B3) {
		chr->SetAnim(0x8B2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	} else if (chr->mAnimId == 11) {
		chr->SetAnim(8, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}
	act->mState              = 0;
	act->mOwner->mAttachment = 0;
	act->mTarget->mStatus.mFlags &= ~0x0200000000000000ULL;
	act->mTarget->SetAnim(7, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
	act->_24       = 20;
	XObjItem* item = (XObjItem*)act->mTarget;
	fn_800DC9B0(&act->mOwner->mParams.mInventory, item->GetItem(), 1, TRUE);
	item->SetFlag();
	fn_8001E904(&lbl_8025C400.mSound, 0x39, 0xFF);
}

extern "C" {

// fades the radar's marker in while it has something, and spins it
/**
 * @note Address: 0x800BE648
 * @note Size: 0xF0
 */
void fn_800BE648(XActRadar* act, u32 frames)
{
	if (act->mState == 0 && act->_2C != NULL && act->mOwner->mTumble.mState == 0) {
		if (act->mFade < 1.0f) {
			if ((act->mFade += 0.05f * frames) > 1.0f) {
				act->mFade = 1.0f;
			}
		}
		act->mAngle[0] -= 0.08726646f * frames;
		act->mAngle[1] -= 0.08726646f * frames;
	} else if (act->mFade > 0.0f) {
		if ((act->mFade -= 0.05f * frames) < 0.0f) {
			act->mFade = 0.0f;
		}
	}
}

// the radar's pick, the nearest drawn object that isn't chibi robo himself
/**
 * @note Address: 0x800BE738
 * @note Size: 0x1F8
 */
XObj* fn_800BE738(XActRadar* act)
{
	XObj* obj;
	CVec pos        = act->mOwner->mMoveTranslate;
	XObj* nearest   = NULL;
	f32 nearestDist = NAN;
	for (int i = 0; i < lbl_8025C400.mXObjs.mObjectCount; i++) {
		obj = lbl_8025C400.GetObject(i);
		if ((obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 0x10000) && !(obj->mStatus.mFlags & 0x4000000000000000ULL)
		    && (obj->mStatus.mFlags & 0x80000) && (s32)act->mOwner->mId != (s32)obj->mId && obj->GetKind() < 10000) {
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
// NONMATCHING: control flow
void fn_800BFC04(XActRadar* act, const CPadOne* pad, f32* weight)
{
	if (act->mOwner->GetAttachment() == 3) {
		if (act->mState == 1 || act->mState == 2) {
			*weight = 0.0f;
		} else if (act->mState == 3 || act->mState == 4 || act->mState == 5) {
			*weight = 0.0f;
		}
	}
}

/**
 * @note Address: 0x800BFC6C
 * @note Size: 0xCC
 */
void fn_800BFC6C(XActRadar* act)
{
	act->Reset();
	act->_2C   = NULL;
	act->_30.x = act->_30.y = act->_30.z = 0.0f;
	act->_3C.x = act->_3C.y = act->_3C.z = 0.0f;
	act->_BC                             = NAN;
	fn_80023EE0(&act->mColor[0], 0, 0, 0, 0, 0xFF, 0x19, 0xFF, 0, 0.0f);
	fn_80023EE0(&act->mColor[1], 0, 0, 0, 0, 0xFF, 0x19, 0xFF, 0, 0.0f);
	act->mFade = 0.0f;
}

/**
 * @note Address: 0x800BFD38
 * @note Size: 0xE0
 */
void fn_800BFD38(XActRadar* act, XObjChr* owner)
{
	act->Init(owner);
	fn_800BFC6C(act);
}

// draws the popper's rings
/**
 * @note Address: 0x800BFE18
 * @note Size: 0x22C
 */
// NONMATCHING: regalloc
void fn_800BFE18(XActPopper* act)
{
	int i;
	XActPopperShot* shot;
	for (i = 0, shot = act->mShot; i < 10; i++, shot++) {
		for (int j = 0; j < 2; j++) {
			if (shot->mRing[j].mAmount > 0.0f) {
				f32 y;
				f32 x;
				fn_80015300(&lbl_8025C400, shot->mRing[j].mPos, &x, &y, NULL, NULL);
				u32 color  = shot->mRing[j].mColor;
				u32 edge   = color & 0xFFFFFF00;
				u32 center = (color & 0xFFFFFF00) | (u8)((color & 0xFF) * shot->mRing[j].mAmount);
				f32 inner  = shot->mRing[j].mRadius * shot->mRing[j].mAmount;
				f32 outer  = shot->mRing[j].mRadius * (1.0f - shot->mRing[j].mAmount);
				f32 glow;
				fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 1, 2, 4, 15);
				glow = 2.0f * inner;
				fn_80198258(&lbl_8025C400.mGs, 16, (u32)(255.0f * shot->mRing[j].mAmount) | 0xFFFFFF00, 0xFFFFFF00, x, y, 0.0f, glow, glow,
				            0.0f, 0.0f, 0.0f);
				glow = 1.5f * inner;
				fn_80198258(&lbl_8025C400.mGs, 16, (u32)(128.0f * shot->mRing[j].mAmount) | 0xFFFFFF00, 0xFFFFFF00, x, y, 0.0f, glow, glow,
				            0.0f, 0.0f, 0.0f);
				fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 1, 4, 5, 15);
				fn_80198258(&lbl_8025C400.mGs, 16, edge, center, x, y, 0.0f, outer, outer, 0.0f, 0.0f, 0.0f);
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
	for (i = 0, shot = act->mShot; i < 10; i++, shot++) {
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
// NONMATCHING: control flow
void fn_800C1610(XActPopper* act, const CPadOne* pad, f32* weight)
{
	s32 state = act->mState;
	if (state == 4) {
		*weight = 1.0f;
	} else if (state == 6) {
		*weight = 1.0f;
	} else if (state == 7 || state == 8 || state == 5) {
		*weight = 1.0f;
	} else if (state != 0) {
		*weight = 0.0f;
	}
}

/**
 * @note Address: 0x800C1674
 * @note Size: 0x148
 */
// NONMATCHING: regalloc
void fn_800C1674(XActPopper* act, XObjChr* owner)
{
	act->Init(owner);
	XActPopperShot* shot;
	int i;
	for (i = 0, shot = act->mShot; i < 10; shot++, i++) {
		ResetShot(shot);
		fn_80024FAC(&shot->mPtcl, shot->mPtcls, sizeof(CPtclDust2), 40);
		shot->mRing[0].mAmount = 0.0f;
		shot->mRing[1].mAmount = 0.0f;
		shot->mGen             = NULL;
		shot->mChargeGen       = NULL;
		if (shot->mJoint == NULL) {
			shot->mJoint = HSD_JObjAlloc();
		}
	}
	act->mCharge.mState     = 0;
	act->mCharge.mChargeGen = NULL;
	act->mCharge.mGen       = NULL;
	if (act->mCharge.mJoint == NULL) {
		act->mCharge.mJoint = HSD_JObjAlloc();
	}
}

/**
 * @note Address: 0x800C17BC
 * @note Size: 0xB0
 */
void fn_800C17BC(XActPopperCharge* charge)
{
	charge->mState = 0;
	charge->mTimer = 0.0f;
	KillGen(charge->mGen);
	KillGen(charge->mChargeGen);
	fn_8001E660(&lbl_8025C400.mSound, 0x186);
	fn_8001E660(&lbl_8025C400.mSound, 0x18B);
}

// fires a shot, kind 1 is the small one
/**
 * @note Address: 0x800C2A14
 * @note Size: 0x304
 */
// NONMATCHING: the position is read once too few
void fn_800C2A14(XActPopperShot* shot, const Vec& pos, s32 kind, void* attachment, f32 a, f32 b, f32 c, f32 d, f32 e, f32 strength)
{
	shot->mActive = TRUE;
	shot->mState  = 0;
	shot->mPos    = pos;
	shot->_14     = a;
	shot->_18     = b;
	shot->_1C     = d;
	shot->mPhase  = 0.0f;
	shot->_24     = e;
	shot->_28     = c;
	for (int i = 0; i < 10; i++) {
		shot->mHistory[i].x = shot->mPos.x;
		shot->mHistory[i].y = shot->mPos.y;
		shot->mHistory[i].z = shot->mPos.z;
	}
	shot->mRing[0].mPos.x  = pos.x;
	shot->mRing[0].mPos.y  = pos.y;
	shot->mRing[0].mPos.z  = pos.z;
	shot->mRing[0].mAmount = 0.9f;
	shot->mRing[0].mRadius = 100.0f;
	shot->mRing[0].mColor  = 0x00FF0096;
	shot->mStrength        = strength;
	shot->mKind            = kind;
	if (attachment != NULL) {
		shot->mAttachment = attachment;
	} else {
		shot->mAttachment = NULL;
	}
	shot->mGen = (&lbl_8025C400.mParticle)->Create((kind == 1 ? 0x4E : 0x4F) | 0x10000, 0, shot->mJoint, TRUE);
	if (strength >= (s32)fn_8006EBC8(31) / 100.0f) {
		shot->mChargeGen = (&lbl_8025C400.mParticle)->Create(0x10050, 0, shot->mJoint, TRUE);
	}
	f32 full = (s32)fn_8006EBC8(31) / 100.0f;
	fn_8001E904(&lbl_8025C400.mSound, strength >= full ? (kind == 1 ? 0x193 : 0x196) : (kind == 1 ? 0xB : 0x195), 0xFF);
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

// flies him while the copter is out
/**
 * @note Address: 0x800C2EB8
 * @note Size: 0x738
 */
void fn_800C2EB8(XActCopter* act, s32 anim, HSD_AObj* aobj, u32 frames, f32 frame, f32 end)
{
	if (act->mState == 0) {
		act->mTime = FRAME_RATE * ((s32)fn_8006EAF0(0) / 100.0f);
	} else if (act->mState == 3) {
		if ((act->mTime -= frames) < 0.0f) {
			act->mTime  = 0.0f;
			act->mState = 4;
		}
	}
	switch (act->mState) {
	case 1:
		if (anim != 0x814) {
			act->mOwner->SetAnim(0x814, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			fn_800D7BF0(&act->mOwner->mCopterObj);
		} else if (aobj->flags & 0x44000000) {
			act->mOwner->SetAnim(0x4A, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			act->mState = 0;
		}
		break;
	case 2:
		if (anim != 0x817) {
			act->mOwner->SetAnim(0x817, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			fn_800D7B68(&act->mOwner->mCopterObj);
		} else if (aobj->flags & 0x44000000) {
			XObjChr* chr = act->mOwner;
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
			act->mOwner->SetAnim(0x28, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			fn_800D7AC8(&act->mOwner->mCopterObj);
		}
		if (act->mOwner->GetHeight() <= 0.1f) {
			act->mOwner->SetAnim(0x816, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			fn_800D7A78(&act->mOwner->mCopterObj);
			act->mState = 6;
		}
		break;
	case 5:
		fn_800C2DB0(act, frames, -1.0f);
		if (anim != 0x28) {
			act->mOwner->SetAnim(0x28, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			act->mOwner->mCopterObj.SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
		if (act->mOwner->GetHeight() <= 0.0f || (act->mOwner->GetHeight() <= 1.0f && fn_800C35F0(act->mOwner).Length() > 0.5f)) {
			act->mOwner->SetAnim(0x816, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			fn_800D7A78(&act->mOwner->mCopterObj);
			act->mState = 6;
		}
		break;
	case 6:
		fn_800C2DB0(act, frames, -1.0f);
		if (aobj->flags & 0x44000000) {
			XObjChr* chr = act->mOwner;
			chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			chr->_A2C   = 3;
			act->mState = 0;
		}
		break;
	case 7:
		if (anim != 0x38) {
			act->mOwner->SetAnim(0x38, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			fn_800D7AC8(&act->mOwner->mCopterObj);
		}
		if (act->mOwner->GetHeight() <= 0.1f) {
			act->mOwner->SetAnim(0x59, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			fn_800D7A78(&act->mOwner->mCopterObj);
			act->mState = 9;
		}
		break;
	case 8:
		if (anim != 0x1A) {
			act->mOwner->SetAnim(0x1A, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			fn_800D7AC8(&act->mOwner->mCopterObj);
		}
		if (act->mOwner->GetHeight() <= 0.1f) {
			act->mOwner->SetAnim(0x1B, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			fn_800D7A78(&act->mOwner->mCopterObj);
			act->mState = 9;
		}
		break;
	case 9:
		if (aobj->flags & 0x44000000) {
			XObjChr* chr = act->mOwner;
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
		fn_8009F4F8(act->mOwner, 1, path, 0.0f, act->mBob, 0.0f);
	} else {
		if (act->mBob > 0.0f) {
			if ((act->mBob -= 0.2f * frames) <= 0.0f) {
				act->mBob = 0.0f;
			}
		}
		s32 path[] = { 0 };
		fn_8009F4F8(act->mOwner, 1, path, 0.0f, act->mBob, 0.0f);
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
void fn_800C361C(XActCopter* act, const CPadOne* pad, f32* weight)
{
	if (act->mState == 3) {
		if ((pad->mTrigger & 0x200) || fn_8009FEDC(act->mOwner)) {
			act->mState = 4;
			fn_800D7AC8(&act->mOwner->mCopterObj);
		}
		*weight = 1.0f;
	} else if (act->mState == 4 || act->mState == 7 || act->mState == 8) {
		if (!(pad->mTrigger & 0x200)) {
			fn_8009FEDC(act->mOwner);
		}
		*weight = 1.0f;
	} else if (act->mState != 0) {
		*weight = 0.0f;
	}
}

// the ring on the floor while he tumbles
/**
 * @note Address: 0x800C36E4
 * @note Size: 0x2F0
 */
// NONMATCHING: regalloc and stack order
void fn_800C36E4(XActTumbleFade* fade, const Vec& pos, f32 angle)
{
	if (fade->mAmount > 0.0f) {
		CVec offset(fade->mOffset);
		Mtx m;
		PSMTXRotRad(m, 'y', angle);
		PSMTXMultVec(m, &offset, &offset);
		Vec center = RingPos(pos, offset);
		f32 size   = 6.0f * fade->mAmount;
		u32 color  = (u32)(160.0f * fade->mAmount) | 0x641E00;
		Vec rot    = { -HALF_PI, 0.0f, 0.0f };
		fn_80198258(&lbl_8025C400.mGs, 0x18, color, color, center.x, center.y, center.z, size, size, rot.x, rot.y, rot.z);
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
		fn_801988EC(&lbl_8025C400.mGs, GX_TRIANGLESTRIP, lbl_8025C400.mGs.mCamera->mViewMtx, verts, 50);
	}
}

/**
 * @note Address: 0x800C39D4
 * @note Size: 0xAC
 */
void fn_800C39D4(XActTumbleFade* fade, BOOL increase, u32 frames)
{
	if (fade->mActive) {
		if (increase) {
			if (fade->mAmount < 1.0f) {
				if ((fade->mAmount += 0.01f * frames) > 1.0f) {
					fade->mAmount = 1.0f;
				}
			}
		} else if (fade->mAmount > 0.0f) {
			if ((fade->mAmount -= 0.05f * frames) < 0.0f) {
				fade->mAmount = 0.0f;
				fade->mActive = FALSE;
			}
		}
	}
}

/**
 * @note Address: 0x800C4468
 * @note Size: 0x4A4
 */
void fn_800C4468(XActTumble* act, const CPadOne* pad, f32* weight)
{
	if (act->mState == 2) {
		if (IsGameRunning() && lbl_8025C400.mEvt.mData.mScript == NULL) {
			if ((pad->mTrigger & 0x100) || (pad->mTrigger & 0x200) || StickLength(pad) > 0.0f) {
				act->mState = 3;
			}
		}
		*weight = 0.0f;
	} else if (act->mState == 0x10) {
		if (IsGameRunning() && lbl_8025C400.mEvt.mData.mScript == NULL) {
			if ((pad->mTrigger & 0x100) || (pad->mTrigger & 0x200) || StickLength(pad) > 0.0f) {
				act->mState = 0x11;
			}
		}
		*weight = 0.0f;
	} else if (act->mState == 5) {
		if (IsGameRunning() && lbl_8025C400.mEvt.mData.mScript == NULL) {
			if ((pad->mTrigger & 0x100) || (pad->mTrigger & 0x200)) {
				act->mState = 6;
			}
		}
		*weight = 0.0f;
	} else if (act->mState == 7) {
		*weight = 0.8f;
	} else if (act->mState != 0) {
		*weight = 0.0f;
	}
}

/**
 * @note Address: 0x800C490C
 * @note Size: 0x40
 */
void fn_800C490C(XActTumble* act)
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
				f32 angle = fn_8008DF34(act->mTarget, &act->mOwner->mMoveTranslate, act->mTargetFloor, 1, 0, 0);
				if (fpclassify(angle) != FP_NAN) {
					act->mOwner->SetAnim(0x3E, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					act->mOwner->SetRotationY(angle, 0);
				} else {
					Release(act);
					act->mState = 0;
				}
			}
		} else if (aobj->flags & 0x44000000) {
			act->mOwner->SetAnim(0x3F, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			act->mState = 2;
			act->_24    = 1;
		}
		break;
	case 2:
		if (act->_0C == 1) {
			if (anim == 0x4B) {
				if (aobj->flags & 0x44000000) {
					act->mOwner->SetAnim(0x40, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					act->_24 = 2;
				}
			} else if (anim != 0x40) {
				act->mOwner->SetAnim(0x4B, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				act->_24 = 0;
			}
		} else if (act->_0C == -1) {
			if (anim == 0x4C) {
				if (aobj->flags & 0x44000000) {
					act->mOwner->SetAnim(0x41, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					act->_24 = 2;
				}
			} else if (anim != 0x41) {
				act->mOwner->SetAnim(0x4C, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				act->_24 = 0;
			}
		} else if (anim != 0x3F) {
			act->mOwner->SetAnim(0x3F, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			act->_24 = 0;
		}
		break;
	case 3:
		if (anim != 0x42) {
			act->mOwner->SetAnim(0x42, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		} else if (aobj->flags & 0x44000000) {
			XObjChr* chr = act->mOwner;
			chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			chr->_A2C   = 3;
			act->mState = 0;
		}
		break;
	}
}

// lets go of what he's pushing once a is let go, japan uses b while the game flag is up
/**
 * @note Address: 0x800C56D4
 * @note Size: 0x248
 */
void fn_800C56D4(XActPush* act, const CPadOne* pad, f32* weight)
{
	if (act->mState == 2) {
		if (IsGameRunning() && lbl_8025C400.mEvt.mData.mScript == NULL) {
#if defined(VERSION_JP)
			if (lbl_8025C400.mStatus.mFlags & 0x0000080000000000ULL) {
				if (pad->mTrigger & 0x200) {
					Release(act);
					act->mState = 3;
					*weight     = 0.0f;
				}
			} else
#endif
			    if (!(pad->mButton & 0x100)) {
				Release(act);
				act->mState = 3;
				*weight     = 0.0f;
			}
		}
	} else if (act->mState != 0) {
		if (act->mState != 3 && !(pad->mButton & 0x100)
#if defined(VERSION_JP)
		    && !(lbl_8025C400.mStatus.mFlags & 0x0000080000000000ULL)
#endif
		) {
			Release(act);
			XObjChr* chr = act->mOwner;
			chr->SetAnim(17, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			chr->_A2C   = 3;
			act->mState = 0;
		}
		*weight = 0.0f;
	}
}

/**
 * @note Address: 0x800C591C
 * @note Size: 0xE4
 */
void fn_800C591C(XActPush* act)
{
	if (act->mState != 0) {
		Release(act);
		act->mState  = 0;
		XObjChr* chr = act->mOwner;
		chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
		chr->_A2C = 3;
	}
}

/**
 * @note Address: 0x800C5A00
 * @note Size: 0x7C
 */
void fn_800C5A00(XActPush* act)
{
	Release(act);
}

// walks him around the target at the same distance
/**
 * @note Address: 0x800C5A7C
 * @note Size: 0x11C
 */
// NONMATCHING: stack order
void fn_800C5A7C(XActPush* act, f32 angle)
{
	if (act->mTarget != NULL) {
		Vec flat = { act->_10.x, 0.0f, act->_10.z };
		CVec dir(flat);
		f32 dist = dir.Length();
		act->mOwner->SetTranslateX(act->mTarget->mMoveTranslate.x - dist * (f32)sin(angle), 0);
		act->mOwner->SetTranslateZ(act->mTarget->mMoveTranslate.z - dist * (f32)cos(angle), 0);
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
 * @note Address: 0x800C7AC4
 * @note Size: 0x100
 */
void fn_800C7AC4(XActRope* act, const CPadOne* pad, f32* weight)
{
	if (act->mState == 5) {
		if (IsGameRunning() && lbl_8025C400.mEvt.mData.mScript == NULL) {
			if (pad->mTrigger & 0x200) {
				act->mState = 8;
			}
			if (act->mOwner->mAnimId == 0x8A || act->mOwner->mAnimId == 0x8C) {
				*weight = 0.0f;
			}
		}
	} else if (act->mState != 0) {
		*weight = 0.0f;
	}
}

/**
 * @note Address: 0x800C7BC4
 * @note Size: 0x1FB0
 */
// NONMATCHING: the target is read once too few
void fn_800C7BC4(XActLadder* act, s32 anim, HSD_AObj* aobj, u32 frames, f32 frame, f32 end)
{
	switch (act->mState) {
	case 1:
		if (anim != 0x66) {
			act->mOwner->SetAnim(0x66, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			if (act->mTarget != NULL) {
				act->mOwner->SetRotationY(WrapAngle(PI + act->mTarget->mMoveRotate.y), 0);
				CVec chrPos(act->mOwner->mMoveTranslate);
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
			act->mOwner->SetPos(fn_8007E7C8(act->mOwner, 1, path), 0);
			fn_800C9C34(act->mOwner);
			s32 path2[] = { 0 };
			fn_8009F4F8(act->mOwner, 1, path2, 0.0f, 0.0f, 0.0f);
			act->mOwner->SetAnim(0x67, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			act->mState = 5;
		} else {
			if (act->mTarget != NULL) {
				Vec move = act->mTarget->mMoveTranslate - act->mTarget->_264;
				act->mPos.x += move.x;
				act->mPos.z += move.z;
				if (act->mDist > 0.0f) {
					if ((act->mDist -= 0.2f * frames) < 0.0f) {
						act->mDist = 0.0f;
					}
					act->mOwner->SetTranslateX(act->mDist * (f32)sin(act->mAngle) + act->mPos.x, 0);
					act->mOwner->SetTranslateZ(act->mDist * (f32)cos(act->mAngle) + act->mPos.z, 0);
				} else {
					act->mOwner->SetTranslateX(act->mPos.x, 0);
					act->mOwner->SetTranslateZ(act->mPos.z, 0);
				}
			}
			if (CheckStageFlag(lbl_8025C400.mStage, 1)) {
				XObjPlug* plug = &act->mOwner->mPlug;
				if (plug->_D3C != 0 && frame >= 25.0f) {
					fn_800B2BD0(plug, TRUE);
					fn_8001E904(&lbl_8025C400.mSound, 0x37, 0xFF);
				}
			}
		}
		break;
	case 2:
		if (anim != 0x6D) {
			act->mOwner->SetAnim(0x6D, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			if (act->mTarget != NULL) {
				act->mOwner->SetRotationY(WrapAngle(act->mTarget->mMoveRotate.y), 0);
				CVec chrPos(act->mOwner->mMoveTranslate);
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
			act->mOwner->SetPos(fn_8007E7C8(act->mOwner, 1, path), 0);
			fn_800C9C34(act->mOwner);
			s32 path2[] = { 0 };
			fn_8009F4F8(act->mOwner, 1, path2, 0.0f, 0.0f, 0.0f);
			act->mOwner->SetRotationY(PI + act->mOwner->mMoveRotate.y, 0);
			s32 path3[] = { 0 };
			fn_800BB3D0(act->mOwner, 1, path3, 0.0f);
			act->mOwner->SetAnim(0x68, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			act->mState = 5;
		} else {
			if (act->mTarget != NULL) {
				Vec move = act->mTarget->mMoveTranslate - act->mTarget->_264;
				act->mPos.x += move.x;
				act->mPos.z += move.z;
				if (act->mDist > 0.0f && frame > 20.0f) {
					if ((act->mDist -= 0.2f * frames) < 0.0f) {
						act->mDist = 0.0f;
					}
					act->mOwner->SetTranslateX(act->mDist * (f32)sin(act->mAngle) + act->mPos.x, 0);
					act->mOwner->SetTranslateZ(act->mDist * (f32)cos(act->mAngle) + act->mPos.z, 0);
				}
			}
			if (CheckStageFlag(lbl_8025C400.mStage, 1)) {
				XObjPlug* plug = &act->mOwner->mPlug;
				if (plug->_D3C != 0 && frame >= 50.0f) {
					fn_800B2BD0(plug, TRUE);
					fn_8001E904(&lbl_8025C400.mSound, 0x37, 0xFF);
				}
			}
		}
		break;
	case 3:
		if (anim != 0x87) {
			if (act->mTarget != NULL) {
				if (act->mTarget->mFloors[act->mTargetFloor].mId == 0) {
					CVec chrPos(act->mOwner->mMoveTranslate);
					chrPos.y = 0.0f;
					CVec pt(fn_800D5190(act));
					pt.y = 0.0f;
					act->mOwner->SetRotationY(PI + (f32)atan2(pt.x - chrPos.x, pt.z - chrPos.z), 0);
				} else {
					act->mOwner->SetRotationY(act->mTarget->mMoveRotate.y, 0);
				}
				act->mOwner->SetAnim(0x87, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				act->mOwner->SetAnim(0x87, TRUE, NULL, NULL, NULL, fn_800A2CE4(act->mOwner), 1.0f, 1.0f, 10.0f);
				act->mOwner->UpdateAnim(0);
				s32 path[] = { 0 };
				CVec d     = fn_8007E7C8(act->mOwner, 1, path) - act->mOwner->mMoveTranslate;
				act->mOwner->SetTranslateX(act->mOwner->mMoveTranslate.x - d.x, 0);
				act->mOwner->SetTranslateZ(act->mOwner->mMoveTranslate.z - d.z, 0);
				act->mOwner->SetTranslateY(act->mTarget->_77C, 0);
				act->mOwner->SetAnim(0x87, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				CVec chrPos(act->mOwner->mMoveTranslate);
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
				XObjPlug* plug = &act->mOwner->mPlug;
				if (plug->_D3C != 0) {
					fn_800B2BD0(plug, TRUE);
					fn_8001E904(&lbl_8025C400.mSound, 0x37, 0xFF);
				}
			}
		} else if (aobj->flags & 0x44000000) {
			s32 path[]      = { 0 };
			HSD_JObj* joint = fn_800B2764(act->mOwner, 1, path);
			f32 turn        = HSD_JObjGetRotationY(joint);
			f32 facing      = HSD_JObjGetRotationY(act->mOwner->mJObj);
			act->mOwner->SetRotationY(facing + turn, 0);
			fn_800C9B74(joint, 0.0f);
			s32 path2[] = { 0 };
			CVec d      = fn_8007E7C8(act->mOwner, 1, path2) - act->mOwner->mMoveTranslate;
			f32 angle   = HSD_JObjGetRotationY(act->mOwner->mJObj);
			d.x         = -d.x;
			d.z         = -d.z;
			act->mOwner->SetPos((act->mOwner->mMoveTranslate + d), 0);
			fn_800C9C34(act->mOwner);
			s32 path3[] = { 0 };
			fn_8009F4F8(act->mOwner, 1, path3, 0.0f, 0.0f, 0.0f);
			act->mOwner->SetAnim(0x68, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			act->mState = 5;
			act->_0C    = 0;
		} else if (act->mTarget != NULL) {
			Vec move = act->mTarget->mMoveTranslate - act->mTarget->_264;
			act->mPos.x += move.x;
			act->mPos.z += move.z;
			if (act->mDist > 0.0f) {
				if ((act->mDist -= 0.4f * frames) < 0.0f) {
					act->mDist = 0.0f;
				}
				act->mOwner->SetTranslateX(act->mDist * (f32)sin(act->mAngle) + act->mPos.x, 0);
				act->mOwner->SetTranslateZ(act->mDist * (f32)cos(act->mAngle) + act->mPos.z, 0);
			} else {
				act->mOwner->SetTranslateX(act->mPos.x, 0);
				act->mOwner->SetTranslateZ(act->mPos.z, 0);
			}
			act->mOwner->SetTranslateY(act->mTarget->_77C, 0);
		}
		break;
	case 4:
		act->mOwner->SetAnim(0x68, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
		act->mState = 5;
		break;
	case 5:
		if (anim == 0x69 || anim == 0x6A || anim == 0x6B || anim == 0x6C) {
			if (aobj->flags & 0x44000000) {
				s32 path[] = { 0 };
				CVec hand  = fn_800BAD80(act->mOwner, 1, path);
				if (hand.y < 0.0f) {
					hand.y = (s32)(hand.y - 0.5f);
				}
				s32 path2[] = { 0 };
				fn_800C7810(act->mOwner, 1, path2, hand);
				s32 path3[] = { 0 };
				act->mOwner->SetPos(fn_8007E7C8(act->mOwner, 1, path3), 0);
				fn_800C9C34(act->mOwner);
				s32 path4[] = { 0 };
				fn_8009F4F8(act->mOwner, 1, path4, 0.0f, 0.0f, 0.0f);
				if (act->_0C == 1) {
					if (AtLadderTop(act)) {
						act->mState = 6;
					} else if (anim == 0x69 || anim == 0x6B) {
						act->mOwner->SetAnim(0x6A, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					} else if (anim == 0x6A || anim == 0x6C) {
						act->mOwner->SetAnim(0x69, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					}
				} else if (act->_0C == -1) {
					if (act->mOwner->GetHeight() <= 2.0f) {
						act->mState = 7;
					} else if (anim == 0x69 || anim == 0x6B) {
						act->mOwner->SetAnim(0x6C, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					} else if (anim == 0x6A || anim == 0x6C) {
						act->mOwner->SetAnim(0x6B, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					}
				} else if (anim == 0x69 || anim == 0x6B) {
					act->mOwner->SetAnim(0x68, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				} else if (anim == 0x6A || anim == 0x6C) {
					act->mOwner->SetAnim(0x67, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				}
				if (act->_2C && act->mState == 5) {
					act->mState = 9;
				}
			}
		} else {
			if (act->_0C == 1) {
				if (AtLadderTop(act)) {
					act->mState = 6;
				} else if (anim == 0x67) {
					act->mOwner->SetAnim(0x69, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				} else if (anim == 0x68) {
					act->mOwner->SetAnim(0x6A, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				}
			} else if (act->_0C == -1) {
				if (act->mOwner->GetHeight() <= 2.0f) {
					act->mState = 7;
				} else if (anim == 0x67) {
					act->mOwner->SetAnim(0x6B, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				} else if (anim == 0x68) {
					act->mOwner->SetAnim(0x6C, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				}
			}
			if (act->_2C && act->mState == 5) {
				act->mState = 9;
			}
		}
		break;
	case 6:
		if (!(anim == 0x6E || anim == 0x6F)) {
			if (anim == 0x69 || anim == 0x6B) {
				act->mOwner->SetAnim(0x6F, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x6A || anim == 0x6C) {
				act->mOwner->SetAnim(0x6E, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x67) {
				act->mOwner->SetAnim(0x6E, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x68) {
				act->mOwner->SetAnim(0x6F, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
		} else if (aobj->flags & 0x44000000) {
			s32 path[] = { 0 };
			act->mOwner->SetPos(fn_8007E7C8(act->mOwner, 1, path), 0);
			fn_800C9C34(act->mOwner);
			s32 path2[] = { 0 };
			fn_8009F4F8(act->mOwner, 1, path2, 0.0f, 0.0f, 0.0f);
			XObjChr* chr = act->mOwner;
			chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			chr->_A2C    = 3;
			act->mState  = 0;
			act->mTarget = NULL;
		}
		break;
	case 7:
		if (!(anim == 0x70 || anim == 0x71)) {
			if (anim == 0x69 || anim == 0x6B) {
				act->mOwner->SetAnim(0x71, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x6A || anim == 0x6C) {
				act->mOwner->SetAnim(0x70, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x67) {
				act->mOwner->SetAnim(0x70, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x68) {
				act->mOwner->SetAnim(0x71, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
		} else if (aobj->flags & 0x44000000) {
			s32 path[] = { 0 };
			act->mOwner->SetPos(fn_8007E7C8(act->mOwner, 1, path), 0);
			fn_800C9C34(act->mOwner);
			s32 path2[] = { 0 };
			fn_8009F4F8(act->mOwner, 1, path2, 0.0f, 0.0f, 0.0f);
			XObjChr* chr = act->mOwner;
			chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			chr->_A2C    = 3;
			act->mState  = 0;
			act->mTarget = NULL;
		}
		break;
	case 8:
		if (anim == 0x69 || anim == 0x6B) {
			act->mOwner->SetAnim(0x73, TRUE, NULL, NULL, NULL, 10.0f, 1.0f, 1.0f, 10.0f);
		} else if (anim == 0x6A || anim == 0x6C) {
			act->mOwner->SetAnim(0x72, TRUE, NULL, NULL, NULL, 10.0f, 1.0f, 1.0f, 10.0f);
		} else if (anim == 0x67) {
			act->mOwner->SetAnim(0x72, TRUE, NULL, NULL, NULL, 10.0f, 1.0f, 1.0f, 10.0f);
		} else if (anim == 0x68) {
			act->mOwner->SetAnim(0x73, TRUE, NULL, NULL, NULL, 10.0f, 1.0f, 1.0f, 10.0f);
		}
		act->mState = 9;
	case 9:
		if (!(anim == 0x72 || anim == 0x73)) {
			if (anim == 0x69 || anim == 0x6B) {
				act->mOwner->SetAnim(0x73, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x6A || anim == 0x6C) {
				act->mOwner->SetAnim(0x72, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x67) {
				act->mOwner->SetAnim(0x72, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x68) {
				act->mOwner->SetAnim(0x73, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
		} else if (aobj->flags & 0x44000000) {
			s32 path[] = { 0 };
			act->mOwner->SetPos(fn_8007E7C8(act->mOwner, 1, path), 0);
			fn_800C9C34(act->mOwner);
			s32 path2[] = { 0 };
			fn_8009F4F8(act->mOwner, 1, path2, 0.0f, 0.0f, 0.0f);
			if (act->mOwner->GetAttachment() == 1) {
				act->mState                 = 0;
				act->mOwner->mCopter.mState = 4;
			} else {
				act->mOwner->SetAnim(0x74, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				act->mState = 10;
			}
			act->mTarget = NULL;
		}
		break;
	case 10:
		if (act->mOwner->GetHeight() <= 0.0f) {
			if (act->mOwner->_9A90 > 30.0f) {
				act->mState = 0;
				act->mOwner->SetAnim(0x8E, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				act->mOwner->mJump.mState = 5;
				fn_800C9D54(&act->mOwner->mJump, act->mOwner->_9A90);
				act->mOwner->vf12C(1, 0xFF, 0, 0, 0.0f);
				act->mOwner->vf12C(1, 0, 0, 0, 0.3f);
				fn_80023EE0(&lbl_8025C400.mScrnTex.mColor, 0xFF, 0, 0, 0, 100, 100, 200, 0, 0.3f);
			} else {
				act->mOwner->SetAnim(0x75, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				act->mState = 11;
			}
		}
		break;
	case 11:
		if (aobj->flags & 0x44000000) {
			XObjChr* chr = act->mOwner;
			chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			chr->_A2C   = 3;
			act->mState = 0;
		}
		break;
	}
}

/**
 * @note Address: 0x800C9B74
 * @note Size: 0xC0
 */
void fn_800C9B74(HSD_JObj* jobj, f32 angle)
{
	HSD_JObjSetRotationY(jobj, angle);
}

/**
 * @note Address: 0x800C9C34
 * @note Size: 0x3C
 */
void fn_800C9C34(XObj* obj)
{
	if (obj->mMoveTranslate.y != obj->_264.y) {
		obj->mStatus.mFlags |= 0x0010000000000000ULL;
	}
	obj->_264.x = obj->mMoveTranslate.x;
	obj->_264.y = obj->mMoveTranslate.y;
	obj->_264.z = obj->mMoveTranslate.z;
}

/**
 * @note Address: 0x800C9C70
 * @note Size: 0xE4
 */
void fn_800C9C70(XActLadder* act, const CPadOne* pad, f32* weight)
{
	act->_2C = FALSE;
	if (act->mState == 5) {
		if (IsGameRunning() && lbl_8025C400.mEvt.mData.mScript == NULL && (pad->mTrigger & 0x200)) {
			act->_2C = TRUE;
		}
	} else if (act->mState != 0) {
		*weight = 0.0f;
	}
}

// the jump's fall, a long one makes him drop what he carries
/**
 * @note Address: 0x800C9D54
 * @note Size: 0x360
 */
void fn_800C9D54(XActJump* act, f32 strength)
{
	if (lbl_8025C400.mEvt.mData.mScript != NULL) {
		act->mTargetStrength = 0.0f;
		act->mStrength       = 0.0f;
	} else {
		act->mTargetStrength = strength;
		act->mStrength       = strength;
		if (act->mTargetStrength >= 50.0f) {
			XObjChr* chr = act->mOwner;
			if (chr->mLift.mState != 0 && IsCarrying(&chr->mLift, 22)) {
				PutAway(&chr->mLift);
				if (act->mTargetStrength >= 60.0f) {
					act->mState                 = 0;
					act->mOwner->mTumble.mState = 1;
					act->mOwner->SetAnim(0x35, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				}
			}
		}
	}
}

/**
 * @note Address: 0x800CBDB4
 * @note Size: 0x18
 */
void fn_800CBDB4(XActJump* act, const CPadOne* pad, f32* weight)
{
	if (act->mState != 0) {
		*weight = 0.0f;
	}
}

/**
 * @note Address: 0x800CE120
 * @note Size: 0x214
 */
void fn_800CE120(XActHang* act, const CPadOne* pad, f32* weight)
{
	act->_2C = FALSE;
	if (act->mState == 4) {
		f32 stick = -(f32)atan2(pad->mStickX, pad->mStickY);
		f32 angle = WrapAngle(stick + lbl_8025C400.mMotionControlCamera.mCamera.mAngle.y - act->mOwner->mMoveRotate.y);
		if (!(angle < 0.5235988f) && !(angle > 5.7595863f)) {
			act->_2C = TRUE;
		}
		*weight = 0.0f;
	} else if (act->mState == 5) {
		if (IsGameRunning() && lbl_8025C400.mEvt.mData.mScript == NULL) {
			if (pad->mTrigger & 0x100) {
				if (act->mTarget == NULL || (act->mTarget->mStatus.mFlags & 0x400000000ULL)) {
					fn_800CDBA8(act);
					act->_0C = 0;
				}
				*weight = 0.0f;
			} else if (pad->mTrigger & 0x200) {
				act->mState = 9;
				act->_0C    = 0;
				*weight     = 0.0f;
			}
		}
	} else if (act->mState != 0) {
		*weight = 0.0f;
	}
}

/**
 * @note Address: 0x800CE334
 * @note Size: 0x20
 */
void fn_800CE334(XActLift* act, u32 value, BOOL apply)
{
	act->_54 = value;
	if (apply) {
		act->mOwner->mPlug._D3C = value | 0x10000;
	}
}

// puts the carried object where he holds it
/**
 * @note Address: 0x800CE904
 * @note Size: 0xA4
 */
void fn_800CE904(XActLift* act, XObj* target)
{
	target->SetPos(fn_800A0978(act->mOwner) - fn_800CE9A8(act, target), 0);
}

/**
 * @note Address: 0x800CE9A8
 * @note Size: 0x354
 */
// NONMATCHING: stack order
Vec fn_800CE9A8(XActLift* act, XObj* target)
{
	if (target->GetCategory() == 0x18) {
		s32 path[] = { 0 };
		CJObj joint(GetJointByPath(target->mJObj, path, 1));
		Vec pos = GetJointPosition(joint.mJObj);
		return CVec(pos) - target->mMoveTranslate;
	}
	CVec offset = target->mMoveTranslate * -1.0f;
	return fn_8008C274(target, act->mTargetFloor, &offset);
}

/**
 * @note Address: 0x800D0D3C
 * @note Size: 0x90
 */
void fn_800D0D3C(HSD_JObj* jobj)
{
	HSD_JObjClearMtxFlags(jobj);
}

/**
 * @note Address: 0x800D0E8C
 * @note Size: 0xC0
 */
void fn_800D0E8C(HSD_JObj* jobj, MtxPtr mtx)
{
	HSD_JObjCopyMtx(jobj, mtx);
}

/**
 * @note Address: 0x800D3A5C
 * @note Size: 0x94
 */
HSD_Generator* fn_800D3A5C(CGame* game, u32 bank, u32 id, s32 status, const Vec& pos, BOOL keep)
{
	Vec rot = { 0.0f, 0.0f, 0.0f };
	return (&game->mParticle)->CreateSRT(id | (bank << 16), status, pos, UnitScale(), rot, keep);
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
 * @note Address: 0x800D3BA4
 * @note Size: 0x5C
 */
HSD_Generator* fn_800D3BA4(CGame* game, u32 bank, u32 id, s32 status, const Vec& pos, const Vec& scale, BOOL keep)
{
	Vec rot = { 0.0f, 0.0f, 0.0f };
	return (&game->mParticle)->CreateSRT(id | (bank << 16), status, pos, scale, rot, keep);
}

/**
 * @note Address: 0x800D3C00
 * @note Size: 0xB8
 */
Vec fn_800D3C00(const CJObj* obj)
{
	HSD_JObj* jobj = obj->mJObj;
	Vec scale;
	HSD_MtxGetScale(HSD_JObjGetMtxPtr(jobj), &scale);
	return scale;
}

/**
 * @note Address: 0x800D3CB8
 * @note Size: 0x24
 */
s32 fn_800D3CB8(Params* params, s16 id, s32 amount)
{
	return fn_800DC754(&params->mInventory, id, amount);
}

/**
 * @note Address: 0x800D3CDC
 * @note Size: 0x30
 */
BOOL fn_800D3CDC(XObj* obj)
{
	return !(obj->mStatus.mFlags & 0x0040000000000000ULL);
}

/**
 * @note Address: 0x800D3D0C
 * @note Size: 0x64
 */
XObj* fn_800D3D0C(CGame* game, s16 id)
{
	CoordInfo info(fn_8006EDB4(id));
	return fn_80132698(&game->mXObjs, &info, 0, TRUE);
}

/**
 * @note Address: 0x800D3D70
 * @note Size: 0x44
 */
void fn_800D3D70(Params* params, f32 battery)
{
	params->mBattery = battery > 0.0f ? battery : 0.0f;
	if (params->mBattery < 0.0f) {
		params->mBattery = 0.0f;
	}
	if (params->mBattery > params->mMaxBattery) {
		params->mBattery = params->mMaxBattery;
	}
}

/**
 * @note Address: 0x800D3DB4
 * @note Size: 0x20
 */
void fn_800D3DB4(CGame* game, u32 flags)
{
	game->mSubwin.mEnabled      = 1;
	game->mSubwin.mFlags.mFlags = 0;
	game->mSubwin.mFlags.mFlags |= flags;
}

/**
 * @note Address: 0x800D3DD4
 * @note Size: 0x24
 */
void fn_800D3DD4(CGame* game, const Vec& interest, f32 angleY, f32 angleX, f32 distance, f32 fov)
{
	fn_800136B4(&game->mSubwin, interest, angleY, angleX, distance, fov);
}

/**
 * @note Address: 0x800D3DF8
 * @note Size: 0xB8
 */
Vec fn_800D3DF8(const CJObj* obj)
{
	HSD_JObj* jobj = obj->mJObj;
	Vec pos;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(jobj), &pos);
	return pos;
}

/**
 * @note Address: 0x800D3EB0
 * @note Size: 0x14
 */
void fn_800D3EB0(CGame* game, f32 a, f32 b, f32 c, f32 d)
{
	game->mSubwin._134 = a;
	game->mSubwin._138 = b;
	game->mSubwin._13C = c;
	game->mSubwin._140 = d;
}

/**
 * @note Address: 0x800D3EC4
 * @note Size: 0x50
 */
void fn_800D3EC4(XObj* obj)
{
	obj->mAnmQueue.Clear();
}

// picks the joint's srt up into the move values
/**
 * @note Address: 0x800D40EC
 * @note Size: 0x164
 */
void fn_800D40EC(CXObj* obj)
{
	union {
		Vec vec;
		Quaternion quat;
	} value;
	HSD_JObjGetTranslation(obj->mJObj, &value.vec);
	obj->mMoveTranslate.Reset(value.vec);
	HSD_JObjGetRotation(obj->mJObj, (Quaternion*)&value.vec);
	obj->mMoveRotate.Reset(value.vec);
	HSD_JObjGetScale(obj->mJObj, &value.vec);
	obj->mMoveScale.Reset(value.vec);
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
void fn_800D4294(XObjChr* chr, s32 anim, f32 blend)
{
	chr->SetAnim(anim, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, blend, 10.0f);
	chr->_A2C = 3;
}

/**
 * @note Address: 0x800D42F4
 * @note Size: 0x3C
 */
HSD_Generator* fn_800D42F4(CGame* game, s32 bank, s32 id, u8 type, HSD_JObj* jobj, BOOL keep)
{
	return (&game->mParticle)->Create(id | (bank << 16), type, jobj, keep);
}

/**
 * @note Address: 0x800D4330
 * @note Size: 0x10
 */
s32 fn_800D4330(Params* params, u32 index)
{
	return params->mInventory.mItems[index].mCount;
}

/**
 * @note Address: 0x800D4340
 * @note Size: 0x28
 */
s16 fn_800D4340(XObj* obj)
{
	return obj->GetItem();
}

/**
 * @note Address: 0x800D4368
 * @note Size: 0x48C
 */
void fn_800D4368(XActLift* act, const CPadOne* pad, f32* weight)
{
	if (act->mState == 4) {
		XObjChr* chr = act->mOwner;
		if (chr->mJump.mState != 0) {
			return;
		}
		if (chr->mHang.mState != 0) {
			return;
		}
		if (!IsGameRunning()) {
			return;
		}
		if (lbl_8025C400.mStatus.mFlags & 0x100) {
			return;
		}
		if (lbl_8025C400.mEvt.mData.mScript != NULL) {
			return;
		}
		if (chr->mAnimId == 0x895 || chr->mAnimId == 0x8AE || chr->mAnimId == 0x8AF) {
			*weight = 0.0f;
			return;
		}
		if (pad->mTrigger & 0x100) {
			if (act->mTarget == NULL) {
				return;
			}
			if (act->mTarget->GetCategory() == 8) {
				return;
			}
			if (chr->_9D4[4] >= 0 && (lbl_8025C400.GetObject(chr->_9D4[4])->mStatus.mFlags & 0x20)) {
				*weight = 0.0f;
				return;
			}
			if (act->mTarget->GetCategory() == 0x18) {
				return;
			}
			act->mState = 6;
			*weight     = 0.0f;
		} else if (pad->mTrigger & 0x200) {
			if (act->mTarget != NULL) {
				if (act->mTarget->GetCategory() == 8) {
					if (chr->GetAttachment() == 0) {
						act->mState = 0x13;
					}
				} else {
					act->mState = 6;
				}
			}
			*weight = 0.0f;
		}
	} else if (act->mState == 0xC || act->mState == 0xD) {
		if (IsGameRunning() && lbl_8025C400.mEvt.mData.mScript == NULL && ((pad->mTrigger & 0x100) || (pad->mTrigger & 0x200))) {
			act->mState = 0xE;
			act->_2C    = 0.0f;
		}
		*weight = 0.0f;
	} else if (act->mState == 0xF) {
		if (IsGameRunning()) {
			if (pad->mStickX > 0.8f) {
				act->mState = 0x10;
				act->_2C    = 0.0f;
			} else if (pad->mTrigger & 0x200) {
				act->mState = 0x11;
				act->_2C    = 0.0f;
			}
		}
		*weight = 0.0f;
	} else if (act->mState == 0x1B) {
		// stays as it is while the lift is thrown
	} else if (act->mState == 0x1C) {
	} else if (act->mState == 0x1F || act->mState == 0x20) {
		if (!(pad->mButton & 0x100)) {
			act->mState = 0x21;
		}
		*weight = 0.0f;
	} else if (act->mState != 0) {
		*weight = 0.0f;
	}
}

// the item particles' color callback, they take his item tint
/**
 * @note Address: 0x800D47F4
 * @note Size: 0x38
 */
s32 fn_800D47F4(ItemPtclColor* particle)
{
	u32 color        = lbl_8025C400.GetPlayer()->mItemTintWord;
	particle->mRed   = color >> 24;
	particle->mGreen = color >> 16;
	particle->mBlue  = color >> 8;
	return 0;
}

/**
 * @note Address: 0x800D482C
 * @note Size: 0x16C
 */
void fn_800D482C(XAction* act)
{
	s32 path[]     = { 0, 1 };
	HSD_JObj* jobj = FindJointByPath(act->mTarget->mJObj, path, 2);
	for (HSD_DObj* dobj = HSD_JObjGetDObj(jobj); dobj != NULL; dobj = HSD_DObjGetNext(dobj)) {
		HSD_MObj* mobj = HSD_DObjGetMObj(dobj);
		if (mobj != NULL) {
			mobj->mat->diffuse = act->mOwner->mItemTint;
			for (HSD_TObj* tobj = HSD_MObjGetTObj(mobj); tobj != NULL; tobj = HSD_TObjGetNext(tobj)) {
				u32 flags   = tobj->flags & ~0xF0000;
				tobj->flags = flags;
				tobj->flags = flags | 0x60000;
			}
			HSD_MObjCompileTev(mobj);
		}
	}
}

/**
 * @note Address: 0x800D4998
 * @note Size: 0x1C8
 */
// NONMATCHING: regalloc
void fn_800D4998(XActLift* act)
{
	PutAway(act);
}

/**
 * @note Address: 0x800D4B60
 * @note Size: 0x118
 */
BOOL fn_800D4B60(XActLift* act)
{
	return IsCarryingStill(act, 0x16);
}

/**
 * @note Address: 0x800D4C78
 * @note Size: 0xDC
 */
BOOL fn_800D4C78(XActLift* act)
{
	return IsCarrying(act, 0x38);
}

/**
 * @note Address: 0x800D4D54
 * @note Size: 0xDC
 */
BOOL fn_800D4D54(XActLift* act)
{
	return IsCarrying(act, 0x16);
}

/**
 * @note Address: 0x800D4E30
 * @note Size: 0xDC
 */
BOOL fn_800D4E30(XActLift* act)
{
	return IsCarrying(act, 0x15);
}

/**
 * @note Address: 0x800D4F0C
 * @note Size: 0xDC
 */
BOOL fn_800D4F0C(XActLift* act)
{
	return IsCarrying(act, 0x10);
}

/**
 * @note Address: 0x800D4FE8
 * @note Size: 0x4C
 */
BOOL fn_800D4FE8(XActLift* act)
{
	return IsTargetPool(act, 0x18);
}

/**
 * @note Address: 0x800D5034
 * @note Size: 0xA0
 */
void fn_800D5034(XActLift* act)
{
	if (act->mTarget != NULL) {
		act->mTarget->mStatus.mFlags &= ~0x10000ULL;
		act->mTarget->mStatus.mFlags &= ~0x0400000000000000ULL;
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
void fn_800D50D4(XActLift* act, XObjChr* owner)
{
	act->Init(owner);
	act->_2C = 0.0f;
	act->_48 = -1;
	fn_800D5034(act);
}

} // extern "C"
