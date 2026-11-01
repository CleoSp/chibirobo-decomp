#include "game/xobjactor.h"
#include "game/xobjs.h"
#include "game/cgame.h"

/**
 * @note Address: 0x8012BB5C
 * @note Size: 0x288
 */
extern "C" void Drop__13XObjActorPapaFv(XObjActorPapa* object)
{
	u32 flags = object->_C8C.mFlags;
	s32 kind  = -1;
	if (flags & 1) {
		object->_C8C.mFlags &= ~1;
		kind = 0x1F4;
	} else if (flags & 2) {
		object->_C8C.mFlags &= ~2;
		kind = 0x360;
	} else if (flags & 4) {
		object->_C8C.mFlags &= ~4;
		kind = 0x1D3;
	} else if (flags & 8) {
		object->_C8C.mFlags &= ~8;
		kind = 0x375;
	} else if (flags & 0x10) {
		object->_C8C.mFlags &= ~0x10;
		kind = 0x3FA;
	} else if (flags & 0x20) {
		object->_C8C.mFlags &= ~0x20;
		kind = 0x401;
	}
	if (kind < 0)
		return;
	switch (kind) {
	case 0x1D3: {
		// retail walks pool 10 here and does nothing with it
		u32 stride = lbl_8025C400.mPools[7].mStride;
		u32 offset = 0;
		for (s32 i = 0; i < 15; ++i)
			offset += stride;
		break;
	}
	case 0x1F4:
	case 0x360:
	case 0x3FA:
	case 0x401:
		for (s32 i = 0; i < 288; ++i) {
			u32 stride   = lbl_8025C400.mPools[19].mStride;
			u8* pool     = lbl_8025C400.mPools[19].mObjects;
			XObj* target = (XObj*)(pool + (u32)i * stride);
			if (kind == target->GetKind())
				target->mStatus.mFlags &= ~0x10000ULL;
		}
		break;
	case 0x375: {
		u32 stride = lbl_8025C400.mPools[5].mStride;
		u32 offset = 0;
		for (s32 i = 0; i < 64; ++i)
			offset += stride;
		break;
	}
	}
}
