#include "game/infoscrn.h"
#include "game/cgame.h"
#include "game/xchr.h"

// how many items chibi robo can carry, by the upgrade he holds
static inline s32 ItemMax()
{
	XObjChr* player = lbl_8025C400.mPlayer;
	for (s32 i = 0; i < 256; ++i) {
		s16 id    = player->mParams.mInventory.mItems[i].mId;
		s16 count = player->mParams.mInventory.mItems[i].mCount;
		if (id >= 0 && count > 0) {
			switch (id) {
			case 0x57:
				return 5;
			case 0x59:
				return 10;
			case 0x5A:
				return 15;
			}
		}
	}
	return 5;
}

/**
 * @note Address: 0x80066250
 * @note Size: 0x274
 */
extern "C" void fn_80066250(InfoScrn* scrn, u8 show, s32 mode, s32 flag)
{
	switch (mode) {
	case 0:
		if (show)
			scrn->_148 = 1;
		else
			scrn->_148 = 2;
		break;
	case 1:
		if (show)
			scrn->_148 = 3;
		else
			scrn->_148 = 4;
		break;
	case 2:
		scrn->mItemMax = ItemMax();
		break;
	case 3:
		scrn->_148         = 5;
		scrn->mNextItemMax = ItemMax();
		break;
	case 4:
		if (show)
			scrn->_148 = 7;
		else
			scrn->_148 = 8;
		break;
	}
}
