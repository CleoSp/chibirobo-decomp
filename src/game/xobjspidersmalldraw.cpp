#include "game/xobjspidersmall.h"

/**
 * @note Address: 0x800FDEF0
 * @note Size: 0x28C
 */
// draws the spider, then its fx_spider_01 where the fx is
extern "C" void vfAC__15XObjSpiderSmallFv(XObjSpiderSmall* spider)
{
	spider->vfB0(NULL);
	u8 index = spider->mFxIndex;
	if (index != 255 && (spider->mFx.mStatus.mFlags & 1)) {
		CXObj* fx = lbl_8037E910[index];
		if (fx != NULL) {
			HSD_JObj* jobj = fx->mJObj;
			HSD_JObjSetTranslateX(jobj, spider->mFx.mPos.x);
			HSD_JObjSetTranslateY(jobj, spider->mFx.mPos.y);
			HSD_JObjSetTranslateZ(jobj, spider->mFx.mPos.z);
			HSD_JObjSetRotationY(jobj, spider->mFx.mRotY);
			HSD_JObjDispAll(jobj, NULL, 7, 0);
		}
	}
}
