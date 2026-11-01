#include "game/xobjspider_shared.h"

void XObjSpider::Update(u32 frames)
{
	CPad pad(0);

	switch (mRumble) {
	case 0:
		break;
	case 1:
		if (GAME->mFlags & 0x800000000ULL) {
			GAME->StartRumble();
			mRumbleTimer = 24 / frames;
			mRumble++;
		} else {
			mRumble = 0;
		}
		break;
	case 2:
		if (--mRumbleTimer < 0) {
			UnkSpiderGame* game = GAME;
			game->StopRumble();
			mRumble = 0;
		}
		break;
	}

	if (GAME->mFlags & 0x400) {
		return;
	}
	if (_258 == 0) {
		fn_800EA780(&lbl_8065B2B8, frames);
	}
	if (!(mFlags & 0x10000) && mState != 1) {
		return;
	}
	vf148();
	fn_800F1B10(this, frames);
	if (!(mStatus.mFlags & 0x8000000)) {
		(this->*lbl_8022AC50[mState])(frames);
		CVec v;
		if (mWall == 4) {
			mMoveTranslate.x = mWallPos.x;
			v.y              = mSpeed * (f32)sin(mDir);
			v.z              = mSpeed * (f32)cos(mDir);
			v.x              = 0.0f;
			mMoveRotate.z    = -1.5707964f;
			SetRotateY(mDir, mTurnSpeed);
		} else if (mWall == 5) {
			mMoveTranslate.x = mWallPos.x;
			v.y              = mSpeed * (f32)sin(mDir);
			v.z              = mSpeed * (f32)cos(mDir);
			v.x              = 0.0f;
			mMoveRotate.z    = 1.5707964f;
			SetRotateY(mDir, mTurnSpeed);
		} else if (mWall == 2) {
			mMoveTranslate.z = mWallPos.z;
			v.y              = mSpeed * (f32)sin(mDir);
			v.x              = mSpeed * (f32)cos(mDir);
			v.z              = 0.0f;
			mMoveRotate.x    = -1.5707964f;
			mMoveRotate.z    = mDir - 1.5707964f;
		} else if (mWall == 3) {
			mMoveTranslate.z = mWallPos.z;
			v.y              = mSpeed * (f32)sin(mDir);
			v.x              = mSpeed * (f32)cos(mDir);
			v.z              = 0.0f;
			mMoveRotate.x    = 1.5707964f;
			mMoveRotate.z    = 1.5707964f + mDir;
		} else if (mStatus.mFlags & 0x100) {
			v.x = 0.0f;
			v.z = 0.0f;
			v.y = mSpeed;
			SetRotateY(mDir, mTurnSpeed);
		} else {
			if (mStatus.mFlags & 8) {
				mMoveTranslate.y += mFallSpeed;
			} else {
				mMoveTranslate.y = mHome.y;
			}
			v.x = mSpeed * (f32)sin(mDir);
			v.z = mSpeed * (f32)cos(mDir);
			v.y = 0.0f;
			SetRotateY(mDir, mTurnSpeed);
		}
		SetNow(mMoveTranslate, mMoveTranslate + v * (f32)frames);
		SetMoveTranslate(&mMoveTranslate, 0);
	}
	fn_80090ED4(this, frames);
	fn_800EAF18(&mPart, mCarry, frames, 0);
	fn_800F1068(this, frames);
}
extern "C" void fn_800F1068(XObjSpider* self, u32 frames);
extern "C" void fn_800F1B10(XObjSpider* self, u32 frames);
extern "C" void fn_800EAF18(UnkSpiderPart* part, u8 carry, u32 frames, s32 mode);
extern "C" void fn_800EA780(void* mgr, u32 frames);
