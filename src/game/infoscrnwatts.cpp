#include "game/infoscrn.h"
#include "game/cgame.h"
#include <string.h>

extern "C" {
// the ascii characters fn_80066630 swaps and their two byte sjis forms
extern const char* lbl_8065A37C;
extern const char* lbl_8065A380;
}

// todo: use XObjChr's mParams from game/xchr.h
struct PlayerParams {
#if defined(VERSION_JP)
	u8 _00[0x964C]; // _00
#else
	u8 _00[0x965C]; // _00
#endif
	f32 mWattsMax; // _965C, _964C in japan
};

/**
 * @note Address: 0x800664C4
 * @note Size: 0xA8
 */
extern "C" void fn_800664C4(InfoScrn* scrn, u8 show, s32 mode)
{
	switch (mode) {
	case 0:
		if (show)
			scrn->_BC = 1;
		else
			scrn->_BC = 2;
		break;
	case 1:
		if (show)
			scrn->_BC = 3;
		else
			scrn->_BC = 4;
		break;
	case 2:
		if (show)
			scrn->_C8 = -52.0f;
		else
			scrn->_C8 = 0.0f;
		break;
	case 3:
		if (show)
			scrn->_BC = 5;
		else
			scrn->_BC = 6;
		break;
	}
}

/**
 * @note Address: 0x8006656C
 * @note Size: 0x74
 */
extern "C" void fn_8006656C(InfoScrn* scrn, f32 watts)
{
	// retail runs this five times over the four slots
	for (s32 i = 0; i < 5; ++i) {
		scrn->_E0[i]  = 1.0f;
		scrn->_100[i] = 0;
		scrn->_104[i] = 0.0f;
		scrn->_114[i] = 0.0f;
	}
	scrn->_D0 = 10.0f * watts;
	scrn->_CC = scrn->_D0;
}

/**
 * @note Address: 0x800665E0
 * @note Size: 0x1C
 */
extern "C" BOOL fn_800665E0(InfoScrn* scrn)
{
	return scrn->_1D4 <= 0.0f;
}

/**
 * @note Address: 0x800665FC
 * @note Size: 0x34
 */
extern "C" BOOL fn_800665FC(InfoScrn* scrn)
{
	PlayerParams* player = (PlayerParams*)lbl_8025C400.mPlayer;
	return scrn->_1D4 == 10.0f * player->mWattsMax;
}

// swaps a string's ascii for sjis in place, gives back how many it swapped
/**
 * @note Address: 0x80066630
 * @note Size: 0xE4
 */
extern "C" s32 fn_80066630(char* text)
{
	char buffer[512];
	s32 count      = 0;
	const char* in = text;
	char* out      = buffer;
	while (*in != '\0') {
		const char* ascii = lbl_8065A37C;
		BOOL found        = FALSE;
		s32 index         = 0;
		while (*ascii != '\0') {
			if (*in == *ascii) {
				strncpy(out, lbl_8065A380 + index * 2, 2)[2] = '\0';
				found                                        = TRUE;
				++count;
				out += 2;
				break;
			}
			++ascii;
			++index;
		}
		if (!found) {
			strncpy(out, in, 1)[1] = '\0';
			++out;
		}
		++in;
	}
	strcpy(text, buffer);
	return count;
}
