#include "game/objb_act_shared.h"

extern "C" {

static inline void PutDown(XActLift* act)
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

// the lift drop, defined early so the jump can inline it
/**
 * @note Address: 0x800D4998
 * @note Size: 0x1C8
 */
// NONMATCHING: the dol keeps the action in r30 and the target in r31
void fn_800D4998(XActLift* act)
{
	PutDown(act);
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
}
