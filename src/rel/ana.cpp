#include "Version.h"
#include "game/hsd_jobj.h"
#include "game/ut.h"
#include "game/cgame.h"

extern "C" {
void OSReport(const char* msg, ...);
f32 PSVECDistance(const Vec* a, const Vec* b);
}

struct UnkIdData {
	s32 mId; // _00
};

struct UnkKindData {
	u8 _00[0x14]; // _00
	s16 mKind;    // _14
};

// game object class from the dol
class XObj {
public:
	HSD_JObj* mJObj; // _00
	u8 _04[0x30];    // _04
	// _34 vtable
	virtual void vf08();  // _08
	virtual void vf0C();  // _0C
	virtual void vf10();  // _10
	virtual void vf14();  // _14
	virtual void vf18();  // _18
	virtual void vf1C();  // _1C
	virtual void vf20();  // _20
	virtual void vf24();  // _24
	virtual void vf28();  // _28
	virtual void vf2C();  // _2C
	virtual void vf30();  // _30
	virtual void vf34();  // _34
	virtual void vf38();  // _38
	virtual void vf3C();  // _3C
	virtual void vf40();  // _40
	virtual void vf44();  // _44
	virtual void vf48();  // _48
	virtual void vf4C();  // _4C
	virtual void vf50();  // _50
	virtual void vf54();  // _54
	virtual void vf58();  // _58
	virtual void vf5C();  // _5C
	virtual void vf60();  // _60
	virtual void vf64();  // _64
	virtual void vf68();  // _68
	virtual void vf6C();  // _6C
	virtual void vf70();  // _70
	virtual void vf74();  // _74
	virtual void vf78();  // _78
	virtual void vf7C();  // _7C
	virtual void vf80();  // _80
	virtual void vf84();  // _84
	virtual void vf88();  // _88
	virtual void vf8C();  // _8C
	virtual void vf90();  // _90
	virtual void vf94();  // _94
	virtual void vf98();  // _98
	virtual void vf9C();  // _9C
	virtual void vfA0();  // _A0
	virtual void vfA4();  // _A4
	virtual void vfA8();  // _A8
	virtual void vfAC();  // _AC
	virtual void vfB0();  // _B0
	virtual void vfB4();  // _B4
	virtual void vfB8();  // _B8
	virtual void vfBC();  // _BC
	virtual void vfC0();  // _C0
	virtual void vfC4();  // _C4
	virtual void vfC8();  // _C8
	virtual void vfCC();  // _CC
	virtual void vfD0();  // _D0
	virtual void vfD4();  // _D4
	virtual void vfD8();  // _D8
	virtual void vfDC();  // _DC
	virtual void vfE0();  // _E0
	virtual void vfE4();  // _E4
	virtual void vfE8();  // _E8
	virtual void vfEC();  // _EC
	virtual void vfF0();  // _F0
	virtual void vfF4();  // _F4
	virtual void vfF8();  // _F8
	virtual void vfFC();  // _FC
	virtual void vf100(); // _100
	virtual void vf104(); // _104
	virtual void vf108(); // _108
	virtual void vf10C(); // _10C
	virtual void vf110(); // _110
	virtual void vf114(); // _114
	virtual void vf118(); // _118
	virtual void vf11C(); // _11C
	virtual void vf120(); // _120
	virtual void vf124(); // _124
	virtual void vf128(); // _128
	virtual void vf12C(); // _12C
	virtual void vf130(); // _130
	virtual void vf134(); // _134
	virtual void vf138(); // _138
	virtual void vf13C(); // _13C
	virtual void vf140(); // _140
	virtual void vf144(); // _144
	virtual u32 vf148();  // _148

#if defined(VERSION_JP)
	u8 _38[0xC0 - 0x38]; // _38
#else
	u8 _38[0xC8 - 0x38]; // _38
#endif
	Vec mPos;             // _C8 (JP: _C0)
	u8 _D4[0x228 - 0xD4]; // _D4 (JP: _CC)
	u64 mFlags;           // _228 (JP: _220)
	u8 _230[0x8];         // _230 (JP: _228)
	UnkIdData* mIdData;   // _238 (JP: _230)
#if defined(VERSION_JP)
	u8 _234[0x4]; // _234
#else
	u8 _23C[0x4]; // _23C
#endif
	UnkKindData* mKind; // _240 (JP: _238)
#if defined(VERSION_JP)
	u8 _23C[0x970 - 0x23C]; // _23C
#else
	u8 _244[0x980 - 0x244]; // _244
#endif
	u32* mBits; // _980 (JP: _970)
};

