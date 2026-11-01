#include "game/objb_leticker_shared.h"

// ---- functions ----

/**
 * @note Address: 0x800E260C
 * @note Size: 0x3AC
 */
// NONMATCHING: this and part swap saved registers
BOOL XObjLeticker::TurnParts(f32 angle)
{
	XObj* part = GAME_OBJTABLE[mParts[0]];
	f32 my     = HSD_JObjGetRotationY(mJObj);
	f32 c      = cos(HSD_JObjGetRotationY(part->mJObj) - (angle + my));
	if (c < (f32)cos(0.08726646f)) {
		_9F8 = 0.0f;
		_A30 = 0;
		if (part->mAnimId == 0) {
			part->SetRotateY(angle + HSD_JObjGetRotationY(mJObj), 0.1f);
			_A28 = angle;
		}
	}
	if (GetAnimFrame(part->mJObj) < GetAnimEndFrame(part->mJObj)) {
		return TRUE;
	}
	s32 path[9]    = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	HSD_JObj* jobj = FindJoint(part->mJObj, path, _99C + 1);
	if (jobj != NULL) {
		MtxPtr mtx = HSD_JObjGetMtxPtr(jobj);
		Vec pos;
		pos.x = pos.y = pos.z = 0.0f;
		PSMTXMultVec(mtx, &pos, &_A04);
	}
	return TRUE;
}
extern "C" {

/**
 * @note Address: 0x800E2EAC
 * @note Size: 0x1A4
 */
// NONMATCHING: the loop index, pointer and part take other saved registers
void fn_800E2EAC(XObjLeticker* obj, Vec* dir)
{
	XObj* part;
	for (s32 i = 0; i < obj->mPartCount; i++) {
		part = GAME_OBJS[obj->mParts[i]];
		part->SetTranslateX(obj->mMoveTranslate.x, 0);
		part->SetTranslateZ(obj->mMoveTranslate.z, 0);
	}
	if (0.0f < PSVECMag(dir)) {
		obj->SetColor(1, 0, 0, 0, 0.5f);
		obj->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	} else {
		obj->SetColor(1, 0xFF, 0, 0, 1.0f);
		obj->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}
	if (obj->mParticleOn) {
		GAME_PARTICLE->Kill(0xA5);
		obj->mParticleOn = FALSE;
	}
}
} // extern "C"

/**
 * @note Address: 0x800E3050
 * @note Size: 0x1DC
 */
void XObjLeticker::vf148()
{
	if (mMode != 3) {
		GAME_FLAGS &= ~0x80ULL;
		GAME_CAMERA->PopAction();
	}
	_A2D = FALSE;
	_A30 = 0;
	if (mMode == 2) {
		_9A0 = _9A4[GetSide(_A28)];
		_9FC = 10.0f * _9A0;
		_9F8 = 10.0f * _99C;
	}
	SetColor(1, 0, 0, 0, 0.5f);
	SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

/**
 * @note Address: 0x800E3AF0
 * @note Size: 0x4
 */
void XObjLeticker::vf138(const Vec* pos, f32 f)
{
}

static inline void PlayLetickerSound(XObjLeticker& obj)
{
	s32 anim  = obj.mAnimId;
	f32 frame = GetAnimFrame(obj.mJObj);
	obj.mSfx.Play(anim, fn_8001DF00(GAME_SOUND, &obj.mMoveTranslate, 0), frame);
}
