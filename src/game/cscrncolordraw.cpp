#include "game/hsd_jobj.h"
#include "game/cscrn.h"
#include "game/cgs.h"

// the screen corners
struct ScrnQuad {
	Vec mPos[4]; // _00
};

// this file's rodata, the screen quad the color is drawn as
struct ScrnColorRodata {
	u8 _00[0x48];   // _00
	ScrnQuad mQuad; // _48
};

extern "C" const ScrnColorRodata lbl_801FEE18;

// todo: cleanup
static inline bool HasColor(CScrnColor* color)
{
	return 0.0f != color->mColor.r || 0.0f != color->mColor.g || 0.0f != color->mColor.b || 0.0f != color->mColor.a;
}

/**
 * @note Address: 0x8002390C
 * @note Size: 0x22C
 */
extern "C" void fn_8002390C(CScrnColor* color, CGs* gs)
{
	const ScrnColorRodata* rodata = &lbl_801FEE18;
	if (HasColor(color)) {
		ScrnQuad quad = rodata->mQuad;
		u32 packed    = ((u32)color->mColor.r << 24) | ((u32)color->mColor.g << 16) | ((u32)color->mColor.b << 8) | (u32)color->mColor.a;
		fn_80199AB0(gs, 0, 0);
		fn_8019B56C(gs->mCurRenderMode, 1, 4, 5, 15);
		fn_8019B464(gs->mCurRenderMode, 1, 1, 0);
		fn_80198614(gs, quad.mPos[0].x, quad.mPos[0].y, quad.mPos[0].z, packed, quad.mPos[1].x, quad.mPos[1].y, quad.mPos[1].z, packed,
		            quad.mPos[2].x, quad.mPos[2].y, quad.mPos[2].z, packed, quad.mPos[3].x, quad.mPos[3].y, quad.mPos[3].z, packed);
	}
}
