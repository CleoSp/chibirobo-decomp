#include "game/cpad.h"

extern "C" void* __vt__4CPad[];

/**
 * @note Address: 0x8001C854
 * @note Size: 0x48
 */
// todo: weak copy
extern "C" CPad* __dt__4CPadFv(CPad* pad, s32 flags)
{
	if (pad != NULL) {
		*(void***)((u8*)pad + 0x4) = __vt__4CPad;
		if ((s16)flags > 0) {
			::operator delete(pad);
		}
	}
	return pad;
}
