#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/xobj.h"
#include "game/stagedata.h"

/**
 * @note Address: 0x8000C398
 * @note Size: 0xB8
 */
// CGame's vf24, every live object's vfC8, then the stage's function 4
extern "C" void fn_8000C398(CGame* game)
{
	if (game->mStatus.mFlags & 1ULL) {
		for (s32 i = 0; i < game->mObjectCount; ++i) {
			XObj* object = game->mObjects[i];
			if (object->mStatus.mFlags & 1ULL)
				object->vfC8();
		}
		fn_8006F338(4, 0, NULL);
	}
}
