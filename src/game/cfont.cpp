/**
 * @note Address: 0x8019E8F4
 * @note Size: 0x34C
 */
// fn_8019E8F4
/*
    stwu     r1, -0x40(r1)
    mflr     r0
    stw      r0, 0x44(r1)
    stmw     r27, 0x2c(r1)
    mr       r27, r3
    mr       r28, r4
    mr       r29, r5
    lwz      r0, 0x24(r3)
    cmplwi   r0, 0x0
    bne      .L_8019E924
    li       r4, 0x0
    bl       fn_8019EC40
.L_8019E924:
    lhz      r0, lbl_8065B860@sda21(r0)
    li       r3, 0x0
    cmplwi   r0, 0x1
    bne      .L_8019E944
    lwz      r0, lbl_8065B868@sda21(r0)
    cmpwi    r0, 0x0
    bne      .L_8019E944
    li       r3, 0x1
.L_8019E944:
    clrlwi.  r0, r3, 24
    beq      .L_8019EB28
    lbz      r4, 0x0(r28)
    li       r3, 0x0
    clrlwi   r0, r4, 24
    mr       r30, r4
    cmplwi   r0, 0x81
    blt      .L_8019E96C
    cmplwi   r0, 0x9f
    ble      .L_8019E980
.L_8019E96C:
    clrlwi   r0, r30, 24
    cmplwi   r0, 0xe0
    blt      .L_8019E984
    cmplwi   r0, 0xfc
    bgt      .L_8019E984
.L_8019E980:
    li       r3, 0x1
.L_8019E984:
    clrlwi.  r0, r3, 24
    beq      .L_8019E998
    lbz      r0, 0x1(r28)
    rlwimi   r0, r30, 8, 8, 23
    clrlwi   r30, r0, 16
.L_8019E998:
    li       r0, 0x0
    lis      r3, lbl_802586E0@ha
    addi     r4, r3, lbl_802586E0@l
    stw      r0, 0x14(r1)
    mr       r3, r4
    clrlwi   r5, r30, 16
    li       r6, 0x0
    b        .L_8019EA04
.L_8019E9B8:
    lhz      r0, 0x4(r3)
    cmplw    r5, r0
    blt      .L_8019E9FC
    lhz      r0, 0x6(r3)
    cmplw    r5, r0
    bge      .L_8019E9FC
    slwi     r0, r6, 3
    add      r3, r4, r0
    lwzx     r4, r4, r0
    lhz      r0, 0x4(r3)
    subf     r0, r0, r5
    mulli    r0, r0, 0x140
    add      r3, r4, r0
    stw      r3, 0x14(r1)
    lha      r0, 0x122(r3)
    stw      r0, 0x8(r1)
    b        .L_8019EA10
.L_8019E9FC:
    addi     r3, r3, 0x8
    addi     r6, r6, 0x1
.L_8019EA04:
    lwz      r0, 0x0(r3)
    cmplwi   r0, 0x0
    bne      .L_8019E9B8
.L_8019EA10:
    lwz      r0, 0x14(r1)
    cmplwi   r0, 0x0
    bne      .L_8019EB18
    lwz      r6, lbl_8065B86C@sda21(r0)
    li       r0, 0x32
    clrlwi   r3, r30, 16
    li       r5, 0x0
    mr       r4, r6
    li       r7, lbl_8065B86C@sda21
    mtctr    r0
.L_8019EA38:
    lhz      r0, 0x120(r4)
    cmplw    r3, r0
    bne      .L_8019EA60
    mulli    r3, r5, 0x140
    lwz      r0, 0x0(r7)
    add      r3, r0, r3
    stw      r3, 0x14(r1)
    lha      r0, 0x122(r3)
    stw      r0, 0x8(r1)
    b        .L_8019EA6C
.L_8019EA60:
    addi     r4, r4, 0x140
    addi     r5, r5, 0x1
    bdnz     .L_8019EA38
.L_8019EA6C:
    lwz      r0, 0x14(r1)
    cmplwi   r0, 0x0
    bne      .L_8019EB18
    li       r31, lbl_8065B86C@sda21
    li       r4, 0x0
    lwz      r0, 0x4(r31)
    li       r5, 0x140
    mulli    r0, r0, 0x140
    add      r3, r6, r0
    stw      r3, 0x14(r1)
    bl       memset
    lwz      r4, 0x14(r1)
    mr       r3, r28
    addi     r7, r1, 0x8
    li       r5, 0x0
    li       r6, 0x6
    bl       OSGetFontTexel
    lwz      r0, 0x4(r31)
    li       r4, 0x140
    lwz      r3, lbl_8065B86C@sda21(r0)
    mulli    r0, r0, 0x140
    add      r3, r3, r0
    sth      r30, 0x120(r3)
    lwz      r0, 0x4(r31)
    lwz      r3, lbl_8065B86C@sda21(r0)
    mulli    r0, r0, 0x140
    lwz      r5, 0x8(r1)
    add      r3, r3, r0
    sth      r5, 0x122(r3)
    lwz      r0, 0x4(r31)
    lwz      r3, lbl_8065B86C@sda21(r0)
    mulli    r0, r0, 0x140
    add      r3, r3, r0
    bl       DCFlushRange
    lwz      r4, 0x4(r31)
    lis      r3, 0x51ec
    subi     r0, r3, 0x7ae1
    addi     r3, r4, 0x1
    mulhwu   r0, r0, r3
    srwi     r0, r0, 4
    mulli    r0, r0, 0x32
    subf     r0, r0, r3
    stw      r0, 0x4(r31)
.L_8019EB18:
    li       r0, 0x0
    stw      r0, 0xc(r1)
    stw      r0, 0x10(r1)
    b        .L_8019EB88
.L_8019EB28:
    lwz      r0, lbl_8065B868@sda21(r0)
    cmpwi    r0, 0x0
    beq      .L_8019EB70
    lbz      r0, 0x0(r28)
    lis      r3, lbl_80237AD0@ha
    lis      r4, lbl_802386E0@ha
    mulli    r0, r0, 0xc
    addi     r3, r3, lbl_80237AD0@l
    addi     r4, r4, lbl_802386E0@l
    stw      r4, 0x14(r1)
    add      r4, r3, r0
    lwzx     r5, r3, r0
    lwz      r3, 0x4(r4)
    lwz      r0, 0x8(r4)
    stw      r5, 0x10(r1)
    stw      r3, 0xc(r1)
    stw      r0, 0x8(r1)
    b        .L_8019EB88
.L_8019EB70:
    mr       r3, r28
    addi     r4, r1, 0x14
    addi     r5, r1, 0x10
    addi     r6, r1, 0xc
    addi     r7, r1, 0x8
    bl       OSGetFontTexture
.L_8019EB88:
    lwz      r0, 0x14(r1)
    addi     r3, r27, 0x3c
    addi     r4, r27, 0x24
    li       r5, 0x0
    stw      r0, 0x8(r27)
    bl       Init__7CTexObjFP13TPLDescriptorUl
    lwz      r5, 0x10(r1)
    lis      r4, 0x4330
    lwz      r0, 0xc(r1)
    addi     r3, r27, 0x78
    xoris    r5, r5, 0x8000
    stw      r4, 0x18(r1)
    xoris    r0, r0, 0x8000
    lfd      f4, lbl_8065F688@sda21(r0)
    stw      r5, 0x1c(r1)
    lfs      f5, 0x2c(r27)
    lfd      f0, 0x18(r1)
    stw      r0, 0x24(r1)
    fsubs    f1, f0, f4
    lfs      f2, 0x30(r27)
    stw      r4, 0x20(r1)
    lfs      f3, lbl_8065F680@sda21(r0)
    lfd      f0, 0x20(r1)
    fmuls    f1, f5, f1
    fsubs    f0, f0, f4
    fmuls    f2, f2, f0
    bl       PSMTXTrans
    li       r0, 0x1
    lfs      f3, lbl_8065F684@sda21(r0)
    stw      r0, 0x74(r27)
    addi     r3, r27, 0xa8
    lfs      f1, 0x34(r27)
    lfs      f2, 0x38(r27)
    bl       PSMTXScale
    li       r0, 0x1
    cmplwi   r29, 0x0
    stw      r0, 0x74(r27)
    beq      .L_8019EC28
    lwz      r0, 0x8(r1)
    stw      r0, 0x0(r29)
.L_8019EC28:
    addi     r3, r27, 0x3c
    lmw      r27, 0x2c(r1)
    lwz      r0, 0x44(r1)
    mtlr     r0
    addi     r1, r1, 0x40
    blr
*/

