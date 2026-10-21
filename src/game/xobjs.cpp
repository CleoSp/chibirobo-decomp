#include "game/xobjs.h"
#include "game/cgame.h"
#include "Dolphin/OS/OSMessage.h"
#include "Dolphin/OS/OSAlloc.h"
#include "Dolphin/OS/OSThread.h"

// Dolphin/os.h resolves to the hsd copy, whose OSHeapHandle clashes with OSAlloc.h
extern "C" void OSPanic(const char* file, int line, const char* message, ...);

struct UnkXObjList;
extern "C" void fn_80132950(UnkXObjList* list);

// one request slot in a reader
struct UnkXObjReadReq {
	void* _00; // _00, the object, null when the slot is free
	void* _04; // _04
};

// name from __vt__9CMsgQueue
class CMsgQueue {
public:
	OSMessageQueue _00; // _00
	OSMessage* _20;     // _20
	// _24 vtable

	virtual void vf08();          // _08
	virtual void vf0C(s32 count); // _0C
};

// class name from RTTI
class XObjRead {
public:
	OSThread _00; // _00
	void* _318;   // _318
	u32 _31C;     // _31C
	// _320 vtable

	virtual void vf08(); // _08
	// _0C
	virtual s32 vf0C(OSThreadStartFunction func, void* param, u32 stackSize, OSPriority priority, u16 attr);
	virtual void vf10();                    // _10
	virtual void vf14();                    // _14
	virtual void vf18(UnkXObjReadReq* req); // _18

	u32 _324;                 // _324
	CMsgQueue _328;           // _328
	UnkXObjReadReq _350[0x8]; // _350

	BOOL ReadStart(void* obj, void* list)
	{
		for (s32 i = 0; i < 8; i++) {
			if (_350[i]._00 == NULL) {
				_350[i]._00 = obj;
				_350[i]._04 = list;
				OSSendMessage(&_328._00, &_350[i], OS_MESSAGE_BLOCK);
				return TRUE;
			}
		}
		OSPanic(__FILE__, 356, "%s%s", "no more space for XObjRead thread!\n", __FUNCTION__);
		return FALSE;
	}
};

/**
 * @note Address: 0x80132040
 * @note Size: 0xB4
 */
extern "C" BOOL fn_80132040(XObjRead* self, UnkXObjReadObj* obj, UnkXObjList* list)
{
	obj->_228 |= 0x0080000000000000ULL;
	return self->ReadStart(obj, list);
}

/**
 * @note Address: 0x801320F4
 * @note Size: 0x4C
 */
extern XObjRead lbl_80380748; // 0x59A8 bytes, some XObjRead subclass

// reader thread entry
extern "C" void fn_801320F4(void* arg)
{
	OSMessage msg;
	while (true) {
		OSReceiveMessage(&lbl_80380748._328._00, &msg, OS_MESSAGE_BLOCK);
		XObjRead* self = &lbl_80380748;
		self->vf18((UnkXObjReadReq*)msg);
	}
}

/**
 * @note Address: 0x80132140
 * @note Size: 0x4
 */
// XObjRead::vf18, XObjReadAll overrides it
extern "C" void fn_80132140(void* self)
{
}

/**
 * @note Address: 0x80132144
 * @note Size: 0xB8
 */
extern "C" BOOL fn_80132144(UnkXObjReadObj* obj, UnkXObjList* list)
{
	obj->_228 |= 0x0080000000000000ULL;
	return lbl_80380748.ReadStart(obj, list);
}

/**
 * @note Address: 0x801321FC
 * @note Size: 0xA8
 */
// vf18 for the requests fn_801322A4 posts
extern "C" void fn_801321FC(void* self, UnkXObjReadReq* req)
{
	UnkXObjReadObj* obj = (UnkXObjReadObj*)req->_00;
	CoordInfo info(req->_04);
	while (!obj->vf9C(info)) {
		OSYieldThread();
	}
	req->_00 = NULL;
	obj->_228 &= ~0x0040000000000000ULL;
}

/**
 * @note Address: 0x801322A4
 * @note Size: 0xB4
 */
extern "C" BOOL fn_801322A4(XObjRead* self, UnkXObjReadObj* obj, UnkXObjList* list)
{
	obj->_228 |= 0x0040000000000000ULL;
	return self->ReadStart(obj, list);
}

/**
 * @note Address: 0x80132358
 * @note Size: 0x4C
 */
