#include "game/xobjactor.h"
#include "game/gmath.h"

/**
 * @note Address: 0x80131AB0
 * @note Size: 0x20
 */
void XObjActorDeca::UpdateAnim(u32 frames)
{
	CXObjIntp::UpdateAnim(frames);
}

/**
 * @note Address: 0x80131AD0
 * @note Size: 0x20
 */
void XObjActorDeca::UpdateObject(u32 frames)
{
	UpdateObject__9XObjActorFUl(this, frames);
}

/**
 * @note Address: 0x80131AF0
 * @note Size: 0x1C
 */
void XObjActorDeca::ProcessPendingEvents(u32)
{
	if (_998.Check(2)) {
		_998.mFlags &= ~2;
	}
}

/**
 * @note Address: 0x80131B0C
 * @note Size: 0xDC
 */
void XObjActorDeca::ResetActorState()
{
	_998.mFlags = 0;
	_C90.mFlags = 0;
	_C98.mFlags = 0;
	_980        = 0.0f;
	_984        = 0.0f;
	_988        = 0.0f;
	_C84        = _988;
	_C88        = 0.0f;
	_C8C        = 0;
	_CA0        = PI;
	_CA4        = PI;
	mState      = &XObjActorDeca::StateHold;
	ResetActorMovementStep(this);
	for (u32 i = 0; i < 20; i++) {
		mCmd[i] = 0;
	}
}