struct RoboView {
#if defined(VERSION_JP)
	u8 _000[0x9CC]; // _000
#else
	u8 _000[0x9DC]; // _000
#endif
	s32 mPlayerIdx; // _9DC (JP: _9CC)
	u8 _9E0[0x68];  // _9E0 (JP: _9D0)
	s32 _A48;       // _A48 (JP: _A38)
	u8 _A4C[0x18];  // _A4C (JP: _A3C)
	XObj* mHeld;    // _A64 (JP: _A54)
};

// sizes of the views, not the real objects
#if defined(VERSION_JP)
typedef char AnaXObjViewSizeCheck[sizeof(XObj) == 0x978 ? 1 : -1];
typedef char AnaRoboViewSizeCheck[sizeof(RoboView) == 0xA58 ? 1 : -1];
#else
typedef char AnaXObjViewSizeCheck[sizeof(XObj) == 0x988 ? 1 : -1];
typedef char AnaRoboViewSizeCheck[sizeof(RoboView) == 0xA68 ? 1 : -1];
#endif

extern "C" CVec fn_800A0978(RoboView* mgr);

class Ana {
public:
	Ana() { }
	~Ana() { }
	virtual bool IsActive();

	void SetJObj(s32 kind, s32 which)
	{
		mJObjKind  = kind;
		mJObjWhich = which;
	}

	Vec mPos;        // _04
	f32 mRotY;       // _10
	CStatus mStatus; // _14
	s32 mId;         // _1C
	s32 mJObjKind;   // _20
	s32 mJObjWhich;  // _24
};

class AnaNull : public Ana {
public:
	virtual bool IsActive() { return false; }
};

#define ANA_NUM        20
#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

s32 activeAnaNum;
static Ana anas[ANA_NUM];
static AnaNull anaNull;
XObj* tutiAnaObj;
XObj* tutiHutaObj;
XObj* flowerObjs[3];

inline Ana* GetAna(s32 id)
{
	if (id < 0 || id >= ARRAY_COUNT(anas)) {
		return &anaNull;
	}
	return &anas[id];
}

inline Ana* GetNearestAna(f32 x, f32 y, f32 z)
{
	Ana* nearest = NULL;
	u32 i;
	CVec pos(x, y, z);
#if !defined(VERSION_GGTP01)
	OSReport("%4.2f %4.2f %4.2f\n", x, y, z);
#endif
	for (i = 0; i < ARRAY_COUNT(anas); i++) {
		if (anas[i].mStatus.mFlags & 1) {
			f32 dist = PSVECDistance(&pos, &anas[i].mPos);
			if (nearest == NULL || dist < PSVECDistance(&pos, &nearest->mPos)) {
				nearest = &anas[i];
			}
		}
	}
	return nearest;
}

inline void ReleaseHeld(RoboView* mgr)
{
	XObj* held = mgr->mHeld;
	if (held != NULL) {
		held->mFlags &= ~0x0400000000000000ULL;
		mgr->mHeld = NULL;
	}
	mgr->_A48 = 0;
}

// caller checks too, retail tests the pointer twice
inline s32 GetKindData(UnkKindData* data)
{
	return data != NULL ? data->mKind : 0;
}

inline s16 GetKind(XObj* obj)
{
	return obj->mKind != NULL ? GetKindData(obj->mKind) : -1;
}

inline void DropObj(XObj* obj)
{
	if (GetKind(obj) == 8) {
		u32 bit = obj->vf148();
		obj->mBits[bit >> 5] |= 1 << (bit & 31);
	}
	obj->vf54();
}

