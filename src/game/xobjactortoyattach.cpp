#include "game/xobjactor.h"
#include "game/xobjs.h"
#include "game/cgame.h"

extern "C" {
extern XObjActorToy::StateFunc lbl_8022CFE0;
}

static inline XObj* PoolObject(s32 pool, s32 index)
{
	return (XObj*)(lbl_8025C400.mPools[pool].mObjects + index * (u32)lbl_8025C400.mPools[pool].mStride);
}

/**
 * @note Address: 0x8011E3A4
 * @note Size: 0x2E4
 */
extern "C" void fn_8011E3A4(XObjActorToy* object)
{
	s32 kind    = -1;
	u32 pending = object->_C84.mFlags;
	if (pending & 1) {
		object->_C84.mFlags &= ~1U;
		kind = 0x3D;
	} else if (pending & 2) {
		object->_C84.mFlags &= ~2U;
		kind = 0x3E;
	} else if (pending & 4) {
		object->_C84.mFlags &= ~4U;
		kind = 0x3F;
	} else if (pending & 8) {
		object->_C84.mFlags &= ~8U;
		kind = 0x3B;
	} else if (pending & 0x80) {
		object->_C84.mFlags &= ~0x80U;
		kind = 0x21D;
	} else if (pending & 0x10) {
		object->_C84.mFlags &= ~0x10U;
		kind = 0x21E;
	} else if (pending & 0x40) {
		object->_C84.mFlags &= ~0x40U;
		kind = 0x341;
	}
	if (kind < 0)
		return;
	switch (kind) {
	case 0x3B:
	case 0x3D:
	case 0x3E:
	case 0x3F:
	case 0x21D:
	case 0x21E:
		for (s32 i = 0; i < 64; ++i) {
			XObj* target = PoolObject(5, i);
			if (kind == target->GetKind())
				target->mStatus.mFlags &= ~0x10000ULL;
		}
		break;
	case 0x341:
		for (s32 i = 0; i < 288; ++i) {
			XObj* target = PoolObject(19, i);
			if (kind == target->GetKind())
				target->mStatus.mFlags &= ~0x10000ULL;
		}
		break;
	}
}

/**
 * @note Address: 0x8011E688
 * @note Size: 0x254
 */
extern "C" void fn_8011E688(XObjActorToy* object)
{
	switch (object->mCmd[0]) {
	case 0:
		break;
	case 1:
		object->_A4C   = object->mCmd[1];
		object->mState = lbl_8022CFE0;
		fn_8010706C(object);
		object->mCmd[0] = 0;
		break;
	case 2:
		object->_A4C = -1;
		if (object->_A50 != NULL)
			object->_A50->Release();
		object->mCmd[0] = 0;
		break;
	case 3:
		fn_8011E3A4(object);
		switch (object->mCmd[1]) {
		case 0x341:
			for (s32 i = 0; i < 288; ++i) {
				XObj* target = PoolObject(19, i);
				s32 kind     = target->GetKind();
				if (kind == object->mCmd[1]) {
					object->_A84 = target;
					target->mStatus.mFlags |= 0x10000ULL;
					switch (object->mCmd[1]) {
					case 0x341:
						object->_C84.mFlags |= 0x40;
						break;
					}
				}
			}
			break;
		default:
			if (object->_A84 != NULL) {
				HSD_JObjClearMtxFlags(object->_A84->mJObj);
				object->_A84 = NULL;
			}
			break;
		}
		object->mCmd[0] = 0;
		break;
	}
}
