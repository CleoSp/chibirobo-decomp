#include "game/cfile.h"

void operator delete(void* object);

extern "C" void* __vt__5CFile[];

/**
 * @note Address: 0x80008D38
 * @note Size: 0x48
 */
// todo: weak copy
extern "C" CFile* __dt__5CFileFv(CFile* file, s32 flags)
{
	if (file != NULL) {
		*(void***)((u8*)file + 0xE8) = __vt__5CFile;
		if ((s16)flags > 0)
			::operator delete(file);
	}
	return file;
}
