#include "game/hsd_jobj.h"
#include "game/ut.h"
#include "game/edgeperpendicular.h"

extern "C" const Vec sTriangleEdgeTestDown;

static inline BOOL IsOutsideEdgeXZ(const Vec* pos, const Vec* a, const Vec* b, Vec& down, Vec& edge, Vec& rel, Vec& normal)
{
	edge.x = a->x - b->x;
	edge.z = a->z - b->z;
	edge.y = 0.f;
	rel.x  = b->x - pos->x;
	rel.z  = b->z - pos->z;
	rel.y  = 0.f;
	C_VECCrossProduct(&rel, &edge, &normal);
	return PSVECDotProduct(&down, &normal) > 0.0f;
}

/**
 * @note Address: 0x8018EC6C
 * @note Size: 0x1C0
 */
BOOL IsPointInsideTriangleXZ(Vec* __restrict pos, Vec* __restrict a, Vec* __restrict b, Vec* __restrict c)
{
	Vec down = sTriangleEdgeTestDown;
	Vec edge, rel, normal;
	if (IsOutsideEdgeXZ(pos, a, b, down, edge, rel, normal))
		return FALSE;
	if (IsOutsideEdgeXZ(pos, b, c, down, edge, rel, normal))
		return FALSE;
	if (IsOutsideEdgeXZ(pos, c, a, down, edge, rel, normal))
		return FALSE;
	return TRUE;
}

// IsPointInsideTriangleXZ
/*
    stwu     r1, -0x50(r1)
    mflr     r0
    lfs      f3, 0x0(r4)
    stw      r0, 0x54(r1)
    lfs      f1, 0x0(r5)
    stw      r31, 0x4c(r1)
    mr       r31, r6
    fsubs    f5, f3, f1
    lfs      f3, 0x8(r4)
    stw      r30, 0x48(r1)
    mr       r30, r5
    lfs      f4, 0x8(r30)
    stw      r29, 0x44(r1)
    mr       r29, r4
    fsubs    f3, f3, f4
    stw      r28, 0x40(r1)
    mr       r28, r3
    lfs      f0, 0x0(r28)
    lis      r3, sTriangleEdgeTestDown@ha
    lwzu     r5, sTriangleEdgeTestDown@l(r3)
    fsubs    f1, f1, f0
    lfs      f0, 0x8(r28)
    lfs      f2, sGeometryZero@sda21(r0)
    addi     r4, r1, 0x20
    fsubs    f0, f4, f0
    lwz      r7, 0x4(r3)
    lwz      r0, 0x8(r3)
    addi     r3, r1, 0x14
    stw      r5, 0x2c(r1)
    addi     r5, r1, 0x8
    stw      r7, 0x30(r1)
    stw      r0, 0x34(r1)
    stfs     f5, 0x20(r1)
    stfs     f3, 0x28(r1)
    stfs     f2, 0x24(r1)
    stfs     f1, 0x14(r1)
    stfs     f0, 0x1c(r1)
    stfs     f2, 0x18(r1)
    bl       C_VECCrossProduct
    addi     r3, r1, 0x2c
    addi     r4, r1, 0x8
    bl       PSVECDotProduct
    lfs      f5, sGeometryZero@sda21(r0)
    fcmpo    cr0, f1, f5
    ble      .L_8018ED28
    li       r3, 0x0
    b        .L_8018EE0C
.L_8018ED28:
    lfs      f2, 0x0(r30)
    addi     r3, r1, 0x14
    lfs      f1, 0x0(r31)
    addi     r4, r1, 0x20
    lfs      f0, 0x0(r28)
    addi     r5, r1, 0x8
    fsubs    f4, f2, f1
    lfs      f2, 0x8(r30)
    lfs      f3, 0x8(r31)
    fsubs    f1, f1, f0
    lfs      f0, 0x8(r28)
    fsubs    f2, f2, f3
    fsubs    f0, f3, f0
    stfs     f4, 0x20(r1)
    stfs     f2, 0x28(r1)
    stfs     f5, 0x24(r1)
    stfs     f1, 0x14(r1)
    stfs     f0, 0x1c(r1)
    stfs     f5, 0x18(r1)
    bl       C_VECCrossProduct
    addi     r3, r1, 0x2c
    addi     r4, r1, 0x8
    bl       PSVECDotProduct
    lfs      f5, sGeometryZero@sda21(r0)
    fcmpo    cr0, f1, f5
    ble      .L_8018ED98
    li       r3, 0x0
    b        .L_8018EE0C
.L_8018ED98:
    lfs      f2, 0x0(r31)
    addi     r3, r1, 0x14
    lfs      f1, 0x0(r29)
    addi     r4, r1, 0x20
    lfs      f0, 0x0(r28)
    addi     r5, r1, 0x8
    fsubs    f4, f2, f1
    lfs      f2, 0x8(r31)
    lfs      f3, 0x8(r29)
    fsubs    f1, f1, f0
    lfs      f0, 0x8(r28)
    fsubs    f2, f2, f3
    fsubs    f0, f3, f0
    stfs     f4, 0x20(r1)
    stfs     f2, 0x28(r1)
    stfs     f5, 0x24(r1)
    stfs     f1, 0x14(r1)
    stfs     f0, 0x1c(r1)
    stfs     f5, 0x18(r1)
    bl       C_VECCrossProduct
    addi     r3, r1, 0x2c
    addi     r4, r1, 0x8
    bl       PSVECDotProduct
    lfs      f0, sGeometryZero@sda21(r0)
    fcmpo    cr0, f1, f0
    mfcr     r0
    extrwi   r0, r0, 1, 1
    cntlzw   r0, r0
    srwi     r3, r0, 5
.L_8018EE0C:
    lwz      r0, 0x54(r1)
    lwz      r31, 0x4c(r1)
    lwz      r30, 0x48(r1)
    lwz      r29, 0x44(r1)
    lwz      r28, 0x40(r1)
    mtlr     r0
    addi     r1, r1, 0x50
    blr
*/

