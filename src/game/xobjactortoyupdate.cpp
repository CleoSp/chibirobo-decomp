#include "game/xobjactor.h"
#include "game/xobjs.h"
#include "game/cgame.h"

extern "C" {
void fn_8011CF94(XObjActorToy* object, u32 frames);
void fn_8011C834(XObjActorToy* object, u32 frames);
void fn_8011C44C(XObjActorToy* object, u32 frames);
void fn_8011BE44(XObjActorToy* object, u32 frames);
void fn_8011B7EC(XObjActorToy* object, u32 frames);
void fn_8011A390(XObjActorToy* object, u32 frames);
void fn_8011A050(XObjActorToy* object, u32 frames);
void fn_80119C24(XObjActorToy* object, u32 frames);
void fn_80119774(XObjActorToy* object, u32 frames);
}

/**
 * @note Address: 0x8011F29C
 * @note Size: 0x220
 */
extern "C" void vf148__12XObjActorToyFUl(XObjActorToy* object, u32 frames)
{
	fn_80107750(object, frames);
	switch (object->GetKind()) {
	case 0xF:
		fn_8011CF94(object, frames);
		break;
	case 0x6F:
		fn_8011C834(object, frames);
		break;
	case 0x110:
		break;
	case 0x117:
		fn_8011C44C(object, frames);
		break;
	case 0x156:
		if (object->_2098.mFlags & 1) {
			for (s32 i = 0; i < 15; ++i) {
				u32 stride   = lbl_8025C400.mPools[7].mStride;
				u8* pool     = lbl_8025C400.mPools[7].mObjects;
				XObj* target = (XObj*)(pool + (u32)i * stride);
				if (target->GetKind() == 0x157) {
					Vec position             = target->mMoveTranslate;
					object->mMoveTranslate.x = position.x;
					object->mMoveTranslate.y = position.y;
					object->mMoveTranslate.z = position.z;
				}
			}
		}
		break;
	case 0x158:
		break;
	case 0x159:
		fn_8011BE44(object, frames);
		break;
	case 0x16C:
	case 0x1D3:
		break;
	case 0x1FB:
		fn_8011B7EC(object, frames);
		break;
	case 0x1FC:
		fn_8011A390(object, frames);
		break;
	case 0x25E:
		fn_80119774(object, frames);
		break;
	case 0x26C:
	case 0x485:
		fn_8011A050(object, frames);
		break;
	case 0x26D:
		fn_80119C24(object, frames);
		break;
	}
}
