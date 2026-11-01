#include "game/xobjdemo.h"
#include "game/cgame.h"

/**
 * @note Address: 0x80028368
 * @note Size: 0xA4
 */
// XObjDemo's Release
extern "C" void fn_80028368(XObjDemo* demo)
{
	if (demo->mArchive.mDesc[0] != NULL) {
		demo->mCamera.Release();
	}
	for (s32 i = 0; i < demo->mLightCount; ++i) {
		if (demo->mArchive.mDesc[i + 1] != NULL) {
			demo->mLights[i].Release();
		}
	}
	fn_800911C0(demo);
}

/**
 * @note Address: 0x8002840C
 * @note Size: 0xD4
 */
// XObjDemo's vf9C, builds the camera and the light list from the archive
extern "C" BOOL fn_8002840C(XObjDemo* demo, CoordInfo* info)
{
	if (fn_80091C80(demo, info)) {
		if (demo->mArchive.mDesc[0] != NULL) {
			demo->mCamera.Init((HSD_CObjDesc*)demo->mArchive.mDesc[0]->_00);
#if defined(VERSION_PAL)
			demo->mCamera.SetViewport(0.0f, 0.0f, (GXRenderModeObj*)lbl_8025C400.mGs);
#endif
		}
		HSD_LObj* prev = NULL;
		for (s32 i = 0; i < 8; ++i) {
			if (demo->mArchive.mDesc[i + 1] != NULL) {
				demo->mLights[i].Init((HSD_LightDesc*)demo->mArchive.mDesc[i + 1]->_00, FALSE);
				demo->mLightCount = (s32)((u32)demo->mLightCount + 1);
				if (prev != NULL) {
					prev->next = &demo->mLights[i];
				}
				prev = &demo->mLights[i];
			}
		}
		if (prev != NULL) {
			prev->next = NULL;
		}
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x800284E0
 * @note Size: 0x64
 */
// XObjDemo's vf98
extern "C" void fn_800284E0(XObjDemo* demo)
{
	fn_800928E8(demo);
	demo->mState      = 0;
	demo->mLoop       = FALSE;
	demo->mCameraAnim = -1;
	demo->mLightCount = 0;
	for (s32 i = 0; i < 8; ++i) {
		demo->mLightAnims[i] = -1;
	}
}
