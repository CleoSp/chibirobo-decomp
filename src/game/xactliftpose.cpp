#include "game/xchr.h"
#include "game/cgame.h"
#include "game/cparticle.h"

/**
 * @note Address: 0x800D40EC
 * @note Size: 0x164
 */
extern "C" void fn_800D40EC(CXObj* object)
{
	// retail uses one slot for all three, the rotation getter fills a quaternion
	union {
		Vec vector;
		Quaternion rotation;
	} value;
	HSD_JObjGetTranslation(object->mJObj, &value.vector);
	object->mMoveTranslate.Reset(value.vector);
	HSD_JObjGetRotation(object->mJObj, (Quaternion*)&value.vector);
	object->mMoveRotate.Reset(value.vector);
	HSD_JObjGetScale(object->mJObj, &value.vector);
	object->mMoveScale.Reset(value.vector);
}

/**
 * @note Address: 0x800D4250
 * @note Size: 0x44
 */
extern "C" f32 fn_800D4250(CXObj* object)
{
	return HSD_JObjGetRotationY(object->mJObj);
}

/**
 * @note Address: 0x800D4294
 * @note Size: 0x60
 */
extern "C" void fn_800D4294(XObjChr* player, s32 animation, f32 blend)
{
	player->SetAnim(animation, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, blend, 10.0f);
	player->_A2C = 3;
}

/**
 * @note Address: 0x800D42F4
 * @note Size: 0x3C
 */
extern "C" HSD_Generator* fn_800D42F4(CGame* game, u32 bank, u32 id, u8 type, HSD_JObj* joint, BOOL keep)
{
	return ((CParticle*)game->mParticle)->Create(id | (bank << 16), type, joint, keep);
}

/**
 * @note Address: 0x800D4330
 * @note Size: 0x10
 */
extern "C" s32 fn_800D4330(Params* stats, u32 index)
{
	return stats->mInventory.mItems[index].mCount;
}

/**
 * @note Address: 0x800D4340
 * @note Size: 0x28
 */
extern "C" s32 fn_800D4340(XObj* object)
{
	return object->GetObjFlags();
}
