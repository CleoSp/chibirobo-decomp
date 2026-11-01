#include "game/xobj.h"

extern "C" {
extern void* __vt__4XObj[];
extern void* __vt__9CStatus64[];
extern void* __vt__9CoordInfo[];
extern void* __vt__8AttrInfo[];
extern void* __vt__4CVec[];
extern void* __vt__4CEvt[];
extern void* __vt__8CEvtData[];
extern void* __vt__6CStack[];
extern void* __vt__9CAnmQueue[];
extern void* __vt__9CJArchive[];
extern void* __vt__6CJAnim[];
CXObjIntp* fn_8001B150(CXObjIntp* obj);
}

// todo: weak copy, CEvt's call data and stacks are still untyped in evt.h
/**
 * @note Address: 0x8001AFAC
 * @note Size: 0x100
 */
extern "C" XObj* fn_8001AFAC(XObj* obj)
{
	fn_8001B150(obj);
	*(void***)((u8*)obj + 0x34)                          = __vt__4XObj; // _34 vtable
	*(void***)((u8*)&obj->mStatus + 0x8)                 = __vt__9CStatus64;
	*(u32*)((u8*)&obj->mStatus + 0x4)                    = 0;
	*(u32*)&obj->mStatus                                 = 0;
	*(void***)((u8*)&obj->mCoord + 0x4)                  = __vt__9CoordInfo;
	obj->mCoord._00                                      = NULL;
	*(void***)((u8*)&obj->mAttr + 0x8)                   = __vt__8AttrInfo;
	obj->mAttr._00                                       = NULL;
	*(void***)((u8*)&obj->_264 + 0xC)                    = __vt__4CVec;
	*(void***)((u8*)&obj->_794 + 0xC)                    = __vt__4CVec;
	*(void***)((u8*)&obj->mEvt + 0x64)                   = __vt__4CEvt;
	*(void***)((u8*)&obj->mEvt + 0xC)                    = __vt__8CEvtData;
	obj->mEvt._04                                        = NULL;
	obj->mEvt._08                                        = 0;
	*(void***)((u8*)&obj->mEvt + 0x30)                   = __vt__6CStack;
	obj->mEvt._24                                        = NULL;
	*(u32*)obj->mEvt._2C                                 = 0;
	obj->mEvt._28                                        = 0;
	*(void***)((u8*)&obj->mEvt + 0x40)                   = __vt__6CStack;
	obj->mEvt._34                                        = NULL;
	*(u32*)obj->mEvt._3C                                 = 0;
	obj->mEvt._38                                        = 0;
	*(void***)((u8*)&obj->mEvt + 0x50)                   = __vt__6CStack;
	obj->mEvt._44                                        = NULL;
	*(u32*)obj->mEvt._4C                                 = 0;
	obj->mEvt._48                                        = 0;
	*(void***)((u8*)&obj->mAnmQueue + 0x84)              = __vt__9CAnmQueue;
	*(void***)((u8*)&obj->mAnim + sizeof(CJArchive) - 4) = __vt__9CJArchive;
	*(void***)((u8*)&obj->mAnim + sizeof(CJArchive) - 4) = __vt__6CJAnim;
	return obj;
}
