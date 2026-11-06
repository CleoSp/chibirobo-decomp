#include "game/hsd_jobj.h"
#include "game/hsd_dobj.h"
#include "game/ut.h"
#include "game/gmath.h"
#include "game/edgeperpendicular.h"
#include "game/trianglenormal.h"
extern "C" {
#include <sysdolphin/mtx.h>
#include <sysdolphin/jobj.h>
#include <sysdolphin/mobj.h>
#include <sysdolphin/tobj.h>
#include <dolphin/mtx/vec.h>
#include <dolphin/mtx/mtx.h>
#include "game/cvec.h"
}
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// geometry, joint and string helpers, backwards for deferred inlining

extern "C" {
char sFullWidthTextBuffer[0x80];
// the formatted text, panics and the event debug lines all go through here
char sFormattedTextBuffer[0x100];
}

// todo: copied inline, clean up
// where the line p0 to p1 meets the plane through point, as a fraction of the way along it
static inline f32 LinePlane(Vec* p0, Vec* p1, Vec* point, Vec* normal, Vec* out)
{
	Vec rel, dir;
	PSVECSubtract(p0, point, &rel);
	PSVECSubtract(p1, p0, &dir);
	f32 d = PSVECDotProduct(&dir, normal);
	f32 t = -1.0f * (PSVECDotProduct(&rel, normal) / d);
	PSVECScale(&dir, out, t);
	PSVECAdd(out, p0, out);
	return t;
}

static inline void CalculatePlaneNormal(Vec* a, Vec* b, Vec* c, Vec* normal)
{
	Vec ab, bc;
	CalculateTriangleNormal(a, b, c, normal, ab, bc);
}

/**
 * @note Address: 0x8018E8A4
 * @note Size: 0x1AC
 */
int IntersectVerticalLineWithTrianglePlane(Vec* a, Vec* b, Vec* c, Vec* pos, Vec* out)
{
	Vec normal;
	CalculatePlaneNormal(a, b, c, &normal);

	const f32 x = pos->x;
	const f32 z = pos->z;
	Vec top     = { x, 1.0f, z };
	Vec bottom  = { x, -1.0f, z };
	f32 t       = LinePlane(&top, &bottom, a, &normal, out);

	if (Abs(t) <= 0.00001f || Abs(1.0f - t) <= 0.00001f) {
		return 1;
	}
	if (t > 0.0f && t < 1.0f) {
		return 2;
	}
	return 3;
}

// IntersectVerticalLineWithTrianglePlane
/*
    stwu     r1, -0x90(r1)
    mflr     r0
    stw      r0, 0x94(r1)
    stfd     f31, 0x80(r1)
    psq_st   f31, 0x88(r1), 0, qr0
    stmw     r27, 0x6c(r1)
    mr       r29, r5
    mr       r27, r3
    mr       r28, r4
    mr       r30, r6
    mr       r31, r7
    addi     r5, r1, 0x20
    bl       PSVECSubtract
    mr       r3, r28
    mr       r4, r29
    addi     r5, r1, 0x2c
    bl       PSVECSubtract
    addi     r3, r1, 0x2c
    addi     r4, r1, 0x20
    addi     r5, r1, 0x50
    bl       C_VECCrossProduct
    lis      r4, sVerticalLineTop@ha
    lis      r3, sVerticalLineBottom@ha
    addi     r10, r4, sVerticalLineTop@l
    lfs      f1, 0x0(r30)
    addi     r7, r3, sVerticalLineBottom@l
    lwz      r9, 0x0(r10)
    lwz      r8, 0x8(r10)
    mr       r4, r27
    lwz      r6, 0x0(r7)
    addi     r3, r1, 0x44
    lwz      r0, 0x8(r7)
    addi     r5, r1, 0x8
    stw      r9, 0x44(r1)
    lwz      r9, 0x4(r10)
    stw      r6, 0x38(r1)
    lwz      r6, 0x4(r7)
    stw      r8, 0x4c(r1)
    lfs      f0, 0x8(r30)
    stw      r0, 0x40(r1)
    stw      r9, 0x48(r1)
    stfs     f1, 0x44(r1)
    stfs     f0, 0x4c(r1)
    stw      r6, 0x3c(r1)
    stfs     f1, 0x38(r1)
    stfs     f0, 0x40(r1)
    bl       PSVECSubtract
    addi     r3, r1, 0x38
    addi     r4, r1, 0x44
    addi     r5, r1, 0x14
    bl       PSVECSubtract
    addi     r3, r1, 0x14
    addi     r4, r1, 0x50
    bl       PSVECDotProduct
    fmr      f31, f1
    addi     r3, r1, 0x8
    addi     r4, r1, 0x50
    bl       PSVECDotProduct
    fdivs    f0, f1, f31
    lfs      f1, sGeometryNegativeOne@sda21(r0)
    mr       r4, r31
    addi     r3, r1, 0x14
    fmuls    f31, f1, f0
    fmr      f1, f31
    bl       PSVECScale
    mr       r3, r31
    mr       r5, r31
    addi     r4, r1, 0x44
    bl       PSVECAdd
    lfs      f0, sGeometryZero@sda21(r0)
    fcmpo    cr0, f31, f0
    bge      .L_8018E9CC
    fneg     f1, f31
    b        .L_8018E9D0
.L_8018E9CC:
    fmr      f1, f31
.L_8018E9D0:
    lfs      f0, sLinePlaneEndpointTolerance@sda21(r0)
    fcmpo    cr0, f1, f0
    cror     eq, lt, eq
    beq      .L_8018EA08
    lfs      f1, sGeometryOne@sda21(r0)
    lfs      f0, sGeometryZero@sda21(r0)
    fsubs    f1, f1, f31
    fcmpo    cr0, f1, f0
    bge      .L_8018E9F8
    fneg     f1, f1
.L_8018E9F8:
    lfs      f0, sLinePlaneEndpointTolerance@sda21(r0)
    fcmpo    cr0, f1, f0
    cror     eq, lt, eq
    bne      .L_8018EA10
.L_8018EA08:
    li       r3, 0x1
    b        .L_8018EA34
.L_8018EA10:
    lfs      f0, sGeometryZero@sda21(r0)
    fcmpo    cr0, f31, f0
    ble      .L_8018EA30
    lfs      f0, sGeometryOne@sda21(r0)
    fcmpo    cr0, f31, f0
    bge      .L_8018EA30
    li       r3, 0x2
    b        .L_8018EA34
.L_8018EA30:
    li       r3, 0x3
.L_8018EA34:
    psq_l    f31, 0x88(r1), 0, qr0
    lfd      f31, 0x80(r1)
    lmw      r27, 0x6c(r1)
    lwz      r0, 0x94(r1)
    mtlr     r0
    addi     r1, r1, 0x90
    blr
*/
