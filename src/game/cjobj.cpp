#define CJOINT_SETTER_SECTION
#pragma section RX ".text.jointdtors"
#pragma section RX ".text.msgfontdtor"
#include "game/cgame.h"
#include "game/cjobjset.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__22CInterfaceCameraAction[];
extern void* __vt__23CInterfaceInterpolation[];
extern void* __vt__14CCameraMemento[];
extern void* __vt__8CMsgData[];
extern void* __vt__7CObjVec[];
extern void* __vt__5CJObj[];
}

#pragma section RX ".text.interfacedtors"

// CJObj's weak copies, everything in it is inline

/**
 * @note Address: 0x80016ECC
 * @note Size: 0x88
 */
// todo: weak copy
extern "C" __declspec(section ".text.interfacedtors") CInterfaceCameraAction*
__dt__22CInterfaceCameraActionFv(CInterfaceCameraAction* action, s32 flags)
{
	if (action != NULL) {
		*(void***)((u8*)action + 0x3C) = __vt__22CInterfaceCameraAction; // _3C vtable
		action->mAngle.CVec::~CVec();
		action->mInterest.CVec::~CVec();
		action->mEye.CVec::~CVec();
		if ((s16)flags > 0) {
			::operator delete(action);
		}
	}
	return action;
}

/**
 * @note Address: 0x80016F54
 * @note Size: 0x48
 */
// todo: weak copy
extern "C" __declspec(section ".text.interfacedtors") CInterfaceInterpolation*
__dt__23CInterfaceInterpolationFv(CInterfaceInterpolation* interp, s32 flags)
{
	if (interp != NULL) {
		*(void***)interp = __vt__23CInterfaceInterpolation;
		if ((s16)flags > 0) {
			::operator delete(interp);
		}
	}
	return interp;
}

/**
 * @note Address: 0x80016F9C
 * @note Size: 0x88
 */
// todo: weak copy
extern "C" __declspec(section ".text.interfacedtors") CCameraMemento* __dt__14CCameraMementoFv(CCameraMemento* memento, s32 flags)
{
	if (memento != NULL) {
		*(void***)memento = __vt__14CCameraMemento;
		memento->mAngle.CVec::~CVec();
		memento->mInterest.CVec::~CVec();
		memento->mEye.CVec::~CVec();
		if ((s16)flags > 0) {
			::operator delete(memento);
		}
	}
	return memento;
}

/**
 * @note Address: 0x80017024
 * @note Size: 0x48
 */
// todo: weak copy
extern "C" __declspec(section ".text.interfacedtors") CMsgData* __dt__8CMsgDataFv(CMsgData* data, s32 flags)
{
	if (data != NULL) {
#if defined(VERSION_JP)
		*(void***)((u8*)data + 0x894) = __vt__8CMsgData; // _894 vtable
#else
		*(void***)((u8*)data + 0x898) = __vt__8CMsgData; // _898 vtable
#endif
		if ((s16)flags > 0) {
			::operator delete(data);
		}
	}
	return data;
}

/**
 * @note Address: 0x8001706C
 * @note Size: 0xC8
 */
// todo: weak copy
extern "C" CMsgFont* __dt__8CMsgFontFv(CMsgFont* font, s32 flags)
{
	if (font != NULL) {
		font->CMsgFont::~CMsgFont();
		if ((s16)flags > 0)
			::operator delete(font);
	}
	return font;
}

/**
 * @note Address: 0x80017134
 * @note Size: 0x88
 */
// todo: weak copy
extern "C" CObjVec* __dt__7CObjVecFv(CObjVec* obj, s32 flags)
{
	if (obj != NULL) {
		*(void***)((u8*)obj + 0xC) = __vt__7CObjVec; // _0C vtable
		obj->mSpeed.CVec::~CVec();
		obj->mTarget.CVec::~CVec();
		obj->CVec::~CVec();
		if ((s16)flags > 0) {
			::operator delete(obj);
		}
	}
	return obj;
}

/**
 * @note Address: 0x800171BC
 * @note Size: 0x88
 */
// todo: weak copy
extern "C" CJObj* __dt__5CJObjFv(CJObj* obj, s32 flags)
{
	if (obj != NULL) {
		*(void***)((u8*)obj + 0x34) = __vt__5CJObj; // _34 vtable
		obj->mScale.CVec::~CVec();
		obj->mRotate.CVec::~CVec();
		obj->mTranslate.CVec::~CVec();
		if ((s16)flags > 0) {
			::operator delete(obj);
		}
	}
	return obj;
}
