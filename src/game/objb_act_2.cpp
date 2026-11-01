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

CVec fn_800A0978(XObjChr* chr);
}
