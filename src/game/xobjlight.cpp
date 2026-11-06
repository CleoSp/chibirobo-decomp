#include "game/xobjmap.h"

#pragma section RX ".text.stagelight"

extern "C" const f32 sStageLightAnimStartFrame;

/**
 * @note Address: 0x80092E38
 * @note Size: 0xC8
 */
__declspec(section ".text.stagelight") void XObjLight::UpdateObject(u32 frames)
{
	if ((s32)GetEnvData()[4] == 2) {
		for (int i = 0; i < mLightNum; ++i) {
			if (GetLightDesc(i) && mArchive.mNum[i + 2] > 0) {
				mLights[i].SetAnimRate((f32)frames);
				mLights[i].Anim();
			}
		}
	}
}

/**
 * @note Address: 0x80092F00
 * @note Size: 0x98
 */
__declspec(section ".text.stagelight") void XObjLight::Release()
{
	if ((s32)GetEnvData()[4] == 2) {
		for (s32 i = 0; i < mLightNum; ++i) {
			if (GetLightDesc(i))
				mLights[i].Release();
		}
	}
	XObj::Release();
}

/**
 * @note Address: 0x80092F98
 * @note Size: 0x14C
 */
__declspec(section ".text.stagelight") BOOL XObjLight::LoadFromPlacement(const CoordInfo& info)
{
	if (XObj::LoadFromPlacement(info)) {
		if ((s32)GetEnvData()[4] == 2) {
			HSD_LObj* previous = NULL;
			for (int i = 0; i < 8; ++i) {
				if (GetLightDesc(i)) {
					int lightMask = GetEnvData()[5];
					mLights[i].Init(GetLightDesc(i)->mDesc, lightMask & (1 << (7 - i)));
					if (mArchive.mNum[i + 2] > 0) {
						mLightAnims[i] = 0;
						mLights[i].SetAnim(GetLightDesc(i)->mAnims[mLightAnims[i]], sStageLightAnimStartFrame);
					}
					++mLightNum;
					if (previous)
						previous->next = &mLights[i];
					previous = &mLights[i];
				}
			}
			if (previous)
				previous->next = NULL;
		}
		mStatus.mFlags &= ~0x10000ULL;
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x800930E4
 * @note Size: 0x30
 */
__declspec(section ".text.stagelight") void XObjLight::ResetObjectState()
{
	mLightNum = 0;
	for (u32 i = 0; i < 8; ++i)
		mLightAnims[i] = -1;
}

/**
 * @note Address: 0x80093114
 * @note Size: 0x20
 */
__declspec(section ".text.stagelight") void XObjLight::InitializeObject(u32 id)
{
	XObj::InitializeObject(id);
}
