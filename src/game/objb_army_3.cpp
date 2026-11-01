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
 * @note Address: 0x800E9B7C
 * @note Size: 0x600
 */
void XObjArmy::Update(u32 frames)
{
	fn_80090ED4(this, frames);
	if (mState) {
		(this->*mState)();
	}
	switch (mAnimId) {
	case 4:
		if (!(mStatus.mFlags & 2) && 20.0f <= GetAnimFrame(mJObj)) {
			SetPoints(this, sPoints[2]);
			mStatus.mFlags |= 2;
		}
		if (50.0f < GetAnimFrame(mJObj)) {
			SetMoveDir(&mMoveTranslate, 1.0f);
		}
		break;
	case 5:
	case 6:
		if (!(mStatus.mFlags & 2) && 15.0f <= GetAnimFrame(mJObj)) {
			SetPoints(this, sPoints[2]);
			mStatus.mFlags |= 2;
		}
		if (50.0f < GetAnimFrame(mJObj)) {
			SetMoveDir(&mMoveTranslate, 1.0f);
		}
		break;
	case 10:
		if (!(mStatus.mFlags & 2) && GetAnimEndFrame(mJObj) <= GetAnimFrame(mJObj)) {
			SetPoints(this, sPoints[1]);
			mStatus.mFlags |= 2;
		}
		break;
	case 17:
		if (!(mStatus.mFlags & 2)) {
			SetPoints(this, sPoints[1]);
			mStatus.mFlags |= 2;
		}
		break;
	case 18:
		if (!(mStatus.mFlags & 2)) {
			SetPoints(this, sPoints[1]);
			mStatus.mFlags |= 2;
		}
		break;
	default:
		if (mStatus.mFlags & 2) {
			SetPoints(this, sPoints[0]);
			mStatus.mFlags &= ~2;
		}
		break;
	}
	vf148();
	for (u32 i = 0; i < 5; i++) {
		mShots[i].Update(frames);
	}
}
