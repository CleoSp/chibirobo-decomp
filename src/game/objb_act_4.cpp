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

CVec fn_800A0978(XObjChr* chr);
}
