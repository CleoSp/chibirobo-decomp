#include "game/xchr.h"
#include "game/xpad.h"
#include "game/cgame.h"
#include "game/gmath.h"
#include "game/csound.h"
#include "game/video.h"

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
				XObjChr* chr = lbl_8025C400.mPlayer;
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
			menu->mChr->SetRotateY(PI + lbl_8025C400._E5DEC[1], DEG2RAD * 8.0f);
		}
		u32 next     = (lbl_8025C400.mStatus.mFlags & 0x80000000ULL) ? 0x80000 : 0x80002;
		u32 previous = (lbl_8025C400.mStatus.mFlags & 0x80000000ULL) ? 0x40000 : 0x40001;
		if (pad->mPad->mRepeat & next) {
			if (menu->_0C + 1 < 2) {
				menu->_0C = menu->_0C + 1;
				fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x87, 255);
			}
		} else if (pad->mPad->mRepeat & previous) {
			if (menu->_0C - 1 >= 0) {
				menu->_0C = menu->_0C - 1;
				fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x87, 255);
			}
		}
		f32 angle                 = menu->_0C == 0 ? -(DEG2RAD * 20.0f) : DEG2RAD * 20.0f;
		XObjChr* chr              = (XObjChr*)menu->mChr;
		chr->mJointRot[1].mMode   = 1;
		chr->mJointRot[1].mTarget = angle;
		if (!(lbl_8025C400.mStatus.mFlags & 0x20ULL) && 1.0f == menu->mFade && (pad->mPad->mTrigger & 0x100)) {
			menu->mStarted = -1;
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
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
