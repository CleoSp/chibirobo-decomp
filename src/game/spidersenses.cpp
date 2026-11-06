#include "game/xobjspider.h"
#include "game/cgame.h"
#include "game/gmath.h"

#pragma section RX ".text.spidersenses"

/**
 * @note Address: 0x800EB928
 * @note Size: 0x108
 */
extern "C" __declspec(section ".text.spidersenses") void PlaySpiderVariantSound(XObjSpider* spider)
{
	switch (spider->mPoolIndex % 5) {
	case 0:
		PlayGameSound(&gGame.mSound, 0x12B, 255);
		break;
	case 1:
		PlayGameSound(&gGame.mSound, 0x12C, 255);
		break;
	case 2:
		PlayGameSound(&gGame.mSound, 0x12D, 255);
		break;
	case 3:
		PlayGameSound(&gGame.mSound, 0x12E, 255);
		break;
	case 4:
		PlayGameSound(&gGame.mSound, 0x12F, 255);
		break;
	}
}

/**
 * @note Address: 0x800EBA30
 * @note Size: 0x1C0
 */
// the index of another spider that dropped down closer than 24, -1 when there is none
extern "C" __declspec(section ".text.spidersenses") s32 FindNearbyDroppedSpider(XObjSpider* spider)
{
	for (int i = 0; i < 16; i++) {
		XObj* obj = gGame.mXObjs.mPools[17].Get(i);
		if ((obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 0x10000)) {
			XObjSpider* other = (XObjSpider*)obj;
			if ((other->mFlags.mFlags & 0x100000) && other->mPoolIndex != spider->mPoolIndex) {
				f32 x = other->mMoveTranslate.x - spider->mMoveTranslate.x;
				f32 y = other->mMoveTranslate.y - spider->mMoveTranslate.y;
				f32 z = other->mMoveTranslate.z - spider->mMoveTranslate.z;
				if (sqrtf(x * x + y * y + z * z) < 24.0f) {
					return other->mPoolIndex;
				}
			}
		}
	}
	return -1;
}

/**
 * @note Address: 0x800EBBF0
 * @note Size: 0x1C0
 */
// whether another spider that dropped down is closer than 24
extern "C" __declspec(section ".text.spidersenses") BOOL HasNearbyDroppedSpider(XObjSpider* spider)
{
	for (int i = 0; i < 16; i++) {
		XObj* obj = gGame.mXObjs.mPools[17].Get(i);
		if ((obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 0x10000)) {
			XObjSpider* other = (XObjSpider*)obj;
			if ((other->mFlags.mFlags & 0x100000) && other->mPoolIndex != spider->mPoolIndex) {
				f32 x = other->mMoveTranslate.x - spider->mMoveTranslate.x;
				f32 y = other->mMoveTranslate.y - spider->mMoveTranslate.y;
				f32 z = other->mMoveTranslate.z - spider->mMoveTranslate.z;
				if (sqrtf(x * x + y * y + z * z) < 24.0f) {
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

/**
 * @note Address: 0x800EBDB0
 * @note Size: 0xC
 */
extern "C" __declspec(section ".text.spidersenses") u8 HasSpiderFinishedDeathAnimation(const XObjSpider* spider)
{
	return (spider->mFlags.mFlags >> 19) & 1;
}

/**
 * @note Address: 0x800EBDBC
 * @note Size: 0x64
 */
// whether it is inside its area, kept mRadius off the edges
extern "C" __declspec(section ".text.spidersenses") BOOL IsSpiderInsideMovementArea(XObjSpider* spider)
{
	f32 z     = spider->mMoveTranslate.z;
	f32 right = (spider->mAreaPos.x + spider->mAreaWidth) - spider->mRadius;
	f32 left  = spider->mAreaPos.x + spider->mRadius;
	f32 near  = spider->mAreaPos.z + spider->mRadius;
	f32 far   = (spider->mAreaPos.z + spider->mAreaDepth) - spider->mRadius;
	if (left > spider->mMoveTranslate.x || right < spider->mMoveTranslate.x || near > z || far < z)
		return FALSE;
	return TRUE;
}
