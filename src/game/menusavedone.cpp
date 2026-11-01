#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/menu.h"

/**
 * @note Address: 0x8003C468
 * @note Size: 0x130
 */
extern "C" void fn_8003C468(MenuSave* menu, u32 frames)
{
	CPad pad(0);
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
		if ((pad.mPad->mTrigger & 0x100) || (menu->mTime -= (f32)frames) < 0.0f) {
			menu->mTime = 0.0f;
			menu->SetMode(16);
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
		}
		break;
	}
}
