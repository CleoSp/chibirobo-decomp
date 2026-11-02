#include "game/cobj.h"

extern "C" {
extern void* __vt__9CXObjIntp[];
extern void* __vt__5CJObj[];
extern void* __vt__4CVec[];
extern void* __vt__5CXObj[];
extern void* __vt__9CJArchive[];
extern void* __vt__7CObjVec[];
CXObj* __ct__5CXObjFv(CXObj* obj);
}

// todo: weak copy
/**
 * @note Address: 0x8001B150
 * @note Size: 0x8C
 */
extern "C" CXObjIntp* __ct__9CXObjIntpFv(CXObjIntp* obj)
{
	__ct__5CXObjFv(obj);
	*(void***)((u8*)obj + 0x34)                   = __vt__9CXObjIntp; // _34 vtable
	*(void***)((u8*)&obj->mIntp + 0x34)           = __vt__5CJObj;
	*(void***)((u8*)&obj->mIntp.mTranslate + 0xC) = __vt__4CVec;
	*(void***)((u8*)&obj->mIntp.mRotate + 0xC)    = __vt__4CVec;
	*(void***)((u8*)&obj->mIntp.mScale + 0xC)     = __vt__4CVec;
	obj->mIntp.mJObj                              = NULL;
	obj->mIntp.mTranslate.x                       = 0.0f;
	obj->mIntp.mTranslate.y                       = 0.0f;
	obj->mIntp.mTranslate.z                       = 0.0f;
	obj->mIntp.mRotate.x                          = 0.0f;
	obj->mIntp.mRotate.y                          = 0.0f;
	obj->mIntp.mRotate.z                          = 0.0f;
	obj->mIntp.mScale.x                           = 0.0f;
	obj->mIntp.mScale.y                           = 0.0f;
	obj->mIntp.mScale.z                           = 0.0f;
	return obj;
}

// todo: weak copy
/**
 * @note Address: 0x8001B1DC
 * @note Size: 0x110
 */
extern "C" CXObj* __ct__5CXObjFv(CXObj* obj)
{
	*(void***)((u8*)obj + 0x34)                             = __vt__5CJObj; // _34 vtable
	*(void***)((u8*)&obj->mTranslate + 0xC)                 = __vt__4CVec;
	*(void***)((u8*)&obj->mRotate + 0xC)                    = __vt__4CVec;
	*(void***)((u8*)&obj->mScale + 0xC)                     = __vt__4CVec;
	obj->mJObj                                              = NULL;
	obj->mTranslate.x                                       = 0.0f;
	obj->mTranslate.y                                       = 0.0f;
	obj->mTranslate.z                                       = 0.0f;
	obj->mRotate.x                                          = 0.0f;
	obj->mRotate.y                                          = 0.0f;
	obj->mRotate.z                                          = 0.0f;
	obj->mScale.x                                           = 0.0f;
	obj->mScale.y                                           = 0.0f;
	obj->mScale.z                                           = 0.0f;
	*(void***)((u8*)obj + 0x34)                             = __vt__5CXObj;
	*(void***)((u8*)&obj->mArchive + sizeof(CJArchive) - 4) = __vt__9CJArchive;
	*(void***)((u8*)&obj->mMoveTranslate + 0xC)             = __vt__4CVec;
	*(void***)((u8*)&obj->mMoveTranslate + 0xC)             = __vt__7CObjVec;
	*(void***)((u8*)&obj->mMoveTranslate.mTarget + 0xC)     = __vt__4CVec;
	*(void***)((u8*)&obj->mMoveTranslate.mSpeed + 0xC)      = __vt__4CVec;
	*(void***)((u8*)&obj->mMoveRotate + 0xC)                = __vt__4CVec;
	*(void***)((u8*)&obj->mMoveRotate + 0xC)                = __vt__7CObjVec;
	*(void***)((u8*)&obj->mMoveRotate.mTarget + 0xC)        = __vt__4CVec;
	*(void***)((u8*)&obj->mMoveRotate.mSpeed + 0xC)         = __vt__4CVec;
	*(void***)((u8*)&obj->mMoveScale + 0xC)                 = __vt__4CVec;
	*(void***)((u8*)&obj->mMoveScale + 0xC)                 = __vt__7CObjVec;
	*(void***)((u8*)&obj->mMoveScale.mTarget + 0xC)         = __vt__4CVec;
	*(void***)((u8*)&obj->mMoveScale.mSpeed + 0xC)          = __vt__4CVec;
	obj->mMoveTranslate.x                                   = 0.0f;
	obj->mMoveTranslate.y                                   = 0.0f;
	obj->mMoveTranslate.z                                   = 0.0f;
	obj->mMoveTranslate.mTarget.x                           = 0.0f;
	obj->mMoveTranslate.mTarget.y                           = 0.0f;
	obj->mMoveTranslate.mTarget.z                           = 0.0f;
	obj->mMoveTranslate.mSpeed.x                            = 0.0f;
	obj->mMoveTranslate.mSpeed.y                            = 0.0f;
	obj->mMoveTranslate.mSpeed.z                            = 0.0f;
	obj->mMoveRotate.x                                      = 0.0f;
	obj->mMoveRotate.y                                      = 0.0f;
	obj->mMoveRotate.z                                      = 0.0f;
	obj->mMoveRotate.mTarget.x                              = 0.0f;
	obj->mMoveRotate.mTarget.y                              = 0.0f;
	obj->mMoveRotate.mTarget.z                              = 0.0f;
	obj->mMoveRotate.mSpeed.x                               = 0.0f;
	obj->mMoveRotate.mSpeed.y                               = 0.0f;
	obj->mMoveRotate.mSpeed.z                               = 0.0f;
	obj->mMoveScale.x                                       = 0.0f;
	obj->mMoveScale.y                                       = 0.0f;
	obj->mMoveScale.z                                       = 0.0f;
	obj->mMoveScale.mTarget.x                               = 0.0f;
	obj->mMoveScale.mTarget.y                               = 0.0f;
	obj->mMoveScale.mTarget.z                               = 0.0f;
	obj->mMoveScale.mSpeed.x                                = 0.0f;
	obj->mMoveScale.mSpeed.y                                = 0.0f;
	obj->mMoveScale.mSpeed.z                                = 0.0f;
	return obj;
}