/**
 * @note Address: 0x8019EC40
 * @note Size: 0x738
 */
// fn_8019EC40
/*
    stwu     r1, -0xa0(r1)
    mflr     r0
    lis      r5, lbl_8020ECA8@ha
    stw      r0, 0xa4(r1)
    stmw     r24, 0x80(r1)
    mr       r28, r3
    mr       r24, r4
    addi     r31, r5, lbl_8020ECA8@l
    lwz      r0, lbl_8065B864@sda21(r0)
    cmplwi   r0, 0x0
    bne      .L_8019F120
    bl       OSGetFontEncode
    neg      r0, r24
    clrlwi   r4, r3, 16
    or       r0, r0, r24
    sth      r3, lbl_8065B860@sda21(r0)
    srwi     r3, r0, 31
    cmplwi   r4, 0x1
    stw      r3, lbl_8065B868@sda21(r0)
    li       r0, 0x0
    bne      .L_8019ECA0
    cmpwi    r3, 0x0
    bne      .L_8019ECA0
    li       r0, 0x1
.L_8019ECA0:
    clrlwi.  r0, r0, 24
    beq      .L_8019F060
    cmplwi   r4, 0x1
    li       r3, 0x0
    bne      .L_8019ECC4
    lwz      r0, lbl_8065B868@sda21(r0)
    cmpwi    r0, 0x0
    bne      .L_8019ECC4
    li       r3, 0x1
.L_8019ECC4:
    clrlwi.  r0, r3, 24
    lis      r3, 0x1
    addi     r3, r3, 0x120
    beq      .L_8019ECDC
    lis      r3, 0x9
    addi     r3, r3, 0xee4
.L_8019ECDC:
    addi     r0, r3, 0x1f
    addi     r4, r31, 0x60
    clrrwi   r3, r0, 5
    bl       Alloc__2mmFUlPCc
    cmplwi   r3, 0x0
    stw      r3, lbl_8065B864@sda21(r0)
    bne      .L_8019ED20
    addi     r3, r31, 0x6c
    crclr    cr1eq
    bl       OSReport
    addi     r3, r31, 0x7c
    addi     r6, r31, 0x88
    li       r4, 0x47
    li       r5, lbl_8065F690@sda21
    li       r7, lbl_8065AD68@sda21
    crclr    cr1eq
    bl       OSPanic
.L_8019ED20:
    lhz      r0, lbl_8065B860@sda21(r0)
    li       r3, 0x0
    cmplwi   r0, 0x1
    bne      .L_8019ED40
    lwz      r0, lbl_8065B868@sda21(r0)
    cmpwi    r0, 0x0
    bne      .L_8019ED40
    li       r3, 0x1
.L_8019ED40:
    clrlwi.  r0, r3, 24
    li       r3, 0x3000
    beq      .L_8019ED54
    lis      r3, 0x5
    subi     r3, r3, 0x3000
.L_8019ED54:
    addi     r0, r3, 0x1f
    li       r4, lbl_8065F698@sda21
    clrrwi   r3, r0, 5
    bl       Alloc__2mmFUlPCc
    mr.      r25, r3
    bne      .L_8019ED94
    addi     r3, r31, 0x6c
    crclr    cr1eq
    bl       OSReport
    addi     r3, r31, 0x7c
    addi     r6, r31, 0xa8
    li       r4, 0x4d
    li       r5, lbl_8065F690@sda21
    li       r7, lbl_8065AD68@sda21
    crclr    cr1eq
    bl       OSPanic
.L_8019ED94:
    lwz      r3, lbl_8065B864@sda21(r0)
    mr       r4, r25
    bl       OSLoadFont
    cmplwi   r3, 0x0
    bne      .L_8019EDD0
    addi     r3, r31, 0x6c
    crclr    cr1eq
    bl       OSReport
    addi     r3, r31, 0x7c
    addi     r6, r31, 0xd0
    li       r4, 0x51
    li       r5, lbl_8065F690@sda21
    li       r7, lbl_8065AD68@sda21
    crclr    cr1eq
    bl       OSPanic
.L_8019EDD0:
    mr       r3, r25
    li       r4, 0x1
    bl       Free__2mmFPvl
    lis      r3, lbl_802586E0@ha
    addi     r29, r1, 0x9
    addi     r30, r3, lbl_802586E0@l
    b        .L_8019EEF8
.L_8019EDEC:
    addi     r3, r31, 0xf0
    crclr    cr1eq
    bl       fn_801911B4
    lhz      r5, 0x4(r30)
    mr       r4, r3
    lhz      r0, 0x6(r30)
    subf     r0, r5, r0
    mulli    r3, r0, 0x140
    bl       Alloc__2mmFUlPCc
    stw      r3, 0x0(r30)
    lwz      r0, 0x0(r30)
    cmplwi   r0, 0x0
    beq      .L_8019EEB8
    lhz      r3, 0x4(r30)
    mr       r26, r0
    lhz      r0, 0x6(r30)
    li       r24, 0x0
    sth      r3, 0x8(r1)
    subf     r27, r3, r0
    b        .L_8019EEAC
.L_8019EE3C:
    lhz      r0, 0x8(r1)
    rlwinm.  r0, r0, 0, 16, 23
    beq      .L_8019EE50
    addi     r25, r1, 0x8
    b        .L_8019EE54
.L_8019EE50:
    mr       r25, r29
.L_8019EE54:
    mr       r3, r26
    li       r4, 0x0
    li       r5, 0x140
    bl       memset
    mr       r3, r25
    mr       r4, r26
    addi     r7, r1, 0xc
    li       r5, 0x0
    li       r6, 0x6
    bl       OSGetFontTexel
    lwz      r0, 0xc(r1)
    mr       r3, r26
    li       r4, 0x140
    sth      r0, 0x122(r26)
    lhz      r0, 0x8(r1)
    sth      r0, 0x120(r26)
    bl       DCFlushRange
    lhz      r3, 0x8(r1)
    addi     r24, r24, 0x1
    addi     r26, r26, 0x140
    addi     r0, r3, 0x1
    sth      r0, 0x8(r1)
.L_8019EEAC:
    cmpw     r24, r27
    blt      .L_8019EE3C
    b        .L_8019EEF4
.L_8019EEB8:
    addi     r3, r31, 0x6c
    crclr    cr1eq
    bl       OSReport
    lhz      r4, 0x4(r30)
    addi     r3, r31, 0xfc
    lhz      r5, 0x6(r30)
    crclr    cr1eq
    bl       fn_801911B4
    mr       r6, r3
    addi     r3, r31, 0x7c
    li       r4, 0x69
    li       r5, lbl_8065F690@sda21
    li       r7, lbl_8065AD68@sda21
    crclr    cr1eq
    bl       OSPanic
.L_8019EEF4:
    addi     r30, r30, 0x8
.L_8019EEF8:
    lhz      r4, 0x4(r30)
    lhz      r5, 0x6(r30)
    cmplw    r4, r5
    bne      .L_8019EDEC
    addi     r4, r31, 0x12c
    li       r29, lbl_8065B86C@sda21
    li       r3, 0x3e80
    bl       Alloc__2mmFUlPCc
    cmplwi   r3, 0x0
    stw      r3, lbl_8065B86C@sda21(r0)
    beq      .L_8019F034
    li       r12, 0x0
    li       r0, 0x3
    stw      r12, 0x4(r29)
    li       r24, 0x0
    mtctr    r0
.L_8019EF38:
    lwz      r3, 0x0(r29)
    addi     r10, r12, 0x120
    li       r11, 0x0
    addi     r9, r12, 0x260
    sthx     r11, r3, r10
    addi     r8, r12, 0x3a0
    addi     r7, r12, 0x4e0
    addi     r6, r12, 0x620
    lwz      r3, 0x0(r29)
    addi     r5, r12, 0x760
    addi     r4, r12, 0x8a0
    addi     r0, r12, 0x9e0
    sthx     r11, r3, r9
    addi     r10, r12, 0xb20
    addi     r9, r12, 0xc60
    addi     r24, r24, 0x10
    lwz      r3, 0x0(r29)
    sthx     r11, r3, r8
    addi     r8, r12, 0xda0
    lwz      r3, 0x0(r29)
    sthx     r11, r3, r7
    addi     r7, r12, 0xee0
    lwz      r3, 0x0(r29)
    sthx     r11, r3, r6
    addi     r6, r12, 0x1020
    lwz      r3, 0x0(r29)
    sthx     r11, r3, r5
    addi     r5, r12, 0x1160
    lwz      r3, 0x0(r29)
    sthx     r11, r3, r4
    addi     r4, r12, 0x12a0
    lwz      r3, 0x0(r29)
    sthx     r11, r3, r0
    addi     r0, r12, 0x13e0
    addi     r12, r12, 0x1400
    lwz      r3, 0x0(r29)
    sthx     r11, r3, r10
    lwz      r3, 0x0(r29)
    sthx     r11, r3, r9
    lwz      r3, 0x0(r29)
    sthx     r11, r3, r8
    lwz      r3, 0x0(r29)
    sthx     r11, r3, r7
    lwz      r3, 0x0(r29)
    sthx     r11, r3, r6
    lwz      r3, 0x0(r29)
    sthx     r11, r3, r5
    lwz      r3, 0x0(r29)
    sthx     r11, r3, r4
    lwz      r3, 0x0(r29)
    sthx     r11, r3, r0
    bdnz     .L_8019EF38
    subfic   r0, r24, 0x32
    mulli    r4, r24, 0x140
    mtctr    r0
    cmpwi    r24, 0x32
    bge      .L_8019F120
.L_8019F01C:
    lwz      r3, 0x0(r29)
    addi     r0, r4, 0x120
    addi     r4, r4, 0x140
    sthx     r11, r3, r0
    bdnz     .L_8019F01C
    b        .L_8019F120
.L_8019F034:
    addi     r3, r31, 0x6c
    crclr    cr1eq
    bl       OSReport
    addi     r3, r31, 0x7c
    addi     r6, r31, 0x138
    li       r4, 0x76
    li       r5, lbl_8065F690@sda21
    li       r7, lbl_8065AD68@sda21
    crclr    cr1eq
    bl       OSPanic
    b        .L_8019F120
.L_8019F060:
    lwz      r3, lbl_8065B868@sda21(r0)
    cmpwi    r3, 0x0
    beq      .L_8019F07C
    lis      r3, lbl_80237AA0@ha
    addi     r0, r3, lbl_80237AA0@l
    stw      r0, lbl_8065B864@sda21(r0)
    b        .L_8019F120
.L_8019F07C:
    cmplwi   r4, 0x1
    li       r0, 0x0
    bne      .L_8019F094
    cmpwi    r3, 0x0
    bne      .L_8019F094
    li       r0, 0x1
.L_8019F094:
    clrlwi.  r0, r0, 24
    lis      r3, 0x2
    addi     r3, r3, 0x120
    beq      .L_8019F0AC
    lis      r3, 0x12
    addi     r3, r3, 0xf00
.L_8019F0AC:
    addi     r4, r31, 0x160
    bl       Alloc__2mmFUlPCc
    cmplwi   r3, 0x0
    stw      r3, lbl_8065B864@sda21(r0)
    bne      .L_8019F0E8
    addi     r3, r31, 0x6c
    crclr    cr1eq
    bl       OSReport
    addi     r3, r31, 0x7c
    addi     r6, r31, 0x88
    li       r4, 0x83
    li       r5, lbl_8065F690@sda21
    li       r7, lbl_8065AD68@sda21
    crclr    cr1eq
    bl       OSPanic
.L_8019F0E8:
    lwz      r3, lbl_8065B864@sda21(r0)
    bl       OSInitFont
    cmpwi    r3, 0x0
    bne      .L_8019F120
    addi     r3, r31, 0x6c
    crclr    cr1eq
    bl       OSReport
    addi     r3, r31, 0x7c
    addi     r6, r31, 0x16c
    li       r4, 0x87
    li       r5, lbl_8065F690@sda21
    li       r7, lbl_8065AD68@sda21
    crclr    cr1eq
    bl       OSPanic
.L_8019F120:
    lwz      r0, lbl_806602C8@sda21(r0)
    li       r3, 0x0
    lwz      r4, lbl_806602CC@sda21(r0)
    stw      r0, 0x10(r1)
    stw      r28, 0x24(r28)
    stw      r4, 0x28(r28)
    lhz      r0, lbl_8065B860@sda21(r0)
    stw      r4, 0x14(r1)
    cmplwi   r0, 0x1
    stw      r28, 0x10(r1)
    bne      .L_8019F15C
    lwz      r0, lbl_8065B868@sda21(r0)
    cmpwi    r0, 0x0
    bne      .L_8019F15C
    li       r3, 0x1
.L_8019F15C:
    clrlwi.  r0, r3, 24
    beq      .L_8019F218
    lwz      r0, 0x18(r31)
    lwz      r12, 0x1c(r31)
    stw      r0, 0x3c(r1)
    lwz      r11, 0x20(r31)
    lwz      r10, 0x24(r31)
    lwz      r9, 0x28(r31)
    lwz      r8, 0x2c(r31)
    lwz      r7, 0x30(r31)
    lwz      r6, 0x34(r31)
    lwz      r4, 0x38(r31)
    lhz      r3, 0x3c(r1)
    lhz      r0, 0x3e(r1)
    sth      r3, 0x0(r28)
    lfs      f1, lbl_8065F680@sda21(r0)
    sth      r0, 0x2(r28)
    lfs      f0, lbl_8065F684@sda21(r0)
    stw      r12, 0x4(r28)
    stw      r11, 0x8(r28)
    stw      r4, 0x5c(r1)
    stw      r10, 0xc(r28)
    lbz      r5, 0x5c(r1)
    stw      r9, 0x10(r28)
    lbz      r4, 0x5d(r1)
    stw      r6, 0x58(r1)
    lbz      r3, 0x5e(r1)
    stw      r8, 0x14(r28)
    lfs      f2, 0x58(r1)
    stw      r7, 0x18(r28)
    lbz      r0, 0x5f(r1)
    stfs     f2, 0x1c(r28)
    stb      r5, 0x20(r28)
    stb      r4, 0x21(r28)
    stb      r3, 0x22(r28)
    stb      r0, 0x23(r28)
    stfs     f1, 0x2c(r28)
    stfs     f1, 0x30(r28)
    stfs     f0, 0x34(r28)
    stw      r12, 0x40(r1)
    stw      r11, 0x44(r1)
    stw      r10, 0x48(r1)
    stw      r9, 0x4c(r1)
    stw      r8, 0x50(r1)
    stw      r7, 0x54(r1)
    stfs     f0, 0x38(r28)
    b        .L_8019F364
.L_8019F218:
    lwz      r4, lbl_8065B864@sda21(r0)
    lis      r0, 0x4330
    lwz      r25, 0x3c(r31)
    lwz      r26, 0x40(r31)
    lwz      r27, 0x44(r31)
    lwz      r30, 0x48(r31)
    lwz      r29, 0x4c(r31)
    lwz      r12, 0x50(r31)
    lwz      r11, 0x54(r31)
    lwz      r10, 0x58(r31)
    lwz      r3, 0x5c(r31)
    lhz      r9, 0x20(r4)
    lhz      r8, 0x1e(r4)
    lhz      r7, 0x18(r4)
    stw      r3, 0x38(r1)
    lfd      f2, lbl_8065F6A0@sda21(r0)
    sth      r9, 0x0(r28)
    lbz      r6, 0x38(r1)
    sth      r8, 0x2(r28)
    lbz      r5, 0x39(r1)
    stw      r7, 0x4(r28)
    lbz      r4, 0x3a(r1)
    stw      r27, 0x8(r28)
    lbz      r3, 0x3b(r1)
    stw      r30, 0xc(r28)
    lfs      f3, lbl_8065F684@sda21(r0)
    stw      r29, 0x10(r28)
    lfd      f1, lbl_8065F688@sda21(r0)
    stw      r10, 0x34(r1)
    stw      r12, 0x14(r28)
    lfs      f0, 0x34(r1)
    stw      r11, 0x18(r28)
    stfs     f0, 0x1c(r28)
    stb      r6, 0x20(r28)
    stb      r5, 0x21(r28)
    stb      r4, 0x22(r28)
    stb      r3, 0x23(r28)
    lwz      r3, lbl_8065B864@sda21(r0)
    stw      r0, 0x60(r1)
    lhz      r3, 0x1e(r3)
    stw      r25, 0x18(r1)
    stw      r3, 0x64(r1)
    lfd      f0, 0x60(r1)
    stw      r26, 0x1c(r1)
    fsubs    f0, f0, f2
    stw      r0, 0x68(r1)
    fdivs    f0, f3, f0
    stw      r0, 0x70(r1)
    stw      r0, 0x78(r1)
    stw      r27, 0x20(r1)
    stw      r30, 0x24(r1)
    stw      r29, 0x28(r1)
    stfs     f0, 0x2c(r28)
    lwz      r3, lbl_8065B864@sda21(r0)
    stw      r12, 0x2c(r1)
    lhz      r0, 0x20(r3)
    stw      r11, 0x30(r1)
    stw      r0, 0x6c(r1)
    lfd      f0, 0x68(r1)
    sth      r9, 0x18(r1)
    fsubs    f0, f0, f2
    sth      r8, 0x1a(r1)
    fdivs    f0, f3, f0
    stw      r7, 0x1c(r1)
    stfs     f0, 0x30(r28)
    lwz      r3, lbl_8065B864@sda21(r0)
    lhz      r3, 0x10(r3)
    subi     r0, r3, 0x2
    xoris    r0, r0, 0x8000
    stw      r0, 0x74(r1)
    lfd      f0, 0x70(r1)
    fsubs    f0, f0, f1
    fdivs    f0, f3, f0
    stfs     f0, 0x34(r28)
    lwz      r3, lbl_8065B864@sda21(r0)
    lhz      r3, 0x12(r3)
    subi     r0, r3, 0x2
    xoris    r0, r0, 0x8000
    stw      r0, 0x7c(r1)
    lfd      f0, 0x78(r1)
    fsubs    f0, f0, f1
    fdivs    f0, f3, f0
    stfs     f0, 0x38(r28)
.L_8019F364:
    lmw      r24, 0x80(r1)
    lwz      r0, 0xa4(r1)
    mtlr     r0
    addi     r1, r1, 0xa0
    blr
*/
