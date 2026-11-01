#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/evt.h"
#include "game/xchr.h"

// todo: cleanup
static inline s16 InventoryCount(s16 id)
{
	XObjChr* player = lbl_8025C400.mPlayer;
	s32 count       = 0;
	for (s32 i = 255; i >= 0; --i) {
		if (id == player->mParams.mInventory.mItems[i].mId) {
			count = player->mParams.mInventory.mItems[i].mCount;
			break;
		}
	}
	return count;
}

/**
 * @note Address: 0x80034248
 * @note Size: 0x360
 */
extern "C" void fn_80034248(u8* unlocked)
{
	unlocked[0] = 1;
	unlocked[1] = 1;
	unlocked[2] = 1;
	unlocked[3] = 1;
	unlocked[4] = 1;
	if (InventoryCount(0x35) > 0)
		unlocked[5] = 1;
	if (InventoryCount(0x36) > 0)
		unlocked[6] = 1;
	if (InventoryCount(0x3E) > 0)
		unlocked[7] = 1;
	if (InventoryCount(0x9E) > 0)
		unlocked[8] = 1;
	if (InventoryCount(0x08) > 0 || (lbl_8036781C[0] & 0x20000000))
		unlocked[9] = 1;
	if (InventoryCount(0x30) > 0 || (lbl_8036781C[0] & 0x40000000))
		unlocked[10] = 1;
	if (InventoryCount(0x6D) > 0 || (lbl_8036781C[0] & 0x10000000))
		unlocked[11] = 1;
	if ((lbl_8036781C[33] & 0x01000000) && (InventoryCount(0x7C) > 0 || (lbl_8036781C[21] & 0x01000000))) {
		unlocked[12] = 1;
	}
}
