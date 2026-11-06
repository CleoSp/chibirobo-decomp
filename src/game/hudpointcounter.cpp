#pragma section RX ".text.hudpointcounter"
#include "game/hudcounterhelpers.h"
#include "game/infopoint.h"
#include "game/xchr.h"
#include "game/stg.h"
#include <math.h>

extern "C" bool fn_80051C60();

// a quarter of the way there each frame, at least 1
static inline f32 ApproachStep(f32 cur, f32 target)
{
	f32 step = (target - cur) / 4;
	if (step < 0.0f) {
		step = -step;
	}
	if (step <= 1.0f) {
		step = 1.0f;
	}
	return step;
}

// the chibi-pc menu is open, the counters stay hidden under it. japan has more menus and
// no empty slots
static inline BOOL IsPCOpen()
{
#if defined(VERSION_JP)
	return (gGame.mMenu.mMenus[8])->IsOpen();
#else
	CMenu* menu = gGame.mMenu.mMenus[3];
	return menu ? menu->IsOpen() : FALSE;
#endif
}

/**
 * @note Address: 0x800650AC
 * @note Size: 0x214
 */
extern "C" __declspec(section ".text.hudpointcounter") void InitHudPointCounter(InfoPoint* point, s32 id)
{
	point->mIndex       = id;
	point->mPulseFrames = 0;
	point->mSlideState  = 0;
	point->mFadeState   = 0;
	point->mSlideDelay  = 0;
	point->mOpacity     = 0.0f;
	for (int i = 0; i < 6; i++) {
		point->mPos[i].x = -220.0f;
	}
	for (int i = 0; i < 4; i++) {
		point->mValue[i] = 0.0f;
	}
	point->mVisible             = 0;
	point->mSuppressIdleDisplay = 0;
	switch (point->mIndex) {
	case INFOPOINT_ID + 2:
		point->mResource = point->mIndex;
		point->mPos[0].y = GetHudBottomEdge() - 64.0f - 40.0f;
		point->mPos[1].y = GetHudBottomEdge() - 64.0f - 40.0f;
		break;
	case INFOPOINT_ID:
		point->mResource = point->mIndex;
		point->mPos[0].y = GetHudBottomEdge() - 64.0f;
		point->mPos[1].y = GetHudBottomEdge() - 64.0f;
		break;
	case INFOPOINT_ID + 1:
		point->mResource = point->mIndex;
		point->mPos[0].y = 40.0f + (GetHudBottomEdge() - 64.0f);
		point->mPos[1].y = 40.0f + (GetHudBottomEdge() - 64.0f);
		break;
	}
	point->mState          = 0;
	point->mIconAlphaScale = 0.0f;
	point->_80             = 0;
}

/**
 * @note Address: 0x800643A4
 * @note Size: 0xD08
 */