extern "C" HSD_JObj* GetAnaJObj(s32 kind, s32 which);

/**
 * @note Address: stage04 .text 0x2A8
 * @note Size: 0x48
 */
// CVec::~CVec

/**
 * @note Address: stage04 .text 0x2F0
 * @note Size: 0x2FC
 */
extern "C" void Ana_Draw(Ana* ana)
{
	HSD_JObj* jobj = GetAnaJObj(ana->mJObjKind, ana->mJObjWhich);
	f32 x, y, z;

	if (jobj == NULL) {
		return;
	}
	if (ana->mStatus.mFlags & 8) {
		const CVec& pos = fn_800A0978((RoboView*)lbl_8025C400.mPlayer);
		x               = pos.x;
		y               = pos.y;
		z               = pos.z;
	} else {
		x = ana->mPos.x;
		y = ana->mPos.y;
		z = ana->mPos.z;
	}
	HSD_JObjSetTranslateX(jobj, x);
	HSD_JObjSetTranslateY(jobj, y);
	HSD_JObjSetTranslateZ(jobj, z);
	HSD_JObjSetRotationY(jobj, ana->mRotY);
	HSD_JObjDispAll(jobj, NULL, 7, 0);
}

/**
 * @note Address: stage04 .text 0x5EC
 * @note Size: 0x460
 */
extern "C" HSD_JObj* GetAnaJObj(s32 kind, s32 which)
{
	HSD_JObj* jobj0;
	HSD_JObj* jobj1;
	XObj* obj;

	if (tutiAnaObj == NULL) {
		return NULL;
	}
	jobj0 = tutiAnaObj->mJObj;
	jobj1 = NULL;
	if (tutiHutaObj != NULL) {
		jobj1 = tutiHutaObj->mJObj;
	}
	if (which < 0 || which >= ARRAY_COUNT(flowerObjs)) {
		which = 0;
	}
	obj = flowerObjs[which];
	switch (kind) {
	case 1:
		return jobj0;
	case 2:
		return jobj1;
	case 3: {
		if (obj == NULL) {
			return NULL;
		}
		s32 path[2] = { 0, 0 };
		return GetJointByPath(obj->mJObj, path, 1);
	}
	case 4: {
		if (obj == NULL) {
			return NULL;
		}
		s32 path[2] = { 1, 0 };
		return GetJointByPath(obj->mJObj, path, 1);
	}
	case 5: {
		if (obj == NULL) {
			return NULL;
		}
		s32 path[2] = { 2, 0 };
		return GetJointByPath(obj->mJObj, path, 1);
	}
	case 6:
		if (obj == NULL) {
			return NULL;
		}
		if (which == 0) {
			s32 path[2] = { 4, 0 };
			return GetJointByPath(obj->mJObj, path, 1);
		} else {
			s32 path[2] = { 3, 0 };
			return GetJointByPath(obj->mJObj, path, 1);
		}
	case 7:
		if (obj == NULL) {
			return NULL;
		}
		if (which == 0) {
			s32 path[2] = { 3, 0 };
			return GetJointByPath(obj->mJObj, path, 1);
		} else {
			s32 path[2] = { 4, 0 };
			return GetJointByPath(obj->mJObj, path, 1);
		}
	}
	return NULL;
}

/**
 * @note Address: stage04 .text 0xA4C
 * @note Size: 0x250
 */
