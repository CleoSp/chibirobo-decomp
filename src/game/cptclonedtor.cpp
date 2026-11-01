#include "game/hsd_jobj.h"
#include "game/cptcl.h"

extern "C" {
extern void* __vt__8CPtclOne[];
extern void* __vt__10CPtclDust2[];
}

/**
 * @note Address: 0x80025030
 * @note Size: 0x88
 */
// todo: weak copy
extern "C" CPtclOne* __dt__8CPtclOneFv(CPtclOne* ptcl, s32 flags)
{
	if (ptcl != NULL) {
		*(void***)((u8*)ptcl + 0x4C) = __vt__8CPtclOne;
		ptcl->mVel.CVec::~CVec();
		ptcl->mSize.CVec::~CVec();
		ptcl->mPos.CVec::~CVec();
		if ((s16)flags > 0) {
			::operator delete(ptcl);
		}
	}
	return ptcl;
}

/**
 * @note Address: 0x800250B8
 * @note Size: 0x98
 */
// todo: weak copy
extern "C" CPtclDust2* __dt__10CPtclDust2Fv(CPtclDust2* ptcl, s32 flags)
{
	if (ptcl != NULL) {
		*(void***)((u8*)ptcl + 0x4C) = __vt__10CPtclDust2;
		ptcl->CPtclOne::~CPtclOne();
		if ((s16)flags > 0) {
			::operator delete(ptcl);
		}
	}
	return ptcl;
}
