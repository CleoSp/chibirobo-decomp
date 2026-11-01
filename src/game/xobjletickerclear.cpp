#include "game/xobjleticker.h"
#include "game/cgame.h"
#include <math.h>
extern "C" {
#include <dolphin/mtx/mtx.h>
}

/**
 * @note Address: 0x800E4790
 * @note Size: 0x31C
 */
extern "C" BOOL fn_800E4790(CXObj* obj, HSD_JObj* joint)
{
	// left unset without a joint, retail does this
	CVec position;
	if (joint != NULL) {
		MtxPtr matrix = HSD_JObjGetMtxPtr(joint);
		position.x = position.y = position.z = 0.0f;
		PSMTXMultVec(matrix, &position, &position);
	}
	position.y -= 6.0f;
	for (s32 i = 0; i < lbl_8025C400.mObjectCount; ++i) {
		XObj* target = lbl_8025C400.GetObject(i);
		if (!(target->mStatus.mFlags & 1ULL) || !(target->mStatus.mFlags & 0x10000ULL))
			continue;
		if ((s32)obj->mId == (s32)target->mId)
			continue;
		if ((s16)target->GetPool() == 2)
			continue;
		f32 x        = target->mMoveTranslate.x - position.x;
		f32 z        = target->mMoveTranslate.z - position.z;
		f32 distance = sqrtf(x * x + z * z);
		if (distance > 100.0f + (5.0f + target->_6F8))
			continue;
		CVec hit, normal;
		if (target->vfFC(&position, &position, 5.0f, 12.0f, &hit, &normal, 0, 0) & 2)
			return FALSE;
	}
	return TRUE;
}
