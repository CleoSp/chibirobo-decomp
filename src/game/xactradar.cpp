#include "game/xchr.h"
#include <math.h>
#include "game/cscrn.h"

/**
 * @note Address: 0x800BFC04
 * @note Size: 0x68
 */
extern "C" void fn_800BFC04(XActRadar* act, void*, f32* amount)
{
	if (act->mOwner->GetAttachment() == 3) {
		if (act->mState == 1 || act->mState == 2)
			*amount = 0.0f;
		else if (act->mState == 3 || act->mState == 4 || act->mState == 5)
			*amount = 0.0f;
	}
}

static inline void ClearRadar(XActRadar* act)
{
	act->_2C   = 0;
	act->_30.x = act->_30.y = act->_30.z = 0.0f;
	act->_3C.x = act->_3C.y = act->_3C.z = 0.0f;
	act->_BC                             = NAN;
	fn_80023EE0(&act->mColor[0], 0, 0, 0, 0, 255, 25, 255, 0, 0.0f);
	fn_80023EE0(&act->mColor[1], 0, 0, 0, 0, 255, 25, 255, 0, 0.0f);
	act->mFade = 0.0f;
}

/**
 * @note Address: 0x800BFC6C
 * @note Size: 0xCC
 */
extern "C" void fn_800BFC6C(XActRadar* act)
{
	act->Reset();
	ClearRadar(act);
}

/**
 * @note Address: 0x800BFD38
 * @note Size: 0xE0
 */
extern "C" void fn_800BFD38(XActRadar* act, XObjChr* owner)
{
	act->Init(owner);
	act->Reset();
	ClearRadar(act);
}