extern XObjRead lbl_803803A8; // 0x3A0 bytes

extern "C" void fn_80132358(void* arg)
{
	OSMessage msg;
	while (true) {
		OSReceiveMessage(&lbl_803803A8._328._00, &msg, OS_MESSAGE_BLOCK);
		XObjRead* self = &lbl_803803A8;
		self->vf18((UnkXObjReadReq*)msg);
	}
}

/**
 * @note Address: 0x801323A4
 * @note Size: 0x38
 */
// XObjReadAll::vf18
extern "C" void fn_801323A4(void* self, UnkXObjReadReq* req)
{
	fn_80132950((UnkXObjList*)req->_04);
	req->_00 = NULL;
}

/**
 * @note Address: 0x801323DC
 * @note Size: 0x4C
 */
extern XObjRead lbl_80380008; // 0x3A0 bytes

extern "C" void fn_801323DC(void* arg)
{
	OSMessage msg;
	while (true) {
		OSReceiveMessage(&lbl_80380008._328._00, &msg, OS_MESSAGE_BLOCK);
		XObjRead* self = &lbl_80380008;
		self->vf18((UnkXObjReadReq*)msg);
	}
}

/**
 * @note Address: 0x80132428
 * @note Size: 0x40
 */
extern "C" void fn_80132428(XObjRead* self)
{
	OSMessage msg;
	while (true) {
		OSReceiveMessage(&self->_328._00, &msg, OS_MESSAGE_BLOCK);
		self->vf18((UnkXObjReadReq*)msg);
	}
}

/**
 * @note Address: 0x80132468
 * @note Size: 0x9C
 */
// out of line XObjRead::ReadStart
extern "C" BOOL fn_80132468(XObjRead* self, void* obj, UnkXObjList* list)
{
	return self->ReadStart(obj, list);
}

// fn_80132468
/*
    stwu     r1, -0x10(r1)
    mflr     r0
    mr       r8, r3
    li       r6, 0x0
    stw      r0, 0x14(r1)
    li       r0, 0x8
    mtctr    r0
.L_80132484:
    lwz      r0, 0x350(r3)
    cmplwi   r0, 0x0
    bne      .L_801324BC
    slwi     r7, r6, 3
    addi     r3, r8, 0x328
    add      r6, r8, r7
    stw      r4, 0x350(r6)
    addi     r4, r7, 0x350
    add      r4, r8, r4
    stw      r5, 0x354(r6)
    li       r5, 0x1
    bl       OSSendMessage
    li       r3, 0x1
    b        .L_801324F4
.L_801324BC:
    addi     r3, r3, 0x8
    addi     r6, r6, 0x1
    bdnz     .L_80132484
    lis      r3, lbl_8020C458@ha
    lis      r6, lbl_8020C464@ha
    lis      r5, lbl_8022DF60@ha
    li       r4, 0x164
    addi     r7, r5, lbl_8022DF60@l
    addi     r3, r3, lbl_8020C458@l
    addi     r6, r6, lbl_8020C464@l
    li       r5, lbl_8065E828@sda21
    crclr    cr1eq
    bl       OSPanic
    li       r3, 0x0
.L_801324F4:
    lwz      r0, 0x14(r1)
    mtlr     r0
    addi     r1, r1, 0x10
    blr
*/

/**
 * @note Address: 0x80132504
 * @note Size: 0xC8
 */
// XObjRead::vf0C
extern "C" s32 fn_80132504(XObjRead* self, OSThreadStartFunction func, void* param, u32 stackSize, OSPriority priority, u16 attr)
{
	self->_328.vf0C(8);
	for (s32 i = 0; i < 8; i++) {
		self->_350[i]._00 = NULL;
	}
	self->_318 = OSAlloc(stackSize);
	self->_31C = stackSize;
	if (OSCreateThread(&self->_00, func, param, (u8*)self->_318 + self->_31C, self->_31C, priority, attr)) {
		self->_31C = stackSize;
	}
	OSResumeThread(&self->_00);
	return 0;
}

/**
 * @note Address: 0x801325CC
 * @note Size: 0x54
 */
// CMsgQueue::vf0C
extern "C" void fn_801325CC(CMsgQueue* self, s32 count)
{
	self->_20 = (OSMessage*)OSAlloc(count * sizeof(OSMessage));
	OSInitMessageQueue(&self->_00, self->_20, count);
}

/**
 * @note Address: 0x80132620
 * @note Size: 0x3C
 */
