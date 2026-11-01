#include "game/xobjactor.h"
#include "game/xobjs.h"
#include "game/cgame.h"
#include "game/hsd_cobj.h"
#include "game/csound.h"

extern "C" {
bool fn_800EBDB0(XObj* object);
}

static inline XObjActor3* PoolObject(u32 index)
{
	u32 stride = lbl_8025C400.mPools[2].mStride;
	u8* pool   = lbl_8025C400.mPools[2].mObjects;
	return (XObjActor3*)(pool + index * stride);
}

/**
 * @note Address: 0x8010CDDC
 * @note Size: 0xA4
 */
extern "C" void fn_8010CDDC(f32 x, f32 y, f32 z)
{
	for (s32 i = 0; i < 48; ++i) {
		XObjActor3* object = PoolObject(i);
		switch (object->GetKind()) {
		case 0x43A:
			if ((object->mStatus.mFlags & 1ULL) && (object->_B8C.mFlags & 4)) {
				object->_B8C.mFlags &= ~4U;
				object->mMoveTranslate.x = x;
				object->mMoveTranslate.y = y;
				object->mMoveTranslate.z = z;
			}
			break;
		}
	}
}

/**
 * @note Address: 0x8010CE80
 * @note Size: 0x294
 */
extern "C" void fn_8010CE80(XObjActor3* object, u32 speed)
{
	switch (object->_B80) {
	case 0:
		if (!(object->_B8C.mFlags & 4)) {
			++object->_B80;
			object->SetAnim(1, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			++lbl_8065B2F4;
			if (lbl_8065B2F4 >= 10)
				lbl_8065B2F4 = 0;
			CSound* sound = (CSound*)&lbl_8025C400.mSound;
			if (!fn_8001E6C8(sound, 0x127))
				fn_8001E904(sound, 0x127, 255);
		}
		break;
	case 1: {
		HSD_AObj* animation = GetAObj(object->mJObj);
		f32 end             = animation != NULL ? HSD_AObjGetEndFrame(animation) : 0.0f;
		animation           = GetAObj(object->mJObj);
		f32 current         = animation != NULL ? HSD_AObjGetCurrentFrame(animation) : 0.0f;
		if (end - (f32)speed <= current) {
			++object->_B80;
			object->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
		break;
	}
	case 2:
		object->_B8C.mFlags |= 4;
		object->_B80 = 0;
		break;
	}
}

/**
 * @note Address: 0x8010D114
 * @note Size: 0x27C
 */
extern "C" void fn_8010D114(XObjActor3* object, u32 speed)
{
	switch (object->_BEC) {
	case 0:
		object->_BEC = 4;
		object->_BF0 = 0.0f;
		break;
	case 1:
		object->_BF0 = 1.0f;
		break;
	case 2:
		object->_BF0 = 0.0f;
		break;
	case 3:
		object->_BF0 += 0.08f * (f32)speed;
		if (object->_BF0 >= 1.0f)
			object->_BF0 = 1.0f;
		break;
	case 4:
		object->_BF0 -= 0.08f * (f32)speed;
		if (object->_BF0 < 0.0f)
			object->_BF0 = 0.0f;
		break;
	}
	if (object->_BF8)
		object->_BF4 -= (f32)speed;
	switch (object->_BFC) {
	case 0:
		object->_BFC = 4;
		object->_C00 = 0.0f;
		break;
	case 1:
		object->_C00 = 1.0f;
		break;
	case 2:
		object->_C00 = 0.0f;
		break;
	case 3:
		object->_C00 += 0.08f * (f32)speed;
		if (object->_C00 >= 1.0f)
			object->_C00 = 1.0f;
		break;
	case 4:
		object->_C00 -= 0.08f * (f32)speed;
		if (object->_C00 < 0.0f)
			object->_C00 = 0.0f;
		break;
	}
	s32 count = 0;
	for (s32 i = 0; i < 16; ++i) {
		u32 stride      = lbl_8025C400.mPools[14].mStride;
		u8* pool        = lbl_8025C400.mPools[14].mObjects;
		XObj* candidate = (XObj*)(pool + i * stride);
		if ((candidate->mStatus.mFlags & 0x10000ULL) && !fn_800EBDB0(candidate))
			++count;
	}
	object->_C04 = count;
}

/**
 * @note Address: 0x8010D390
 * @note Size: 0x2CC
 */
extern "C" void fn_8010D390(s32 mode)
{
	if (mode == 0) {
		for (s32 i = 0; i < 48; ++i) {
			XObjActor3* object = PoolObject(i);
			if (object->GetKind() == 0x3E0 && (object->mStatus.mFlags & 1ULL)) {
				object->_B8C.mFlags &= ~4U;
				object->_B80 = 0;
				lbl_8065B2F0 = 0;
			}
		}
	}
	if (mode == 2) {
		for (s32 i = 0; i < 48; ++i) {
			XObjActor3* object = PoolObject(i);
			if (object->GetKind() == 0x3E0 && (object->mStatus.mFlags & 1ULL) && (object->_B8C.mFlags & 4)) {
				object->_B8C.mFlags &= ~4U;
				object->_B80 = 20;
				lbl_8065B2F0 = 0;
			}
		}
	} else {
		u32 stride = lbl_8025C400.mPools[2].mStride;
		u8* pool   = lbl_8025C400.mPools[2].mObjects;
		u32 offset = 0;
		for (s32 i = 0; i < 48; ++i, offset += stride) {
			XObjActor3* object = (XObjActor3*)(pool + offset);
			if (object->GetKind() == 0x3E0) {
				if (!(object->mStatus.mFlags & 1ULL))
					return;
				if (object->_B8C.mFlags & 4) {
					object->_B8C.mFlags &= ~4U;
					object->_B80 = 10;
					return;
				}
			}
		}
	}
}
