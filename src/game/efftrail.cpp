#include "game/xchr.h"
#include "game/hsd_dobj.h"
#include "game/xobjmap.h"
#include "game/xpad.h"
#include "game/cgame.h"
#include "game/cjobjset.h"
#include "game/globals.h"
#include "game/pointptcl.h"
#include <stdlib.h>

// one point of a spiral trail
struct EffTrailPoint {
	f32 mX;      // _00
	f32 mY;      // _04
	f32 mZ;      // _08
	f32 mAmount; // _0C, fades to 0
	u32 _10;     // _10
	f32 mPhase;  // _14
};
STATIC_ASSERT(sizeof(EffTrailPoint) == 0x18);

// two spirals rising off chibi robo, the newest point first
struct EffTrail {
	BOOL mActive;                // _00
	EffTrailPoint mPoint[2][10]; // _04
};
STATIC_ASSERT(sizeof(EffTrail) == 0x1E4);

// fades in and out over 0.3 seconds
#if defined(VERSION_PAL)
#define OX_FADE_STEP (1.0f / (0.3f * lbl_80660D4C))
#else
#define OX_FADE_STEP (1.0f / 18.0f)
#endif

extern "C" {
void fn_800D5874(XObjOX* object);
void fn_800D5804(XObjOX* object);
}

/**
 * @note Address: 0x800940A4
 * @note Size: 0x298
 */
extern "C" void fn_800940A4(EffTrail* trail, u32 frames)
{
	if (trail->mActive != 0) {
		f32 total = 0.0f;
		for (int i = 0; i < 2; i++) {
			EffTrailPoint* point = trail->mPoint[i];
			for (int j = 9; j > 0; j--) {
				point[j].mX      = point[j - 1].mX;
				point[j].mY      = point[j - 1].mY;
				point[j].mZ      = point[j - 1].mZ;
				point[j].mAmount = point[j - 1].mAmount;
				point[j].mPhase  = point[j - 1].mPhase;
			}
			if ((point[0].mAmount -= 0.02f * frames) <= 0.0f)
				point[0].mAmount = 0.0f;
			point[0].mPhase += DEG2RAD * 9.0f * frames;
			f32 sine    = sin(point[0].mPhase);
			point[0].mX = point[0].mAmount * (15.0f * ((1.0f - point[0].mAmount) * sine));
			f32 cosine  = cos(point[0].mPhase);
			point[0].mZ = point[0].mAmount * (15.0f * ((1.0f - point[0].mAmount) * cosine));
			point[0].mY += 0.3f * frames;
			for (int j = 0; j < 10; j++)
				total += point[j].mAmount;
		}
		if (total == 0.0f)
			trail->mActive = 0;
	}
}

/**
 * @note Address: 0x80095AD8
 * @note Size: 0x1F8
 */
extern "C" void fn_80095AD8(PointPtcl* ptcl, s32 type, s32 amount)
{
	ptcl->mType   = type;
	ptcl->mState  = 1;
	ptcl->mAmount = amount;
	switch (ptcl->mType) {
	case 0:
		ptcl->mColor  = amount >= 0 ? 0xCECEFF00 : 0xFF000000;
		ptcl->mColor2 = amount >= 0 ? 0xE037EE00 : 0xFF000000;
		ptcl->mNum    = __abs(amount);
		if (ptcl->mNum > 40)
			ptcl->mNum = 40;
		for (s32 i = 0; i < ptcl->mNum; ++i)
			ptcl->mPtcls[i].mState = -1;
		break;
	case 2:
		ptcl->mColor  = amount >= 0 ? 0xEFEBC500 : 0xFF000000;
		ptcl->mColor2 = amount >= 0 ? 0xEDA83600 : 0xFF000000;
		ptcl->mNum    = __abs(amount);
		if (ptcl->mNum > 40)
			ptcl->mNum = 40;
		for (s32 i = 0; i < ptcl->mNum; ++i)
			ptcl->mPtcls[i].mState = -2;
		break;
	case 3:
		ptcl->mColor  = amount >= 0 ? 0xCECEFF00 : 0xFF000000;
		ptcl->mColor2 = amount >= 0 ? 0x5959FF00 : 0xFF000000;
		ptcl->mNum    = __abs(amount);
		if (ptcl->mNum > 40)
			ptcl->mNum = 40;
		for (s32 i = 0; i < ptcl->mNum; ++i)
			ptcl->mPtcls[i].mState = -3;
		break;
	default:
		ptcl->mColor  = 0xFFFFFF00;
		ptcl->mColor2 = 0xFFFFFF00;
		ptcl->mNum    = 40;
		for (s32 i = 0; i < ptcl->mNum; ++i)
			ptcl->mPtcls[i].mState = 5;
		break;
	}
}

