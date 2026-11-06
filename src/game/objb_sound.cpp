#include "game/xobjsound.h"
#include "game/cgame.h"

// XObjSound, an object that plays sounds along with its anim

extern "C" AnimSfxEvent gSoundObjectAnimEvents[3];

#pragma section RX ".text.soundobjectactions"
/**
 * @note Address: 0x800DD32C
 * @note Size: 0x60
 */
__declspec(section ".text.soundobjectactions") XObjSound::~XObjSound()
{
}

extern "C" {

/**
 * @note Address: 0x800DD38C
 * @note Size: 0xB0
 */
__declspec(section ".text.soundobjectactions") void UpdateSfx__9XObjSoundFf(XObjSound* object, f32 frame)
{
	if ((object->mStatus.mFlags & 1) && (object->mStatus.mFlags & 0x10000)) {
		object->mSfx.Update(object->mAnimId, frame, GetSoundVolumeAtPosition(&gGame.mSound, &object->mMoveTranslate, 0));
	}
}

#pragma section RX ".text.soundobjectreset"
/**
 * @note Address: 0x800DD504
 * @note Size: 0x68
 */
__declspec(section ".text.soundobjectreset") void ResetObjectState__9XObjSoundFv(XObjSound* object)
{
	ResetObjectState__4XObjFv(object);
	object->_990 = 0;
	object->_994 = 0;
	switch (object->GetKind()) {
	case 0x1D2:
		object->mSfx.mEvents = gSoundObjectAnimEvents;
		break;
	}
}

/**
 * @note Address: 0x800DD56C
 * @note Size: 0x34
 */
__declspec(section ".text.soundobjectreset") void InitializeObject__9XObjSoundFUl(XObjSound* object, u32 id)
{
	InitializeObject__4XObjFUl(object, id);
	object->mSfx.mEvents = NULL;
}

} // extern "C"
