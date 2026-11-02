#include "game/cgame.h"

void operator delete(void* object);

extern "C" void* __vt__10CStopwatch[];

/**
 * @note Address: 0x80008608
 * @note Size: 0x48
 */
// todo: weak copy
extern "C" CStopwatch* __dt__10CStopwatchFv(CStopwatch* stopwatch, s32 flags)
{
	if (stopwatch != NULL) {
		*(void***)((u8*)stopwatch + 0x38) = __vt__10CStopwatch;
		if ((s16)flags > 0)
			::operator delete(stopwatch);
	}
	return stopwatch;
}
