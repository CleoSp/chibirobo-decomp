#include "game/cmovie.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__6CMovie[];
}

// todo: weak copy
/**
 * @note Address: 0x800192C8
 * @note Size: 0x48
 */
extern "C" CMovie* __dt__6CMovieFv(CMovie* movie, s32 flags)
{
	if (movie != NULL) {
		*(void***)((u8*)movie + 0xB4) = __vt__6CMovie; // _B4 vtable
		if ((s16)flags > 0) {
			::operator delete(movie);
		}
	}
	return movie;
}
