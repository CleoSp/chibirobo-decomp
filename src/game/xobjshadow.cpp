#include "game/xobjs.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/ccamera.h"
#include <math.h>
#include "game/gmath.h"

static inline u32 ShadowSize(XObj* object)
{
	return object->mAttr._00 != NULL ? object->mAttr._00->_0C : 0;
}

/**
 * @note Address: 0x8009056C
 * @note Size: 0x2F8
 */
extern "C" void fn_8009056C(XObj* object, const Vec* origin, f32 opacity)
{
	if (!(object->mStatus.mFlags & 8ULL))
		return;
	if (ShadowSize(object) == 0)
		return;
	Vec position = *origin;
	Vec rotation = { -HALF_PI, 0.0f, 0.0f };
	Vec size     = object->mMoveScale * (f32)ShadowSize(object);
	fn_8019B56C(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 4, 5, 15);
	u32 edge   = (u32)(128.0f * opacity);
	u32 center = (u32)(160.0f * opacity);
	fn_80198258((CGs*)lbl_8025C400.mGs, 12, center, edge, position.x, position.y, position.z, size.x, size.z, rotation.x, rotation.y,
	            rotation.z);
	PrimVertex vertices[26];
	f32 halfWidth      = size.x * 0.5f;
	f32 halfHeight     = size.z * 0.5f;
	PrimVertex* vertex = vertices;
	for (s32 i = 0; i < 13; ++i, vertex += 2) {
		f32 angle       = (PI / 6.0f) * i;
		vertex[0].pos.x = __fnmsubs(halfWidth, (f32)sin(angle), position.x);
		vertex[0].pos.z = __fmadds(halfHeight, (f32)cos(angle), position.z);
		vertex[0].pos.y = position.y;
		vertex[0].color = edge;
		// retail reads the scale again here
		f32 sine        = sin(angle);
		f32 outerWidth  = __fmadds(1.5f, object->mMoveScale.x, halfWidth);
		vertex[1].pos.x = __fnmsubs(outerWidth, sine, position.x);
		f32 cosine      = cos(angle);
		f32 outerHeight = __fmadds(1.5f, object->mMoveScale.z, halfHeight);
		vertex[1].pos.z = __fmadds(outerHeight, cosine, position.z);
		vertex[1].pos.y = position.y;
		vertex[1].color = 0;
	}
	fn_801988EC((CGs*)lbl_8025C400.mGs, GX_TRIANGLESTRIP, ((CGs*)lbl_8025C400.mGs)->mCamera->mViewMtx, vertices, 26);
}
