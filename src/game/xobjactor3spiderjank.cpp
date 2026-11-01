#include "game/xobjactor.h"
#include "game/xobjs.h"
#include "game/cgame.h"
#include <math.h>
#include "game/gmath.h"
#include "game/csound.h"

extern "C" {
extern const f32 lbl_8065E1CC;
}

/**
 * @note Address: 0x8010EF70
 * @note Size: 0x298
 */
extern "C" void fn_8010EF70()
{
	u32 stride = lbl_8025C400.mPools[2].mStride;
	u8* pool   = lbl_8025C400.mPools[2].mObjects;
	u32 offset = 0;
	for (s32 i = 0; i < 48; ++i, offset += stride) {
		XObjActor3* object = (XObjActor3*)(pool + offset);
		if ((s16)GetObjDataFlags(object->mAttr._00) == 12) {
			if ((object->mStatus.mFlags & 1ULL) && (object->mStatus.mFlags & 0x10000ULL)) {
				XObj* target = (XObj*)lbl_8025C400.mPools[16].mObjects;
				if (!(target->mStatus.mFlags & 1ULL))
					return;
				if (!(target->mStatus.mFlags & 0x10000ULL))
					return;
				if (object->_B80 != 1)
					return;
				object->_B80 = 2;
				object->_B78 = 6.0f;
				HSD_JObjClearMtxFlags(object->mJObj);
				object->mMoveTranslate.x = lbl_8065E1CC + HSD_JObjGetTranslationX(target->mJObj);
				object->mMoveTranslate.y = 18.0f;
				object->mMoveTranslate.z = lbl_8065E1CC + HSD_JObjGetTranslationZ(target->mJObj);
				object->SetPos(object->mMoveTranslate, 0);
				f32 heading
				    = atan2(target->mMoveTranslate.x - object->mMoveTranslate.x, target->mMoveTranslate.z - object->mMoveTranslate.z);
				object->_B7C = PI + heading;
				object->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x160021, 255);
				return;
			}
		}
	}
}

/**
 * @note Address: 0x8010F208
 * @note Size: 0x110
 */
extern "C" void fn_8010F208()
{
	u32 stride = lbl_8025C400.mPools[2].mStride;
	u8* pool   = lbl_8025C400.mPools[2].mObjects;
	u32 offset = 0;
	for (s32 i = 0; i < 48; ++i, offset += stride) {
		XObjActor3* object = (XObjActor3*)(pool + offset);
		if ((s16)GetObjDataFlags(object->mAttr._00) == 12) {
			if (object->mStatus.mFlags & 1ULL) {
				if (object->_B80 != 0)
					return;
				object->_B8C.mFlags &= ~4U;
				return;
			}
		}
	}
}
