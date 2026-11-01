#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "Dolphin/OS/OSUtil.h"
#include "game/memcard.h"
#include "game/video.h"
#include "game/menu.h"

// todo: cleanup
static inline void SetPreview(MenuSave* menu, u32 timeMode, s32 rank, s32 scrap, s32 money, f32 maxBattery, f32 battery, s32 frames,
                              s32 level, s32 exp, s32 stage)
{
	menu->mSlots[menu->mFile].mUsed       = 1;
	menu->mSlots[menu->mFile].mLevel      = level;
	menu->mSlots[menu->mFile].mBattery    = battery;
	menu->mSlots[menu->mFile].mMaxBattery = maxBattery;
	menu->mSlots[menu->mFile].mMoney      = money;
	menu->mSlots[menu->mFile].mExp        = exp;
	menu->mSlots[menu->mFile].mScrap      = scrap;
	menu->mSlots[menu->mFile].mRank       = rank;
	menu->mSlots[menu->mFile].mHours      = frames / 216000;
	menu->mSlots[menu->mFile].mMinutes    = (frames / 3600) % 60;
	menu->mSlots[menu->mFile].mTimeMode   = timeMode;
	menu->mSlots[menu->mFile].mStage      = stage;
	menu->mSlots[menu->mFile].mState      = 0;
}

/**
 * @note Address: 0x8003C0A8
 * @note Size: 0x3C0
 */
extern "C" void fn_8003C0A8(MenuSave* menu, u32 frames)
{
	Memcard* card = &lbl_8025C400.mMemcard;
	s32 state     = card->mState;
	menu->mSaved  = 0;
	menu->mDelay -= frames;
	if (menu->mDelay < 0)
		menu->mDelay = 0;
	switch (state) {
	case 0: {
		SavePreviewRecord* save = &card->mSaves[0][menu->mFile];
		if (save->mState) {
			OSCalendarTime calendar;
			OSTicksToCalendarTime(OSSecondsToTicks((OSTime)save->mStat.time), &calendar);
			SetPreview(menu, save->mHeader.mTimeMode, save->mHeader.mRank, save->mHeader.mScrap, save->mHeader.mMoney,
			           save->mHeader.mMaxBattery, save->mHeader.mBattery, save->mHeader.mFrames, save->mHeader.mLevel, save->mHeader.mExp,
			           save->mHeader.mStage);
			if (save->mHeader._34 > 0)
				menu->mSlots[menu->mFile].mActive = 1;
			else
				menu->mSlots[menu->mFile].mActive = 0;
		}
		menu->_250 = lbl_8025C400.mMemcard._16A04;
		menu->_254 = lbl_8025C400.mMemcard._16A00;
		fn_80089890(&lbl_8025C400.mMemcard, menu->mChannel, TRUE);
		menu->SetMode(1);
		menu->mLastFile = menu->mFile;
		menu->mSaved    = 1;
		menu->mOffset   = 0.0f;
		break;
	}
	case 1:
	case 2:
	case 3:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15: {
		s32 result    = lbl_8025C400.mMemcard.mCardResult;
		menu->mResult = result;
		switch (result) {
		case -128:
		case -14:
		case -13:
		case -12:
		case -11:
		case -10:
		case -9:
		case -8:
		case -7:
		case -6:
		case -5:
		case -4:
		case -3:
		case -2:
		case -1:
		case 0:
			if (state == 2 && (result == -6 || result == -13)) {
				menu->SetMode(8);
				menu->mChoice      = 1;
				menu->mShowMessage = 1;
			} else if (result == -3 || result == -5) {
				menu->SetMode(14);
				menu->mMessage     = 3;
				menu->mShowMessage = 1;
			} else {
				menu->SetMode(11);
				menu->mOffset = -32.0f;
			}
			menu->mAlpha = 1.0f;
			break;
		default:
			menu->SetMode(11);
			menu->mAlpha  = 1.0f;
			menu->mOffset = -32.0f;
			break;
		}
		break;
	}
	}
}
