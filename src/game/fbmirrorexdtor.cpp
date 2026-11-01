#include "game/hsd_jobj.h"
#include "game/fbmirror.h"

void operator delete(void* object);
extern "C" void* __vt__10FbMirrorEx[];

// todo: weak copy
/**
 * @note Address: 0x80019600
 * @note Size: 0x100
 */
extern "C" FbMirrorEx* fn_80019600(FbMirrorEx* mirror, s32 flags)
{
	if (mirror != NULL) {
		*(void***)((u8*)mirror + 0x120) = __vt__10FbMirrorEx; // _120 vtable
		mirror->FbMirror::~FbMirror();
		if ((s16)flags > 0) {
			::operator delete(mirror);
		}
	}
	return mirror;
}
