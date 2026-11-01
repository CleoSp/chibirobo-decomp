#include "game/cjobj.h"

extern "C" {
extern void* __vt__5CJObj[];
extern void* __vt__4CVec[];
}

/**
 * @note Address: 0x80121AB4
 * @note Size: 0x54
 */
// todo: weak copy
extern "C" CJObj* fn_80121AB4(CJObj* object)
{
	*(void***)((u8*)object + 0x34)             = __vt__5CJObj;
	*(void***)((u8*)&object->mTranslate + 0xC) = __vt__4CVec;
	*(void***)((u8*)&object->mRotate + 0xC)    = __vt__4CVec;
	*(void***)((u8*)&object->mScale + 0xC)     = __vt__4CVec;
	object->mJObj                              = NULL;
	object->mTranslate.x                       = 0.0f;
	object->mTranslate.y                       = 0.0f;
	object->mTranslate.z                       = 0.0f;
	object->mRotate.x                          = 0.0f;
	object->mRotate.y                          = 0.0f;
	object->mRotate.z                          = 0.0f;
	object->mScale.x                           = 0.0f;
	object->mScale.y                           = 0.0f;
	object->mScale.z                           = 0.0f;
	return object;
}
