#include "game/title.h"
#include "game/cgame.h"
#include "game/menusave.h"
#include "game/csound.h"

/**
 * @note Address: 0x8002D18C
 * @note Size: 0x148
 */
extern "C" void fn_8002D18C(Title* title)
{
	CPad pad(0);
	if (title->_37E2 == 1 || title->mReady == 1) {
		for (s32 i = 0; i < 3; ++i) {
			fn_8003D7DC(&lbl_80361E8C[i]);
		}
		fn_80089890(&lbl_8025C400.mMemcard, title->mChannel, TRUE);
		title->SetMode(1);
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
		title->mOffset = 0.0f;
		return;
	}
	if (pad.mPad->mTrigger & 0x100) {
		title->SetMode(5);
		title->mOffset = -32.0f;
		title->mChoice = 1;
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
	}
}
