#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/memcard.h"
#include "game/menu.h"

/**
 * @note Address: 0x8003B7AC
 * @note Size: 0x20C
 */
extern "C" void fn_8003B7AC(MenuSave* menu, u32 frames)
{
	CPad pad(0);
	if (menu->mReady == 0) {
		menu->ResetResults();
		return;
	}
	switch (menu->mPhase) {
	case 0:
		menu->mDelay -= frames;
		if (menu->mDelay < 0) {
			menu->mDelay = 0;
			menu->mPhase++;
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
		}
		break;
	case 1:
		if (pad.mPad->mTrigger & 0x100) {
			for (s32 i = 0; i < 3; ++i)
				ClearSaveSlot(&menu->mSlots[i]);
			fn_80089890(&lbl_8025C400.mMemcard, menu->mChannel, TRUE);
			menu->SetMode(1);
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
			return;
		}
		break;
	}
}