extern "C" s32 AddAna(s32 id, f32 x, f32 y, f32 z)
{
	CVec pos(x, y, z);
	u32 i;

	if (id >= 0 && id < ARRAY_COUNT(anas)) {
		if (!(anas[id].mStatus.mFlags & 1)) {
			activeAnaNum++;
		}
#if !defined(VERSION_GGTP01)
		OSReport("id=%d,%4.2f,%4.2f,%4.2f\n", id, x, y, z);
		OSReport("Ana::%4.2f, %4.2f, %4.2f\n", x, y, z);
#endif
		anas[id].mStatus.mFlags |= 1;
		anas[id].mPos.x = x;
		anas[id].mPos.y = y;
		anas[id].mPos.z = z;
		anas[id].mId    = id;
		return id;
	}

	for (i = 0; i < ARRAY_COUNT(anas); i++) {
		if ((anas[i].mStatus.mFlags & 1) && PSVECDistance(&pos, &anas[i].mPos) < 5.0f) {
			return -1;
		}
	}
	for (i = 0; i < ARRAY_COUNT(anas); i++) {
		if (!(anas[i].mStatus.mFlags & 1)) {
			activeAnaNum++;
#if !defined(VERSION_GGTP01)
			OSReport("Ana::%4.2f, %4.2f, %4.2f\n", x, y, z);
#endif
			anas[i].mStatus.mFlags |= 1;
			anas[i].mPos.x = x;
			anas[i].mPos.y = y;
			anas[i].mPos.z = z;
			anas[i].mId    = i;
			return i;
		}
	}
	return -1;
}

