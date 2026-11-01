#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/memcard.h"
#include "game/menu.h"

// todo: cleanup
static inline void RestartRead(MenuSave* menu)
{
	for (s32 i = 0; i < 3; ++i)
		ClearSaveSlot(&menu->mSlots[i]);
	fn_80089890(&lbl_8025C400.mMemcard, menu->mChannel, TRUE);
	menu->SetMode(1);
	fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x88, 255);
}

/**
 * @note Address: 0x8003BE94
 * @note Size: 0x214
 */
extern "C" void fn_8003BE94(MenuSave* menu)
{
	CPad pad(0);
	if (menu->_262 == 1) {
		RestartRead(menu);
		return;
	}
	if (menu->mReady == 1) {
		RestartRead(menu);
		return;
	}
	if (pad.mPad->mTrigger & 0x100) {
		menu->SetMode(16);
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x86, 255);
		return;
	}
}
