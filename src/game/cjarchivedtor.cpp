#include "game/cobj.h"

void operator delete(void* object);
extern "C" void* __vt__9CJArchive[];

// todo: weak copy
/**
 * @note Address: 0x8001B108
 * @note Size: 0x48
 */
extern "C" CJArchive* __dt__9CJArchiveFv(CJArchive* archive, s32 flags)
{
	if (archive != NULL) {
		*(void***)((u8*)archive + sizeof(CJArchive) - 4) = __vt__9CJArchive; // _7C vtable, _74 in japan
		if ((s16)flags > 0) {
			::operator delete(archive);
		}
	}
	return archive;
}