// first free archive in a pool of 0x400
extern "C" CObjArchive* fn_80132620(CObjArchive* archives)
{
	for (s32 i = 0; i < 0x400; i++) {
		if (archives[i].mFile.mBuffer == NULL) {
			return &archives[i];
		}
	}
	return NULL;
}

/**
 * @note Address: 0x8013265C
 * @note Size: 0x3C
 */
// one pool of same sized objects
struct UnkXObjPool {
	u8* _00;         // _00
	s32 _04;         // _04, objects handed out so far
	s32 _08;         // _08, count
	const char* _0C; // _0C, the pool's name, printed when it runs out
	s32 _10;         // _10, size of one object

	UnkXObjReadObj* Get(s32 i) { return (UnkXObjReadObj*)(_00 + i * _10); }

	s32 Find(u64 mask, u64 value)
	{
		for (s32 i = 0; i < _08; i++) {
			if ((Get(i)->_228 & mask) == value) {
				return i;
			}
		}
		return -1;
	}
};

struct UnkXObjList {
	UnkXObjPool _00[0x17];       // _00
	UnkXObjReadObj* _1CC[0x400]; // _1CC
	s32 _11CC;                   // _11CC, entries used in _1CC
	s32 _11D0;                   // _11D0
	s32 _11D4;                   // _11D4, fn_80132B98's state, 0 when idle

	inline void ReadAll();
};

extern "C" s32 fn_8013265C(UnkXObjList* list, CXObj* obj)
{
	for (s32 i = 0; i < list->_11CC; i++) {
		if (list->_1CC[i] == obj) {
			return i;
		}
	}
	return -1;
}

/**
 * @note Address: 0x80132698
 * @note Size: 0x230
 */
// an entry of objTable or objTableEx, 0x18 bytes
struct UnkObjData {
	u8 _00[0x14]; // _00
	s16 _14;      // _14, which pool in UnkXObjList
};

extern "C" UnkObjData* GetObjData(CoordInfo* info);

// name from __vt__8AttrInfo
class AttrInfo {
public:
	UnkObjData* _00; // _00
	void* _04;       // _04
	// _08 vtable

	AttrInfo(CoordInfo* info)
	    : _00(GetObjData(info))
	    , _04(info->_00)
	{
	}
	virtual ~AttrInfo() { } // _08
};

extern "C" UnkXObjReadObj* fn_80132698(UnkXObjList* list, CoordInfo* info, s32 unused, BOOL async)
{
	UnkXObjReadObj* obj;
	AttrInfo attr(info);
	s16 pool = attr._00 ? attr._00->_14 : 0;

	s32 idx = list->_00[pool].Find(1, 0);
	if (idx >= 0) {
		obj = list->_00[pool].Get(idx);
		if (idx >= list->_00[pool]._04) {
			list->_1CC[list->_11CC]       = obj;
			list->_1CC[list->_11CC]->_258 = list->_00[pool]._04;
			list->_00[pool]._04++;
			list->_11CC++;
		}
		if (async) {
			obj->_228 |= 0x0040000000000000ULL;
			lbl_803803A8.ReadStart(obj, info->_00);
		} else {
			while (!obj->vf9C(*info)) { }
		}
	} else {
		obj = NULL;
	}
	return obj;
}

