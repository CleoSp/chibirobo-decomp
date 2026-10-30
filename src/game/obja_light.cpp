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

	s32 GetLightMode() { return fn_8006F4B8()->mInfo->mLightMode; }

};

// ---- functions ----

extern "C" {

// NONMATCHING: the desc and anim count arrays share one induction pointer here, two in the dol
void fn_80092E38(XObjLight* obj, u32 frames)
{
	if (obj->GetLightMode() == 2) {
		for (int i = 0; i < obj->mLightCount; i++) {
			if (obj->mArchive.mDesc[i + 1] != NULL && obj->mArchive.mNum[i + 2] > 0) {
				obj->mLights[i].SetAnimRate(frames);
				obj->mLights[i].Anim();
			}
		}
	}
}

void fn_80092F00(XObjLight* obj)
{
	if (obj->GetLightMode() == 2) {
		for (int i = 0; i < obj->mLightCount; i++) {
			if (obj->mArchive.mDesc[i + 1] != NULL) {
				obj->mLights[i].Release();
			}
		}
	}
	fn_800911C0(obj);
}

// NONMATCHING: same induction pointer difference as fn_80092E38
BOOL fn_80092F98(XObjLight* obj, void* info)
{
	if (fn_80091C80(obj, info)) {
		if (obj->GetLightMode() == 2) {
			CLight* prev = NULL;
			for (int i = 0; i < 8; i++) {
				if (obj->mArchive.mDesc[i + 1] != NULL) {
					obj->mLights[i].Init((HSD_LightDesc*)obj->mArchive.mDesc[i + 1]->_00, fn_8006F4B8()->mInfo->mShadowMask & (1 << (7 - i)));
					if (obj->mArchive.mNum[i + 2] > 0) {
						obj->mAnims[i] = 0;
						obj->mLights[i].SetAnim((HSD_LightAnim*)obj->mArchive.mDesc[i + 1]->_04[obj->mAnims[i]], 0.0f);
					}
					obj->mLightCount++;
					if (prev != NULL) {
						prev->next = &obj->mLights[i];
					}
					prev = &obj->mLights[i];
				}
			}
			if (prev != NULL) {
				prev->next = NULL;
			}
		}
		obj->mFlags &= ~0x10000ULL;
		return TRUE;
	}
	return FALSE;
}

void fn_800930E4(XObjLight* obj)
{
	obj->mLightCount = 0;
	for (int i = 0; i < 8; i++) {
		obj->mAnims[i] = -1;
	}
}

void fn_80093114(XObjLight* obj, u32 id)
{
	fn_800929C4(obj, id);
}

} // extern "C"
