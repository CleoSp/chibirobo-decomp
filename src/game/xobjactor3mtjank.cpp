#include "game/xobjactor.h"
#include "game/xobjs.h"
#include "game/cgame.h"
#include <math.h>
#include "game/gmath.h"
#include "game/csound.h"

extern "C" void fn_8010FB1C(XObjActor3* effect);

static inline s16 PlacementType(XObjActor3* effect)
{
	UnkObjData* descriptor = effect->mAttr._00;
	return descriptor != NULL ? descriptor->_16 : 0;
}

/**
 * @note Address: 0x8010FDA0
 * @note Size: 0xC8
 */
extern "C" void fn_8010FDA0(s32 index)
{
	u32 stride  = lbl_8025C400.mPools[2].mStride;
	u8* objects = lbl_8025C400.mPools[2].mObjects;
	s32 ordinal = 0;
	u32 offset  = 0;
	for (s32 i = 0; i < 48; ++i) {
		XObjActor3* effect = (XObjActor3*)(objects + offset);
		if (PlacementType(effect) == 11) {
			if (ordinal == index && (effect->mStatus.mFlags & 1ULL) && (effect->mStatus.mFlags & 0x10000ULL)) {
				fn_8010FB1C(effect);
				return;
			}
			++ordinal;
		}
		offset += stride;
	}
}

/**
 * @note Address: 0x8010FE68
 * @note Size: 0x12C
 */
extern "C" void fn_8010FE68(s32 index)
{
	u32 stride  = lbl_8025C400.mPools[2].mStride;
	u8* objects = lbl_8025C400.mPools[2].mObjects;
	s32 ordinal = 0;
	u32 offset  = 0;
	for (s32 i = 0; i < 48; ++i) {
		XObjActor3* effect = (XObjActor3*)(objects + offset);
		if (PlacementType(effect) == 11) {
			if (ordinal == index && (effect->mStatus.mFlags & 1ULL)) {
				if (effect->_B80 == 0)
					effect->_B8C.mFlags &= ~4U;
				return;
			}
			++ordinal;
		}
		offset += stride;
	}
}

/**
 * @note Address: 0x8010FB1C
 * @note Size: 0x284
 */
extern "C" void fn_8010FB1C(XObjActor3* effect)
{
	XObj* owner = (XObj*)lbl_8025C400.mPools[16].mObjects;
	if (owner->mStatus.mFlags & 1ULL) {
		if (owner->mStatus.mFlags & 0x10000ULL) {
			if (effect->_B80 == 1) {
				f32 x        = effect->_BD8 * (f32)sin(effect->_BC8 + HSD_JObjGetRotationY(owner->mJObj));
				f32 y        = effect->_BD0;
				f32 z        = effect->_BD8 * (f32)cos(effect->_BC8 + HSD_JObjGetRotationY(owner->mJObj));
				effect->_B80 = 2;
				effect->_B78 = 6.0f;
				HSD_JObjClearMtxFlags(effect->mJObj);
				effect->mMoveTranslate.x = x + HSD_JObjGetTranslationX(owner->mJObj);
				effect->mMoveTranslate.y = 10.0f + (8.0f + y);
				effect->mMoveTranslate.z = z + HSD_JObjGetTranslationZ(owner->mJObj);
				effect->SetPos(effect->mMoveTranslate, 0);
				effect->_B7C
				    = PI
				    + (f32)atan2(owner->mMoveTranslate.x - effect->mMoveTranslate.x, owner->mMoveTranslate.z - effect->mMoveTranslate.z);
				fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x160021, 255);
			}
		}
	}
}