// fn_80132698
/*
    stwu     r1, -0x30(r1)
    mflr     r0
    lis      r5, __vt__8AttrInfo@ha
    stw      r0, 0x34(r1)
    addi     r0, r5, __vt__8AttrInfo@l
    stw      r31, 0x2c(r1)
    stw      r30, 0x28(r1)
    mr       r30, r6
    stw      r29, 0x24(r1)
    mr       r29, r4
    stw      r28, 0x20(r1)
    mr       r28, r3
    mr       r3, r29
    stw      r0, 0x10(r1)
    bl       GetObjData
    lwz      r0, 0x0(r29)
    cmplwi   r3, 0x0
    stw      r3, 0x8(r1)
    stw      r0, 0xc(r1)
    beq      .L_801326F0
    lha      r0, 0x14(r3)
    b        .L_801326F4
.L_801326F0:
    li       r0, 0x0
.L_801326F4:
    extsh    r0, r0
    li       r9, 0x0
    mulli    r11, r0, 0x14
    li       r7, 0x1
    mr       r6, r9
    mr       r5, r9
    add      r10, r28, r11
    mr       r4, r9
    lwz      r0, 0x8(r10)
    mtctr    r0
    cmpwi    r0, 0x0
    ble      .L_80132764
.L_80132724:
    lwz      r0, 0x10(r10)
    lwz      r8, 0x0(r10)
    mullw    r3, r9, r0
    addi     r0, r3, 0x228
    add      r3, r8, r0
    lwz      r0, 0x0(r3)
    lwz      r3, 0x4(r3)
    and      r0, r0, r6
    and      r3, r3, r7
    xor      r3, r3, r5
    xor      r0, r0, r4
    or.      r0, r3, r0
    bne      .L_8013275C
    b        .L_80132768
.L_8013275C:
    addi     r9, r9, 0x1
    bdnz     .L_80132724
.L_80132764:
    li       r9, -0x1
.L_80132768:
    cmpwi    r9, 0x0
    blt      .L_80132894
    add      r5, r28, r11
    lwz      r4, 0x0(r10)
    lwz      r3, 0x10(r5)
    lwz      r0, 0x4(r5)
    mullw    r3, r9, r3
    cmpw     r9, r0
    add      r31, r4, r3
    blt      .L_801327D0
    lwz      r0, 0x11cc(r28)
    slwi     r0, r0, 2
    add      r3, r28, r0
    stw      r31, 0x1cc(r3)
    lwz      r0, 0x11cc(r28)
    lwz      r4, 0x4(r5)
    slwi     r0, r0, 2
    add      r3, r28, r0
    lwz      r3, 0x1cc(r3)
    stw      r4, 0x258(r3)
    lwz      r3, 0x4(r5)
    addi     r0, r3, 0x1
    stw      r0, 0x4(r5)
    lwz      r3, 0x11cc(r28)
    addi     r0, r3, 0x1
    stw      r0, 0x11cc(r28)
.L_801327D0:
    cmpwi    r30, 0x0
    beq      .L_80132870
    lwz      r4, 0x228(r31)
    lis      r3, lbl_803803A8@ha
    lis      r0, 0x40
    lwz      r7, 0x0(r29)
    addi     r6, r3, lbl_803803A8@l
    or       r3, r4, r0
    li       r0, 0x8
    stw      r3, 0x228(r31)
    li       r4, 0x0
    mtctr    r0
.L_80132800:
    lwz      r0, 0x350(r6)
    cmplwi   r0, 0x0
    bne      .L_80132838
    lis      r3, lbl_803803A8@ha
    slwi     r0, r4, 3
    addi     r3, r3, lbl_803803A8@l
    li       r5, 0x1
    add      r6, r3, r0
    stw      r31, 0x350(r6)
    addi     r3, r3, 0x328
    addi     r4, r6, 0x350
    stw      r7, 0x354(r6)
    bl       OSSendMessage
    b        .L_80132898
.L_80132838:
    addi     r6, r6, 0x8
    addi     r4, r4, 0x1
    bdnz     .L_80132800
    lis      r3, lbl_8020C458@ha
    lis      r6, lbl_8020C464@ha
    lis      r5, lbl_8022DF60@ha
    li       r4, 0x164
    addi     r7, r5, lbl_8022DF60@l
    addi     r3, r3, lbl_8020C458@l
    addi     r6, r6, lbl_8020C464@l
    li       r5, lbl_8065E828@sda21
    crclr    cr1eq
    bl       OSPanic
    b        .L_80132898
.L_80132870:
    mr       r3, r31
    mr       r4, r29
    lwz      r12, 0x34(r31)
    lwz      r12, 0x9c(r12)
    mtctr    r12
    bctrl
    cmpwi    r3, 0x0
    beq      .L_80132870
    b        .L_80132898
.L_80132894:
    li       r31, 0x0
.L_80132898:
    lis      r4, __vt__8AttrInfo@ha
    mr       r3, r31
    addi     r0, r4, __vt__8AttrInfo@l
    stw      r0, 0x10(r1)
    lwz      r31, 0x2c(r1)
    lwz      r30, 0x28(r1)
    lwz      r29, 0x24(r1)
    lwz      r28, 0x20(r1)
    lwz      r0, 0x34(r1)
    mtlr     r0
    addi     r1, r1, 0x30
    blr
*/

/**
 * @note Address: 0x801328C8
 * @note Size: 0x88
 */
