#include "game/xobjs.h"
#include <math.h>
#include <types.h>

// one of the 400 entries at CMsg _1348, CMsgFont_Draw uses a CTexObj at +4 in each,
// japan's is 4 bytes shorter before _2A6
struct CMsgUnkEntry {
#if defined(VERSION_JP)
	u8 _00[0x2A2]; // _00
#else
	u8 _00[0x2A6]; // _00
#endif
	s16 _2A6;     // _2A6
	s16 _2A8;     // _2A8
	u8 _2AA[0x2]; // _2AA
};

struct CMsgUnk8A0 {
	u32 _00;             // _00
	u8 _04_80 : 1;       // _04
	u8 _04_40 : 1;       // _04
	u8 _04_20 : 1;       // _04
	u8 _04_10 : 1;       // _04
	u8 _04_08 : 1;       // _04
	u8 _04_04 : 1;       // _04
	u8 _04_03 : 2;       // _04
	u8 _05[0x120 - 0x5]; // _05
};

// the message window. japan has everything from _9C0 on 0x650 lower, its UnkXObjReadObj
// is 8 shorter, there are 8 more before the entries, and each entry is 4 shorter
struct CMsg {
	u8 _00[0x804];          // _00
	u8* _804;               // _804, read cursor
	u8 _808[0x898 - 0x808]; // _808
	u32 _898;               // _898
	u8 _89C[0x8A0 - 0x89C]; // _89C
	CMsgUnk8A0 _8A0;        // _8A0
	UnkXObjReadObj _9C0;    // _9C0
#if defined(VERSION_JP)
	u8 _C18[0x1338 - 0xC18]; // _C18
#else
	u8 _C20[0x1348 - 0xC20]; // _C20
#endif
	CMsgUnkEntry _1348[400];      // _1348
	u8 _44008[0x4450C - 0x44008]; // _44008
	s32 _4450C;                   // _4450C
	f32 _44510;                   // _44510
	f32 _44514;                   // _44514
	s32 _44518;                   // _44518, current entry index in CMsgFont_Draw
	s32 _4451C;                   // _4451C
	s32 _44520;                   // _44520
	Vec _44524;                   // _44524
	u8 _44530[0x44570 - 0x44530]; // _44530
	f32 _44570;                   // _44570
	f32 _44574;                   // _44574
	u32 _44578;                   // _44578, rgba out of msgColorTable
	f32 _4457C;                   // _4457C
	f32 _44580;                   // _44580
	f32 _44584;                   // _44584
	u8 _44588;                    // _44588
	s32 _4458C;                   // _4458C
	u8 _44590[0x445AC - 0x44590]; // _44590
	s32 _445AC;                   // _445AC
	f32 _445B0;                   // _445B0
	f32 _445B4;                   // _445B4
	f32 _445B8;                   // _445B8
	u8 _445BC[0x445C0 - 0x445BC]; // _445BC
	s32 _445C0;                   // _445C0
	f32 _445C4;                   // _445C4
	f32 _445C8;                   // _445C8
	u8 _445CC[0x445D0 - 0x445CC]; // _445CC
	s32 _445D0;                   // _445D0
	s32 _445D4;                   // _445D4
	s32 _445D8;                   // _445D8
	u8 _445DC[9];                 // _445DC, three groups of 3
	u8 _445E5[0x445E8 - 0x445E5]; // _445E5
	s32 _445E8;                   // _445E8
	s32 _445EC;                   // _445EC
	s32 _445F0;                   // _445F0
	u8 _445F4[0x445F8 - 0x445F4]; // _445F4
	s32 _445F8;                   // _445F8
	s32 _445FC;                   // _445FC
	f32 _44600;                   // _44600, 320.0f at reset, maybe a screen x
	f32 _44604;                   // _44604, 344.0f at reset, maybe a screen y
	s32 _44608;                   // _44608
	Vec _4460C;                   // _4460C
};

extern "C" u8 lbl_8025C400[];
extern u32 msgColorTable[];

// europe has an extra word somewhere before this
#if defined(VERSION_PAL)
#define GAME_290 (*(void**)(lbl_8025C400 + 0x294))
#else
#define GAME_290 (*(void**)(lbl_8025C400 + 0x290))
#endif

extern "C" {
void CMsg_ResetPos(CMsg* msg);
void CMsgFont_Draw(void* p, f32 t);
void fn_80199AB0(void* game, s32 a, s32 b);
void fn_8019B56C(void* gx, s32 a, s32 b, s32 c, s32 d);
void fn_8019B464(void* gx, s32 a, s32 b, s32 c);
void* GetSpawnData(s32 index);
}

/**
 * @note Address: 0x8007FFB4
 * @note Size: 0x2D0
 */
// CMsg__cmd_SETFONT
/*
    stwu     r1, -0x20(r1)
    mflr     r0
    clrlwi   r4, r4, 24
    stw      r0, 0x24(r1)
    subi     r0, r4, 0xd
    cmplwi   r0, 0x8
    bgt      .L_80080240
    lis      r5, jumptable_80225488@ha
    slwi     r0, r0, 2
    addi     r5, r5, jumptable_80225488@l
    lwzx     r0, r5, r0
    mtctr    r0
    bctr
.L_8007FFE8:
    lwz      r5, 0x804(r3)
    lbz      r4, 0x0(r5)
    addi     r0, r5, 0x1
    cmplwi   r4, 0xff
    stw      r0, 0x804(r3)
    mr       r0, r4
    bne      .L_80080008
    li       r0, 0x16
.L_80080008:
    clrlwi   r4, r0, 24
    lis      r0, 0x4330
    stw      r4, 0xc(r1)
    addis    r3, r3, 0x4
    lfd      f1, lbl_8065D048@sda21(r0)
    stw      r0, 0x8(r1)
    lfd      f0, 0x8(r1)
    fsubs    f0, f0, f1
    stfs     f0, 0x4570(r3)
    b        .L_80080270
.L_80080030:
    lwz      r5, 0x804(r3)
    lbz      r4, 0x0(r5)
    addi     r0, r5, 0x1
    cmplwi   r4, 0xff
    stw      r0, 0x804(r3)
    mr       r0, r4
    bne      .L_80080050
    li       r0, 0x0
.L_80080050:
    lis      r4, msgColorTable@ha
    clrlslwi r0, r0, 24, 2
    addi     r4, r4, msgColorTable@l
    addis    r3, r3, 0x4
    lwzx     r0, r4, r0
    stw      r0, 0x4578(r3)
    b        .L_80080270
.L_8008006C:
    lwz      r5, 0x804(r3)
    addis    r4, r3, 0x4
    lbz      r7, 0x0(r5)
    addi     r0, r5, 0x1
    stw      r0, 0x804(r3)
    lwz      r6, 0x804(r3)
    lbz      r5, 0x0(r6)
    addi     r0, r6, 0x1
    stw      r0, 0x804(r3)
    slwi     r0, r5, 16
    rlwimi   r0, r7, 24, 0, 7
    lwz      r5, 0x804(r3)
    lbz      r6, 0x0(r5)
    addi     r5, r5, 0x1
    stw      r5, 0x804(r3)
    rlwimi   r0, r6, 8, 16, 23
    lwz      r5, 0x804(r3)
    lbz      r6, 0x0(r5)
    addi     r5, r5, 0x1
    stw      r5, 0x804(r3)
    or       r0, r6, r0
    stw      r0, 0x4578(r4)
    b        .L_80080270
.L_800800C8:
    lwz      r5, 0x804(r3)
    lbz      r4, 0x0(r5)
    addi     r0, r5, 0x1
    stw      r0, 0x804(r3)
    lbz      r0, 0x8a4(r3)
    rlwimi   r0, r4, 2, 29, 29
    stb      r0, 0x8a4(r3)
    b        .L_80080270
.L_800800E8:
    lwz      r5, 0x804(r3)
    lbz      r4, 0x0(r5)
    addi     r0, r5, 0x1
    stw      r0, 0x804(r3)
    lbz      r0, 0x8a4(r3)
    rlwimi   r0, r4, 3, 28, 28
    stb      r0, 0x8a4(r3)
    b        .L_80080270
.L_80080108:
    lwz      r5, 0x804(r3)
    addis    r4, r3, 0x4
    lbz      r8, 0x0(r5)
    addi     r0, r5, 0x1
    stw      r0, 0x804(r3)
    lwz      r5, 0x804(r3)
    lbz      r7, 0x0(r5)
    addi     r0, r5, 0x1
    stw      r0, 0x804(r3)
    lwz      r6, 0x804(r3)
    lbz      r5, 0x0(r6)
    addi     r0, r6, 0x1
    stw      r0, 0x804(r3)
    stb      r8, 0x4588(r4)
    stb      r7, 0x4589(r4)
    stb      r5, 0x458a(r4)
    b        .L_80080270
.L_8008014C:
    lwz      r5, 0x804(r3)
    lis      r0, 0x4330
    stw      r0, 0x8(r1)
    addis    r4, r3, 0x4
    lbz      r7, 0x0(r5)
    addi     r0, r5, 0x1
    lfd      f1, lbl_8065D050@sda21(r0)
    stw      r0, 0x804(r3)
    lfs      f2, lbl_8065D030@sda21(r0)
    lwz      r6, 0x804(r3)
    lbz      r5, 0x0(r6)
    addi     r0, r6, 0x1
    rlwimi   r7, r5, 8, 16, 23
    stw      r0, 0x804(r3)
    extsh    r0, r7
    xoris    r0, r0, 0x8000
    stw      r0, 0xc(r1)
    lfd      f0, 0x8(r1)
    fsubs    f0, f0, f1
    fmuls    f0, f2, f0
    stfs     f0, 0x457c(r4)
    b        .L_80080270
.L_800801A4:
    lwz      r5, 0x804(r3)
    lis      r0, 0x4330
    stw      r0, 0x8(r1)
    addis    r4, r3, 0x4
    lbz      r7, 0x0(r5)
    addi     r5, r5, 0x1
    stw      r0, 0x10(r1)
    lfd      f3, lbl_8065D050@sda21(r0)
    stw      r5, 0x804(r3)
    lfs      f2, lbl_8065D034@sda21(r0)
    lwz      r6, 0x804(r3)
    lbz      r5, 0x0(r6)
    addi     r0, r6, 0x1
    stw      r0, 0x804(r3)
    rlwimi   r7, r5, 8, 16, 23
    extsh    r0, r7
    lwz      r5, 0x804(r3)
    xoris    r0, r0, 0x8000
    stw      r0, 0xc(r1)
    lbz      r7, 0x0(r5)
    addi     r0, r5, 0x1
    lfd      f0, 0x8(r1)
    stw      r0, 0x804(r3)
    fsubs    f0, f0, f3
    lwz      r6, 0x804(r3)
    lbz      r5, 0x0(r6)
    addi     r0, r6, 0x1
    fdivs    f1, f0, f2
    stw      r0, 0x804(r3)
    rlwimi   r7, r5, 8, 16, 23
    extsh    r0, r7
    xoris    r0, r0, 0x8000
    stw      r0, 0x14(r1)
    lfd      f0, 0x10(r1)
    stfs     f1, 0x4580(r4)
    fsubs    f0, f0, f3
    fdivs    f0, f0, f2
    stfs     f0, 0x4584(r4)
    b        .L_80080270
.L_80080240:
    lis      r3, lbl_80209150@ha
    addi     r3, r3, lbl_80209150@l
    crclr    cr1eq
    bl       fn_801911B4
    lis      r4, lbl_80225478@ha
    mr       r6, r3
    addi     r7, r4, lbl_80225478@l
    li       r3, lbl_8065D038@sda21
    li       r4, 0x831
    li       r5, lbl_8065D040@sda21
    crclr    cr1eq
    bl       OSPanic
.L_80080270:
    lwz      r0, 0x24(r1)
    li       r3, 0x0
    mtlr     r0
    addi     r1, r1, 0x20
    blr
*/

/**
 * @note Address: 0x80080284
 * @note Size: 0x2FC
 */
// CMsg__cmd_SFX
/*
    stwu     r1, -0x20(r1)
    mflr     r0
    stw      r0, 0x24(r1)
    stmw     r27, 0xc(r1)
    mr       r30, r3
    lwz      r4, 0x804(r3)
    lbz      r31, 0x0(r4)
    addi     r0, r4, 0x1
    stw      r0, 0x804(r3)
    lwz      r4, 0x804(r30)
    lbz      r3, 0x0(r4)
    addi     r0, r4, 0x1
    stw      r0, 0x804(r30)
    rlwimi   r31, r3, 8, 16, 23
    lwz      r4, 0x804(r30)
    lbz      r3, 0x0(r4)
    addi     r0, r4, 0x1
    stw      r0, 0x804(r30)
    slwi     r0, r3, 16
    or       r31, r31, r0
    lwz      r4, 0x804(r30)
    lbz      r3, 0x0(r4)
    addi     r0, r4, 0x1
    stw      r0, 0x804(r30)
    slwi     r0, r3, 24
    or       r31, r31, r0
    lwz      r4, 0x804(r30)
    lbz      r3, 0x0(r4)
    addi     r0, r4, 0x1
    extsb    r3, r3
    stw      r0, 0x804(r30)
    addi     r0, r3, 0x1
    cmplwi   r0, 0x7
    bgt      .L_80080568
    lis      r3, jumptable_802254AC@ha
    slwi     r0, r0, 2
    addi     r3, r3, jumptable_802254AC@l
    lwzx     r0, r3, r0
    mtctr    r0
    bctr
.L_80080324:
    cmpwi    r31, -0x2
    bne      .L_80080348
    lis      r3, lbl_8025C400@ha
    li       r4, -0x1
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    addi     r3, r3, 0x7e24
    bl       fn_8001E630
    b        .L_80080568
.L_80080348:
    lis      r3, lbl_8025C400@ha
    mr       r4, r31
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    addi     r3, r3, 0x7e24
    bl       fn_8001E660
    b        .L_80080568
.L_80080364:
    lis      r3, lbl_8025C400@ha
    mr       r4, r31
    addi     r3, r3, lbl_8025C400@l
    li       r5, 0xff
    addis    r3, r3, 0xe
    addi     r3, r3, 0x7e24
    bl       fn_8001E904
    b        .L_80080568
.L_80080384:
    lwz      r5, 0x804(r30)
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    mr       r4, r31
    lbz      r7, 0x0(r5)
    addi     r0, r5, 0x1
    addis    r3, r3, 0xe
    stw      r0, 0x804(r30)
    addi     r3, r3, 0x7e24
    lwz      r6, 0x804(r30)
    lbz      r5, 0x0(r6)
    addi     r0, r6, 0x1
    rlwimi   r7, r5, 8, 16, 23
    stw      r0, 0x804(r30)
    clrlwi   r5, r7, 16
    bl       fn_8001E1A4
    b        .L_80080568
.L_800803C8:
    lwz      r5, 0x804(r30)
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    mr       r4, r31
    lbz      r7, 0x0(r5)
    addi     r0, r5, 0x1
    addis    r3, r3, 0xe
    stw      r0, 0x804(r30)
    addi     r3, r3, 0x7e24
    lwz      r6, 0x804(r30)
    lbz      r5, 0x0(r6)
    addi     r0, r6, 0x1
    rlwimi   r7, r5, 8, 16, 23
    stw      r0, 0x804(r30)
    clrlwi   r5, r7, 16
    bl       fn_8001E2CC
    b        .L_80080568
.L_8008040C:
    lwz      r5, 0x804(r30)
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    mr       r4, r31
    lbz      r7, 0x0(r5)
    addi     r0, r5, 0x1
    addis    r28, r3, 0xe
    stw      r0, 0x804(r30)
    addi     r28, r28, 0x7e24
    mr       r3, r28
    lwz      r6, 0x804(r30)
    lbz      r5, 0x0(r6)
    addi     r0, r6, 0x1
    stw      r0, 0x804(r30)
    rlwimi   r7, r5, 8, 16, 23
    clrlwi   r27, r7, 16
    lwz      r5, 0x804(r30)
    lbz      r29, 0x0(r5)
    addi     r0, r5, 0x1
    stw      r0, 0x804(r30)
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    bne      .L_8008047C
    mr       r3, r28
    mr       r4, r31
    mr       r5, r29
    bl       fn_8001E904
    b        .L_80080568
.L_8008047C:
    mr       r3, r28
    mr       r4, r31
    mr       r5, r29
    mr       r6, r27
    bl       fn_8001E468
    b        .L_80080568
.L_80080494:
    cmpwi    r31, -0x2
    bne      .L_800804B8
    lis      r3, lbl_8025C400@ha
    li       r4, -0x1
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    addi     r3, r3, 0x7e24
    bl       fn_8001E590
    b        .L_80080568
.L_800804B8:
    clrrwi   r3, r31, 16
    addis    r0, r3, 0x1
    cmplwi   r0, 0x0
    bne      .L_800804E8
    lis      r3, lbl_8025C400@ha
    mr       r4, r31
    addi     r3, r3, lbl_8025C400@l
    li       r5, 0x0
    addis    r3, r3, 0xe
    addi     r3, r3, 0x7e24
    bl       fn_8001E5C0
    b        .L_80080568
.L_800804E8:
    mr       r3, r31
    bl       fn_801E94E4
    b        .L_80080568
.L_800804F4:
    cmpwi    r31, -0x2
    bne      .L_80080518
    lis      r3, lbl_8025C400@ha
    li       r4, -0x1
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    addi     r3, r3, 0x7e24
    bl       fn_8001E4D4
    b        .L_80080568
.L_80080518:
    lis      r3, lbl_8025C400@ha
    mr       r4, r31
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    addi     r3, r3, 0x7e24
    bl       fn_8001E504
    b        .L_80080568
.L_80080534:
    lis      r3, lbl_8025C400@ha
    mr       r4, r31
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    addi     r3, r3, 0x7e24
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_80080568
    lwz      r4, 0x804(r30)
    li       r3, 0x1
    subi     r0, r4, 0x3
    stw      r0, 0x804(r30)
    b        .L_8008056C
.L_80080568:
    li       r3, 0x0
.L_8008056C:
    lmw      r27, 0xc(r1)
    lwz      r0, 0x24(r1)
    mtlr     r0
    addi     r1, r1, 0x20
    blr
*/

/**
 * @note Address: 0x80080580
 * @note Size: 0x364
 */
// CMsg__cmd_ANIM
/*
    stwu     r1, -0x20(r1)
    mflr     r0
    stw      r0, 0x24(r1)
    stmw     r27, 0xc(r1)
    mr       r28, r3
    lwz      r3, 0x804(r3)
    lbz      r31, 0x0(r3)
    addi     r0, r3, 0x1
    stw      r0, 0x804(r28)
    clrlwi.  r0, r31, 31
    lwz      r3, 0x804(r28)
    lbz      r6, 0x0(r3)
    addi     r0, r3, 0x1
    stw      r0, 0x804(r28)
    lwz      r4, 0x804(r28)
    lbz      r3, 0x0(r4)
    addi     r0, r4, 0x1
    stw      r0, 0x804(r28)
    rlwimi   r6, r3, 8, 16, 23
    extsh    r4, r6
    lwz      r5, 0x804(r28)
    lbz      r29, 0x0(r5)
    addi     r0, r5, 0x1
    stw      r0, 0x804(r28)
    lwz      r5, 0x804(r28)
    lbz      r3, 0x0(r5)
    addi     r0, r5, 0x1
    stw      r0, 0x804(r28)
    rlwimi   r29, r3, 8, 16, 23
    lwz      r5, 0x804(r28)
    lbz      r3, 0x0(r5)
    addi     r0, r5, 0x1
    stw      r0, 0x804(r28)
    slwi     r0, r3, 16
    or       r29, r29, r0
    lwz      r3, 0x804(r28)
    lbz      r0, 0x0(r3)
    addi     r3, r3, 0x1
    slwi     r0, r0, 24
    stw      r3, 0x804(r28)
    or       r29, r29, r0
    beq      .L_80080638
    lis      r3, lbl_80367A2C@ha
    slwi     r0, r29, 2
    addi     r3, r3, lbl_80367A2C@l
    lwzx     r29, r3, r0
.L_80080638:
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x6
    lwzu     r0, -0x2f80(r3)
    cmplwi   r0, 0x0
    beq      .L_8008065C
    bl       fn_8007F570
    mr       r30, r3
    b        .L_80080660
.L_8008065C:
    li       r30, 0x0
.L_80080660:
    cmplwi   r30, 0x0
    beq      .L_800808CC
    lwz      r0, 0x228(r30)
    li       r5, 0x0
    lwz      r4, 0x22c(r30)
    li       r3, 0x1
    and      r0, r0, r5
    and      r3, r4, r3
    xor      r3, r3, r5
    xor      r0, r0, r5
    or.      r0, r3, r0
    beq      .L_800808CC
    cmpwi    r29, -0x1
    bne      .L_80080704
    lwz      r29, 0x0(r30)
    cmplwi   r29, 0x0
    beq      .L_800806E0
    mr       r3, r29
    bl       fn_801908F8
    cmplwi   r3, 0x0
    beq      .L_800806B8
    b        .L_800806E4
.L_800806B8:
    mr       r3, r29
    bl       fn_801905D4
    cmplwi   r3, 0x0
    beq      .L_800806CC
    b        .L_800806E4
.L_800806CC:
    mr       r3, r29
    bl       fn_801902C4
    cmplwi   r3, 0x0
    beq      .L_800806E0
    b        .L_800806E4
.L_800806E0:
    li       r3, 0x0
.L_800806E4:
    lwz      r0, 0x0(r3)
    andis.   r0, r0, 0x4400
    bne      .L_800808CC
    lwz      r4, 0x804(r28)
    li       r3, 0x1
    subi     r0, r4, 0x8
    stw      r0, 0x804(r28)
    b        .L_800808D0
.L_80080704:
    rlwinm.  r0, r31, 0, 27, 27
    bne      .L_80080870
    stw      r5, 0x810(r30)
    li       r3, -0x1
    clrrwi.  r0, r29, 16
    stw      r3, 0x814(r30)
    stw      r3, 0x81c(r30)
    stw      r3, 0x824(r30)
    stw      r3, 0x82c(r30)
    stw      r3, 0x834(r30)
    stw      r3, 0x83c(r30)
    stw      r3, 0x844(r30)
    stw      r3, 0x84c(r30)
    stw      r3, 0x854(r30)
    stw      r3, 0x85c(r30)
    stw      r3, 0x864(r30)
    stw      r3, 0x86c(r30)
    stw      r3, 0x874(r30)
    stw      r3, 0x87c(r30)
    stw      r3, 0x884(r30)
    stw      r3, 0x88c(r30)
    beq      .L_8008081C
    mr       r3, r30
    clrlwi   r28, r29, 16
    lwz      r12, 0x34(r30)
    mr       r4, r28
    srawi    r29, r29, 16
    lwz      r12, 0xf0(r12)
    mtctr    r12
    bctrl
    mr       r0, r3
    mr       r3, r30
    lwz      r12, 0x34(r30)
    mr       r31, r0
    mr       r4, r29
    lwz      r12, 0xf4(r12)
    mtctr    r12
    bctrl
    mr       r0, r3
    mr       r3, r30
    lwz      r12, 0x34(r30)
    mr       r27, r0
    mr       r4, r28
    lwz      r12, 0xf8(r12)
    mtctr    r12
    bctrl
    lwz      r12, 0x34(r30)
    mr       r0, r3
    lfs      f1, lbl_8065D058@sda21(r0)
    mr       r3, r30
    lwz      r12, 0x80(r12)
    mr       r8, r0
    fmr      f3, f1
    mr       r6, r31
    mr       r7, r27
    lfs      f2, lbl_8065D05C@sda21(r0)
    li       r4, 0x0
    lfs      f4, lbl_8065D060@sda21(r0)
    li       r5, 0x1
    mtctr    r12
    bctrl
    cmpwi    r28, 0x3e8
    bge      .L_80080808
    cmpwi    r29, 0x3e8
    blt      .L_80080854
.L_80080808:
    lwz      r3, 0x228(r30)
    lis      r0, 0x8
    or       r0, r3, r0
    stw      r0, 0x228(r30)
    b        .L_80080854
.L_8008081C:
    mr       r3, r30
    lfs      f1, lbl_8065D058@sda21(r0)
    lwz      r12, 0x34(r30)
    mr       r4, r29
    fmr      f3, f1
    lfs      f2, lbl_8065D05C@sda21(r0)
    lwz      r12, 0x80(r12)
    li       r5, 0x0
    lfs      f4, lbl_8065D060@sda21(r0)
    li       r6, 0x0
    li       r7, 0x0
    li       r8, 0x0
    mtctr    r12
    bctrl
.L_80080854:
    mr       r3, r30
    li       r4, 0x1
    lwz      r12, 0x34(r30)
    lwz      r12, 0x88(r12)
    mtctr    r12
    bctrl
    b        .L_800808CC
.L_80080870:
    li       r0, 0x10
    mtctr    r0
.L_80080878:
    lwz      r0, 0x810(r30)
    add      r3, r0, r5
    slwi     r0, r3, 28
    srwi     r3, r3, 31
    subf     r0, r3, r0
    rotlwi   r0, r0, 4
    add      r0, r0, r3
    slwi     r3, r0, 3
    addi     r0, r3, 0x814
    lwzx     r0, r30, r0
    cmpwi    r0, -0x1
    bne      .L_800808C4
    add      r4, r30, r3
    li       r3, 0x0
    stw      r29, 0x814(r4)
    li       r0, 0x64
    sth      r3, 0x818(r4)
    sth      r0, 0x81a(r4)
    b        .L_800808CC
.L_800808C4:
    addi     r5, r5, 0x1
    bdnz     .L_80080878
.L_800808CC:
    li       r3, 0x0
.L_800808D0:
    lmw      r27, 0xc(r1)
    lwz      r0, 0x24(r1)
    mtlr     r0
    addi     r1, r1, 0x20
    blr
*/

/**
 * @note Address: 0x800808E4
 * @note Size: 0x2A4
 */
// CMsg__cmd_WAIT
/*
    stwu     r1, -0x30(r1)
    mflr     r0
    stw      r0, 0x34(r1)
    stw      r31, 0x2c(r1)
    stw      r30, 0x28(r1)
    stw      r29, 0x24(r1)
    mr       r29, r3
    lwz      r3, 0x804(r3)
    lbz      r31, 0x0(r3)
    addi     r0, r3, 0x1
    cmpwi    r31, 0xff
    stw      r0, 0x804(r29)
    beq      .L_80080930
    cmpwi    r31, 0xfe
    beq      .L_80080930
    cmpwi    r31, 0xfd
    beq      .L_80080930
    cmpwi    r31, 0xfc
    bne      .L_80080AEC
.L_80080930:
    lbz      r0, 0x0(r29)
    cmplwi   r0, 0x23
    bne      .L_80080AE0
    lbz      r0, 0x8a4(r29)
    extrwi.  r0, r0, 1, 27
    bne      .L_800809A8
    cmpwi    r31, 0xff
    li       r4, -0x1
    bne      .L_80080958
    li       r4, 0x0
.L_80080958:
    cmpwi    r31, 0xfe
    bne      .L_80080964
    li       r4, 0x1
.L_80080964:
    cmpwi    r31, 0xfd
    beq      .L_80080974
    cmpwi    r31, 0xfc
    bne      .L_80080978
.L_80080974:
    li       r4, 0x0
.L_80080978:
    lbz      r0, 0x8a4(r29)
    li       r3, 0x1
    rlwimi   r0, r3, 4, 27, 27
    addis    r3, r29, 0x4
    stb      r0, 0x8a4(r29)
    lfs      f0, lbl_8065D058@sda21(r0)
    stw      r4, 0x450c(r3)
    stfs     f0, 0x4510(r3)
    lwz      r3, 0x804(r29)
    subi     r0, r3, 0x2
    stw      r0, 0x804(r29)
    b        .L_80080AE0
.L_800809A8:
    addi     r3, r1, 0x8
    li       r30, 0x0
    li       r4, 0x0
    bl       __ct__4CPadFUc
    cmpwi    r31, 0xfd
    beq      .L_800809C8
    cmpwi    r31, 0xfc
    bne      .L_80080A44
.L_800809C8:
    lwz      r3, 0x8(r1)
    lwz      r0, 0x14(r3)
    rlwinm.  r0, r0, 0, 21, 21
    beq      .L_80080A90
    lis      r4, lbl_8025C400@ha
    li       r3, 0x20
    addi     r6, r4, lbl_8025C400@l
    lwz      r5, 0x1b14(r6)
    li       r4, 0x0
    lwz      r0, 0x1b10(r6)
    and      r3, r5, r3
    and      r0, r0, r4
    xor      r3, r3, r4
    xor      r0, r0, r4
    or.      r0, r3, r0
    bne      .L_80080A90
    cmpwi    r31, 0xfd
    li       r30, 0x1
    bne      .L_80080A28
    addis    r3, r6, 0x1
    li       r4, 0x0
    lwz      r3, -0x6944(r3)
    bl       fn_800B54A4
    b        .L_80080A90
.L_80080A28:
    cmpwi    r31, 0xfc
    bne      .L_80080A90
    addis    r3, r6, 0x1
    li       r4, 0x1
    lwz      r3, -0x6944(r3)
    bl       fn_800B54A4
    b        .L_80080A90
.L_80080A44:
    lwz      r3, 0x8(r1)
    lwz      r3, 0x14(r3)
    rlwinm.  r0, r3, 0, 23, 23
    bne      .L_80080A5C
    rlwinm.  r0, r3, 0, 22, 22
    beq      .L_80080A90
.L_80080A5C:
    lis      r4, lbl_8025C400@ha
    li       r3, 0x20
    addi     r5, r4, lbl_8025C400@l
    lwz      r0, 0x1b10(r5)
    li       r4, 0x0
    lwz      r5, 0x1b14(r5)
    and      r0, r0, r4
    and      r3, r5, r3
    xor      r3, r3, r4
    xor      r0, r0, r4
    or.      r0, r3, r0
    bne      .L_80080A90
    li       r30, 0x1
.L_80080A90:
    cmpwi    r30, 0x0
    beq      .L_80080AC8
    lbz      r0, 0x8a4(r29)
    li       r3, 0x0
    rlwimi   r0, r3, 4, 27, 27
    lis      r3, lbl_8025C400@ha
    stb      r0, 0x8a4(r29)
    addi     r3, r3, lbl_8025C400@l
    addis    r4, r29, 0x4
    addis    r3, r3, 0xe
    lwz      r4, 0x45fc(r4)
    addi     r3, r3, 0x7e24
    bl       fn_8001DC10
    b        .L_80080AD4
.L_80080AC8:
    lwz      r3, 0x804(r29)
    subi     r0, r3, 0x2
    stw      r0, 0x804(r29)
.L_80080AD4:
    lis      r3, __vt__4CPad@ha
    addi     r0, r3, __vt__4CPad@l
    stw      r0, 0xc(r1)
.L_80080AE0:
    lbz      r0, 0x8a4(r29)
    extrwi   r3, r0, 1, 27
    b        .L_80080B6C
.L_80080AEC:
    lis      r3, 0x6666
    lis      r0, 0x4330
    addi     r3, r3, 0x6667
    stw      r0, 0x18(r1)
    mulhw    r5, r3, r31
    lfd      f3, lbl_8065D050@sda21(r0)
    lfs      f4, lbl_8065D064@sda21(r0)
    addis    r4, r29, 0x4
    lfs      f0, lbl_8065D060@sda21(r0)
    li       r3, 0x0
    srawi    r6, r5, 2
    stw      r0, 0x10(r1)
    srawi    r0, r5, 2
    srwi     r5, r0, 31
    srwi     r7, r6, 31
    add      r0, r0, r5
    mulli    r0, r0, 0xa
    add      r5, r6, r7
    xoris    r5, r5, 0x8000
    subf     r0, r0, r31
    stw      r5, 0x14(r1)
    xoris    r0, r0, 0x8000
    stw      r0, 0x1c(r1)
    lfd      f2, 0x10(r1)
    lfd      f1, 0x18(r1)
    fsubs    f2, f2, f3
    fsubs    f1, f1, f3
    fmuls    f2, f4, f2
    fmuls    f1, f4, f1
    fdivs    f0, f1, f0
    fadds    f2, f2, f0
    stfs     f2, 0x45cc(r4)
.L_80080B6C:
    lwz      r0, 0x34(r1)
    lwz      r31, 0x2c(r1)
    lwz      r30, 0x28(r1)
    lwz      r29, 0x24(r1)
    mtlr     r0
    addi     r1, r1, 0x30
    blr
*/

/**
 * @note Address: 0x80080B88
 * @note Size: 0x944
 */