extern "C" __declspec(section ".text.hudpointcounter") void UpdateHudPointValue(InfoPoint* point, u32 frames)
{
	switch (point->mIndex) {
	case INFOPOINT_ID + 2:
		point->mValue[1] = gGame.GetPlayer()->mParams.mExperience;
		break;
	case INFOPOINT_ID:
		point->mValue[1] = gGame.GetPlayer()->mParams.mMoney;
		break;
	case INFOPOINT_ID + 1:
		point->mValue[1] = gGame.GetPlayer()->mParams.mScrap;
		break;
	}

	bool changed = false;
	if (point->mValue[1] - point->mValue[0]) {
		changed = true;
	}
	if (IsPCOpen()) {
		point->mValue[0]   = point->mValue[1];
		point->mValue[2]   = point->mValue[3];
		point->mPos[1].x   = -220.0f;
		point->mPos[0].x   = -220.0f;
		point->mPos[3].x   = -220.0f;
		point->mPos[2].x   = -220.0f;
		point->mPos[5].x   = -220.0f;
		point->mPos[4].x   = -220.0f;
		point->mSlideState = 0;
		changed            = false;
	} else {
		f32 step = (point->mValue[1] - point->mValue[0]) / 4;
		if (step < 0.0f) {
			step = -step;
		}
		f32 speed = step;
		if (step <= 1.0f) {
			speed = 1.0f;
		}
		Approach(&point->mValue[0], &point->mValue[1], speed);
		Approach(&point->mValue[2], &point->mValue[3], 1.0f);
	}
	Approach(&point->mPos[0].x, &point->mPos[1].x, ApproachStep(point->mPos[0].x, point->mPos[1].x));
	Approach(&point->mPos[2].x, &point->mPos[3].x, ApproachStep(point->mPos[2].x, point->mPos[3].x));
	Approach(&point->mPos[4].x, &point->mPos[5].x, ApproachStep(point->mPos[4].x, point->mPos[5].x));

	bool idle = false;
	if (0.0f == gGame.GetPlayer()->_A30) {
		idle = true;
	}
#if defined(VERSION_JP)
	// The JP HUD looks up stage19 here and discards the result.
	FindStageIndexByName("stage19");
#endif
	if (point->mSuppressIdleDisplay) {
		idle = false;
	}
	switch (point->mIndex) {
	case INFOPOINT_ID + 2:
		point->mPos[0].y = GetHudBottomEdge() - 64.0f - 40.0f;
		point->mPos[1].y = GetHudBottomEdge() - 64.0f - 40.0f;
		break;
	case INFOPOINT_ID:
		point->mPos[0].y = GetHudBottomEdge() - 64.0f;
		point->mPos[1].y = GetHudBottomEdge() - 64.0f;
		break;
	case INFOPOINT_ID + 1:
		point->mPos[0].y = 40.0f + (GetHudBottomEdge() - 64.0f);
		point->mPos[1].y = 40.0f + (GetHudBottomEdge() - 64.0f);
		break;
	}

	switch (point->mSlideState) {
	case 0:
		if (IsPCOpen()) {
			break;
		}
		if (idle) {
			point->mPos[1].x   = 60.0f;
			point->mSlideDelay = 0;
			point->mSlideState++;
		} else if (changed) {
			point->mPos[1].x   = 60.0f;
			point->mSlideDelay = 0;
			point->mSlideState = 10;
		}
		break;
	case 1:
		point->mSlideDelay++;
		if (point->mSlideDelay > 6) {
			point->mPos[3].x   = 60.0f;
			point->mSlideDelay = 0;
			point->mSlideState++;
		}
		break;
	case 2:
		point->mSlideDelay++;
		if (point->mSlideDelay > 6) {
			point->mPos[5].x = 60.0f;
			point->mSlideState++;
		}
		break;
	case 3:
		if (!idle) {
			point->mPos[1].x   = -220.0f;
			point->mSlideDelay = 0;
			point->mSlideState++;
		}
		break;
	case 4:
		if (changed) {
			point->mPos[1].x   = 60.0f;
			point->mSlideDelay = 0;
			point->mSlideState = 10;
		} else {
			point->mSlideDelay++;
			if (point->mSlideDelay > 6) {
				point->mPos[5].x   = -220.0f;
				point->mSlideDelay = 0;
				point->mSlideState++;
			}
		}
		break;
	case 5:
		if (changed) {
			point->mPos[1].x   = 60.0f;
			point->mSlideDelay = 0;
			point->mSlideState = 10;
		} else {
			point->mSlideDelay++;
			if (point->mSlideDelay > 6) {
				point->mPos[3].x   = -220.0f;
				point->mSlideState = 0;
			}
		}
		break;
	case 10:
		point->mSlideDelay++;
		if (point->mSlideDelay > 3) {
			point->mPos[3].x   = 60.0f;
			point->mSlideDelay = 0;
			point->mSlideState++;
		}
		break;
	case 11:
		point->mSlideDelay++;
		if (point->mSlideDelay > 3) {
			point->mPos[5].x   = 60.0f;
			point->mSlideDelay = 0;
			point->mSlideState++;
		}
		break;
	case 12:
		if (idle) {
			point->mSlideState = 3;
		} else {
			point->mSlideDelay++;
			if (changed) {
				point->mSlideDelay = 0;
			}
			if (point->mSlideDelay > 90) {
				point->mPos[1].x   = -220.0f;
				point->mSlideDelay = 0;
				point->mSlideState++;
			}
		}
		break;
	case 13:
		if (changed) {
			point->mPos[1].x   = 60.0f;
			point->mSlideDelay = 0;
			point->mSlideState = 10;
		} else {
			point->mSlideDelay++;
			if (point->mSlideDelay > 6) {
				point->mPos[5].x   = -220.0f;
				point->mSlideDelay = 0;
				point->mSlideState++;
			}
		}
		break;
	case 14:
		if (changed) {
			point->mPos[1].x   = 60.0f;
			point->mSlideDelay = 0;
			point->mSlideState = 10;
		} else {
			point->mSlideDelay++;
			if (point->mSlideDelay > 6) {
				point->mPos[3].x   = -220.0f;
				point->mSlideState = 0;
			}
		}
		break;
	}

	if (point->mVisible) {
		point->mPos[1].x   = 60.0f;
		point->mPos[3].x   = 60.0f;
		point->mPos[5].x   = 60.0f;
		point->mSlideDelay = 0;
		point->mSlideState = 3;
	}

	switch (point->mFadeState) {
	case 0:
		point->mOpacity = 1.0f;
		break;
	case 1:
		point->mOpacity = 1.0f;
		break;
	case 2:
		point->mOpacity = 0.0f;
		break;
	case 3:
		point->mOpacity += 0.08f * frames;
		if (point->mOpacity >= 1.0f) {
			point->mOpacity = 1.0f;
		}
		if (gGame.mEvt.mData.mScript) {
			point->mFadeState = 4;
		}
		break;
	case 4:
		point->mOpacity -= 0.08f * frames;
		if (point->mOpacity < 0.0f) {
			point->mOpacity = 0.0f;
		}
		if (!gGame.mEvt.mData.mScript) {
			point->mFadeState = 3;
		}
		break;
	}

	point->mIconAlphaScale = 1.0f;
	switch (point->mIndex) {
	case INFOPOINT_ID + 1:
		if (fn_80051C60()) {
			point->mState          = 0;
			point->mPulseFrames    = 0;
			point->mIconAlphaScale = 1.0f;
			break;
		}
		switch (point->mState) {
		case 0:
			if (point->mValue[1] >= 300.0f) {
				point->mState++;
				point->mPulseFrames = 0;
			}
			break;
		case 1:
			point->mPulseFrames += frames;
			point->mIconAlphaScale = sin(0.017453292f * ((point->mPulseFrames * 6) % 360));
			if (point->mIconAlphaScale > 0.0f) {
				PlayGameSound(&gGame.mSound, 0x183, 0xFF);
				point->mState++;
			}
			break;
		case 2:
			point->mPulseFrames += frames;
			point->mIconAlphaScale = sin(0.017453292f * ((point->mPulseFrames * 6) % 360));
			if (point->mValue[1] < 300.0f) {
				point->mState = 0;
			}
			break;
		case 3:
			if (point->mValue[1] < 300.0f) {
				point->mState = 0;
			}
			break;
		}
		break;
	}
}
