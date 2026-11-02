#include "game/xobjactor.h"
#include "game/xobjspidersmall.h"
#include "game/cgame.h"

/**
 * @note Address: 0x80106A8C
 * @note Size: 0x4
 */
extern "C" void vf15C__9XObjActorFv(XObjActor* actor)
{
}

/**
 * @note Address: 0x80106A90
 * @note Size: 0x8
 */
extern "C" HSD_JObj* vf168__9XObjActorFv(XObjActor* actor)
{
	return actor->mJObj;
}

/**
 * @note Address: 0x80106A98
 * @note Size: 0x28
 */
extern "C" CVec vf16C__9XObjActorFv(XObjActor* actor)
{
	return actor->mMoveTranslate;
}

/**
 * @note Address: 0x80106AC0
 * @note Size: 0x90
 */
extern "C" s32 fn_80106AC0()
{
	s32 count = 0;
	for (s32 i = 0; i < 16; ++i) {
		XObjSpiderSmall* spider = (XObjSpiderSmall*)(lbl_8025C400.mXObjs.mPools[18].mObjects + i * lbl_8025C400.mXObjs.mPools[18].mStride);
		if (spider->mStatus.mFlags & 1ULL) {
			if (fn_800F5678(spider)) {
				++count;
			}
		}
	}
	return count;
}

/**
 * @note Address: 0x80106B50
 * @note Size: 0x90
 */
// small spiders holding on to chibi robo
extern "C" s32 fn_80106B50()
{
	s32 count = 0;
	for (s32 i = 0; i < 16; ++i) {
		XObjSpiderSmall* spider = (XObjSpiderSmall*)(lbl_8025C400.mXObjs.mPools[18].mObjects + i * lbl_8025C400.mXObjs.mPools[18].mStride);
		if (spider->mStatus.mFlags & 1ULL) {
			if (fn_800F5684(spider)) {
				++count;
			}
		}
	}
	return count;
}

/**
 * @note Address: 0x80106BE0
 * @note Size: 0xC4
 */
extern "C" void vf164__9XObjActorFf(XObjActor* actor, f32 frame)
{
	if (actor->mStatus.mFlags & 1ULL) {
		if (actor->mStatus.mFlags & 0x10000ULL) {
			u8 volume;
			if (actor->_998.mFlags & 0x800) {
				volume = 255;
			} else {
				volume = fn_8001DF00(&lbl_8025C400.mSound, &actor->mMoveTranslate, 0);
			}
			actor->mAnimSfx.Update(actor->mAnimId, frame, volume);
		}
	}
}