/**
 * @note Address: 0x8018EA50
 * @note Size: 0x21C
 */
#pragma push
#pragma opt_propagation off
BOOL IsPointInsideQuadXZ(Vec* __restrict pos, Vec* __restrict a, Vec* __restrict b, Vec* __restrict c, Vec* __restrict d)
{
	Vec down = { 0.0f, -1.0f, 0.0f };
	Vec edge, rel, normal;
	if (IsOutsideEdgeXZ(pos, a, b, down, edge, rel, normal))
		return FALSE;
	if (IsOutsideEdgeXZ(pos, b, c, down, edge, rel, normal))
		return FALSE;
	if (IsOutsideEdgeXZ(pos, c, d, down, edge, rel, normal))
		return FALSE;
	if (IsOutsideEdgeXZ(pos, d, a, down, edge, rel, normal))
		return FALSE;
	return TRUE;
}
#pragma pop

// IsPointInsideQuadXZ
/*
    stwu     r1, -0x50(r1)
    mflr     r0
    lfs      f3, 0x0(r4)
    stw      r0, 0x54(r1)
    lfs      f1, 0x0(r5)
    stmw     r27, 0x3c(r1)
    mr       r27, r3
    lfs      f2, sGeometryZero@sda21(r0)
    mr       r29, r5
    fsubs    f5, f3, f1
    lfs      f0, 0x0(r27)
    mr       r28, r4
    lis      r3, sQuadEdgeTestDown@ha
    fsubs    f1, f1, f0
    lfs      f3, 0x8(r28)
    lfs      f4, 0x8(r29)
    mr       r30, r6
    lwzu     r4, sQuadEdgeTestDown@l(r3)
    mr       r31, r7
    lfs      f0, 0x8(r27)
    fsubs    f3, f3, f4
    lwz      r5, 0x4(r3)
    lwz      r0, 0x8(r3)
    fsubs    f0, f4, f0
    stw      r4, 0x2c(r1)
    addi     r3, r1, 0x14
    addi     r4, r1, 0x20
    stw      r5, 0x30(r1)
    addi     r5, r1, 0x8
    stw      r0, 0x34(r1)
    stfs     f5, 0x20(r1)
    stfs     f3, 0x28(r1)
    stfs     f2, 0x24(r1)
    stfs     f1, 0x14(r1)
    stfs     f0, 0x1c(r1)
    stfs     f2, 0x18(r1)
    bl       C_VECCrossProduct
    addi     r3, r1, 0x2c
    addi     r4, r1, 0x8
    bl       PSVECDotProduct
    lfs      f5, sGeometryZero@sda21(r0)
    fcmpo    cr0, f1, f5
    ble      .L_8018EB04
    li       r3, 0x0
    b        .L_8018EC58
.L_8018EB04:
    lfs      f2, 0x0(r29)
    addi     r3, r1, 0x14
    lfs      f1, 0x0(r30)
    addi     r4, r1, 0x20
    lfs      f0, 0x0(r27)
    addi     r5, r1, 0x8
    fsubs    f4, f2, f1
    lfs      f2, 0x8(r29)
    lfs      f3, 0x8(r30)
    fsubs    f1, f1, f0
    lfs      f0, 0x8(r27)
    fsubs    f2, f2, f3
    fsubs    f0, f3, f0
    stfs     f4, 0x20(r1)
    stfs     f2, 0x28(r1)
    stfs     f5, 0x24(r1)
    stfs     f1, 0x14(r1)
    stfs     f0, 0x1c(r1)
    stfs     f5, 0x18(r1)
    bl       C_VECCrossProduct
    addi     r3, r1, 0x2c
    addi     r4, r1, 0x8
    bl       PSVECDotProduct
    lfs      f5, sGeometryZero@sda21(r0)
    fcmpo    cr0, f1, f5
    ble      .L_8018EB74
    li       r3, 0x0
    b        .L_8018EC58
.L_8018EB74:
    lfs      f2, 0x0(r30)
    addi     r3, r1, 0x14
    lfs      f1, 0x0(r31)
    addi     r4, r1, 0x20
    lfs      f0, 0x0(r27)
    addi     r5, r1, 0x8
    fsubs    f4, f2, f1
    lfs      f2, 0x8(r30)
    lfs      f3, 0x8(r31)
    fsubs    f1, f1, f0
    lfs      f0, 0x8(r27)
    fsubs    f2, f2, f3
    fsubs    f0, f3, f0
    stfs     f4, 0x20(r1)
    stfs     f2, 0x28(r1)
    stfs     f5, 0x24(r1)
    stfs     f1, 0x14(r1)
    stfs     f0, 0x1c(r1)
    stfs     f5, 0x18(r1)
    bl       C_VECCrossProduct
    addi     r3, r1, 0x2c
    addi     r4, r1, 0x8
    bl       PSVECDotProduct
    lfs      f5, sGeometryZero@sda21(r0)
    fcmpo    cr0, f1, f5
    ble      .L_8018EBE4
    li       r3, 0x0
    b        .L_8018EC58
.L_8018EBE4:
    lfs      f2, 0x0(r31)
    addi     r3, r1, 0x14
    lfs      f1, 0x0(r28)
    addi     r4, r1, 0x20
    lfs      f0, 0x0(r27)
    addi     r5, r1, 0x8
    fsubs    f4, f2, f1
    lfs      f2, 0x8(r31)
    lfs      f3, 0x8(r28)
    fsubs    f1, f1, f0
    lfs      f0, 0x8(r27)
    fsubs    f2, f2, f3
    fsubs    f0, f3, f0
    stfs     f4, 0x20(r1)
    stfs     f2, 0x28(r1)
    stfs     f5, 0x24(r1)
    stfs     f1, 0x14(r1)
    stfs     f0, 0x1c(r1)
    stfs     f5, 0x18(r1)
    bl       C_VECCrossProduct
    addi     r3, r1, 0x2c
    addi     r4, r1, 0x8
    bl       PSVECDotProduct
    lfs      f0, sGeometryZero@sda21(r0)
    fcmpo    cr0, f1, f0
    mfcr     r0
    extrwi   r0, r0, 1, 1
    cntlzw   r0, r0
    srwi     r3, r0, 5
.L_8018EC58:
    lmw      r27, 0x3c(r1)
    lwz      r0, 0x54(r1)
    mtlr     r0
    addi     r1, r1, 0x50
    blr
*/
