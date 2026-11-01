#include "game/hsd_jobj.h"
#include "game/menu.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
extern void* __vt__10MenuCursor[];
extern void* __vt__5Title[];
extern void* __vt__5CFile[];
extern void* __vt__4CTpl[];
extern void* __vt__7CTexObj[];
extern void* __vt__8XObjDemo[];
extern void* __vt__7CCamera[];
extern void* __vt__4CVec[];
Cursor* fn_8001AF70(Cursor* cursor);
XObj* fn_8001AFAC(XObj* obj);
XObjDemo* fn_8001B4B4(XObjDemo* demo);
CTexObj* __ct__7CTexObjFv(CTexObj* tex);
CLight* fn_8001B538(CLight* light);
void* __dt__7CTexObjFv(void* tex, s32 flags);
void* __dt__6CLightFv(void* light, s32 flags);
}

// todo: weak copy
/**
 * @note Address: 0x8001B3CC
 * @note Size: 0x3C
 */
extern "C" MenuCursor* fn_8001B3CC(MenuCursor* cursor)
{
	fn_8001AF70(cursor);
	*(void***)((u8*)cursor + 0x34) = __vt__10MenuCursor; // _34 vtable
	return cursor;
}

// todo: weak copy
/**
 * @note Address: 0x8001B408
 * @note Size: 0xAC
 */
extern "C" Title* fn_8001B408(Title* title)
{
#if defined(VERSION_JP)
	*(void***)((u8*)title + 0x3C24) = __vt__5Title; // _3C24 vtable
#else
	*(void***)((u8*)title + 0x3C48) = __vt__5Title; // _3C48 vtable
#endif
	fn_8001AF70(&title->mCursor);
	fn_8001B4B4(&title->mDemo);
	CTpl* tpl                          = &title->mTpl;
	*(void***)((u8*)tpl + 0xE8)        = __vt__5CFile; // the inlined CFile ctor
	tpl->mArchive                      = NULL;
	tpl->mEntrynum                     = -1;
	tpl->mBuffer                       = NULL;
	tpl->mResult                       = 0;
	tpl->mState                        = 1;
	*(void***)((u8*)tpl + 0xE8)        = __vt__4CTpl;
	*(void***)((u8*)&tpl->mTex + 0xFC) = __vt__7CTexObj;
	__construct_array(title->mTex, (ConstructorDestructor)__ct__7CTexObjFv, (ConstructorDestructor)__dt__7CTexObjFv, sizeof(CTexObj), 6);
	return title;
}

// todo: weak copy
/**
 * @note Address: 0x8001B4B4
 * @note Size: 0x84
 */
extern "C" XObjDemo* fn_8001B4B4(XObjDemo* demo)
{
	fn_8001AFAC(demo);
	*(void***)((u8*)demo + 0x34)            = __vt__8XObjDemo; // _34 vtable
	*(void***)((u8*)&demo->mCamera + 0x190) = __vt__7CCamera;
	*(void***)((u8*)&demo->mCamera + 0xA0)  = __vt__4CVec;
	*(void***)((u8*)&demo->mCamera + 0xB0)  = __vt__4CVec;
	*(void***)((u8*)&demo->mCamera + 0xC0)  = __vt__4CVec;
	*(void***)((u8*)&demo->mCamera + 0xD0)  = __vt__4CVec;
	*(void***)((u8*)&demo->mCamera + 0x124) = __vt__4CVec;
	__construct_array(demo->mLights, (ConstructorDestructor)fn_8001B538, (ConstructorDestructor)__dt__6CLightFv, sizeof(CLight), 8);
	return demo;
}
