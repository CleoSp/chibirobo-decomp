#ifndef _GAME_ARCACHE_H
#define _GAME_ARCACHE_H

#include "game/cfile.h"

// aram file caching, class names from RTTI, layouts are partial. one of each is a global
// in CGame's file

class ARCacheInfo {
public:
	u32 _00;              // _00
	u32 _04;              // _04
	u8 _08[0x4008 - 0x8]; // _08
	// _4008 vtable

	virtual ~ARCacheInfo(); // _08
};
STATIC_ASSERT(sizeof(ARCacheInfo) == 0x400C);

class ARPreCache {
public:
	u32 _00;                // _00
	CFile mFile0;           // _04
	CFile mFile1;           // _F0
	u8 _1DC[0x1E4 - 0x1DC]; // _1DC
	// _1E4 vtable

	virtual ~ARPreCache(); // _08
};
STATIC_ASSERT(sizeof(ARPreCache) == 0x1E8);

#endif
