#include "game/title.h"
#include "game/cgame.h"
#include "game/menusave.h"
#include "game/csound.h"

extern "C" void fn_80068B98(void* info);

/**
 * @note Address: 0x8002D2D4
 * @note Size: 0x218
 */
extern "C" void fn_8002D2D4(Title* title)
{
	s32 cardState = lbl_8025C400.mMemcard.mState;
	switch (cardState) {
	case 0:
		title->mLabels[0].mAlpha  = 0.0f;
		title->mLabels[1].mAlpha  = 0.0f;
		title->mLabels[2].mAlpha  = 0.0f;
		title->mLabels[3].mAlpha  = 0.0f;
		title->mLabels[4].mAlpha  = 0.0f;
		title->mLabels[5].mAlpha  = 0.0f;
		title->mLabels[6].mAlpha  = 0.0f;
		title->mLabels[7].mAlpha  = 0.0f;
		title->mLabels[8].mAlpha  = 0.0f;
		title->mLabels[9].mAlpha  = 0.0f;
		title->mLabels[10].mAlpha = 0.0f;
		title->mLabels[11].mAlpha = 0.0f;
		title->mLabels[12].mAlpha = 0.0f;
		title->mLabels[13].mAlpha = 0.0f;
		title->mLabels[14].mAlpha = 0.0f;
		title->mLabels[15].mAlpha = 0.0f;
		title->mLabels[16].mAlpha = 0.0f;
		title->mLabels[17].mAlpha = 0.0f;
		title->mLabels[18].mAlpha = 0.0f;
		title->mLabels[19].mAlpha = 0.0f;
		fn_8001E660((CSound*)&lbl_8025C400.mSound, (s32)0xFFFF0003);
		title->_37BC       = -200.0f;
		title->mAlpha      = 0.0f;
		lbl_8025C400._9108 = title->mFile;
		fn_80068B98(lbl_8025C400.mInfoScrn);
		title->SetMode(25);
		title->mState   = 13;
		title->mTimeout = 20000;
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
			if (cardState == 2 && (result == -6 || result == -13)) {
				title->SetMode(11);
				title->mChoice      = 1;
				title->mShowMessage = 1;
			} else if (result == -3 || result == -5) {
				title->SetMode(22);
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
