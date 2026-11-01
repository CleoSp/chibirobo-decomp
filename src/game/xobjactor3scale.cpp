#include "game/xobjactor.h"
#include "game/cgame.h"

/**
 * @note Address: 0x8010BF7C
 * @note Size: 0x4
 */
extern "C" void fn_8010BF7C(XObjActor3* actor, u32 frames)
{
}

/**
 * @note Address: 0x8010BF80
 * @note Size: 0x170
 */
// the first item 16 actor out goes back to its small first anim
extern "C" void fn_8010BF80()
{
	u32 stride  = lbl_8025C400.mPools[2].mStride;
	u8* objects = lbl_8025C400.mPools[2].mObjects;
	u32 offset  = 0;
	for (s32 i = 0; i < 48; ++i) {
		XObjActor3* actor = (XObjActor3*)(objects + offset);
		if (actor->mAttr._00->GetItem() == 16 && (actor->mStatus.mFlags & 1ULL)) {
			actor->_B80 = 0;
			actor->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			actor->_B8C.mFlags |= 4;
			actor->_B8C.mFlags |= 0x10;
			actor->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			actor->_B8C.mFlags &= ~4U;
			actor->_B8C.mFlags &= ~0x10U;
			actor->SetScale(0.1f, 0.1f, 0.1f, 0);
			actor->_B80 = 0;
			break;
		}
		offset += stride;
	}
}

/**
 * @note Address: 0x8010C0F0
 * @note Size: 0x214
 */
extern "C" void fn_8010C0F0(s32 anim)
{
	u32 stride  = lbl_8025C400.mPools[2].mStride;
	u8* objects = lbl_8025C400.mPools[2].mObjects;
	u32 offset  = 0;
	for (s32 i = 0; i < 48; ++i) {
		XObjActor3* actor = (XObjActor3*)(objects + offset);
		if (actor->mAttr._00->GetItem() == 16 && (actor->mStatus.mFlags & 1ULL)) {
			actor->SetAnim(anim, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			actor->_B8C.mFlags &= ~4U;
			actor->_B8C.mFlags &= ~0x10U;
			switch (anim) {
			case 0:
				actor->SetScale(0.1f, 0.1f, 0.1f, 0);
				actor->_B80 = 0;
				break;
			case 1:
				actor->SetScale(1.4f, 1.4f, 1.4f, 0);
				actor->_B80 = 1;
				break;
			case 2:
				actor->SetScale(1.0f, 1.0f, 1.0f, 0);
				actor->_B80 = 2;
				break;
			case 3:
				actor->SetScale(1.0f, 1.0f, 1.0f, 0);
				actor->_B80 = 1;
				break;
			case 4:
				actor->SetScale(1.0f, 1.0f, 1.0f, 0);
				actor->_B80 = 4;
				break;
			}
			break;
		}
		offset += stride;
	}
}
