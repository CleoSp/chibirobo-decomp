#include "game/cgs.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__9CGsScreen[];
extern void* __vt__13CGsRenderMode[];
}

// todo: weak copy
/**
 * @note Address: 0x8001874C
 * @note Size: 0x48
 */
extern "C" CGsScreen* fn_8001874C(CGsScreen* screen, s32 flags)
{
	if (screen != NULL) {
		*(void***)((u8*)screen + sizeof(CGsScreen) - 4) = __vt__9CGsScreen; // _40 vtable, _44 in europe
		if ((s16)flags > 0) {
			::operator delete(screen);
		}
	}
	return screen;
}

// todo: weak copy
/**
 * @note Address: 0x80018794
 * @note Size: 0x48
 */
extern "C" CGsRenderMode* fn_80018794(CGsRenderMode* mode, s32 flags)
{
	if (mode != NULL) {
		*(void***)((u8*)mode + 0x30) = __vt__13CGsRenderMode; // _30 vtable
		if ((s16)flags > 0) {
			::operator delete(mode);
		}
	}
	return mode;
}
