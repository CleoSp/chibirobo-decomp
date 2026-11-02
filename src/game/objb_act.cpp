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
void fn_800136B4(FbSubwin* subwin, const Vec& interest, f32 angleY, f32 angleX, f32 distance, f32 fov);
f32 fn_8008DF34(XObj* obj, const Vec* pos, s32 floor, s32 a, s32 b, s32 c);    // the angle to face a floor of obj from pos
void fn_8009F4F8(CXObj* obj, s32 depth, const s32* path, f32 x, f32 y, f32 z); // puts a joint at a translation
f32 fn_800A2CE4(const CJObj* obj);                                             // the end frame of the anim
HSD_JObj* fn_800B2764(CXObj* obj, s32 depth, s32* path);
void fn_800B2BD0(XObjPlug* plug, BOOL reset);
CVec fn_800BAD80(CJObj* obj, s32 depth, s32* path);
void fn_800BB3D0(CXObj* obj, s32 depth, s32* path, f32 angle);
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

// a += b, b by value like the other CVec operators
static inline void AddVec(CVec& a, Vec b)
{
	PSVECAdd(&a, &b, &a);
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
		if (lbl_8025C400.IsRunning() && lbl_8025C400.mEvt.mData.mScript == NULL) {
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
		if (lbl_8025C400.IsRunning() && lbl_8025C400.mEvt.mData.mScript == NULL) {
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
		if (lbl_8025C400.IsRunning() && lbl_8025C400.mEvt.mData.mScript == NULL && (pad->mTrigger & 0x200)) {
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
		if (lbl_8025C400.IsRunning() && lbl_8025C400.mEvt.mData.mScript == NULL) {
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
		if (!lbl_8025C400.IsRunning()) {
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
		if (lbl_8025C400.IsRunning() && lbl_8025C400.mEvt.mData.mScript == NULL && ((pad->mTrigger & 0x100) || (pad->mTrigger & 0x200))) {
			act->mState = 0xE;
			act->_2C    = 0.0f;
		}
		*weight = 0.0f;
	} else if (act->mState == 0xF) {
		if (lbl_8025C400.IsRunning()) {
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
