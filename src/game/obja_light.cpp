#include "game/obja.h"
#include "game/clight.h"
#include "game/gmath.h"
extern "C" {
#include <dolphin/mtx/vec.h>
}
#include "game/cvec.h"

// XObjLight, an XObj that lights the stage with up to eight lights from its archive

// __vt__9XObjLight
class XObjLight : public XObj {
public:
	s32 mLightCount;   // _980
	CLight mLights[8]; // _984
	s32 mAnims[8];     // _2444, the anim each light plays, -1 for none

	virtual void Release();          // _54
	virtual void Init(u32 id);       // _90
	virtual void Reset();            // _98
	virtual BOOL vf9C(void* info);   // _9C
	virtual void Update(u32 frames); // _A4

	s32 GetLightMode() { return fn_8006F4B8()->mInfo->mLightMode; }
};

// ---- functions ----

// NONMATCHING: the desc and anim count arrays share one induction pointer here, two in the dol
void XObjLight::Update(u32 frames)
{
	if (GetLightMode() == 2) {
		for (int i = 0; i < mLightCount; i++) {
			if (mArchive.mDesc[i + 1] != NULL && mArchive.mNum[i + 2] > 0) {
				mLights[i].SetAnimRate(frames);
				mLights[i].Anim();
			}
		}
	}
}

void XObjLight::Release()
{
	if (GetLightMode() == 2) {
		for (int i = 0; i < mLightCount; i++) {
			if (mArchive.mDesc[i + 1] != NULL) {
				mLights[i].Release();
			}
		}
	}
	fn_800911C0(this);
}

// NONMATCHING: same induction pointer difference as Update
BOOL XObjLight::vf9C(void* info)
{
	if (fn_80091C80(this, info)) {
		if (GetLightMode() == 2) {
			CLight* prev = NULL;
			for (int i = 0; i < 8; i++) {
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
		mFlags &= ~0x10000ULL;
		return TRUE;
	}
	return FALSE;
}

void XObjLight::Reset()
{
	mLightCount = 0;
	for (int i = 0; i < 8; i++) {
		mAnims[i] = -1;
	}
}

void XObjLight::Init(u32 id)
{
	fn_800929C4(this, id);
}
