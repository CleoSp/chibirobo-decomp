#include "game/title.h"
#include "game/cgame.h"
#include "game/menusave.h"
#include "game/csound.h"

/**
 * @note Address: 0x8002CB40
 * @note Size: 0x1B4
 */
extern "C" void fn_8002CB40(Title* title, u32 frames)
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
		title->mDelay -= frames;
		if (title->mDelay < 0) {
			title->mDelay = 0;
			++title->mPhase;
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
		}
		break;
	case 1:
		if (pad.mPad->mTrigger & 0x100) {
			for (s32 i = 0; i < 3; ++i) {
				fn_8003D7DC(&lbl_80361E8C[i]);
			}
			fn_80089890(&lbl_8025C400.mMemcard, title->mChannel, TRUE);
			title->SetMode(1);
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
			return;
		}
		break;
	}
}
