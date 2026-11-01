#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/menusave.h"
#include "game/csound.h"
#include "game/memcard.h"
#include "game/video.h"
#include "game/title.h"

/**
 * @note Address: 0x8002DCFC
 * @note Size: 0x240
 */
extern "C" void fn_8002DCFC(Title* title)
{
	CPad pad(0);
	if (title->mReady == 0) {
		title->mResult = -3;
		title->SetMode(9);
		title->mOffset = -32.0f;
		for (s32 i = 0; i < 3; ++i)
			fn_8003D7DC(&lbl_80361E8C[i]);
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
			title->mChoice++;
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x87, 255);
			return;
		}
	}
	if (pad.mPad->mTrigger & 0x100) {
		switch (title->mChoice) {
		case 0:
			fn_8008A1F4(&lbl_8025C400.mMemcard, title->mChannel, title->mFile, TRUE);
			title->SetMode(15);
#if defined(VERSION_PAL)
			title->mDelay = 1.5f * lbl_80660D4C;
#else
			title->mDelay = 90;
#endif
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
			title->mOffset = -32.0f;
			return;
		case 1:
			title->SetMode(2);
			title->mChoice = 0;
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x86, 255);
			return;
		}
	}
}
