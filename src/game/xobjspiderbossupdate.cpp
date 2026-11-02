#include "game/xobjspiderboss.h"
#include "game/xchr.h"
#include "game/cgame.h"

// todo: cleanup
static inline void SetPosAndTarget(CObjVec* vec, const Vec& value)
{
	vec->Set(value);
	vec->SetTarget(value);
}

/**
 * @note Address: 0x80105D58
 * @note Size: 0x2A8
 */
// vfA4, the update
extern "C" void vfA4__14XObjSpiderBossFUl(XObjSpiderBoss* spider, u32 frames)
{
	CPad pad(0); // retail does this
	if (lbl_8025C400.mStatus.mFlags & 0x400ULL) {
		Vec& move = lbl_8025C400.GetPlayer()->_158C4;
		move.x    = 0.0f;
		move.y    = 0.0f;
		move.z    = 0.0f;
		return;
	}
	if (spider->_258 == 0) {
		fn_800EA780(&lbl_8065B2E8, frames);
	}
	if (!(spider->mStatus.mFlags & 0x10000ULL) && spider->mState != 1) {
		return;
	}
	spider->vf148();
	fn_8010473C(spider, frames);
	if (!(spider->mSpiderStatus.mFlags & 0x02000000)) {
		(spider->*lbl_8022BB7C[spider->mState])(frames);
		CVec step;
		if (spider->mSpiderStatus.mFlags & 8) {
			spider->mMoveTranslate.y += spider->mSpeedY;
		} else {
			spider->mMoveTranslate.y = spider->mBasePos.y;
		}
		step.x = spider->mSpeed * (f32)sin(spider->mHeading);
		step.z = spider->mSpeed * (f32)cos(spider->mHeading);
		step.y = 0.0f;
		spider->SetRotateY(spider->mTargetRotY, spider->mRotSpeed);
		SetPosAndTarget(&spider->mMoveTranslate, spider->mMoveTranslate + step * (f32)frames);
		spider->SetPos(spider->mMoveTranslate, 0);
	}
	vfA4__4XObjFUl(spider, frames);
	fn_80103F54(spider, frames);
}
