#include "game/xobjactor.h"

extern "C" XObjActorTao::StateFunc sTaoIdleState;

/**
 * @note Address: 0x8012F364
 * @note Size: 0xC4
 */
void XObjActorTao::ResetActorState()
{
	_998.mFlags = 0;
	_C8C.mFlags = 0;
	_980        = 0.0f;
	_984        = 0.0f;
	_988        = 0.0f;
	_C84        = _988;
	_C88        = 0;
	mState      = sTaoIdleState;
	ResetActorMovementStep(this);
	mCmd[2] = 0;
	mCmd[3] = 0;
	mCmd[4] = 0;
	mCmd[5] = 0;
	mCmd[6] = 0;
	mCmd[7] = 0;
	mCmd[8] = 0;
	mCmd[9] = 0;
	_CF4    = -1;
	_CF8    = 0;
	_CFC    = NULL;
	_CF4    = 0x82;
	_CF8    = 1;
	_998.mFlags |= 0x2000;
}
