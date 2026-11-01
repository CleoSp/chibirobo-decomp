#include "game/xobjspidersmall.h"
#include "game/cgame.h"

/**
 * @note Address: 0x800FEDD0
 * @note Size: 0x398
 */
// vf9C, also takes back what the spider was doing the last time the stage was left
extern "C" BOOL fn_800FEDD0(XObjSpiderSmall* spider, const CoordInfo& info)
{
	XObjSpiderSmallWork* work = &lbl_8037E7D0;
	if (fn_80091C80(spider, &info)) {
		if (spider->_258 == 0) {
			for (u32 i = 0; i < 16; ++i) {
				XObjSpiderSmall* other = (XObjSpiderSmall*)(lbl_8025C400.mPools[15].mObjects + i * lbl_8025C400.mPools[15].mStride);
				other->mSpiderStatus.mFlags |= 0x80000;
			}
			if (spider->mTimeMode != lbl_8025C400.mTimeState.mMode) {
				for (u32 i = 0; i < 39; ++i) {
					for (u32 j = 0; j < 16; ++j) {
						work->mStatus[i][j].mFlags = 0;
					}
				}
			}
			spider->mTimeMode = lbl_8025C400.mTimeState.mMode;
		}
		spider->mStage = lbl_8025C400.mStage;
		fn_800FFB00(spider);
		fn_800FF168(spider);
		spider->_AB4 = spider->_258;
		if (work->mStatus[spider->mStage][spider->_258].mFlags & 0x80000) {
			spider->mStep      = 0;
			spider->mTimer     = 0;
			spider->mPrevState = spider->mState;
			spider->mState     = 2;
		}
		if (spider->_258 == 0) {
			s32 found  = 0;
			XObj** out = work->mFx;
			for (u32 i = 0; i < 12; ++i) {
				work->mFx[i]  = NULL;
				work->_170[i] = -1;
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
		return TRUE;
	}
	return FALSE;
}
