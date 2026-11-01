#ifndef _GAME_COORDINFO_H
#define _GAME_COORDINFO_H

#include "types.h"

// one 0x38 byte entry of the stage's object placement table
struct UnkStageObj {
	s32 _00;       // _00, the object id, negative ends the table
	s32 mPos[3];   // _04, in hundredths
	s32 _10[3];    // _10
	s32 mScale[3]; // _1C, in hundredths
	u8 _28[0x4];   // _28
	s32 _2C;       // _2C
	s32 _30;       // _30, starting anim
	u32 _34;       // _34, flags, 0x80000000 skips the read
};
STATIC_ASSERT(sizeof(UnkStageObj) == 0x38);

// name from __vt__9CoordInfo
class CoordInfo {
public:
	UnkStageObj* _00; // _00
	// _04 vtable

	CoordInfo(void* value)
	    : _00((UnkStageObj*)value)
	{
	}
	virtual ~CoordInfo() { } // _08
};

// an entry of objTable or objTableEx, 0x18 bytes. the kind record an object points at
struct UnkObjData {
	u8 _00[0xC]; // _00
	u32 _0C;     // _0C, shadow size
	u8 _10[0x4]; // _10
	s16 _14;     // _14, category, also which pool in UnkXObjList
	s16 _16;     // _16, item index, some callers test it as flags

	s16 GetCategory() const { return this != NULL ? _14 : 0; }
	s16 GetItem() const { return this != NULL ? _16 : 0; }
};
STATIC_ASSERT(sizeof(UnkObjData) == 0x18);
extern "C" UnkObjData* GetObjData(CoordInfo* info);

// todo: cleanup
inline s32 GetObjDataPool(const UnkObjData* data)
{
	return data != NULL ? data->_14 : 0;
}
inline s32 GetObjDataFlags(const UnkObjData* data)
{
	return data != NULL ? data->_16 : 0;
}

// name from __vt__8AttrInfo
class AttrInfo {
public:
	UnkObjData* _00;  // _00
	UnkStageObj* _04; // _04, the placement CoordInfo points at
	// _08 vtable

	AttrInfo(CoordInfo* info)
	    : _00(GetObjData(info))
	    , _04(info->_00)
	{
	}
	virtual ~AttrInfo() { } // _08
};

extern "C" UnkStageObj* fn_8006F4A0(s32 i);

#endif
