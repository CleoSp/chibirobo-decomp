#include "game/xobjarmy.h"
#include "game/texturecopy.h"

/**
 * @note Address: 0x800E980C
 * @note Size: 0x370
 */
extern "C" void fn_800E980C(XObjArmy* army)
{
	CGs* gs = (CGs*)lbl_8025C400.mGs;
	for (u32 i = 0; i < 5; ++i) {
		XObjArmyShot* shot = &army->mShots[i];
		if (shot->mActive) {
			if (lbl_8065B2A0 == NULL) {
				gs->_29C = TRUE;
				Vec pos  = shot->mPos;
				fn_8019B56C(gs->mCurRenderMode, 1, 1, 1, 15);
				Vec rot = { 0.0f, 0.0f, 0.0f };
				fn_80198258((CGs*)lbl_8025C400.mGs, 16, 0x64FF64C8, 0x000000FF, pos.x, pos.y, pos.z, 3.0f, 3.0f, rot.x, rot.y, rot.z);
				gs->_29C = FALSE;
			} else {
				HSD_JObj* joint = lbl_8065B2A0->mJObj;
				HSD_JObjSetTranslateX(joint, shot->mPos.x);
				HSD_JObjSetTranslateY(joint, shot->mPos.y);
				HSD_JObjSetTranslateZ(joint, shot->mPos.z);
				HSD_JObjDispAll(joint, NULL, 7, 0);
			}
		}
	}
	InvalidateCaptureGraphics(gs);
}
