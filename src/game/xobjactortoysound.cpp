#include "game/xobjactor.h"
#include "game/xobjs.h"
#include "game/ccameraaction.h"
#include "game/cgame.h"
#include "game/hsd_cobj.h"
#include "game/hsd_dobj.h"
extern "C" {
#include <sysdolphin/tobj.h>
#include "game/csound.h"
}

static inline HSD_AObj* TextureAnimation(HSD_JObj* joint)
{
	if (joint == NULL)
		return NULL;
	HSD_DObj* draw = HSD_JObjGetDObj(joint);
	for (s32 i = 0; i < 4; ++i) {
		if (draw == NULL)
			return NULL;
		draw = HSD_DObjGetNext(draw);
	}
	HSD_MObj* material = HSD_DObjGetMObj(draw);
	if (material == NULL)
		return NULL;
	HSD_TObj* texture = HSD_MObjGetTObj(material);
	if (texture == NULL)
		return NULL;
	return texture->aobj;
}

/**
 * @note Address: 0x8011566C
 * @note Size: 0x44
 */
extern "C" bool fn_8011566C(XObjActor* object)
{
	if (!(object->mStatus.mFlags & 1ULL))
		return true;
	return !(object->_998.mFlags & 0x2000);
}

/**
 * @note Address: 0x801156B0
 * @note Size: 0x2D0
 */
extern "C" void fn_801156B0(XObjActorToy* object, f32 frame)
{
	switch (object->GetKind()) {
	case 0x1FC:
		if (object->mAnimId == 0x3F6) {
			s32 path[3]         = { 0, 2, 0 };
			HSD_AObj* animation = TextureAnimation(GetJointByPath(object->mJObj, path, 3));
			frame               = animation != NULL ? HSD_AObjGetCurrentFrame(animation) : 0.0f;
		}
		if (fn_8001E6C8((CSound*)&lbl_8025C400.mSound, 0x10001F)) {
			if (object->mAnimId != 16) {
				fn_8001E660((CSound*)&lbl_8025C400.mSound, 0x10001F);
			} else {
				fn_8001E468((CSound*)&lbl_8025C400.mSound, 0x10001F, fn_8001DF00((CSound*)&lbl_8025C400.mSound, &object->mMoveTranslate, 0),
				            0);
			}
		}
		break;
	case 0x159:
		if (fn_8001E6C8((CSound*)&lbl_8025C400.mSound, 0xE0008)) {
			if (object->mAnimId != 7)
				fn_8001E660((CSound*)&lbl_8025C400.mSound, 0xE0008);
		}
		break;
	}
	fn_80106BE0(object, frame);
}
