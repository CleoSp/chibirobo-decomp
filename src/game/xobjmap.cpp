#include "game/xobjmap.h"

struct XObjMapJointPath {
	s32 mSteps[1]; // _00
};
extern "C" {
extern const XObjMapJointPath lbl_8065FFD8;
}

// XObjMap::vfA4
/**
 * @note Address: 0x8009369C
 * @note Size: 0xCC
 */
extern "C" void Update__7XObjMapFUl(XObjMap* map, u32 frames)
{
	fn_80090ED4(map, frames);
	if ((s32)GetEnvData()[4] == 1) {
		for (s32 i = 0; i < map->mLightNum; ++i) {
			if (map->GetLightDesc(i) && map->GetLightAnimNum(i) > 0) {
				map->mLights[i].SetAnimRate((f32)frames);
				map->mLights[i].Anim();
			}
		}
	}
}

// XObjMap::Release
/**
 * @note Address: 0x80093768
 * @note Size: 0xC0
 */
extern "C" void Release__7XObjMapFv(XObjMap* map)
{
	for (s32 i = 0; i < map->mTrigonCount; ++i)
		map->mTrigon[i].Release();
	if ((s32)GetEnvData()[4] == 1) {
		for (s32 i = 0; i < map->mLightNum; ++i) {
			if (map->GetLightDesc(i))
				map->mLights[i].Release();
		}
	}
	fn_800911C0(map);
}

// XObjMap::vf9C, a floor for every child of the root joint that has triangles
/**
 * @note Address: 0x80093828
 * @note Size: 0x298
 */
extern "C" BOOL vf9C__7XObjMapFPv(XObjMap* map, CoordInfo* info)
{
	if (fn_80091C80(map, info)) {
		s32 child = 0;
		s32 count = 0;
		for (;;) {
			XObjMapJointPath path = lbl_8065FFD8;
			path.mSteps[0]        = child;
			HSD_JObj* joint       = GetJointByPath(map->mJObj, path.mSteps, 1);
			if (!joint)
				break;
			if (map->mTrigon[count].Create(joint)) {
				map->mTrigonOrder[count] = count;
				++count;
			}
			++child;
		}
		map->mTrigonCount = count;
		for (s32 i = 0; i < map->mTrigonCount - 1; ++i) {
			for (s32 j = i + 1; j < map->mTrigonCount; ++j) {
				s32 a = map->mTrigonOrder[i];
				s32 b = map->mTrigonOrder[j];
				if (map->mTrigon[a].mMinY < map->mTrigon[b].mMinY) {
					map->mTrigonOrder[i] = b;
					map->mTrigonOrder[j] = a;
				}
			}
		}
		// retail keeps this empty loop
		for (s32 i = 0; i < map->mTrigonCount; ++i) { }
		if ((s32)GetEnvData()[4] == 1) {
			HSD_LObj* previous = NULL;
			for (s32 i = 0; i < 8; ++i) {
				if (map->GetLightDesc(i)) {
					map->mLights[i].Init(map->GetLightDesc(i)->mDesc, GetEnvData()[5] & (1 << (7 - i)));
					if (map->GetLightAnimNum(i) > 0) {
						map->mLightAnims[i] = 0;
						map->mLights[i].SetAnim(map->GetLightDesc(i)->mAnims[map->mLightAnims[i]], 0.0f);
					}
					++map->mLightNum;
					if (previous)
						previous->next = &map->mLights[i];
					previous = &map->mLights[i];
				}
			}
			if (previous)
				previous->next = NULL;
		}
		return TRUE;
	}
	return FALSE;
}

// XObjMap::vf98
/**
 * @note Address: 0x80093AC0
 * @note Size: 0x9C
 */
extern "C" void Reset__7XObjMapFv(XObjMap* map)
{
	fn_800928E8(map);
	map->mTrigonCount = 0;
	for (u32 i = 0; i < 16; ++i)
		map->mTrigonOrder[i] = 0;
	map->mLightNum = 0;
	for (u32 i = 0; i < 8; ++i)
		map->mLightAnims[i] = -1;
}

// XObjMap::vf90
/**
 * @note Address: 0x80093B5C
 * @note Size: 0x20
 */
extern "C" void Init__7XObjMapFUl(XObjMap* map, u32 id)
{
	fn_800929C4(map, id);
}
