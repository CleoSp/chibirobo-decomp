#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/globals.h"
#include "game/xchr.h"
#include "game/menu.h"

// todo: cleanup
static inline void ClearEntry(MenuStatusItem* entry)
{
	entry->mItem          = -1;
	entry->mCount         = -1;
	entry->mStatus.mFlags = 0;
}

/**
 * @note Address: 0x80034E4C
 * @note Size: 0x60
 */
extern "C" s32 fn_80034E4C(s16 id)
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
 * @note Address: 0x80034EAC
 * @note Size: 0x2D8
 */
extern "C" void fn_80034EAC(MenuStatusItems* page)
{
	for (s32 i = 0; i < 256; ++i) {
		page->mItems[i].mItem          = -1;
		page->mItems[i].mCount         = -1;
		page->mItems[i].mStatus.mFlags = 0;
	}

	s32 eligible = 0;
	s32 count    = 0;
	for (s32 i = 255; i >= 0; --i) {
		XObjChr* player = lbl_8025C400.mPlayer;
		s16 id          = player->mParams.mInventory.mItems[i].mId;
		s16 amount      = player->mParams.mInventory.mItems[i].mCount;
		if (fn_8006E44C(id, 4) && id >= 0 && amount > 0) {
			if (eligible >= page->mPage * 4 && eligible < (page->mPage + 2) * 4) {
				page->mItems[count].mItem  = id;
				page->mItems[count].mCount = amount;
				if (++count >= 8)
					break;
			}
			++eligible;
		}
	}

	for (s32 i = 0; i < 4; ++i) {
		ClearEntry(&page->mPrev[i]);
		ClearEntry(&page->mNext[i]);
	}
	count    = 0;
	eligible = 0;
	for (s32 i = 255; i >= 0; --i) {
		XObjChr* player = lbl_8025C400.mPlayer;
		s16 id          = player->mParams.mInventory.mItems[i].mId;
		s16 amount      = player->mParams.mInventory.mItems[i].mCount;
		if (fn_8006E44C(id, 4) && id >= 0 && amount > 0) {
			if (eligible >= (page->mPage + 2) * 4 && eligible < (page->mPage + 4) * 4) {
				page->mNext[count].mItem  = id;
				page->mNext[count].mCount = amount;
				if (++count >= 4)
					break;
			}
			++eligible;
		}
	}

	count    = 0;
	eligible = 0;
	for (s32 i = 255; i >= 0; --i) {
		XObjChr* player = lbl_8025C400.mPlayer;
		s16 id          = player->mParams.mInventory.mItems[i].mId;
		s16 amount      = player->mParams.mInventory.mItems[i].mCount;
		if (fn_8006E44C(id, 4) && id >= 0 && amount > 0) {
			if (eligible >= (page->mPage - 1) * 4 && eligible < (page->mPage + 1) * 4) {
				page->mPrev[count].mItem  = id;
				page->mPrev[count].mCount = amount;
				if (++count >= 4)
					break;
			}
			++eligible;
		}
	}
}
