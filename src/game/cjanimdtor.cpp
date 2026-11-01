#include "game/cobj.h"

void operator delete(void* object);
extern "C" void* __vt__6CJAnim[];

// todo: weak copy
/**
 * @note Address: 0x8001B0AC
 * @note Size: 0x5C
 */
extern "C" CJAnim* fn_8001B0AC(CJAnim* anim, s32 flags)
{
	if (anim != NULL) {
		*(void***)((u8*)anim + sizeof(CJArchive) - 4) = __vt__6CJAnim; // _7C vtable, _74 in japan
		anim->CJArchive::~CJArchive();
		if ((s16)flags > 0) {
			::operator delete(anim);
		}
	}
	return anim;
}
