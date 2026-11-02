#include "game/cfile.h"
#include "game/xsound.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__6CSound[];
extern void* __vt__6XSound[];
}

// todo: weak copy
/**
 * @note Address: 0x800191A4
 * @note Size: 0x74
 */
extern "C" XSound* __dt__6XSoundFv(XSound* sound, s32 flags)
{
	if (sound != NULL) {
		*(void***)sound = __vt__6XSound;
		sound->mFile.CFile::~CFile();
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
