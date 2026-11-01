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

} // extern "C"
