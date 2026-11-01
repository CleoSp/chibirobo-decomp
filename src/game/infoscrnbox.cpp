#include "game/cgs.h"
#include "game/ccamera.h"
#include "game/cgame.h"
#include <math.h>
#include "game/gmath.h"

// a box with round corners, 16 steps a corner and the first point again to close it
/**
 * @note Address: 0x80065E44
 * @note Size: 0x29C
 */
extern "C" void fn_80065E44(u32 color, f32 x, f32 y, f32 width, f32 height, f32 radiusX, f32 radiusY)
{
	Vec points[65];
	f32 rx     = 9.0f * radiusX;
	f32 ry     = 9.0f * radiusY;
	f32 right  = x + width;
	f32 bottom = y + height;
	for (s32 i = 0; i < 65; ++i) {
		f32 angle  = (TWO_PI / 64.0f) * i - HALF_PI;
		f32 sine   = (f32)sin(angle);
		f32 cosine = -(f32)cos(angle);
		f32 cx, cy;
		if ((i >= 0 && i < 16) || i == 64) {
			cx = x;
			cy = y;
		} else if ((i >= 16 && i < 32) || i == 64) {
			cx = right;
			cy = y;
		} else if ((i >= 32 && i < 48) || i == 64) {
			cx = right;
			cy = bottom;
		} else {
			cx = x;
			cy = bottom;
		}
		points[i].x = sine * rx + cx - width * 0.5f;
		points[i].y = cosine * ry + cy - height * 0.5f;
		points[i].z = 0.0f;
	}
	fn_8019B464(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 7, 1);
	fn_80198D08((CGs*)lbl_8025C400.mGs, GX_TRIANGLEFAN, ((CGs*)lbl_8025C400.mGs)->mCamera->mViewMtx, points, color, 65);
	fn_8019B464(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 7, 0);
}
