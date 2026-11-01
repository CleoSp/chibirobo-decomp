#include "game/xobjspider_shared.h"

// dies: blinks, stops its sound, drops what it carries, shakes the screen, then
// leaves one of the drops where it was
void XObjSpider::State3(u32 frames)
{
	mTimer++;
	if (mTimer % 30 < 15) {
		SetColor(1, 0, 0x80, 0, 0.0f);
	} else {
		SetColor(1, 0, 0, 0, 0.0f);
	}
	Vec pos;
	switch (mStep) {
	case 0:
		fn_8001E660(GAME_SOUND, 0x132);
		SetAnim(0x19, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		if (mCarry != 0xFF) {
			if (mPart2.mObj != NULL) {
				mPart2.mObj->mFlags &= ~0x10000ULL;
			}
			lbl_8037E600[mCarry] = -1;
			mPart2.mObj          = NULL;
			mCarry               = 0xFF;
		}
		mWait = 0x2C;
		mStep++;
		break;
	case 1:
		mWait -= frames;
		if (mWait < 0) {
			fn_8010CDDC(mMoveTranslate.x, mMoveTranslate.y, mMoveTranslate.z);
			if (!(GAME->mFlags & 0x100)) {
				fn_800242A8(GAME_QUAKE, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.4f);
			}
			mStep++;
		}
		break;
	case 2:
		f32 end = GetAnimEndFrame(mJObj);
		if (end - frames <= GetAnimFrame(mJObj)) {
			mStep++;
			mTimer = 0;
			mFlags &= ~0x10000ULL;
			mStatus.mFlags |= 0x80000;
			mStatus.mFlags &= ~0x100000;
			pos.x = mMoveTranslate.x;
			pos.y = mMoveTranslate.y;
			pos.z = mMoveTranslate.z;
			s32 anim;
			if (rand() % 2 > 0) {
				anim = 2;
			} else if (rand() % 4 > 1) {
				anim = 1;
			} else {
				anim = 0;
			}
			Vec to;
			to.x = mHome.x;
			to.y = 0.0f;
			to.z = mHome.z;
			fn_800EA554(&pos, &to, anim);
		}
		break;
	case 3:
		break;
	}
}
extern "C" void fn_800EA554(Vec* pos, Vec* to, s32 anim);
