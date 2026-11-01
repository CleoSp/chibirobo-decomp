#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/menu.h"

extern "C" void fn_8003ADC8(MenuSave* menu);

/**
 * @note Address: 0x8003AC48
 * @note Size: 0xE0
 */
extern "C" void fn_8003AC48(MenuSave* menu)
{
	if (menu->_264 <= 0.0f) {
		CSound* sound = (CSound*)&lbl_8025C400.mSound;
		fn_8001E660(sound, 0x14B);
		fn_8001E904(sound, 0x185, 255);
		lbl_8025C400.mStatus.mFlags &= ~2ULL;
		menu->Close();
		if (lbl_803622B4.mSuspended != 0)
			lbl_8025C400.mStatus.mFlags |= 2ULL;
		lbl_803622B4.mSuspended   = FALSE;
		lbl_803622B4.mSuspendFade = 0;
	}
}

/**
 * @note Address: 0x8003AD28
 * @note Size: 0x80
 */
extern "C" void fn_8003AD28(MenuSave* menu)
{
	menu->mTitle[0].mTime  = 0.0f;
	menu->mTitle[1].mTime  = 0.0f;
	menu->mTitle[2].mTime  = 0.0f;
	menu->mTitle[3].mTime  = 0.0f;
	menu->mTitle[4].mTime  = 0.0f;
	menu->mTitle[5].mTime  = 0.0f;
	menu->mTitle[6].mTime  = 0.0f;
	menu->mTitle[7].mTime  = 0.0f;
	menu->mTitle[8].mTime  = 0.0f;
	menu->mTitle[9].mTime  = 0.0f;
	menu->mTitle[10].mTime = 0.0f;
	menu->mTitle[11].mTime = 0.0f;
	menu->mTitle[12].mTime = 0.0f;
	menu->mTitle[13].mTime = 0.0f;
	menu->mTitle[14].mTime = 0.0f;
	menu->mTitle[15].mTime = 0.0f;
	menu->mTitle[16].mTime = 0.0f;
	menu->mTitle[17].mTime = 0.0f;
	menu->mTitle[18].mTime = 0.0f;
	menu->mTitle[19].mTime = 0.0f;
	menu->mAlpha           = 0.0f;
	menu->_164             = -200.0f;
	menu->_16C             = 0.0f;
	menu->SetMode(17);
}

/**
 * @note Address: 0x8003ADA8
 * @note Size: 0x20
 */
extern "C" void fn_8003ADA8(MenuSave* menu)
{
	fn_8003ADC8(menu);
}
