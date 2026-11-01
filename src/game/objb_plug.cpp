#include "game/objb.h"

// XObjWireEff, the sparks along the wire, and XObjPlug, the plug the robot pulls around

extern "C" {
void fn_8019B278(UnkRenderA* obj, s32 on);
void fn_8019AED0(UnkRenderB* obj, s32 on);
void fn_8019AE94(UnkRenderC* obj, s32 on);
void HSD_StateInvalidate(s32 state);
void fn_800DA4D0(XObjPlugWire* wire, void* stage, f32 x, f32 y, f32 z);
void fn_800DA68C(XObjPlugWire* wire, u32 frames, f32 x, f32 y, f32 z);
void fn_800DB7A8(XObjPlugWire* wire, s32 on, u8 mode);
void fn_800DB810(XObjPlugWire* wire, f32 x, f32 y, f32 z);
void fn_800DBA7C(XObjPlugWire* wire, s32 count, f32 x, f32 y, f32 z, f32 w);
}

static inline void KillGen(HSD_Generator*& gen)
{
	if (gen != NULL) {
		lbl_8025C400.mParticle.Kill(gen);
		gen = NULL;
	}
}

static inline void StopWireEff(XObjWireEff* obj)
{
	obj->mFlags &= ~0x10000ULL;
	obj->mAnim   = -1;
	obj->mEffect = -1;
	if (obj->mGen != NULL) {
		lbl_8025C400.mParticle.Kill(obj->mGen);
		obj->mGen = NULL;
	}
}

// ---- functions ----

