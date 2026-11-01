#include "game/cobj.h"

void operator delete(void* object);
extern "C" {
extern void* __vt__7CObjVec[];
extern void* __vt__5CJObj[];
}

// todo: weak copy
/**
 * @note Address: 0x80017134
 * @note Size: 0x88
 */
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

// todo: weak copy
/**
 * @note Address: 0x800171BC
 * @note Size: 0x88
 */
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
