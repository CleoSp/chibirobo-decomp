#include "game/xobjflags.h"
#include "game/xobjhbox.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include <math.h>
#include <stdlib.h>
#include "game/gmath.h"
#include "game/csound.h"
#include "game/globals.h"
#include "game/stagedata.h"
#include "game/evt.h"
#include "game/xchr.h"

extern "C" {
#include <sysdolphin/axdriver.h>
}

/**
 * @note Address: 0x800DF9A0
 * @note Size: 0x184
 */
extern "C" void fn_800DF9A0(XObjDust* dust)
{
	fn_800928E8(dust);
	if (dust->GetObjFlags() & 1) {
		dust->mBaseY        = dust->mMoveTranslate.y;
		dust->mPhase        = fmod((f32)rand(), TWO_PI);
		dust->mPhaseStep    = (rand() % 200 + 200) / 10000.0f;
		dust->mBouncePhase  = 0.0f;
		dust->mBounceHeight = 0.0f;
		dust->mHeading      = NAN;
		dust->mSpeed        = 0.0f;
		dust->mFade         = 1.0f;
	}
	dust->mEventId  = -1;
	dust->mEventArg = -1;
	dust->mMatAnim  = NULL;
	if (dust->mJObj != NULL && (dust->mStatus.mFlags & 0x1000ULL)) {
		UnkStageObj* placement = dust->mCoord._00;
		dust->SetAnim(placement->_30, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
	}
}

/**
 * @note Address: 0x800DFB24
 * @note Size: 0x44
 */
extern "C" s32 fn_800DFB24(XObjDust* dust)
{
	s32 id = dust->_260;
	if (id > 0)
		return (s32)((u32)fn_8006F390(2) + (u32)id);
	return 0;
}

/**
 * @note Address: 0x800DFB68
 * @note Size: 0x100
 */
extern "C" u32 fn_800DFB68(XObjHBox* box, s32 unused, u32 flags)
{
	if ((flags & 3) == 1)
		return flags;
	if (box->mAnimId == 0)
		return flags;
	lbl_8025C400.mPlayer->_9A90 = 0.0f;
	box->mStatus.mFlags |= 0x4000000000000000ULL;
	box->mPart.mHit = 1.0f;
	fn_8001E904((CSound*)&lbl_8025C400.mSound, 285, 255);
	const void* script = box->vf128();
	if (!fn_8009FDE4(lbl_8025C400.mPlayer)) {
		fn_8007EEA4((CEvt*)(&lbl_8025C400._5D07C), (u8*)script, (u32)box, -1, -1);
	}
	HSD_AudioSFXKeyOff(box->mPart.mSfx);
	return 0;
}

/**
 * @note Address: 0x800DFC68
 * @note Size: 0xBC
 */
extern "C" void fn_800DFC68(XObjHBox* box, f32 strength)
{
	if (box->mAnimId <= 0) {
		fn_8008A6C0(box, strength);
		if ((box->mStatus.mFlags & 0x04000000ULL) && strength >= (s32)fn_8006EBC8(31) / 100.0f)
			box->mStatus.mFlags |= 0x2000000000000000ULL;
	}
}

/**
 * @note Address: 0x800DFD24
 * @note Size: 0x1F0
 */
extern "C" void fn_800DFD24(XObjHBox* box)
{
	if (box->mRing > 0.0f) {
		Vec pos = { 0.0f, 0.0f, 0.0f };
		f32 x, y;
		box->vf10C(&pos, &x, &y, NULL, 0);
		f32 ring  = box->mRing;
		f32 inner = 400.0f * ring;
		f32 outer = 400.0f * (1.0f - ring);
		fn_8019B464(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 0, 7, 0);
		fn_8019B56C(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 2, 4, 15);
		// retail adds an int zero to both radii
		s32 pad   = 0;
		u32 color = (u32)(255.0f * ring) | 0xFFFFFF00;
		fn_80198258((CGs*)lbl_8025C400.mGs, 16, color, 0xFFFFFF00, x, y, 0.0f, inner + pad, inner + pad, 0.0f, 0.0f, 0.0f);
		fn_80198258((CGs*)lbl_8025C400.mGs, 16, color, 0xFFFFFF00, x, y, 0.0f, outer + pad, outer + pad, 0.0f, 0.0f, 0.0f);
		fn_8019B56C(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 4, 5, 15);
		fn_8019B464(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 3, 0);
	}
}

/**
 * @note Address: 0x800DFF14
 * @note Size: 0x48
 */
extern "C" void fn_800DFF14(XObjHBox* box, const Vec* origin, f32 opacity)
{
	if (box->mPart.mObj.mStatus.mFlags & 0x10000ULL)
		fn_8009056C(box, origin, opacity);
}

/**
 * @note Address: 0x800DFF5C
 * @note Size: 0x78
 */
extern "C" void fn_800DFF5C(XObjHBox* box, Mtx view)
{
	fn_800908C0(box, view);
	if (box->mPart.mObj.mStatus.mFlags & 0x10000ULL)
		box->mPart.mObj.vfB0(view);
}
