#include "game/hsd_jobj.h"
#include "game/msg.h"

void operator delete(void* object);

// todo: weak copy
/**
 * @note Address: 0x8001706C
 * @note Size: 0xC8
 */
extern "C" CMsgFont* __dt__8CMsgFontFv(CMsgFont* font, s32 flags)
{
	if (font != NULL) {
		font->CMsgFont::~CMsgFont();
		if ((s16)flags > 0)
			::operator delete(font);
	}
	return font;
}
