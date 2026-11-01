#include "game/xobjdemo.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__9CXObjIntp[];
extern void* __vt__5CXObj[];
extern void* __vt__8XObjDemo[];
extern void* __vt__7CTexObj[];
CXObj* __dt__5CXObjFv(CXObj* object, s32 flags);
CLight* __dt__6CLightFv(CLight* light, s32 flags);
}

/**
 * @note Address: 0x800088E4
 * @note Size: 0xB4
 */
// todo: weak copy
extern "C" CXObjIntp* __dt__9CXObjIntpFv(CXObjIntp* object, s32 flags)
{
	if (object != NULL) {
		*(void***)((u8*)object + 0x34) = __vt__9CXObjIntp;
		object->mIntp.CJObj::~CJObj();
		__dt__5CXObjFv(object, 0);
		if ((s16)flags > 0)
			::operator delete(object);
	}
	return object;
}

/**
 * @note Address: 0x80008998
 * @note Size: 0x1A0
 */
// todo: weak copy
extern "C" CXObj* __dt__5CXObjFv(CXObj* object, s32 flags)
{
	if (object != NULL) {
		*(void***)((u8*)object + 0x34) = __vt__5CXObj;
		object->mMoveScale.CObjVec::~CObjVec();
		object->mMoveRotate.CObjVec::~CObjVec();
		object->mMoveTranslate.CObjVec::~CObjVec();
		object->mArchive.CJArchive::~CJArchive();
		object->CJObj::~CJObj();
		if ((s16)flags > 0)
			::operator delete(object);
	}
	return object;
}

/**
 * @note Address: 0x80008B38
 * @note Size: 0xF4
 */
// todo: weak copy
extern "C" XObjDemo* __dt__8XObjDemoFv(XObjDemo* demo, s32 flags)
{
	if (demo != NULL) {
		*(void***)((u8*)demo + 0x34) = __vt__8XObjDemo;
		__destroy_arr(demo->mLights, (ConstructorDestructor*)__dt__6CLightFv, sizeof(CLight), 8);
		demo->mCamera.CCamera::~CCamera();
		__dt__4XObjFv(demo, 0);
		if ((s16)flags > 0)
			::operator delete(demo);
	}
	return demo;
}

/**
 * @note Address: 0x80008C2C
 * @note Size: 0x48
 */
// todo: weak copy
// void*, the c++ ~CTexObj the inlined dtors above use has the same name
extern "C" void* __dt__7CTexObjFv(void* tex, s32 flags)
{
	if (tex != NULL) {
		*(void***)((u8*)tex + 0xFC) = __vt__7CTexObj;
		if ((s16)flags > 0)
			::operator delete(tex);
	}
	return tex;
}
