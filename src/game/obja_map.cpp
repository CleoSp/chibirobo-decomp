#include "game/xobjmap.h"
#include "game/clight.h"
#include "game/ctrigon.h"

// XObjMap, the room model. its children are the floor collision, one CTrigon each, and it
// can carry the room's lights

// who owns the stage lights, 1 the map and 2 the light objects
static inline s32 GetLightMode()
{
	return GetEnvData()[4];
}

/**
 * @note Address: 0x8009369C
 * @note Size: 0xCC
 */
#pragma section RX ".text.maplightupdate"
__declspec(section ".text.maplightupdate") void XObjMap::UpdateObject(u32 frames)
{
	UpdateObject__4XObjFUl(this, frames);
	if (GetLightMode() == 1) {
		for (int i = 0; i < mLightNum; i++) {
			if (GetLightDesc(i) != NULL && mArchive.mNum[i + 2] > 0) {
				mLights[i].SetAnimRate(frames);
				mLights[i].Anim();
			}
		}
	}
}

/**
 * @note Address: 0x80093768
 * @note Size: 0xC0
 */
__declspec(section ".text.maplightupdate") void XObjMap::Release()
{
	for (int i = 0; i < mTrigonCount; i++) {
		mTrigon[i].Release();
	}
	if (GetLightMode() == 1) {
		for (int i = 0; i < mLightNum; i++) {
			if (GetLightDesc(i) != NULL) {
				mLights[i].Release();
			}
		}
	}
	Release__4XObjFv(this);
}

// a floor for every child of the root joint that has triangles, sorted from the highest down
/**
 * @note Address: 0x80093828
 * @note Size: 0x298
 */
BOOL XObjMap::LoadFromPlacement(const CoordInfo& info)
{
	// NONMATCHING: the dol copies a zero path template before storing the index
	if (LoadFromPlacement__4XObjFRC9CoordInfo(this, &info)) {
		int i = 0;
		int j;
		int n = 0;
		for (;; i++) {
			s32 path[]     = { 0 };
			path[0]        = i;
			HSD_JObj* jobj = GetJointByPath(mJObj, path, 1);
			if (jobj == NULL) {
				break;
			}
			if (mTrigon[n].Create(jobj) != NULL) {
				mTrigonOrder[n] = n;
				n++;
			}
		}
		mTrigonCount = n;
		for (i = 0; i < mTrigonCount - 1; i++) {
			for (j = i + 1; j < mTrigonCount; j++) {
				s32 a = mTrigonOrder[i];
				s32 b = mTrigonOrder[j];
				if (mTrigon[a].mMinY < mTrigon[b].mMinY) {
					mTrigonOrder[i] = b;
					mTrigonOrder[j] = a;
				}
			}
		}
		for (i = 0; i < mTrigonCount; i++) { }
		if (GetLightMode() == 1) {
			i            = 0;
			CLight* prev = NULL;
			for (; i < 8; i++) {
				if (GetLightDesc(i) != NULL) {
					mLights[i].Init(GetLightDesc(i)->mDesc, GetEnvData()[5] & (1 << (7 - i)));
					if (GetLightAnimNum(i) > 0) {
						mLightAnims[i] = 0;
						mLights[i].SetAnim(GetLightDesc(i)->mAnims[mLightAnims[i]], 0.0f);
					}
					mLightNum++;
					if (prev != NULL) {
						prev->next = &mLights[i];
					}
					prev = &mLights[i];
				}
			}
			if (prev != NULL) {
				prev->next = NULL;
			}
		}
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x80093AC0
 * @note Size: 0x9C
 */
#pragma section RX ".text.mapreset"
__declspec(section ".text.mapreset") void XObjMap::ResetObjectState()
{
	ResetObjectState__4XObjFv(this);
	mTrigonCount = 0;
	for (int i = 0; i < 16; i++) {
		mTrigonOrder[i] = 0;
	}
	mLightNum = 0;
	for (int i = 0; i < 8; i++) {
		mLightAnims[i] = -1;
	}
}

/**
 * @note Address: 0x80093B5C
 * @note Size: 0x20
 */
__declspec(section ".text.mapreset") void XObjMap::InitializeObject(u32 id)
{
	InitializeObject__4XObjFUl(this, id);
}
