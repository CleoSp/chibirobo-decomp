#include "game/xobjspidersmall.h"
#include "game/cgame.h"

/**
 * @note Address: 0x800FEBB4
 * @note Size: 0x70
 */
extern "C" void fn_800FEBB4(XObjSpiderSmall* spider)
{
	CSound* sound = (CSound*)&lbl_8025C400.mSound;
	if (fn_8001E6C8(sound, (s32)0xFFFF000C)) {
		fn_8001E660(sound, (s32)0xFFFF000C);
	}
	fn_800911C0(spider);
}

/**
 * @note Address: 0x800FEC24
 * @note Size: 0x1AC
 */
// the first spider also gathers the stage's fx_spider_01 objects
extern "C" void fn_800FEC24(XObjSpiderSmall* spider)
{
	fn_800928E8(spider);
	spider->mAnimSfx.mEvents = lbl_8022B170;
	spider->_AAC             = lbl_8025C400.mSound._2C4 <= 0 ? lbl_8025C400.mSound._2B4 : 0;
	if (spider->_258 == 0) {
		s32 found  = 0;
		XObj** out = lbl_8037E910;
		for (u32 i = 0; i < 12; ++i) {
			lbl_8037E910[i] = NULL;
			lbl_8037E940[i] = -1;
		}
		lbl_8065B2E0 = 0;
		lbl_8065B2DC = 0;
		// the anim callback can stop this and change the count
		for (s32 i = 0; lbl_8065B2DC == 0 && i < lbl_8025C400.mObjectCount; ++i) {
			XObj* obj = lbl_8025C400.mObjects[i];
			if (obj->mStatus.mFlags & 1ULL) {
				if (obj->mCoord._00->_00 == 0x1BC) {
					*out = obj;
					(*out)->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					++found;
					++out;
					if (found >= 12) {
						break;
					}
				}
			}
		}
	}
}