// AddAna
/*
    stwu     r1, -0x70(r1)
    mflr     r0
    stw      r0, 0x74(r1)
    stfd     f31, 0x60(r1)
    psq_st   f31, 0x68(r1), 0, qr0
    stfd     f30, 0x50(r1)
    psq_st   f30, 0x58(r1), 0, qr0
    stfd     f29, 0x40(r1)
    psq_st   f29, 0x48(r1), 0, qr0
    stfd     f28, 0x30(r1)
    psq_st   f28, 0x38(r1), 0, qr0
    stw      r31, 0x2c(r1)
    stw      r30, 0x28(r1)
    stw      r29, 0x24(r1)
    fmr      f28, f1
    mr.      r29, r3
    fmr      f29, f2
    lis      r3, __vt__4CVec@ha
    fmr      f30, f3
    addi     r0, r3, __vt__4CVec@l
    stw      r0, 0x14(r1)
    stfs     f28, 0x8(r1)
    stfs     f29, 0xc(r1)
    stfs     f30, 0x10(r1)
    blt      .L_00000B60
    cmplwi   r29, 0x14
    bge      .L_00000B60
    mulli    r30, r29, 0x28
    lis      r3, anas@ha
    addi     r0, r3, anas@l
    add      r31, r0, r30
    lwzu     r0, 0x14(r31)
    clrlwi.  r0, r0, 31
    bne      .L_00000AE8
    lis      r3, activeAnaNum@ha
    addi     r4, r3, activeAnaNum@l
    lwz      r3, 0x0(r4)
    addi     r0, r3, 0x1
    stw      r0, 0x0(r4)
.L_00000AE8:
    fmr      f1, f28
    lis      r3, lbl_7_rodata_120@ha
    fmr      f2, f29
    addi     r3, r3, lbl_7_rodata_120@l
    fmr      f3, f30
    mr       r4, r29
    crset    cr1eq
    bl       OSReport
    fmr      f1, f28
    lis      r3, lbl_7_rodata_F0@ha
    fmr      f2, f29
    addi     r3, r3, lbl_7_rodata_F0@l
    fmr      f3, f30
    crset    cr1eq
    bl       OSReport
    lwz      r5, 0x0(r31)
    lis      r3, anas@ha
    addi     r0, r3, anas@l
    lis      r4, __vt__4CVec@ha
    ori      r5, r5, 0x1
    mr       r3, r29
    stw      r5, 0x0(r31)
    add      r5, r0, r30
    addi     r0, r4, __vt__4CVec@l
    stfs     f28, 0x4(r5)
    stfs     f29, 0x8(r5)
    stfs     f30, 0xc(r5)
    stw      r29, 0x1c(r5)
    stw      r0, 0x14(r1)
    b        .L_00000C60
.L_00000B60:
    lis      r3, lbl_7_rodata_13C@ha
    lis      r4, anas@ha
    addi     r31, r4, anas@l
    lfs      f31, lbl_7_rodata_13C@l(r3)
    li       r30, 0x0
    mr       r29, r31
.L_00000B78:
    lwz      r0, 0x14(r29)
    clrlwi.  r0, r0, 31
    beq      .L_00000BAC
    addi     r3, r1, 0x8
    addi     r4, r29, 0x4
    bl       PSVECDistance
    fcmpo    cr0, f1, f31
    bge      .L_00000BAC
    lis      r4, __vt__4CVec@ha
    li       r3, -0x1
    addi     r0, r4, __vt__4CVec@l
    stw      r0, 0x14(r1)
    b        .L_00000C60
.L_00000BAC:
    addi     r30, r30, 0x1
    addi     r29, r29, 0x28
    cmplwi   r30, 0x14
    blt      .L_00000B78
    li       r0, 0x14
    li       r30, 0x0
    mtctr    r0
.L_00000BC8:
    lwz      r0, 0x14(r31)
    clrlwi.  r0, r0, 31
    bne      .L_00000C44
    lis      r4, activeAnaNum@ha
    lis      r3, lbl_7_rodata_F0@ha
    addi     r5, r4, activeAnaNum@l
    fmr      f1, f28
    lwz      r4, 0x0(r5)
    fmr      f2, f29
    fmr      f3, f30
    addi     r3, r3, lbl_7_rodata_F0@l
    addi     r0, r4, 0x1
    stw      r0, 0x0(r5)
    crset    cr1eq
    bl       OSReport
    mulli    r5, r30, 0x28
    lis      r4, anas@ha
    lis      r3, __vt__4CVec@ha
    addi     r0, r4, anas@l
    add      r5, r0, r5
    addi     r0, r3, __vt__4CVec@l
    lwz      r4, 0x14(r5)
    mr       r3, r30
    stw      r0, 0x14(r1)
    ori      r0, r4, 0x1
    stw      r0, 0x14(r5)
    stfs     f28, 0x4(r5)
    stfs     f29, 0x8(r5)
    stfs     f30, 0xc(r5)
    stw      r30, 0x1c(r5)
    b        .L_00000C60
.L_00000C44:
    addi     r31, r31, 0x28
    addi     r30, r30, 0x1
    bdnz     .L_00000BC8
    lis      r4, __vt__4CVec@ha
    li       r3, -0x1
    addi     r0, r4, __vt__4CVec@l
    stw      r0, 0x14(r1)
.L_00000C60:
    psq_l    f31, 0x68(r1), 0, qr0
    lfd      f31, 0x60(r1)
    psq_l    f30, 0x58(r1), 0, qr0
    lfd      f30, 0x50(r1)
    psq_l    f29, 0x48(r1), 0, qr0
    lfd      f29, 0x40(r1)
    psq_l    f28, 0x38(r1), 0, qr0
    lfd      f28, 0x30(r1)
    lwz      r31, 0x2c(r1)
    lwz      r30, 0x28(r1)
    lwz      r0, 0x74(r1)
    lwz      r29, 0x24(r1)
    mtlr     r0
    addi     r1, r1, 0x70
    blr
*/

/**
 * @note Address: stage04 .text 0xC9C
 * @note Size: 0x5C
 */
extern "C" void DrawAnas(void)
{
	u32 i;
	for (i = 0; i < ANA_NUM; i++) {
		if (anas[i].mStatus.mFlags & 1) {
			Ana_Draw(&anas[i]);
		}
	}
}

/**
 * @note Address: stage04 .text 0xCF8
 * @note Size: 0x4
 */
extern "C" void UpdateAnas(void)
{
}

/**
 * @note Address: stage04 .text 0xCFC
 * @note Size: 0x14C
 */