// CMsg__cmd_END
/*
    stwu     r1, -0x20(r1)
    mflr     r0
    stw      r0, 0x24(r1)
    stw      r31, 0x1c(r1)
    mr       r31, r3
    stw      r30, 0x18(r1)
    lbz      r3, 0x8a4(r3)
    extrwi.  r0, r3, 1, 24
    beq      .L_800814A4
    extrwi.  r0, r3, 1, 26
    bne      .L_80081494
    lbz      r0, 0x0(r31)
    cmplwi   r0, 0x2b
    bne      .L_80080DF8
    addis    r4, r31, 0x4
    lwz      r0, 0x45d0(r4)
    cmpwi    r0, 0x0
    bge      .L_80080C00
    lwz      r0, 0x45d8(r4)
    lfs      f0, lbl_8065D058@sda21(r0)
    stw      r0, 0x45d0(r4)
    lwz      r0, 0x45d0(r4)
    stw      r0, 0x458c(r4)
    lwz      r0, 0x458c(r4)
    slwi     r0, r0, 2
    add      r3, r4, r0
    lfs      f1, 0x4548(r3)
    stfs     f1, 0x4590(r4)
    stfs     f0, 0x4594(r4)
    b        .L_80081494
.L_80080C00:
    lis      r4, lbl_8025C400@ha
    li       r3, 0x20
    addi     r30, r4, lbl_8025C400@l
    lwz      r5, 0x1b14(r30)
    li       r4, 0x0
    lwz      r0, 0x1b10(r30)
    and      r3, r5, r3
    and      r0, r0, r4
    xor      r3, r3, r4
    xor      r0, r0, r4
    or.      r0, r3, r0
    bne      .L_80081494
    addi     r3, r1, 0x10
    li       r4, 0x0
    bl       __ct__4CPadFUc
    lwz      r4, 0x1b10(r30)
    li       r0, 0x0
    lwz      r5, 0x1b14(r30)
    lis      r3, 0x8000
    and      r6, r4, r0
    and      r5, r5, r3
    xor      r3, r5, r0
    xor      r0, r6, r0
    or.      r0, r3, r0
    beq      .L_80080C6C
    lis      r4, 0x1
    b        .L_80080C74
.L_80080C6C:
    lis      r3, 0x1
    addi     r4, r3, 0x8
.L_80080C74:
    li       r0, 0x0
    xor      r3, r5, r0
    xor      r0, r6, r0
    or.      r0, r3, r0
    beq      .L_80080C90
    lis      r6, 0x2
    b        .L_80080C98
.L_80080C90:
    lis      r3, 0x2
    addi     r6, r3, 0x4
.L_80080C98:
    lwz      r5, 0x10(r1)
    lwz      r3, 0x1c(r5)
    and.     r0, r3, r4
    beq      .L_80080CF4
    addis    r4, r31, 0x4
    lwz      r3, 0x45d0(r4)
    subic.   r0, r3, 0x1
    stw      r0, 0x45d0(r4)
    bge      .L_80080CC8
    lwz      r3, 0x808(r31)
    subi     r0, r3, 0x1
    stw      r0, 0x45d0(r4)
.L_80080CC8:
    addis    r5, r31, 0x4
    lis      r3, lbl_8025C400@ha
    lwz      r0, 0x45d0(r5)
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    li       r4, 0x87
    stw      r0, 0x458c(r5)
    li       r5, 0xff
    addi     r3, r3, 0x7e24
    bl       fn_8001E904
    b        .L_80080DE8
.L_80080CF4:
    and.     r0, r3, r6
    beq      .L_80080D4C
    addis    r4, r31, 0x4
    lwz      r3, 0x45d0(r4)
    addi     r3, r3, 0x1
    stw      r3, 0x45d0(r4)
    lwz      r0, 0x808(r31)
    cmpw     r3, r0
    blt      .L_80080D20
    li       r0, 0x0
    stw      r0, 0x45d0(r4)
.L_80080D20:
    addis    r5, r31, 0x4
    lis      r3, lbl_8025C400@ha
    lwz      r0, 0x45d0(r5)
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    li       r4, 0x87
    stw      r0, 0x458c(r5)
    li       r5, 0xff
    addi     r3, r3, 0x7e24
    bl       fn_8001E904
    b        .L_80080DE8
.L_80080D4C:
    lwz      r3, 0x14(r5)
    rlwinm.  r0, r3, 0, 23, 23
    beq      .L_80080DA0
    addis    r8, r31, 0x4
    lis      r3, lbl_8025C400@ha
    lwz      r0, 0x45d0(r8)
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    li       r7, -0x1
    stw      r0, 0x45d4(r8)
    li       r6, 0x1
    li       r4, 0x88
    li       r5, 0xff
    stw      r7, 0x45d0(r8)
    addi     r3, r3, 0x7e24
    lbz      r0, 0x8a4(r31)
    rlwimi   r0, r6, 5, 26, 26
    stb      r0, 0x8a4(r31)
    stw      r7, 0x458c(r8)
    bl       fn_8001E904
    b        .L_80080DE8
.L_80080DA0:
    rlwinm.  r0, r3, 0, 22, 22
    beq      .L_80080DE8
    addis    r6, r31, 0x4
    li       r7, -0x1
    stw      r7, 0x45d4(r6)
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    li       r5, 0x1
    stw      r7, 0x45d0(r6)
    addis    r3, r3, 0xe
    li       r4, 0x86
    lbz      r0, 0x8a4(r31)
    rlwimi   r0, r5, 5, 26, 26
    li       r5, 0xff
    addi     r3, r3, 0x7e24
    stb      r0, 0x8a4(r31)
    stw      r7, 0x458c(r6)
    bl       fn_8001E904
.L_80080DE8:
    lis      r3, __vt__4CPad@ha
    addi     r0, r3, __vt__4CPad@l
    stw      r0, 0x14(r1)
    b        .L_80081494
.L_80080DF8:
    addis    r5, r31, 0x4
    lwz      r3, 0x45e8(r5)
    cmpwi    r3, 0x0
    ble      .L_8008142C
    lwz      r0, 0x45ac(r5)
    cmpwi    r0, 0x0
    bge      .L_80080E84
    lwz      r6, 0x45f0(r5)
    addi     r7, r31, 0x3
    lwz      r4, 0x45ec(r5)
    addis    r8, r7, 0x4
    lbz      r0, 0x45dc(r5)
    addi     r7, r31, 0x6
    stb      r0, 0x4598(r5)
    lbz      r0, 0x45dd(r5)
    stb      r0, 0x4599(r5)
    lbz      r0, 0x45de(r5)
    stb      r0, 0x459a(r5)
    lbz      r0, 0x45dc(r8)
    stb      r0, 0x4598(r8)
    lbz      r0, 0x45dd(r8)
    stb      r0, 0x4599(r8)
    lbz      r0, 0x45de(r8)
    stb      r0, 0x459a(r8)
    addis    r8, r7, 0x4
    lbz      r0, 0x45dc(r8)
    stb      r0, 0x4598(r8)
    lbz      r0, 0x45dd(r8)
    stb      r0, 0x4599(r8)
    lbz      r0, 0x45de(r8)
    stb      r0, 0x459a(r8)
    stw      r3, 0x45a4(r5)
    stw      r4, 0x45a8(r5)
    stw      r6, 0x45ac(r5)
    b        .L_80081494
.L_80080E84:
    addi     r3, r1, 0x8
    li       r4, 0x0
    bl       __ct__4CPadFUc
    lis      r4, lbl_8025C400@ha
    lis      r3, 0x8000
    addi     r5, r4, lbl_8025C400@l
    li       r0, 0x0
    lwz      r4, 0x1b10(r5)
    lwz      r5, 0x1b14(r5)
    and      r6, r4, r0
    and      r5, r5, r3
    xor      r3, r5, r0
    xor      r0, r6, r0
    or.      r0, r3, r0
    beq      .L_80080EC8
    lis      r4, 0x1
    b        .L_80080ED0
.L_80080EC8:
    lis      r3, 0x1
    addi     r4, r3, 0x8
.L_80080ED0:
    li       r0, 0x0
    xor      r3, r5, r0
    xor      r0, r6, r0
    or.      r0, r3, r0
    beq      .L_80080EEC
    lis      r7, 0x2
    b        .L_80080EF4
.L_80080EEC:
    lis      r3, 0x2
    addi     r7, r3, 0x4
.L_80080EF4:
    li       r0, 0x0
    xor      r3, r5, r0
    xor      r0, r6, r0
    or.      r0, r3, r0
    beq      .L_80080F10
    lis      r8, 0x4
    b        .L_80080F18
.L_80080F10:
    lis      r3, 0x4
    addi     r8, r3, 0x1
.L_80080F18:
    li       r0, 0x0
    xor      r3, r5, r0
    xor      r0, r6, r0
    or.      r0, r3, r0
    beq      .L_80080F34
    lis      r6, 0x8
    b        .L_80080F3C
.L_80080F34:
    lis      r3, 0x8
    addi     r6, r3, 0x2
.L_80080F3C:
    lwz      r5, 0x8(r1)
    lwz      r3, 0x1c(r5)
    and.     r0, r3, r4
    beq      .L_80081014
    addis    r5, r31, 0x4
    lwz      r0, 0x45f0(r5)
    add      r4, r5, r0
    lbz      r3, 0x45dc(r4)
    addi     r3, r3, 0x1
    extsb    r0, r3
    stb      r3, 0x45dc(r4)
    cmpwi    r0, 0x9
    ble      .L_80080F80
    lwz      r0, 0x45f0(r5)
    li       r4, 0x0
    add      r3, r5, r0
    stb      r4, 0x45dc(r3)
.L_80080F80:
    addis    r6, r31, 0x4
    addi     r10, r31, 0x3
    lwz      r9, 0x45f0(r6)
    addis    r11, r10, 0x4
    lwz      r8, 0x45ec(r6)
    lis      r3, lbl_8025C400@ha
    lwz      r7, 0x45e8(r6)
    addi     r3, r3, lbl_8025C400@l
    lbz      r0, 0x45dc(r6)
    addis    r3, r3, 0xe
    addi     r10, r31, 0x6
    li       r4, 0x87
    stb      r0, 0x4598(r6)
    li       r5, 0xff
    addi     r3, r3, 0x7e24
    lbz      r0, 0x45dd(r6)
    stb      r0, 0x4599(r6)
    lbz      r0, 0x45de(r6)
    stb      r0, 0x459a(r6)
    lbz      r0, 0x45dc(r11)
    stb      r0, 0x4598(r11)
    lbz      r0, 0x45dd(r11)
    stb      r0, 0x4599(r11)
    lbz      r0, 0x45de(r11)
    stb      r0, 0x459a(r11)
    addis    r11, r10, 0x4
    lbz      r0, 0x45dc(r11)
    stb      r0, 0x4598(r11)
    lbz      r0, 0x45dd(r11)
    stb      r0, 0x4599(r11)
    lbz      r0, 0x45de(r11)
    stb      r0, 0x459a(r11)
    stw      r7, 0x45a4(r6)
    stw      r8, 0x45a8(r6)
    stw      r9, 0x45ac(r6)
    bl       fn_8001E904
    b        .L_8008141C
.L_80081014:
    and.     r0, r3, r7
    beq      .L_800810E0
    addis    r5, r31, 0x4
    lwz      r0, 0x45f0(r5)
    add      r4, r5, r0
    lbz      r3, 0x45dc(r4)
    subi     r3, r3, 0x1
    extsb.   r0, r3
    stb      r3, 0x45dc(r4)
    bge      .L_8008104C
    lwz      r0, 0x45f0(r5)
    li       r4, 0x9
    add      r3, r5, r0
    stb      r4, 0x45dc(r3)
.L_8008104C:
    addis    r6, r31, 0x4
    addi     r10, r31, 0x3
    lwz      r9, 0x45f0(r6)
    addis    r11, r10, 0x4
    lwz      r8, 0x45ec(r6)
    lis      r3, lbl_8025C400@ha
    lwz      r7, 0x45e8(r6)
    addi     r3, r3, lbl_8025C400@l
    lbz      r0, 0x45dc(r6)
    addis    r3, r3, 0xe
    addi     r10, r31, 0x6
    li       r4, 0x87
    stb      r0, 0x4598(r6)
    li       r5, 0xff
    addi     r3, r3, 0x7e24
    lbz      r0, 0x45dd(r6)
    stb      r0, 0x4599(r6)
    lbz      r0, 0x45de(r6)
    stb      r0, 0x459a(r6)
    lbz      r0, 0x45dc(r11)
    stb      r0, 0x4598(r11)
    lbz      r0, 0x45dd(r11)
    stb      r0, 0x4599(r11)
    lbz      r0, 0x45de(r11)
    stb      r0, 0x459a(r11)
    addis    r11, r10, 0x4
    lbz      r0, 0x45dc(r11)
    stb      r0, 0x4598(r11)
    lbz      r0, 0x45dd(r11)
    stb      r0, 0x4599(r11)
    lbz      r0, 0x45de(r11)
    stb      r0, 0x459a(r11)
    stw      r7, 0x45a4(r6)
    stw      r8, 0x45a8(r6)
    stw      r9, 0x45ac(r6)
    bl       fn_8001E904
    b        .L_8008141C
.L_800810E0:
    and.     r0, r3, r8
    beq      .L_8008119C
    addis    r7, r31, 0x4
    lwz      r3, 0x45f0(r7)
    addi     r3, r3, 0x1
    stw      r3, 0x45f0(r7)
    lwz      r6, 0x45e8(r7)
    subi     r0, r6, 0x1
    cmpw     r3, r0
    ble      .L_80081110
    stw      r0, 0x45f0(r7)
    b        .L_8008141C
.L_80081110:
    lwz      r9, 0x45f0(r7)
    addi     r10, r31, 0x3
    lwz      r8, 0x45ec(r7)
    addis    r11, r10, 0x4
    lbz      r0, 0x45dc(r7)
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addi     r10, r31, 0x6
    stb      r0, 0x4598(r7)
    addis    r3, r3, 0xe
    li       r4, 0x87
    li       r5, 0xff
    lbz      r0, 0x45dd(r7)
    addi     r3, r3, 0x7e24
    stb      r0, 0x4599(r7)
    lbz      r0, 0x45de(r7)
    stb      r0, 0x459a(r7)
    lbz      r0, 0x45dc(r11)
    stb      r0, 0x4598(r11)
    lbz      r0, 0x45dd(r11)
    stb      r0, 0x4599(r11)
    lbz      r0, 0x45de(r11)
    stb      r0, 0x459a(r11)
    addis    r11, r10, 0x4
    lbz      r0, 0x45dc(r11)
    stb      r0, 0x4598(r11)
    lbz      r0, 0x45dd(r11)
    stb      r0, 0x4599(r11)
    lbz      r0, 0x45de(r11)
    stb      r0, 0x459a(r11)
    stw      r6, 0x45a4(r7)
    stw      r8, 0x45a8(r7)
    stw      r9, 0x45ac(r7)
    bl       fn_8001E904
    b        .L_8008141C
.L_8008119C:
    and.     r0, r3, r6
    beq      .L_80081254
    addis    r7, r31, 0x4
    lwz      r3, 0x45f0(r7)
    subi     r0, r3, 0x1
    stw      r0, 0x45f0(r7)
    lwz      r6, 0x45ec(r7)
    cmpw     r0, r6
    bge      .L_800811C8
    stw      r6, 0x45f0(r7)
    b        .L_8008141C
.L_800811C8:
    lwz      r9, 0x45f0(r7)
    addi     r10, r31, 0x3
    lwz      r8, 0x45e8(r7)
    addis    r11, r10, 0x4
    lbz      r0, 0x45dc(r7)
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addi     r10, r31, 0x6
    stb      r0, 0x4598(r7)
    addis    r3, r3, 0xe
    li       r4, 0x87
    li       r5, 0xff
    lbz      r0, 0x45dd(r7)
    addi     r3, r3, 0x7e24
    stb      r0, 0x4599(r7)
    lbz      r0, 0x45de(r7)
    stb      r0, 0x459a(r7)
    lbz      r0, 0x45dc(r11)
    stb      r0, 0x4598(r11)
    lbz      r0, 0x45dd(r11)
    stb      r0, 0x4599(r11)
    lbz      r0, 0x45de(r11)
    stb      r0, 0x459a(r11)
    addis    r11, r10, 0x4
    lbz      r0, 0x45dc(r11)
    stb      r0, 0x4598(r11)
    lbz      r0, 0x45dd(r11)
    stb      r0, 0x4599(r11)
    lbz      r0, 0x45de(r11)
    stb      r0, 0x459a(r11)
    stw      r8, 0x45a4(r7)
    stw      r6, 0x45a8(r7)
    stw      r9, 0x45ac(r7)
    bl       fn_8001E904
    b        .L_8008141C
.L_80081254:
    lwz      r3, 0x14(r5)
    rlwinm.  r0, r3, 0, 23, 23
    beq      .L_800813C8
    lbz      r0, 0x8a4(r31)
    li       r3, 0x1
    rlwimi   r0, r3, 5, 26, 26
    addis    r4, r31, 0x4
    lis      r3, lbl_8025C400@ha
    stb      r0, 0x8a4(r31)
    addi     r3, r3, lbl_8025C400@l
    li       r0, -0x1
    addis    r3, r3, 0xe
    stw      r0, 0x45ac(r4)
    li       r4, 0x88
    li       r5, 0xff
    addi     r3, r3, 0x7e24
    bl       fn_8001E904
    li       r7, 0x9
    li       r0, 0x0
    addis    r6, r7, 0x4
    lis      r3, lbl_8025C400@ha
    addi     r6, r6, 0x45dc
    li       r7, 0x8
    lbzx     r5, r31, r6
    mulli    r0, r0, 0xa
    addis    r6, r7, 0x4
    extsb    r4, r5
    add      r0, r4, r0
    addi     r6, r6, 0x45dc
    lbzx     r5, r31, r6
    mulli    r0, r0, 0xa
    li       r7, 0x7
    extsb    r4, r5
    addis    r6, r7, 0x4
    addi     r3, r3, lbl_8025C400@l
    add      r0, r4, r0
    li       r7, 0x6
    addi     r6, r6, 0x45dc
    addis    r3, r3, 0x6
    lbzx     r5, r31, r6
    addis    r6, r7, 0x4
    mulli    r0, r0, 0xa
    li       r7, 0x5
    extsb    r4, r5
    addi     r6, r6, 0x45dc
    add      r0, r4, r0
    lbzx     r5, r31, r6
    addis    r6, r7, 0x4
    li       r7, 0x4
    mulli    r0, r0, 0xa
    extsb    r4, r5
    addi     r6, r6, 0x45dc
    lbzx     r5, r31, r6
    add      r0, r4, r0
    addis    r6, r7, 0x4
    mulli    r0, r0, 0xa
    extsb    r4, r5
    addi     r6, r6, 0x45dc
    lbzx     r5, r31, r6
    li       r7, 0x3
    add      r0, r4, r0
    addis    r6, r7, 0x4
    extsb    r4, r5
    mulli    r0, r0, 0xa
    li       r7, 0x2
    addi     r6, r6, 0x45dc
    lbzx     r5, r31, r6
    add      r0, r4, r0
    addis    r6, r7, 0x4
    mulli    r0, r0, 0xa
    extsb    r4, r5
    addi     r6, r6, 0x45dc
    lbzx     r5, r31, r6
    li       r7, 0x1
    add      r0, r4, r0
    addis    r6, r7, 0x4
    extsb    r4, r5
    mulli    r0, r0, 0xa
    li       r7, 0x0
    addi     r6, r6, 0x45dc
    lbzx     r5, r31, r6
    add      r0, r4, r0
    addis    r6, r7, 0x4
    mulli    r0, r0, 0xa
    extsb    r4, r5
    addi     r6, r6, 0x45dc
    lbzx     r5, r31, r6
    add      r0, r4, r0
    mulli    r0, r0, 0xa
    extsb    r4, r5
    add      r0, r4, r0
    stw      r0, -0x2f74(r3)
    b        .L_8008141C
.L_800813C8:
    rlwinm.  r0, r3, 0, 22, 22
    beq      .L_8008141C
    lbz      r0, 0x8a4(r31)
    li       r3, 0x1
    rlwimi   r0, r3, 5, 26, 26
    addis    r4, r31, 0x4
    lis      r3, lbl_8025C400@ha
    stb      r0, 0x8a4(r31)
    addi     r3, r3, lbl_8025C400@l
    li       r0, -0x1
    addis    r3, r3, 0xe
    stw      r0, 0x45ac(r4)
    li       r4, 0x86
    li       r5, 0xff
    addi     r3, r3, 0x7e24
    bl       fn_8001E904
    lis      r3, lbl_8025C400@ha
    li       r0, -0x1
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x6
    stw      r0, -0x2f74(r3)
.L_8008141C:
    lis      r3, __vt__4CPad@ha
    addi     r0, r3, __vt__4CPad@l
    stw      r0, 0xc(r1)
    b        .L_80081494
.L_8008142C:
    lwz      r4, 0x45f8(r5)
    cmpwi    r4, 0x0
    beq      .L_80081484
    cmpwi    r4, 0x1
    bne      .L_80081470
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x1
    lwz      r3, -0x6944(r3)
    addis    r3, r3, 0x1
    lwz      r0, 0x3394(r3)
    cmpwi    r0, 0x0
    beq      .L_80081494
    bge      .L_80081494
    addi     r0, r4, 0x1
    stw      r0, 0x45f8(r5)
    b        .L_80081494
.L_80081470:
    lbz      r0, 0x8a4(r31)
    li       r3, 0x1
    rlwimi   r0, r3, 5, 26, 26
    stb      r0, 0x8a4(r31)
    b        .L_80081494
.L_80081484:
    lbz      r0, 0x8a4(r31)
    li       r3, 0x1
    rlwimi   r0, r3, 5, 26, 26
    stb      r0, 0x8a4(r31)
.L_80081494:
    lwz      r3, 0x804(r31)
    subi     r0, r3, 0x1
    stw      r0, 0x804(r31)
    b        .L_800814B0
.L_800814A4:
    li       r0, 0x0
    stw      r0, 0x804(r31)
    stw      r0, 0x898(r31)
.L_800814B0:
    lwz      r0, 0x24(r1)
    li       r3, 0x1
    lwz      r31, 0x1c(r1)
    lwz      r30, 0x18(r1)
    mtlr     r0
    addi     r1, r1, 0x20
    blr
*/

/**
 * @note Address: 0x800814CC
 * @note Size: 0xD0
 */
// CMsg_Draw
extern "C" void CMsg_Draw(CMsg* msg)
{
	fn_80199AB0(lbl_8025C400, 0, 0);
	fn_8019B56C(GAME_290, 1, 4, 5, 15);
	fn_8019B464(GAME_290, 0, 3, 0);
	if (msg->_445C8 == 0.0f) {
		CMsgFont_Draw(&msg->_8A0, 1.0f);
	} else {
		f32 t = (msg->_445C8 - msg->_445C4) / msg->_445C8;
		if (t > 1.0f) {
			t = 1.0f;
		}
		if (t < 0.0f) {
			t = 0.0f;
		}
		CMsgFont_Draw(&msg->_8A0, t);
	}
}

/**
 * @note Address: 0x8008159C
 * @note Size: 0x6D8
 */
// CMsg_Update
/*
    stwu     r1, -0x40(r1)
    mflr     r0
    stw      r0, 0x44(r1)
    stmw     r25, 0x24(r1)
    mr       r31, r3
    mr       r30, r4
    lwz      r0, 0x804(r3)
    cmplwi   r0, 0x0
    beq      .L_80081C60
    lis      r3, lbl_8025C400@ha
    li       r25, 0x0
    addi     r28, r3, lbl_8025C400@l
    addis    r27, r28, 0xe
    addis    r29, r28, 0x1
    addis    r26, r28, 0x6
    addi     r27, r27, 0x7e24
    b        .L_80081C4C
.L_800815E0:
    lbz      r0, 0x8a4(r31)
    extrwi.  r0, r0, 1, 25
    beq      .L_800815F4
    li       r25, 0x1
    b        .L_80081C4C
.L_800815F4:
    addis    r3, r31, 0x4
    lfs      f2, lbl_8065D058@sda21(r0)
    lfs      f3, 0x45cc(r3)
    fcmpo    cr0, f3, f2
    ble      .L_80081640
    lis      r0, 0x4330
    stw      r30, 0x1c(r1)
    lfd      f1, lbl_8065D048@sda21(r0)
    stw      r0, 0x18(r1)
    lfd      f0, 0x18(r1)
    fsubs    f0, f0, f1
    fsubs    f0, f3, f0
    fcmpo    cr0, f0, f2
    stfs     f0, 0x45cc(r3)
    bge      .L_80081638
    stfs     f2, 0x45cc(r3)
    b        .L_80081C4C
.L_80081638:
    li       r25, 0x1
    b        .L_80081C4C
.L_80081640:
    lfs      f3, 0x45c4(r3)
    fcmpo    cr0, f3, f2
    bge      .L_80081654
    li       r25, 0x1
    b        .L_80081C4C
.L_80081654:
    ble      .L_800816BC
    lis      r0, 0x4330
    stw      r30, 0x1c(r1)
    lfd      f1, lbl_8065D048@sda21(r0)
    stw      r0, 0x18(r1)
    lfd      f0, 0x18(r1)
    fsubs    f0, f0, f1
    fsubs    f0, f3, f0
    fcmpo    cr0, f0, f2
    stfs     f0, 0x45c4(r3)
    bge      .L_800816B4
    stfs     f2, 0x45c4(r3)
    lfs      f3, 0x45c8(r3)
    fcmpo    cr0, f3, f2
    ble      .L_80081C4C
    stw      r30, 0x1c(r1)
    stw      r0, 0x18(r1)
    lfd      f0, 0x18(r1)
    fsubs    f0, f0, f1
    fsubs    f0, f3, f0
    fcmpo    cr0, f0, f2
    bge      .L_80081C4C
    li       r25, 0x1
    b        .L_80081C4C
.L_800816B4:
    li       r25, 0x1
    b        .L_80081C4C
.L_800816BC:
    lwz      r5, 0x804(r31)
    lbz      r4, 0x0(r5)
    addi     r0, r5, 0x1
    cmplwi   r4, 0x18
    stw      r0, 0x804(r31)
    bgt      .L_80081B68
    lis      r5, jumptable_802254CC@ha
    slwi     r0, r4, 2
    addi     r5, r5, jumptable_802254CC@l
    lwzx     r0, r5, r0
    mtctr    r0
    bctr
.L_800816EC:
    lwz      r4, 0x451c(r3)
    addi     r0, r4, 0x1
    stw      r0, 0x451c(r3)
    lbz      r0, 0x8a4(r31)
    extrwi.  r0, r0, 1, 28
    beq      .L_8008171C
    lwz      r0, 0x451c(r3)
    lwz      r4, 0x8a0(r31)
    slwi     r3, r0, 2
    addi     r0, r3, 0x830
    lfsx     f0, r4, r0
    b        .L_80081720
.L_8008171C:
    lfs      f0, 0x4538(r3)
.L_80081720:
    lfs      f3, lbl_8065D068@sda21(r0)
    addis    r5, r31, 0x4
    lfs      f1, lbl_8065D06C@sda21(r0)
    li       r25, 0x0
    fmuls    f0, f0, f3
    fneg     f0, f0
    stfs     f0, 0x4530(r5)
    lwz      r3, 0x451c(r5)
    lwz      r4, 0x8a0(r31)
    subi     r0, r3, 0x1
    lfs      f2, 0x453c(r5)
    slwi     r3, r0, 2
    addi     r0, r3, 0x858
    lfsx     f0, r4, r0
    fadds    f0, f2, f0
    stfs     f0, 0x453c(r5)
    lwz      r0, 0x451c(r5)
    lwz      r4, 0x8a0(r31)
    slwi     r3, r0, 2
    lfs      f0, 0x453c(r5)
    addi     r0, r3, 0x858
    lfsx     f2, r4, r0
    addis    r3, r3, 0x4
    addi     r3, r3, 0x4548
    fsubs    f1, f2, f1
    fmadds   f0, f1, f3, f0
    stfsx    f0, r31, r3
    b        .L_80081C4C
.L_80081790:
    lwz      r4, 0x451c(r3)
    addi     r0, r4, 0x1
    stw      r0, 0x451c(r3)
    lbz      r0, 0x8a4(r31)
    extrwi.  r0, r0, 1, 28
    beq      .L_800817C0
    lwz      r0, 0x451c(r3)
    lwz      r4, 0x8a0(r31)
    slwi     r3, r0, 2
    addi     r0, r3, 0x830
    lfsx     f0, r4, r0
    b        .L_800817C4
.L_800817C0:
    lfs      f0, 0x4538(r3)
.L_800817C4:
    lfs      f3, lbl_8065D068@sda21(r0)
    addis    r5, r31, 0x4
    lfs      f1, lbl_8065D06C@sda21(r0)
    li       r25, 0x0
    fmuls    f0, f0, f3
    fneg     f0, f0
    stfs     f0, 0x4530(r5)
    lwz      r3, 0x451c(r5)
    lwz      r4, 0x8a0(r31)
    subi     r0, r3, 0x1
    lfs      f2, 0x453c(r5)
    slwi     r3, r0, 2
    addi     r0, r3, 0x858
    lfsx     f0, r4, r0
    fadds    f0, f2, f0
    stfs     f0, 0x453c(r5)
    lwz      r0, 0x451c(r5)
    lwz      r4, 0x8a0(r31)
    slwi     r3, r0, 2
    lfs      f0, 0x453c(r5)
    addi     r0, r3, 0x858
    lfsx     f2, r4, r0
    addis    r3, r3, 0x4
    addi     r3, r3, 0x4548
    fsubs    f1, f2, f1
    fmadds   f0, f1, f3, f0
    stfsx    f0, r31, r3
    b        .L_80081C4C
.L_80081834:
    mr       r3, r31
    bl       CMsg__cmd_END
    mr       r25, r3
    b        .L_80081C4C
.L_80081844:
    lwz      r3, 0x804(r31)
    lbz      r4, 0x0(r3)
    addi     r0, r3, 0x1
    cmpwi    r4, 0xff
    stw      r0, 0x804(r31)
    bne      .L_80081874
    lbz      r0, 0x0(r31)
    cmplwi   r0, 0x23
    bne      .L_80081870
    li       r4, 0x2
    b        .L_80081874
.L_80081870:
    li       r4, 0x0
.L_80081874:
    xoris    r3, r4, 0x8000
    lis      r0, 0x4330
    stw      r3, 0x1c(r1)
    addis    r3, r31, 0x4
    lfd      f1, lbl_8065D050@sda21(r0)
    li       r25, 0x0
    stw      r0, 0x18(r1)
    lfd      f0, 0x18(r1)
    fsubs    f0, f0, f1
    stfs     f0, 0x45c8(r3)
    b        .L_80081C4C
.L_800818A0:
    mr       r3, r31
    bl       CMsg__cmd_WAIT
    mr       r25, r3
    b        .L_80081C4C
.L_800818B0:
    mr       r3, r31
    bl       CMsg__cmd_ANIM
    mr       r25, r3
    b        .L_80081C4C
.L_800818C0:
    lwz      r4, 0x804(r31)
    lbz      r3, 0x0(r4)
    addi     r0, r4, 0x1
    stw      r0, 0x804(r31)
    clrlwi.  r0, r3, 31
    lwz      r3, 0x804(r31)
    lbz      r5, 0x0(r3)
    addi     r0, r3, 0x1
    stw      r0, 0x804(r31)
    lwz      r4, 0x804(r31)
    lbz      r3, 0x0(r4)
    addi     r0, r4, 0x1
    stw      r0, 0x804(r31)
    rlwimi   r5, r3, 8, 16, 23
    lwz      r4, 0x804(r31)
    lbz      r3, 0x0(r4)
    addi     r0, r4, 0x1
    stw      r0, 0x804(r31)
    slwi     r0, r3, 16
    or       r5, r5, r0
    lwz      r3, 0x804(r31)
    lbz      r0, 0x0(r3)
    addi     r3, r3, 0x1
    slwi     r0, r0, 24
    stw      r3, 0x804(r31)
    or       r5, r5, r0
    beq      .L_8008193C
    lis      r3, lbl_80367A2C@ha
    slwi     r0, r5, 2
    addi     r3, r3, lbl_80367A2C@l
    lwzx     r5, r3, r0
.L_8008193C:
    addis    r3, r31, 0x4
    li       r25, 0x0
    stw      r5, 0x45d8(r3)
    b        .L_80081C4C
.L_8008194C:
    mr       r3, r31
    bl       CMsg__cmd_SFX
    mr       r25, r3
    b        .L_80081C4C
.L_8008195C:
    lwz      r5, 0x804(r31)
    lbz      r4, 0x0(r5)
    addi     r0, r5, 0x1
    stw      r0, 0x804(r31)
    stw      r4, 0x45fc(r3)
    lwz      r0, 0x45fc(r3)
    cmpwi    r0, 0xff
    bne      .L_80081984
    li       r0, -0x1
    stw      r0, 0x45fc(r3)
.L_80081984:
    li       r25, 0x0
    b        .L_80081C4C
.L_8008198C:
    mr       r3, r31
    bl       CMsg__cmd_SETFONT
    mr       r25, r3
    b        .L_80081C4C
.L_8008199C:
    mr       r3, r31
    bl       CMsg__cmd_SETFONT
    mr       r25, r3
    b        .L_80081C4C
.L_800819AC:
    mr       r3, r31
    bl       CMsg__cmd_SETFONT
    mr       r25, r3
    b        .L_80081C4C
.L_800819BC:
    mr       r3, r31
    bl       CMsg__cmd_SETFONT
    mr       r25, r3
    b        .L_80081C4C
.L_800819CC:
    mr       r3, r31
    bl       CMsg__cmd_SETFONT
    mr       r25, r3
    b        .L_80081C4C
.L_800819DC:
    mr       r3, r31
    bl       CMsg__cmd_SETFONT
    mr       r25, r3
    b        .L_80081C4C
.L_800819EC:
    mr       r3, r31
    bl       CMsg__cmd_SETFONT
    mr       r25, r3
    b        .L_80081C4C
.L_800819FC:
    mr       r3, r31
    bl       CMsg__cmd_SETFONT
    mr       r25, r3
    b        .L_80081C4C
.L_80081A0C:
    lwz      r5, 0x804(r31)
    lbz      r4, 0x0(r5)
    addi     r0, r5, 0x1
    stw      r0, 0x804(r31)
    stw      r4, 0x45e8(r3)
    lwz      r5, 0x804(r31)
    lbz      r4, 0x0(r5)
    addi     r0, r5, 0x1
    stw      r0, 0x804(r31)
    stw      r4, 0x45ec(r3)
    lwz      r4, 0x45ec(r3)
    lwz      r0, 0x45e8(r3)
    subf     r0, r4, r0
    stw      r0, 0x45ec(r3)
    lwz      r5, 0x804(r31)
    lbz      r4, 0x0(r5)
    addi     r0, r5, 0x1
    stw      r0, 0x804(r31)
    subi     r0, r4, 0x1
    stw      r0, 0x45f0(r3)
    lwz      r4, 0x45e8(r3)
    lwz      r5, 0x45f0(r3)
    subi     r0, r4, 0x1
    cmpw     r5, r0
    ble      .L_80081A78
    stw      r0, 0x45f0(r3)
    b        .L_80081A88
.L_80081A78:
    lwz      r0, 0x45ec(r3)
    cmpw     r5, r0
    bge      .L_80081A88
    stw      r0, 0x45f0(r3)
.L_80081A88:
    li       r25, 0x0
    b        .L_80081C4C
.L_80081A90:
    lwz      r3, 0x804(r31)
    lbz      r4, 0x0(r3)
    addi     r0, r3, 0x1
    stw      r0, 0x804(r31)
    lwz      r3, 0x804(r31)
    lbz      r5, 0x0(r3)
    addi     r0, r3, 0x1
    stw      r0, 0x804(r31)
    lwz      r3, -0x6944(r29)
    addis    r6, r3, 0x1
    lwz      r0, 0x3394(r6)
    cmpwi    r0, 0x0
    bne      .L_80081ADC
    bl       fn_800B51FC
    lwz      r3, 0x804(r31)
    li       r5, 0x1
    subi     r0, r3, 0x3
    stw      r0, 0x804(r31)
    b        .L_80081B50
.L_80081ADC:
    addi     r3, r1, 0xc
    li       r4, 0x0
    bl       __ct__4CPadFUc
    lwz      r3, -0x6944(r29)
    addi     r4, r1, 0xc
    addis    r3, r3, 0x1
    addi     r3, r3, 0x3390
    bl       fn_80096E38
    lwz      r3, 0xc(r1)
    lwz      r0, 0x14(r3)
    rlwinm.  r0, r0, 0, 23, 23
    beq      .L_80081B20
    lwz      r3, -0x6944(r29)
    addis    r3, r3, 0x1
    lwz      r0, 0x46d0(r3)
    stw      r0, -0x2f74(r26)
    b        .L_80081B40
.L_80081B20:
    lwz      r4, 0x804(r31)
    lis      r3, __vt__4CPad@ha
    addi     r0, r3, __vt__4CPad@l
    li       r5, 0x1
    subi     r3, r4, 0x3
    stw      r3, 0x804(r31)
    stw      r0, 0x10(r1)
    b        .L_80081B50
.L_80081B40:
    lis      r3, __vt__4CPad@ha
    li       r5, 0x0
    addi     r0, r3, __vt__4CPad@l
    stw      r0, 0x10(r1)
.L_80081B50:
    mr       r25, r5
    b        .L_80081C4C
.L_80081B58:
    li       r0, 0x1
    li       r25, 0x0
    stw      r0, 0x45f8(r3)
    b        .L_80081C4C
.L_80081B68:
    lbz      r0, 0x3318(r28)
    cmplwi   r0, 0x6
    bne      .L_80081BD0
    clrlwi   r0, r4, 24
    li       r3, 0x0
    cmplwi   r0, 0x81
    blt      .L_80081B8C
    cmplwi   r0, 0x9f
    ble      .L_80081BA0
.L_80081B8C:
    clrlwi   r0, r4, 24
    cmplwi   r0, 0xe0
    blt      .L_80081BA4
    cmplwi   r0, 0xfc
    bgt      .L_80081BA4
.L_80081BA0:
    li       r3, 0x1
.L_80081BA4:
    clrlwi.  r0, r3, 24
    beq      .L_80081BD0
    stb      r4, 0x8(r1)
    li       r0, 0x0
    lwz      r3, 0x804(r31)
    lbz      r4, 0x0(r3)
    addi     r3, r3, 0x1
    stw      r3, 0x804(r31)
    stb      r4, 0x9(r1)
    stb      r0, 0xa(r1)
    b        .L_80081C0C
.L_80081BD0:
    clrlwi   r0, r4, 24
    cmplwi   r0, 0x11
    bne      .L_80081C00
    stb      r4, 0x8(r1)
    li       r0, 0x0
    lwz      r3, 0x804(r31)
    lbz      r4, 0x0(r3)
    addi     r3, r3, 0x1
    stw      r3, 0x804(r31)
    stb      r4, 0x9(r1)
    stb      r0, 0xa(r1)
    b        .L_80081C0C
.L_80081C00:
    li       r0, 0x0
    stb      r4, 0x8(r1)
    stb      r0, 0x9(r1)
.L_80081C0C:
    addi     r3, r31, 0x8a0
    addi     r4, r1, 0x8
    bl       CMsgFont_AddChar
    addis    r3, r31, 0x4
    lfs      f0, 0x45c8(r3)
    stfs     f0, 0x45c4(r3)
    lwz      r4, 0x45fc(r3)
    cmpwi    r4, 0x0
    blt      .L_80081C4C
    lfs      f1, 0x4570(r3)
    mr       r3, r27
    lfs      f0, lbl_8065D06C@sda21(r0)
    lbz      r5, 0x8(r1)
    fdivs    f1, f1, f0
    lbz      r6, 0x9(r1)
    bl       fn_8001DC20
.L_80081C4C:
    cmpwi    r25, 0x0
    beq      .L_800815E0
    mr       r4, r30
    addi     r3, r31, 0x8a0
    bl       CMsgFont_Update
.L_80081C60:
    lmw      r25, 0x24(r1)
    lwz      r0, 0x44(r1)
    mtlr     r0
    addi     r1, r1, 0x40
    blr
*/

