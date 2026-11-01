#include "game/xobjspiderboss.h"

extern "C" {
extern AnimSfxEvent lbl_8022B9D8[]; // the anim sound events
}

/**
 * @note Address: 0x80106000
 * @note Size: 0x38
 */
extern "C" void fn_80106000(XObjSpiderBoss* spider)
{
	fn_800928E8(spider);
	spider->mAnimSfx.mEvents = lbl_8022B9D8;
}

/**
 * @note Address: 0x80106038
 * @note Size: 0x50
 */
extern "C" BOOL fn_80106038(XObjSpiderBoss* spider, const CoordInfo& info)
{
	if (fn_80091C80(spider, &info)) {
		fn_8010660C(spider);
		fn_80106088(spider);
		return TRUE;
	}
	return FALSE;
}
