#include "game/xchr.h"
#include "game/xobjflags.h"
#include "game/cgame.h"
#include "game/cjobjset.h"
#include "game/globals.h"
#include "game/hsd_dobj.h"

// chibi robo's radar, popper, copter and tumble actions

extern "C" {
void fn_80015300(CGame* game, const Vec& pos, f32* x, f32* y, f32* z, CCamera* camera); // where pos is on the screen
void fn_8009F4F8(CXObj* obj, s32 depth, const s32* path, f32 x, f32 y, f32 z);          // puts a joint at a translation
BOOL fn_8009FEDC(XObjChr* chr);
void fn_800C21A0(XActPopperShot* shot);
void fn_800C2DB0(XActCopter* act, u32 frames, f32 speed);
CVec fn_800C35F0(XObjChr* chr);
}

// how far the stick is pushed
static inline f32 StickLength(const CPadOne* pad)
{
	f32 x = pad->mStickX;
	f32 y = pad->mStickY;
	return sqrtf(x * x + y * y);
}

// the tumble ring sits just above the floor
static inline Vec RingPos(const Vec& pos, const Vec& offset)
{
	Vec v = { pos.x + offset.x, 0.01f + pos.y, pos.z + offset.z };
	return v;
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
		if (lbl_8025C400.IsRunning() && lbl_8025C400.mEvt.mData.mScript == NULL) {
			if ((pad->mTrigger & 0x100) || (pad->mTrigger & 0x200) || StickLength(pad) > 0.0f) {
				act->mState = 3;
			}
		}
		*weight = 0.0f;
	} else if (act->mState == 0x10) {
		if (lbl_8025C400.IsRunning() && lbl_8025C400.mEvt.mData.mScript == NULL) {
			if ((pad->mTrigger & 0x100) || (pad->mTrigger & 0x200) || StickLength(pad) > 0.0f) {
				act->mState = 0x11;
			}
		}
		*weight = 0.0f;
	} else if (act->mState == 5) {
		if (lbl_8025C400.IsRunning() && lbl_8025C400.mEvt.mData.mScript == NULL) {
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

} // extern "C"
