#include "game/scrc_shared.h"

extern "C" {

void fn_8010904C(XObjActor3* self);
void fn_801095E8(XObjActor3* self);
void fn_8010AD88(XObjActor3* self);
void fn_8010FB1C(XObjActor3* self);
void fn_8010B394(XObjActor3* self);
void fn_8010B528(XObjActor3* self);
void fn_8010AF74(XObjActor3* self);
void fn_8010B7B4(XObjActor3* self, u32 frames);
void fn_80024D84(CPtcl* ptcl, u32 frames);
void fn_8010FB1C(XObjActor3* self);
void fn_8010B528(XObjActor3* self);
void fn_8010B7B4(XObjActor3* self, u32 frames);
void fn_8010B394(XObjActor3* self);
void fn_8010AD88(XObjActor3* self);
void fn_8010AF74(XObjActor3* self);

// kind 0xE on stage02: the panel and the count of _C04 in two digits over it, faded by _C00
// NONMATCHING: the colours, the texture matrix pointer and the loop count take other registers
void fn_8010904C(XObjActor3* self)
{
	f32 alpha = self->_C00;
	if (alpha <= 0.0f) {
		return;
	}
	if (!IsStage("stage02")) {
		return;
	}
	if (!self->mTpl.mBound) {
		return;
	}
	CTexObj tex;
	tex       = self->mTpl.mTex.Init(&((TPLPalette*)self->mTpl.mBuffer)->descriptorArray[9], 0);
	f32 scale = 1.0f;
	f32 w     = scale * GXGetTexObjWidth(&tex);
	u16 h     = GXGetTexObjHeight(&tex);
	fn_8019AFA0(GS_TEV, 0);
	fn_80197800(lbl_8025C400, &tex, -1, -1, -1, -1, 320.0f, 40.0f, 0.0f, w, TexSize(h, 1.0f), 0.0f, 0.0f, 0.0f);
	fn_8019AFA0(GS_TEV, 4);
	tex    = self->mTpl.mTex.Init(&((TPLPalette*)self->mTpl.mBuffer)->descriptorArray[1], 0);
	f32 dw = 1.4f * TexSize(GXGetTexObjWidth(&tex) / 11, 1.0f);
	f32 x;
	f32 dh    = 1.4f * TexSize(GXGetTexObjHeight(&tex), 1.0f);
	x         = 110.0f - (1 - self->_C04 / 10) * 12;
	f32 count = self->_C04;
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
			fn_8019AFA0(GS_TEV, 0);
			fn_80197800(lbl_8025C400, &tex, c0, c0, c1, c1, 7.0f * dw + (x - i * (dw - 8.0f)) - 32.0f, 76.0f, 0.0f, dw, dh, 0.0f, 0.0f,
			            0.0f);
			fn_8019AFA0(GS_TEV, 4);
		}
	}
}
}