extern "C" void InitAnas(void)
{
	s32 num;
	s32 i;
	u32 j;

	for (j = 0; j < ANA_NUM; j++) {
		anas[j].mStatus.mFlags = 0;
	}
	for (j = 0; j < ARRAY_COUNT(flowerObjs); j++) {
		flowerObjs[j] = NULL;
	}
	num          = lbl_8025C400.mObjectCount;
	tutiAnaObj   = NULL;
	tutiHutaObj  = NULL;
	activeAnaNum = 0;

	for (i = 0; i < num; i++) {
		XObj* obj = lbl_8025C400.mObjects[i];
		if (obj->mFlags & 1) {
			if (obj->mIdData->mId == 0x2A9) {
				tutiAnaObj = obj;
			}
			if (obj->mIdData->mId == 0x178) {
				tutiHutaObj = obj;
			}
			if (obj->mIdData->mId == 0x2B7) {
				flowerObjs[0] = obj;
			}
			if (obj->mIdData->mId == 0x2B8) {
				flowerObjs[1] = obj;
			}
			if (obj->mIdData->mId == 0x2B9) {
				flowerObjs[2] = obj;
			}
		}
	}
}

// InitAnas
/*
    lis      r4, anas@ha
    li       r0, 0x0
    addi     r6, r4, anas@l
    lis      r3, lbl_8025C400@ha
    stw      r0, 0x14(r6)
    addi     r4, r3, lbl_8025C400@l
    addis    r5, r4, 0x1
    lis      r3, flowerObjs@ha
    stw      r0, 0x3c(r6)
    lis      r9, tutiAnaObj@ha
    lis      r8, tutiHutaObj@ha
    lis      r7, activeAnaNum@ha
    stw      r0, 0x64(r6)
    lwz      r5, -0x57a0(r5)
    stw      r0, 0x8c(r6)
    stw      r0, 0xb4(r6)
    stw      r0, 0xdc(r6)
    stw      r0, 0x104(r6)
    stw      r0, 0x12c(r6)
    stw      r0, 0x154(r6)
    stw      r0, 0x17c(r6)
    stw      r0, 0x1a4(r6)
    stw      r0, 0x1cc(r6)
    stw      r0, 0x1f4(r6)
    stw      r0, 0x21c(r6)
    stw      r0, 0x244(r6)
    stw      r0, 0x26c(r6)
    stw      r0, 0x294(r6)
    stw      r0, 0x2bc(r6)
    stwu     r0, flowerObjs@l(r3)
    stw      r0, 0x2e4(r6)
    stw      r0, 0x30c(r6)
    stw      r0, tutiAnaObj@l(r9)
    stw      r0, tutiHutaObj@l(r8)
    stw      r0, 0x4(r3)
    stw      r0, 0x8(r3)
    stw      r0, activeAnaNum@l(r7)
    mtctr    r5
    cmpwi    r5, 0x0
    blelr
.L_00000D9C:
    addis    r6, r4, 0x1
    li       r5, 0x1
    lwz      r8, -0x67a0(r6)
    li       r6, 0x0
    lwz      r0, 0x228(r8)
    lwz      r7, 0x22c(r8)
    and      r0, r0, r6
    and      r5, r7, r5
    xor      r5, r5, r6
    xor      r0, r0, r6
    or.      r0, r5, r0
    beq      .L_00000E3C
    lwz      r5, 0x238(r8)
    lwz      r0, 0x0(r5)
    cmpwi    r0, 0x2a9
    bne      .L_00000DE4
    lis      r5, tutiAnaObj@ha
    stw      r8, tutiAnaObj@l(r5)
.L_00000DE4:
    lwz      r5, 0x238(r8)
    lwz      r0, 0x0(r5)
    cmpwi    r0, 0x178
    bne      .L_00000DFC
    lis      r5, tutiHutaObj@ha
    stw      r8, tutiHutaObj@l(r5)
.L_00000DFC:
    lwz      r5, 0x238(r8)
    lwz      r0, 0x0(r5)
    cmpwi    r0, 0x2b7
    bne      .L_00000E14
    lis      r5, flowerObjs@ha
    stw      r8, flowerObjs@l(r5)
.L_00000E14:
    lwz      r5, 0x238(r8)
    lwz      r0, 0x0(r5)
    cmpwi    r0, 0x2b8
    bne      .L_00000E28
    stw      r8, 0x4(r3)
.L_00000E28:
    lwz      r5, 0x238(r8)
    lwz      r0, 0x0(r5)
    cmpwi    r0, 0x2b9
    bne      .L_00000E3C
    stw      r8, 0x8(r3)
.L_00000E3C:
    addi     r4, r4, 0x4
    bdnz     .L_00000D9C
    blr
*/

