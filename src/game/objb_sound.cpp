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
XObjSound* fn_800DD32C(XObjSound* object, s32 flags)
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
void fn_800DD38C(XObjSound* object, f32 frame)
{
	if ((object->mStatus.mFlags & 1) && (object->mStatus.mFlags & 0x10000)) {
		object->mSfx.Update(object->mAnimId, frame, fn_8001DF00(&lbl_8025C400.mSound, &object->mMoveTranslate, 0));
	}
}

/**
 * @note Address: 0x800DD43C
 * @note Size: 0xC8
 */
void fn_800DD43C(XObjSound* object, u32 frames)
{
	fn_80090ED4(object, frames);
	HSD_AObj* aobj = GetAObj(object->mJObj);
	object->UpdateSfx(aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f);
}

/**
 * @note Address: 0x800DD504
 * @note Size: 0x68
 */
void fn_800DD504(XObjSound* object)
{
	fn_800928E8(object);
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
void fn_800DD56C(XObjSound* object, u32 id)
{
	fn_800929C4(object, id);
	object->mSfx.mEvents = NULL;
}

} // extern "C"