extern "C" void fn_801328C8(UnkXObjList* list)
{
	for (s32 i = 0; i < list->_11CC; i++) {
		list->_1CC[i]->Release();
		list->_1CC[i] = NULL;
	}
	list->_11CC = 0;
	list->_11D0 = 0;
}

/**
 * @note Address: 0x80132950
 * @note Size: 0x248
 */
// grabs an object from its pool for every stage entry, then reads them in order
inline void UnkXObjList::ReadAll()
{
	for (s32 i = 0; i < (s32)ARRAY_SIZE(_00); i++) {
		_00[i]._04 = 0;
	}

	s32 i;
	for (i = 0; fn_8006F4A0(i)->_00 >= 0; i++) {
		CoordInfo info(fn_8006F4A0(i));
		if (_1CC[i] == NULL) {
			s16 idx = GetObjData(&info)->_14;
			if (_00[idx]._04 >= _00[idx]._08) {
				OSPanic(__FILE__, 161, "%s%s", fn_801911B4("%s:: cnt=%d, max=%d\n", _00[idx]._0C, _00[idx]._04, _00[idx]._08),
				        __FUNCTION__);
			}
			_1CC[i]       = _00[idx].Get(_00[idx]._04);
			_1CC[i]->_258 = _00[idx]._04;
			_00[idx]._04++;
		}
	}
	_11CC = i;
	_11D0 = 0;

	for (s32 i = 0; i < _11CC; i++) {
		UnkStageObj* obj = fn_8006F4A0(i);
		CoordInfo info(obj);
		if ((obj->_34 & 0x80000000ULL) == 0) {
			while (!_1CC[_11D0]->vf9C(info)) { }
		}
		_11D0++;
		if (lbl_8025C400._3330 != 0) {
			break;
		}
	}
	_11D4 = 2;
}

// out of line UnkXObjList::ReadAll
extern "C" void fn_80132950(UnkXObjList* list)
{
	list->ReadAll();
}

