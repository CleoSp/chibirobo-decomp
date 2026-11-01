#include "game/xobjleticker.h"
#include "game/cgame.h"

/**
 * @note Address: 0x800E51BC
 * @note Size: 0x324
 */
extern "C" void vfAC__12XObjLetickerFv(XObjLeticker* leticker)
{
	leticker->vfB0(NULL);
	switch (leticker->mState) {
	case 1: {
		CXObj* target   = lbl_8025C400.GetObject(leticker->mLinked[0]);
		s32 path[8]     = { 0, 0, 0, 0, 0, 0, 0, 0 };
		HSD_JObj* joint = GetJointByPath(target->mJObj, path, 7);
		HSD_JObjClearFlags(joint, 0x10);
		s32 i = 0;
		f32 height;
		while (0.0f < (height = leticker->mHeight - 24.0f * i)) {
			HSD_JObjSetTranslateY(joint, height);
			HSD_JObjDispAll(joint, NULL, 7, 0);
			++i;
		}
		HSD_JObjSetFlags(joint, 0x10);
		HSD_JObjSetTranslateY(joint, 0.0f);
		break;
	}
	case 2:
		lbl_8025C400.GetObject(leticker->mLinked[0])->mStatus.mFlags &= ~0x80000000ULL;
		break;
	}
}
