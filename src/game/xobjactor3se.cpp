#include "game/xobjactor.h"
#include "game/cgame.h"
#include "game/hsd_cobj.h"

extern "C" {
#include <sysdolphin/axdriver.h>
}

/**
 * @note Address: 0x80108540
 * @note Size: 0x154
 */
// anim sounds, only the fourth one plays them
extern "C" void fn_80108540(XObjActor3* actor)
{
	if (actor->mStatus.mFlags & 1ULL) {
		if (actor->mStatus.mFlags & 0x10000ULL) {
			if (actor->_258 == 3) {
				HSD_JObj* jobj = actor->mJObj;
				s32 id         = actor->mAnimId;
				HSD_AObj* aobj = GetAObj(jobj);
				f32 frame      = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
				actor->mAnimSfx.Update(id, frame, fn_8001DF00((CSound*)&lbl_8025C400.mSound, &actor->mMoveTranslate, 0));
			}
		}
	}
}

/**
 * @note Address: 0x80108694
 * @note Size: 0x94
 */
extern "C" void fn_80108694(XObjActor3* actor, s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate,
                            f32 blend, f32 blendRate)
{
	fn_800902A4(actor, id, force, jointAnim, matAnim, shapeAnim, frame, rate, blend, blendRate);
	if (actor->GetKind() == 0x31B) {
		if (actor->mAnimId == 7) {
			actor->mSe = fn_8001E904((CSound*)&lbl_8025C400.mSound, 0xB0000, 255); // pipe_nezumi_mawasu
		} else {
			if (HSD_AudioSFXCheck(actor->mSe)) {
				HSD_AudioSFXKeyOff(actor->mSe);
			}
		}
	}
}

/**
 * @note Address: 0x80108728
 * @note Size: 0x6C
 */
extern "C" void fn_80108728(XObjActor3* actor, const Vec* pos, f32 strength)
{
	s16 flags = actor->GetObjFlags();
	if (flags == 1) {
		fn_8008A6C0(actor, strength);
	} else {
		if (!actor->_B8C.Check(1)) {
			actor->_B8C.mFlags |= 1;
		}
	}
}
