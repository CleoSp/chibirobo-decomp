#ifndef _GAME_CFONT_H
#define _GAME_CFONT_H

#include "game/ctpl.h"

// the game font, class name is RTTI
class CFont {
public:
	CFont() { _24 = NULL; }

	u8 _00[0x24]; // _00
	void* _24;    // _24, the font loads itself on first use while this is null
	u8 _28[0x14]; // _28
	CTexObj mTex; // _3C
	// _13C vtable

	virtual ~CFont() { } // _08 (weak)
};
typedef char CFont_size_check[sizeof(CFont) == 0x140 ? 1 : -1];

#endif
