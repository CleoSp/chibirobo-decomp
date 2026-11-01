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
 * @note Address: 0x800C2D18
 * @note Size: 0x98
 */
void fn_800C2D18(XActPopperShot* shot)
{
	ResetShot(shot);
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

CVec fn_800A0978(XObjChr* chr);
}
