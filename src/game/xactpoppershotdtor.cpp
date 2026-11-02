#include "game/xact.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__5CPtcl[];
void* __dt__10CPtclDust2Fv(void* object, s32 flags);
}

/**
 * @note Address: 0x800BCBB8
 * @note Size: 0x94
 */
// todo: weak copy
extern "C" XActPopperShot* fn_800BCBB8(XActPopperShot* shot, s32 flags)
{
	if (shot != NULL) {
		// retail checks the member addresses too
		if (&shot->mPtcl != NULL) {
			__destroy_arr(shot->mPtcls, (ConstructorDestructor*)__dt__10CPtclDust2Fv, sizeof(CPtclDust2), 40);
			if (&shot->mPtcl != NULL)
				*(void***)((u8*)&shot->mPtcl + sizeof(CPtcl) - 4) = __vt__5CPtcl;
		}
		shot->mPos.CVec::~CVec();
		if ((s16)flags > 0)
			::operator delete(shot);
	}
	return shot;
}
