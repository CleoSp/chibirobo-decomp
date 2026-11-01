#include "game/title.h"
#include "game/cgame.h"
#include "game/menusave.h"
#include "game/csound.h"
#include "game/video.h"

// todo: cleanup
static inline void MoveFormatSelection(Title* title, CPad* pad)
{
	u32 repeat = pad->mPad->mRepeat;
	if (repeat & 0x40001) {
		if (title->mChoice > 0) {
			--title->mChoice;
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x87, 255);
		}
	} else if (repeat & 0x80002) {
		if (title->mChoice < 1) {
			++title->mChoice;
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x87, 255);
		}
	}
}

// todo: cleanup
static inline void CancelFormat(Title* title)
{
	title->SetMode(19);
	fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
	// japan doesn't have the flag
#if !defined(VERSION_JP)
	title->_3C44 = 1;
#endif
}

/**
 * @note Address: 0x8002CE40
 * @note Size: 0x348
 */
extern "C" void fn_8002CE40(Title* title)
{
	CPad pad(0);
	if (title->mReady == 0) {
		title->mResult = -3;
		title->SetMode(9);
		title->mOffset = -32.0f;
		for (s32 i = 0; i < 3; ++i) {
			fn_8003D7DC(&lbl_80361E8C[i]);
		}
		return;
	}
	switch (title->mPhase) {
	case 0:
		MoveFormatSelection(title, &pad);
		if (pad.mPad->mTrigger & 0x100) {
			if (title->mChoice == 0) {
				fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
				title->mChoice = 1;
				++title->mPhase;
			} else {
				CancelFormat(title);
			}
			return;
		}
		break;
	case 1:
		MoveFormatSelection(title, &pad);
		if (pad.mPad->mTrigger & 0x100) {
			if (title->mChoice == 0) {
				fn_80088E78(&lbl_8025C400.mMemcard, title->mChannel, TRUE);
				title->SetMode(12);
				title->mDelay = (s32)(1.5f * FRAME_RATE);
				fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
			} else {
				CancelFormat(title);
			}
			return;
		}
	}
}
