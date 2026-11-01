#include "game/xobj.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__4XObj[];
extern void* __vt__6CJAnim[];
extern void* __vt__9CAnmQueue[];
extern void* __vt__4CEvt[];
CXObjIntp* __dt__9CXObjIntpFv(CXObjIntp* object, s32 flags);
}

// todo: cleanup, stand-ins for dtors retail inlines but our headers keep out of line
struct CJAnimDtor {
	CJArchive mArchive; // _00

	~CJAnimDtor() { *(void***)((u8*)this + sizeof(CJArchive) - 4) = __vt__6CJAnim; }
};

struct CAnmQueueDtor {
	u8 _00[0x84];   // _00
	void** mVtable; // _84

	~CAnmQueueDtor() { mVtable = __vt__9CAnmQueue; }
};

struct CEvtDtor {
	u32 _00;             // _00
	CEvtData mData;      // _04
	u8 _10[0x24 - 0x10]; // _10
	CStack mStack0;      // _24
	CStack mStack1;      // _34
	CStack mStack2;      // _44
	u8 _54[0x64 - 0x54]; // _54
	void** mVtable;      // _64

	~CEvtDtor() { mVtable = __vt__4CEvt; }
};
STATIC_ASSERT(sizeof(CEvtDtor) == sizeof(CEvt));

/**
 * @note Address: 0x8000877C
 * @note Size: 0x168
 */
// todo: weak copy
extern "C" XObj* fn_8000877C(XObj* object, s32 flags)
{
	if (object != NULL) {
		*(void***)((u8*)object + 0x34) = __vt__4XObj;
		((CJAnimDtor*)&object->mAnim)->~CJAnimDtor();
		((CAnmQueueDtor*)&object->mAnmQueue)->~CAnmQueueDtor();
		((CEvtDtor*)&object->mEvt)->~CEvtDtor();
		object->_794.CVec::~CVec();
		object->_264.CVec::~CVec();
		object->mAttr.AttrInfo::~AttrInfo();
		object->mCoord.CoordInfo::~CoordInfo();
		object->mStatus.CStatus64::~CStatus64();
		__dt__9CXObjIntpFv(object, 0);
		if ((s16)flags > 0)
			::operator delete(object);
	}
	return object;
}
