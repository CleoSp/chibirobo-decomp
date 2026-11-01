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

CVec fn_800A0978(XObjChr* chr);
}
