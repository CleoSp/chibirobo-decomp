#include "game/cgs.h"
#include "game/cgame.h"

static inline void DrawBar(Vec* points, u32 color)
{
	fn_80198614((CGs*)lbl_8025C400.mGs, points[0].x, points[0].y, points[0].z, color, points[1].x, points[1].y, points[1].z, color,
	            points[2].x, points[2].y, points[2].z, color, points[3].x, points[3].y, points[3].z, color);
}

// a scroll bar, start and end are the shown part from 0 to 1
/**
 * @note Address: 0x8003EBBC
 * @note Size: 0x354
 */
extern "C" void fn_8003EBBC(void*, f32 x, f32 y, f32 start, f32 end, f32 alpha)
{
	fn_80199AB0((CGs*)lbl_8025C400.mGs, 0, 0);
	CGs* gs = (CGs*)lbl_8025C400.mGs;
	fn_8019B56C(gs->mCurRenderMode, 1, 4, 5, 15);
	fn_8019B464(gs->mCurRenderMode, 0, 3, 0);
	y               = 48.0f + (1.0f + y);
	x               = 10.0f + (27.0f + (x - 4.0f));
	f32 top         = 4.0f + y;
	f32 bottom      = 172.0f + top;
	f32 startY      = 172.0f * start + top;
	f32 endY        = 172.0f * end + top;
	Vec quads[4][4] = { 0 };
	quads[0][0].x   = x;
	quads[0][0].y   = top;
	quads[0][1].x   = 4.0f + x;
	quads[0][1].y   = top;
	quads[0][2].x   = 4.0f + x;
	quads[0][2].y   = bottom;
	quads[0][3].x   = x;
	quads[0][3].y   = bottom;
	quads[1][0].x   = x;
	quads[1][0].y   = top;
	quads[1][1].x   = 4.0f + x;
	quads[1][1].y   = top;
	quads[1][2].x   = 4.0f + x;
	quads[1][2].y   = startY;
	quads[1][3].x   = x;
	quads[1][3].y   = startY;
	quads[2][0].x   = x;
	quads[2][0].y   = startY;
	quads[2][1].x   = 4.0f + x;
	quads[2][1].y   = startY;
	quads[2][2].x   = 4.0f + x;
	quads[2][2].y   = endY;
	quads[2][3].x   = x;
	quads[2][3].y   = endY;
	quads[3][0].x   = x;
	quads[3][0].y   = endY;
	quads[3][1].x   = 4.0f + x;
	quads[3][1].y   = endY;
	quads[3][2].x   = 4.0f + x;
	quads[3][2].y   = bottom;
	quads[3][3].x   = x;
	quads[3][3].y   = bottom;
	u32 a           = (u32)(255.0f * alpha);
	fn_801990EC((CGs*)lbl_8025C400.mGs, quads[0], 0xC8C8C800 | a, 5);
	fn_8019B34C(gs->mCurRenderMode, 6, 5);
	DrawBar(quads[1], a);
	DrawBar(quads[2], 0xFFB62800 | a);
	DrawBar(quads[3], a);
	fn_801990EC((CGs*)lbl_8025C400.mGs, quads[2], 0xFFFFFF00 | a, 5);
	fn_8019B34C(gs->mCurRenderMode, 12, 5);
}