// fn_80132950
/*
    stwu     r1, -0x30(r1)
    mflr     r0
    li       r4, 0x0
    li       r5, 0x15
    stw      r0, 0x34(r1)
    subfic   r0, r5, 0x17
    stmw     r27, 0x1c(r1)
    mr       r27, r3
    stw      r4, 0x4(r3)
    mulli    r3, r5, 0x14
    stw      r4, 0x18(r27)
    add      r3, r27, r3
    stw      r4, 0x2c(r27)
    stw      r4, 0x40(r27)
    stw      r4, 0x54(r27)
    stw      r4, 0x68(r27)
    stw      r4, 0x7c(r27)
    stw      r4, 0x90(r27)
    stw      r4, 0xa4(r27)
    stw      r4, 0xb8(r27)
    stw      r4, 0xcc(r27)
    stw      r4, 0xe0(r27)
    stw      r4, 0xf4(r27)
    stw      r4, 0x108(r27)
    stw      r4, 0x11c(r27)
    stw      r4, 0x130(r27)
    stw      r4, 0x144(r27)
    stw      r4, 0x158(r27)
    stw      r4, 0x16c(r27)
    stw      r4, 0x180(r27)
    stw      r4, 0x194(r27)
    mtctr    r0
    cmpwi    r5, 0x17
    bge      .L_801329E4
.L_801329D8:
    stw      r4, 0x4(r3)
    addi     r3, r3, 0x14
    bdnz     .L_801329D8
.L_801329E4:
    lis      r3, __vt__9CoordInfo@ha
    mr       r31, r27
    addi     r30, r3, __vt__9CoordInfo@l
    li       r28, 0x0
    b        .L_80132AA8
.L_801329F8:
    mr       r3, r28
    bl       fn_8006F4A0
    stw      r30, 0x14(r1)
    stw      r3, 0x10(r1)
    lwz      r0, 0x1cc(r31)
    cmplwi   r0, 0x0
    bne      .L_80132A9C
    addi     r3, r1, 0x10
    bl       GetObjData
    lha      r0, 0x14(r3)
    mulli    r0, r0, 0x14
    add      r29, r27, r0
    lwz      r5, 0x4(r29)
    lwz      r6, 0x8(r29)
    cmpw     r5, r6
    blt      .L_80132A6C
    lis      r3, lbl_8020C488@ha
    lwz      r4, 0xc(r29)
    addi     r3, r3, lbl_8020C488@l
    crclr    cr1eq
    bl       fn_801911B4
    lis      r4, lbl_8020C458@ha
    mr       r6, r3
    addi     r3, r4, lbl_8020C458@l
    li       r5, lbl_8065E828@sda21
    li       r4, 0xa1
    li       r7, lbl_8065A9A8@sda21
    crclr    cr1eq
    bl       OSPanic
.L_80132A6C:
    lwz      r3, 0x4(r29)
    lwz      r0, 0x10(r29)
    lwz      r4, 0x0(r29)
    mullw    r0, r3, r0
    add      r0, r4, r0
    stw      r0, 0x1cc(r31)
    lwz      r0, 0x4(r29)
    lwz      r3, 0x1cc(r31)
    stw      r0, 0x258(r3)
    lwz      r3, 0x4(r29)
    addi     r0, r3, 0x1
    stw      r0, 0x4(r29)
.L_80132A9C:
    stw      r30, 0x14(r1)
    addi     r31, r31, 0x4
    addi     r28, r28, 0x1
.L_80132AA8:
    mr       r3, r28
    bl       fn_8006F4A0
    lwz      r0, 0x0(r3)
    cmpwi    r0, 0x0
    bge      .L_801329F8
    stw      r28, 0x11cc(r27)
    li       r0, 0x0
    lis      r4, lbl_8025C400@ha
    lis      r3, __vt__9CoordInfo@ha
    stw      r0, 0x11d0(r27)
    addi     r30, r4, lbl_8025C400@l
    addi     r31, r3, __vt__9CoordInfo@l
    li       r28, 0x0
    lis      r29, 0x8000
    b        .L_80132B70
.L_80132AE4:
    mr       r3, r28
    bl       fn_8006F4A0
    stw      r31, 0xc(r1)
    li       r4, 0x0
    and      r0, r4, r4
    stw      r3, 0x8(r1)
    xor      r0, r0, r4
    lwz      r3, 0x34(r3)
    and      r3, r3, r29
    xor      r3, r3, r4
    or.      r0, r3, r0
    bne      .L_80132B40
.L_80132B14:
    lwz      r0, 0x11d0(r27)
    addi     r4, r1, 0x8
    slwi     r3, r0, 2
    addi     r0, r3, 0x1cc
    lwzx     r3, r27, r0
    lwz      r12, 0x34(r3)
    lwz      r12, 0x9c(r12)
    mtctr    r12
    bctrl
    cmpwi    r3, 0x0
    beq      .L_80132B14
.L_80132B40:
    lwz      r3, 0x11d0(r27)
    addi     r0, r3, 0x1
    stw      r0, 0x11d0(r27)
    lwz      r0, 0x3330(r30)
    cmpwi    r0, 0x0
    beq      .L_80132B68
    lis      r3, __vt__9CoordInfo@ha
    addi     r0, r3, __vt__9CoordInfo@l
    stw      r0, 0xc(r1)
    b        .L_80132B7C
.L_80132B68:
    stw      r31, 0xc(r1)
    addi     r28, r28, 0x1
.L_80132B70:
    lwz      r0, 0x11cc(r27)
    cmpw     r28, r0
    blt      .L_80132AE4
.L_80132B7C:
    li       r0, 0x2
    stw      r0, 0x11d4(r27)
    lmw      r27, 0x1c(r1)
    lwz      r0, 0x34(r1)
    mtlr     r0
    addi     r1, r1, 0x30
    blr
*/

/**
 * @note Address: 0x80132B98
 * @note Size: 0x170
 */

extern "C" BOOL fn_800B8EC4(XObj* player);
extern "C" BOOL fn_800B78FC(XObj* player);
extern "C" BOOL fn_800B9758(XObj* player);

// steps through the stage read and the player setup calls, true when done
extern "C" u8 fn_80132B98(UnkXObjList* list)
{
	if (list->_11D4 == 0) {
		lbl_80380008.ReadStart((void*)1, list);
		list->_11D4 = 1;
	}
	if (list->_11D4 == 2) {
		list->_11D4 = 3;
	}
	if (list->_11D4 == 3 && fn_800B8EC4(lbl_8025C400.mPlayer)) {
		list->_11D4 = 4;
	}
	if (list->_11D4 == 4 && fn_800B78FC(lbl_8025C400.mPlayer)) {
		list->_11D4 = 5;
	}
	if (list->_11D4 == 5 && fn_800B9758(lbl_8025C400.mPlayer)) {
		list->_11D4 = 0;
	}
	return list->_11D4 == 0;
}
