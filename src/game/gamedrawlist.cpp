#include "game/cgame.h"
#include "game/xchr.h"
#include "game/xobjs.h"

/**
 * @note Address: 0x80011270
 * @note Size: 0x1FC
 */
// fills the first draw list, flag 0x4000 objects first, then the rest, then chibi robo and pool 0's object
extern "C" void fn_80011270(CGame* game)
{
	s32 count = 0;
	for (s32 i = 0; i < game->mObjectCount; ++i) {
		XObj* object = game->mObjects[i];
		if (object != game->mPlayer && object != (XObj*)game->_9694[0].mObjects && (object->mStatus.mFlags & 1ULL)
		    && (object->mStatus.mFlags & 0x4000ULL)) {
			game->mDrawLists[0].mObjects[count] = object;
			game->mDrawLists[0].mIndices[count] = i;
			++count;
		}
	}
	for (s32 i = 0; i < game->mObjectCount; ++i) {
		XObj* object = game->mObjects[i];
		if (object != game->mPlayer && object != (XObj*)game->_9694[0].mObjects && (object->mStatus.mFlags & 1ULL)
		    && !(object->mStatus.mFlags & 0x4000ULL)) {
			game->mDrawLists[0].mObjects[count] = object;
			game->mDrawLists[0].mIndices[count] = i;
			++count;
		}
	}
	XObj* player = game->mPlayer;
	if (player->mStatus.mFlags & 1ULL) {
		game->mDrawLists[0].mObjects[count++] = player;
	}
	XObj* other = (XObj*)game->_9694[0].mObjects;
	if (other->mStatus.mFlags & 1ULL) {
		game->mDrawLists[0].mObjects[count++] = other;
	}
	game->mDrawLists[0].mCount = count;
}
