#include "game/xobj.h"
#include "game/hsd_jobj.h"
#include "game/title.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__5Title[];
extern void* __vt__6Cursor[];
CTexObj* __dt__7CTexObjFv(CTexObj* tex, s32 flags);
XObjDemo* fn_80008B38(XObjDemo* demo, s32 flags);
Cursor* fn_8000871C(Cursor* cursor, s32 flags);
}

/**
 * @note Address: 0x80008650
 * @note Size: 0xCC
 */
// todo: weak copy
extern "C" Title* fn_80008650(Title* title, s32 flags)
{
	if (title != NULL) {
#if defined(VERSION_JP)
		*(void***)((u8*)title + 0x3C24) = __vt__5Title; // _3C24 vtable
#else
		*(void***)((u8*)title + 0x3C48) = __vt__5Title; // _3C48 vtable
#endif
		__destroy_arr(title->mTex, (ConstructorDestructor*)__dt__7CTexObjFv, sizeof(CTexObj), 6);
		title->mTpl.CTpl::~CTpl();
		fn_80008B38(&title->mDemo, -1);
		fn_8000871C(&title->mCursor, -1);
		if ((s16)flags > 0)
			::operator delete(title);
	}
	return title;
}

/**
 * @note Address: 0x8000871C
 * @note Size: 0x60
 */
// todo: weak copy
extern "C" Cursor* fn_8000871C(Cursor* cursor, s32 flags)
{
	if (cursor != NULL) {
		*(void***)((u8*)cursor + 0x34) = __vt__6Cursor;
		fn_8000877C(cursor, 0);
		if ((s16)flags > 0)
			::operator delete(cursor);
	}
	return cursor;
}
