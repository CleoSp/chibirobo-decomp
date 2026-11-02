#include "game/hsd_jobj.h"
#include "game/cshadow.h"

void operator delete(void* object);

extern "C" void* __vt__7CShadow[];

// todo: weak copy
/**
 * @note Address: 0x80018688
 * @note Size: 0xC4
 */
extern "C" CShadow* __dt__7CShadowFv(CShadow* shadow, s32 flags)
{
	if (shadow != NULL) {
		*(void***)((u8*)shadow + 0x1F4) = __vt__7CShadow; // _1F4 vtable
		shadow->mCamera.CCamera::~CCamera();
		if ((s16)flags > 0) {
			::operator delete(shadow);
		}
	}
	return shadow;
}
