#include "game/cobj.h"

void operator delete(void* object);

extern "C" void* __vt__11CObjArchive[];

/**
 * @note Address: 0x80008E10
 * @note Size: 0x60
 */
// todo: weak copy
extern "C" CObjArchive* __dt__11CObjArchiveFv(CObjArchive* archive, s32 flags)
{
	if (archive != NULL) {
		*(void***)((u8*)archive + 0x134) = __vt__11CObjArchive;
		archive->mFile.CFile::~CFile();
		if ((s16)flags > 0)
			::operator delete(archive);
	}
	return archive;
}