// OXMenu's update
/**
 * @note Address: 0x80096B90
 * @note Size: 0x2A8
 */
extern "C" void fn_80096B90(OXMenu* menu, u32 frames)
{
	if (menu->mStarted > 0) {
		if (menu->mFade < 1.0f) {
			f32 fade    = menu->mFade + OX_FADE_STEP * frames;
			menu->mFade = fade;
			if (fade >= 1.0f)
				menu->mFade = 1.0f;
		}
	} else if (menu->mStarted < 0) {
		if (menu->mFade > 0.0f) {
			f32 fade    = menu->mFade - OX_FADE_STEP * frames;
			menu->mFade = fade;
			if (fade <= 0.0f)
				menu->mFade = 0.0f;
		}
		if (0.0f == menu->mFade) {
			s32 anim = menu->mChr->mAnimId;
			if (anim != 0x78 && anim != 0xEF && anim != 0x13B) {
				menu->mStarted = 0;
				lbl_8025C400.mStatus.mFlags &= ~0x800ULL;
				menu->mChr->SetRotateY(menu->mStartRot, DEG2RAD * 12.0f);
				XObjChr* chr = lbl_8025C400.GetPlayer();
				((XPad*)chr->mPad)->Update(0);
			}
		}
	}
	if (menu->mFade > 0.0f) {
		f32 spin = DEG2RAD * 8.0f * frames;
		f32 tilt = DEG2RAD * 3.0f * frames;
		f32 grow = 0.05f * frames;
		for (s32 i = 0; i < 2; ++i) {
			OXMenuMark* mark = &menu->mMark[i];
			mark->mSpin += spin;
			mark->mTilt += tilt;
			if (i == menu->_0C) {
				f32 scale    = mark->mScale + grow;
				mark->mScale = scale;
				if (scale > 0.8f)
					mark->mScale = 0.8f;
			} else {
				f32 scale    = mark->mScale - grow;
				mark->mScale = scale;
				if (scale < 0.5f)
					mark->mScale = 0.5f;
			}
#if !defined(VERSION_JP)
			mark->mObj.vfA4(frames);
#endif
		}
	}
}

// OXMenu's input, left and right pick, a confirms
/**
 * @note Address: 0x80096E38
 * @note Size: 0x2AC
 */
extern "C" void fn_80096E38(OXMenu* menu, CPad* pad)
{
	if (menu->mStarted > 0) {
		if (!(lbl_8025C400.mStatus.mFlags & 0x100ULL)) {
			menu->mChr->SetRotateY(PI + lbl_8025C400.mMotionControlCamera.mCamera.mAngle.y, DEG2RAD * 8.0f);
		}
		u32 next     = (lbl_8025C400.mStatus.mFlags & 0x80000000ULL) ? 0x80000 : 0x80002;
		u32 previous = (lbl_8025C400.mStatus.mFlags & 0x80000000ULL) ? 0x40000 : 0x40001;
		if (pad->mPad->mRepeat & next) {
			if (menu->_0C + 1 < 2) {
				menu->_0C = menu->_0C + 1;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 255);
			}
		} else if (pad->mPad->mRepeat & previous) {
			if (menu->_0C - 1 >= 0) {
				menu->_0C = menu->_0C - 1;
				fn_8001E904(&lbl_8025C400.mSound, 0x87, 255);
			}
		}
		f32 angle                 = menu->_0C == 0 ? -(DEG2RAD * 20.0f) : DEG2RAD * 20.0f;
		XObjChr* chr              = (XObjChr*)menu->mChr;
		chr->mJointRot[1].mMode   = 1;
		chr->mJointRot[1].mTarget = angle;
		if (!(lbl_8025C400.mStatus.mFlags & 0x20ULL) && 1.0f == menu->mFade && (pad->mPad->mTrigger & 0x100)) {
			menu->mStarted = -1;
			fn_8001E904(&lbl_8025C400.mSound, 0x88, 255);
			menu->_1340 = menu->_0C;
			menu->mChr->SetAnim(0x78, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			if (!(menu->_08 & (1U << menu->_1340))) {
				fn_800D5874(&((XObjChr*)menu->mChr)->mOXObj);
			} else {
				fn_800D5804(&((XObjChr*)menu->mChr)->mOXObj);
			}
			chr                       = (XObjChr*)menu->mChr;
			chr->mJointRot[1].mMode   = 2;
			chr->mJointRot[1].mTarget = 0.0f;
			chr->mJointRot[1].mLookAt = NULL;
		}
	}
}
