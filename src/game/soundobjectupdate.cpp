#include "game/xobjsound.h"

extern "C" const float sSoundObjectDefaultFrame;

/**
 * @note Address: 0x800DD43C
 * @note Size: 0xC8
 */
extern "C" void UpdateObject__9XObjSoundFUl(XObjSound* object, u32 frames)
{
	UpdateObject__4XObjFUl(object, frames);
	HSD_AObj* aobj = GetAObj(object->mJObj);
	object->UpdateSfx(aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : sSoundObjectDefaultFrame);
}
