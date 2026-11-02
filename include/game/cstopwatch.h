#ifndef _GAME_CSTOPWATCH_H
#define _GAME_CSTOPWATCH_H

#include "types.h"

// one of CBase's frame timers, class name from RTTI
class CStopwatch {
public:
	u8 mWatch[0x30]; // _00, the sdk's OSStopwatch
	s64 mElapsed;    // _30, OSCheckStopwatch before it was stopped
	// _38 vtable

	virtual ~CStopwatch() { } // _08
};
STATIC_ASSERT(sizeof(CStopwatch) == 0x40);

#endif
