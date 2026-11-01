#include "game/hsd_jobj.h"
#include "game/ctpl.h"

extern "C" void* __vt__4CTpl[];

/**
 * @note Address: 0x8001B7C8
 * @note Size: 0x7C
 */
// todo: weak copy
extern "C" CTpl* __dt__4CTplFv(CTpl* tpl, s32 flags)
{
	if (tpl != NULL) {
		*(void***)((u8*)tpl + 0xE8) = __vt__4CTpl;
		tpl->mBound                 = FALSE;
		tpl->mTex.CTexObj::~CTexObj();
		tpl->CFile::~CFile();
		if ((s16)flags > 0) {
			::operator delete(tpl);
		}
	}
	return tpl;
}
