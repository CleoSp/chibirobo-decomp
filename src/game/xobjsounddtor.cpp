#include "game/xobjsound.h"

// todo: weak copy
/**
 * @note Address: 0x800DD32C
 * @note Size: 0x60
 */
extern "C" XObjSound* fn_800DD32C(XObjSound* object, s32 flags)
{
	if (object != NULL) {
		object->XObjSound::~XObjSound();
		if ((s16)flags > 0)
			::operator delete(object);
	}
	return object;
}
