#include "game/xobjactor.h"
#include "game/xobjs.h"
#include "game/cgame.h"

/**
 * @note Address: 0x80128230
 * @note Size: 0x38C
 */
extern "C" void fn_80128230(XObjActorMama* object)
{
	u32 flags = object->_C8C.mFlags;
	s32 kind  = -1;
	if (flags & 0x4) {
		object->_C8C.mFlags &= ~0x4U;
		kind = 0x7d;
	} else if (flags & 0x1) {
		object->_C8C.mFlags &= ~0x1U;
		kind = 0x40;
	} else if (flags & 0x2) {
		object->_C8C.mFlags &= ~0x2U;
		kind = 0x5b;
	} else if (flags & 0x8) {
		object->_C8C.mFlags &= ~0x8U;
		kind = 0x196;
	} else if (flags & 0x10) {
		object->_C8C.mFlags &= ~0x10U;
		kind = 0x1a1;
	} else if (flags & 0x20) {
		object->_C8C.mFlags &= ~0x20U;
		kind = 0x45b;
	}
	if (kind < 0)
		return;
	switch (kind) {
	case 0x40:
		for (s32 i = 0; i < 64; ++i) {
			u32 stride   = lbl_8025C400.mPools[5].mStride;
			u8* pool     = lbl_8025C400.mPools[5].mObjects;
			XObj* target = (XObj*)(pool + (u32)i * stride);
			if (kind == target->GetKind())
				target->mStatus.mFlags &= ~0x10000ULL;
		}
		break;
	case 0x5B:
	case 0x7D:
	case 0x196:
	case 0x45B:
		for (s32 i = 0; i < 288; ++i) {
			u32 stride   = lbl_8025C400.mPools[19].mStride;
			u8* pool     = lbl_8025C400.mPools[19].mObjects;
			XObj* target = (XObj*)(pool + (u32)i * stride);
			if (kind == target->GetKind())
				target->mStatus.mFlags &= ~0x10000ULL;
		}
		break;
	case 0x1A1:
		for (s32 i = 0; i < 288; ++i) {
			u32 stride   = lbl_8025C400.mPools[19].mStride;
			u8* pool     = lbl_8025C400.mPools[19].mObjects;
			XObj* target = (XObj*)(pool + (u32)i * stride);
			if (target->GetKind() == 0x1A0)
				target->mStatus.mFlags &= ~0x10000ULL;
			else if (kind == target->GetKind())
				target->mStatus.mFlags &= ~0x10000ULL;
		}
		break;
	}
}
