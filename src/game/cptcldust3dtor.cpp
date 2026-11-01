#include "game/hsd_jobj.h"
#include "game/cptcl.h"

extern "C" void* __vt__10CPtclDust3[];

/**
 * @note Address: 0x80024558
 * @note Size: 0x98
 */
// todo: weak copy
extern "C" CPtclDust3* fn_80024558(CPtclDust3* ptcl, s32 flags)
{
	if (ptcl != NULL) {
		*(void***)((u8*)ptcl + 0x4C) = __vt__10CPtclDust3;
		ptcl->CPtclOne::~CPtclOne();
		if ((s16)flags > 0) {
			::operator delete(ptcl);
		}
	}
	return ptcl;
}
