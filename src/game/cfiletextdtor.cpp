#include "game/cfile.h"

extern "C" void* __vt__9CFileText[];

/**
 * @note Address: 0x8001CEA0
 * @note Size: 0x5C
 */
// todo: weak copy
extern "C" CFileText* __dt__9CFileTextFv(CFileText* file, s32 flags)
{
	if (file != NULL) {
		*(void***)((u8*)file + 0xE8) = __vt__9CFileText;
		file->CFile::~CFile();
		if ((s16)flags > 0) {
			::operator delete(file);
		}
	}
	return file;
}
