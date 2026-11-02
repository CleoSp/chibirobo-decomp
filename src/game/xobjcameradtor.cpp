#include "game/xobj.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__10XObjCamera[];
}

// todo: weak copy
/**
 * @note Address: 0x80092AA4
 * @note Size: 0xDC
 */
extern "C" XObjCamera* __dt__10XObjCameraFv(XObjCamera* object, s32 flags)
{
	if (object != NULL) {
		*(void***)((u8*)object + 0x34) = __vt__10XObjCamera;
		object->mCamera.CCamera::~CCamera();
		__dt__4XObjFv(object, 0);
		if ((s16)flags > 0)
			::operator delete(object);
	}
	return object;
}