/**
 * @note Address: stage04 .text 0xE48
 * @note Size: 0x88
 */
extern "C" s32 CmdAttachAnaToRobo(s32 cmd, s32 argc, s32* args)
{
	Ana* ana = GetAna(args[0]);
	if (!ana->IsActive()) {
		return 0;
	}
	ana->mStatus.mFlags |= 8;
	return 0;
}

/**
 * @note Address: stage04 .text 0xED0
 * @note Size: 0xC
 */
bool Ana::IsActive()
{
	return mStatus.mFlags & 1;
}

/**
 * @note Address: stage04 .text 0xEDC
 * @note Size: 0x118
 */
extern "C" s32 CmdDropHeldObj(s32 cmd, s32 argc, s32* args)
{
	XObj* obj = ((RoboView*)lbl_8025C400.mPlayer)->mHeld;
	if (obj != NULL) {
		obj->mFlags &= ~0x10000ULL;
		ReleaseHeld((RoboView*)lbl_8025C400.mPlayer);
		DropObj(obj);
	}
	return 0;
}

// CmdDropHeldObj
/*
    stwu     r1, -0x10(r1)
    mflr     r0
    lis      r3, lbl_8025C400@ha
    stw      r0, 0x14(r1)
    addi     r3, r3, lbl_8025C400@l
    addis    r5, r3, 0x1
    stw      r31, 0xc(r1)
    lwz      r3, -0x6944(r5)
    lwz      r31, 0xa64(r3)
    cmplwi   r31, 0x0
    beq      .L_00000FDC
    lis      r3, 0xffff
    lwz      r4, 0x22c(r31)
    subi     r0, r3, 0x1
    lwz      r3, 0x228(r31)
    and      r0, r4, r0
    li       r6, -0x1
    stw      r0, 0x22c(r31)
    and      r0, r3, r6
    stw      r0, 0x228(r31)
    lwz      r7, -0x6944(r5)
    lwz      r8, 0xa64(r7)
    cmplwi   r8, 0x0
    beq      .L_00000F64
    lwz      r0, 0x22c(r8)
    lis      r3, 0xfc00
    lwz      r5, 0x228(r8)
    subi     r4, r3, 0x1
    and      r3, r0, r6
    li       r0, 0x0
    stw      r3, 0x22c(r8)
    and      r3, r5, r4
    stw      r3, 0x228(r8)
    stw      r0, 0xa64(r7)
.L_00000F64:
    li       r0, 0x0
    stw      r0, 0xa48(r7)
    lwz      r3, 0x240(r31)
    cmplwi   r3, 0x0
    beq      .L_00000F84
    beq      .L_00000F88
    lha      r0, 0x14(r3)
    b        .L_00000F88
.L_00000F84:
    li       r0, -0x1
.L_00000F88:
    extsh    r0, r0
    cmpwi    r0, 0x8
    bne      .L_00000FC8
    mr       r3, r31
    lwz      r12, 0x34(r31)
    lwz      r12, 0x148(r12)
    mtctr    r12
    bctrl
    lwz      r6, 0x980(r31)
    rlwinm   r5, r3, 29, 3, 29
    clrlwi   r0, r3, 27
    li       r3, 0x1
    lwzx     r4, r6, r5
    slw      r0, r3, r0
    or       r0, r4, r0
    stwx     r0, r6, r5
.L_00000FC8:
    mr       r3, r31
    lwz      r12, 0x34(r31)
    lwz      r12, 0x54(r12)
    mtctr    r12
    bctrl
.L_00000FDC:
    lwz      r0, 0x14(r1)
    li       r3, 0x0
    lwz      r31, 0xc(r1)
    mtlr     r0
    addi     r1, r1, 0x10
    blr
*/

