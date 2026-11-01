#include "game/xobjspider.h"
#include "game/gmath.h"

/**
 * @note Address: 0x800EAB58
 * @note Size: 0x3C0
 */
extern "C" void fn_800EAB58(CarryInfo* carry, u8 slot, bool sideways)
{
	if (slot != 255 && (carry->mFlags.mFlags & 1)) {
		XObj* model = lbl_8037E5C0[slot];
		if (model != NULL) {
			HSD_JObj* joint = model->mJObj;
			HSD_JObjSetTranslateX(joint, carry->mPos.x);
			HSD_JObjSetTranslateY(joint, carry->mPos.y);
			HSD_JObjSetTranslateZ(joint, carry->mPos.z);
			if (sideways) {
				HSD_JObjSetRotationX(joint, HALF_PI);
				HSD_JObjSetRotationZ(joint, HALF_PI + carry->mAngle);
			} else {
				HSD_JObjSetRotationY(joint, carry->mAngle);
			}
			HSD_JObjDispAll(joint, NULL, 7, 0);
		}
	}
}
