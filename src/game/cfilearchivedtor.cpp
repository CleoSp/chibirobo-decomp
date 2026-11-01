#include "game/cfile.h"

extern "C" void* __vt__12CFileArchive[];

/**
 * @note Address: 0x8001B844
 * @note Size: 0x5C
 */
// todo: weak copy
extern "C" CFileArchive* __dt__12CFileArchiveFv(CFileArchive* file, s32 flags)
{
	if (file != NULL) {
		*(void***)((u8*)file + 0xE8) = __vt__12CFileArchive;
		file->CFile::~CFile();
		if ((s16)flags > 0) {
			::operator delete(file);
		}
	}
	return file;
}
