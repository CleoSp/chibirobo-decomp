#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/menu.h"

/**
 * @note Address: 0x8003B9B8
 * @note Size: 0x160
 */
extern "C" void fn_8003B9B8(MenuSave* menu, u32 frames)
{
	menu->mDelay -= frames;
	if (menu->mDelay < 0)
		menu->mDelay = 0;
	switch (lbl_8025C400.mMemcard.mState) {
	case 0:
		menu->_250 = lbl_8025C400.mMemcard._16A04;
		menu->_254 = lbl_8025C400.mMemcard._16A00;
		menu->SetMode(10);
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
				menu->SetMode(13);
				menu->mShowMessage = 1;
			} else {
				menu->SetMode(15);
				menu->mMessage     = 8;
				menu->mShowMessage = 1;
			}
			break;
		default:
			menu->SetMode(11);
			menu->mOffset = -32.0f;
			break;
		}
		break;
	}
	default:
		menu->SetMode(11);
		menu->mOffset = -32.0f;
		break;
	}
}
