#include "game/cobj.h"

extern "C" {
extern void* __vt__11CObjArchive[];
extern void* __vt__5CFile[];
}

// todo: weak copy
/**
 * @note Address: 0x8001B2EC
 * @note Size: 0x40
 */
// NONMATCHING: regalloc
extern "C" CObjArchive* __ct__11CObjArchiveFv(CObjArchive* archive)
{
	*(void***)((u8*)archive + 0x134)        = __vt__11CObjArchive; // _134 vtable
	*(void***)((u8*)&archive->mFile + 0xE8) = __vt__5CFile;
	archive->mFile.mArchive                 = NULL;
	archive->mFile.mEntrynum                = -1;
	archive->mFile.mBuffer                  = NULL;
	archive->mFile.mResult                  = 0;
	archive->mFile.mState                   = 1;
	archive->mRefCount                      = 0;
	return archive;
}
