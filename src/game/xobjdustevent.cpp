#include "game/xobjhitevent.h"
#include "game/xobjflags.h"
#include "game/cgame.h"
#include "game/xchr.h"
#include "game/hsd_cobj.h"
#include "game/evt.h"
#include "game/xobjchr.h"

static inline BOOL AnimDone(HSD_JObj* joint)
{
	HSD_AObj* aobj = GetAObj(joint);
	return aobj != NULL ? (aobj->flags & 0x44000000) != 0 : TRUE;
}

/**
 * @note Address: 0x800DED24
 * @note Size: 0x37C
 */
// event 16 waits for its own anim, event 56 shrinks it towards mTo while chibi robo plays anim 0x8C0
extern "C" BOOL UpdateDustEvent(XObjDust* dust, u32 frames)
{
	HSD_JObj* joint;
	if (dust->mStatus.mFlags & 0x4000000000000000ULL) {
		if (dust->mEventId == 16) {
			joint          = dust->mJObj;
			HSD_AObj* aobj = GetAObj(joint);
			if (aobj != NULL) {
				if (aobj->flags & 0x44000000) {
					if (dust->mStatus.mFlags & 0xC0000000000ULL) {
						StartObjectHitEvent(dust, dust->mEventId, dust->mEventArg);
					}
					return TRUE;
				}
			} else
				return TRUE;
		} else if (dust->mEventId == 56) {
			XObj* player = gGame.GetPlayer();
			if (player->mAnimId == 0x8C0) {
				if (AnimDone(player->mJObj))
					return TRUE;
				HSD_AObj* aobj         = GetAObj(player->mJObj);
				f32 end                = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
				aobj                   = GetAObj(player->mJObj);
				f32 frame              = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
				f32 t                  = 1.0f - frame / end;
				dust->mMoveTranslate.x = t * (dust->mFrom.x - dust->mTo.x) + dust->mTo.x;
				dust->mMoveTranslate.z = t * (dust->mFrom.z - dust->mTo.z) + dust->mTo.z;
				dust->mMoveScale.Set(dust->mStartScale.x * t, dust->mStartScale.y * t, dust->mStartScale.z * t);
			}
		}
	}
	return FALSE;
}
