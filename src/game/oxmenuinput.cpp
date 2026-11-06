#include "game/xobjchrstate.h"
#include "game/xpad.h"
#include "game/cgame.h"
#include "game/globals.h"
#include "game/video.h"

// fades in and out over 0.3 seconds
#if defined(VERSION_PAL)
#define OX_FADE_STEP (1.0f / (0.3f * gVideoFramesPerSecond))
#else
#define OX_FADE_STEP (1.0f / 18.0f)
#endif

extern "C" {
void ShowOXCircle(XObjOX* object);
void ShowOXCross(XObjOX* object);
}

#pragma section RX ".text.oxmenuinput"

// OXMenu's update
/**
 * @note Address: 0x80096B90
 * @note Size: 0x2A8
 */
extern "C" __declspec(section ".text.oxmenuinput") void UpdateOXMenuTransition(OXMenu* menu, u32 frames)
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
				gGame.mStatus.mFlags &= ~0x800ULL;
				menu->mChr->SetRotateY(menu->mStartRot, DEG2RAD * 12.0f);
				XObjChr* chr = gGame.GetPlayer();
				chr->mPad.Update(0);
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
			if (i == menu->mSelectedIndex) {
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
			mark->mObj.UpdateObject(frames);
#endif
		}
	}
}

// OXMenu's input, left and right pick, a confirms
/**
 * @note Address: 0x80096E38
 * @note Size: 0x2AC
 */
extern "C" __declspec(section ".text.oxmenuinput") void HandleOXMenuInput(OXMenu* menu, CPad* pad)
{
	if (menu->mStarted > 0) {
		if (!(gGame.mStatus.mFlags & 0x100ULL)) {
			menu->mChr->SetRotateY(PI + gGame.mMotionControlCamera.mCamera.mAngle.y, DEG2RAD * 8.0f);
		}
		u32 next     = (gGame.mStatus.mFlags & 0x80000000ULL) ? 0x80000 : 0x80002;
		u32 previous = (gGame.mStatus.mFlags & 0x80000000ULL) ? 0x40000 : 0x40001;
		if (pad->mPad->mRepeat & next) {
			if (menu->mSelectedIndex + 1 < 2) {
				menu->mSelectedIndex = menu->mSelectedIndex + 1;
				PlayGameSound(&gGame.mSound, 0x87, 255);
			}
		} else if (pad->mPad->mRepeat & previous) {
			if (menu->mSelectedIndex - 1 >= 0) {
				menu->mSelectedIndex = menu->mSelectedIndex - 1;
				PlayGameSound(&gGame.mSound, 0x87, 255);
			}
		}
		f32 angle             = menu->mSelectedIndex == 0 ? -(DEG2RAD * 20.0f) : DEG2RAD * 20.0f;
		XObjChr* chr          = (XObjChr*)menu->mChr;
		chr->mNeckRot.mMode   = 1;
		chr->mNeckRot.mTarget = angle;
		if (!(gGame.mStatus.mFlags & 0x20ULL) && 1.0f == menu->mFade && (pad->mPad->mTrigger & 0x100)) {
			menu->mStarted = -1;
			PlayGameSound(&gGame.mSound, 0x88, 255);
			menu->mConfirmedIndex = menu->mSelectedIndex;
			menu->mChr->SetAnim(0x78, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			if (!(menu->mCrossMask & (1U << menu->mConfirmedIndex))) {
				ShowOXCircle(&((XObjChr*)menu->mChr)->mOXObj);
			} else {
				ShowOXCross(&((XObjChr*)menu->mChr)->mOXObj);
			}
			chr                   = (XObjChr*)menu->mChr;
			chr->mNeckRot.mMode   = 2;
			chr->mNeckRot.mTarget = 0.0f;
			chr->mNeckRot.mLookAt = NULL;
		}
	}
}

#if defined(VERSION_JP)
extern "C" __declspec(section ".text.oxmenuinput") void SetRingMenuOption(XObjChrMenu* menu, s32 value)
{
	menu->mAtc._280 = value;
	menu->mDrs._280 = value;
}

#endif
