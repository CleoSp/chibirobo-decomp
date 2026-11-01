#include "game/menulistentry.h"

// todo: cleanup
static inline void ClearEntry(MenuPCItem* entry)
{
	entry->mItem = -1;
	entry->_02   = -1;
	entry->_04   = -1;
}

/**
 * @note Address: 0x8003466C
 * @note Size: 0x33C
 */
extern "C" void fn_8003466C(MenuPCPage* page)
{
	for (s32 i = 0; i < 20; ++i) {
		ClearEntry(&page->mRows[i]);
		page->mIndex[i] = -1;
	}

	s32 eligible = 0;
	s32 count    = 0;
	for (s32 i = 0; i < 20; ++i) {
		s16 item  = lbl_80366DB4[i].mItem;
		s16 value = lbl_80366DB4[i]._02;
		s16 extra = lbl_80366DB4[i]._04;
		if (item >= 0 && value >= 0) {
			if (eligible >= page->mStart && eligible < page->mStart + 9 && count < 9) {
				page->mRows[count].mItem = item;
				page->mRows[count]._02   = value;
				page->mRows[count]._04   = extra;
				page->mIndex[count]      = i;
				++count;
			}
			++eligible;
		}
	}

	ClearEntry(&page->mPrev[0]);
	ClearEntry(&page->mNext[0]);
	count    = 0;
	eligible = 0;
	for (s32 i = 0; i < 20; ++i) {
		s16 item  = lbl_80366DB4[i].mItem;
		s16 value = lbl_80366DB4[i]._02;
		s16 extra = lbl_80366DB4[i]._04;
		if (item >= 0 && value >= 0) {
			if (eligible >= page->mStart + 9 && eligible < page->mStart + 10) {
				page->mNext[count].mItem = item;
				page->mNext[count]._02   = value;
				page->mNext[count]._04   = extra;
				if (++count >= 1)
					break;
			}
			++eligible;
		}
	}

	count    = 0;
	eligible = 0;
	for (s32 i = 0; i < 20; ++i) {
		s16 item  = lbl_80366DB4[i].mItem;
		s16 value = lbl_80366DB4[i]._02;
		s16 extra = lbl_80366DB4[i]._04;
		if (item >= 0 && value >= 0) {
			if (eligible >= page->mStart - 1 && eligible < page->mStart) {
				page->mPrev[count].mItem = item;
				page->mPrev[count]._02   = value;
				page->mPrev[count]._04   = extra;
				if (++count >= 1)
					break;
			}
			++eligible;
		}
	}
}
