#include "game/menulistentry.h"

// todo: cleanup
static inline void ClearEntry(MenuPCItem& entry)
{
	entry.mItem   = -1;
	entry._02     = -1;
	entry._04     = -1;
	entry.mMarked = 0;
}

/**
 * @note Address: 0x80034A6C
 * @note Size: 0x3E0
 */
extern "C" void fn_80034A6C(MenuPCPage* page)
{
	for (u32 i = 0; i < 20; i++) {
		page->mRows[i].mItem   = -1;
		page->mRows[i]._02     = -1;
		page->mRows[i]._04     = -1;
		page->mRows[i].mMarked = 0;
		page->mIndex[i]        = -1;
	}
	s32 valid  = 0;
	s32 output = 0;
	for (s32 i = 0; i < 20; i++) {
		s16 item  = lbl_80366C9C[i].mItem;
		s16 value = lbl_80366C9C[i]._02;
		s16 extra = lbl_80366C9C[i]._04;
		if (item >= 0 && value >= 0) {
			if (valid >= page->mStart && valid < page->mStart + 9 && output < 9) {
				page->mRows[output].mItem = item;
				page->mRows[output]._02   = value;
				page->mRows[output]._04   = extra;
				page->mIndex[output]      = i;
				if (lbl_80366C9C[i].mMarked)
					page->mRows[output].mMarked = 1;
				output++;
			}
			valid++;
		}
	}
	ClearEntry(page->mPrev[0]);
	ClearEntry(page->mNext[0]);
	output = 0;
	valid  = 0;
	for (s32 i = 0; i < 20; i++) {
		s16 item  = lbl_80366C9C[i].mItem;
		s16 value = lbl_80366C9C[i]._02;
		s16 extra = lbl_80366C9C[i]._04;
		if (item >= 0 && value >= 0) {
			if (valid >= page->mStart + 9 && valid < page->mStart + 10) {
				MenuPCItem* entry = &page->mNext[output];
				entry->mItem      = item;
				entry->_02        = value;
				entry->_04        = extra;
				if (lbl_80366C9C[i].mMarked)
					entry->mMarked = 1;
				if (++output >= 1)
					break;
			}
			valid++;
		}
	}
	output = 0;
	valid  = 0;
	for (s32 i = 0; i < 20; i++) {
		s16 item  = lbl_80366C9C[i].mItem;
		s16 value = lbl_80366C9C[i]._02;
		s16 extra = lbl_80366C9C[i]._04;
		if (item >= 0 && value >= 0) {
			if (valid >= page->mStart - 1 && valid < page->mStart) {
				MenuPCItem* entry = &page->mPrev[output];
				entry->mItem      = item;
				entry->_02        = value;
				entry->_04        = extra;
				if (lbl_80366C9C[i].mMarked)
					entry->mMarked = 1;
				if (++output >= 1)
					break;
			}
			valid++;
		}
	}
}
