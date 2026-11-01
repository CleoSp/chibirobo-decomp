#include "game/xobjspider.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/gmath.h"

/**
 * @note Address: 0x800F2E44
 * @note Size: 0x33C
 */
extern "C" void fn_800F2E44(XObjSpider* spider, const Vec* origin, f32 opacity)
{
	if (!(spider->mStatus.mFlags & 8ULL))
		return;
	switch (spider->_25C) {
	case 3:
	case 4:
	case 5:
	case 7:
	case 8:
	case 9:
		return;
	}
	Vec pos          = *origin;
	Vec rot          = { -HALF_PI, 0.0f, 0.0f };
	UnkObjData* data = spider->mAttr._00;
	Vec size         = spider->mMoveScale * (f32)(data != NULL ? data->_0C : 0);
	f32 radius       = spider->mRadius;
	size.x           = radius;
	size.y           = radius;
	size.z           = radius;
	fn_8019B56C(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 4, 5, 15);
	u32 edge   = (u32)(128.0f * opacity);
	u32 center = (u32)(160.0f * opacity);
	fn_80198258((CGs*)lbl_8025C400.mGs, 12, center, edge, pos.x, pos.y, pos.z, size.x, size.z, rot.x, rot.y, rot.z);
	PrimVertex verts[26];
	f32 halfW     = size.x * 0.5f;
	f32 halfH     = size.z * 0.5f;
	f32 outerW    = 1.5f + halfW;
	f32 outerH    = 1.5f + halfH;
	PrimVertex* v = verts;
	for (s32 i = 0; i < 13; ++i, v += 2) {
		f32 angle  = (PI / 6) * i;
		v[0].pos.x = pos.x - halfW * (f32)sin(angle);
		v[0].pos.z = halfH * (f32)cos(angle) + pos.z;
		v[0].pos.y = pos.y;
		v[0].color = edge;
		v[1].pos.x = pos.x - outerW * (f32)sin(angle);
		v[1].pos.z = outerH * (f32)cos(angle) + pos.z;
		v[1].pos.y = pos.y;
		v[1].color = 0;
	}
	fn_801988EC((CGs*)lbl_8025C400.mGs, GX_TRIANGLESTRIP, ((CGs*)lbl_8025C400.mGs)->mCamera->mViewMtx, verts, 26);
}
