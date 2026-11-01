#include "game/cfile.h"
#include "game/xsound.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__6CSound[];
extern void* __vt__6XSound[];
}

// todo: cleanup, the CFile at _08 goes on XSound once CSound moves to csound.h
// todo: weak copy
/**
 * @note Address: 0x800191A4
 * @note Size: 0x74
 */
extern "C" XSound* fn_800191A4(XSound* sound, s32 flags)
{
	if (sound != NULL) {
		*(void***)sound = __vt__6XSound;
		((CFile*)&sound->_00[0x8])->CFile::~CFile();
		// the inlined CSound dtor, it checks again
		if (sound != NULL) {
			*(void***)sound = __vt__6CSound;
		}
		if ((s16)flags > 0) {
			::operator delete(sound);
		}
	}
	return sound;
}
