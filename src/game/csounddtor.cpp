#include "game/csound.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__6CSound[];
}

// todo: weak copy
/**
 * @note Address: 0x8001915C
 * @note Size: 0x48
 */
extern "C" CSound* __dt__6CSoundFv(CSound* sound, s32 flags)
{
	if (sound != NULL) {
		*(void***)sound = __vt__6CSound;
		if ((s16)flags > 0) {
			::operator delete(sound);
		}
	}
	return sound;
}
