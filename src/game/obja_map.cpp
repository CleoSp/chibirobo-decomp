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

	virtual void Release();          // _54
	virtual void Init(u32 id);       // _90
	virtual void Reset();            // _98
	virtual BOOL vf9C(void* info);   // _9C
	virtual void Update(u32 frames); // _A4

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

extern "C" s32 fn_80093210(XObjMap* map, s32 trigon, s32 tri)
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

// NONMATCHING: induction pointer difference, see XObjLight::Update
void XObjMap::Update(u32 frames)
{
	fn_80090ED4(this, frames);
	if (GetLightMode() == 1) {
		for (int i = 0; i < mLightCount; i++) {
			if (mArchive.mDesc[i + 1] != NULL && mArchive.mNum[i + 2] > 0) {
				mLights[i].SetAnimRate(frames);
				mLights[i].Anim();
			}
		}
	}
}

void XObjMap::Release()
{
	for (int i = 0; i < mTrigonCount; i++) {
		mTrigons[i].Release();
	}
	if (GetLightMode() == 1) {
		for (int i = 0; i < mLightCount; i++) {
			if (mArchive.mDesc[i + 1] != NULL) {
				mLights[i].Release();
			}
		}
	}
	fn_800911C0(this);
}

// NONMATCHING: the dol copies a zero path template before storing the index, this drops it
BOOL XObjMap::vf9C(void* info)
{
	if (fn_80091C80(this, info)) {
		int i;
		int j;
		int n = 0;
		for (i = 0;; i++) {
			s32 path[]     = { 0 };
			path[0]        = i;
			HSD_JObj* jobj = GetJointByPath(mJObj, path, 1);
			if (jobj == NULL) {
				break;
			}
			if (mTrigons[n].Create(jobj) != NULL) {
				mOrder[n] = n;
				n++;
			}
		}
		mTrigonCount = n;
		for (i = 0; i < mTrigonCount - 1; i++) {
			for (j = i + 1; j < mTrigonCount; j++) {
				s32 a = mOrder[i];
				s32 b = mOrder[j];
				if (mTrigons[a].mMinY < mTrigons[b].mMinY) {
					mOrder[i] = b;
					mOrder[j] = a;
				}
			}
		}
		for (i = 0; i < mTrigonCount; i++) { }
		if (GetLightMode() == 1) {
			CLight* prev = NULL;
			for (i = 0; i < 8; i++) {
				if (mArchive.mDesc[i + 1] != NULL) {
					mLights[i].Init((HSD_LightDesc*)mArchive.mDesc[i + 1]->_00, fn_8006F4B8()->mInfo->mShadowMask & (1 << (7 - i)));
					if (mArchive.mNum[i + 2] > 0) {
						mAnims[i] = 0;
						mLights[i].SetAnim((HSD_LightAnim*)mArchive.mDesc[i + 1]->_04[mAnims[i]], 0.0f);
					}
					mLightCount++;
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

void XObjMap::Reset()
{
	fn_800928E8(this);
	mTrigonCount = 0;
	for (int i = 0; i < 16; i++) {
		mOrder[i] = 0;
	}
	mLightCount = 0;
	for (int i = 0; i < 8; i++) {
		mAnims[i] = -1;
	}
}

void XObjMap::Init(u32 id)
{
	fn_800929C4(this, id);
}
