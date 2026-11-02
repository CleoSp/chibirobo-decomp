#include "game/hsd_jobj.h"
#include "game/ut.h"

void operator delete(void* object);
extern "C" void* __vt__6CColor[];

// todo: weak copy
/**
 * @note Address: 0x80018640
 * @note Size: 0x48
 */
extern "C" CColor* __dt__6CColorFv(CColor* color, s32 flags)
{
	if (color != NULL) {
		*(void***)((u8*)color + 0x4) = __vt__6CColor; // _04 vtable
		if ((s16)flags > 0) {
			::operator delete(color);
		}
	}
	return color;
}