/**
 * @note Address: stage04 .text 0xFF4
 * @note Size: 0x74
 */
extern "C" s32 CmdSetAnaRotY(s32 cmd, s32 argc, s32* args)
{
	Ana* ana   = GetAna(args[0]);
	ana->mRotY = 57.295776f * args[1];
	return 1;
}

/**
 * @note Address: stage04 .text 0x1068
 * @note Size: 0x98
 */
extern "C" s32 CmdSetAnaJObj(s32 cmd, s32 argc, s32* args)
{
	Ana* ana = GetAna(args[0]);
	if (!ana->IsActive()) {
		return 0;
	}
	ana->SetJObj(args[1], args[2]);
	return 1;
}

/**
 * @note Address: stage04 .text 0x1100
 * @note Size: 0x14C
 */
extern "C" s32 CmdPlantTane(s32 cmd, s32 argc, s32* args)
{
	XObj* obj;
	Ana* ana = GetAna(args[0]);
	if (!ana->IsActive()) {
		return 0;
	}
	ana->SetJObj(2, 0);
	obj = ((RoboView*)lbl_8025C400.mPlayer)->mHeld;
	if (obj != NULL) {
		ReleaseHeld((RoboView*)lbl_8025C400.mPlayer);
		DropObj(obj);
	}
	return 1;
}

/**
 * @note Address: stage04 .text 0x124C
 * @note Size: 0x240
 */
extern "C" s32 CmdFindNearestAna(s32 cmd, s32 argc, s32* args)
{
	Ana* best = NULL;

	if (argc == 3) {
		f32 x = args[0] / 100.0f;
		f32 y = args[1] / 100.0f;
		f32 z = args[2] / 100.0f;
		best  = GetNearestAna(x, y, z);
	} else {
		XObj* player = lbl_8025C400.mObjects[((RoboView*)lbl_8025C400.mPlayer)->mPlayerIdx];
		if (player != NULL) {
			best = GetNearestAna(player->mPos.x, player->mPos.y, player->mPos.z);
			// retail tests the player's result here and again below
			if (best != NULL) {
				return best->mId;
			}
		}
	}
	if (best != NULL) {
		return best->mId;
	}
	return -1;
}

/**
 * @note Address: stage04 .text 0x148C
 * @note Size: 0x8C
 */
extern "C" s32 CmdSetTutiAna(s32 cmd, s32 argc, s32* args)
{
	Ana* ana = GetAna(args[0]);
	if (!ana->IsActive()) {
		return 0;
	}
	ana->SetJObj(1, 0);
	return 1;
}

/**
 * @note Address: stage04 .text 0x1518
 * @note Size: 0x10C
 */
extern "C" s32 CmdAddAna(s32 cmd, s32 argc, s32* args)
{
	if (argc >= 4) {
		return AddAna(args[3], args[0] / 100.0f, args[1] / 100.0f, args[2] / 100.0f);
	}
	return AddAna(-1, args[0] / 100.0f, args[1] / 100.0f, args[2] / 100.0f);
}

/**
 * @note Address: stage04 .text 0x1624
 * @note Size: 0x9C
 */
// static initializer for anas and anaNull

/**
 * @note Address: stage04 .text 0x16C0
 * @note Size: 0x8
 */
// AnaNull::IsActive, inline in the class above

/**
 * @note Address: stage04 .text 0x16C8
 * @note Size: 0x70
 */
// AnaNull::~AnaNull, compiler made

/**
 * @note Address: stage04 .text 0x1738
 * @note Size: 0x48
 */
// CStatus::~CStatus, inline in the class above

/**
 * @note Address: stage04 .text 0x1780
 * @note Size: 0x38
 */
// destroys anas at exit, compiler made

/**
 * @note Address: stage04 .text 0x17B8
 * @note Size: 0x60
 */
// Ana::~Ana

/**
 * @note Address: stage04 .text 0x1818
 * @note Size: 0x24
 */
// Ana::Ana
