#include "game/xchr.h"
#include "game/hsd_cobj.h"

extern "C" {
extern const s32 lbl_8066008C;
extern const s32 lbl_80660090;
}

// todo: cleanup
static inline f32 CurrentFrame(HSD_JObj* joint)
{
	HSD_AObj* animation = GetAObj(joint);
	return animation != NULL ? HSD_AObjGetCurrentFrame(animation) : 0.0f;
}

// his radius, bigger while he holds something up, by the scale of the held joint
/**
 * @note Address: 0x800A0414
 * @note Size: 0x280
 */
extern "C" f32 fn_800A0414(XObjChr* player)
{
	f32 size = 1.5f * player->mMoveScale.x;
	if (fn_800D4D54(&player->mLift)) {
		if (player->mAnimId == 11 && CurrentFrame(player->mJObj) < 45.0f)
			goto done;
		if (player->mAnimId == 10 && CurrentFrame(player->mJObj) > 22.0f)
			goto done;
		CVec scale;
		s32 path[2];
		path[0] = lbl_8066008C;
		path[1] = lbl_80660090;
		HSD_JObjGetScale(GetJointByPath(player->mJObj, path, 2), &scale);
		size += 4.0f * scale.x;
	}
done:
	return size;
}
