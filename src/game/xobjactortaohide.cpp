#include "game/xobjactor.h"
#include "game/xobjs.h"
#include "game/cgame.h"

/**
 * @note Address: 0x8012EFF4
 * @note Size: 0x320
 */
extern "C" void Drop__12XObjActorTaoFv(XObjActorTao* object)
{
	u32 flags = object->_C8C.mFlags;
	s32 kind  = -1;
	if (flags & 2) {
		object->_C8C.mFlags &= ~2U;
		kind = 0x102;
	} else if (flags & 1) {
		object->_C8C.mFlags &= ~1U;
		kind = 0x159;
	} else if (flags & 4) {
		object->_C8C.mFlags &= ~4U;
		kind = 0xF8;
	} else if (flags & 8) {
		object->_C8C.mFlags &= ~8U;
		kind = 0x2A3;
	} else if (flags & 0x20) {
		object->_C8C.mFlags &= ~0x20U;
		kind = 0x46F;
	}
	if (kind < 0)
		return;
	switch (kind) {
	case 0x102:
		for (s32 i = 0; i < 64; ++i) {
			u32 stride   = lbl_8025C400.mPools[5].mStride;
			u8* pool     = lbl_8025C400.mPools[5].mObjects;
			XObj* target = (XObj*)(pool + (u32)i * stride);
			if (kind == target->GetKind())
				target->mStatus.mFlags &= ~0x10000ULL;
		}
		break;
	case 0x159: {
		// empty walks, retail still adds up the strides
		u32 stride = lbl_8025C400.mPools[7].mStride;
		u32 offset = 0;
		for (s32 i = 0; i < 15; ++i)
			offset += stride;
		break;
	}
	case 0xF8: {
		u32 stride = lbl_8025C400.mPools[13].mStride;
		u32 offset = 0;
		for (s32 i = 0; i < 21; ++i)
			offset += stride;
		break;
	}
	case 0x2A3:
	case 0x46F:
		for (s32 i = 0; i < 288; ++i) {
			u32 stride   = lbl_8025C400.mPools[19].mStride;
			u8* pool     = lbl_8025C400.mPools[19].mObjects;
			XObj* target = (XObj*)(pool + (u32)i * stride);
			if (kind == target->GetKind())
				target->mStatus.mFlags &= ~0x10000ULL;
		}
		break;
	}
}
