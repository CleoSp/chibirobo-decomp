#include "game/title.h"
#include "game/cgame.h"
#include "game/menusave.h"
#include "game/csound.h"
#include "game/video.h"

/**
 * @note Address: 0x8002D4F0
 * @note Size: 0x304
 */
extern "C" void fn_8002D4F0(Title* title)
{
	CPad pad(0);
	if ((title->mReady == 0 || title->_3809 == 0) && title->_37A8 == 1) {
		title->SetMode(25);
		title->mState = 11;
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
		title->_37BC              = -200.0f;
		title->mOffset            = 0.0f;
		title->mAlpha             = 0.0f;
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
		title->_37A8              = 0;
		title->_3809              = 0;
		title->_3808              = 0;
		return;
	}

	if (title->mReady == 0) {
		title->mResult = -3;
		title->SetMode(9);
		title->mOffset = -32.0f;
		for (s32 i = 0; i < 3; ++i) {
			fn_8003D7DC(&lbl_80361E8C[i]);
		}
		return;
	}
	u32 repeat = pad.mPad->mRepeat;
	if (repeat & 0x40001) {
		if (title->mChoice > 0) {
			--title->mChoice;
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x87, 255);
			return;
		}
	} else if (repeat & 0x80002) {
		if (title->mChoice < 1) {
			++title->mChoice;
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x87, 255);
			return;
		}
	}
	if (pad.mPad->mTrigger & 0x100) {
		switch (title->mChoice) {
		case 0:
			fn_8008A1F4(&lbl_8025C400.mMemcard, title->mChannel, title->mFile, TRUE);
			title->SetMode(15);
			title->mDelay = (s32)(1.5f * FRAME_RATE);
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
			title->mOffset = -32.0f;
			break;
		case 1:
			title->SetMode(2);
			title->mChoice = 0;
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x86, 255);
			break;
		}
	}
}
