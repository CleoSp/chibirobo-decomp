#include "game/xobjspidersmall.h"
#include "game/cgame.h"

// todo: cleanup
static inline bool IsGameRunning()
{
	bool running = false;
	if (lbl_8025C400.mStatus.mFlags & 1ULL) {
		if (!(lbl_8025C400.mStatus.mFlags & 0x22ULL)) {
			bool waiting = false;
			if (lbl_8025C400.mDvdStatus != 0 || lbl_8025C400.mPrevDvdStatus != 0) {
				if (lbl_8025C400.mDiskOk == 0) {
					waiting = true;
				}
			}
			if (!waiting) {
				running = true;
			}
		}
	}
	return running;
}

// todo: cleanup
static inline bool CanPlaySe()
{
	bool ok = false;
	if (IsGameRunning() && lbl_8025C400._5D080 == NULL && lbl_8025C400._8580 == 0 && !(lbl_8025C400.mStatus.mFlags & 0xC00ULL)
	    && lbl_8025C400.mTimeState.mDuration > 0.0f && lbl_8025C400.mTimeState.mRunning == 0) {
		ok = true;
	}
	return ok;
}

/**
 * @note Address: 0x800F4DB8
 * @note Size: 0x190
 */
extern "C" void fn_800F4DB8(XObjSpiderSmallFx* fx, const Vec* pos)
{
	f32 x = pos->x, y = pos->y, z = pos->z;
	fx->mStatus.mFlags |= 1;
	fx->mPos.x = x;
	fx->mPos.y = y;
	fx->mPos.z = z;
	fx->mState = 1;
	if (CanPlaySe()) {
		CSound* sound = (CSound*)&lbl_8025C400.mSound;
		fn_8001E904(sound, 0x184, 255); // npc_spider_transfer
		fn_8001DF00(sound, &fx->mPos, 0);
	}
}
