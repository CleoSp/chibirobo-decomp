#include "game/particle_srt.h"
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
 * @note Address: 0x800BD320
 * @note Size: 0x4
 */
void fn_800BD320(void)
{
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

CVec fn_800A0978(XObjChr* chr);
}