/**
 * @note Address: 0x80081C74
 * @note Size: 0x508
 */
// CMsg_Open
/*
    stwu     r1, -0x150(r1)
    mflr     r0
    lis      r8, msgColorTable@ha
    li       r7, 0x0
    stw      r0, 0x154(r1)
    addi     r12, r8, msgColorTable@l
    lis      r10, lbl_80208ED0@ha
    lfs      f2, lbl_8065D06C@sda21(r0)
    stmw     r23, 0x12c(r1)
    mr       r30, r3
    addi     r24, r10, lbl_80208ED0@l
    lis      r9, __float_nan@ha
    addi     r23, r9, __float_nan@l
    mr       r10, r5
    mr       r31, r6
    li       r26, 0x1
    addis    r25, r30, 0x4
    lfs      f1, lbl_8065D058@sda21(r0)
    lfs      f0, lbl_8065D05C@sda21(r0)
    li       r11, -0x1
    mr       r8, r7
    mr       r9, r30
    stw      r7, 0x804(r3)
    stw      r7, 0x898(r3)
    lis      r3, 0xb60b
    addi     r6, r3, 0x60b7
    lbz      r0, 0x8a4(r30)
    rlwimi   r0, r7, 7, 24, 24
    stb      r0, 0x8a4(r30)
    lbz      r0, 0x8a4(r30)
    rlwimi   r0, r7, 6, 25, 25
    stb      r0, 0x8a4(r30)
    li       r0, 0x32
    lbz      r5, 0x8a4(r30)
    rlwimi   r5, r7, 5, 26, 26
    stb      r5, 0x8a4(r30)
    lbz      r3, 0x8a4(r30)
    rlwimi   r3, r7, 4, 27, 27
    stb      r3, 0x8a4(r30)
    lbz      r3, 0x8a4(r30)
    rlwimi   r3, r26, 3, 28, 28
    stb      r3, 0x8a4(r30)
    lbz      r3, 0x8a4(r30)
    rlwimi   r3, r26, 2, 29, 29
    stb      r3, 0x8a4(r30)
    stw      r7, 0x4518(r25)
    stw      r7, 0x451c(r25)
    stw      r7, 0x4520(r25)
    lwz      r3, 0x0(r24)
    lwz      r5, 0x4(r24)
    stw      r3, 0x8(r1)
    lwz      r3, 0x8(r24)
    stw      r5, 0xc(r1)
    lfs      f3, 0x8(r1)
    stw      r3, 0x10(r1)
    lfs      f4, 0xc(r1)
    stfs     f3, 0x4524(r25)
    lfs      f3, 0x10(r1)
    stfs     f4, 0x4528(r25)
    stfs     f3, 0x452c(r25)
    stfs     f2, 0x4570(r25)
    lfs      f2, 0x0(r23)
    stfs     f2, 0x4574(r25)
    lwz      r3, 0x0(r12)
    stw      r3, 0x4578(r25)
    stfs     f1, 0x457c(r25)
    stfs     f0, 0x4584(r25)
    stfs     f0, 0x4580(r25)
    stb      r7, 0x4588(r25)
    stw      r11, 0x458c(r25)
    stw      r11, 0x45ac(r25)
    stfs     f1, 0x45b0(r25)
    stfs     f0, 0x45b4(r25)
    stfs     f1, 0x45b8(r25)
    stw      r7, 0x450c(r25)
    stfs     f1, 0x4510(r25)
    stfs     f1, 0x4514(r25)
    mtctr    r0
.L_80081DAC:
    addi     r0, r7, 0x1
    addi     r5, r7, 0x2
    mulli    r0, r0, 0x4b
    addi     r3, r7, 0x3
    addi     r28, r7, 0x5
    addi     r12, r7, 0x6
    mulhw    r11, r6, r8
    addi     r27, r7, 0x4
    mulhw    r24, r6, r0
    add      r11, r11, r8
    srawi    r25, r11, 8
    addi     r11, r7, 0x7
    srwi     r23, r25, 31
    addi     r7, r7, 0x8
    add      r26, r24, r0
    add      r23, r25, r23
    mulli    r5, r5, 0x4b
    srawi    r26, r26, 8
    srwi     r24, r26, 31
    add      r24, r26, r24
    mulhw    r25, r6, r5
    mulli    r23, r23, 0x168
    add      r26, r25, r5
    subf     r23, r23, r8
    srawi    r29, r26, 8
    mulli    r3, r3, 0x4b
    sth      r23, 0x15f0(r9)
    srwi     r25, r29, 31
    sth      r23, 0x15ee(r9)
    addi     r8, r8, 0x258
    mulli    r24, r24, 0x168
    add      r25, r29, r25
    subf     r24, r24, r0
    mulhw    r26, r6, r3
    sth      r24, 0x189c(r9)
    sth      r24, 0x189a(r9)
    mulli    r0, r27, 0x4b
    add      r29, r26, r3
    mulli    r25, r25, 0x168
    srawi    r29, r29, 8
    srwi     r26, r29, 31
    subf     r5, r25, r5
    sth      r5, 0x1b48(r9)
    mulhw    r27, r6, r0
    add      r26, r29, r26
    sth      r5, 0x1b46(r9)
    mulli    r28, r28, 0x4b
    add      r29, r27, r0
    srawi    r29, r29, 8
    srwi     r27, r29, 31
    mulhw    r5, r6, r28
    add      r27, r29, r27
    mulli    r29, r26, 0x168
    add      r5, r5, r28
    subf     r3, r29, r3
    srawi    r5, r5, 8
    sth      r3, 0x1df4(r9)
    srwi     r29, r5, 31
    add      r29, r5, r29
    mulli    r5, r27, 0x168
    sth      r3, 0x1df2(r9)
    subf     r0, r5, r0
    mulli    r12, r12, 0x4b
    sth      r0, 0x20a0(r9)
    sth      r0, 0x209e(r9)
    mulhw    r3, r6, r12
    mulli    r5, r11, 0x4b
    add      r3, r3, r12
    srawi    r3, r3, 8
    srwi     r11, r3, 31
    mulhw    r0, r6, r5
    add      r3, r3, r11
    mulli    r11, r29, 0x168
    add      r0, r0, r5
    subf     r29, r11, r28
    srawi    r0, r0, 8
    mulli    r11, r3, 0x168
    sth      r29, 0x234c(r9)
    srwi     r3, r0, 31
    sth      r29, 0x234a(r9)
    subf     r11, r11, r12
    add      r0, r0, r3
    sth      r11, 0x25f8(r9)
    mulli    r0, r0, 0x168
    sth      r11, 0x25f6(r9)
    subf     r0, r0, r5
    sth      r0, 0x28a4(r9)
    sth      r0, 0x28a2(r9)
    addi     r9, r9, 0x1560
    bdnz     .L_80081DAC
    addis    r7, r30, 0x4
    li       r0, 0x1
    stw      r0, 0x45c0(r7)
    li       r6, 0x0
    lfs      f0, lbl_8065D058@sda21(r0)
    addis    r9, r6, 0x4
    addi     r9, r9, 0x45dc
    li       r0, -0x1
    stfs     f0, 0x45c8(r7)
    add      r9, r30, r9
    li       r8, 0x3
    mr       r5, r31
    stfs     f0, 0x45c4(r7)
    addi     r3, r30, 0x4
    stw      r0, 0x45d4(r7)
    stw      r0, 0x45d0(r7)
    stw      r6, 0x45d8(r7)
    stb      r6, 0x0(r9)
    stb      r6, 0x1(r9)
    stb      r6, 0x2(r9)
    addis    r9, r8, 0x4
    addi     r9, r9, 0x45dc
    li       r8, 0x6
    add      r9, r30, r9
    stb      r6, 0x0(r9)
    stb      r6, 0x1(r9)
    stb      r6, 0x2(r9)
    addis    r9, r8, 0x4
    addi     r9, r9, 0x45dc
    add      r9, r30, r9
    stb      r6, 0x0(r9)
    stb      r6, 0x1(r9)
    stb      r6, 0x2(r9)
    stw      r6, 0x45f0(r7)
    stw      r6, 0x45ec(r7)
    stw      r6, 0x45e8(r7)
    stw      r6, 0x45f8(r7)
    stw      r0, 0x45fc(r7)
    stb      r4, 0x0(r30)
    mr       r4, r10
    lwz      r23, 0x4(r31)
    bl       CMsgData_Format
    addi     r3, r30, 0x4
    bl       CMsgData_Measure
    stw      r23, 0x4(r31)
    addi     r0, r30, 0x4
    stw      r0, 0x804(r30)
    lwz      r0, 0x898(r30)
    cmpwi    r0, 0x0
    bne      .L_800820D8
    lis      r3, lbl_8025C400@ha
    li       r5, 0x0
    addi     r3, r3, lbl_8025C400@l
    lwz      r4, 0x3d4c(r3)
    addi     r3, r3, 0x3d6c
    lwz      r4, 0x8(r4)
    addi     r4, r4, 0x3b8
    bl       Init__7CTexObjFP13TPLDescriptorUl
    lwz      r8, 0x0(r3)
    lis      r4, __vt__7CTexObj@ha
    lwz      r7, 0x4(r3)
    addi     r6, r4, __vt__7CTexObj@l
    li       r0, 0x18
    addi     r5, r1, 0x4c
    stw      r8, 0x14(r1)
    addi     r4, r3, 0x38
    stw      r7, 0x18(r1)
    lwz      r8, 0x8(r3)
    lwz      r7, 0xc(r3)
    stw      r8, 0x1c(r1)
    stw      r7, 0x20(r1)
    lwz      r8, 0x10(r3)
    lwz      r7, 0x14(r3)
    stw      r8, 0x24(r1)
    stw      r7, 0x28(r1)
    lwz      r8, 0x18(r3)
    lwz      r7, 0x1c(r3)
    stw      r8, 0x2c(r1)
    stw      r7, 0x30(r1)
    stw      r6, 0x110(r1)
    lwz      r6, 0x20(r3)
    stw      r6, 0x34(r1)
    lwz      r6, 0x24(r3)
    stw      r6, 0x38(r1)
    lwz      r7, 0x28(r3)
    lwz      r6, 0x2c(r3)
    stw      r7, 0x3c(r1)
    stw      r6, 0x40(r1)
    lwz      r6, 0x30(r3)
    stw      r6, 0x44(r1)
    lwz      r6, 0x34(r3)
    stw      r6, 0x48(r1)
    lwz      r3, 0x38(r3)
    stw      r3, 0x4c(r1)
    mtctr    r0
.L_80082090:
    lwz      r3, 0x4(r4)
    lwzu     r0, 0x8(r4)
    stw      r3, 0x4(r5)
    stwu     r0, 0x8(r5)
    bdnz     .L_80082090
    addi     r3, r1, 0x14
    bl       GXGetTexObjWidth
    lis      r5, 0x2aab
    lis      r4, __vt__7CTexObj@ha
    clrlwi   r3, r3, 16
    subi     r5, r5, 0x5555
    addi     r0, r4, __vt__7CTexObj@l
    mulhw    r3, r5, r3
    srawi    r3, r3, 2
    srwi     r4, r3, 31
    add      r3, r3, r4
    stw      r3, 0x898(r30)
    stw      r0, 0x110(r1)
.L_800820D8:
    addis    r5, r30, 0x4
    lwz      r23, 0x804(r30)
    lfs      f1, 0x4600(r5)
    addi     r3, r30, 0x8a0
    lfs      f2, 0x4604(r5)
    addi     r4, r30, 0x4
    lwz      r5, 0x4608(r5)
    lwz      r6, 0x894(r30)
    lwz      r7, 0x890(r30)
    bl       CMsgFont_Init
    addis    r7, r30, 0x4
    lis      r0, 0x4330
    lfs      f4, 0x4614(r7)
    li       r4, 0x2
    lfs      f3, 0x4610(r7)
    mr       r3, r23
    lfs      f0, 0x460c(r7)
    lfs      f2, lbl_8065D058@sda21(r0)
    stfs     f0, 0x4524(r7)
    lfd      f1, lbl_8065D050@sda21(r0)
    stfs     f3, 0x4528(r7)
    stfs     f4, 0x452c(r7)
    stfs     f2, 0x45c4(r7)
    lbz      r6, 0x0(r30)
    stw      r0, 0x118(r1)
    subi     r5, r6, 0x23
    subfic   r0, r6, 0x23
    nor      r0, r5, r0
    srawi    r0, r0, 31
    and      r0, r4, r0
    xoris    r0, r0, 0x8000
    stw      r0, 0x11c(r1)
    lfd      f0, 0x118(r1)
    fsubs    f0, f0, f1
    stfs     f0, 0x45c8(r7)
    stfs     f2, 0x45cc(r7)
    lmw      r23, 0x12c(r1)
    lwz      r0, 0x154(r1)
    mtlr     r0
    addi     r1, r1, 0x150
    blr
*/

/**
 * @note Address: 0x8008217C
 * @note Size: 0x60
 */
// CMsg_ResetPos
extern "C" void CMsg_ResetPos(CMsg* msg)
{
	msg->_44600   = 320.0f;
	msg->_44604   = 344.0f;
	msg->_44608   = 0;
	Vec zero      = { 0.0f, 0.0f, 0.0f };
	msg->_4460C.x = zero.x;
	msg->_4460C.y = zero.y;
	msg->_4460C.z = zero.z;
}

/**
 * @note Address: 0x800821DC
 * @note Size: 0x32C
 */
extern "C" void fn_800821DC(CMsg* msg)
{
	msg->_804 = NULL;
#if !defined(VERSION_JP)
	msg->_898 = 0;
#endif
	msg->_8A0._04_80 = 0;
	msg->_8A0._04_40 = 0;
	msg->_8A0._04_20 = 0;
	msg->_8A0._04_10 = 0;
	msg->_8A0._04_08 = 1;
	msg->_8A0._04_04 = 1;
	msg->_44518      = 0;
	msg->_4451C      = 0;
	msg->_44520      = 0;
	Vec zero         = { 0.0f, 0.0f, 0.0f };
	msg->_44524      = zero;
	msg->_44570      = 22.0f;
#if defined(VERSION_JP)
	msg->_44574 = 22.0f;
#else
	msg->_44574 = NAN;
#endif
	msg->_44578 = msgColorTable[0];
	msg->_4457C = 0.0f;
	msg->_44584 = 1.0f;
	msg->_44580 = 1.0f;
	msg->_44588 = 0;
	msg->_4458C = -1;
	msg->_445AC = -1;
	msg->_445B0 = 0.0f;
	msg->_445B4 = 1.0f;
	msg->_445B8 = 0.0f;
	msg->_4450C = 0;
	msg->_44510 = 0.0f;
	msg->_44514 = 0.0f;
	for (int i = 0; i < 400; i++) {
		msg->_1348[i]._2A6 = msg->_1348[i]._2A8 = (i * 75) % 360;
	}
	msg->_445C0 = 1;
	msg->_445C8 = 0.0f;
	msg->_445C4 = 0.0f;
	msg->_445D4 = -1;
	msg->_445D0 = -1;
	msg->_445D8 = 0;
	for (int i = 0; i < 9; i += 3) {
		for (int j = 0; j < 3; j++) {
			msg->_445DC[i + j] = 0;
		}
	}
	msg->_445F0 = 0;
	msg->_445EC = 0;
	msg->_445E8 = 0;
	msg->_445F8 = 0;
	msg->_445FC = -1;
}

/*
    stwu     r1, -0x30(r1)
    lis      r6, __float_nan@ha
    addi     r11, r6, __float_nan@l
    lis      r5, msgColorTable@ha
    stmw     r26, 0x18(r1)
    li       r4, 0x0
    lis      r7, lbl_80208ED0@ha
    addi     r10, r5, msgColorTable@l
    addi     r26, r7, lbl_80208ED0@l
    lis      r8, 0xb60b
    li       r12, 0x1
    addis    r27, r3, 0x4
    lfs      f2, lbl_8065D06C@sda21(r0)
    li       r9, -0x1
    lfs      f1, lbl_8065D058@sda21(r0)
    li       r7, 0x32
    lfs      f0, lbl_8065D05C@sda21(r0)
    mr       r5, r4
    mr       r6, r3
    stw      r4, 0x804(r3)
    stw      r4, 0x898(r3)
    lbz      r0, 0x8a4(r3)
    rlwimi   r0, r4, 7, 24, 24
    stb      r0, 0x8a4(r3)
    lbz      r0, 0x8a4(r3)
    rlwimi   r0, r4, 6, 25, 25
    stb      r0, 0x8a4(r3)
    addi     r0, r8, 0x60b7
    lbz      r28, 0x8a4(r3)
    rlwimi   r28, r4, 5, 26, 26
    stb      r28, 0x8a4(r3)
    lbz      r8, 0x8a4(r3)
    rlwimi   r8, r4, 4, 27, 27
    stb      r8, 0x8a4(r3)
    lbz      r8, 0x8a4(r3)
    rlwimi   r8, r12, 3, 28, 28
    stb      r8, 0x8a4(r3)
    lbz      r8, 0x8a4(r3)
    rlwimi   r8, r12, 2, 29, 29
    stb      r8, 0x8a4(r3)
    stw      r4, 0x4518(r27)
    stw      r4, 0x451c(r27)
    stw      r4, 0x4520(r27)
    lwz      r8, 0x0(r26)
    lwz      r12, 0x4(r26)
    stw      r8, 0x8(r1)
    lwz      r8, 0x8(r26)
    stw      r12, 0xc(r1)
    lfs      f3, 0x8(r1)
    stw      r8, 0x10(r1)
    lfs      f4, 0xc(r1)
    stfs     f3, 0x4524(r27)
    lfs      f3, 0x10(r1)
    stfs     f4, 0x4528(r27)
    stfs     f3, 0x452c(r27)
    stfs     f2, 0x4570(r27)
    lfs      f2, 0x0(r11)
    stfs     f2, 0x4574(r27)
    lwz      r8, 0x0(r10)
    stw      r8, 0x4578(r27)
    stfs     f1, 0x457c(r27)
    stfs     f0, 0x4584(r27)
    stfs     f0, 0x4580(r27)
    stb      r4, 0x4588(r27)
    stw      r9, 0x458c(r27)
    stw      r9, 0x45ac(r27)
    stfs     f1, 0x45b0(r27)
    stfs     f0, 0x45b4(r27)
    stfs     f1, 0x45b8(r27)
    stw      r4, 0x450c(r27)
    stfs     f1, 0x4510(r27)
    stfs     f1, 0x4514(r27)
    mtctr    r7
.L_80082300:
    addi     r7, r4, 0x1
    addi     r12, r4, 0x2
    mulli    r26, r7, 0x4b
    addi     r11, r4, 0x3
    addi     r10, r4, 0x4
    addi     r9, r4, 0x5
    mulhw    r8, r0, r5
    add      r7, r8, r5
    srawi    r28, r7, 8
    addi     r8, r4, 0x6
    srwi     r29, r28, 31
    addi     r7, r4, 0x7
    mulli    r31, r12, 0x4b
    addi     r4, r4, 0x8
    add      r29, r28, r29
    mulhw    r27, r0, r26
    add      r28, r27, r26
    srawi    r28, r28, 8
    srwi     r27, r28, 31
    add      r27, r28, r27
    mulli    r28, r29, 0x168
    subf     r30, r28, r5
    addi     r5, r5, 0x258
    mulhw    r12, r0, r31
    sth      r30, 0x15f0(r6)
    sth      r30, 0x15ee(r6)
    mulli    r28, r11, 0x4b
    add      r12, r12, r31
    mulhw    r11, r0, r28
    srawi    r12, r12, 8
    srwi     r29, r12, 31
    add      r30, r12, r29
    mulli    r12, r27, 0x168
    add      r11, r11, r28
    mulli    r29, r10, 0x4b
    srawi    r11, r11, 8
    subf     r27, r12, r26
    srwi     r12, r11, 31
    sth      r27, 0x189c(r6)
    mulhw    r10, r0, r29
    add      r12, r11, r12
    sth      r27, 0x189a(r6)
    mulli    r11, r30, 0x168
    add      r10, r10, r29
    subf     r30, r11, r31
    srawi    r10, r10, 8
    mulli    r31, r9, 0x4b
    sth      r30, 0x1b48(r6)
    srwi     r11, r10, 31
    sth      r30, 0x1b46(r6)
    add      r30, r10, r11
    mulhw    r9, r0, r31
    mulli    r10, r12, 0x168
    add      r9, r9, r31
    mulli    r11, r8, 0x4b
    srawi    r9, r9, 8
    subf     r12, r10, r28
    sth      r12, 0x1df4(r6)
    srwi     r10, r9, 31
    mulhw    r8, r0, r11
    sth      r12, 0x1df2(r6)
    add      r12, r9, r10
    mulli    r9, r30, 0x168
    add      r8, r8, r11
    subf     r30, r9, r29
    srawi    r8, r8, 8
    mulli    r9, r7, 0x4b
    sth      r30, 0x20a0(r6)
    srwi     r10, r8, 31
    sth      r30, 0x209e(r6)
    mulhw    r7, r0, r9
    add      r8, r8, r10
    mulli    r10, r12, 0x168
    add      r7, r7, r9
    subf     r12, r10, r31
    srawi    r7, r7, 8
    mulli    r10, r8, 0x168
    sth      r12, 0x234c(r6)
    srwi     r8, r7, 31
    sth      r12, 0x234a(r6)
    subf     r10, r10, r11
    add      r7, r7, r8
    sth      r10, 0x25f8(r6)
    mulli    r7, r7, 0x168
    sth      r10, 0x25f6(r6)
    subf     r7, r7, r9
    sth      r7, 0x28a4(r6)
    sth      r7, 0x28a2(r6)
    addi     r6, r6, 0x1560
    bdnz     .L_80082300
    addis    r5, r3, 0x4
    li       r0, 0x1
    stw      r0, 0x45c0(r5)
    li       r7, 0x0
    lfs      f0, lbl_8065D058@sda21(r0)
    addis    r6, r7, 0x4
    addi     r6, r6, 0x45dc
    li       r4, -0x1
    stfs     f0, 0x45c8(r5)
    li       r0, 0x0
    add      r6, r3, r6
    li       r7, 0x3
    stfs     f0, 0x45c4(r5)
    stw      r4, 0x45d4(r5)
    stw      r4, 0x45d0(r5)
    stw      r0, 0x45d8(r5)
    stb      r0, 0x0(r6)
    stb      r0, 0x1(r6)
    stb      r0, 0x2(r6)
    addis    r6, r7, 0x4
    addi     r6, r6, 0x45dc
    li       r7, 0x6
    add      r6, r3, r6
    stb      r0, 0x0(r6)
    stb      r0, 0x1(r6)
    stb      r0, 0x2(r6)
    addis    r6, r7, 0x4
    addi     r6, r6, 0x45dc
    add      r6, r3, r6
    stb      r0, 0x0(r6)
    stb      r0, 0x1(r6)
    stb      r0, 0x2(r6)
    stw      r0, 0x45f0(r5)
    stw      r0, 0x45ec(r5)
    stw      r0, 0x45e8(r5)
    stw      r0, 0x45f8(r5)
    stw      r4, 0x45fc(r5)
    lmw      r26, 0x18(r1)
    addi     r1, r1, 0x30
    blr
*/

/**
 * @note Address: 0x80082508
 * @note Size: 0xA0
 */
