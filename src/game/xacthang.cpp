#include "game/xchr.h"
#include "game/cgame.h"
#include <math.h>
#include "game/gmath.h"

// todo: cleanup
static inline u8 GameStatus(CGame* game)
{
	return (u8)((game->mDvdStatus || game->mPrevDvdStatus) && game->mDiskOk == 0);
}

// todo: cleanup
static inline u8 InputEnabled(CGame* game)
{
	return (u8)((game->mStatus.mFlags & 1) && !(game->mStatus.mFlags & 0x22) && !GameStatus(game));
}

extern "C" void fn_800CDBA8(XActHang* action);

/**
 * @note Address: 0x800CE120
 * @note Size: 0x214
 */
extern "C" void fn_800CE120(XActHang* action, const CPadOne* pad, f32* movement)
{
	action->_2C = 0;
	if (action->mState == 4) {
		f32 stick = -(f32)atan2(pad->mStickX, pad->mStickY);
		f32 angle = stick + lbl_8025C400._E5DEC[1] - action->mOwner->mMoveRotate.y;
		angle     = WrapAngle(angle);
		if (!(angle < 0.5235988f || angle > 5.7595863f))
			action->_2C = 1;
		*movement = 0.0f;
	} else if (action->mState == 5) {
		if (InputEnabled(&lbl_8025C400) && lbl_8025C400._5D080 == NULL) {
			if (pad->mTrigger & 0x100) {
				if (action->mTarget == NULL || (action->mTarget->mStatus.mFlags & 0x400000000ULL)) {
					fn_800CDBA8(action);
					action->_0C = 0;
				}
				*movement = 0.0f;
			} else if (pad->mTrigger & 0x200) {
				action->mState = 9;
				action->_0C    = 0;
				*movement      = 0.0f;
			}
		}
	} else if (action->mState != 0) {
		*movement = 0.0f;
	}
}

/**
 * @note Address: 0x800CE334
 * @note Size: 0x20
 */
extern "C" void fn_800CE334(XActLift* action, u32 value, BOOL apply)
{
	action->_54 = value;
	if (apply)
		action->mOwner->mPlug._D3C = value | 0x10000;
}
