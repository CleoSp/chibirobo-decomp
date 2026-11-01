#include "game/hsd_jobj.h"
#include "game/ut.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__7CStatus[];
}

// todo: weak copy
/**
 * @note Address: 0x80018CE0
 * @note Size: 0x48
 */
extern "C" CStatus* __dt__7CStatusFv(CStatus* status, s32 flags)
{
	if (status != NULL) {
		*(void***)((u8*)status + 0x4) = __vt__7CStatus; // _04 vtable
		if ((s16)flags > 0) {
			::operator delete(status);
		}
	}
	return status;
}
