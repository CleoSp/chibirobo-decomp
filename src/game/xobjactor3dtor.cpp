#include "game/xobjactor.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern void* __vt__10XObjActor3[];
extern void* __vt__5CPtcl[];
CPtclDust3* __dt__10CPtclDust3Fv(CPtclDust3* ptcl, s32 flags);
CTexObj* __dt__7CTexObjFv(CTexObj* tex, s32 flags);
}

/**
 * @note Address: 0x80108424
 * @note Size: 0x11C
 */
// todo: weak copy
extern "C" XObjActor3* __dt__10XObjActor3Fv(XObjActor3* actor, s32 flags)
{
	if (actor != NULL) {
		*(void***)((u8*)actor + 0x34) = __vt__10XObjActor3;
		__destroy_arr(actor->mTex, (ConstructorDestructor*)__dt__7CTexObjFv, sizeof(CTexObj), 2);
		// retail checks the member too
		if (&actor->mPtcl != NULL) {
			__destroy_arr(actor->mPtcls, (ConstructorDestructor*)__dt__10CPtclDust3Fv, sizeof(CPtclDust3), 50);
			if (&actor->mPtcl != NULL) {
				*(void***)((u8*)&actor->mPtcl + 0x10) = __vt__5CPtcl;
			}
		}
		actor->_BDC.CVec::~CVec();
		actor->_B8C.CStatus::~CStatus();
		actor->_980.CTpl::~CTpl();
		__dt__4XObjFv(actor, 0);
		if ((s16)flags > 0) {
			::operator delete(actor);
		}
	}
	return actor;
}
