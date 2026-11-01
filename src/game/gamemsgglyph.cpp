#include "game/hsd_jobj.h"
#include "game/msg.h"

void operator delete(void* object);

/**
 * @note Address: 0x80008FB0
 * @note Size: 0x7C
 */
// todo: weak copy
extern "C" MsgFontGlyph* __dt__12MsgFontGlyphFv(MsgFontGlyph* glyph, s32 flags)
{
	if (glyph != NULL) {
		glyph->mSprite.CSprite::~CSprite();
		glyph->mTexture.CTexObj::~CTexObj();
		if ((s16)flags > 0)
			::operator delete(glyph);
	}
	return glyph;
}