extern "C" {

/**
 * @note Address: 0x800D7D3C
 * @note Size: 0x7C
 */
void fn_800D7D3C(XObjWireEff* obj)
{
	StopWireEff(obj);
}

/**
 * @note Address: 0x800D7DB8
 * @note Size: 0x16C
 */
void fn_800D7DB8(XObjWireEff* obj, s32 anim, s32 effect, f32 rate, u32 color)
{
	obj->mAnim   = anim;
	obj->mEffect = effect;
	obj->mRate   = rate;
	if (obj->mAnim >= 0) {
		obj->mFlags |= 0x10000ULL;
		obj->SetAnim(obj->mAnim, FALSE, NULL, NULL, NULL, 0.0f, obj->mRate, 0.0f, 10.0f);
	} else {
		obj->mFlags &= ~0x10000ULL;
	}
	if (obj->mEffect >= 0) {
		KillGen(obj->mGen);
		obj->mGen = lbl_8025C400.mParticle.Create(0x10052, 0, obj->mJObj, FALSE);
	} else {
		KillGen(obj->mGen);
	}
	obj->SetColor(0, color >> 24, color >> 16, color >> 8, 0.0f);
}

/**
 * @note Address: 0x800D7F24
 * @note Size: 0x60
 */
void fn_800D7F24(XObjWireEff* obj, u32 frames)
{
	if ((obj->mFlags & 1) && (obj->mFlags & 0x10000)) {
		fn_800908C0(obj, frames);
	}
}

/**
 * @note Address: 0x800D7F84
 * @note Size: 0x60
 */
void fn_800D7F84(XObjWireEff* obj, u32 frames)
{
	if (obj->mFlags & 1) {
		if (obj->mAnim >= 0 || obj->mEffect >= 0) {
			fn_80090ED4(obj, frames);
		}
	}
}

/**
 * @note Address: 0x800D7FE4
 * @note Size: 0x84
 */
void fn_800D7FE4(XObjWireEff* obj)
{
	StopWireEff(obj);
	fn_800911C0(obj);
}

/**
 * @note Address: 0x800D8068
 * @note Size: 0x60
 */
void fn_800D8068(XObjWireEff* obj)
{
	fn_800928E8(obj);
	obj->mFlags &= ~0x10000ULL;
	obj->mAnim   = -1;
	obj->mEffect = -1;
	obj->mGen    = NULL;
}

/**
 * @note Address: 0x800D80C8
 * @note Size: 0xC
 */
void fn_800D80C8(XObjPlug* obj)
{
	obj->_16D8 = -1;
}

/**
 * @note Address: 0x800D80D4
 * @note Size: 0x10
 */
void fn_800D80D4(XObjPlug* obj, s32 value)
{
	obj->_16D8 = value;
	obj->_16DC = -1;
}

/**
 * @note Address: 0x800D80E4
 * @note Size: 0x54
 */
void fn_800D80E4(XObjPlug* obj, s32 on, u8 mode)
{
	fn_80091834(obj, on, mode);
	fn_800DB7A8(&obj->mWire, on, mode);
}

/**
 * @note Address: 0x800D8138
 * @note Size: 0x4C
 */
void fn_800D8138(XObjPlug* obj)
{
	if (obj->mJObj != NULL) {
		fn_800DA4D0(&obj->mWire, lbl_8025C400._44 + 0xE8, 1.0f, 1.0f, 1.0f);
	}
}

/**
 * @note Address: 0x800D8184
 * @note Size: 0x1A8
 */
void fn_800D8184(XObjPlug* obj)
{
	obj->mWire.mColors[0] = 0x14141440;
	obj->mWire.mColors[1] = 0x14141440;
	obj->mWire._58        = 0.15f;
	if (obj->mJObj != NULL) {
		UnkGame* game = &lbl_8025C400;
		if (game->_290 != NULL) {
			fn_8019B278(game->_290, 1);
		}
		if (game->_294 != NULL) {
			fn_8019AED0(game->_294, 1);
		}
		if (game->_298 != NULL) {
			fn_8019AE94(game->_298, 1);
		}
		fn_800DA68C(&obj->mWire, 0, 1.0f, 1.0f, 1.0f);
		UnkGame* game2 = &lbl_8025C400;
		HSD_StateInvalidate(-1);
		if (game->_290 != NULL) {
			UnkRenderA* a = game2->_290;
			if (a->_0C == 0) {
				a->_0C = 1;
			}
			if (a->_10 == 0) {
				a->_10 = 1;
			}
			if (a->_14 == 0) {
				a->_14 = 1;
			}
			if (a->_18 == 0) {
				a->_18 = 1;
			}
		}
		UnkRenderB* b = game2->_294;
		if (b != NULL) {
			if (b->_74 == 0) {
				b->_74 = 1;
			}
			if (b->_78 == 0) {
				b->_78 = 1;
			}
			if (b->_7C == 0) {
				b->_7C = 1;
			}
		}
		UnkRenderC* c = game2->_298;
		if (c != NULL) {
			if (c->_30 == 0) {
				c->_30 = 1;
			}
		}
	}
	obj->mWire.mColors[0] = 0x141414FF;
	obj->mWire.mColors[1] = 0x141414FF;
	obj->mWire._58        = 0.2f;
}

/**
 * @note Address: 0x800D832C
 * @note Size: 0x2C
 */
void fn_800D832C(XObjPlug* obj, u32 frames)
{
	obj->vfB0(frames);
}

/**
 * @note Address: 0x800D8358
 * @note Size: 0x74
 */
void fn_800D8358(XObjPlug* obj, u32 frames)
{
	fn_800908C0(obj, frames);
	obj->mWireEff.vfB0(frames);
	if (obj->mJObj != NULL) {
		fn_800DA68C(&obj->mWire, frames, 1.0f, 1.0f, 1.0f);
	}
}

/**
 * @note Address: 0x800D9068
 * @note Size: 0x34
 */
CVec fn_800D9068(XObjPlugWire* wire, s32 i)
{
	return wire->mPoints[i].mPos;
}

/**
 * @note Address: 0x800D93C8
 * @note Size: 0x4C
 */
void fn_800D93C8(XObjPlug* obj)
{
	obj->mWireEff.Release();
	fn_800911C0(obj);
	obj->mLoadState = 0;
}

/**
 * @note Address: 0x800D9414
 * @note Size: 0x104
 */
BOOL fn_800D9414(XObjPlug* obj, const CoordInfo& info)
{
	switch (obj->mLoadState) {
	case 0:
		if (!fn_80091C80(obj, info)) {
			break;
		}
		obj->mLoadState++;
	case 1:
		if (obj->mWireEff.vf9C(CoordInfo(GetSpawnData(0x10)))) {
			Vec pos = lbl_8025C400.mChr->mMoveTranslate;
			fn_800DB810(&obj->mWire, pos.x, pos.y, pos.z);
			obj->mLoadState++;
		}
		break;
	}
	return obj->mLoadState > 1;
}

/**
 * @note Address: 0x800D9A54
 * @note Size: 0x28
 */
void fn_800D9A54(XObjPlug* obj)
{
	obj->mFlags &= ~0x400000000000000ULL;
}

/**
 * @note Address: 0x800D9A7C
 * @note Size: 0x8
 */
void fn_800D9A7C(XObjPlug* obj, f32 value)
{
	obj->mWire._08 = value;
}

/**
 * @note Address: 0x800D9A84
 * @note Size: 0x8C
 */
void fn_800D9A84(XObjPlug* obj, u32 id)
{
	fn_800929C4(obj, id);
	fn_800DBA7C(&obj->mWire, 20, 0.0f, 0.0f, 0.0f, 0.5f);
	obj->mWire.mColors[0] = 0x141414FF;
	obj->mWire.mColors[1] = 0x141414FF;
	obj->mWire._58        = 0.2f;
	obj->mLoadState       = 0;
	obj->mWireEff.Init(id);
}

} // extern "C"
