#include "game/title.h"
#include "game/cgame.h"
#include "game/menusave.h"
#include "game/memcard.h"

/**
 * @note Address: 0x8002C3F0
 * @note Size: 0x348
 */
extern "C" void fn_8002C3F0(Title* title, u32 frames)
{
	Memcard* card = &lbl_8025C400.mMemcard;
	s32 state     = card->mState;
	title->mDelay -= frames;
	if (title->mDelay < 0) {
		title->mDelay = 0;
	}
	switch (state) {
	case 0: {
		title->_37D0            = lbl_8025C400.mMemcard._16A04;
		title->_37D4            = lbl_8025C400.mMemcard._16A00;
		SavePreviewRecord* save = &card->mSaves[0][title->mFile];
		if (save->mState) {
			s32 active                             = save->mHeader._34;
			s32 level                              = save->mHeader.mLevel;
			lbl_80361E8C[title->mFile].mUsed       = 1;
			f32 battery                            = save->mHeader.mBattery;
			s32 totalFrames                        = save->mHeader.mFrames;
			f32 maxBattery                         = save->mHeader.mMaxBattery;
			s32 money                              = save->mHeader.mMoney;
			s32 exp                                = save->mHeader.mExp;
			s32 scrap                              = save->mHeader.mScrap;
			s32 rank                               = save->mHeader.mRank;
			s32 hours                              = totalFrames / 216000;
			s32 minutes                            = (totalFrames / 3600) % 60;
			s32 stage                              = save->mHeader.mStage;
			u32 timeMode                           = save->mHeader.mTimeMode;
			lbl_80361E8C[title->mFile].mLevel      = level;
			lbl_80361E8C[title->mFile].mBattery    = battery;
			lbl_80361E8C[title->mFile].mMaxBattery = maxBattery;
			lbl_80361E8C[title->mFile].mMoney      = money;
			lbl_80361E8C[title->mFile].mExp        = exp;
			lbl_80361E8C[title->mFile].mScrap      = scrap;
			lbl_80361E8C[title->mFile].mRank       = rank;
			lbl_80361E8C[title->mFile].mHours      = hours;
			lbl_80361E8C[title->mFile].mMinutes    = minutes;
			lbl_80361E8C[title->mFile].mStage      = stage;
			lbl_80361E8C[title->mFile].mTimeMode   = timeMode;
			lbl_80361E8C[title->mFile].mState      = 0;
			if (active > 0) {
				lbl_80361E8C[title->mFile].mActive = 1;
			} else {
				lbl_80361E8C[title->mFile].mActive = 0;
			}
		}
		title->SetMode(24);
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
			if (state == 2 && (result == -6 || result == -13)) {
				title->SetMode(11);
				title->mChoice      = 1;
				title->mShowMessage = 1;
			} else if (result == -3 || result == -5) {
				title->SetMode(20);
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
