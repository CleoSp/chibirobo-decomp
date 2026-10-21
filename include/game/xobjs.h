#ifndef _GAME_XOBJS_H
#define _GAME_XOBJS_H

#include "game/cobj.h"

// name from __vt__9CoordInfo
class CoordInfo {
public:
	void* _00; // _00
	// _04 vtable

	CoordInfo(void* value)
	    : _00(value)
	{
	}
	virtual ~CoordInfo() { } // _08
};

// one 0x38 byte entry of the stage's object placement table
struct UnkStageObj {
	s32 _00;      // _00, the object id, negative ends the table
	u8 _04[0x30]; // _04
	u32 _34;      // _34, flags, 0x80000000 skips the read
};

extern "C" UnkStageObj* fn_8006F4A0(s32 i);

class UnkXObjReadObj : public CXObjIntp {
public:
	u64 _228;      // _228, flags, 0x0040000000000000 while a read is pending
	u8 _230[0x28]; // _230
	s32 _258;      // _258, index in its pool

	virtual void vf90();                      // _90
	virtual void vf94();                      // _94
	virtual void vf98();                      // _98
	virtual BOOL vf9C(const CoordInfo& info); // _9C, called until it returns true
};
struct UnkXObjList;
// index of obj in the list, -1 when it isn't there
extern "C" s32 fn_8013265C(UnkXObjList* list, CXObj* obj);
extern "C" UnkXObjReadObj* fn_80132698(UnkXObjList* list, CoordInfo* info, s32 unused, BOOL async);

#if defined(VERSION_JP)
typedef char UnkXObjReadObj_size_check[sizeof(UnkXObjReadObj) == 0x258 ? 1 : -1];
#else
typedef char UnkXObjReadObj_size_check[sizeof(UnkXObjReadObj) == 0x260 ? 1 : -1];
#endif

#endif
