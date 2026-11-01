#include "game/xobjsound.h"
#include "game/cgame.h"
#include "game/hsd_cobj.h"
#include "game/csound.h"

extern "C" AnimSfxEvent lbl_80228FF8[3];

// XObjSound::UpdateSfx
/**
 * @note Address: 0x800DD38C
 * @note Size: 0xB0
 */
extern "C" void fn_800DD38C(XObjSound* object, f32 frame)
{
	if (object->mStatus.mFlags & 1ULL) {
		if (object->mStatus.mFlags & 0x10000ULL) {
			u8 volume = fn_8001DF00((CSound*)&lbl_8025C400.mSound, &object->mMoveTranslate, 0);
			object->mSfx.Update(object->mAnimId, frame, volume);
		}
	}
}

// XObjSound::vfA4
/**
 * @note Address: 0x800DD43C
 * @note Size: 0xC8
 */
extern "C" void fn_800DD43C(XObjSound* object, u32 frames)
{
	fn_80090ED4(object, frames);
	HSD_AObj* animation = GetAObj(object->mJObj);
	f32 frame           = animation != NULL ? HSD_AObjGetCurrentFrame(animation) : 0.0f;
	object->UpdateSfx(frame);
}

// XObjSound::vf98
/**
 * @note Address: 0x800DD504
 * @note Size: 0x68
 */
extern "C" void fn_800DD504(XObjSound* object)
{
	fn_800928E8(object);
	object->_990 = 0;
	object->_994 = 0;
	s32 kind     = object->GetKind();
	switch (kind) {
	case 0x1D2:
		object->mSfx.mEvents = lbl_80228FF8;
		break;
	}
}

// XObjSound::vf90
/**
 * @note Address: 0x800DD56C
 * @note Size: 0x34
 */
extern "C" void fn_800DD56C(XObjSound* object, u32 id)
{
	fn_800929C4(object, id);
	object->mSfx.mEvents = NULL;
}
