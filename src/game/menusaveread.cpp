#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "Dolphin/OS/OSUtil.h"
#include "game/memcard.h"
#include "game/video.h"
#include "game/menu.h"

// todo: cleanup
static inline void SetPreview(MenuSaveSlot* slot, s32 level, f32 battery, f32 maxBattery, s32 money, s32 exp, s32 scrap, s32 rank,
                              s32 frames, s32 stage, u32 timeMode)
{
	slot->mUsed       = 1;
	slot->mLevel      = level;
	slot->mBattery    = battery;
	slot->mMaxBattery = maxBattery;
	slot->mMoney      = money;
	slot->mExp        = exp;
	slot->mScrap      = scrap;
	slot->mRank       = rank;
	slot->mHours      = frames / 216000;
	slot->mMinutes    = (frames / 3600) % 60;
	slot->mStage      = stage;
	slot->mTimeMode   = timeMode;
}

/**
 * @note Address: 0x8003CA2C
 * @note Size: 0x33C
 */
extern "C" void fn_8003CA2C(MenuSave* menu, u32 frames)
{
	Memcard* card = &lbl_8025C400.mMemcard;
	s32 state     = card->mState;
	menu->mDelay -= frames;
	if (menu->mDelay < 0)
		menu->mDelay = 0;
	switch (state) {
	case 0:
		for (s32 i = 0; i < 3; ++i) {
			SavePreviewRecord* save = &card->mSaves[0][i];
			if (save->mState) {
				OSCalendarTime calendar;
				OSTicksToCalendarTime(OSSecondsToTicks((OSTime)save->mStat.time), &calendar);
				SetPreview(&menu->mSlots[i], save->mHeader.mLevel, save->mHeader.mBattery, save->mHeader.mMaxBattery, save->mHeader.mMoney,
				           save->mHeader.mExp, save->mHeader.mScrap, save->mHeader.mRank, save->mHeader.mFrames, save->mHeader.mStage,
				           save->mHeader.mTimeMode);
				if (save->mHeader._34 > 0)
					menu->mSlots[i].mActive = 1;
				else
					menu->mSlots[i].mActive = 0;
				OSCalendarTimeToTicks(&calendar);
				if (save->mStatus != 0 || !fn_80086F24(save))
					menu->mSlots[i].mState = 1;
				else
					menu->mSlots[i].mState = 0;
			}
		}
		menu->_250 = lbl_8025C400.mMemcard._16A04;
		menu->_254 = lbl_8025C400.mMemcard._16A00;
		if (menu->mSaved == 1) {
			menu->SetMode(4);
#if defined(VERSION_PAL)
			menu->mTime = 0.5f * lbl_80660D4C;
#else
			menu->mTime = 30.0f;
#endif
			menu->mSaved = 0;
		} else {
			menu->SetMode(2);
		}
		break;
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
			if (result == -6 || result == -13) {
				menu->SetMode(8);
				menu->mChoice = 1;
			} else if (result == -5) {
				menu->SetMode(13);
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
