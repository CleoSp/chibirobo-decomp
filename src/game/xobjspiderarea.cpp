#include "game/xobjspider.h"
#include "game/cgame.h"
#include "game/gmath.h"
#include "game/csound.h"

/**
 * @note Address: 0x800EB928
 * @note Size: 0x108
 */
extern "C" void fn_800EB928(XObjSpider* spider)
{
	switch (spider->_258 % 5) {
	case 0:
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x12B, 255);
		break;
	case 1:
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x12C, 255);
		break;
	case 2:
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x12D, 255);
		break;
	case 3:
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x12E, 255);
		break;
	case 4:
		fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x12F, 255);
		break;
	}
}

/**
 * @note Address: 0x800EBA30
 * @note Size: 0x1C0
 */
extern "C" s32 fn_800EBA30(XObjSpider* spider)
{
	for (s32 i = 0; i < 16; ++i) {
		XObjSpider* other = (XObjSpider*)(lbl_8025C400.mPools[14].mObjects + i * lbl_8025C400.mPools[14].mStride);
		if (other->mStatus.mFlags & 1ULL) {
			if ((other->mStatus.mFlags & 1ULL) && (other->mStatus.mFlags & 0x10000ULL)) {
				if ((other->mFlags.mFlags & 0x100000) && other->_258 != spider->_258) {
					f32 x = other->mMoveTranslate.x - spider->mMoveTranslate.x;
					f32 y = other->mMoveTranslate.y - spider->mMoveTranslate.y;
					f32 z = other->mMoveTranslate.z - spider->mMoveTranslate.z;
					if (sqrtf(x * x + y * y + z * z) < 24.0f)
						return other->_258;
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
extern "C" BOOL fn_800EBBF0(XObjSpider* spider)
{
	for (s32 i = 0; i < 16; ++i) {
		XObjSpider* other = (XObjSpider*)(lbl_8025C400.mPools[14].mObjects + i * lbl_8025C400.mPools[14].mStride);
		if (other->mStatus.mFlags & 1ULL) {
			if ((other->mStatus.mFlags & 1ULL) && (other->mStatus.mFlags & 0x10000ULL)) {
				if ((other->mFlags.mFlags & 0x100000) && other->_258 != spider->_258) {
					f32 x = other->mMoveTranslate.x - spider->mMoveTranslate.x;
					f32 y = other->mMoveTranslate.y - spider->mMoveTranslate.y;
					f32 z = other->mMoveTranslate.z - spider->mMoveTranslate.z;
					if (sqrtf(x * x + y * y + z * z) < 24.0f)
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
extern "C" u32 fn_800EBDB0(XObjSpider* spider)
{
	return (spider->mFlags.mFlags >> 19) & 1;
}

/**
 * @note Address: 0x800EBDBC
 * @note Size: 0x64
 */
extern "C" BOOL fn_800EBDBC(XObjSpider* spider)
{
	f32 left  = spider->mAreaPos.x + spider->mRadius;
	f32 right = (spider->mAreaPos.x + spider->mAreaWidth) - spider->mRadius;
	f32 near  = spider->mAreaPos.z + spider->mRadius;
	f32 far   = (spider->mAreaPos.z + spider->mAreaDepth) - spider->mRadius;
	if (left > spider->mMoveTranslate.x || right < spider->mMoveTranslate.x || near > spider->mMoveTranslate.z
	    || far < spider->mMoveTranslate.z)
		return FALSE;
	return TRUE;
}
