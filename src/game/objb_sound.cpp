#include "game/xobjsound.h"
#include "game/cgame.h"

// XObjSound, an object that plays sounds along with its anim

extern "C" AnimSfxEvent lbl_80228FF8[3];

extern "C" {

// the weak destructor
/**
 * @note Address: 0x800DD32C
 * @note Size: 0x60
 */
// NONMATCHING: control flow
XObjSound* __dt__9XObjSoundFv(XObjSound* object, s32 flags)
{
	if (object != NULL) {
		object->XObjSound::~XObjSound();
		if ((s16)flags > 0) {
			::operator delete(object);
		}
	}
	return object;
}

/**
 * @note Address: 0x800DD38C
 * @note Size: 0xB0
 */
void UpdateSfx__9XObjSoundFf(XObjSound* object, f32 frame)
{
	if ((object->mStatus.mFlags & 1) && (object->mStatus.mFlags & 0x10000)) {
		object->mSfx.Update(object->mAnimId, frame, fn_8001DF00(&lbl_8025C400.mSound, &object->mMoveTranslate, 0));
	}
}

/**
 * @note Address: 0x800DD43C
 * @note Size: 0xC8
 */
void vfA4__9XObjSoundFUl(XObjSound* object, u32 frames)
{
	vfA4__4XObjFUl(object, frames);
	HSD_AObj* aobj = GetAObj(object->mJObj);
	object->UpdateSfx(aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f);
}

/**
 * @note Address: 0x800DD504
 * @note Size: 0x68
 */
void vf98__9XObjSoundFv(XObjSound* object)
{
	vf98__4XObjFv(object);
	object->_990 = 0;
	object->_994 = 0;
	switch (object->GetKind()) {
	case 0x1D2:
		object->mSfx.mEvents = lbl_80228FF8;
		break;
	}
}

/**
 * @note Address: 0x800DD56C
 * @note Size: 0x34
 */
void vf90__9XObjSoundFUl(XObjSound* object, u32 id)
{
	vf90__4XObjFUl(object, id);
	object->mSfx.mEvents = NULL;
}

} // extern "C"
