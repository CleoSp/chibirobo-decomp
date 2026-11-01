#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/memcard.h"
#include "game/menu.h"

/**
 * @note Address: 0x8003CD68
 * @note Size: 0xB0
 */
extern "C" void fn_8003CD68(MenuSave* menu)
{
	for (s32 i = 0; i < 3; ++i)
		ClearSaveSlot(&menu->mSlots[i]);
	fn_80089890(&lbl_8025C400.mMemcard, menu->mChannel, TRUE);
	menu->SetMode(1);
#if !defined(VERSION_JP)
	menu->_538 = FALSE;
#endif
}
