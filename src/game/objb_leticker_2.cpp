#include "game/objb_leticker_shared.h"

extern "C" {

/**
 * @note Address: 0x800E4AAC
 * @note Size: 0x3AC
 */
s32 fn_800E4AAC(XObj* obj, const Vec* pos)
{
	f32 angle = WrapAngle((f32)atan2(pos->x - obj->mMoveTranslate.x, pos->z - obj->mMoveTranslate.z) - obj->mMoveRotate.y);
	if (3.0f + obj->mMoveTranslate.y < GAME_PLAYER->mMoveTranslate.y) {
		return 0;
	}
	if (Cos(0.7853982f) < Cos(angle)) {
		s32 path[2] = { 0, 2 };
		if (fn_800E4790(obj, FindJoint(obj->mJObj, path, 2))) {
			return -1;
		}
	} else if (Cos(angle) < Cos(3.9269907f)) {
		s32 path[2] = { 0, 6 };
		if (fn_800E4790(obj, FindJoint(obj->mJObj, path, 2))) {
			return -2;
		}
	} else if (0.0f < Sin(angle)) {
		s32 path[2] = { 0, 4 };
		if (fn_800E4790(obj, FindJoint(obj->mJObj, path, 2))) {
			return -3;
		}
	} else {
		s32 path[2] = { 0, 8 };
		if (fn_800E4790(obj, FindJoint(obj->mJObj, path, 2))) {
			return -4;
		}
	}
	return 0;
}
} // extern "C"

/**
 * @note Address: 0x800E4E58
 * @note Size: 0x364
 */
// NONMATCHING: part and the two loop pointers take other saved registers
void XObjLeticker::vfC0()
{
	XObj* part = GAME_OBJTABLE[mParts[0]];
	switch (mMode) {
	case 3:
		if (!(0.0f < _9F8) || (!_A2D && part->mAnimId != 1)) {
			break;
		}
		Mtx mtx;
		u32 colors[50];
		Vec pts[50];
		Vec pos = mMoveTranslate;
		PSMTXTrans(mtx, pos.x, pos.y, pos.z);
		f32 twist = 6.0f * (_9F8 / _9FC) * (f32)sin(0.017453292f * (mStateTime * 5)) + 72.0f;
		u32 i;
		for (i = 0; i < 50; i++) {
			f32 angle = 0.017453292f * mStateTime + 0.017453292f * (twist * (s32)i);
			f32 y     = 12.0f + 0.2f * (24.0f * (s32)i / 50.0f) * _9F8 / _9FC;
			pts[i].x  = 7.1f * (f32)sin(angle);
			pts[i].y  = y;
			pts[i].z  = 7.1f * (f32)cos(angle);
			colors[i] = ((48 - (s32)i) * 32 / 48 + 32) | 0x40F000;
		}
		pts[49].x = pts[44].x;
		pts[49].y = pts[44].y;
		pts[49].z = pts[44].z;
		PSMTXMultVecArray(mtx, pts, pts, 50);
		fn_80198F1C(lbl_8025C400, pts, colors, 50);
		if (!fn_8001E6C8(GAME_SOUND, 0xB0)) {
			fn_8001E904(GAME_SOUND, 0xB0, 0xFF);
			HSD_AudioSFXSetPitchByID(0xB0, 600.0f * (_A34 - 1.0f));
		} else {
			fn_8001E468(GAME_SOUND, 0xB0, fn_8001DF00(GAME_SOUND, &part->mMoveTranslate, 0), 0);
		}
		break;
	}
}

static inline void PlayLetickerSound(XObjLeticker& obj)
{
	s32 anim  = obj.mAnimId;
	f32 frame = GetAnimFrame(obj.mJObj);
	obj.mSfx.Play(anim, fn_8001DF00(GAME_SOUND, &obj.mMoveTranslate, 0), frame);
}
