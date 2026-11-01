#include "game/hsd_jobj.h"
#include "game/ut.h"
#include "game/cparticle.h"

extern "C" {
extern void* __vt__4CVec[];
extern void* __vt__10CGenerator[];
extern void* __vt__5CFile[];
}

// todo: weak copy
/**
 * @note Address: 0x8001B32C
 * @note Size: 0x10
 */
extern "C" CVec* __ct__4CVecFv(CVec* vec)
{
	*(void***)((u8*)vec + 0xC) = __vt__4CVec; // _0C vtable
	return vec;
}

// todo: weak copy
/**
 * @note Address: 0x8001B33C
 * @note Size: 0x20
 */
extern "C" CGenerator* __ct__10CGeneratorFv(CGenerator* gen)
{
	*(void***)((u8*)gen + 0x8) = __vt__10CGenerator; // _08 vtable
	gen->mId                   = -1;
	gen->mGen                  = NULL;
	return gen;
}

// todo: weak copy
/**
 * @note Address: 0x8001B35C
 * @note Size: 0x30
 */
extern "C" CFile* __ct__5CFileFv(CFile* file)
{
	*(void***)((u8*)file + 0xE8) = __vt__5CFile; // _E8 vtable
	file->mArchive               = NULL;
	file->mEntrynum              = -1;
	file->mBuffer                = NULL;
	file->mResult                = 0;
	file->mState                 = 1;
	return file;
}
