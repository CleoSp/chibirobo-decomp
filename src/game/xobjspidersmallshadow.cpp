#include "game/xobjspidersmall.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/ccamera.h"
#include "game/cprimitive.h"
#include "game/gmath.h"
#include <math.h>

/**
 * @note Address: 0x800FDBBC
 * @note Size: 0x330
 */
// the shadow, a disc with a soft edge
extern "C" void vfBC__15XObjSpiderSmallFP3Vecf(XObjSpiderSmall* spider, const Vec* origin, f32 opacity)
{
	if (!(spider->mStatus.mFlags & 8ULL)) {
		return;
	}
	if (!(spider->mSpiderStatus.mFlags & 0x40)) {
		return;
	}
	if (!(spider->mStatus.mFlags & 0x10000ULL)) {
		return;
	}
	Vec pos          = *origin;
	Vec rot          = { -HALF_PI, 0.0f, 0.0f };
	UnkObjData* data = spider->mAttr._00;
	Vec size         = spider->mMoveScale * (f32)(data != NULL ? data->_0C : 0);
	f32 diameter     = spider->mRadius;
	size.x           = diameter;
	size.y           = diameter;
	size.z           = diameter;
	CGs* gs          = (CGs*)lbl_8025C400.mGs;
	fn_8019B56C(gs->mCurRenderMode, 1, 4, 5, 15);
	u32 edge   = (u32)(128.0f * opacity);
	u32 center = (u32)(160.0f * opacity);
	fn_80198258((CGs*)lbl_8025C400.mGs, 12, center, edge, pos.x, pos.y, pos.z, size.x, size.z, rot.x, rot.y, rot.z);
	PrimVertex vtx[26];
	f32 halfWidth   = size.x * 0.5f;
	f32 halfHeight  = size.z * 0.5f;
	f32 outerWidth  = 1.5f + halfWidth;
	f32 outerHeight = 1.5f + halfHeight;
	PrimVertex* v   = vtx;
	for (s32 i = 0; i < 13; ++i, v += 2) {
		f32 angle  = (PI / 6.0f) * i;
		v[0].pos.x = pos.x - halfWidth * (f32)sin(angle);
		v[0].pos.z = pos.z + halfHeight * (f32)cos(angle);
		v[0].pos.y = pos.y;
		v[0].color = edge;
		v[1].pos.x = pos.x - outerWidth * (f32)sin(angle);
		v[1].pos.z = pos.z + outerHeight * (f32)cos(angle);
		v[1].pos.y = pos.y;
		v[1].color = 0;
	}
	fn_801988EC((CGs*)lbl_8025C400.mGs, GX_TRIANGLESTRIP, ((CGs*)lbl_8025C400.mGs)->mCamera->mViewMtx, vtx, 26);
}
