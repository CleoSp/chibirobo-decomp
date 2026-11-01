#include "game/octamenu.h"

void operator delete(void* object);

/**
 * @note Address: 0x800BCFC4
 * @note Size: 0x58
 */
// usa and europe only, japan's marks have no object
// todo: weak copy
extern "C" OXMenuMark* fn_800BCFC4(OXMenuMark* mark, s32 flags)
{
	if (mark != NULL) {
		__dt__4XObjFv(&mark->mObj, -1);
		if ((s16)flags > 0)
			::operator delete(mark);
	}
	return mark;
}
