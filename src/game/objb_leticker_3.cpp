#include "game/objb_leticker_shared.h"

/**
 * @note Address: 0x800E54E0
 * @note Size: 0x1A8
 */
void XObjLeticker::StateRide()
{
	XObj* player   = GAME_PLAYER;
	s32 path[2]    = { 0, 9 };
	HSD_JObj* jobj = FindJoint(mJObj, path, 2);
	switch (mStep) {
	case 0:
		break;
	case 10:
		Vec pos;
		pos.x = pos.y = pos.z = 0.0f;
		pos.y                 = 0.5f;
		PSMTXMultVec(HSD_JObjGetMtxPtr(jobj), &pos, &pos);
		player->SetMoveTranslate(&pos, 0);
		player->SetRotationY(player->mMoveRotate.y - 0.5235988f, 0);
		break;
	}
}

/**
 * @note Address: 0x800E5688
 * @note Size: 0x410
 */
void XObjLeticker::StateCount()
{
	XObj* part;
	s32 count = (s32)_9F8 / 10;
	if (_9A0 < count) {
		count = 10;
	}
	part = GAME_OBJTABLE[mParts[0]];
	if (_99C != count) {
		if (GetAnimEndFrame(part->mJObj) <= GetAnimFrame(part->mJObj)) {
			if (_99C < count) {
				_99C++;
				f32 rate = 1.0f;
				if (!_A2D) {
					rate = 8.0f;
				}
				part->SetAnim(_99C, FALSE, NULL, NULL, NULL, 0.0f, rate, 0.0f, 10.0f);
			} else if (_A2D) {
				_99C--;
				part->SetAnim(24 - _99C, FALSE, NULL, NULL, NULL, 0.0f, 2.0f, 0.0f, 10.0f);
				if (_99C == 0) {
					PushMsg(part, 0, 0, 100);
				}
			} else {
				_9F8 = (_99C - 1) * 10;
			}
			if (_A2D) {
				fn_8001E904(GAME_SOUND, 0xAC, 0xFF);
				HSD_AudioSFXSetPitchByID(0xAC, 600.0f * (_A34 - 1.0f));
			}
		}
		part->mPoints       = lbl_8022A008[_99C];
		part->mPointMaskOld = 0;
		part->mPointMask    = 0;
		fn_8008D220(part);
		fn_8008C8A4(part);
	}
	if (!(mFlags & 0x0800000000000000ULL)) {
		SetColor(1, 0, 0, 0, 0.5f);
		SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}
}

/**
 * @note Address: 0x800E5A98
 * @note Size: 0x3E0
 */
void XObjLeticker::StateLift()
{
	XObj* base = GAME_OBJTABLE[mParts[0]];
	XObj* top  = GAME_OBJTABLE[mParts[1]];
	if (base == NULL || top == NULL) {
		return;
	}
	f32 height = 4.0f * (s32)(_9F8 / 3.0f) + 0.5f;
	if (_A00 == height) {
		_A24 = TRUE;
	} else if (_A00 < height) {
		_A00 += 0.5f;
		if (height < _A00) {
			_A00 = height;
		}
	} else if (height < _A00 && 0.0f < _A00) {
		_A00 -= 0.5f;
		if (_A00 < 0.0f) {
			_A00 = 0.0f;
		}
	}
	if (_A00 <= 2.5f) {
		_A00 = 2.5f;
	}
	s32 step = _A00 / 4.0f;
	if (_99C != step) {
		if (!fn_8001E6C8(GAME_SOUND, 0xA8)) {
			fn_8001E904(GAME_SOUND, 0xA8, 0xFF);
		}
		HSD_AudioSFXSetPitchByID(0xA8, 600.0f * (_A34 - 1.0f));
		_99C = step;
		_A24 = FALSE;
	}
	top->SetTranslateY(9.0f + (_A00 + HSD_JObjGetTranslationY(mJObj)), 0);
	_A04.x = top->mMoveTranslate.x;
	_A04.y = top->mMoveTranslate.y;
	_A04.z = top->mMoveTranslate.z;
	if (mFlags & 0x0800000000000000ULL) {
		f32 h;
		if (_A00 < 10.0f) {
			h = 19.0f;
		} else {
			h = 5.0f + (9.0f + _A00) - 2.0f;
		}
		lbl_8065A6D0[0]->mPos2[1] = (s32)h * 100;
		lbl_8065A6D0[0]->mPos[1]  = (s32)(h / sqrtf(3.0f)) * 100;
		mPoints                   = lbl_8065A6D0[0];
		mPointMaskOld             = 0;
		mPointMask                = 0;
		fn_8008D220(this);
		fn_8008C8A4(this);
	} else {
		s32 k               = (_A00 - 12.5f) / 4.0f;
		mBox[0].mPos[1]     = k * 0xA9 + 0x654;
		mBox[0].mPos2[1]    = k * 400 + 1000;
		base->mPoints       = mBox;
		base->mPointMaskOld = 0;
		base->mPointMask    = 0;
		fn_8008C8A4(base);
		mPoints       = lbl_8065A6D0[1];
		mPointMaskOld = 0;
		mPointMask    = 0;
		fn_8008D220(this);
		fn_8008C8A4(this);
		SetColor(1, 0, 0, 0, 0.5f);
		SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}
}

static inline void PlayLetickerSound(XObjLeticker& obj)
{
	s32 anim  = obj.mAnimId;
	f32 frame = GetAnimFrame(obj.mJObj);
	obj.mSfx.Play(anim, fn_8001DF00(GAME_SOUND, &obj.mMoveTranslate, 0), frame);
}

/**
 * @note Address: 0x800E5E78
 * @note Size: 0x598
 */
void XObjLeticker::Update(u32 frames)
{
	if (mAnimId == 1) {
		if (GetAnimEndFrame(mJObj) <= frames + GetAnimFrame(mJObj)) {
			SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
	} else if (mAnimId == 3) {
		if (GetAnimEndFrame(mJObj) <= frames + GetAnimFrame(mJObj)) {
			SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
	}
	fn_80090ED4(this, frames);
	mStateTime += frames;
	if (mState) {
		(this->*mState)();
	}
	s32 path[3]    = { 0, 0, 0 };
	HSD_JObj* jobj = FindJoint(mJObj, path, 2);
	XObj* player   = GAME_PLAYER;
	f32 angle      = atan2(player->mMoveTranslate.x - mMoveTranslate.x, player->mMoveTranslate.z - mMoveTranslate.z);
	HSD_JObjSetRotationY(jobj, angle - HSD_JObjGetRotationY(mJObj));
	PlayLetickerSound(*this);
}
