#include "game/objb_army_shared.h"

// Initialize a shot from the soldier's position and firing direction.
static inline void FireShot(ArmyShot* shot, const CVec& pos, const CVec& dir, u8 quiet)
{
	shot->mActive = 1;
	shot->mLife   = 120;
	shot->mPos    = pos;
	shot->mPos.y += 6.0f;
	shot->mVel = dir;
	PSVECNormalize(&shot->mVel, &shot->mVel);
	shot->mSpeed = 3.0f;
	shot->mPhase = 0;
	if (!quiet) {
		if (sB2A4 < 0 || sB2A4 >= 4) {
			sB2A4 = 0;
		}
		fn_8001E904(GAME_SOUND, sB2A4 + 0x110012, 0xFF);
		sB2A4++;
	}
}

/**
 * @note Address: 0x800E77F4
 * @note Size: 0x5E8
 */
void XObjArmy::State77F4()
{
	switch (mStep) {
	case 0:
		SetAnim(9, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		SetRotateY(GetAngleY(mGuardPos, mMoveTranslate), 1.0f);
		mStep++;
		break;
	case 1:
		if (20.0f <= GetAnimFrame(mJObj)) {
			u32 i;
			for (i = 0; mShots[i].mActive && i < 5; i++) { }
			if (i < 5) {
				FireShot(&mShots[i], mMoveTranslate, mGuardPos - mMoveTranslate, (mStatus.mFlags & 0x80) != 0);
			}
			mStep++;
		}
		break;
	case 2:
		if (GetAnimEndFrame(mJObj) <= 2.0f + GetAnimFrame(mJObj)) {
			SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			SetMode(0);
		}
		break;
	}
}

/**
 * @note Address: 0x800E7DDC
 * @note Size: 0x3C4
 */
void XObjArmy::State7DDC()
{
	CVec pos;
	switch (mStep) {
	case 0: {
		mStatus.mFlags |= 1;
		sB2A8++;
		f32 angle = GetAngleY(mMoveTranslate, GAME_PLAYER->mMoveTranslate);
		pos       = mMoveTranslate;
		SetAnim(0x13, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		SetRotateY(angle - 3.1415927f, 1.0f);
		SetMoveDir(&pos, 0.5f);
		mStep++;
		break;
	}
	case 1:
		if (GetAnimEndFrame(mJObj) <= GetAnimFrame(mJObj)) {
			SetMoveDir(&mMoveTranslate, 1.0f);
			mStep++;
		}
		break;
	}
}

/**
 * @note Address: 0x800E81A0
 * @note Size: 0x3C4
 */
void XObjArmy::State81A0()
{
	CVec pos;
	switch (mStep) {
	case 0: {
		mStatus.mFlags |= 1;
		sB2A8++;
		f32 angle = GetAngleY(mMoveTranslate, GAME_PLAYER->mMoveTranslate);
		pos       = mMoveTranslate;
		SetAnim(0xA, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		SetRotateY(angle - 3.1415927f, 1.0f);
		SetMoveDir(&pos, 0.1f);
		mStep++;
		break;
	}
	case 1:
		if (GetAnimEndFrame(mJObj) <= GetAnimFrame(mJObj)) {
			SetMoveDir(&mMoveTranslate, 0.0f);
			mStep++;
		}
		break;
	}
}
