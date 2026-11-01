#include "game/hsd_jobj.h"
#include "game/cfont.h"

void operator delete(void* object);

extern "C" void* __vt__5CFont[];

/**
 * @note Address: 0x8000B334
 * @note Size: 0x60
 */
// todo: weak copy
extern "C" CFont* __dt__5CFontFv(CFont* font, s32 flags)
{
	if (font != NULL) {
		*(void***)((u8*)font + 0x13C) = __vt__5CFont;
		font->mTex.CTexObj::~CTexObj();
		if ((s16)flags > 0)
			::operator delete(font);
	}
	return font;
}
