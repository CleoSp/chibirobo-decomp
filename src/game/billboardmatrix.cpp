#include <dolphin/mtx.h>
#include "game/ut.h"
#include <math.h>

extern "C" {
extern const Vec sBillboardViewZAxis;
void C_VECCrossProduct(const Vec* a, const Vec* b, Vec* axb);
void PSMTXRotRad(Mtx m, char axis, f32 rad);
}

/**
 * @note Address: 0x80190E60
 * @note Size: 0x354
 */
void BuildBillboardMatrixFromUp(Mtx src, Mtx dst)
{
	Vec x, y;
	Vec z  = sBillboardViewZAxis;
	f32 sx = sqrtf(src[0][0] * src[0][0] + src[1][0] * src[1][0] + src[2][0] * src[2][0]);
	f32 sz = sqrtf(src[0][2] * src[0][2] + src[1][2] * src[1][2] + src[2][2] * src[2][2]);
	y.x    = src[0][1];
	y.y    = src[1][1];
	y.z    = src[2][1];
	f32 sy = PSVECMag(&y);
	C_VECCrossProduct(&y, &z, &x);
	C_VECCrossProduct(&z, &x, &y);
	sx /= PSVECMag(&x);
	sy /= PSVECMag(&y);
	dst[0][0] = sx * x.x;
	dst[1][0] = sx * x.y;
	dst[2][0] = sx * x.z;
	dst[0][1] = sy * y.x;
	dst[1][1] = sy * y.y;
	dst[2][1] = sy * y.z;
	dst[0][2] = sz * z.x;
	dst[1][2] = sz * z.y;
	dst[2][2] = sz * z.z;
	dst[0][3] = src[0][3];
	dst[1][3] = src[1][3];
	dst[2][3] = src[2][3];
}

/**
 * @note Address: 0x80190AF0
 * @note Size: 0x370
 */
void BuildBillboardMatrixWithZRotation(Mtx src, Mtx dst, f32 angle)
{
	Mtx rot, scale;
	f32 sx = sqrtf(src[0][0] * src[0][0] + src[1][0] * src[1][0] + src[2][0] * src[2][0]);
	f32 sy = sqrtf(src[0][1] * src[0][1] + src[1][1] * src[1][1] + src[2][1] * src[2][1]);
	f32 sz = sqrtf(src[0][2] * src[0][2] + src[1][2] * src[1][2] + src[2][2] * src[2][2]);
	PSMTXScale(scale, sx, sy, sz);
	PSMTXRotRad(rot, 'z', angle);
	rot[0][3] = src[0][3];
	rot[1][3] = src[1][3];
	rot[2][3] = src[2][3];
	PSMTXConcat(rot, scale, dst);
}
