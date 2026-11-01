#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/menusave.h"
#include "Dolphin/OS/OSUtil.h"
#include "game/memcard.h"
#include "game/video.h"
#include "game/title.h"

extern "C" s32 fn_80035184(s64 first, s64 second, s64 third);

/**
 * @note Address: 0x8002E574
 * @note Size: 0x358
 */
extern "C" void fn_8002E574(Title* title, u32 frames)
{
	Memcard* card = &lbl_8025C400.mMemcard;
	s32 state     = card->mState;
	title->mDelay -= frames;
	if (title->mDelay < 0)
		title->mDelay = 0;
	switch (state) {
	case 0: {
		OSTime timestamps[3];
		for (s32 i = 0; i < 3; ++i) {
			SavePreviewRecord* save = &card->mSaves[0][i];
			if (save->mState) {
				OSCalendarTime calendar;
				OSTicksToCalendarTime(OSSecondsToTicks((OSTime)save->mStat.time), &calendar);
				s32 frames                  = save->mHeader.mFrames;
				s32 active                  = save->mHeader._34;
				lbl_80361E8C[i].mUsed       = 1;
				lbl_80361E8C[i].mLevel      = save->mHeader.mLevel;
				lbl_80361E8C[i].mBattery    = save->mHeader.mBattery;
				lbl_80361E8C[i].mMaxBattery = save->mHeader.mMaxBattery;
				lbl_80361E8C[i].mMoney      = save->mHeader.mMoney;
				lbl_80361E8C[i].mExp        = save->mHeader.mExp;
				lbl_80361E8C[i].mScrap      = save->mHeader.mScrap;
				lbl_80361E8C[i].mRank       = save->mHeader.mRank;
				lbl_80361E8C[i].mHours      = frames / 216000;
				lbl_80361E8C[i].mMinutes    = (frames / 3600) % 60;
				lbl_80361E8C[i].mStage      = save->mHeader.mStage;
				lbl_80361E8C[i].mTimeMode   = save->mHeader.mTimeMode;
				if (active > 0)
					lbl_80361E8C[i].mActive = 1;
				else
					lbl_80361E8C[i].mActive = 0;
				timestamps[i] = OSCalendarTimeToTicks(&calendar);
				if (save->mStatus != 0 || !fn_80086F24(save))
					lbl_80361E8C[i].mState = 1;
				else
					lbl_80361E8C[i].mState = 0;
			}
		}
		title->_37D0 = lbl_8025C400.mMemcard._16A04;
		title->_37D4 = lbl_8025C400.mMemcard._16A00;
		title->SetMode(2);
		if (title->_37A8 == 0)
			title->mFile = 0;
		else
			title->mFile = fn_80035184(timestamps[0], timestamps[1], timestamps[2]);
		title->mAlpha = 1.0f;
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
		s32 result     = lbl_8025C400.mMemcard.mCardResult;
		title->mResult = result;
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
				title->SetMode(11);
				title->mChoice = 1;
			} else if (result == -5) {
				title->SetMode(19);
				title->mShowMessage = 1;
			} else {
				title->SetMode(14);
				title->mOffset = -32.0f;
			}
			title->mAlpha = 1.0f;
			break;
		default:
			title->SetMode(14);
			title->mAlpha  = 1.0f;
			title->mOffset = -32.0f;
			break;
		}
		break;
	}
	}
}
