#include "game/cstatus64.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__9CStatus64[];
}

// todo: weak copy
/**
 * @note Address: 0x80018B88
 * @note Size: 0x48
 */
extern "C" CStatus64* __dt__9CStatus64Fv(CStatus64* status, s32 flags)
{
	if (status != NULL) {
		*(void***)((u8*)status + 0x8) = __vt__9CStatus64; // _08 vtable
		if ((s16)flags > 0) {
			::operator delete(status);
		}
	}
	return status;
}
