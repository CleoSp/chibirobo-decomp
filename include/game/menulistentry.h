#ifndef _GAME_MENULISTENTRY_H
#define _GAME_MENULISTENTRY_H

#include "types.h"

// a row of one of the chibi-pc lists, MenuPC pages through them. names are ours
struct MenuPCItem {
	s16 mItem;  // _00, -1 for an empty row
	s16 _02;    // _02, -1 for an empty row
	s16 _04;    // _04
	u8 mMarked; // _06
	u8 _07;     // _07
};
STATIC_ASSERT(sizeof(MenuPCItem) == 8);

// the nine rows on screen and the ones just off either end
struct MenuPCPage {
	s32 mStart;           // _00, list index of the first row
	MenuPCItem mRows[20]; // _04
	s32 mIndex[20];       // _A4, list index of each row
	MenuPCItem mPrev[1];  // _F4
	MenuPCItem mNext[1];  // _FC
};
STATIC_ASSERT(sizeof(MenuPCPage) == 0x104);

extern "C" {
extern MenuPCItem lbl_80366C9C[20];
extern MenuPCItem lbl_80366DB4[20];
}

#endif
