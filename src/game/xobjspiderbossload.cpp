#include "game/xobjspiderboss.h"

extern "C" {
extern AnimSfxEvent lbl_8022B9D8[]; // the anim sound events
}

/**
 * @note Address: 0x80106000
 * @note Size: 0x38
 */
extern "C" void vf98__14XObjSpiderBossFv(XObjSpiderBoss* spider)
{
	vf98__4XObjFv(spider);
	spider->mAnimSfx.mEvents = lbl_8022B9D8;
}

/**
 * @note Address: 0x80106038
 * @note Size: 0x50
 */
extern "C" BOOL vf9C__14XObjSpiderBossFRC9CoordInfo(XObjSpiderBoss* spider, const CoordInfo& info)
{
	if (vf9C__4XObjFRC9CoordInfo(spider, &info)) {
		fn_8010660C(spider);
		fn_80106088(spider);
		return TRUE;
	}
	return FALSE;
}