extern "C" BOOL fn_80082508(CMsg* msg)
{
	void* spawn         = GetSpawnData(13);
	UnkXObjReadObj* obj = &msg->_9C0;
	if (obj->vf9C(CoordInfo(spawn))) {
		obj->SetScale(3.0f, 3.0f, 3.0f, 0);
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x800825A8
 * @note Size: 0x3C8
 */
// fn_800825A8
/*
    stwu     r1, -0x40(r1)
    mflr     r0
    mr       r6, r3
    lis      r5, __float_nan@ha
    stw      r0, 0x44(r1)
    addi     r11, r5, __float_nan@l
    lis      r4, msgColorTable@ha
    li       r3, 0x0
    stmw     r26, 0x28(r1)
    lis      r7, lbl_80208ED0@ha
    addi     r26, r7, lbl_80208ED0@l
    addi     r10, r4, msgColorTable@l
    lis      r8, 0xb60b
    li       r12, 0x1
    addis    r27, r6, 0x4
    lfs      f2, lbl_8065D06C@sda21(r0)
    lfs      f1, lbl_8065D058@sda21(r0)
    li       r9, -0x1
    lfs      f0, lbl_8065D05C@sda21(r0)
    li       r7, 0x32
    mr       r4, r3
    mr       r5, r6
    stw      r3, 0x804(r6)
    stw      r3, 0x898(r6)
    lbz      r0, 0x8a4(r6)
    rlwimi   r0, r3, 7, 24, 24
    stb      r0, 0x8a4(r6)
    lbz      r0, 0x8a4(r6)
    rlwimi   r0, r3, 6, 25, 25
    stb      r0, 0x8a4(r6)
    addi     r0, r8, 0x60b7
    lbz      r28, 0x8a4(r6)
    rlwimi   r28, r3, 5, 26, 26
    stb      r28, 0x8a4(r6)
    lbz      r8, 0x8a4(r6)
    rlwimi   r8, r3, 4, 27, 27
    stb      r8, 0x8a4(r6)
    lbz      r8, 0x8a4(r6)
    rlwimi   r8, r12, 3, 28, 28
    stb      r8, 0x8a4(r6)
    lbz      r8, 0x8a4(r6)
    rlwimi   r8, r12, 2, 29, 29
    stb      r8, 0x8a4(r6)
    stw      r3, 0x4518(r27)
    stw      r3, 0x451c(r27)
    stw      r3, 0x4520(r27)
    lwz      r8, 0x0(r26)
    lwz      r12, 0x4(r26)
    stw      r8, 0xc(r1)
    lwz      r8, 0x8(r26)
    stw      r12, 0x10(r1)
    lfs      f3, 0xc(r1)
    stw      r8, 0x14(r1)
    lfs      f4, 0x10(r1)
    stfs     f3, 0x4524(r27)
    lfs      f3, 0x14(r1)
    stfs     f4, 0x4528(r27)
    stfs     f3, 0x452c(r27)
    stfs     f2, 0x4570(r27)
    lfs      f2, 0x0(r11)
    stfs     f2, 0x4574(r27)
    lwz      r8, 0x0(r10)
    stw      r8, 0x4578(r27)
    stfs     f1, 0x457c(r27)
    stfs     f0, 0x4584(r27)
    stfs     f0, 0x4580(r27)
    stb      r3, 0x4588(r27)
    stw      r9, 0x458c(r27)
    stw      r9, 0x45ac(r27)
    stfs     f1, 0x45b0(r27)
    stfs     f0, 0x45b4(r27)
    stfs     f1, 0x45b8(r27)
    stw      r3, 0x450c(r27)
    stfs     f1, 0x4510(r27)
    stfs     f1, 0x4514(r27)
    mtctr    r7
.L_800826D8:
    addi     r7, r3, 0x1
    addi     r12, r3, 0x2
    mulli    r26, r7, 0x4b
    addi     r11, r3, 0x3
    addi     r10, r3, 0x4
    addi     r9, r3, 0x5
    mulhw    r8, r0, r4
    add      r7, r8, r4
    srawi    r28, r7, 8
    addi     r8, r3, 0x6
    srwi     r29, r28, 31
    addi     r7, r3, 0x7
    mulli    r31, r12, 0x4b
    addi     r3, r3, 0x8
    add      r29, r28, r29
    mulhw    r27, r0, r26
    add      r28, r27, r26
    srawi    r28, r28, 8
    srwi     r27, r28, 31
    add      r27, r28, r27
    mulli    r28, r29, 0x168
    subf     r30, r28, r4
    addi     r4, r4, 0x258
    mulhw    r12, r0, r31
    sth      r30, 0x15f0(r5)
    sth      r30, 0x15ee(r5)
    mulli    r28, r11, 0x4b
    add      r12, r12, r31
    mulhw    r11, r0, r28
    srawi    r12, r12, 8
    srwi     r29, r12, 31
    add      r30, r12, r29
    mulli    r12, r27, 0x168
    add      r11, r11, r28
    mulli    r29, r10, 0x4b
    srawi    r11, r11, 8
    subf     r27, r12, r26
    srwi     r12, r11, 31
    sth      r27, 0x189c(r5)
    mulhw    r10, r0, r29
    add      r12, r11, r12
    sth      r27, 0x189a(r5)
    mulli    r11, r30, 0x168
    add      r10, r10, r29
    subf     r30, r11, r31
    srawi    r10, r10, 8
    mulli    r31, r9, 0x4b
    sth      r30, 0x1b48(r5)
    srwi     r11, r10, 31
    sth      r30, 0x1b46(r5)
    add      r30, r10, r11
    mulhw    r9, r0, r31
    mulli    r10, r12, 0x168
    add      r9, r9, r31
    mulli    r11, r8, 0x4b
    srawi    r9, r9, 8
    subf     r12, r10, r28
    sth      r12, 0x1df4(r5)
    srwi     r10, r9, 31
    mulhw    r8, r0, r11
    sth      r12, 0x1df2(r5)
    add      r12, r9, r10
    mulli    r9, r30, 0x168
    add      r8, r8, r11
    subf     r30, r9, r29
    srawi    r8, r8, 8
    mulli    r9, r7, 0x4b
    sth      r30, 0x20a0(r5)
    srwi     r10, r8, 31
    sth      r30, 0x209e(r5)
    mulhw    r7, r0, r9
    add      r8, r8, r10
    mulli    r10, r12, 0x168
    add      r7, r7, r9
    subf     r12, r10, r31
    srawi    r7, r7, 8
    mulli    r10, r8, 0x168
    sth      r12, 0x234c(r5)
    srwi     r8, r7, 31
    sth      r12, 0x234a(r5)
    subf     r10, r10, r11
    add      r7, r7, r8
    sth      r10, 0x25f8(r5)
    mulli    r7, r7, 0x168
    sth      r10, 0x25f6(r5)
    subf     r7, r7, r9
    sth      r7, 0x28a4(r5)
    sth      r7, 0x28a2(r5)
    addi     r5, r5, 0x1560
    bdnz     .L_800826D8
    addis    r8, r6, 0x4
    li       r0, 0x1
    stw      r0, 0x45c0(r8)
    li       r5, 0x0
    lfs      f0, lbl_8065D058@sda21(r0)
    addis    r10, r5, 0x4
    addi     r10, r10, 0x45dc
    lis      r3, lbl_80209144@ha
    stfs     f0, 0x45c8(r8)
    addi     r7, r3, lbl_80209144@l
    li       r0, -0x1
    add      r10, r6, r10
    stfs     f0, 0x45c4(r8)
    li       r9, 0x3
    lfs      f1, lbl_8065D070@sda21(r0)
    addi     r3, r6, 0x9c0
    stw      r0, 0x45d4(r8)
    li       r4, 0x0
    lfs      f0, lbl_8065D074@sda21(r0)
    stw      r0, 0x45d0(r8)
    stw      r5, 0x45d8(r8)
    stb      r5, 0x0(r10)
    stb      r5, 0x1(r10)
    stb      r5, 0x2(r10)
    addis    r10, r9, 0x4
    addi     r10, r10, 0x45dc
    li       r9, 0x6
    add      r10, r6, r10
    stb      r5, 0x0(r10)
    stb      r5, 0x1(r10)
    stb      r5, 0x2(r10)
    addis    r10, r9, 0x4
    addi     r10, r10, 0x45dc
    add      r10, r6, r10
    stb      r5, 0x0(r10)
    stb      r5, 0x1(r10)
    stb      r5, 0x2(r10)
    stw      r5, 0x45f0(r8)
    stw      r5, 0x45ec(r8)
    stw      r5, 0x45e8(r8)
    stw      r5, 0x45f8(r8)
    stw      r0, 0x45fc(r8)
    stfs     f1, 0x4600(r8)
    stfs     f0, 0x4604(r8)
    stw      r5, 0x4608(r8)
    lwz      r0, 0x0(r7)
    lwz      r5, 0x4(r7)
    stw      r0, 0x18(r1)
    lwz      r0, 0x8(r7)
    stw      r5, 0x1c(r1)
    lfs      f0, 0x18(r1)
    stw      r0, 0x20(r1)
    lfs      f1, 0x1c(r1)
    stfs     f0, 0x460c(r8)
    lfs      f0, 0x20(r1)
    stfs     f1, 0x4610(r8)
    stfs     f0, 0x4614(r8)
    lwz      r0, lbl_8065A428@sda21(r0)
    stw      r0, 0x8(r1)
    lbz      r5, 0x8(r1)
    lbz      r0, 0x9(r1)
    stb      r5, 0x9b8(r6)
    lbz      r5, 0xa(r1)
    stb      r0, 0x9b9(r6)
    lbz      r0, 0xb(r1)
    stb      r5, 0x9ba(r6)
    stb      r0, 0x9bb(r6)
    lwz      r12, 0x9f4(r6)
    lwz      r12, 0x90(r12)
    mtctr    r12
    bctrl
    lmw      r26, 0x28(r1)
    lwz      r0, 0x44(r1)
    mtlr     r0
    addi     r1, r1, 0x40
    blr
*/

/**
 * @note Address: 0x80082970
 * @note Size: 0x2308
 */
// CMsgFont_Draw
/*
    stwu     r1, -0x660(r1)
    mflr     r0
    stw      r0, 0x664(r1)
    stfd     f31, 0x650(r1)
    psq_st   f31, 0x658(r1), 0, qr0
    stfd     f30, 0x640(r1)
    psq_st   f30, 0x648(r1), 0, qr0
    stfd     f29, 0x630(r1)
    psq_st   f29, 0x638(r1), 0, qr0
    stfd     f28, 0x620(r1)
    psq_st   f28, 0x628(r1), 0, qr0
    stfd     f27, 0x610(r1)
    psq_st   f27, 0x618(r1), 0, qr0
    stfd     f26, 0x600(r1)
    psq_st   f26, 0x608(r1), 0, qr0
    stfd     f25, 0x5f0(r1)
    psq_st   f25, 0x5f8(r1), 0, qr0
    stfd     f24, 0x5e0(r1)
    psq_st   f24, 0x5e8(r1), 0, qr0
    stfd     f23, 0x5d0(r1)
    psq_st   f23, 0x5d8(r1), 0, qr0
    stfd     f22, 0x5c0(r1)
    psq_st   f22, 0x5c8(r1), 0, qr0
    stfd     f21, 0x5b0(r1)
    psq_st   f21, 0x5b8(r1), 0, qr0
    stfd     f20, 0x5a0(r1)
    psq_st   f20, 0x5a8(r1), 0, qr0
    stfd     f19, 0x590(r1)
    psq_st   f19, 0x598(r1), 0, qr0
    stfd     f18, 0x580(r1)
    psq_st   f18, 0x588(r1), 0, qr0
    stfd     f17, 0x570(r1)
    psq_st   f17, 0x578(r1), 0, qr0
    stfd     f16, 0x560(r1)
    psq_st   f16, 0x568(r1), 0, qr0
    stfd     f15, 0x550(r1)
    psq_st   f15, 0x558(r1), 0, qr0
    stmw     r21, 0x524(r1)
    mr       r30, r3
    lis      r5, lbl_80208E88@ha
    addis    r4, r30, 0x4
    fmr      f31, f1
    lwz      r3, 0x3c80(r4)
    addi     r31, r5, lbl_80208E88@l
    cmplwi   r3, 0x0
    beq      .L_80082AA8
    lwz      r5, 0x228(r3)
    li       r7, 0x0
    lwz      r8, 0x22c(r3)
    li       r0, 0x1
    and      r6, r5, r7
    and      r0, r8, r0
    xor      r5, r0, r7
    xor      r0, r6, r7
    or.      r0, r5, r0
    beq      .L_80082AA8
    lis      r5, 0x1
    xor      r0, r6, r7
    and      r5, r8, r5
    xor      r5, r5, r7
    or.      r0, r5, r0
    beq      .L_80082AA8
    lwz      r12, 0x34(r3)
    mr       r5, r4
    mr       r6, r4
    li       r7, 0x0
    lwz      r12, 0x10c(r12)
    li       r8, 0x0
    mtctr    r12
    addi     r4, r4, 0x3c84
    addi     r5, r5, 0x3c94
    addi     r6, r6, 0x3ca0
    bctrl
    addis    r3, r30, 0x4
    lfs      f1, 0x3ca0(r3)
    lfs      f0, 0x3ca4(r3)
    fsubs    f0, f1, f0
    stfs     f0, 0x3ca0(r3)
.L_80082AA8:
    addis    r29, r30, 0x4
    lfs      f0, lbl_8065D068@sda21(r0)
    lfs      f1, 0x3c98(r29)
    lfs      f2, 0x3c94(r29)
    fmuls    f3, f1, f0
    lfs      f0, lbl_8065D094@sda21(r0)
    fsubs    f1, f2, f3
    fcmpo    cr0, f1, f0
    bge      .L_80082AD4
    fadds    f0, f0, f3
    stfs     f0, 0x3c94(r29)
.L_80082AD4:
    lfs      f1, 0x3c98(r29)
    lfs      f0, lbl_8065D068@sda21(r0)
    lfs      f2, 0x3c94(r29)
    fmuls    f3, f1, f0
    lfs      f0, lbl_8065D098@sda21(r0)
    fadds    f1, f2, f3
    fcmpo    cr0, f1, f0
    ble      .L_80082AFC
    fsubs    f0, f0, f3
    stfs     f0, 0x3c94(r29)
.L_80082AFC:
    lfs      f1, 0x3ca0(r29)
    lfs      f0, lbl_8065D09C@sda21(r0)
    fcmpo    cr0, f1, f0
    bge      .L_80082B10
    stfs     f0, 0x3ca0(r29)
.L_80082B10:
    lfs      f1, 0x3ca0(r29)
    lfs      f2, 0x3ca4(r29)
    lfs      f0, lbl_8065D0A0@sda21(r0)
    fadds    f1, f1, f2
    fcmpo    cr0, f1, f0
    ble      .L_80082B30
    fsubs    f0, f0, f2
    stfs     f0, 0x3ca0(r29)
.L_80082B30:
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    lwz      r0, 0x4168(r3)
    cmpwi    r0, 0x0
    beq      .L_80082B4C
    lfs      f26, lbl_8065D0A4@sda21(r0)
    b        .L_80082B58
.L_80082B4C:
    lfs      f1, 0x3ca4(r29)
    lfs      f0, lbl_8065D0A8@sda21(r0)
    fdivs    f26, f1, f0
.L_80082B58:
    lfs      f0, lbl_8065D0AC@sda21(r0)
    fcmpo    cr0, f26, f0
    ble      .L_80082B68
    fmr      f26, f0
.L_80082B68:
    lfs      f0, lbl_8065D068@sda21(r0)
    fcmpo    cr0, f26, f0
    bge      .L_80082B78
    fmr      f26, f0
.L_80082B78:
    lwz      r3, 0xaa8(r30)
    li       r0, 0x0
    cmpwi    r3, 0x0
    blt      .L_80082B94
    cmpwi    r3, 0x1
    bge      .L_80082B94
    li       r0, 0x1
.L_80082B94:
    clrlwi.  r0, r0, 24
    beq      .L_80082BA8
    lfs      f0, lbl_8065D0B0@sda21(r0)
    fmuls    f19, f0, f26
    b        .L_80082BAC
.L_80082BA8:
    lfs      f19, lbl_8065D058@sda21(r0)
.L_80082BAC:
    addis    r3, r30, 0x4
    lfs      f1, lbl_8065D05C@sda21(r0)
    lfs      f0, 0x3d10(r3)
    fmr      f25, f19
    lfs      f2, lbl_8065D0B4@sda21(r0)
    fsubs    f0, f1, f0
    fmuls    f1, f2, f0
    bl       sin
    addis    r3, r30, 0x4
    lfs      f2, lbl_8065D05C@sda21(r0)
    lfs      f0, 0x3d10(r3)
    frsp     f4, f1
    lfs      f3, lbl_8065D0B8@sda21(r0)
    fsubs    f0, f2, f0
    lfs      f1, lbl_8065D0B4@sda21(r0)
    fmuls    f24, f3, f4
    fmuls    f1, f1, f0
    bl       sin
    addis    r3, r30, 0x4
    frsp     f3, f1
    lfs      f2, lbl_8065D0B8@sda21(r0)
    lfs      f1, 0x3d10(r3)
    lfs      f0, lbl_8065D058@sda21(r0)
    fmuls    f23, f2, f3
    fcmpo    cr0, f1, f0
    ble      .L_80084378
    lis      r3, lbl_8025C400@ha
    li       r4, 0xc
    addi     r28, r3, lbl_8025C400@l
    li       r5, 0x5
    lwz      r3, 0x290(r28)
    bl       fn_8019B34C
    lfs      f0, lbl_8065D068@sda21(r0)
    lfs      f1, 0x3c98(r29)
    lbz      r0, 0x11b(r30)
    fmuls    f29, f19, f0
    fmuls    f0, f1, f0
    lfs      f1, 0x3c94(r29)
    lfs      f4, lbl_8065D0B0@sda21(r0)
    cmplwi   r0, 0x0
    lfs      f5, 0x3ca0(r29)
    fsubs    f3, f1, f0
    fadds    f1, f1, f0
    lfs      f0, 0x3ca4(r29)
    lfs      f2, lbl_8065D0BC@sda21(r0)
    fsubs    f3, f3, f4
    fadds    f1, f4, f1
    fadds    f0, f5, f0
    fsubs    f4, f3, f24
    fadds    f3, f24, f1
    fsubs    f1, f5, f2
    fadds    f0, f2, f0
    fsubs    f22, f4, f29
    fadds    f21, f29, f3
    fsubs    f20, f1, f23
    fadds    f19, f23, f0
    beq      .L_80082C9C
    addis    r3, r30, 0x4
    lfs      f30, 0x3d10(r3)
    b        .L_80082CA0
.L_80082C9C:
    lfs      f30, lbl_8065D058@sda21(r0)
.L_80082CA0:
    lwz      r3, 0x290(r28)
    li       r4, 0x1
    li       r5, 0x7
    li       r6, 0x1
    bl       fn_8019B464
    lfs      f4, lbl_8065D0BC@sda21(r0)
    lis      r3, lbl_8025C400@ha
    lfs      f3, lbl_8065D0C4@sda21(r0)
    addi     r3, r3, lbl_8025C400@l
    fadds    f2, f4, f19
    lfs      f5, lbl_8065D0C0@sda21(r0)
    fadds    f0, f4, f21
    stfs     f3, 0x8(r1)
    fsubs    f1, f22, f4
    lis      r4, 0xff00
    fadds    f28, f5, f2
    lis      r5, 0xff00
    fadds    f27, f5, f0
    stfs     f1, 0xc(r1)
    fsubs    f2, f20, f4
    lis      r6, 0xff00
    stfs     f28, 0x10(r1)
    fmr      f4, f27
    fmr      f5, f2
    lis      r7, 0xff00
    fmr      f6, f3
    stfs     f3, 0x14(r1)
    fmr      f7, f27
    fmr      f8, f28
    bl       fn_80198614
    li       r0, 0xf
    addi     r5, r1, 0x288
    addi     r4, r31, 0xc8
    mtctr    r0
.L_80082D28:
    lwz      r3, 0x4(r4)
    lwzu     r0, 0x8(r4)
    stw      r3, 0x4(r5)
    stwu     r0, 0x8(r5)
    bdnz     .L_80082D28
    lfs      f0, lbl_8065D05C@sda21(r0)
    lwz      r21, 0x54(r31)
    fadds    f4, f0, f22
    lfs      f2, lbl_8065D0BC@sda21(r0)
    fadds    f5, f0, f20
    lwz      r12, 0x58(r31)
    fsubs    f1, f21, f0
    lwz      r0, 0x60(r31)
    fsubs    f0, f19, f0
    lwz      r11, 0x64(r31)
    lwz      r10, 0x6c(r31)
    fadds    f18, f2, f5
    lwz      r9, 0x70(r31)
    fadds    f16, f2, f4
    lwz      r8, 0x78(r31)
    fsubs    f17, f1, f2
    lwz      r5, 0x7c(r31)
    fsubs    f15, f0, f2
    lwz      r7, 0x84(r31)
    lwz      r6, 0x88(r31)
    lwz      r4, 0x90(r31)
    lwz      r3, 0x94(r31)
    stw      r0, 0xbc(r1)
    lwz      r0, 0x5c(r31)
    stw      r11, 0xc0(r1)
    lwz      r26, 0x68(r31)
    stw      r10, 0xb0(r1)
    lwz      r23, 0x74(r31)
    stw      r9, 0xb4(r1)
    lwz      r11, 0x80(r31)
    stw      r8, 0xa4(r1)
    lwz      r8, 0x8c(r31)
    stw      r5, 0xa8(r1)
    lwz      r5, 0x98(r31)
    stw      r7, 0x98(r1)
    lfs      f3, 0x3c94(r29)
    lfs      f2, 0x3ca0(r29)
    stw      r21, 0xc8(r1)
    stw      r12, 0xcc(r1)
    stw      r6, 0x9c(r1)
    stw      r4, 0x8c(r1)
    stw      r3, 0x90(r1)
    stfs     f3, 0xc8(r1)
    stfs     f2, 0xcc(r1)
    lwz      r21, 0xc8(r1)
    stfs     f4, 0xbc(r1)
    lwz      r27, 0xcc(r1)
    stfs     f18, 0xc0(r1)
    lwz      r25, 0xbc(r1)
    stfs     f16, 0xb0(r1)
    lwz      r24, 0xc0(r1)
    stfs     f5, 0xb4(r1)
    lwz      r22, 0xb0(r1)
    stfs     f17, 0xa4(r1)
    lwz      r12, 0xb4(r1)
    stfs     f5, 0xa8(r1)
    lwz      r10, 0xa4(r1)
    stfs     f1, 0x98(r1)
    lwz      r9, 0xa8(r1)
    stfs     f18, 0x9c(r1)
    lwz      r7, 0x98(r1)
    stfs     f1, 0x8c(r1)
    lwz      r6, 0x9c(r1)
    stfs     f15, 0x90(r1)
    lwz      r4, 0x8c(r1)
    lwz      r3, 0x90(r1)
    stw      r0, 0xd0(r1)
    stw      r21, 0x28c(r1)
    stw      r27, 0x290(r1)
    stw      r0, 0x294(r1)
    stw      r26, 0xc4(r1)
    stw      r25, 0x298(r1)
    stw      r24, 0x29c(r1)
    stw      r26, 0x2a0(r1)
    stw      r23, 0xb8(r1)
    stw      r22, 0x2a4(r1)
    stw      r12, 0x2a8(r1)
    stw      r23, 0x2ac(r1)
    stw      r11, 0xac(r1)
    stw      r10, 0x2b0(r1)
    stw      r9, 0x2b4(r1)
    stw      r11, 0x2b8(r1)
    stw      r8, 0xa0(r1)
    stw      r7, 0x2bc(r1)
    stw      r6, 0x2c0(r1)
    stw      r8, 0x2c4(r1)
    stw      r5, 0x94(r1)
    stw      r4, 0x2c8(r1)
    stw      r3, 0x2cc(r1)
    stw      r5, 0x2d0(r1)
    lwz      r23, 0x9c(r31)
    lis      r0, 0x4330
    lwz      r22, 0xa0(r31)
    addis    r3, r30, 0x4
    lwz      r11, 0xa8(r31)
    lwz      r10, 0xac(r31)
    lwz      r8, 0xb4(r31)
    lwz      r7, 0xb8(r31)
    lwz      r24, 0xa4(r31)
    lwz      r12, 0xb0(r31)
    lwz      r9, 0xbc(r31)
    lwz      r6, 0xc8(r31)
    lwz      r5, 0xc0(r31)
    lwz      r4, 0xc4(r31)
    stw      r23, 0x80(r1)
    lfd      f2, lbl_8065D048@sda21(r0)
    stw      r22, 0x84(r1)
    stw      r11, 0x74(r1)
    stw      r10, 0x78(r1)
    stw      r8, 0x68(r1)
    stw      r7, 0x6c(r1)
    stw      r5, 0x5c(r1)
    stw      r4, 0x60(r1)
    stfs     f17, 0x80(r1)
    stfs     f0, 0x84(r1)
    lwz      r23, 0x80(r1)
    stfs     f16, 0x74(r1)
    lwz      r22, 0x84(r1)
    stfs     f0, 0x78(r1)
    lwz      r11, 0x74(r1)
    stfs     f4, 0x68(r1)
    lwz      r10, 0x78(r1)
    stfs     f15, 0x6c(r1)
    lwz      r8, 0x68(r1)
    stfs     f4, 0x5c(r1)
    lwz      r7, 0x6c(r1)
    stfs     f18, 0x60(r1)
    lwz      r5, 0x5c(r1)
    lwz      r4, 0x60(r1)
    stw      r23, 0x2d4(r1)
    stw      r22, 0x2d8(r1)
    stw      r24, 0x2dc(r1)
    stw      r11, 0x2e0(r1)
    stw      r10, 0x2e4(r1)
    stw      r12, 0x2e8(r1)
    stw      r8, 0x2ec(r1)
    stw      r7, 0x2f0(r1)
    stw      r9, 0x2f4(r1)
    stw      r5, 0x2f8(r1)
    stw      r4, 0x2fc(r1)
    stw      r6, 0x300(r1)
    lbz      r4, 0x11b(r30)
    stw      r0, 0x508(r1)
    lfs      f0, 0x3d10(r3)
    stw      r4, 0x50c(r1)
    lfd      f1, 0x508(r1)
    stw      r24, 0x88(r1)
    fsubs    f1, f1, f2
    stw      r12, 0x7c(r1)
    fmuls    f1, f1, f0
    stw      r9, 0x70(r1)
    stw      r6, 0x64(r1)
    bl       __cvt_fp2unsigned
    lbz      r0, 0x119(r30)
    li       r4, 0x1
    lbz      r6, 0x118(r30)
    li       r5, 0x7
    slwi     r0, r0, 16
    lbz      r7, 0x11a(r30)
    rlwimi   r0, r6, 24, 0, 7
    li       r6, 0x1
    rlwimi   r0, r7, 8, 16, 23
    or       r21, r0, r3
    lwz      r3, 0x290(r28)
    bl       fn_8019B464
    lis      r3, lbl_8025C400@ha
    mr       r7, r21
    addi     r22, r3, lbl_8025C400@l
    addi     r6, r1, 0x28c
    lwz      r5, 0x44(r22)
    mr       r3, r22
    li       r4, 0xa0
    li       r8, 0xa
    addi     r5, r5, 0xe8
    bl       fn_80198D08
    lwz      r3, 0x290(r28)
    li       r4, 0x1
    li       r5, 0x1
    li       r6, 0x1
    bl       fn_8019B464
    lfs      f3, lbl_8065D058@sda21(r0)
    lis      r3, lbl_8025C400@ha
    lfs      f4, lbl_8065D094@sda21(r0)
    fmr      f1, f16
    fmr      f2, f18
    addi     r3, r3, lbl_8025C400@l
    fmr      f5, f4
    mr       r5, r21
    fmr      f6, f3
    fmr      f7, f3
    fmr      f8, f3
    mr       r6, r21
    li       r4, 0x10
    bl       fn_80198258
    lfs      f3, lbl_8065D058@sda21(r0)
    lis      r3, lbl_8025C400@ha
    lfs      f4, lbl_8065D094@sda21(r0)
    fmr      f1, f17
    fmr      f2, f18
    addi     r3, r3, lbl_8025C400@l
    fmr      f5, f4
    mr       r5, r21
    fmr      f6, f3
    fmr      f7, f3
    fmr      f8, f3
    mr       r6, r21
    li       r4, 0x10
    bl       fn_80198258
    lfs      f3, lbl_8065D058@sda21(r0)
    lis      r3, lbl_8025C400@ha
    lfs      f4, lbl_8065D094@sda21(r0)
    fmr      f1, f16
    fmr      f2, f15
    addi     r3, r3, lbl_8025C400@l
    fmr      f5, f4
    mr       r5, r21
    fmr      f6, f3
    fmr      f7, f3
    fmr      f8, f3
    mr       r6, r21
    li       r4, 0x10
    bl       fn_80198258
    lfs      f3, lbl_8065D058@sda21(r0)
    lis      r3, lbl_8025C400@ha
    lfs      f4, lbl_8065D094@sda21(r0)
    fmr      f1, f17
    fmr      f2, f15
    addi     r3, r3, lbl_8025C400@l
    fmr      f5, f4
    mr       r5, r21
    fmr      f6, f3
    fmr      f7, f3
    fmr      f8, f3
    mr       r6, r21
    li       r4, 0x10
    bl       fn_80198258
    lfs      f0, lbl_8065D0C8@sda21(r0)
    fmuls    f1, f0, f30
    bl       __cvt_fp2unsigned
    lfs      f1, lbl_8065D0C0@sda21(r0)
    mr       r21, r3
    lfs      f5, 0x28c(r1)
    lis      r5, lbl_8025C400@ha
    lfs      f3, 0x290(r1)
    addi     r3, r5, lbl_8025C400@l
    fadds    f0, f5, f1
    lfs      f5, 0x298(r1)
    fadds    f12, f3, f1
    lfs      f3, 0x29c(r1)
    fadds    f11, f5, f1
    lfs      f5, 0x2a4(r1)
    fadds    f10, f3, f1
    lfs      f3, 0x2a8(r1)
    fadds    f9, f5, f1
    lfs      f5, 0x2b0(r1)
    fadds    f8, f3, f1
    lfs      f3, 0x2b4(r1)
    fadds    f7, f5, f1
    lfs      f5, 0x2bc(r1)
    fadds    f6, f3, f1
    lfs      f3, 0x2c0(r1)
    fadds    f4, f5, f1
    lfs      f5, 0x2c8(r1)
    fadds    f2, f3, f1
    stfs     f0, 0x28c(r1)
    fadds    f0, f5, f1
    lfs      f3, 0x2cc(r1)
    stfs     f12, 0x290(r1)
    mr       r7, r21
    fadds    f12, f3, f1
    lfs      f5, 0x2d4(r1)
    stfs     f11, 0x298(r1)
    addi     r6, r1, 0x28c
    fadds    f11, f5, f1
    lfs      f3, 0x2d8(r1)
    stfs     f10, 0x29c(r1)
    fadds    f10, f3, f1
    lwz      r4, 0x44(r22)
    li       r8, 0xa
    lfs      f5, 0x2e0(r1)
    stfs     f9, 0x2a4(r1)
    addi     r5, r4, 0xe8
    fadds    f9, f5, f1
    lfs      f3, 0x2e4(r1)
    stfs     f8, 0x2a8(r1)
    li       r4, 0xa0
    fadds    f8, f3, f1
    lfs      f5, 0x2ec(r1)
    stfs     f7, 0x2b0(r1)
    fadds    f7, f5, f1
    lfs      f3, 0x2f0(r1)
    stfs     f6, 0x2b4(r1)
    fadds    f6, f3, f1
    lfs      f5, 0x2f8(r1)
    stfs     f4, 0x2bc(r1)
    fadds    f4, f5, f1
    lfs      f3, 0x2fc(r1)
    stfs     f2, 0x2c0(r1)
    fadds    f2, f3, f1
    stfs     f0, 0x2c8(r1)
    stfs     f12, 0x2cc(r1)
    stfs     f11, 0x2d4(r1)
    stfs     f10, 0x2d8(r1)
    stfs     f9, 0x2e0(r1)
    stfs     f8, 0x2e4(r1)
    stfs     f7, 0x2ec(r1)
    stfs     f6, 0x2f0(r1)
    stfs     f4, 0x2f8(r1)
    stfs     f2, 0x2fc(r1)
    bl       fn_80198D08
    lfs      f0, lbl_8065D0C0@sda21(r0)
    lis      r3, lbl_8025C400@ha
    lfs      f3, lbl_8065D058@sda21(r0)
    addi     r3, r3, lbl_8025C400@l
    fadds    f1, f0, f22
    lfs      f2, lbl_8065D05C@sda21(r0)
    fadds    f0, f0, f20
    lfs      f4, lbl_8065D094@sda21(r0)
    lfs      f7, lbl_8065D0BC@sda21(r0)
    fmr      f6, f3
    fadds    f1, f2, f1
    mr       r5, r21
    fadds    f0, f2, f0
    mr       r6, r21
    fmr      f5, f4
    li       r4, 0x10
    fadds    f15, f7, f1
    fadds    f16, f7, f0
    fmr      f7, f3
    fmr      f1, f15
    fmr      f2, f16
    fmr      f8, f3
    bl       fn_80198258
    lfs      f0, lbl_8065D0C0@sda21(r0)
    lis      r3, lbl_8025C400@ha
    lfs      f3, lbl_8065D058@sda21(r0)
    fmr      f2, f16
    fadds    f5, f0, f21
    lfs      f1, lbl_8065D05C@sda21(r0)
    lfs      f0, lbl_8065D0BC@sda21(r0)
    fmr      f6, f3
    lfs      f4, lbl_8065D094@sda21(r0)
    fsubs    f1, f5, f1
    fmr      f5, f4
    addi     r3, r3, lbl_8025C400@l
    fmr      f7, f3
    mr       r5, r21
    fsubs    f16, f1, f0
    fmr      f8, f3
    mr       r6, r21
    li       r4, 0x10
    fmr      f1, f16
    bl       fn_80198258
    lfs      f0, lbl_8065D0C0@sda21(r0)
    lis      r3, lbl_8025C400@ha
    lfs      f3, lbl_8065D058@sda21(r0)
    fmr      f1, f15
    fadds    f5, f0, f19
    lfs      f2, lbl_8065D05C@sda21(r0)
    lfs      f0, lbl_8065D0BC@sda21(r0)
    fmr      f6, f3
    lfs      f4, lbl_8065D094@sda21(r0)
    fsubs    f2, f5, f2
    fmr      f5, f4
    addi     r3, r3, lbl_8025C400@l
    fmr      f7, f3
    mr       r5, r21
    fsubs    f15, f2, f0
    fmr      f8, f3
    mr       r6, r21
    li       r4, 0x10
    fmr      f2, f15
    bl       fn_80198258
    lfs      f3, lbl_8065D058@sda21(r0)
    lis      r3, lbl_8025C400@ha
    lfs      f4, lbl_8065D094@sda21(r0)
    fmr      f1, f16
    fmr      f2, f15
    addi     r3, r3, lbl_8025C400@l
    fmr      f5, f4
    mr       r5, r21
    fmr      f6, f3
    fmr      f7, f3
    fmr      f8, f3
    mr       r6, r21
    li       r4, 0x10
    bl       fn_80198258
    lis      r3, lbl_8025C400@ha
    li       r5, 0x0
    addi     r3, r3, lbl_8025C400@l
    lwz      r4, 0x3d4c(r3)
    addi     r3, r3, 0x3d6c
    lwz      r4, 0x8(r4)
    addi     r4, r4, 0x390
    bl       Init__7CTexObjFP13TPLDescriptorUl
    lwz      r5, 0x0(r3)
    lis      r4, __vt__7CTexObj@ha
    lwz      r7, 0x4(r3)
    addi     r25, r1, 0x440
    addi     r6, r4, __vt__7CTexObj@l
    li       r0, 0x18
    stw      r5, 0x404(r1)
    subi     r5, r25, 0x4
    addi     r4, r3, 0x38
    stw      r7, 0x408(r1)
    lwz      r8, 0x8(r3)
    lwz      r7, 0xc(r3)
    stw      r8, 0x40c(r1)
    stw      r7, 0x410(r1)
    lwz      r8, 0x10(r3)
    lwz      r7, 0x14(r3)
    stw      r8, 0x414(r1)
    stw      r7, 0x418(r1)
    lwz      r8, 0x18(r3)
    lwz      r7, 0x1c(r3)
    stw      r8, 0x41c(r1)
    stw      r7, 0x420(r1)
    stw      r6, 0x500(r1)
    lwz      r6, 0x20(r3)
    stw      r6, 0x424(r1)
    lwz      r6, 0x24(r3)
    stw      r6, 0x428(r1)
    lwz      r7, 0x28(r3)
    lwz      r6, 0x2c(r3)
    stw      r7, 0x42c(r1)
    stw      r6, 0x430(r1)
    lwz      r6, 0x30(r3)
    stw      r6, 0x434(r1)
    lwz      r6, 0x34(r3)
    stw      r6, 0x438(r1)
    lwz      r3, 0x38(r3)
    stw      r3, 0x43c(r1)
    mtctr    r0
.L_80083400:
    lwz      r3, 0x4(r4)
    lwzu     r0, 0x8(r4)
    stw      r3, 0x4(r5)
    stwu     r0, 0x8(r5)
    bdnz     .L_80083400
    addi     r3, r1, 0x404
    bl       GXGetTexObjWidth
    clrlwi   r3, r3, 16
    lis      r0, 0x4330
    stw      r3, 0x50c(r1)
    addi     r3, r1, 0x404
    lfd      f1, lbl_8065D048@sda21(r0)
    stw      r0, 0x508(r1)
    lfd      f0, 0x508(r1)
    fsubs    f15, f0, f1
    bl       GXGetTexObjHeight
    fsubs    f1, f21, f22
    clrlwi   r3, r3, 16
    lfs      f0, lbl_8065D0CC@sda21(r0)
    lis      r0, 0x4330
    stw      r3, 0x514(r1)
    fsubs    f9, f1, f0
    stw      r0, 0x510(r1)
    lfs      f0, lbl_8065D058@sda21(r0)
    lfd      f2, lbl_8065D048@sda21(r0)
    lfd      f1, 0x510(r1)
    fcmpo    cr0, f9, f0
    fsubs    f5, f1, f2
    bge      .L_80083478
    fmr      f9, f0
.L_80083478:
    fsubs    f2, f19, f20
    lfs      f1, lbl_8065D0C8@sda21(r0)
    lfs      f0, lbl_8065D058@sda21(r0)
    fsubs    f10, f2, f1
    fcmpo    cr0, f10, f0
    bge      .L_80083494
    fmr      f10, f0
.L_80083494:
    li       r0, 0x20
    addi     r5, r1, 0x300
    addi     r4, r31, 0x140
    mtctr    r0
.L_800834A4:
    lwz      r3, 0x4(r4)
    lwzu     r0, 0x8(r4)
    stw      r3, 0x4(r5)
    stwu     r0, 0x8(r5)
    bdnz     .L_800834A4
    lfs      f7, lbl_8065D05C@sda21(r0)
    lfs      f2, lbl_8065D094@sda21(r0)
    fdivs    f8, f7, f15
    lfs      f13, lbl_8065D0D0@sda21(r0)
    lfs      f16, lbl_8065D0D4@sda21(r0)
    lfs      f12, lbl_8065D0D8@sda21(r0)
    lfs      f11, lbl_8065D0DC@sda21(r0)
    lfs      f1, lbl_8065D060@sda21(r0)
    fadds    f0, f2, f20
    lfs      f4, lbl_8065D078@sda21(r0)
    fadds    f6, f13, f22
    lfs      f3, lbl_8065D0BC@sda21(r0)
    fsubs    f15, f21, f13
    stfs     f8, 0x32c(r1)
    fsubs    f17, f0, f1
    lfs      f13, lbl_8065D068@sda21(r0)
    fsubs    f6, f6, f4
    lfs      f0, lbl_8065D0B8@sda21(r0)
    fdivs    f7, f7, f5
    stfs     f6, 0x314(r1)
    fadds    f5, f3, f17
    fadds    f12, f12, f10
    fsubs    f10, f19, f2
    stfs     f5, 0x318(r1)
    fadds    f2, f16, f9
    fadds    f15, f4, f15
    lfs      f9, 0x3c94(r29)
    fsubs    f12, f12, f3
    fadds    f10, f1, f10
    stfs     f9, 0x334(r1)
    fmuls    f1, f11, f30
    stfs     f5, 0x338(r1)
    stfs     f2, 0x33c(r1)
    stfs     f15, 0x354(r1)
    stfs     f5, 0x358(r1)
    stfs     f7, 0x370(r1)
    stfs     f6, 0x374(r1)
    lfs      f4, 0x3ca4(r29)
    lfs      f3, 0x3ca0(r29)
    fmadds   f3, f4, f13, f3
    stfs     f12, 0x380(r1)
    stfs     f7, 0x390(r1)
    fadds    f0, f0, f3
    stfs     f15, 0x394(r1)
    stfs     f0, 0x378(r1)
    stfs     f0, 0x398(r1)
    stfs     f12, 0x3a0(r1)
    stfs     f6, 0x3b4(r1)
    stfs     f10, 0x3b8(r1)
    stfs     f8, 0x3cc(r1)
    stfs     f9, 0x3d4(r1)
    stfs     f10, 0x3d8(r1)
    stfs     f2, 0x3dc(r1)
    stfs     f15, 0x3f4(r1)
    stfs     f10, 0x3f8(r1)
    bl       __cvt_fp2unsigned
    lis      r4, lbl_8025C400@ha
    li       r0, -0x100
    addi     r27, r4, lbl_8025C400@l
    li       r4, 0x0
    or       r24, r3, r0
    lwz      r3, 0x294(r27)
    bl       fn_8019AFA0
    lwz      r3, 0x290(r28)
    li       r4, 0x1
    li       r5, 0x1
    li       r6, 0x1
    bl       fn_8019B464
    lis      r3, lbl_8025C400@ha
    addi     r21, r1, 0x304
    addi     r23, r1, 0x470
    li       r26, 0x0
    addi     r22, r3, lbl_8025C400@l
.L_800835DC:
    lfs      f1, 0x0(r21)
    mr       r3, r25
    lfs      f2, 0x4(r21)
    lfs      f3, lbl_8065D058@sda21(r0)
    bl       PSMTXTrans
    li       r0, 0x1
    lfs      f1, 0x8(r21)
    stw      r0, 0x43c(r1)
    mr       r3, r23
    lfs      f2, 0xc(r21)
    lfs      f3, lbl_8065D05C@sda21(r0)
    bl       PSMTXScale
    lfs      f6, lbl_8065D058@sda21(r0)
    li       r0, 0x1
    stw      r0, 0x43c(r1)
    mr       r3, r22
    fmr      f7, f6
    lfs      f1, 0x10(r21)
    fmr      f8, f6
    lfs      f2, 0x14(r21)
    lfs      f3, lbl_8065D05C@sda21(r0)
    mr       r5, r24
    lfs      f4, 0x18(r21)
    mr       r6, r24
    lfs      f5, 0x1c(r21)
    mr       r7, r24
    mr       r8, r24
    addi     r4, r1, 0x404
    bl       fn_80197800
    addi     r26, r26, 0x1
    addi     r21, r21, 0x20
    cmpwi    r26, 0x8
    blt      .L_800835DC
    lwz      r3, 0x294(r27)
    li       r4, 0x4
    bl       fn_8019AFA0
    lwz      r3, 0x290(r28)
    li       r4, 0x1
    li       r5, 0x7
    li       r6, 0x1
    bl       fn_8019B464
    lfs      f0, lbl_8065D0BC@sda21(r0)
    lis      r3, lbl_8025C400@ha
    lfs      f3, lbl_8065D0C4@sda21(r0)
    fmr      f4, f27
    fsubs    f1, f22, f0
    addi     r3, r3, lbl_8025C400@l
    stfs     f3, 0x8(r1)
    fsubs    f2, f20, f0
    fmr      f6, f3
    stfs     f1, 0xc(r1)
    fmr      f5, f2
    lis      r4, 0xff00
    stfs     f28, 0x10(r1)
    fmr      f7, f27
    fmr      f8, f28
    lis      r5, 0xff00
    stfs     f3, 0x14(r1)
    lis      r6, 0xff00
    lis      r7, 0xff00
    bl       fn_80198614
    lwz      r3, 0x290(r28)
    li       r4, 0x1
    li       r5, 0x3
    li       r6, 0x0
    bl       fn_8019B464
    lwz      r0, 0xaa8(r30)
    cmpwi    r0, 0x0
    blt      .L_80083D80
    cmpwi    r0, 0x1
    bge      .L_80083D80
    lbz      r0, 0x11b(r30)
    cmplwi   r0, 0x0
    beq      .L_800837A4
    lfs      f0, lbl_8065D0BC@sda21(r0)
    lfs      f1, lbl_8065D05C@sda21(r0)
    fadds    f2, f0, f23
    lfs      f3, lbl_8065D0E0@sda21(r0)
    lfs      f0, 0x3ca4(r29)
    lfs      f15, 0x3c98(r29)
    fsubs    f1, f2, f1
    fmadds   f1, f3, f1, f0
    bl       __cvt_fp2unsigned
    lfs      f1, lbl_8065D0B0@sda21(r0)
    mr       r24, r3
    lfs      f0, lbl_8065D05C@sda21(r0)
    fadds    f1, f1, f24
    lfs      f2, lbl_8065D0E0@sda21(r0)
    fsubs    f0, f1, f0
    fmadds   f1, f2, f0, f15
    bl       __cvt_fp2unsigned
    lfs      f1, 0x3ca0(r29)
    mr       r23, r3
    lfs      f0, lbl_8065D0BC@sda21(r0)
    lfs      f2, lbl_8065D05C@sda21(r0)
    fsubs    f0, f1, f0
    fsubs    f0, f0, f23
    fadds    f1, f2, f0
    bl       __cvt_fp2unsigned
    lfs      f2, lbl_8065D068@sda21(r0)
    mr       r22, r3
    lfs      f1, 0x3c94(r29)
    lfs      f0, lbl_8065D0B0@sda21(r0)
    fnmsubs  f1, f15, f2, f1
    lfs      f2, lbl_8065D05C@sda21(r0)
    fsubs    f0, f1, f0
    fsubs    f0, f0, f24
    fsubs    f0, f0, f29
    fadds    f1, f2, f0
    bl       __cvt_fp2unsigned
    mr       r4, r22
    mr       r5, r23
    mr       r6, r24
    bl       GXSetScissor
.L_800837A4:
    lfs      f1, 0x3c98(r29)
    lis      r3, lbl_8025C400@ha
    lfs      f4, lbl_8065D068@sda21(r0)
    addi     r3, r3, lbl_8025C400@l
    lfs      f0, 0x3c94(r29)
    addis    r3, r3, 0x1
    lfs      f2, lbl_8065D0B0@sda21(r0)
    li       r22, 0x0
    fnmsubs  f3, f1, f4, f0
    lfs      f1, 0x3ca4(r29)
    lfs      f0, 0x3ca0(r29)
    li       r7, 0x0
    lfs      f5, lbl_8065D0E4@sda21(r0)
    fsubs    f2, f3, f2
    fmadds   f0, f1, f4, f0
    lfs      f1, lbl_8065D0E8@sda21(r0)
    lwz      r0, -0x68a0(r3)
    fsubs    f2, f2, f29
    lwz      r9, -0x6894(r3)
    fmadds   f19, f1, f26, f0
    lwz      r8, -0x68a4(r3)
    fmadds   f18, f5, f26, f2
    mtctr    r0
    cmpwi    r0, 0x0
    ble      .L_80083868
.L_80083808:
    add      r5, r8, r7
    li       r0, 0x1
    lwz      r3, 0x22c(r5)
    li       r6, 0x0
    lwz      r4, 0x228(r5)
    and      r3, r3, r0
    and      r0, r4, r6
    xor      r3, r3, r6
    xor      r0, r0, r6
    or.      r0, r3, r0
    beq      .L_80083860
    lwz      r3, 0x240(r5)
    cmplwi   r3, 0x0
    beq      .L_8008384C
    beq      .L_80083850
    lha      r6, 0x16(r3)
    b        .L_80083850
.L_8008384C:
    li       r6, -0x1
.L_80083850:
    extsh    r0, r6
    cmpwi    r0, 0xa
    bne      .L_80083860
    mr       r22, r5
.L_80083860:
    add      r7, r7, r9
    bdnz     .L_80083808
.L_80083868:
    cmplwi   r22, 0x0
    beq      .L_80083C20
    lwz      r4, 0x0(r22)
    lwz      r0, 0x14(r4)
    rlwinm.  r0, r0, 0, 8, 8
    beq      .L_80083888
    lfs      f25, lbl_8065D058@sda21(r0)
    b        .L_80083C88
.L_80083888:
    lfs      f0, lbl_8065D0C0@sda21(r0)
    addis    r3, r30, 0x4
    lwz      r9, 0xc8(r22)
    fmuls    f1, f0, f26
    lfs      f0, 0x3d10(r3)
    lwz      r8, 0xcc(r22)
    addi     r3, r1, 0x254
    lwz      r7, 0xd0(r22)
    lwz      r6, 0x128(r22)
    lwz      r5, 0x12c(r22)
    fmuls    f20, f1, f0
    lwz      r0, 0x130(r22)
    stw      r9, 0xe0(r1)
    lfs      f15, 0xfc(r22)
    stw      r8, 0xe4(r1)
    stw      r7, 0xe8(r1)
    stw      r6, 0xd4(r1)
    stw      r5, 0xd8(r1)
    stw      r0, 0xdc(r1)
    bl       __ct__5CJObjFP9_HSD_JObj
    lfs      f0, lbl_8065D0EC@sda21(r0)
    fmr      f1, f18
    fmr      f2, f19
    addi     r3, r1, 0x254
    fmuls    f16, f0, f26
    li       r4, 0x1
    fmr      f3, f16
    bl       fn_80017BB0
    fneg     f17, f20
    addi     r3, r1, 0x254
    fmr      f1, f20
    li       r4, 0x1
    fmr      f3, f20
    fmr      f2, f17
    bl       fn_80017E68
    lfs      f1, lbl_8065D058@sda21(r0)
    addi     r3, r1, 0x254
    li       r4, 0x1
    bl       fn_80017878
    lwz      r21, 0x254(r1)
    cmplwi   r21, 0x0
    beq      .L_80083970
    bne      .L_80083944
    li       r3, lbl_8065D07C@sda21
    li       r4, 0x25d
    li       r5, lbl_8065D084@sda21
    bl       __assert
.L_80083944:
    lwz      r4, 0x14(r21)
    li       r3, 0x0
    rlwinm.  r0, r4, 0, 8, 8
    bne      .L_80083960
    rlwinm.  r0, r4, 0, 25, 25
    beq      .L_80083960
    li       r3, 0x1
.L_80083960:
    clrlwi.  r0, r3, 24
    beq      .L_80083970
    mr       r3, r21
    bl       HSD_JObjSetupMatrixSub
.L_80083970:
    lwz      r11, 0x244(r31)
    mr       r4, r22
    lwz      r10, 0x248(r31)
    addi     r3, r1, 0x4c
    lwz      r9, 0x24c(r31)
    addi     r6, r1, 0xec
    lwz      r8, 0x250(r31)
    li       r5, 0x6
    lwz      r7, 0x254(r31)
    lwz      r0, 0x258(r31)
    stw      r11, 0xec(r1)
    stw      r10, 0xf0(r1)
    stw      r9, 0xf4(r1)
    stw      r8, 0xf8(r1)
    stw      r7, 0xfc(r1)
    stw      r0, 0x100(r1)
    bl       fn_8007E7F4
    lfs      f1, 0x50(r1)
    lis      r3, __vt__4CVec@ha
    addi     r0, r3, __vt__4CVec@l
    lfs      f0, lbl_8065D0E8@sda21(r0)
    fsubs    f2, f19, f1
    stw      r0, 0x58(r1)
    fmr      f1, f18
    addi     r3, r1, 0x254
    fmr      f3, f16
    li       r4, 0x1
    fsubs    f0, f2, f0
    fadds    f2, f19, f0
    bl       fn_80017BB0
    fmr      f1, f20
    addi     r3, r1, 0x254
    fmr      f2, f17
    li       r4, 0x1
    fmr      f3, f20
    bl       fn_80017E68
    lfs      f1, lbl_8065D058@sda21(r0)
    addi     r3, r1, 0x254
    li       r4, 0x1
    bl       fn_80017878
    lwz      r3, 0x0(r22)
    li       r4, 0x8
    bl       HSD_JObjSetFlags
    lwz      r26, 0x168(r22)
    li       r4, -0x100
    lwz      r25, 0x164(r22)
    li       r0, 0x0
    mr       r3, r22
    stw      r4, 0x168(r22)
    stw      r0, 0x164(r22)
    lwz      r12, 0x34(r22)
    lwz      r12, 0xac(r12)
    mtctr    r12
    bctrl
    lwz      r21, 0x254(r1)
    addi     r24, r1, 0x258
    lfs      f2, 0xe0(r1)
    lfs      f1, 0xe4(r1)
    cmplwi   r21, 0x0
    lfs      f0, 0xe8(r1)
    stfs     f2, 0x258(r1)
    stfs     f1, 0x25c(r1)
    stfs     f0, 0x260(r1)
    bne      .L_80083A80
    li       r3, lbl_8065D07C@sda21
    li       r4, 0x3a9
    li       r5, lbl_8065D084@sda21
    bl       __assert
.L_80083A80:
    cmplwi   r24, 0x0
    bne      .L_80083A98
    addi     r5, r31, 0x2e8
    li       r3, lbl_8065D07C@sda21
    li       r4, 0x3aa
    bl       __assert
.L_80083A98:
    lfs      f0, 0x258(r1)
    stfs     f0, 0x38(r21)
    lfs      f0, 0x25c(r1)
    stfs     f0, 0x3c(r21)
    lfs      f0, 0x260(r1)
    stfs     f0, 0x40(r21)
    lwz      r0, 0x14(r21)
    rlwinm.  r0, r0, 0, 6, 6
    bne      .L_80083B04
    cmplwi   r21, 0x0
    beq      .L_80083B04
    bne      .L_80083AD8
    li       r3, lbl_8065D07C@sda21
    li       r4, 0x25d
    li       r5, lbl_8065D084@sda21
    bl       __assert
.L_80083AD8:
    lwz      r4, 0x14(r21)
    li       r3, 0x0
    rlwinm.  r0, r4, 0, 8, 8
    bne      .L_80083AF4
    rlwinm.  r0, r4, 0, 25, 25
    beq      .L_80083AF4
    li       r3, 0x1
.L_80083AF4:
    clrlwi.  r0, r3, 24
    bne      .L_80083B04
    mr       r3, r21
    bl       HSD_JObjSetMtxDirtySub
.L_80083B04:
    lwz      r21, 0x254(r1)
    addi     r23, r1, 0x278
    lfs      f2, 0xd4(r1)
    lfs      f1, 0xd8(r1)
    cmplwi   r21, 0x0
    lfs      f0, 0xdc(r1)
    stfs     f2, 0x278(r1)
    stfs     f1, 0x27c(r1)
    stfs     f0, 0x280(r1)
    bne      .L_80083B3C
    li       r3, lbl_8065D07C@sda21
    li       r4, 0x316
    li       r5, lbl_8065D084@sda21
    bl       __assert
.L_80083B3C:
    cmplwi   r23, 0x0
    bne      .L_80083B54
    li       r3, lbl_8065D07C@sda21
    li       r4, 0x317
    li       r5, lbl_8065D08C@sda21
    bl       __assert
.L_80083B54:
    lfs      f0, 0x278(r1)
    stfs     f0, 0x2c(r21)
    lfs      f0, 0x27c(r1)
    stfs     f0, 0x30(r21)
    lfs      f0, 0x280(r1)
    stfs     f0, 0x34(r21)
    lwz      r0, 0x14(r21)
    rlwinm.  r0, r0, 0, 6, 6
    bne      .L_80083BC0
    cmplwi   r21, 0x0
    beq      .L_80083BC0
    bne      .L_80083B94
    li       r3, lbl_8065D07C@sda21
    li       r4, 0x25d
    li       r5, lbl_8065D084@sda21
    bl       __assert
.L_80083B94:
    lwz      r4, 0x14(r21)
    li       r3, 0x0
    rlwinm.  r0, r4, 0, 8, 8
    bne      .L_80083BB0
    rlwinm.  r0, r4, 0, 25, 25
    beq      .L_80083BB0
    li       r3, 0x1
.L_80083BB0:
    clrlwi.  r0, r3, 24
    bne      .L_80083BC0
    mr       r3, r21
    bl       HSD_JObjSetMtxDirtySub
.L_80083BC0:
    fmr      f1, f15
    addi     r3, r1, 0x254
    li       r4, 0x1
    bl       fn_80017878
    lwz      r3, 0x0(r22)
    li       r4, 0x8
    bl       HSD_JObjClearFlags
    stw      r26, 0x168(r22)
    lis      r3, __vt__5CJObj@ha
    cmplwi   r23, 0x0
    stw      r25, 0x164(r22)
    addi     r0, r3, __vt__5CJObj@l
    stw      r0, 0x288(r1)
    beq      .L_80083C04
    lis      r3, __vt__4CVec@ha
    addi     r0, r3, __vt__4CVec@l
    stw      r0, 0x284(r1)
.L_80083C04:
    lis      r3, __vt__4CVec@ha
    cmplwi   r24, 0x0
    addi     r0, r3, __vt__4CVec@l
    stw      r0, 0x274(r1)
    beq      .L_80083C88
    stw      r0, 0x264(r1)
    b        .L_80083C88
.L_80083C20:
    addi     r3, r30, 0x120
    lfs      f0, lbl_8065D0EC@sda21(r0)
    lwz      r12, 0x154(r30)
    fmr      f1, f18
    fmr      f2, f19
    li       r4, 0x0
    lwz      r12, 0x68(r12)
    fmuls    f3, f0, f26
    mtctr    r12
    bctrl
    lfs      f0, lbl_8065D0C0@sda21(r0)
    addi     r3, r30, 0x120
    addis    r4, r30, 0x4
    lwz      r12, 0x154(r30)
    fmuls    f1, f0, f26
    lfs      f0, 0x3d10(r4)
    lwz      r12, 0x6c(r12)
    li       r4, 0x0
    fmuls    f1, f1, f0
    mtctr    r12
    bctrl
    addi     r3, r30, 0x120
    lwz      r12, 0x154(r30)
    lwz      r12, 0xac(r12)
    mtctr    r12
    bctrl
.L_80083C88:
    lis      r4, lbl_8025C400@ha
    li       r3, -0x1
    addi     r26, r4, lbl_8025C400@l
    bl       HSD_StateInvalidate
    lwz      r0, 0x290(r28)
    cmplwi   r0, 0x0
    beq      .L_80083CF8
    lwz      r3, 0x290(r26)
    lwz      r0, 0xc(r3)
    cmpwi    r0, 0x0
    bne      .L_80083CBC
    li       r0, 0x1
    stw      r0, 0xc(r3)
.L_80083CBC:
    lwz      r0, 0x10(r3)
    cmpwi    r0, 0x0
    bne      .L_80083CD0
    li       r0, 0x1
    stw      r0, 0x10(r3)
.L_80083CD0:
    lwz      r0, 0x14(r3)
    cmpwi    r0, 0x0
    bne      .L_80083CE4
    li       r0, 0x1
    stw      r0, 0x14(r3)
.L_80083CE4:
    lwz      r0, 0x18(r3)
    cmpwi    r0, 0x0
    bne      .L_80083CF8
    li       r0, 0x1
    stw      r0, 0x18(r3)
.L_80083CF8:
    lwz      r3, 0x294(r26)
    cmplwi   r3, 0x0
    beq      .L_80083D40
    lwz      r0, 0x74(r3)
    cmpwi    r0, 0x0
    bne      .L_80083D18
    li       r0, 0x1
    stw      r0, 0x74(r3)
.L_80083D18:
    lwz      r0, 0x78(r3)
    cmpwi    r0, 0x0
    bne      .L_80083D2C
    li       r0, 0x1
    stw      r0, 0x78(r3)
.L_80083D2C:
    lwz      r0, 0x7c(r3)
    cmpwi    r0, 0x0
    bne      .L_80083D40
    li       r0, 0x1
    stw      r0, 0x7c(r3)
.L_80083D40:
    lwz      r3, 0x298(r26)
    cmplwi   r3, 0x0
    beq      .L_80083D60
    lwz      r0, 0x30(r3)
    cmpwi    r0, 0x0
    bne      .L_80083D60
    li       r0, 0x1
    stw      r0, 0x30(r3)
.L_80083D60:
    lbz      r0, 0x11b(r30)
    cmplwi   r0, 0x0
    beq      .L_80083D80
    li       r3, 0x0
    li       r4, 0x0
    li       r5, 0x280
    li       r6, 0x1c0
    bl       GXSetScissor
.L_80083D80:
    addis    r4, r30, 0x4
    lwz      r0, 0x3cec(r4)
    cmpwi    r0, 0x0
    blt      .L_80084000
    lfs      f1, lbl_8065D0BC@sda21(r0)
    slwi     r0, r0, 2
    lfs      f0, 0x3ca0(r29)
    lwz      r3, 0x0(r30)
    fadds    f0, f1, f0
    lfs      f4, 0x3cf0(r4)
    lfs      f5, lbl_8065D068@sda21(r0)
    add      r3, r3, r0
    lfs      f1, 0x3c94(r29)
    lfs      f3, lbl_8065D0F0@sda21(r0)
    lfs      f2, 0x3cf4(r4)
    fmadds   f16, f25, f5, f1
    lfs      f1, lbl_8065D0F4@sda21(r0)
    fadds    f15, f4, f0
    lfs      f0, 0x3d18(r4)
    fadds    f17, f3, f2
    lfs      f19, 0x858(r3)
    fmuls    f1, f1, f0
    bl       __cvt_fp2unsigned
    oris     r21, r3, 0xc87f
    lwz      r3, 0x294(r27)
    ori      r21, r21, 0x1600
    li       r4, 0x0
    bl       fn_8019AFA0
    lfs      f2, lbl_8065D05C@sda21(r0)
    addis    r22, r30, 0x4
    addi     r3, r22, 0x3ad8
    lfs      f1, lbl_8065D0F8@sda21(r0)
    fmr      f3, f2
    addi     r22, r22, 0x3a6c
    bl       PSMTXScale
    lfs      f2, lbl_8065D058@sda21(r0)
    li       r0, 0x1
    stw      r0, 0x38(r22)
    addi     r3, r22, 0x3c
    fmr      f3, f2
    lfs      f1, lbl_8065D0F8@sda21(r0)
    bl       PSMTXTrans
    lfs      f0, lbl_8065D0E0@sda21(r0)
    li       r0, 0x1
    lfs      f3, lbl_8065D058@sda21(r0)
    lis      r3, lbl_8025C400@ha
    fadds    f15, f0, f15
    stw      r0, 0x38(r22)
    fmr      f1, f16
    addi     r3, r3, lbl_8025C400@l
    fmr      f4, f17
    mr       r4, r22
    fmr      f2, f15
    mr       r5, r21
    fmr      f5, f19
    mr       r6, r21
    fmr      f6, f3
    mr       r7, r21
    fmr      f7, f3
    mr       r8, r21
    fmr      f8, f3
    bl       fn_80197800
    lfs      f1, lbl_8065D058@sda21(r0)
    addi     r3, r22, 0x3c
    fmr      f2, f1
    fmr      f3, f1
    bl       PSMTXTrans
    lfs      f0, lbl_8065D068@sda21(r0)
    li       r0, 0x1
    lfs      f3, lbl_8065D058@sda21(r0)
    lis      r3, lbl_8025C400@ha
    fmuls    f18, f17, f0
    lfs      f0, lbl_8065D0B8@sda21(r0)
    fmr      f2, f15
    stw      r0, 0x38(r22)
    fmr      f5, f19
    lfs      f4, lbl_8065D0BC@sda21(r0)
    fsubs    f1, f16, f18
    addi     r3, r3, lbl_8025C400@l
    fmr      f6, f3
    mr       r4, r22
    fmr      f7, f3
    mr       r5, r21
    fmr      f8, f3
    mr       r6, r21
    fsubs    f1, f1, f0
    mr       r7, r21
    mr       r8, r21
    bl       fn_80197800
    lfs      f2, lbl_8065D058@sda21(r0)
    addi     r3, r22, 0x3c
    lfs      f1, lbl_8065D068@sda21(r0)
    fmr      f3, f2
    bl       PSMTXTrans
    fadds    f0, f16, f18
    lfs      f3, lbl_8065D058@sda21(r0)
    lfs      f1, lbl_8065D0B8@sda21(r0)
    li       r0, 0x1
    lis      r3, lbl_8025C400@ha
    fmr      f2, f15
    fmr      f5, f19
    stw      r0, 0x38(r22)
    fmr      f6, f3
    lfs      f4, lbl_8065D0BC@sda21(r0)
    fmr      f7, f3
    addi     r3, r3, lbl_8025C400@l
    fmr      f8, f3
    mr       r4, r22
    fadds    f1, f1, f0
    mr       r5, r21
    mr       r6, r21
    mr       r7, r21
    mr       r8, r21
    bl       fn_80197800
    addis    r21, r30, 0x4
    addi     r21, r21, 0x3b6c
    mr       r3, r21
    bl       GXGetTexObjWidth
    lis      r0, 0x4330
    clrlwi   r3, r3, 16
    stw      r3, 0x514(r1)
    addi     r3, r21, 0x6c
    lfd      f1, lbl_8065D048@sda21(r0)
    stw      r0, 0x510(r1)
    lfs      f2, lbl_8065D068@sda21(r0)
    lfd      f0, 0x510(r1)
    lfs      f3, lbl_8065D05C@sda21(r0)
    fsubs    f0, f0, f1
    fdivs    f0, f17, f0
    fmuls    f1, f2, f0
    bl       PSMTXScale
    li       r0, 0x1
    addis    r3, r30, 0x4
    stw      r0, 0x38(r21)
    lfs      f1, lbl_8065D0B0@sda21(r0)
    lfs      f0, 0x3d18(r3)
    fmuls    f1, f1, f0
    bl       __cvt_fp2unsigned
    lfs      f3, lbl_8065D058@sda21(r0)
    li       r0, -0x100
    lfs      f1, lbl_8065D0E0@sda21(r0)
    or       r5, r3, r0
    lfs      f0, lbl_8065D0B8@sda21(r0)
    fmr      f2, f15
    lis      r3, lbl_8025C400@ha
    fmr      f4, f17
    fmr      f6, f3
    addi     r3, r3, lbl_8025C400@l
    fmr      f7, f3
    fmr      f8, f3
    mr       r4, r21
    fadds    f1, f1, f16
    fsubs    f5, f19, f0
    mr       r6, r5
    mr       r7, r5
    mr       r8, r5
    bl       fn_80197800
    lwz      r3, 0x294(r27)
    li       r4, 0x4
    bl       fn_8019AFA0
.L_80084000:
    addis    r3, r30, 0x4
    lwz      r4, 0x3d0c(r3)
    cmpwi    r4, 0x0
    blt      .L_8008435C
    lwz      r0, 0x3d04(r3)
    lis      r8, 0x4330
    lwz      r5, 0x3c78(r3)
    addi     r4, r4, 0x1
    xoris    r0, r0, 0x8000
    lwz      r6, 0x25c(r31)
    mulli    r7, r5, 0x2ac
    stw      r0, 0x50c(r1)
    xoris    r9, r4, 0x8000
    lwz      r5, 0x260(r31)
    stw      r8, 0x508(r1)
    add      r4, r30, r7
    lwz      r0, 0xa8c(r4)
    lfd      f4, lbl_8065D050@sda21(r0)
    lfd      f0, 0x508(r1)
    slwi     r0, r0, 2
    add      r4, r3, r0
    lfs      f5, lbl_8065D068@sda21(r0)
    lfs      f1, 0x3c94(r29)
    fsubs    f0, f0, f4
    lfs      f6, 0x3cd0(r3)
    lwz      r7, 0x0(r30)
    fmadds   f16, f25, f5, f1
    fmuls    f2, f0, f6
    lfs      f1, lbl_8065D0BC@sda21(r0)
    lfs      f0, 0x3ca0(r29)
    add      r7, r7, r0
    stw      r9, 0x514(r1)
    fadds    f0, f1, f0
    stw      r8, 0x510(r1)
    fmadds   f2, f2, f5, f16
    lfs      f1, 0x3ca8(r4)
    lfd      f3, 0x510(r1)
    lwz      r4, 0x268(r31)
    fadds    f0, f1, f0
    fsubs    f3, f3, f4
    lfs      f1, 0x858(r7)
    lwz      r0, 0x26c(r31)
    lwz      r27, 0x28c(r31)
    fnmsubs  f0, f1, f5, f0
    fnmsubs  f4, f3, f6, f2
    lfs      f1, lbl_8065D078@sda21(r0)
    stw      r6, 0x40(r1)
    lwz      r26, 0x290(r31)
    fadds    f15, f1, f0
    stw      r5, 0x44(r1)
    fadds    f2, f4, f6
    lwz      r5, 0x294(r31)
    stw      r4, 0x34(r1)
    lwz      r25, 0x298(r31)
    stw      r0, 0x38(r1)
    lwz      r24, 0x29c(r31)
    stfs     f4, 0x40(r1)
    lwz      r0, 0x2a0(r31)
    stfs     f15, 0x44(r1)
    lwz      r23, 0x2a4(r31)
    stfs     f2, 0x34(r1)
    lwz      r22, 0x2a8(r31)
    stfs     f15, 0x38(r1)
    lwz      r12, 0x2ac(r31)
    lwz      r11, 0x2b0(r31)
    lwz      r10, 0x2b4(r31)
    lwz      r9, 0x2b8(r31)
    lwz      r8, 0x264(r31)
    stw      r5, 0x22c(r1)
    lwz      r5, 0x270(r31)
    stw      r0, 0x238(r1)
    lwz      r7, 0x40(r1)
    stw      r27, 0x224(r1)
    lwz      r6, 0x44(r1)
    stw      r26, 0x228(r1)
    lwz      r4, 0x34(r1)
    stw      r25, 0x230(r1)
    lwz      r0, 0x38(r1)
    stw      r24, 0x234(r1)
    stw      r23, 0x23c(r1)
    stw      r22, 0x240(r1)
    stw      r12, 0x244(r1)
    stw      r11, 0x248(r1)
    stw      r10, 0x24c(r1)
    stw      r9, 0x250(r1)
    stw      r8, 0x48(r1)
    stw      r7, 0x224(r1)
    stw      r6, 0x228(r1)
    stw      r8, 0x22c(r1)
    stw      r5, 0x3c(r1)
    stw      r4, 0x230(r1)
    stw      r0, 0x234(r1)
    stw      r5, 0x238(r1)
    lwz      r4, 0x274(r31)
    fadds    f3, f15, f6
    lwz      r6, 0x278(r31)
    lwz      r5, 0x280(r31)
    lwz      r0, 0x284(r31)
    stw      r4, 0x28(r1)
    lfs      f1, lbl_8065D0F4@sda21(r0)
    lwz      r7, 0x27c(r31)
    lwz      r4, 0x288(r31)
    stw      r6, 0x2c(r1)
    lfs      f0, 0x3d18(r3)
    stw      r5, 0x1c(r1)
    fmuls    f1, f1, f0
    stw      r0, 0x20(r1)
    stfs     f2, 0x28(r1)
    stfs     f3, 0x2c(r1)
    lwz      r6, 0x28(r1)
    stfs     f4, 0x1c(r1)
    lwz      r5, 0x2c(r1)
    stfs     f3, 0x20(r1)
    lwz      r3, 0x1c(r1)
    lwz      r0, 0x20(r1)
    stw      r7, 0x30(r1)
    stw      r6, 0x23c(r1)
    stw      r5, 0x240(r1)
    stw      r7, 0x244(r1)
    stw      r4, 0x24(r1)
    stw      r3, 0x248(r1)
    stw      r0, 0x24c(r1)
    stw      r4, 0x250(r1)
    bl       __cvt_fp2unsigned
    lfs      f1, 0x244(r1)
    ori      r4, r3, 0xff00
    lis      r3, lbl_8025C400@ha
    lfs      f0, 0x248(r1)
    stfs     f1, 0x8(r1)
    addi     r3, r3, lbl_8025C400@l
    lfs      f1, 0x24c(r1)
    mr       r5, r4
    stfs     f0, 0xc(r1)
    mr       r6, r4
    lfs      f0, 0x250(r1)
    mr       r7, r4
    stfs     f1, 0x10(r1)
    lfs      f1, 0x224(r1)
    stfs     f0, 0x14(r1)
    lfs      f2, 0x228(r1)
    lfs      f3, 0x22c(r1)
    lfs      f4, 0x230(r1)
    lfs      f5, 0x234(r1)
    lfs      f6, 0x238(r1)
    lfs      f7, 0x23c(r1)
    lfs      f8, 0x240(r1)
    bl       fn_80198614
    lis      r3, lbl_8025C400@ha
    lfd      f19, lbl_8065D050@sda21(r0)
    lfs      f17, lbl_8065D068@sda21(r0)
    addi     r22, r3, lbl_8025C400@l
    addis    r25, r30, 0x4
    li       r21, 0x0
    lis      r24, 0x4330
    b        .L_80084350
.L_8008426C:
    xoris    r0, r0, 0x8000
    addi     r3, r21, 0x1
    stw      r0, 0x50c(r1)
    xoris    r3, r3, 0x8000
    lwz      r0, 0x3d08(r25)
    stw      r24, 0x508(r1)
    lfs      f2, 0x3cd0(r25)
    cmpw     r21, r0
    lfd      f0, 0x508(r1)
    stw      r3, 0x514(r1)
    fsubs    f0, f0, f19
    stw      r24, 0x510(r1)
    fmuls    f0, f0, f2
    lfd      f1, 0x510(r1)
    fsubs    f1, f1, f19
    fmadds   f0, f0, f17, f16
    fnmsubs  f18, f1, f2, f0
    bge      .L_800842D0
    lfs      f1, lbl_8065D0DC@sda21(r0)
    lfs      f0, 0x3d14(r25)
    fmuls    f1, f1, f0
    bl       __cvt_fp2unsigned
    li       r0, -0x100
    or       r23, r3, r0
    b        .L_800842E8
.L_800842D0:
    lfs      f1, lbl_8065D0DC@sda21(r0)
    lfs      f0, 0x3d14(r25)
    fmuls    f1, f1, f0
    bl       __cvt_fp2unsigned
    oris     r23, r3, 0x64ff
    ori      r23, r23, 0x6400
.L_800842E8:
    addis    r5, r21, 0x4
    addi     r3, r1, 0x18
    addi     r5, r5, 0x3cf8
    li       r4, lbl_8065D0FC@sda21
    lbzx     r5, r30, r5
    extsb    r5, r5
    crclr    cr1eq
    bl       sprintf
    lfs      f1, 0x3cd0(r25)
    mr       r3, r22
    addi     r4, r1, 0x18
    li       r5, 0x1
    bl       fn_801968BC
    lfs      f4, 0x3cd0(r25)
    fmr      f2, f15
    lfs      f3, lbl_8065D058@sda21(r0)
    mr       r3, r22
    fsubs    f0, f4, f1
    mr       r4, r23
    fmr      f5, f4
    addi     r5, r1, 0x18
    fmadds   f18, f0, f17, f18
    fmr      f1, f18
    crset    cr1eq
    bl       fn_80196610
    addi     r21, r21, 0x1
.L_80084350:
    lwz      r0, 0x3d04(r25)
    cmpw     r21, r0
    blt      .L_8008426C
.L_8008435C:
    lwz      r3, 0x290(r28)
    li       r4, 0xc
    li       r5, 0x5
    bl       fn_8019B34C
    lis      r3, __vt__7CTexObj@ha
    addi     r0, r3, __vt__7CTexObj@l
    stw      r0, 0x500(r1)
.L_80084378:
    addis    r3, r30, 0x4
    lfs      f0, lbl_8065D058@sda21(r0)
    lfs      f1, 0x3d14(r3)
    fcmpo    cr0, f1, f0
    ble      .L_800848FC
    lis      r3, lbl_8025C400@ha
    li       r4, 0x0
    addi     r28, r3, lbl_8025C400@l
    li       r5, 0x0
    lwz      r3, 0x294(r28)
    li       r6, 0x4
    bl       fn_8019B040
    lwz      r3, 0x294(r28)
    li       r4, 0x0
    bl       fn_8019AFA0
    lwz      r7, 0x294(r28)
    li       r6, 0x1
    lis      r3, lbl_8025C400@ha
    li       r5, 0x0
    stb      r6, 0x80(r7)
    li       r4, 0x4
    li       r0, 0x1e
    addi     r22, r3, lbl_8025C400@l
    stw      r6, 0x78(r7)
    lwz      r3, 0x294(r28)
    stw      r5, 0x84(r3)
    stw      r6, 0x88(r3)
    stw      r4, 0x8c(r3)
    stw      r0, 0x90(r3)
    stw      r6, 0x7c(r3)
    lwz      r3, 0x290(r22)
    cmplwi   r3, 0x0
    beq      .L_80084404
    li       r4, 0x0
    bl       fn_8019B278
.L_80084404:
    lwz      r3, 0x294(r22)
    cmplwi   r3, 0x0
    beq      .L_80084418
    li       r4, 0x0
    bl       fn_8019AED0
.L_80084418:
    lwz      r3, 0x298(r22)
    cmplwi   r3, 0x0
    beq      .L_8008442C
    li       r4, 0x0
    bl       fn_8019AE94
.L_8008442C:
    lfs      f0, lbl_8065D068@sda21(r0)
    addis    r3, r30, 0x4
    lfs      f2, 0x3d14(r3)
    addi     r3, r1, 0x1f4
    fmuls    f17, f25, f0
    lfs      f0, 0x3c94(r29)
    lfs      f5, lbl_8065D0E0@sda21(r0)
    fmr      f15, f2
    lfs      f1, lbl_8065D05C@sda21(r0)
    fmuls    f16, f2, f31
    fadds    f4, f0, f17
    lfs      f0, 0x3ca0(r29)
    lfs      f3, lbl_8065D058@sda21(r0)
    fadds    f2, f1, f0
    fadds    f1, f5, f4
    bl       PSMTXTrans
    lis      r3, lbl_8025C400@ha
    addi     r4, r1, 0x1f4
    addi     r23, r3, lbl_8025C400@l
    addi     r5, r1, 0x1c4
    lwz      r3, 0x44(r23)
    addi     r3, r3, 0xe8
    bl       PSMTXConcat
    mr       r21, r30
    addis    r22, r30, 0x4
    li       r24, 0x0
    b        .L_8008450C
.L_80084498:
    lwz      r3, 0xd38(r21)
    lwz      r0, 0x3cec(r22)
    cmpw     r3, r0
    bne      .L_80084504
    subi     r0, r4, 0x1
    lfs      f1, lbl_8065D100@sda21(r0)
    cmpw     r24, r0
    bne      .L_800844C0
    fmr      f0, f16
    b        .L_800844C4
.L_800844C0:
    fmr      f0, f15
.L_800844C4:
    fmuls    f1, f1, f0
    bl       __cvt_fp2unsigned
    li       r0, 0x0
    mr       r8, r3
    stb      r0, 0xbb0(r21)
    li       r0, 0x1
    addi     r3, r21, 0xbac
    mr       r9, r8
    stw      r0, 0xbac(r21)
    mr       r10, r8
    addi     r4, r1, 0x1c4
    li       r5, 0x0
    stw      r8, 0x8(r1)
    li       r6, 0x1e
    li       r7, 0x1
    bl       Draw__7CSpriteFPA4_f11_GXTexMapIDUl13_GXTexMtxTypeUlUlUlUl
.L_80084504:
    addi     r21, r21, 0x2ac
    addi     r24, r24, 0x1
.L_8008450C:
    lwz      r4, 0x3c78(r22)
    cmpw     r24, r4
    blt      .L_80084498
    lfs      f0, 0x3c94(r29)
    addi     r3, r1, 0x1f4
    lfs      f2, 0x3ca0(r29)
    fadds    f1, f0, f17
    lfs      f3, lbl_8065D058@sda21(r0)
    bl       PSMTXTrans
    lwz      r3, 0x44(r23)
    addi     r4, r1, 0x1f4
    addi     r5, r1, 0x1c4
    addi     r3, r3, 0xe8
    bl       PSMTXConcat
    lfd      f19, lbl_8065D048@sda21(r0)
    mr       r25, r30
    addis    r24, r30, 0x4
    li       r22, 0x0
    lis      r27, 0x4330
    b        .L_800848BC
.L_8008455C:
    subi     r0, r3, 0x1
    cmpw     r22, r0
    bne      .L_80084570
    fmr      f1, f16
    b        .L_80084574
.L_80084570:
    fmr      f1, f15
.L_80084574:
    lwz      r5, 0xd34(r25)
    li       r3, 0x0
    stw      r27, 0x510(r1)
    li       r0, 0x1
    clrlwi   r4, r5, 24
    stw      r4, 0x514(r1)
    lfd      f0, 0x510(r1)
    stb      r3, 0xbb0(r25)
    fsubs    f0, f0, f19
    stw      r0, 0xbac(r25)
    fmuls    f0, f0, f1
    lbz      r3, 0xd4c(r25)
    rlwinm   r0, r3, 0, 24, 27
    fctiwz   f0, f0
    cmpwi    r0, 0x20
    stfd     f0, 0x508(r1)
    lwz      r23, 0x50c(r1)
    rlwimi   r23, r5, 0, 0, 23
    beq      .L_800845D0
    bge      .L_8008481C
    cmpwi    r0, 0x10
    beq      .L_800845D0
    b        .L_8008481C
.L_800845D0:
    clrlwi.  r0, r3, 31
    lfs      f17, 0xd3c(r25)
    beq      .L_80084630
    lha      r3, 0xd50(r25)
    lis      r0, 0x4330
    stw      r0, 0x510(r1)
    xoris    r0, r3, 0x8000
    lfd      f1, lbl_8065D050@sda21(r0)
    stw      r0, 0x514(r1)
    lfs      f2, lbl_8065D030@sda21(r0)
    lfd      f0, 0x510(r1)
    fsubs    f0, f0, f1
    fmuls    f1, f2, f0
    bl       sin
    lbz      r3, 0xd4d(r25)
    lis      r0, 0x4330
    stw      r0, 0x508(r1)
    frsp     f2, f1
    lfd      f1, lbl_8065D048@sda21(r0)
    stw      r3, 0x50c(r1)
    lfd      f0, 0x508(r1)
    fsubs    f0, f0, f1
    fmuls    f20, f0, f2
    b        .L_80084634
.L_80084630:
    lfs      f20, lbl_8065D058@sda21(r0)
.L_80084634:
    lbz      r0, 0xd4c(r25)
    rlwinm.  r0, r0, 0, 30, 30
    beq      .L_80084694
    lha      r3, 0xd4e(r25)
    lis      r0, 0x4330
    stw      r0, 0x510(r1)
    xoris    r0, r3, 0x8000
    lfd      f1, lbl_8065D050@sda21(r0)
    stw      r0, 0x514(r1)
    lfs      f2, lbl_8065D030@sda21(r0)
    lfd      f0, 0x510(r1)
    fsubs    f0, f0, f1
    fmuls    f1, f2, f0
    bl       sin
    lbz      r3, 0xd4d(r25)
    lis      r0, 0x4330
    stw      r0, 0x508(r1)
    frsp     f2, f1
    lfd      f1, lbl_8065D048@sda21(r0)
    stw      r3, 0x50c(r1)
    lfd      f0, 0x508(r1)
    fsubs    f0, f0, f1
    fmuls    f21, f0, f2
    b        .L_80084698
.L_80084694:
    lfs      f21, lbl_8065D058@sda21(r0)
.L_80084698:
    lfs      f0, lbl_8065D104@sda21(r0)
    fadds    f22, f0, f17
    fmr      f1, f22
    bl       sin
    frsp     f18, f1
    fmr      f1, f17
    bl       sin
    frsp     f2, f1
    fmuls    f0, f21, f18
    fmr      f1, f22
    fmadds   f18, f20, f2, f0
    bl       cos
    frsp     f22, f1
    fmr      f1, f17
    bl       cos
    fmuls    f0, f21, f22
    lbz      r0, 0xd4c(r25)
    frsp     f1, f1
    lfs      f21, 0xd40(r25)
    rlwinm.  r0, r0, 0, 29, 29
    lfs      f22, 0xd44(r25)
    fmadds   f20, f20, f1, f0
    beq      .L_80084748
    lha      r3, 0xd54(r25)
    lis      r0, 0x4330
    stw      r0, 0x510(r1)
    xoris    r0, r3, 0x8000
    lfd      f1, lbl_8065D050@sda21(r0)
    stw      r0, 0x514(r1)
    lfs      f2, lbl_8065D030@sda21(r0)
    lfd      f0, 0x510(r1)
    fsubs    f0, f0, f1
    fmuls    f1, f2, f0
    bl       sin
    frsp     f1, f1
    li       r0, 0x1
    lfs      f3, lbl_8065D05C@sda21(r0)
    addi     r3, r25, 0xbe8
    lfs      f0, lbl_8065D108@sda21(r0)
    stw      r0, 0xbac(r25)
    fmadds   f0, f0, f1, f3
    fmuls    f1, f21, f0
    fmuls    f2, f22, f0
    bl       PSMTXScale
.L_80084748:
    lbz      r0, 0xd4c(r25)
    rlwinm.  r0, r0, 0, 28, 28
    beq      .L_800847CC
    lha      r3, 0xd52(r25)
    lis      r0, 0x4330
    stw      r0, 0x510(r1)
    xoris    r0, r3, 0x8000
    lfd      f1, lbl_8065D050@sda21(r0)
    stw      r0, 0x514(r1)
    lfs      f2, lbl_8065D030@sda21(r0)
    lfd      f0, 0x510(r1)
    fsubs    f0, f0, f1
    fmuls    f1, f2, f0
    bl       sin
    frsp     f2, f1
    lfs      f0, lbl_8065D10C@sda21(r0)
    li       r0, 0x1
    lfs      f1, lbl_8065D058@sda21(r0)
    stw      r0, 0xbac(r25)
    addi     r3, r25, 0xc18
    fmuls    f21, f0, f2
    li       r4, 0x78
    bl       PSMTXRotRad
    lfs      f1, lbl_8065D058@sda21(r0)
    addi     r3, r25, 0xc48
    li       r4, 0x79
    bl       PSMTXRotRad
    lfs      f0, lbl_8065D068@sda21(r0)
    addi     r3, r25, 0xc78
    li       r4, 0x7a
    fnmsubs  f0, f21, f0, f17
    fadds    f1, f21, f0
    bl       PSMTXRotRad
.L_800847CC:
    fmr      f1, f18
    lfs      f3, lbl_8065D058@sda21(r0)
    fmr      f2, f20
    addi     r3, r1, 0x194
    bl       PSMTXTrans
    addi     r3, r1, 0x1c4
    addi     r4, r1, 0x194
    addi     r5, r1, 0x164
    bl       PSMTXConcat
    stw      r23, 0x8(r1)
    mr       r8, r23
    mr       r9, r23
    mr       r10, r23
    addi     r3, r25, 0xbac
    addi     r4, r1, 0x164
    li       r5, 0x0
    li       r6, 0x1e
    li       r7, 0x1
    bl       Draw__7CSpriteFPA4_f11_GXTexMapIDUl13_GXTexMtxTypeUlUlUlUl
    b        .L_800848B4
.L_8008481C:
    lfs      f1, 0xd3c(r25)
    lfs      f0, lbl_8065D058@sda21(r0)
    fcmpo    cr0, f1, f0
    ble      .L_8008488C
    bl       sin
    lfs      f4, 0xd48(r25)
    frsp     f2, f1
    lfs      f3, lbl_8065D058@sda21(r0)
    addi     r3, r1, 0x134
    fneg     f0, f4
    fmr      f1, f4
    fmuls    f2, f0, f2
    bl       PSMTXTrans
    addi     r3, r1, 0x1c4
    addi     r4, r1, 0x134
    addi     r5, r1, 0x104
    bl       PSMTXConcat
    stw      r23, 0x8(r1)
    mr       r8, r23
    mr       r9, r23
    mr       r10, r23
    addi     r3, r25, 0xbac
    addi     r4, r1, 0x104
    li       r5, 0x0
    li       r6, 0x1e
    li       r7, 0x1
    bl       Draw__7CSpriteFPA4_f11_GXTexMapIDUl13_GXTexMtxTypeUlUlUlUl
    b        .L_800848B4
.L_8008488C:
    stw      r23, 0x8(r1)
    mr       r8, r23
    mr       r9, r23
    mr       r10, r23
    addi     r3, r25, 0xbac
    addi     r4, r1, 0x1c4
    li       r5, 0x0
    li       r6, 0x1e
    li       r7, 0x1
    bl       Draw__7CSpriteFPA4_f11_GXTexMapIDUl13_GXTexMtxTypeUlUlUlUl
.L_800848B4:
    addi     r25, r25, 0x2ac
    addi     r22, r22, 0x1
.L_800848BC:
    lwz      r3, 0x3c78(r24)
    cmpw     r22, r3
    blt      .L_8008455C
    lwz      r3, 0x294(r28)
    li       r4, 0xff
    li       r5, 0xff
    li       r6, 0x4
    bl       fn_8019B040
    lwz      r3, 0x294(r28)
    li       r4, 0x4
    bl       fn_8019AFA0
    lwz      r4, 0x294(r28)
    li       r3, 0x0
    li       r0, 0x1
    stb      r3, 0x80(r4)
    stw      r0, 0x78(r4)
.L_800848FC:
    lbz      r0, 0x4(r30)
    extrwi.  r0, r0, 1, 27
    beq      .L_80084BDC
    addis    r3, r30, 0x4
    lwz      r0, 0x3c6c(r3)
    cmpwi    r0, 0x0
    blt      .L_80084BDC
    lis      r3, lbl_8025C400@ha
    li       r4, 0x0
    addi     r27, r3, lbl_8025C400@l
    li       r5, 0x0
    lwz      r3, 0x294(r27)
    li       r6, 0x4
    bl       fn_8019B040
    lwz      r3, 0x294(r27)
    li       r4, 0x0
    bl       fn_8019AFA0
    lwz      r7, 0x294(r27)
    li       r6, 0x1
    li       r5, 0x0
    li       r4, 0x4
    stb      r6, 0x80(r7)
    li       r0, 0x1e
    addis    r3, r30, 0x4
    stw      r6, 0x78(r7)
    lwz      r7, 0x294(r27)
    stw      r5, 0x84(r7)
    stw      r6, 0x88(r7)
    stw      r4, 0x8c(r7)
    stw      r0, 0x90(r7)
    stw      r6, 0x7c(r7)
    lwz      r0, 0x3c6c(r3)
    cmpwi    r0, 0x2
    blt      .L_80084988
    li       r0, 0x2
.L_80084988:
    slwi     r3, r0, 8
    addis    r22, r3, 0x4
    addi     r22, r22, 0x376c
    add      r22, r30, r22
    mr       r3, r22
    bl       GXGetTexObjWidth
    clrlwi   r3, r3, 16
    lis      r0, 0x4330
    stw      r3, 0x514(r1)
    mr       r3, r22
    lfd      f1, lbl_8065D048@sda21(r0)
    stw      r0, 0x510(r1)
    lfd      f0, 0x510(r1)
    fsubs    f15, f0, f1
    bl       GXGetTexObjHeight
    clrlwi   r0, r3, 16
    lis      r4, 0x4330
    stw      r0, 0x50c(r1)
    addis    r3, r30, 0x4
    lwz      r0, 0x3c6c(r3)
    stw      r4, 0x508(r1)
    lfd      f1, lbl_8065D048@sda21(r0)
    cmpwi    r0, 0x2
    lfd      f0, 0x508(r1)
    fsubs    f16, f0, f1
    blt      .L_80084A94
    mr       r3, r22
    bl       GXGetTexObjWidth
    lis      r4, 0x2e8c
    clrlwi   r3, r3, 16
    subi     r4, r4, 0x5d17
    lis      r0, 0x4330
    mulhw    r4, r4, r3
    lfs      f2, lbl_8065D05C@sda21(r0)
    stw      r0, 0x510(r1)
    addi     r3, r22, 0x6c
    lfd      f4, lbl_8065D050@sda21(r0)
    fmr      f3, f2
    srawi    r0, r4, 1
    lfs      f1, lbl_8065D110@sda21(r0)
    srwi     r4, r0, 31
    add      r0, r0, r4
    xoris    r0, r0, 0x8000
    stw      r0, 0x514(r1)
    lfd      f0, 0x510(r1)
    fsubs    f15, f0, f4
    bl       PSMTXScale
    li       r3, 0x1
    lis      r0, 0x4330
    stw      r3, 0x38(r22)
    addis    r4, r30, 0x4
    lfs      f2, lbl_8065D058@sda21(r0)
    addi     r3, r22, 0x3c
    lwz      r4, 0x3c6c(r4)
    stw      r0, 0x508(r1)
    fmr      f3, f2
    subi     r0, r4, 0x2
    lfd      f1, lbl_8065D050@sda21(r0)
    xoris    r0, r0, 0x8000
    lfs      f4, lbl_8065D110@sda21(r0)
    stw      r0, 0x50c(r1)
    lfd      f0, 0x508(r1)
    fsubs    f0, f0, f1
    fmuls    f1, f4, f0
    bl       PSMTXTrans
    li       r0, 0x1
    stw      r0, 0x38(r22)
.L_80084A94:
    addis    r5, r30, 0x4
    lfs      f1, lbl_8065D0BC@sda21(r0)
    lwz      r0, 0x3c78(r5)
    lfs      f0, 0x3ca0(r29)
    mulli    r0, r0, 0x2ac
    lfs      f5, lbl_8065D068@sda21(r0)
    lfs      f2, 0x3c94(r29)
    fadds    f0, f1, f0
    lwz      r4, 0x0(r30)
    add      r3, r30, r0
    lwz      r0, 0xa8c(r3)
    fmadds   f1, f25, f5, f2
    lfs      f2, 0x3c90(r5)
    slwi     r0, r0, 2
    lfs      f3, lbl_8065D0F8@sda21(r0)
    add      r3, r5, r0
    fadds    f4, f2, f1
    lfs      f1, 0x3ca8(r3)
    add      r3, r4, r0
    lfs      f2, 0x858(r3)
    fadds    f0, f1, f0
    lfs      f1, 0x3c70(r5)
    fmadds   f17, f15, f5, f4
    fmadds   f0, f2, f5, f0
    fnmsubs  f18, f16, f3, f0
    bl       sin
    frsp     f1, f1
    lfs      f0, lbl_8065D058@sda21(r0)
    fcmpo    cr0, f1, f0
    bge      .L_80084B10
    fmr      f1, f0
.L_80084B10:
    lfs      f0, lbl_8065D0F4@sda21(r0)
    fmuls    f1, f0, f1
    bl       __cvt_fp2unsigned
    lfs      f3, lbl_8065D058@sda21(r0)
    mr       r5, r3
    lfs      f0, lbl_8065D0E0@sda21(r0)
    lis      r3, lbl_8025C400@ha
    fmr      f4, f15
    li       r0, -0x100
    fmr      f5, f16
    addi     r3, r3, lbl_8025C400@l
    fmr      f6, f3
    mr       r4, r22
    fmr      f7, f3
    mr       r6, r5
    fmr      f8, f3
    mr       r7, r5
    fadds    f1, f0, f17
    mr       r8, r5
    fadds    f2, f0, f18
    or       r21, r5, r0
    bl       fn_80197800
    lfs      f3, lbl_8065D058@sda21(r0)
    lis      r3, lbl_8025C400@ha
    fmr      f1, f17
    addi     r3, r3, lbl_8025C400@l
    fmr      f2, f18
    mr       r4, r22
    fmr      f4, f15
    mr       r5, r21
    fmr      f5, f16
    mr       r6, r21
    fmr      f6, f3
    mr       r7, r21
    fmr      f7, f3
    mr       r8, r21
    fmr      f8, f3
    bl       fn_80197800
    lwz      r3, 0x294(r27)
    li       r4, 0xff
    li       r5, 0xff
    li       r6, 0x4
    bl       fn_8019B040
    lwz      r3, 0x294(r27)
    li       r4, 0x4
    bl       fn_8019AFA0
    lwz      r4, 0x294(r27)
    li       r3, 0x0
    li       r0, 0x1
    stb      r3, 0x80(r4)
    stw      r0, 0x78(r4)
.L_80084BDC:
    psq_l    f31, 0x658(r1), 0, qr0
    lfd      f31, 0x650(r1)
    psq_l    f30, 0x648(r1), 0, qr0
    lfd      f30, 0x640(r1)
    psq_l    f29, 0x638(r1), 0, qr0
    lfd      f29, 0x630(r1)
    psq_l    f28, 0x628(r1), 0, qr0
    lfd      f28, 0x620(r1)
    psq_l    f27, 0x618(r1), 0, qr0
    lfd      f27, 0x610(r1)
    psq_l    f26, 0x608(r1), 0, qr0
    lfd      f26, 0x600(r1)
    psq_l    f25, 0x5f8(r1), 0, qr0
    lfd      f25, 0x5f0(r1)
    psq_l    f24, 0x5e8(r1), 0, qr0
    lfd      f24, 0x5e0(r1)
    psq_l    f23, 0x5d8(r1), 0, qr0
    lfd      f23, 0x5d0(r1)
    psq_l    f22, 0x5c8(r1), 0, qr0
    lfd      f22, 0x5c0(r1)
    psq_l    f21, 0x5b8(r1), 0, qr0
    lfd      f21, 0x5b0(r1)
    psq_l    f20, 0x5a8(r1), 0, qr0
    lfd      f20, 0x5a0(r1)
    psq_l    f19, 0x598(r1), 0, qr0
    lfd      f19, 0x590(r1)
    psq_l    f18, 0x588(r1), 0, qr0
    lfd      f18, 0x580(r1)
    psq_l    f17, 0x578(r1), 0, qr0
    lfd      f17, 0x570(r1)
    psq_l    f16, 0x568(r1), 0, qr0
    lfd      f16, 0x560(r1)
    psq_l    f15, 0x558(r1), 0, qr0
    lfd      f15, 0x550(r1)
    lmw      r21, 0x524(r1)
    lwz      r0, 0x664(r1)
    mtlr     r0
    addi     r1, r1, 0x660
    blr
*/

/**
 * @note Address: 0x80084C78
 * @note Size: 0x148
 */
// a CJObj that starts out holding a jobj's current transform
CJObj::CJObj(HSD_JObj* jobj)
{
	Attach(jobj);
}

/**
 * @note Address: 0x80084DC0
 * @note Size: 0x684
 */
// CMsgFont_Update
/*
    stwu     r1, -0x90(r1)
    mflr     r0
    stw      r0, 0x94(r1)
    stw      r31, 0x8c(r1)
    stw      r30, 0x88(r1)
    stw      r29, 0x84(r1)
    mr       r29, r4
    stw      r28, 0x80(r1)
    mr       r28, r3
    lbz      r3, 0x4(r3)
    extrwi.  r0, r3, 1, 24
    beq      .L_800850BC
    extrwi.  r0, r3, 1, 25
    beq      .L_80084E88
    addis    r3, r28, 0x4
    lfs      f0, lbl_8065D05C@sda21(r0)
    lfs      f1, 0x3d10(r3)
    fcmpo    cr0, f1, f0
    bge      .L_80084E88
    lbz      r0, 0x11b(r28)
    cmplwi   r0, 0x0
    bne      .L_80084E38
    lwz      r0, 0xaa8(r28)
    cmpwi    r0, 0x0
    bge      .L_80084E38
    stfs     f0, 0x3d10(r3)
    li       r3, 0x0
    lbz      r0, 0x4(r28)
    rlwimi   r0, r3, 6, 25, 25
    stb      r0, 0x4(r28)
.L_80084E38:
    lis      r0, 0x4330
    addis    r3, r28, 0x4
    stw      r29, 0xc(r1)
    lfd      f2, lbl_8065D048@sda21(r0)
    stw      r0, 0x8(r1)
    lfs      f3, lbl_8065D11C@sda21(r0)
    lfd      f0, 0x8(r1)
    lfs      f1, 0x3d10(r3)
    fsubs    f2, f0, f2
    lfs      f0, lbl_8065D05C@sda21(r0)
    fmadds   f1, f3, f2, f1
    fcmpo    cr0, f1, f0
    stfs     f1, 0x3d10(r3)
    cror     eq, gt, eq
    bne      .L_80084E88
    stfs     f0, 0x3d10(r3)
    li       r3, 0x0
    lbz      r0, 0x4(r28)
    rlwimi   r0, r3, 6, 25, 25
    stb      r0, 0x4(r28)
.L_80084E88:
    lbz      r0, 0x4(r28)
    extrwi.  r0, r0, 1, 26
    beq      .L_80084F04
    addis    r3, r28, 0x4
    lfs      f3, lbl_8065D058@sda21(r0)
    lfs      f4, 0x3d10(r3)
    fcmpo    cr0, f4, f3
    ble      .L_80084EF8
    lis      r0, 0x4330
    stw      r29, 0xc(r1)
    lfd      f1, lbl_8065D048@sda21(r0)
    stw      r0, 0x8(r1)
    lfs      f2, lbl_8065D11C@sda21(r0)
    lfd      f0, 0x8(r1)
    fsubs    f0, f0, f1
    fnmsubs  f0, f2, f0, f4
    fcmpo    cr0, f0, f3
    stfs     f0, 0x3d10(r3)
    cror     eq, lt, eq
    bne      .L_80084EF8
    stfs     f3, 0x3d10(r3)
    li       r3, 0x0
    lbz      r0, 0x4(r28)
    rlwimi   r0, r3, 5, 26, 26
    stb      r0, 0x4(r28)
    lbz      r0, 0x4(r28)
    rlwimi   r0, r3, 7, 24, 24
    stb      r0, 0x4(r28)
.L_80084EF8:
    addis    r3, r28, 0x4
    lfs      f0, 0x3d10(r3)
    stfs     f0, 0x3d14(r3)
.L_80084F04:
    addis    r3, r28, 0x4
    lwz      r0, 0x3cec(r3)
    cmpwi    r0, 0x0
    bge      .L_80084F20
    lwz      r0, 0x3d0c(r3)
    cmpwi    r0, 0x0
    blt      .L_80084F6C
.L_80084F20:
    addis    r3, r28, 0x4
    lfs      f3, lbl_8065D05C@sda21(r0)
    lfs      f4, 0x3d18(r3)
    fcmpo    cr0, f4, f3
    bge      .L_80084FB0
    lis      r0, 0x4330
    stw      r29, 0xc(r1)
    lfd      f1, lbl_8065D048@sda21(r0)
    stw      r0, 0x8(r1)
    lfs      f2, lbl_8065D11C@sda21(r0)
    lfd      f0, 0x8(r1)
    fsubs    f0, f0, f1
    fmadds   f0, f2, f0, f4
    fcmpo    cr0, f0, f3
    stfs     f0, 0x3d18(r3)
    cror     eq, gt, eq
    bne      .L_80084FB0
    stfs     f3, 0x3d18(r3)
    b        .L_80084FB0
.L_80084F6C:
    lfs      f4, 0x3d18(r3)
    lfs      f3, lbl_8065D058@sda21(r0)
    fcmpo    cr0, f4, f3
    ble      .L_80084FB0
    lis      r0, 0x4330
    stw      r29, 0xc(r1)
    lfd      f1, lbl_8065D048@sda21(r0)
    stw      r0, 0x8(r1)
    lfs      f2, lbl_8065D11C@sda21(r0)
    lfd      f0, 0x8(r1)
    fsubs    f0, f0, f1
    fnmsubs  f0, f2, f0, f4
    fcmpo    cr0, f0, f3
    stfs     f0, 0x3d18(r3)
    cror     eq, lt, eq
    bne      .L_80084FB0
    stfs     f3, 0x3d18(r3)
.L_80084FB0:
    addis    r3, r28, 0x4
    lwz      r4, 0x3cec(r3)
    cmpwi    r4, 0x0
    blt      .L_8008506C
    lbz      r0, 0x4(r28)
    extrwi.  r0, r0, 1, 28
    beq      .L_80084FE0
    lwz      r3, 0x0(r28)
    slwi     r0, r4, 2
    add      r3, r3, r0
    lfs      f7, 0x830(r3)
    b        .L_80084FE8
.L_80084FE0:
    lwz      r3, 0x0(r28)
    lfs      f7, 0x880(r3)
.L_80084FE8:
    addis    r5, r28, 0x4
    lis      r0, 0x4330
    lwz      r4, 0x3cec(r5)
    addi     r3, r5, 0x3ba8
    lfs      f2, lbl_8065D058@sda21(r0)
    slwi     r4, r4, 2
    lfs      f4, 0x3cf0(r5)
    add      r4, r5, r4
    lfs      f5, lbl_8065D120@sda21(r0)
    lfs      f0, 0x3ca8(r4)
    fmr      f3, f2
    lfs      f6, lbl_8065D124@sda21(r0)
    fsubs    f0, f0, f4
    stw      r29, 0xc(r1)
    lfd      f1, lbl_8065D048@sda21(r0)
    stw      r0, 0x8(r1)
    fmadds   f5, f5, f0, f4
    lfs      f4, lbl_8065D128@sda21(r0)
    lfd      f0, 0x8(r1)
    stfs     f5, 0x3cf0(r5)
    fsubs    f1, f0, f1
    lfs      f5, 0x3cf4(r5)
    fsubs    f0, f7, f5
    fmadds   f0, f6, f0, f5
    stfs     f0, 0x3cf4(r5)
    lfs      f0, 0x3c74(r5)
    fnmsubs  f0, f4, f1, f0
    stfs     f0, 0x3c74(r5)
    lfs      f1, 0x3c74(r5)
    bl       PSMTXTrans
    addis    r3, r28, 0x4
    li       r0, 0x1
    stw      r0, 0x3ba4(r3)
.L_8008506C:
    lbz      r0, 0x4(r28)
    extrwi.  r0, r0, 1, 27
    beq      .L_800850BC
    lis      r0, 0x4330
    addis    r3, r28, 0x4
    stw      r29, 0xc(r1)
    lfd      f2, lbl_8065D048@sda21(r0)
    stw      r0, 0x8(r1)
    lfs      f3, lbl_8065D12C@sda21(r0)
    lfd      f1, 0x8(r1)
    lfs      f0, 0x3c70(r3)
    fsubs    f1, f1, f2
    lfd      f2, lbl_8065D130@sda21(r0)
    fmadds   f0, f3, f1, f0
    stfs     f0, 0x3c70(r3)
    lfs      f1, 0x3c70(r3)
    bl       fmod
    frsp     f0, f1
    addis    r3, r28, 0x4
    stfs     f0, 0x3c70(r3)
.L_800850BC:
    lbz      r0, 0x4(r28)
    extrwi.  r0, r0, 1, 24
    beq      .L_80085380
    li       r30, 0x0
    mr       r31, r28
.L_800850D0:
    lbz      r0, 0xd4c(r31)
    rlwinm   r0, r0, 0, 24, 27
    cmpwi    r0, 0x20
    beq      .L_800852A4
    bge      .L_80085370
    cmpwi    r0, 0x10
    beq      .L_800850F0
    b        .L_80085370
.L_800850F0:
    lbz      r3, 0xd56(r31)
    lis      r0, 0x4330
    lha      r4, 0xd50(r31)
    extsb    r3, r3
    stw      r0, 0x8(r1)
    mullw    r0, r3, r29
    lfd      f1, lbl_8065D048@sda21(r0)
    lfd      f2, lbl_8065D138@sda21(r0)
    add      r0, r4, r0
    stw      r0, 0xc(r1)
    lfd      f0, 0x8(r1)
    fsubs    f1, f0, f1
    bl       fmod
    frsp     f0, f1
    lis      r0, 0x4330
    stw      r0, 0x18(r1)
    lfd      f4, lbl_8065D050@sda21(r0)
    fctiwz   f0, f0
    stw      r29, 0x24(r1)
    lfs      f5, lbl_8065D068@sda21(r0)
    stw      r0, 0x20(r1)
    lfd      f1, lbl_8065D048@sda21(r0)
    stfd     f0, 0x10(r1)
    lfd      f0, 0x20(r1)
    lwz      r3, 0x14(r1)
    stw      r0, 0x28(r1)
    fsubs    f1, f0, f1
    lfd      f2, lbl_8065D138@sda21(r0)
    sth      r3, 0xd50(r31)
    lbz      r3, 0xd56(r31)
    lha      r0, 0xd4e(r31)
    extsb    r3, r3
    xoris    r3, r3, 0x8000
    xoris    r0, r0, 0x8000
    stw      r3, 0x1c(r1)
    lfd      f0, 0x18(r1)
    stw      r0, 0x2c(r1)
    fsubs    f3, f0, f4
    lfd      f0, 0x28(r1)
    fmuls    f3, f5, f3
    fsubs    f0, f0, f4
    fmadds   f1, f3, f1, f0
    bl       fmod
    frsp     f0, f1
    lis      r0, 0x4330
    stw      r0, 0x38(r1)
    lfd      f4, lbl_8065D050@sda21(r0)
    fctiwz   f0, f0
    stw      r29, 0x44(r1)
    lfs      f5, lbl_8065D124@sda21(r0)
    stw      r0, 0x40(r1)
    lfd      f1, lbl_8065D048@sda21(r0)
    stfd     f0, 0x30(r1)
    lfd      f0, 0x40(r1)
    lwz      r3, 0x34(r1)
    stw      r0, 0x48(r1)
    fsubs    f1, f0, f1
    lfd      f2, lbl_8065D138@sda21(r0)
    sth      r3, 0xd4e(r31)
    lbz      r3, 0xd56(r31)
    lha      r0, 0xd52(r31)
    extsb    r3, r3
    xoris    r3, r3, 0x8000
    xoris    r0, r0, 0x8000
    stw      r3, 0x3c(r1)
    lfd      f0, 0x38(r1)
    stw      r0, 0x4c(r1)
    fsubs    f3, f0, f4
    lfd      f0, 0x48(r1)
    fmuls    f3, f5, f3
    fsubs    f0, f0, f4
    fmadds   f1, f3, f1, f0
    bl       fmod
    frsp     f0, f1
    lis      r0, 0x4330
    stw      r0, 0x58(r1)
    lfd      f4, lbl_8065D050@sda21(r0)
    fctiwz   f0, f0
    stw      r29, 0x64(r1)
    lfs      f5, lbl_8065D120@sda21(r0)
    stw      r0, 0x60(r1)
    lfd      f1, lbl_8065D048@sda21(r0)
    stfd     f0, 0x50(r1)
    lfd      f0, 0x60(r1)
    lwz      r3, 0x54(r1)
    stw      r0, 0x68(r1)
    fsubs    f1, f0, f1
    lfd      f2, lbl_8065D138@sda21(r0)
    sth      r3, 0xd52(r31)
    lbz      r3, 0xd56(r31)
    lha      r0, 0xd54(r31)
    extsb    r3, r3
    xoris    r3, r3, 0x8000
    xoris    r0, r0, 0x8000
    stw      r3, 0x5c(r1)
    lfd      f0, 0x58(r1)
    stw      r0, 0x6c(r1)
    fsubs    f3, f0, f4
    lfd      f0, 0x68(r1)
    fmuls    f3, f5, f3
    fsubs    f0, f0, f4
    fmadds   f1, f3, f1, f0
    bl       fmod
    frsp     f0, f1
    fctiwz   f0, f0
    stfd     f0, 0x70(r1)
    lwz      r0, 0x74(r1)
    sth      r0, 0xd54(r31)
    b        .L_80085370
.L_800852A4:
    lbz      r3, 0xd57(r31)
    subi     r3, r3, 0x1
    extsb.   r0, r3
    stb      r3, 0xd57(r31)
    bgt      .L_80085370
    bl       rand
    lis      r4, 0xb60b
    addi     r0, r4, 0x60b7
    mulhw    r0, r0, r3
    add      r0, r0, r3
    srawi    r0, r0, 8
    srwi     r4, r0, 31
    add      r0, r0, r4
    mulli    r0, r0, 0x168
    subf     r0, r0, r3
    sth      r0, 0xd50(r31)
    bl       rand
    lis      r4, 0xb60b
    addi     r0, r4, 0x60b7
    mulhw    r0, r0, r3
    add      r0, r0, r3
    srawi    r0, r0, 8
    srwi     r4, r0, 31
    add      r0, r0, r4
    mulli    r0, r0, 0x168
    subf     r0, r0, r3
    sth      r0, 0xd4e(r31)
    bl       rand
    lis      r4, 0xb60b
    addi     r0, r4, 0x60b7
    mulhw    r0, r0, r3
    add      r0, r0, r3
    srawi    r0, r0, 8
    srwi     r4, r0, 31
    add      r0, r0, r4
    mulli    r0, r0, 0x168
    subf     r0, r0, r3
    sth      r0, 0xd52(r31)
    bl       rand
    lis      r4, 0xb60b
    addi     r0, r4, 0x60b7
    mulhw    r0, r0, r3
    add      r0, r0, r3
    srawi    r0, r0, 8
    srwi     r4, r0, 31
    add      r0, r0, r4
    mulli    r0, r0, 0x168
    subf     r0, r0, r3
    sth      r0, 0xd54(r31)
    lbz      r0, 0xd56(r31)
    stb      r0, 0xd57(r31)
.L_80085370:
    addi     r30, r30, 0x1
    addi     r31, r31, 0x2ac
    cmpwi    r30, 0x190
    blt      .L_800850D0
.L_80085380:
    lis      r0, 0x4330
    stw      r29, 0x74(r1)
    lfd      f1, lbl_8065D048@sda21(r0)
    addi     r3, r28, 0x44
    stw      r0, 0x70(r1)
    lfs      f2, lbl_8065D140@sda21(r0)
    lfd      f0, 0x70(r1)
    lfs      f4, 0x108(r28)
    fsubs    f0, f0, f1
    lfs      f3, lbl_8065D058@sda21(r0)
    fmuls    f1, f2, f0
    fadds    f0, f4, f1
    stfs     f0, 0x108(r28)
    lfs      f0, 0x10c(r28)
    fadds    f0, f0, f1
    stfs     f0, 0x10c(r28)
    lfs      f1, 0x108(r28)
    lfs      f2, 0x10c(r28)
    bl       PSMTXTrans
    li       r5, 0x1
    stw      r5, 0x40(r28)
    lwz      r0, 0xaa8(r28)
    cmpwi    r0, 0x0
    blt      .L_80085424
    cmpwi    r0, 0x1
    bge      .L_80085424
    lwz      r3, 0x34c(r28)
    li       r4, 0x0
    lwz      r0, 0x348(r28)
    and      r3, r3, r5
    and      r0, r0, r4
    xor      r3, r3, r4
    xor      r0, r0, r4
    or.      r0, r3, r0
    beq      .L_80085424
    addi     r3, r28, 0x120
    mr       r4, r29
    lwz      r12, 0x154(r28)
    lwz      r12, 0xa4(r12)
    mtctr    r12
    bctrl
.L_80085424:
    lwz      r0, 0x94(r1)
    lwz      r31, 0x8c(r1)
    lwz      r30, 0x88(r1)
    lwz      r29, 0x84(r1)
    lwz      r28, 0x80(r1)
    mtlr     r0
    addi     r1, r1, 0x90
    blr
*/

/**
 * @note Address: 0x80085444
 * @note Size: 0x6F0
 */
// CMsgFont_AddChar
/*
    stwu     r1, -0x190(r1)
    mflr     r0
    stw      r0, 0x194(r1)
    stfd     f31, 0x180(r1)
    psq_st   f31, 0x188(r1), 0, qr0
    stfd     f30, 0x170(r1)
    psq_st   f30, 0x178(r1), 0, qr0
    stfd     f29, 0x160(r1)
    psq_st   f29, 0x168(r1), 0, qr0
    stfd     f28, 0x150(r1)
    psq_st   f28, 0x158(r1), 0, qr0
    stfd     f27, 0x140(r1)
    psq_st   f27, 0x148(r1), 0, qr0
    stw      r31, 0x13c(r1)
    stw      r30, 0x138(r1)
    stw      r29, 0x134(r1)
    stw      r28, 0x130(r1)
    mr       r29, r3
    lis      r3, lbl_8025C400@ha
    addis    r5, r29, 0x4
    lbz      r0, 0x4(r29)
    lfs      f1, 0x3cd0(r5)
    addi     r3, r3, lbl_8025C400@l
    mr       r30, r4
    extrwi   r5, r0, 1, 29
    bl       fn_801968BC
    addis    r3, r29, 0x4
    lis      r0, 0x7f80
    lfs      f0, 0x3cd4(r3)
    fmr      f30, f1
    stfs     f0, 0x8(r1)
    lwz      r4, 0x8(r1)
    rlwinm   r3, r4, 0, 1, 8
    cmpw     r3, r0
    beq      .L_800854E0
    bge      .L_80085510
    cmpwi    r3, 0x0
    beq      .L_800854F8
    b        .L_80085510
.L_800854E0:
    clrlwi.  r0, r4, 9
    beq      .L_800854F0
    li       r0, 0x1
    b        .L_80085514
.L_800854F0:
    li       r0, 0x2
    b        .L_80085514
.L_800854F8:
    clrlwi.  r0, r4, 9
    beq      .L_80085508
    li       r0, 0x5
    b        .L_80085514
.L_80085508:
    li       r0, 0x3
    b        .L_80085514
.L_80085510:
    li       r0, 0x4
.L_80085514:
    cmpwi    r0, 0x1
    bne      .L_80085524
    lfs      f31, lbl_8065D058@sda21(r0)
    b        .L_8008553C
.L_80085524:
    addis    r3, r29, 0x4
    lfs      f0, lbl_8065D068@sda21(r0)
    lfs      f2, 0x3cd0(r3)
    lfs      f1, 0x3cd4(r3)
    fsubs    f1, f2, f1
    fmuls    f31, f1, f0
.L_8008553C:
    lbz      r0, 0x0(r30)
    cmplwi   r0, 0x11
    bne      .L_80085700
    lis      r3, lbl_8025C400@ha
    li       r5, 0x0
    addi     r3, r3, lbl_8025C400@l
    lwz      r4, 0x3d4c(r3)
    addi     r3, r3, 0x3d6c
    lwz      r4, 0x8(r4)
    addi     r4, r4, 0x3b8
    bl       Init__7CTexObjFP13TPLDescriptorUl
    lwz      r5, 0x0(r3)
    lis      r4, __vt__7CTexObj@ha
    lwz      r7, 0x4(r3)
    addi     r31, r1, 0x48
    addi     r6, r4, __vt__7CTexObj@l
    li       r0, 0x18
    stw      r5, 0xc(r1)
    subi     r5, r31, 0x4
    addi     r4, r3, 0x38
    stw      r7, 0x10(r1)
    lwz      r8, 0x8(r3)
    lwz      r7, 0xc(r3)
    stw      r8, 0x14(r1)
    stw      r7, 0x18(r1)
    lwz      r8, 0x10(r3)
    lwz      r7, 0x14(r3)
    stw      r8, 0x1c(r1)
    stw      r7, 0x20(r1)
    lwz      r8, 0x18(r3)
    lwz      r7, 0x1c(r3)
    stw      r8, 0x24(r1)
    stw      r7, 0x28(r1)
    stw      r6, 0x108(r1)
    lwz      r6, 0x20(r3)
    stw      r6, 0x2c(r1)
    lwz      r6, 0x24(r3)
    stw      r6, 0x30(r1)
    lwz      r7, 0x28(r3)
    lwz      r6, 0x2c(r3)
    stw      r7, 0x34(r1)
    stw      r6, 0x38(r1)
    lwz      r6, 0x30(r3)
    stw      r6, 0x3c(r1)
    lwz      r6, 0x34(r3)
    stw      r6, 0x40(r1)
    lwz      r3, 0x38(r3)
    stw      r3, 0x44(r1)
    mtctr    r0
.L_80085600:
    lwz      r3, 0x4(r4)
    lwzu     r0, 0x8(r4)
    stw      r3, 0x4(r5)
    stwu     r0, 0x8(r5)
    bdnz     .L_80085600
    lwz      r3, 0x0(r29)
    lbz      r28, 0x1(r30)
    lwz      r30, 0x894(r3)
    cmpw     r28, r30
    blt      .L_8008562C
    subi     r28, r30, 0x1
.L_8008562C:
    xoris    r3, r30, 0x8000
    lis      r0, 0x4330
    stw      r3, 0x114(r1)
    addi     r3, r1, 0x78
    lfs      f2, lbl_8065D05C@sda21(r0)
    stw      r0, 0x110(r1)
    lfd      f1, lbl_8065D050@sda21(r0)
    fmr      f3, f2
    lfd      f0, 0x110(r1)
    fsubs    f0, f0, f1
    fdivs    f29, f2, f0
    fmr      f1, f29
    bl       PSMTXScale
    xoris    r3, r28, 0x8000
    lis      r0, 0x4330
    stw      r3, 0x11c(r1)
    li       r4, 0x1
    lfs      f2, lbl_8065D058@sda21(r0)
    mr       r3, r31
    stw      r0, 0x118(r1)
    lfd      f1, lbl_8065D050@sda21(r0)
    fmr      f3, f2
    lfd      f0, 0x118(r1)
    stw      r4, 0x44(r1)
    fsubs    f0, f0, f1
    fmuls    f1, f29, f0
    bl       PSMTXTrans
    li       r0, 0x1
    addis    r3, r29, 0x4
    stw      r0, 0x44(r1)
    addi     r4, r1, 0xc
    lwz      r0, 0x3c78(r3)
    mulli    r3, r0, 0x2ac
    addi     r3, r3, 0xaac
    add      r3, r29, r3
    bl       Set__7CTexObjFRC7CTexObj
    divw     r3, r3, r30
    addis    r4, r29, 0x4
    lis      r5, 0x4330
    lfs      f1, 0x3cd0(r4)
    lfs      f0, lbl_8065D06C@sda21(r0)
    lis      r4, __vt__7CTexObj@ha
    xoris    r0, r3, 0x8000
    fdivs    f0, f1, f0
    stw      r0, 0x124(r1)
    addi     r0, r4, __vt__7CTexObj@l
    lfd      f2, lbl_8065D050@sda21(r0)
    stw      r5, 0x120(r1)
    lfd      f1, 0x120(r1)
    fsubs    f1, f1, f2
    stw      r0, 0x108(r1)
    fmuls    f30, f1, f0
    b        .L_8008571C
.L_80085700:
    addis    r3, r29, 0x4
    mr       r4, r30
    lwz      r0, 0x3c78(r3)
    mulli    r3, r0, 0x2ac
    addi     r0, r3, 0xd14
    lwzx     r3, r29, r0
    bl       SetChar__7CTexObjFPCc
.L_8008571C:
    xoris    r0, r3, 0x8000
    lis      r4, 0x4330
    stw      r0, 0x124(r1)
    addis    r5, r29, 0x4
    lwz      r3, 0x3c7c(r5)
    li       r0, 0x1
    stw      r4, 0x120(r1)
    lfd      f2, lbl_8065D050@sda21(r0)
    slwi     r3, r3, 2
    lfd      f0, 0x120(r1)
    lwz      r4, 0x0(r29)
    fsubs    f2, f0, f2
    lfs      f1, lbl_8065D144@sda21(r0)
    lfs      f0, 0x3c90(r5)
    add      r4, r4, r3
    lwz      r3, 0x3c78(r5)
    fadds    f0, f1, f0
    mulli    r3, r3, 0x2ac
    fsubs    f2, f30, f2
    lfs      f4, lbl_8065D068@sda21(r0)
    fadds    f1, f0, f31
    lfs      f0, 0x3c9c(r5)
    fmuls    f3, f2, f4
    lfs      f2, 0x858(r4)
    addi     r3, r3, 0xbac
    add      r3, r29, r3
    fadds    f1, f3, f1
    stw      r0, 0x0(r3)
    fmadds   f2, f2, f4, f0
    lfs      f3, lbl_8065D058@sda21(r0)
    addi     r3, r3, 0xc
    bl       PSMTXTrans
    addis    r3, r29, 0x4
    lfs      f0, lbl_8065D058@sda21(r0)
    lfs      f2, 0x3ce0(r3)
    fcmpo    cr0, f2, f0
    cror     eq, gt, eq
    bne      .L_800857D0
    lfs      f1, 0x3ce4(r3)
    fcmpo    cr0, f1, f0
    cror     eq, gt, eq
    bne      .L_800857D0
    fmr      f4, f2
    fmr      f5, f1
    b        .L_8008585C
.L_800857D0:
    lfs      f1, lbl_8065D034@sda21(r0)
    lfs      f0, lbl_8065D058@sda21(r0)
    fmuls    f29, f1, f2
    fcmpo    cr0, f29, f0
    bge      .L_800857E8
    fneg     f29, f29
.L_800857E8:
    addis    r3, r29, 0x4
    lfs      f2, lbl_8065D034@sda21(r0)
    lfs      f1, 0x3ce4(r3)
    lfs      f0, lbl_8065D058@sda21(r0)
    fmuls    f28, f2, f1
    fcmpo    cr0, f28, f0
    bge      .L_80085808
    fneg     f28, f28
.L_80085808:
    bl       rand
    xoris    r3, r3, 0x8000
    lis      r0, 0x4330
    stw      r3, 0x124(r1)
    fsubs    f2, f28, f29
    lfd      f1, lbl_8065D050@sda21(r0)
    stw      r0, 0x120(r1)
    lfd      f0, 0x120(r1)
    fsubs    f1, f0, f1
    bl       fmod
    addis    r3, r29, 0x4
    lfs      f0, lbl_8065D058@sda21(r0)
    lfs      f2, 0x3ce0(r3)
    frsp     f1, f1
    fcmpo    cr0, f2, f0
    bge      .L_8008584C
    fneg     f2, f2
.L_8008584C:
    lfs      f0, lbl_8065D034@sda21(r0)
    fdivs    f0, f1, f0
    fadds    f5, f2, f0
    fmr      f4, f5
.L_8008585C:
    addis    r3, r29, 0x4
    lfs      f2, lbl_8065D068@sda21(r0)
    lfs      f1, 0x3cd0(r3)
    li       r0, 0x1
    lwz      r3, 0x3c78(r3)
    fneg     f0, f1
    lfs      f3, lbl_8065D05C@sda21(r0)
    fmuls    f1, f1, f2
    mulli    r3, r3, 0x2ac
    fmuls    f0, f0, f2
    fmuls    f29, f1, f4
    addi     r3, r3, 0xbac
    add      r3, r29, r3
    fmuls    f27, f0, f5
    stw      r0, 0x0(r3)
    fmr      f1, f29
    addi     r3, r3, 0x3c
    fmr      f2, f27
    bl       PSMTXScale
    addis    r3, r29, 0x4
    lfs      f1, lbl_8065D058@sda21(r0)
    lfs      f28, 0x3cdc(r3)
    fcmpo    cr0, f28, f1
    cror     eq, gt, eq
    bne      .L_800858C4
    b        .L_80085918
.L_800858C4:
    lfs      f0, lbl_8065D0E0@sda21(r0)
    fmuls    f28, f0, f28
    fcmpo    cr0, f28, f1
    bge      .L_800858D8
    fneg     f28, f28
.L_800858D8:
    bl       rand
    xoris    r3, r3, 0x8000
    lis      r0, 0x4330
    lfs      f0, lbl_8065D148@sda21(r0)
    stw      r3, 0x124(r1)
    lfd      f1, lbl_8065D050@sda21(r0)
    fmuls    f2, f0, f28
    stw      r0, 0x120(r1)
    lfd      f0, 0x120(r1)
    fsubs    f1, f0, f1
    bl       fmod
    addis    r3, r29, 0x4
    frsp     f2, f1
    lfs      f1, lbl_8065D030@sda21(r0)
    lfs      f0, 0x3cdc(r3)
    fmadds   f28, f1, f2, f0
.L_80085918:
    addis    r3, r29, 0x4
    li       r0, 0x1
    lwz      r3, 0x3c78(r3)
    li       r4, 0x78
    lfs      f1, lbl_8065D058@sda21(r0)
    mulli    r3, r3, 0x2ac
    addi     r30, r3, 0xbac
    add      r30, r29, r30
    stw      r0, 0x0(r30)
    addi     r3, r30, 0x6c
    bl       PSMTXRotRad
    lfs      f1, lbl_8065D058@sda21(r0)
    addi     r3, r30, 0x9c
    li       r4, 0x79
    bl       PSMTXRotRad
    fmr      f1, f28
    addi     r3, r30, 0xcc
    li       r4, 0x7a
    bl       PSMTXRotRad
    addis    r5, r29, 0x4
    lfs      f0, lbl_8065D150@sda21(r0)
    lwz      r0, 0x3c78(r5)
    lwz      r4, 0x3cd8(r5)
    fcmpo    cr0, f28, f0
    mulli    r0, r0, 0x2ac
    add      r3, r29, r0
    stw      r4, 0xd34(r3)
    lwz      r0, 0x3c78(r5)
    lwz      r4, 0x3c7c(r5)
    mulli    r0, r0, 0x2ac
    add      r3, r29, r0
    stw      r4, 0xd38(r3)
    lwz      r0, 0x3c78(r5)
    mulli    r0, r0, 0x2ac
    add      r3, r29, r0
    stfs     f28, 0xd3c(r3)
    lwz      r0, 0x3c78(r5)
    mulli    r0, r0, 0x2ac
    add      r3, r29, r0
    stfs     f29, 0xd40(r3)
    lwz      r0, 0x3c78(r5)
    mulli    r0, r0, 0x2ac
    add      r3, r29, r0
    stfs     f27, 0xd44(r3)
    cror     eq, lt, eq
    bne      .L_800859D4
    b        .L_800859DC
.L_800859D4:
    lfs      f0, lbl_8065D14C@sda21(r0)
    fsubs    f28, f0, f28
.L_800859DC:
    lfs      f1, lbl_8065D150@sda21(r0)
    addis    r5, r29, 0x4
    lfs      f0, 0x3cd0(r5)
    fdivs    f1, f28, f1
    lwz      r0, 0x3c78(r5)
    mulli    r0, r0, 0x2ac
    add      r3, r29, r0
    fsubs    f0, f30, f0
    fmuls    f0, f1, f0
    stfs     f0, 0xd48(r3)
    lwz      r0, 0x3c78(r5)
    lbz      r4, 0x3ce8(r5)
    mulli    r0, r0, 0x2ac
    add      r3, r29, r0
    stb      r4, 0xd4c(r3)
    lwz      r0, 0x3c78(r5)
    lbz      r4, 0x3ce9(r5)
    mulli    r0, r0, 0x2ac
    add      r3, r29, r0
    stb      r4, 0xd4d(r3)
    bl       rand
    lis      r5, 0xb60b
    addis    r4, r29, 0x4
    addi     r5, r5, 0x60b7
    lwz      r0, 0x3c78(r4)
    mulhw    r4, r5, r3
    add      r4, r4, r3
    srawi    r4, r4, 8
    srwi     r5, r4, 31
    add      r4, r4, r5
    mulli    r4, r4, 0x168
    mulli    r0, r0, 0x2ac
    subf     r4, r4, r3
    add      r3, r29, r0
    sth      r4, 0xd52(r3)
    bl       rand
    lis      r4, 0xb60b
    addis    r5, r29, 0x4
    addi     r4, r4, 0x60b7
    lwz      r0, 0x3c78(r5)
    mulhw    r4, r4, r3
    fadds    f0, f30, f31
    add      r4, r4, r3
    srawi    r4, r4, 8
    srwi     r6, r4, 31
    add      r4, r4, r6
    mulli    r4, r4, 0x168
    mulli    r0, r0, 0x2ac
    subf     r4, r4, r3
    add      r3, r29, r0
    sth      r4, 0xd54(r3)
    lwz      r0, 0x3c78(r5)
    lbz      r4, 0x3cea(r5)
    mulli    r0, r0, 0x2ac
    add      r3, r29, r0
    stb      r4, 0xd57(r3)
    lwz      r0, 0x3c78(r5)
    mulli    r0, r0, 0x2ac
    add      r3, r29, r0
    stb      r4, 0xd56(r3)
    lfs      f1, 0x3c90(r5)
    fadds    f0, f1, f0
    stfs     f0, 0x3c90(r5)
    lfs      f0, 0x3cd0(r5)
    stfs     f0, 0x3cd4(r5)
    lwz      r3, 0x3c78(r5)
    addi     r0, r3, 0x1
    stw      r0, 0x3c78(r5)
    psq_l    f31, 0x188(r1), 0, qr0
    lfd      f31, 0x180(r1)
    psq_l    f30, 0x178(r1), 0, qr0
    lfd      f30, 0x170(r1)
    psq_l    f29, 0x168(r1), 0, qr0
    lfd      f29, 0x160(r1)
    psq_l    f28, 0x158(r1), 0, qr0
    lfd      f28, 0x150(r1)
    psq_l    f27, 0x148(r1), 0, qr0
    lfd      f27, 0x140(r1)
    lwz      r31, 0x13c(r1)
    lwz      r30, 0x138(r1)
    lwz      r29, 0x134(r1)
    lwz      r0, 0x194(r1)
    lwz      r28, 0x130(r1)
    mtlr     r0
    addi     r1, r1, 0x190
    blr
*/

/**
 * @note Address: 0x80085B34
 * @note Size: 0x950
 */
// CMsgFont_Init
/*
    stwu     r1, -0xb0(r1)
    mflr     r0
    stw      r0, 0xb4(r1)
    stfd     f31, 0xa0(r1)
    psq_st   f31, 0xa8(r1), 0, qr0
    stfd     f30, 0x90(r1)
    psq_st   f30, 0x98(r1), 0, qr0
    stfd     f29, 0x80(r1)
    psq_st   f29, 0x88(r1), 0, qr0
    stfd     f28, 0x70(r1)
    psq_st   f28, 0x78(r1), 0, qr0
    stfd     f27, 0x60(r1)
    psq_st   f27, 0x68(r1), 0, qr0
    stmw     r23, 0x3c(r1)
    stw      r4, 0x0(r3)
    mr       r27, r3
    lis      r10, lbl_80208ED0@ha
    lis      r9, __float_nan@ha
    lbz      r0, 0x4(r3)
    li       r8, 0x0
    rlwimi   r0, r8, 7, 24, 24
    addi     r11, r9, __float_nan@l
    stb      r0, 0x4(r27)
    mr       r29, r7
    mr       r28, r5
    lis      r3, 0xb60b
    lbz      r0, 0x4(r27)
    rlwimi   r0, r8, 6, 25, 25
    li       r12, 0x1
    addis    r24, r27, 0x4
    stb      r0, 0x4(r27)
    addi     r23, r10, lbl_80208ED0@l
    lis      r4, msgColorTable@ha
    fmr      f28, f1
    lbz      r0, 0x4(r27)
    rlwimi   r0, r8, 5, 26, 26
    addi     r10, r4, msgColorTable@l
    fmr      f29, f2
    stb      r0, 0x4(r27)
    lfs      f4, lbl_8065D06C@sda21(r0)
    li       r4, -0x1
    lbz      r5, 0x4(r27)
    rlwimi   r5, r8, 4, 27, 27
    lfs      f3, lbl_8065D058@sda21(r0)
    li       r0, 0x32
    stb      r5, 0x4(r27)
    addi     r5, r3, 0x60b7
    lfs      f0, lbl_8065D05C@sda21(r0)
    mr       r7, r8
    lbz      r3, 0x4(r27)
    rlwimi   r3, r12, 3, 28, 28
    mr       r9, r27
    stb      r3, 0x4(r27)
    lbz      r3, 0x4(r27)
    rlwimi   r3, r12, 2, 29, 29
    stb      r3, 0x4(r27)
    stw      r8, 0x3c78(r24)
    stw      r8, 0x3c7c(r24)
    stw      r8, 0x3c80(r24)
    lwz      r3, 0x0(r23)
    lwz      r12, 0x4(r23)
    stw      r3, 0x10(r1)
    lwz      r3, 0x8(r23)
    stw      r12, 0x14(r1)
    lfs      f1, 0x10(r1)
    stw      r3, 0x18(r1)
    lfs      f2, 0x14(r1)
    stfs     f1, 0x3c84(r24)
    lfs      f1, 0x18(r1)
    stfs     f2, 0x3c88(r24)
    stfs     f1, 0x3c8c(r24)
    stfs     f4, 0x3cd0(r24)
    lfs      f1, 0x0(r11)
    stfs     f1, 0x3cd4(r24)
    lwz      r3, 0x0(r10)
    stw      r3, 0x3cd8(r24)
    stfs     f3, 0x3cdc(r24)
    stfs     f0, 0x3ce4(r24)
    stfs     f0, 0x3ce0(r24)
    stb      r8, 0x3ce8(r24)
    stw      r4, 0x3cec(r24)
    stw      r4, 0x3d0c(r24)
    stfs     f3, 0x3d10(r24)
    stfs     f0, 0x3d14(r24)
    stfs     f3, 0x3d18(r24)
    stw      r8, 0x3c6c(r24)
    stfs     f3, 0x3c70(r24)
    stfs     f3, 0x3c74(r24)
    mtctr    r0
.L_80085C98:
    mulhw    r10, r5, r7
    addi     r0, r8, 0x1
    addi     r4, r8, 0x2
    addi     r3, r8, 0x3
    addi     r11, r8, 0x6
    addi     r26, r8, 0x4
    mulli    r0, r0, 0x4b
    add      r10, r10, r7
    addi     r12, r8, 0x5
    srawi    r25, r10, 8
    mulhw    r24, r5, r0
    addi     r10, r8, 0x7
    srwi     r23, r25, 31
    addi     r8, r8, 0x8
    add      r23, r25, r23
    mulli    r4, r4, 0x4b
    add      r30, r24, r0
    srawi    r30, r30, 8
    srwi     r24, r30, 31
    add      r24, r30, r24
    mulhw    r25, r5, r4
    add      r30, r25, r4
    mulli    r24, r24, 0x168
    srawi    r31, r30, 8
    mulli    r3, r3, 0x4b
    srwi     r25, r31, 31
    subf     r24, r24, r0
    add      r25, r31, r25
    mulli    r23, r23, 0x168
    subf     r23, r23, r7
    addi     r7, r7, 0x258
    mulhw    r30, r5, r3
    sth      r23, 0xd50(r9)
    sth      r23, 0xd4e(r9)
    sth      r24, 0xffc(r9)
    mulli    r0, r26, 0x4b
    add      r30, r30, r3
    sth      r24, 0xffa(r9)
    mulhw    r26, r5, r0
    srawi    r30, r30, 8
    srwi     r31, r30, 31
    add      r31, r30, r31
    mulli    r30, r25, 0x168
    add      r26, r26, r0
    subf     r4, r30, r4
    srawi    r30, r26, 8
    sth      r4, 0x12a8(r9)
    mulli    r26, r12, 0x4b
    srwi     r12, r30, 31
    sth      r4, 0x12a6(r9)
    mulhw    r4, r5, r26
    add      r30, r30, r12
    mulli    r12, r31, 0x168
    add      r4, r4, r26
    subf     r3, r12, r3
    srawi    r4, r4, 8
    sth      r3, 0x1554(r9)
    srwi     r12, r4, 31
    add      r12, r4, r12
    mulli    r4, r30, 0x168
    sth      r3, 0x1552(r9)
    subf     r0, r4, r0
    mulli    r11, r11, 0x4b
    sth      r0, 0x1800(r9)
    sth      r0, 0x17fe(r9)
    mulhw    r3, r5, r11
    mulli    r4, r10, 0x4b
    add      r3, r3, r11
    srawi    r3, r3, 8
    srwi     r10, r3, 31
    mulhw    r0, r5, r4
    add      r3, r3, r10
    mulli    r10, r12, 0x168
    add      r0, r0, r4
    subf     r12, r10, r26
    srawi    r0, r0, 8
    mulli    r10, r3, 0x168
    sth      r12, 0x1aac(r9)
    srwi     r3, r0, 31
    sth      r12, 0x1aaa(r9)
    subf     r10, r10, r11
    add      r0, r0, r3
    sth      r10, 0x1d58(r9)
    mulli    r0, r0, 0x168
    sth      r10, 0x1d56(r9)
    subf     r0, r0, r4
    sth      r0, 0x2004(r9)
    sth      r0, 0x2002(r9)
    addi     r9, r9, 0x1560
    bdnz     .L_80085C98
    lbz      r0, 0x4(r27)
    li       r8, 0x1
    rlwimi   r0, r8, 7, 24, 24
    lis      r3, lbl_8025C400@ha
    stb      r0, 0x4(r27)
    addi     r31, r3, lbl_8025C400@l
    addi     r30, r31, 0x3d6c
    li       r7, 0x0
    lbz      r0, 0x4(r27)
    rlwimi   r0, r8, 6, 25, 25
    mr       r3, r30
    li       r5, 0x0
    stb      r0, 0x4(r27)
    lbz      r0, 0x4(r27)
    rlwimi   r0, r7, 5, 26, 26
    stb      r0, 0x4(r27)
    lbz      r0, 0x4(r27)
    rlwimi   r0, r7, 4, 27, 27
    stb      r0, 0x4(r27)
    lbz      r0, 0x4(r27)
    rlwimi   r0, r6, 3, 28, 28
    stb      r0, 0x4(r27)
    lbz      r0, 0x4(r27)
    rlwimi   r0, r8, 2, 29, 29
    stb      r0, 0x4(r27)
    lwz      r4, 0x3d4c(r31)
    lwz      r6, 0x0(r27)
    lwz      r4, 0x8(r4)
    lfs      f31, 0x880(r6)
    lfs      f30, 0x888(r6)
    addi     r4, r4, 0x450
    bl       Init__7CTexObjFP13TPLDescriptorUl
    lwz      r7, 0x0(r3)
    li       r0, 0x18
    lwz      r6, 0x4(r3)
    addi     r5, r27, 0x40
    addi     r4, r3, 0x38
    stw      r7, 0x8(r27)
    stw      r6, 0xc(r27)
    lwz      r7, 0x8(r3)
    lwz      r6, 0xc(r3)
    stw      r7, 0x10(r27)
    stw      r6, 0x14(r27)
    lwz      r7, 0x10(r3)
    lwz      r6, 0x14(r3)
    stw      r7, 0x18(r27)
    stw      r6, 0x1c(r27)
    lwz      r7, 0x18(r3)
    lwz      r6, 0x1c(r3)
    stw      r7, 0x20(r27)
    stw      r6, 0x24(r27)
    lwz      r6, 0x20(r3)
    stw      r6, 0x28(r27)
    lwz      r6, 0x24(r3)
    stw      r6, 0x2c(r27)
    lwz      r7, 0x28(r3)
    lwz      r6, 0x2c(r3)
    stw      r7, 0x30(r27)
    stw      r6, 0x34(r27)
    lwz      r6, 0x30(r3)
    stw      r6, 0x38(r27)
    lwz      r6, 0x34(r3)
    stw      r6, 0x3c(r27)
    lwz      r3, 0x38(r3)
    stw      r3, 0x40(r27)
    mtctr    r0
.L_80085F08:
    lwz      r3, 0x4(r4)
    lwzu     r0, 0x8(r4)
    stw      r3, 0x4(r5)
    stwu     r0, 0x8(r5)
    bdnz     .L_80085F08
    lfs      f3, lbl_8065D058@sda21(r0)
    addi     r3, r27, 0x44
    stfs     f3, 0x10c(r27)
    stfs     f3, 0x108(r27)
    lfs      f1, 0x108(r27)
    lfs      f2, 0x10c(r27)
    bl       PSMTXTrans
    li       r0, 0x1
    addi     r3, r27, 0x8
    stw      r0, 0x40(r27)
    bl       GXGetTexObjWidth
    clrlwi   r3, r3, 16
    lis      r0, 0x4330
    stw      r3, 0x24(r1)
    addi     r3, r27, 0x8
    lfd      f1, lbl_8065D048@sda21(r0)
    stw      r0, 0x20(r1)
    lfd      f0, 0x20(r1)
    fsubs    f0, f0, f1
    fdivs    f0, f31, f0
    stfs     f0, 0x110(r27)
    bl       GXGetTexObjHeight
    clrlwi   r3, r3, 16
    lis      r0, 0x4330
    stw      r3, 0x2c(r1)
    addi     r3, r27, 0x74
    lfd      f1, lbl_8065D048@sda21(r0)
    stw      r0, 0x28(r1)
    lfs      f3, lbl_8065D05C@sda21(r0)
    lfd      f0, 0x28(r1)
    fsubs    f0, f0, f1
    fdivs    f0, f30, f0
    stfs     f0, 0x114(r27)
    lfs      f1, 0x110(r27)
    lfs      f2, 0x114(r27)
    bl       PSMTXScale
    li       r0, 0x1
    lis      r3, msgColorTable@ha
    stw      r0, 0x40(r27)
    mr       r23, r27
    lfs      f27, lbl_8065D058@sda21(r0)
    addi     r26, r3, msgColorTable@l
    stw      r29, 0xaa8(r27)
    li       r24, 0x0
    li       r29, 0x0
.L_80085FD0:
    stfs     f27, 0x8(r1)
    addi     r3, r23, 0xbac
    lfs      f4, lbl_8065D05C@sda21(r0)
    fmr      f1, f27
    lwz      r12, 0xd10(r23)
    fmr      f2, f27
    fmr      f3, f27
    lwz      r12, 0xc(r12)
    fmr      f5, f4
    fmr      f6, f4
    fmr      f7, f27
    fmr      f8, f27
    mtctr    r12
    bctrl
    addi     r0, r23, 0xaac
    addi     r24, r24, 0x1
    stw      r0, 0xd14(r23)
    cmpwi    r24, 0x190
    lwz      r0, 0x0(r26)
    stw      r0, 0xd34(r23)
    stw      r29, 0xd38(r23)
    addi     r23, r23, 0x2ac
    blt      .L_80085FD0
    addis    r3, r27, 0x4
    stw      r28, 0x3c80(r3)
    lbz      r0, 0x4(r27)
    extrwi.  r0, r0, 1, 28
    beq      .L_80086058
    lwz      r0, 0x3c7c(r3)
    lwz      r3, 0x0(r27)
    slwi     r0, r0, 2
    add      r3, r3, r0
    lfs      f1, 0x830(r3)
    b        .L_8008605C
.L_80086058:
    fmr      f1, f31
.L_8008605C:
    lfs      f3, lbl_8065D068@sda21(r0)
    addis    r6, r27, 0x4
    lfs      f0, lbl_8065D058@sda21(r0)
    mr       r3, r30
    fmuls    f2, f1, f3
    lfs      f1, lbl_8065D06C@sda21(r0)
    li       r5, 0x0
    fneg     f2, f2
    stfs     f2, 0x3c90(r6)
    stfs     f28, 0x3c94(r6)
    stfs     f31, 0x3c98(r6)
    stfs     f0, 0x3c9c(r6)
    stfs     f29, 0x3ca0(r6)
    stfs     f30, 0x3ca4(r6)
    lwz      r4, 0x0(r27)
    lwz      r0, 0x3c7c(r6)
    lfs      f2, 0x858(r4)
    slwi     r0, r0, 2
    lfs      f0, 0x3c9c(r6)
    fsubs    f1, f2, f1
    add      r4, r6, r0
    fmadds   f0, f1, f3, f0
    stfs     f0, 0x3ca8(r4)
    lwz      r4, 0x3d4c(r31)
    lwz      r4, 0x8(r4)
    addi     r4, r4, 0x398
    bl       Init__7CTexObjFP13TPLDescriptorUl
    lwz      r7, 0x0(r3)
    addis    r8, r27, 0x4
    lwz      r6, 0x4(r3)
    li       r0, 0x18
    addi     r5, r8, 0x37a4
    addi     r4, r3, 0x38
    stw      r7, 0x376c(r8)
    stw      r6, 0x3770(r8)
    lwz      r7, 0x8(r3)
    lwz      r6, 0xc(r3)
    stw      r7, 0x3774(r8)
    stw      r6, 0x3778(r8)
    lwz      r7, 0x10(r3)
    lwz      r6, 0x14(r3)
    stw      r7, 0x377c(r8)
    stw      r6, 0x3780(r8)
    lwz      r7, 0x18(r3)
    lwz      r6, 0x1c(r3)
    stw      r7, 0x3784(r8)
    stw      r6, 0x3788(r8)
    lwz      r6, 0x20(r3)
    stw      r6, 0x378c(r8)
    lwz      r6, 0x24(r3)
    stw      r6, 0x3790(r8)
    lwz      r7, 0x28(r3)
    lwz      r6, 0x2c(r3)
    stw      r7, 0x3794(r8)
    stw      r6, 0x3798(r8)
    lwz      r6, 0x30(r3)
    stw      r6, 0x379c(r8)
    lwz      r6, 0x34(r3)
    stw      r6, 0x37a0(r8)
    lwz      r3, 0x38(r3)
    stw      r3, 0x37a4(r8)
    mtctr    r0
.L_80086154:
    lwz      r3, 0x4(r4)
    lwzu     r0, 0x8(r4)
    stw      r3, 0x4(r5)
    stwu     r0, 0x8(r5)
    bdnz     .L_80086154
    lwz      r4, 0x3d4c(r31)
    mr       r3, r30
    li       r5, 0x0
    lwz      r4, 0x8(r4)
    addi     r4, r4, 0x3a0
    bl       Init__7CTexObjFP13TPLDescriptorUl
    lwz      r7, 0x0(r3)
    addis    r8, r27, 0x4
    lwz      r6, 0x4(r3)
    li       r0, 0x18
    addi     r5, r8, 0x38a4
    addi     r4, r3, 0x38
    stw      r7, 0x386c(r8)
    stw      r6, 0x3870(r8)
    lwz      r7, 0x8(r3)
    lwz      r6, 0xc(r3)
    stw      r7, 0x3874(r8)
    stw      r6, 0x3878(r8)
    lwz      r7, 0x10(r3)
    lwz      r6, 0x14(r3)
    stw      r7, 0x387c(r8)
    stw      r6, 0x3880(r8)
    lwz      r7, 0x18(r3)
    lwz      r6, 0x1c(r3)
    stw      r7, 0x3884(r8)
    stw      r6, 0x3888(r8)
    lwz      r6, 0x20(r3)
    stw      r6, 0x388c(r8)
    lwz      r6, 0x24(r3)
    stw      r6, 0x3890(r8)
    lwz      r7, 0x28(r3)
    lwz      r6, 0x2c(r3)
    stw      r7, 0x3894(r8)
    stw      r6, 0x3898(r8)
    lwz      r6, 0x30(r3)
    stw      r6, 0x389c(r8)
    lwz      r6, 0x34(r3)
    stw      r6, 0x38a0(r8)
    lwz      r3, 0x38(r3)
    stw      r3, 0x38a4(r8)
    mtctr    r0
.L_8008620C:
    lwz      r3, 0x4(r4)
    lwzu     r0, 0x8(r4)
    stw      r3, 0x4(r5)
    stwu     r0, 0x8(r5)
    bdnz     .L_8008620C
    lwz      r4, 0x3d4c(r31)
    mr       r3, r30
    li       r5, 0x0
    lwz      r4, 0x8(r4)
    addi     r4, r4, 0x3b8
    bl       Init__7CTexObjFP13TPLDescriptorUl
    lwz      r7, 0x0(r3)
    addis    r8, r27, 0x4
    lwz      r6, 0x4(r3)
    li       r0, 0x18
    addi     r5, r8, 0x39a4
    addi     r4, r3, 0x38
    stw      r7, 0x396c(r8)
    stw      r6, 0x3970(r8)
    lwz      r7, 0x8(r3)
    lwz      r6, 0xc(r3)
    stw      r7, 0x3974(r8)
    stw      r6, 0x3978(r8)
    lwz      r7, 0x10(r3)
    lwz      r6, 0x14(r3)
    stw      r7, 0x397c(r8)
    stw      r6, 0x3980(r8)
    lwz      r7, 0x18(r3)
    lwz      r6, 0x1c(r3)
    stw      r7, 0x3984(r8)
    stw      r6, 0x3988(r8)
    lwz      r6, 0x20(r3)
    stw      r6, 0x398c(r8)
    lwz      r6, 0x24(r3)
    stw      r6, 0x3990(r8)
    lwz      r7, 0x28(r3)
    lwz      r6, 0x2c(r3)
    stw      r7, 0x3994(r8)
    stw      r6, 0x3998(r8)
    lwz      r6, 0x30(r3)
    stw      r6, 0x399c(r8)
    lwz      r6, 0x34(r3)
    stw      r6, 0x39a0(r8)
    lwz      r3, 0x38(r3)
    stw      r3, 0x39a4(r8)
    mtctr    r0
.L_800862C4:
    lwz      r3, 0x4(r4)
    lwzu     r0, 0x8(r4)
    stw      r3, 0x4(r5)
    stwu     r0, 0x8(r5)
    bdnz     .L_800862C4
    lwz      r4, 0x3d4c(r31)
    mr       r3, r30
    li       r5, 0x0
    lwz      r4, 0x8(r4)
    addi     r4, r4, 0x3a8
    bl       Init__7CTexObjFP13TPLDescriptorUl
    lwz      r7, 0x0(r3)
    addis    r8, r27, 0x4
    lwz      r6, 0x4(r3)
    li       r0, 0x18
    addi     r5, r8, 0x3aa4
    addi     r4, r3, 0x38
    stw      r7, 0x3a6c(r8)
    stw      r6, 0x3a70(r8)
    lwz      r7, 0x8(r3)
    lwz      r6, 0xc(r3)
    stw      r7, 0x3a74(r8)
    stw      r6, 0x3a78(r8)
    lwz      r7, 0x10(r3)
    lwz      r6, 0x14(r3)
    stw      r7, 0x3a7c(r8)
    stw      r6, 0x3a80(r8)
    lwz      r7, 0x18(r3)
    lwz      r6, 0x1c(r3)
    stw      r7, 0x3a84(r8)
    stw      r6, 0x3a88(r8)
    lwz      r6, 0x20(r3)
    stw      r6, 0x3a8c(r8)
    lwz      r6, 0x24(r3)
    stw      r6, 0x3a90(r8)
    lwz      r7, 0x28(r3)
    lwz      r6, 0x2c(r3)
    stw      r7, 0x3a94(r8)
    stw      r6, 0x3a98(r8)
    lwz      r6, 0x30(r3)
    stw      r6, 0x3a9c(r8)
    lwz      r6, 0x34(r3)
    stw      r6, 0x3aa0(r8)
    lwz      r3, 0x38(r3)
    stw      r3, 0x3aa4(r8)
    mtctr    r0
.L_8008637C:
    lwz      r3, 0x4(r4)
    lwzu     r0, 0x8(r4)
    stw      r3, 0x4(r5)
    stwu     r0, 0x8(r5)
    bdnz     .L_8008637C
    lwz      r4, 0x3d4c(r31)
    mr       r3, r30
    li       r5, 0x0
    lwz      r4, 0x8(r4)
    addi     r4, r4, 0x3b0
    bl       Init__7CTexObjFP13TPLDescriptorUl
    lwz      r7, 0x0(r3)
    addis    r8, r27, 0x4
    lwz      r6, 0x4(r3)
    li       r0, 0x18
    addi     r5, r8, 0x3ba4
    addi     r4, r3, 0x38
    stw      r7, 0x3b6c(r8)
    stw      r6, 0x3b70(r8)
    lwz      r7, 0x8(r3)
    lwz      r6, 0xc(r3)
    stw      r7, 0x3b74(r8)
    stw      r6, 0x3b78(r8)
    lwz      r7, 0x10(r3)
    lwz      r6, 0x14(r3)
    stw      r7, 0x3b7c(r8)
    stw      r6, 0x3b80(r8)
    lwz      r7, 0x18(r3)
    lwz      r6, 0x1c(r3)
    stw      r7, 0x3b84(r8)
    stw      r6, 0x3b88(r8)
    lwz      r6, 0x20(r3)
    stw      r6, 0x3b8c(r8)
    lwz      r6, 0x24(r3)
    stw      r6, 0x3b90(r8)
    lwz      r7, 0x28(r3)
    lwz      r6, 0x2c(r3)
    stw      r7, 0x3b94(r8)
    stw      r6, 0x3b98(r8)
    lwz      r6, 0x30(r3)
    stw      r6, 0x3b9c(r8)
    lwz      r6, 0x34(r3)
    stw      r6, 0x3ba0(r8)
    lwz      r3, 0x38(r3)
    stw      r3, 0x3ba4(r8)
    mtctr    r0
.L_80086434:
    lwz      r3, 0x4(r4)
    lwzu     r0, 0x8(r4)
    stw      r3, 0x4(r5)
    stwu     r0, 0x8(r5)
    bdnz     .L_80086434
    psq_l    f31, 0xa8(r1), 0, qr0
    lfd      f31, 0xa0(r1)
    psq_l    f30, 0x98(r1), 0, qr0
    lfd      f30, 0x90(r1)
    psq_l    f29, 0x88(r1), 0, qr0
    lfd      f29, 0x80(r1)
    psq_l    f28, 0x78(r1), 0, qr0
    lfd      f28, 0x70(r1)
    psq_l    f27, 0x68(r1), 0, qr0
    lfd      f27, 0x60(r1)
    lmw      r23, 0x3c(r1)
    lwz      r0, 0xb4(r1)
    mtlr     r0
    addi     r1, r1, 0xb0
    blr
*/

/**
 * @note Address: 0x80086484
 * @note Size: 0x528
 */
// CMsgData_Measure
/*
    stwu     r1, -0x140(r1)
    mflr     r0
    stw      r0, 0x144(r1)
    stfd     f31, 0x130(r1)
    psq_st   f31, 0x138(r1), 0, qr0
    stmw     r26, 0x118(r1)
    mr       r26, r3
    li       r5, 0x0
    stw      r5, 0x804(r3)
    lis      r3, lbl_8025C400@ha
    addi     r31, r3, lbl_8025C400@l
    lfs      f0, lbl_8065D058@sda21(r0)
    lwz      r4, 0x804(r26)
    li       r3, -0x1
    li       r0, 0x1
    lfs      f31, lbl_8065D06C@sda21(r0)
    slwi     r4, r4, 2
    mr       r29, r26
    add      r4, r26, r4
    addi     r30, r31, 0x3d6c
    stw      r5, 0x808(r4)
    li       r28, 0x1
    li       r27, 0x0
    lwz      r4, 0x804(r26)
    slwi     r4, r4, 2
    add      r4, r26, r4
    stfs     f0, 0x830(r4)
    lwz      r4, 0x804(r26)
    slwi     r4, r4, 2
    add      r4, r26, r4
    stfs     f0, 0x858(r4)
    stw      r3, 0x88c(r26)
    stw      r0, 0x890(r26)
    b        .L_80086920
.L_8008650C:
    lbz      r4, 0x0(r29)
    cmplwi   r4, 0x18
    bgt      .L_80086640
    lis      r3, jumptable_80225594@ha
    slwi     r0, r4, 2
    addi     r3, r3, jumptable_80225594@l
    lwzx     r0, r3, r0
    mtctr    r0
    bctr
.L_80086530:
    lwz      r0, 0x804(r26)
    lfs      f1, lbl_8065D058@sda21(r0)
    slwi     r3, r0, 2
    addi     r0, r3, 0x858
    lfsx     f0, r26, r0
    fcmpu    cr0, f1, f0
    bne      .L_80086550
    stfsx    f31, r26, r0
.L_80086550:
    lwz      r3, 0x804(r26)
    li       r4, 0x0
    lfs      f0, lbl_8065D058@sda21(r0)
    addi     r0, r3, 0x1
    stw      r0, 0x804(r26)
    lwz      r0, 0x804(r26)
    slwi     r3, r0, 2
    addi     r0, r3, 0x808
    stwx     r4, r26, r0
    lwz      r0, 0x804(r26)
    slwi     r3, r0, 2
    addi     r0, r3, 0x830
    stfsx    f0, r26, r0
    lwz      r0, 0x804(r26)
    slwi     r3, r0, 2
    addi     r0, r3, 0x858
    stfsx    f31, r26, r0
    b        .L_8008683C
.L_80086598:
    lwz      r0, 0x804(r26)
    lfs      f1, lbl_8065D058@sda21(r0)
    slwi     r3, r0, 2
    addi     r0, r3, 0x858
    lfsx     f0, r26, r0
    fcmpu    cr0, f1, f0
    bne      .L_800865BC
    lfs      f0, lbl_8065D06C@sda21(r0)
    stfsx    f0, r26, r0
.L_800865BC:
    lwz      r3, 0x804(r26)
    li       r27, 0x1
    addi     r0, r3, 0x1
    stw      r0, 0x804(r26)
    b        .L_8008683C
.L_800865D0:
    lbz      r3, 0x1(r29)
    lis      r0, 0x4330
    stw      r0, 0x110(r1)
    xoris    r0, r3, 0x8000
    lfd      f2, lbl_8065D050@sda21(r0)
    stw      r0, 0x114(r1)
    lfs      f0, lbl_8065D0DC@sda21(r0)
    lfd      f1, 0x110(r1)
    fsubs    f31, f1, f2
    fcmpu    cr0, f0, f31
    bne      .L_80086600
    lfs      f31, lbl_8065D06C@sda21(r0)
.L_80086600:
    lwz      r0, 0x804(r26)
    slwi     r3, r0, 2
    addi     r0, r3, 0x858
    lfsx     f0, r26, r0
    fcmpo    cr0, f31, f0
    ble      .L_8008683C
    stfsx    f31, r26, r0
    b        .L_8008683C
.L_80086620:
    lbz      r28, 0x1(r29)
    b        .L_8008683C
.L_80086628:
    lbz      r0, 0x1(r29)
    stw      r0, 0x890(r26)
    b        .L_8008683C
.L_80086634:
    lbz      r0, 0x1(r29)
    stw      r0, 0x88c(r26)
    b        .L_8008683C
.L_80086640:
    lbz      r0, 0x3318(r31)
    cmplwi   r0, 0x6
    bne      .L_800866E8
    cmplwi   r4, 0x81
    li       r0, 0x0
    blt      .L_80086660
    cmplwi   r4, 0x9f
    ble      .L_80086670
.L_80086660:
    cmplwi   r4, 0xe0
    blt      .L_80086674
    cmplwi   r4, 0xfc
    bgt      .L_80086674
.L_80086670:
    li       r0, 0x1
.L_80086674:
    clrlwi.  r0, r0, 24
    beq      .L_800866E8
    lwz      r0, 0x804(r26)
    lis      r3, lbl_8025C400@ha
    fmr      f1, f31
    addi     r3, r3, lbl_8025C400@l
    slwi     r4, r0, 2
    mr       r5, r28
    addi     r7, r4, 0x808
    lwzx     r6, r26, r7
    addi     r4, r1, 0xc
    addi     r0, r6, 0x1
    stwx     r0, r26, r7
    lhz      r6, lbl_8065FFB0@sda21(r0)
    lbz      r0, lbl_8065FFB2@sda21(r0)
    sth      r6, 0xc(r1)
    stb      r0, 0xe(r1)
    lbz      r0, 0x0(r29)
    stb      r0, 0xc(r1)
    lbz      r0, 0x1(r29)
    stb      r0, 0xd(r1)
    bl       fn_801968BC
    lwz      r0, 0x804(r26)
    slwi     r3, r0, 2
    addi     r0, r3, 0x830
    lfsx     f0, r26, r0
    fadds    f0, f0, f1
    stfsx    f0, r26, r0
    b        .L_8008683C
.L_800866E8:
    cmplwi   r4, 0x11
    bne      .L_800867E4
    lwz      r4, 0x3d4c(r31)
    mr       r3, r30
    li       r5, 0x0
    lwz      r4, 0x8(r4)
    addi     r4, r4, 0x3b8
    bl       Init__7CTexObjFP13TPLDescriptorUl
    lwz      r8, 0x0(r3)
    lis      r4, __vt__7CTexObj@ha
    lwz      r7, 0x4(r3)
    addi     r6, r4, __vt__7CTexObj@l
    li       r0, 0x18
    addi     r5, r1, 0x48
    stw      r8, 0x10(r1)
    addi     r4, r3, 0x38
    stw      r7, 0x14(r1)
    lwz      r8, 0x8(r3)
    lwz      r7, 0xc(r3)
    stw      r8, 0x18(r1)
    stw      r7, 0x1c(r1)
    lwz      r8, 0x10(r3)
    lwz      r7, 0x14(r3)
    stw      r8, 0x20(r1)
    stw      r7, 0x24(r1)
    lwz      r8, 0x18(r3)
    lwz      r7, 0x1c(r3)
    stw      r8, 0x28(r1)
    stw      r7, 0x2c(r1)
    stw      r6, 0x10c(r1)
    lwz      r6, 0x20(r3)
    stw      r6, 0x30(r1)
    lwz      r6, 0x24(r3)
    stw      r6, 0x34(r1)
    lwz      r7, 0x28(r3)
    lwz      r6, 0x2c(r3)
    stw      r7, 0x38(r1)
    stw      r6, 0x3c(r1)
    lwz      r6, 0x30(r3)
    stw      r6, 0x40(r1)
    lwz      r6, 0x34(r3)
    stw      r6, 0x44(r1)
    lwz      r3, 0x38(r3)
    stw      r3, 0x48(r1)
    mtctr    r0
.L_8008679C:
    lwz      r3, 0x4(r4)
    lwzu     r0, 0x8(r4)
    stw      r3, 0x4(r5)
    stwu     r0, 0x8(r5)
    bdnz     .L_8008679C
    lfs      f0, lbl_8065D06C@sda21(r0)
    lis      r3, __vt__7CTexObj@ha
    lwz      r4, 0x804(r26)
    addi     r0, r3, __vt__7CTexObj@l
    fdivs    f1, f31, f0
    lfs      f2, lbl_8065D0F0@sda21(r0)
    slwi     r3, r4, 2
    addi     r3, r3, 0x830
    lfsx     f0, r26, r3
    fmadds   f0, f2, f1, f0
    stfsx    f0, r26, r3
    stw      r0, 0x10c(r1)
    b        .L_8008683C
.L_800867E4:
    lwz      r0, 0x804(r26)
    lis      r3, lbl_8025C400@ha
    fmr      f1, f31
    addi     r3, r3, lbl_8025C400@l
    slwi     r4, r0, 2
    mr       r5, r28
    addi     r7, r4, 0x808
    lwzx     r6, r26, r7
    addi     r4, r1, 0x8
    addi     r0, r6, 0x1
    stwx     r0, r26, r7
    lhz      r0, lbl_8065FFB4@sda21(r0)
    sth      r0, 0x8(r1)
    lbz      r0, 0x0(r29)
    stb      r0, 0x8(r1)
    bl       fn_801968BC
    lwz      r0, 0x804(r26)
    slwi     r3, r0, 2
    addi     r0, r3, 0x830
    lfsx     f0, r26, r0
    fadds    f0, f0, f1
    stfsx    f0, r26, r0
.L_8008683C:
    lbz      r4, 0x0(r29)
    cmplwi   r4, 0x18
    bgt      .L_800868D4
    lis      r3, jumptable_80225530@ha
    slwi     r0, r4, 2
    addi     r3, r3, jumptable_80225530@l
    lwzx     r0, r3, r0
    mtctr    r0
    bctr
.L_80086860:
    li       r0, 0x1
    b        .L_8008691C
.L_80086868:
    li       r0, 0x2
    b        .L_8008691C
.L_80086870:
    li       r0, 0x6
    b        .L_8008691C
.L_80086878:
    li       r0, 0x8
    b        .L_8008691C
.L_80086880:
    lbz      r0, 0x5(r29)
    cmplwi   r0, 0x2
    beq      .L_80086894
    cmplwi   r0, 0x3
    bne      .L_8008689C
.L_80086894:
    li       r0, 0x8
    b        .L_8008691C
.L_8008689C:
    cmplwi   r0, 0x4
    bne      .L_800868AC
    li       r0, 0x9
    b        .L_8008691C
.L_800868AC:
    li       r0, 0x6
    b        .L_8008691C
.L_800868B4:
    li       r0, 0x2
    b        .L_8008691C
.L_800868BC:
    li       r0, 0x3
    b        .L_8008691C
.L_800868C4:
    li       r0, 0x4
    b        .L_8008691C
.L_800868CC:
    li       r0, 0x5
    b        .L_8008691C
.L_800868D4:
    lbz      r0, 0x3318(r31)
    cmplwi   r0, 0x6
    bne      .L_80086918
    cmplwi   r4, 0x81
    li       r0, 0x0
    blt      .L_800868F4
    cmplwi   r4, 0x9f
    ble      .L_80086904
.L_800868F4:
    cmplwi   r4, 0xe0
    blt      .L_80086908
    cmplwi   r4, 0xfc
    bgt      .L_80086908
.L_80086904:
    li       r0, 0x1
.L_80086908:
    clrlwi.  r0, r0, 24
    beq      .L_80086918
    li       r0, 0x2
    b        .L_8008691C
.L_80086918:
    li       r0, 0x1
.L_8008691C:
    add      r29, r29, r0
.L_80086920:
    cmpwi    r27, 0x0
    beq      .L_8008650C
    lfs      f0, lbl_8065D058@sda21(r0)
    mr       r3, r26
    li       r4, 0x0
    stfs     f0, 0x880(r26)
    stfs     f0, 0x884(r26)
    stfs     f0, 0x888(r26)
    b        .L_80086984
.L_80086944:
    lfs      f0, 0x880(r26)
    lfs      f1, 0x830(r3)
    fcmpo    cr0, f0, f1
    bge      .L_80086958
    stfs     f1, 0x880(r26)
.L_80086958:
    lfs      f0, 0x884(r26)
    lfs      f1, 0x858(r3)
    fcmpo    cr0, f0, f1
    bge      .L_8008696C
    stfs     f1, 0x884(r26)
.L_8008696C:
    lfs      f0, 0x858(r3)
    addi     r3, r3, 0x4
    lfs      f1, 0x888(r26)
    addi     r4, r4, 0x1
    fadds    f0, f1, f0
    stfs     f0, 0x888(r26)
.L_80086984:
    lwz      r0, 0x804(r26)
    cmpw     r4, r0
    blt      .L_80086944
    psq_l    f31, 0x138(r1), 0, qr0
    lfd      f31, 0x130(r1)
    lmw      r26, 0x118(r1)
    lwz      r0, 0x144(r1)
    mtlr     r0
    addi     r1, r1, 0x140
    blr
*/
