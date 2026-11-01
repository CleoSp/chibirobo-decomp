#include "game/xobjactor.h"
#include "game/xobjs.h"
#include "game/cgame.h"

/**
 * @note Address: 0x8012527C
 * @note Size: 0x298
 */
extern "C" void fn_8012527C(XObjActorJenny* object)
{
	u32 flags = object->_C98.mFlags;
	s32 kind  = -1;
	if (flags & 1) {
		object->_C98.mFlags &= ~1;
		kind = 0x3D;
	} else if (flags & 2) {
		object->_C98.mFlags &= ~2;
		kind = 0x3E;
	} else if (flags & 4) {
		object->_C98.mFlags &= ~4;
		kind = 0x3F;
	} else if (flags & 8) {
		object->_C98.mFlags &= ~8;
		kind = 0x3B;
	} else if (flags & 0x10) {
		object->_C98.mFlags &= ~0x10;
		kind = 0x401;
	}
	if (kind < 0)
		return;
	switch (kind) {
	case 0x3B:
	case 0x3D:
	case 0x3E:
	case 0x3F:
		for (s32 i = 0; i < 64; ++i) {
			u32 stride   = lbl_8025C400.mPools[5].mStride;
			u8* pool     = lbl_8025C400.mPools[5].mObjects;
			XObj* target = (XObj*)(pool + (u32)i * stride);
			if (kind == target->GetKind())
				target->mStatus.mFlags &= ~0x10000ULL;
		}
		break;
	case 0x401:
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
