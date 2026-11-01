#include "game/xact.h"
#include "game/cgame.h"
#include "game/cparticle.h"
#include "game/csound.h"

/**
 * @note Address: 0x800C159C
 * @note Size: 0x74
 */
extern "C" CVec fn_800C159C(const CVec& input, f32 amount)
{
	CVec value = input;
	value.x += amount;
	value.y += amount;
	value.z += amount;
	return value;
}

/**
 * @note Address: 0x800C1610
 * @note Size: 0x64
 */
extern "C" void fn_800C1610(XActPopper* act, void*, f32* amount)
{
	if (act->mState == 4)
		*amount = 1.0f;
	else if (act->mState == 6)
		*amount = 1.0f;
	else if (act->mState == 7 || act->mState == 8 || act->mState == 5)
		*amount = 1.0f;
	else if (act->mState != 0)
		*amount = 0.0f;
}

/**
 * @note Address: 0x800C1674
 * @note Size: 0x148
 */
extern "C" void fn_800C1674(XActPopper* act, XObjChr* owner)
{
	act->Init(owner);
	XActPopperShot* shot = act->mShot;
	for (s32 i = 0; i < 10; ++i, ++shot) {
		shot->mActive = FALSE;
		shot->mState  = 0;
		fn_80024E0C(&shot->mPtcl);
		shot->mRing[0].mAmount = 0.0f;
		shot->mRing[1].mAmount = 0.0f;
		if (shot->mGen != NULL) {
			((CParticle*)lbl_8025C400.mParticle)->Kill(shot->mGen);
			shot->mGen = NULL;
		}
		if (shot->mChargeGen != NULL) {
			((CParticle*)lbl_8025C400.mParticle)->Kill(shot->mChargeGen);
			shot->mChargeGen = NULL;
		}
		fn_80024FAC(&shot->mPtcl, shot->mPtcls, sizeof(CPtclOne), 40);
		shot->mRing[0].mAmount = 0.0f;
		shot->mRing[1].mAmount = 0.0f;
		shot->mGen             = NULL;
		shot->mChargeGen       = NULL;
		if (shot->mJoint == NULL)
			shot->mJoint = HSD_JObjAlloc();
	}
	act->mCharge.mState     = 0;
	act->mCharge.mChargeGen = NULL;
	act->mCharge.mGen       = NULL;
	if (act->mCharge.mJoint == NULL)
		act->mCharge.mJoint = HSD_JObjAlloc();
}

/**
 * @note Address: 0x800C17BC
 * @note Size: 0xB0
 */
extern "C" void fn_800C17BC(XActPopperCharge* charge)
{
	charge->mState = 0;
	charge->mTimer = 0.0f;
	if (charge->mGen != NULL) {
		((CParticle*)lbl_8025C400.mParticle)->Kill(charge->mGen);
		charge->mGen = NULL;
	}
	if (charge->mChargeGen != NULL) {
		((CParticle*)lbl_8025C400.mParticle)->Kill(charge->mChargeGen);
		charge->mChargeGen = NULL;
	}
	CSound* sound = (CSound*)&lbl_8025C400.mSound;
	fn_8001E660(sound, 0x186);
	fn_8001E660(sound, 0x18B);
}
