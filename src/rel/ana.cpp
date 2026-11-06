#include "Version.h"
#include "rel/anaobj.h"
#include "rel/ana.h"
#include "game/cgame.h"
#include "game/xchr.h"

extern "C" {
void OSReport(const char* msg, ...);
}

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
	AnaObj* mHeld;  // _A64 (JP: _A54)
};

// sizes of the views, not the real objects
#if defined(VERSION_JP)
typedef char AnaXObjViewSizeCheck[sizeof(AnaObj) == 0x978 ? 1 : -1];
typedef char AnaRoboViewSizeCheck[sizeof(RoboView) == 0xA58 ? 1 : -1];
#else
typedef char AnaXObjViewSizeCheck[sizeof(AnaObj) == 0x988 ? 1 : -1];
typedef char AnaRoboViewSizeCheck[sizeof(RoboView) == 0xA68 ? 1 : -1];
#endif

#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

extern s32 activeAnaNum;
extern Ana anas[ANA_NUM];
extern AnaNull anaNull;

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
	if (mgr->mHeld != NULL) {
		mgr->mHeld->mFlags &= ~0x0400000000000000ULL;
		mgr->mHeld = NULL;
	}
	mgr->_A48 = 0;
}

// caller checks too, retail tests the pointer twice
inline s32 GetKindData(UnkKindData* data)
{
	return data != NULL ? data->mKind : 0;
}

inline s16 GetKind(AnaObj* obj)
{
	return obj->mKind != NULL ? GetKindData(obj->mKind) : -1;
}

inline void DropObj(AnaObj* obj)
{
	if (GetKind(obj) == 8) {
		u32 bit = obj->vf148();
		obj->mBits[bit >> 5] |= 1 << (bit & 31);
	}
	obj->vf54();
}

/**
 * @note Address: stage04 .text 0x2F0
 * @note Size: 0x2FC
 */
#pragma section RX ".text.anadraw"
extern "C" __declspec(section ".text.anadraw") void Ana_Draw(Ana* ana)
{
	HSD_JObj* jobj = GetAnaJObj(ana->mJObjKind, ana->mJObjWhich);
	f32 x, y, z;

	if (jobj == NULL) {
		return;
	}
	if (ana->mStatus.mFlags & 8) {
		const CVec& pos = GetPlayerCarryJointWorldPosition(gGame.GetPlayer());
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
 * @note Address: stage04 .text 0xA4C
 * @note Size: 0x250
 */
#pragma section RX ".text.anaplacement"
extern "C" __declspec(section ".text.anaplacement") s32 AddAna(s32 id, f32 x, f32 y, f32 z)
{
	CVec pos(x, y, z);
	u32 i;

	if (id >= 0 && id < ARRAY_COUNT(anas)) {
#if defined(VERSION_PAL)
		Ana& entry = anas[id];
		if (!(entry.mStatus.mFlags & 1)) {
			activeAnaNum++;
		}
		entry.mStatus.mFlags |= 1;
#else
		if (!(anas[id].mStatus.mFlags & 1)) {
			activeAnaNum++;
		}
		OSReport("id=%d,%4.2f,%4.2f,%4.2f\n", id, x, y, z);
		OSReport("Ana::%4.2f, %4.2f, %4.2f\n", x, y, z);
		anas[id].mStatus.mFlags |= 1;
		Ana& entry = anas[id];
#endif
		entry.mPos.x = x;
		entry.mPos.y = y;
		entry.mPos.z = z;
		entry.mId    = id;
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
extern "C" __declspec(section ".text.anaplacement") void DrawAnas(void)
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
extern "C" __declspec(section ".text.anaplacement") void UpdateAnas(void)
{
}

/**
 * @note Address: stage04 .text 0xCFC
 * @note Size: 0x14C
 */
extern "C" __declspec(section ".text.anaplacement") void InitAnas(void)
{
	s32 num;
	s32 i;
	u32 j;

	tutiAnaObj  = NULL;
	tutiHutaObj = NULL;
	for (j = 0; j < ARRAY_COUNT(flowerObjs); j++) {
		flowerObjs[j] = NULL;
	}
	activeAnaNum = 0;
	for (j = 0; j < ANA_NUM; j++) {
		anas[j].mStatus.mFlags = 0;
	}
	num = gGame.mXObjs.mObjectCount;

	for (i = 0; i < num; i++) {
		AnaObj* obj = (AnaObj*)gGame.mXObjs.mObjects[i];
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
    lis      r3, gGame@ha
    stw      r0, 0x14(r6)
    addi     r4, r3, gGame@l
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
extern "C" __declspec(section ".text.anaplacement") s32 CmdAttachAnaToRobo(s32 cmd, s32 argc, s32* args)
{
	Ana* ana = GetAna(args[0]);
	if (!ana->IsActive()) {
		return 0;
	}
	ana->mStatus.mFlags |= 8;
	return 0;
}

/**
 * @note Address: stage04 .text 0xEDC
 * @note Size: 0x118
 */
#pragma section RX ".text.anainteraction"
extern "C" __declspec(section ".text.anainteraction") s32 CmdDropHeldObj(s32 cmd, s32 argc, s32* args)
{
	AnaObj* obj = ((RoboView*)gGame.GetPlayer())->mHeld;
	if (obj != NULL) {
		obj->mFlags &= ~0x10000ULL;
		ReleaseHeld((RoboView*)gGame.GetPlayer());
		DropObj(obj);
	}
	return 0;
}

// CmdDropHeldObj
/*
    stwu     r1, -0x10(r1)
    mflr     r0
    lis      r3, gGame@ha
    stw      r0, 0x14(r1)
    addi     r3, r3, gGame@l
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
#pragma section RX ".text.anacommands"
extern "C" __declspec(section ".text.anacommands") s32 CmdSetAnaRotY(s32 cmd, s32 argc, s32* args)
{
	Ana* ana   = GetAna(args[0]);
	ana->mRotY = 57.295776f * args[1];
	return 1;
}

/**
 * @note Address: stage04 .text 0x1068
 * @note Size: 0x98
 */
extern "C" __declspec(section ".text.anacommands") s32 CmdSetAnaJObj(s32 cmd, s32 argc, s32* args)
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
extern "C" __declspec(section ".text.anacommands") s32 CmdPlantTane(s32 cmd, s32 argc, s32* args)
{
	AnaObj* obj;
	Ana* ana = GetAna(args[0]);
	if (!ana->IsActive()) {
		return 0;
	}
	ana->SetJObj(2, 0);
	obj = ((RoboView*)gGame.GetPlayer())->mHeld;
	if (obj != NULL) {
		ReleaseHeld((RoboView*)gGame.GetPlayer());
		DropObj(obj);
	}
	return 1;
}

/**
 * @note Address: stage04 .text 0x124C
 * @note Size: 0x240
 */
extern "C" __declspec(section ".text.anacommands") s32 CmdFindNearestAna(s32 cmd, s32 argc, s32* args)
{
	Ana* best = NULL;

	if (argc == 3) {
		f32 x = args[0] / 100.0f;
		f32 y = args[1] / 100.0f;
		f32 z = args[2] / 100.0f;
		best  = GetNearestAna(x, y, z);
	} else {
		AnaObj* player = (AnaObj*)gGame.mXObjs.mObjects[((RoboView*)gGame.GetPlayer())->mPlayerIdx];
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
extern "C" __declspec(section ".text.anacommands") s32 CmdSetTutiAna(s32 cmd, s32 argc, s32* args)
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
extern "C" __declspec(section ".text.anacommands") s32 CmdAddAna(s32 cmd, s32 argc, s32* args)
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
