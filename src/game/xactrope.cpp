#include "game/xchr.h"
#include "game/cgame.h"

static inline u8 IsDiskBusy(CGame* game)
{
	return (u8)((game->mDvdStatus || game->mPrevDvdStatus) && game->mDiskOk == 0);
}

static inline u8 IsGameRunning(CGame* game)
{
	return (u8)((game->mStatus.mFlags & 1) && !(game->mStatus.mFlags & 0x22) && !IsDiskBusy(game));
}

/**
 * @note Address: 0x800C7AC4
 * @note Size: 0x100
 */
extern "C" void fn_800C7AC4(XActRope* act, const CPadOne* pad, f32* weight)
{
	if (act->mState == 5) {
		if (!IsGameRunning(&lbl_8025C400))
			return;
		if (lbl_8025C400._5D080 != NULL)
			return;
		if (pad->mTrigger & 0x200)
			act->mState = 8;
		if (act->mOwner->mAnimId == 0x8A || act->mOwner->mAnimId == 0x8C)
			*weight = 0.0f;
	} else if (act->mState != 0) {
		*weight = 0.0f;
	}
}
