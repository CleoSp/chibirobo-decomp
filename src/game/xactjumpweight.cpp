#include "game/xact.h"

/**
 * @note Address: 0x800CBDB4
 * @note Size: 0x18
 */
extern "C" void fn_800CBDB4(XActJump* action, void* unused, f32* weight)
{
	if (action->mState)
		*weight = 0.0f;
}
