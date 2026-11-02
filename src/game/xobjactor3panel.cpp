#include "game/xobjactor.h"
#include "game/xchr.h"
#include <stdlib.h>

// XObjActor3's panel on stage02, the count over it

extern "C" {
u16 GXGetTexObjWidth(const GXTexObj* obj);
u16 GXGetTexObjHeight(const GXTexObj* obj);
}

// a texture's size on screen at the scale
static inline f32 TexSize(f32 size, f32 scale)
{
	return scale * size;
}

/**
 * @note Address: 0x8010904C
 * @note Size: 0x59C
 */
// kind 0xE on stage02: the panel and the count of _C04 in two digits over it, faded by _C00
// NONMATCHING: the colours, the texture matrix pointer and the loop count take other registers
extern "C" void fn_8010904C(XObjActor3* actor)
{
	f32 alpha = actor->_C00;
	if (alpha <= 0.0f) {
		return;
	}
	if (!IsStage("stage02")) {
		return;
	}
	if (!actor->_980.mBound) {
		return;
	}
	CTexObj tex;
	tex       = actor->_980.mTex.Init(&((TPLPalette*)actor->_980.mBuffer)->descriptorArray[9], 0);
	f32 scale = 1.0f;
	f32 w     = scale * GXGetTexObjWidth(&tex);
	u16 h     = GXGetTexObjHeight(&tex);
	fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
	fn_80197800(&lbl_8025C400.mGs, &tex, -1, -1, -1, -1, 320.0f, 40.0f, 0.0f, w, TexSize(h, 1.0f), 0.0f, 0.0f, 0.0f);
	fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
	tex    = actor->_980.mTex.Init(&((TPLPalette*)actor->_980.mBuffer)->descriptorArray[1], 0);
	f32 dw = 1.4f * TexSize(GXGetTexObjWidth(&tex) / 11, 1.0f);
	f32 x;
	f32 dh    = 1.4f * TexSize(GXGetTexObjHeight(&tex), 1.0f);
	x         = 110.0f - (1 - actor->_C04 / 10) * 12;
	f32 count = actor->_C04;
	PSMTXScale(tex.mMtx[1], 0.09090909f, 1.0f, 1.0f);
	tex.mMtxDirty = TRUE;
	s32 i         = 0;
	u32 a         = 255.0f * alpha;
	u32 c0;
	u32 c1 = a | 0x1F33FF00;
	c0     = a | 0xFFFFFF00;
	for (; i < 2; i++) {
		f32 p = 1.0f;
		if (i > 0) {
			for (s32 k = 0; k < i; k++) {
				p *= 10.0f;
			}
		}
		s32 d = count / p;
		if (i == 0 || d != 0) {
			d %= 10;
			PSMTXTrans(tex.mMtx[0], d * 0.09090909f, 0.0f, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
			fn_80197800(&lbl_8025C400.mGs, &tex, c0, c0, c1, c1, 7.0f * dw + (x - i * (dw - 8.0f)) - 32.0f, 76.0f, 0.0f, dw, dh, 0.0f, 0.0f,
			            0.0f);
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
		}
	}
}
