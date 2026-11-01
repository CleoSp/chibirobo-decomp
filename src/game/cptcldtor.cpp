extern "C" {
#include <dolphin/mtx/vec.h>
}
#include "game/cptcl.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__5CPtcl[];

/**
 * @note Address: 0x800BD2D8
 * @note Size: 0x48
 */
// CPtcl::~CPtcl
// todo: weak copy
CPtcl* fn_800BD2D8(CPtcl* ptcl, s32 flags)
{
	if (ptcl != NULL) {
		*(void***)((u8*)ptcl + sizeof(CPtcl) - 4) = __vt__5CPtcl;
		if ((s16)flags > 0)
			::operator delete(ptcl);
	}
	return ptcl;
}
}
