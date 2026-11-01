#include "game/memcard.h"

// ccard.h's sdk card header can't come in next to the hsd ones
class CCard;

void operator delete(void* object);
extern "C" {
extern void* __vt__5CCard[];
extern void* __vt__7Memcard[];
}

// todo: weak copy
/**
 * @note Address: 0x80019218
 * @note Size: 0x4C
 */
extern "C" CCard* fn_80019218(CCard* card, s32 flags)
{
	if (card != NULL) {
		*(void***)((u8*)card + 0xA020) = __vt__5CCard; // _A020 vtable
		if ((s16)flags > 0) {
			::operator delete(card);
		}
	}
	return card;
}

// todo: weak copy
/**
 * @note Address: 0x80019264
 * @note Size: 0x64
 */
extern "C" Memcard* fn_80019264(Memcard* memcard, s32 flags)
{
	if (memcard != NULL) {
		*(void***)((u8*)memcard + 0x16A20) = __vt__7Memcard; // _16A20 vtable
		// the inlined dtor of the CCard at _20
		if ((CCard*)((u8*)memcard + 0x20) != NULL) {
			*(void***)((u8*)memcard + 0x20 + 0xA020) = __vt__5CCard;
		}
		if ((s16)flags > 0) {
			::operator delete(memcard);
		}
	}
	return memcard;
}
