#include "game/xobjspidersmall.h"
#include "game/xchr.h"
#include "game/cgame.h"
#include "game/infoscrn.h"
#include "game/gmath.h"
#include <stdlib.h>

/**
 * @note Address: 0x800F53F8
 * @note Size: 0xBC
 */
// shows or hides the item_junk_unko objects
extern "C" void fn_800F53F8(u8 enabled)
{
	for (s32 i = 0; i < 96; ++i) {
		XObj* obj = (XObj*)(lbl_8025C400.mPools[4].mObjects + i * lbl_8025C400.mPools[4].mStride);
		if (obj->mStatus.mFlags & 1) {
			if (obj->GetKind() == 0x104) {
				if (enabled) {
					obj->mStatus.mFlags |= 0x10000ULL;
				} else {
					obj->mStatus.mFlags &= ~0x10000ULL;
				}
			}
		}
	}
}

/**
 * @note Address: 0x800F54B4
 * @note Size: 0xD4
 */
// lets go of the player
extern "C" void fn_800F54B4(XObjSpiderSmall* spider)
{
	spider->mSpiderStatus.mFlags &= ~0x00800000U;
	if (spider->mSpiderStatus.mFlags & 0x01000000U) {
		spider->mSpiderStatus.mFlags &= ~0x01000000U;
		lbl_8065B2D4 = 0;
		lbl_8025C400.mPlayer->vf12C(1, 0, 0, 0, 0.0f);
		fn_80066190((InfoScrn*)lbl_8025C400.mInfoScrn, 0, 1);
	}
	lbl_8065B2D8 = (s32)((u32)lbl_8065B2D8 - 1);
	if (lbl_8065B2D8 < 0) {
		lbl_8065B2D8 = 0;
	}
	if (lbl_8065B2D8 == 0) {
		lbl_8025C400.mPlayer->_A40 = 1.0f;
	}
}

/**
 * @note Address: 0x800F5588
 * @note Size: 0xF0
 */
extern "C" s32 fn_800F5588()
{
	switch (lbl_8025C400.mPlayer->mSuit) {
	case 1:
	case 6:
		return rand() % 10 + 10;
	case 2:
	case 3:
		return rand() % 2 + 2;
	case 4:
	case 8:
		return rand() % 20 + 20;
	default:
		return rand() % 10 + 10;
	}
}

/**
 * @note Address: 0x800F5678
 * @note Size: 0xC
 */
extern "C" u8 fn_800F5678(XObjSpiderSmall* spider)
{
	return (spider->mSpiderStatus.mFlags >> 6) & 1;
}

/**
 * @note Address: 0x800F5684
 * @note Size: 0xC
 */
extern "C" u8 fn_800F5684(XObjSpiderSmall* spider)
{
	return (spider->mSpiderStatus.mFlags >> 23) & 1;
}

/**
 * @note Address: 0x800F5690
 * @note Size: 0x130
 */
extern "C" BOOL fn_800F5690(XObjSpiderSmall* spider)
{
	f32 x    = spider->mTargetPos.x - spider->mMoveTranslate.x;
	f32 y    = spider->mTargetPos.y - spider->mMoveTranslate.y;
	f32 z    = spider->mTargetPos.z - spider->mMoveTranslate.z;
	f32 dist = sqrtf(x * x + y * y + z * z);
	return dist <= spider->mTargetRadius + spider->mRadius;
}
