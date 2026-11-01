#include "game/xobjspider_shared.h"

// lands: drops into its anim, then takes the first free one of the carried objects
void XObjSpider::State5(u32 frames)
{
	switch (mStep) {
	case 0:
		mStatus.mFlags |= 0x40;
		mStatus.mFlags |= 0x400;
		mStatus.mFlags |= 0x8000;
		SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		mStep++;
		break;
	case 1:
		if (GAME_5D080 == NULL) {
			mStatus.mFlags &= ~0x40;
			mStatus.mFlags &= ~0x80;
			mStatus.mFlags &= ~0x100;
			mStatus.mFlags &= ~0x8000;
			TakeCarried(this);
			SetState(4);
		}
		break;
	case 2:
		break;
	}
}

// NONMATCHING: the spider count loop isn't strength reduced, the robot position temp sits elsewhere
// hangs from the ceiling and swings, drops down its thread when the robot comes near,
// climbs back, then takes one of the carried objects and walks off
void XObjSpider::State6(u32 frames)
{
	CVec chr(GetChrPos());
	CPad pad(0);
	switch (mStep) {
	case 0:
		mTimer = rand() % 30;
		mStep  = 10;
		break;
	case 10:
		if (--mTimer < 0) {
			mTimer = rand() % 30;
			SetAnim(0x11, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mStep = 1;
		}
		break;
	case 1:
		mTimer++;
		mMoveTranslate.x = mHome.x + (f32)(8.0f * (f32)sin(DEG2RAD * (f32)((mTimer * 4) % 360))) - 4.0f;
		if (Get25C() == 11) {
			if (mStatus.mFlags & 0x200000) {
				fn_8001E904(GAME_SOUND, 0x131, 0xFF);
				mStatus.mFlags |= 0x100000;
				mStep++;
				SetAnim(0x10, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				mSpeed = -1.6f;
			}
		} else {
			f32 dist;
			if (mWall == 3) {
				f32 dx = chr.x - mMoveTranslate.x;
				f32 dy = chr.y - mMoveTranslate.y;
				f32 dz = 0.0f;
				dist   = sqrtf(dx * dx + dy * dy + dz * dz);
			} else {
				f32 dx = chr.x - mMoveTranslate.x;
				f32 dy = 0.0f;
				f32 dz = chr.z - mMoveTranslate.z;
				dist   = sqrtf(dx * dx + dy * dy + dz * dz);
			}
			if (dist <= 68.0f && GAME->CanAct() && GAME_5D080 == NULL) {
				s32 n = 0;
				for (int i = 0; i < 16; i++) {
					XObjSpider* other = (XObjSpider*)GAME_POOL2->Get(i);
					if ((other->mFlags & 1) && (other->mStatus.mFlags & 0x100000)) {
						n++;
					}
				}
				if (n < 10) {
					fn_8001E904(GAME_SOUND, 0x131, 0xFF);
					mStatus.mFlags |= 0x100000;
					mStep++;
					SetAnim(0x10, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					mSpeed = -1.6f;
				}
			}
		}
		break;
	case 2:
		if (mMoveTranslate.y < 20.0f + mHome.y) {
			fn_8001E660(GAME_SOUND, 0x131);
			mStep++;
			SetAnim(0x12, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mDir   = atan2(_9FC.x - mMoveTranslate.x, _9FC.z - mMoveTranslate.z);
			_988   = mDir;
			mSpeed = 0.0f;
		}
		break;
	case 3:
		f32 end = GetAnimEndFrame(mJObj);
		if (end - frames <= GetAnimFrame(mJObj)) {
			mStep++;
			SetAnim(0x13, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mDir   = atan2(_9FC.x - mMoveTranslate.x, _9FC.z - mMoveTranslate.z);
			_988   = mDir;
			mSpeed = -0.5f;
		}
		break;
	case 4:
		if (mMoveTranslate.y < 8.0f + mHome.y) {
			mStep++;
			SetAnim(0x14, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			fn_8001E904(GAME_SOUND, 0x128, 0xFF);
		}
		break;
	case 5:
		if (mMoveTranslate.y < mHome.y) {
			mMoveTranslate.y = mHome.y;
			mSpeed           = 0.0f;
		}
		f32 end2 = GetAnimEndFrame(mJObj);
		if (end2 - frames <= GetAnimFrame(mJObj)) {
			mStatus.mFlags &= ~0x40;
			mStatus.mFlags &= ~0x80;
			mStatus.mFlags &= ~0x100;
			SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mStatus.mFlags &= ~0x8000;
			TakeCarried(this);
			SetState(4);
		}
		break;
	case 6:
		if (pad.mPad->mTrigger & 0x200) {
			mStatus.mFlags = 0;
			mStatus.mFlags |= 0x40;
			mStatus.mFlags |= 0x80;
			mStatus.mFlags |= 0x100;
			mMoveTranslate.y = 48.0f + mHome.y;
			mStep            = 0;
		}
		break;
	}
}

// backs off to its home spot, its steps sounding, then waits out the event
void XObjSpider::State7(u32 frames)
{
	switch (mStep) {
	case 0:
		SetAnim(0xC, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		mSpeed = 0.0f;
		mStep++;
		break;
	case 1:
		f32 end = GetAnimEndFrame(mJObj);
		if (end - frames <= GetAnimFrame(mJObj)) {
			SetAnim(8, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			_9FC.x = mHome.x;
			_9FC.y = mMoveTranslate.y;
			_9FC.z = mHome.z;
			mDir   = PI + (f32)atan2(_9FC.x - mMoveTranslate.x, _9FC.z - mMoveTranslate.z);
			mSpeed = -0.5f;
			mStep++;
		}
		break;
	case 2:
		if (1.0f <= GetAnimFrame(mJObj) && 1.0f >= GetAnimFrame(mJObj)) {
			PlayStep(this);
		} else if (15.0f <= GetAnimFrame(mJObj) && 15.0f >= GetAnimFrame(mJObj)) {
			PlayStep(this);
		}
		if (DistXZ(&_9FC, &mMoveTranslate) <= 2.0f) {
			mSpeed   = 0.0f;
			f32 end2 = GetAnimEndFrame(mJObj);
			if (end2 - frames <= GetAnimFrame(mJObj)) {
				SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				if (GAME_5D080 != NULL) {
					mStep++;
				} else {
					SetState(4);
				}
			}
		}
		break;
	case 3:
		if (GAME_5D080 == NULL) {
			SetState(4);
		}
		break;
	}
}

void XObjSpider::State8(u32 frames)
{
}

extern "C" u32 fn_800EBDB0(XObjSpider* self)
{
	return (self->mStatus.mFlags >> 19) & 1;
}

// whether another spider is closer than 24
extern "C" BOOL fn_800EBBF0(XObjSpider* self)
{
	for (int i = 0; i < 16; i++) {
		XObj* obj = GAME_POOL2->Get(i);
		if ((obj->mFlags & 1) && (obj->mFlags & 1) && (obj->mFlags & 0x10000)) {
			XObjSpider* other = (XObjSpider*)obj;
			if ((other->mStatus.mFlags & 0x100000) && other->_258 != self->_258) {
				f32 dx = other->mMoveTranslate.x - self->mMoveTranslate.x;
				f32 dy = other->mMoveTranslate.y - self->mMoveTranslate.y;
				f32 dz = other->mMoveTranslate.z - self->mMoveTranslate.z;
				if (sqrtf(dx * dx + dy * dy + dz * dz) < 24.0f) {
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

// the id of another spider closer than 24, -1 when there is none
extern "C" s32 fn_800EBA30(XObjSpider* self)
{
	for (int i = 0; i < 16; i++) {
		XObj* obj = GAME_POOL2->Get(i);
		if ((obj->mFlags & 1) && (obj->mFlags & 1) && (obj->mFlags & 0x10000)) {
			XObjSpider* other = (XObjSpider*)obj;
			if ((other->mStatus.mFlags & 0x100000) && other->_258 != self->_258) {
				f32 dx = other->mMoveTranslate.x - self->mMoveTranslate.x;
				f32 dy = other->mMoveTranslate.y - self->mMoveTranslate.y;
				f32 dz = other->mMoveTranslate.z - self->mMoveTranslate.z;
				if (sqrtf(dx * dx + dy * dy + dz * dz) < 24.0f) {
					return other->_258;
				}
			}
		}
	}
	return -1;
}

extern "C" void fn_800EB928(XObjSpider* self)
{
	switch (self->_258 % 5) {
	case 0:
		fn_8001E904(GAME_SOUND, 0x12B, 0xFF);
		break;
	case 1:
		fn_8001E904(GAME_SOUND, 0x12C, 0xFF);
		break;
	case 2:
		fn_8001E904(GAME_SOUND, 0x12D, 0xFF);
		break;
	case 3:
		fn_8001E904(GAME_SOUND, 0x12E, 0xFF);
		break;
	case 4:
		fn_8001E904(GAME_SOUND, 0x12F, 0xFF);
		break;
	}
}

// NONMATCHING: retail has another 0x24 bytes of .rodata in front of the joint paths
// the part's anims: it opens, follows its joint on the carried object while the anim
// plays, then closes and hides
extern "C" void fn_800EAF18(UnkSpiderPart* part, u8 carry, u32 frames, s32 mode)
{
	if (carry == 0xFF) {
		part->mState = 0;
		part->mStatus.mFlags &= ~1;
		return;
	}
	if (lbl_8037E5C0[carry] == NULL) {
		part->mState = 0;
		part->mStatus.mFlags &= ~1;
		return;
	}
	switch (part->mState) {
	case 0:
		break;
	case 1:
		lbl_8037E5C0[carry]->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		part->mState++;
		break;
	case 2:
		if (part->mObj != NULL) {
			CJObj joint;
			s32 path[8] = { 0, 0, 0, 0, 1, 1, 0, 0 };
			joint.Attach(GetJoint(part->mObj->mJObj, path, 8));
			part->mPos = GetJObjPos(joint.mJObj);
		}
		f32 end = GetAnimEndFrame(lbl_8037E5C0[carry]->mJObj);
		if (end - frames <= GetAnimFrame(lbl_8037E5C0[carry]->mJObj)) {
			lbl_8037E5C0[carry]->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			part->mState++;
		}
		break;
	case 3:
		if (part->mObj != NULL) {
			CJObj joint;
			s32 path[8] = { 0, 0, 0, 0, 1, 1, 0, 0 };
			joint.Attach(GetJoint(part->mObj->mJObj, path, 8));
			part->mPos = GetJObjPos(joint.mJObj);
		}
		f32 end2 = GetAnimEndFrame(lbl_8037E5C0[carry]->mJObj);
		if (end2 - frames <= GetAnimFrame(lbl_8037E5C0[carry]->mJObj)) {
			lbl_8037E5C0[carry]->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			part->mState = 0;
			part->mStatus.mFlags &= ~1;
			part->mStatus.mFlags &= ~2;
		}
		break;
	}
}

// a test mode: the pad's direction sends it a step that way, up its thread and back
void XObjSpider::State9(u32 frames)
{
	CPad pad(0);
	if (pad.mPad->mTrigger & 0x20) {
		mStep            = 0;
		mSpeed           = 0.0f;
		mMoveTranslate.x = mHome.x;
		mMoveTranslate.y = mHome.y;
		mMoveTranslate.z = mHome.z;
		SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		mStatus.mFlags &= ~0x100;
	}
	switch (mStep) {
	case 0:
		if (pad.mPad->mTrigger & 0x200) {
			u32 button = pad.mPad->mButton;
			if (button & 8) {
				_9FC.x = mHome.x;
				_9FC.y = mHome.y;
				_9FC.z = mHome.z - 60.0f;
				SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				mStep++;
				mSpeed = 0.3f;
				mDir   = PI;
			} else if (button & 4) {
				_9FC.x = mHome.x;
				_9FC.y = mHome.y;
				_9FC.z = 60.0f + mHome.z;
				SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				mStep++;
				mSpeed = 0.3f;
				mDir   = PI;
			} else if (button & 1) {
				_9FC.x = mHome.x - 60.0f;
				_9FC.y = mHome.y;
				_9FC.z = mHome.z;
				SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				mStep++;
				mSpeed = 0.3f;
				mDir   = PI;
			} else if (button & 2) {
				_9FC.x = 60.0f + mHome.x;
				_9FC.y = mHome.y;
				_9FC.z = mHome.z;
				SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				mStep++;
				mSpeed = 0.3f;
				mDir   = PI;
			}
		}
		break;
	case 1:
		if (HeadTo(this, _9FC) < 20.0f) {
			mStep++;
			SetAnim(4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
		break;
	case 2:
		if (HeadTo(this, _9FC) < 1.0f) {
			mStatus.mFlags |= 0x100;
			mSpeed = 0.0f;
		}
		HSD_AObj* aobj = GetAObj(mJObj);
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		aobj           = GetAObj(mJObj);
		if (end - frames <= (aobj != NULL ? HSD_AObjGetCurrFrame(aobj) : 0.0f)) {
			mStatus.mFlags |= 0x100;
			mSpeed = 0.0f;
			mStep++;
			SetAnim(5, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mSpeed = 0.3f;
		}
		break;
	case 3:
		if (mMoveTranslate.y >= 80.0f) {
			mStep++;
			mSpeed = -0.3f;
		}
		break;
	case 4:
		if (mMoveTranslate.y <= 20.0f + mHome.y) {
			mStep++;
			mMoveTranslate.y = 0.0f;
			mStatus.mFlags &= ~0x100;
			_9FC.x = mHome.x;
			_9FC.y = mHome.y;
			_9FC.z = mHome.z;
			SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			mSpeed = 0.3f;
		}
		break;
	case 5: {
		f32 distance = HeadTo(this, _9FC);
		s32 done     = 0;
		if (distance < 1.0f) {
			mSpeed = 0.0f;
			done   = 1;
		}
		f32 end2 = GetAnimEndFrame(mJObj);
		if (end2 - frames <= GetAnimFrame(mJObj)) {
			done++;
		}
		if (done >= 2) {
			mStep = 0;
			SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
		break;
	}
	}
}
