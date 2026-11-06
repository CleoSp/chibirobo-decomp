#include "types.h"

class CScrnColor;
void operator delete(void* object);
extern "C" void* __vt__10CScrnColor[];

// todo: weak copy
/**
 * @note Address: 0x80018E14
 * @note Size: 0x48
 */
extern "C" CScrnColor* __dt__10CScrnColorFv(CScrnColor* color, s32 flags)
{
	if (color != NULL) {
		*(void***)((u8*)color + 0x30) = __vt__10CScrnColor; // _30 vtable
		if ((s16)flags > 0) {
			::operator delete(color);
		}
	}
	return color;
}
