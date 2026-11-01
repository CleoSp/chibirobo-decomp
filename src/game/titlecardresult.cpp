#include "game/title.h"
#include "game/cgame.h"
#include "game/menusave.h"
#include "game/memcard.h"

/**
 * @note Address: 0x8002CCF4
 * @note Size: 0x14C
 */
extern "C" void fn_8002CCF4(Title* title, u32 frames)
{
	title->mDelay -= frames;
	if (title->mDelay < 0) {
		title->mDelay = 0;
	}
	switch (lbl_8025C400.mMemcard.mState) {
	case 0:
		title->_37D0 = lbl_8025C400.mMemcard._16A04;
		title->_37D4 = lbl_8025C400.mMemcard._16A00;
		title->SetMode(13);
		title->mAlpha = 1.0f;
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
			if (result == -6 || result == -13) {
				title->SetMode(19);
				title->mShowMessage = 1;
			} else {
				title->SetMode(21);
				title->mShowMessage = 1;
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
