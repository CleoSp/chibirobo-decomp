#include "game/ccameraaction.h"

void operator delete(void* object);

extern "C" void* __vt__21CCameraActionLeticker[];

/**
 * @note Address: 0x800E6818
 * @note Size: 0xA8
 */
// todo: weak copy
extern "C" CCameraActionLeticker* fn_800E6818(CCameraActionLeticker* camera, s32 flags)
{
	if (camera != NULL) {
		*(void***)((u8*)camera + 0x3C) = __vt__21CCameraActionLeticker;
		camera->CCameraActionLadr::~CCameraActionLadr();
		if ((s16)flags > 0)
			::operator delete(camera);
	}
	return camera;
}
