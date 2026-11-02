#include "game/hsd_jobj.h"
#include "game/cscrn.h"

void operator delete(void* object);

extern "C" void* __vt__15CScrnWipeCircle[];

// todo: weak copy
/**
 * @note Address: 0x80018BD0
 * @note Size: 0x6C
 */
extern "C" CScrnWipeCircle* __dt__15CScrnWipeCircleFv(CScrnWipeCircle* wipe, s32 flags)
{
	if (wipe != NULL) {
		*(void***)((u8*)wipe + 0x30) = __vt__15CScrnWipeCircle; // _30 vtable
		wipe->CScrnWipe::~CScrnWipe();
		if ((s16)flags > 0) {
			::operator delete(wipe);
		}
	}
	return wipe;
}
