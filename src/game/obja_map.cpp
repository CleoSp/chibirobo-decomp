#include "game/obja.h"
#include "game/clight.h"
#include "game/ctrigon.h"
#include "game/gmath.h"
extern "C" {
#include <dolphin/mtx/vec.h>
}
#include "game/cvec.h"

// XObjMap, the room model. its children are the floor collision, one CTrigon each, and it
// can carry the room's lights

extern "C" void fn_80090ED4(XObj* obj, u32 frames); // XObj::Update
extern "C" void fn_800928E8(XObj* obj);             // XObj::Reset

// __vt__7XObjMap
class XObjMap : public XObj {
public:
	s32 mTrigonCount;     // _980
	s32 mOrder[16];       // _984, mTrigons by mMinY, highest first
	CTrigon mTrigons[16]; // _9C4
	s32 mLightCount;      // _B84
	CLight mLights[8];    // _B88
	s32 mAnims[8];        // _2648, the anim each light plays, -1 for none

	s32 GetLightMode() { return fn_8006F4B8()->mInfo->mLightMode; }
};

static inline u32 MakeRGB(s32 r, s32 g, s32 b)
{
	s32 cr = r;
	if (cr >= 256) {
		cr = 255;
	}
	s32 cg = g;
	if (cg >= 256) {
		cg = 255;
	}
	s32 cb = b;
	if (cb >= 256) {
		cb = 255;
	}
	return (cr << 16) | (cg << 8) | cb;
}

// ---- functions ----

extern "C" {

s32 fn_80093210(XObjMap* map, s32 trigon, s32 tri)
{
	static u32 sColors[13] = {
		MakeRGB(0, 0, 0),       MakeRGB(256, 256, 128), MakeRGB(256, 0, 0), MakeRGB(0, 0, 0), MakeRGB(256, 256, 0),
		MakeRGB(256, 256, 256), MakeRGB(0, 0, 0),       MakeRGB(0, 0, 0),   MakeRGB(0, 0, 0), MakeRGB(0, 256, 256),
		MakeRGB(256, 0, 256),   MakeRGB(256, 0, 128),   MakeRGB(0, 0, 0),
	};
	u32 color = map->mTrigons[trigon].mTri[tri].mColor;
	for (s32 i = 1; i < 13; i++) {
		if (color == sColors[i]) {
			return i;
		}
	}
	return 0;
}

// NONMATCHING: induction pointer difference, see fn_80092E38
void fn_8009369C(XObjMap* map, u32 frames)
{
	fn_80090ED4(map, frames);
	if (map->GetLightMode() == 1) {
		for (int i = 0; i < map->mLightCount; i++) {
			if (map->mArchive.mDesc[i + 1] != NULL && map->mArchive.mNum[i + 2] > 0) {
				map->mLights[i].SetAnimRate(frames);
				map->mLights[i].Anim();
			}
		}
	}
}

void fn_80093768(XObjMap* map)
{
	for (int i = 0; i < map->mTrigonCount; i++) {
		map->mTrigons[i].Release();
	}
	if (map->GetLightMode() == 1) {
		for (int i = 0; i < map->mLightCount; i++) {
			if (map->mArchive.mDesc[i + 1] != NULL) {
				map->mLights[i].Release();
			}
		}
	}
	fn_800911C0(map);
}

// NONMATCHING: the dol copies a zero path template before storing the index, this drops it
BOOL fn_80093828(XObjMap* map, void* info)
{
	if (fn_80091C80(map, info)) {
		int i;
		int j;
		int n = 0;
		for (i = 0;; i++) {
			s32 path[]     = { 0 };
			path[0]        = i;
			HSD_JObj* jobj = GetJointByPath(map->mJObj, path, 1);
			if (jobj == NULL) {
				break;
			}
			if (map->mTrigons[n].Create(jobj) != NULL) {
				map->mOrder[n] = n;
				n++;
			}
		}
		map->mTrigonCount = n;
		for (i = 0; i < map->mTrigonCount - 1; i++) {
			for (j = i + 1; j < map->mTrigonCount; j++) {
				s32 a = map->mOrder[i];
				s32 b = map->mOrder[j];
				if (map->mTrigons[a].mMinY < map->mTrigons[b].mMinY) {
					map->mOrder[i] = b;
					map->mOrder[j] = a;
				}
			}
		}
		for (i = 0; i < map->mTrigonCount; i++) { }
		if (map->GetLightMode() == 1) {
			CLight* prev = NULL;
			for (i = 0; i < 8; i++) {
				if (map->mArchive.mDesc[i + 1] != NULL) {
					map->mLights[i].Init((HSD_LightDesc*)map->mArchive.mDesc[i + 1]->_00,
					                     fn_8006F4B8()->mInfo->mShadowMask & (1 << (7 - i)));
					if (map->mArchive.mNum[i + 2] > 0) {
						map->mAnims[i] = 0;
						map->mLights[i].SetAnim((HSD_LightAnim*)map->mArchive.mDesc[i + 1]->_04[map->mAnims[i]], 0.0f);
					}
					map->mLightCount++;
					if (prev != NULL) {
						prev->next = &map->mLights[i];
					}
					prev = &map->mLights[i];
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

void fn_80093AC0(XObjMap* map)
{
	fn_800928E8(map);
	map->mTrigonCount = 0;
	for (int i = 0; i < 16; i++) {
		map->mOrder[i] = 0;
	}
	map->mLightCount = 0;
	for (int i = 0; i < 8; i++) {
		map->mAnims[i] = -1;
	}
}

void fn_80093B5C(XObjMap* map, u32 id)
{
	fn_800929C4(map, id);
}

} // extern "C"
