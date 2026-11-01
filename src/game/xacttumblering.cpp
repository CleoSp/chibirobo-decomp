#include "game/xact.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/ccamera.h"
#include <math.h>
#include "game/gmath.h"

/**
 * @note Address: 0x800C36E4
 * @note Size: 0x2F0
 */
// the ring on the floor while he tumbles
extern "C" void fn_800C36E4(XActTumbleFade* ring, const Vec* origin, f32 angle)
{
	if (!(ring->mAmount > 0.0f))
		return;
	CVec offset;
	Mtx rotationMatrix;
	offset.x = ring->mOffset.x;
	offset.y = ring->mOffset.y;
	offset.z = ring->mOffset.z;
	PSMTXRotRad(rotationMatrix, 'y', angle);
	PSMTXMultVec(rotationMatrix, &offset, &offset);
	Vec position = { 0.0f, 0.0f, 0.0f };
	position.x   = origin->x + offset.x;
	position.y   = 0.01f + origin->y;
	position.z   = origin->z + offset.z;
	Vec center   = position;
	f32 diameter = 6.0f * ring->mAmount;
	u32 color    = (u32)(160.0f * ring->mAmount) | 0x00641E00;
	Vec rotation = { -HALF_PI, 0.0f, 0.0f };
	fn_80198258((CGs*)lbl_8025C400.mGs, 24, color, color, center.x, center.y, center.z, diameter, diameter, rotation.x, rotation.y,
	            rotation.z);
	PrimVertex vertices[50];
	f32 outerRadius    = diameter * 0.5f + 1.0f;
	PrimVertex* vertex = vertices;
	for (s32 i = 0; i < 25; ++i, vertex += 2) {
		f32 theta       = 15.0f * DEG2RAD * i;
		vertex[0].pos.x = center.x - diameter * (f32)sin(theta) * 0.5f;
		vertex[0].pos.z = diameter * (f32)cos(theta) * 0.5f + center.z;
		vertex[0].pos.y = center.y;
		vertex[0].color = color;
		vertex[1].pos.x = center.x - outerRadius * (f32)sin(theta);
		vertex[1].pos.z = outerRadius * (f32)cos(theta) + center.z;
		vertex[1].pos.y = center.y;
		vertex[1].color = 0;
	}
	fn_801988EC((CGs*)lbl_8025C400.mGs, GX_TRIANGLESTRIP, ((CGs*)lbl_8025C400.mGs)->mCamera->mViewMtx, vertices, 50);
}
