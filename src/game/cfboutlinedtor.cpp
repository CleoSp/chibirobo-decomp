#include "types.h"

// fbeffect.h has the layouts, it can't come in with the hsd gx headers
class CFbOutline;
class CFbBlur;

void operator delete(void* object);
extern "C" {
extern void* __vt__10CFbOutline[];
extern void* __vt__7CFbBlur[];
}

// todo: weak copy
/**
 * @note Address: 0x80018C3C
 * @note Size: 0x48
 */
extern "C" CFbOutline* fn_80018C3C(CFbOutline* outline, s32 flags)
{
	if (outline != NULL) {
		*(void***)((u8*)outline + 0x60) = __vt__10CFbOutline; // _60 vtable
		if ((s16)flags > 0) {
			::operator delete(outline);
		}
	}
	return outline;
}

// todo: weak copy
/**
 * @note Address: 0x80018C84
 * @note Size: 0x48
 */
extern "C" CFbBlur* fn_80018C84(CFbBlur* blur, s32 flags)
{
	if (blur != NULL) {
		*(void***)((u8*)blur + 0x40) = __vt__7CFbBlur; // _40 vtable
		if ((s16)flags > 0) {
			::operator delete(blur);
		}
	}
	return blur;
}
