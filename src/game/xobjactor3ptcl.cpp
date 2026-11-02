#include "game/xobjactor.h"

/**
 * @note Address: 0x80109004
 * @note Size: 0x4
 */
extern "C" void fn_80109004(XObjActor3* actor)
{
}

/**
 * @note Address: 0x80109008
 * @note Size: 0x44
 */
extern "C" void fn_80109008(XObjActor3* actor)
{
	if (actor->GetKind() == 0x31B) {
		fn_80024CA0(&actor->mDust.mPtcl);
	}
}
