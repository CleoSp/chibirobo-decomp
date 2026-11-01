#include "game/xobjleticker.h"
#include "game/ccameraaction.h"
#include "game/cgame.h"
#include "game/cparticle.h"
#include "game/cmotioncontrolcamera.h"
#include "game/gmath.h"

/**
 * @note Address: 0x800E2EAC
 * @note Size: 0x1A4
 */
extern "C" void fn_800E2EAC(XObjLeticker* leticker, Vec* motion)
{
	for (s32 i = 0; i < leticker->mLinkedCount; ++i) {
		CXObj* linked = lbl_8025C400.GetObject(leticker->mLinked[i]);
		linked->SetTranslateX(leticker->mMoveTranslate.x, 0);
		linked->SetTranslateZ(leticker->mMoveTranslate.z, 0);
	}
	if (0.0f < PSVECMag(motion)) {
		leticker->vf12C(1, 0, 0, 0, 0.5f);
		leticker->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	} else {
		leticker->vf12C(1, 255, 0, 0, 1.0f);
		leticker->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}
	if (leticker->mPtcl) {
		((CParticle*)lbl_8025C400.mParticle)->Kill(165);
		leticker->mPtcl = 0;
	}
}

/**
 * @note Address: 0x800E3050
 * @note Size: 0x1DC
 */
extern "C" void fn_800E3050(XObjLeticker* leticker)
{
	if (leticker->mState != 3) {
		lbl_8025C400.mStatus.mFlags &= ~0x80ULL;
		((CMotionControlCamera*)lbl_8025C400.mCamera)->PopAction();
	}
	leticker->_A2D = 0;
	leticker->_A30 = 0;
	if (leticker->mState == 2) {
		f32 heading = leticker->mHeading;
		s32 dir;
		if ((f32)cos(DEG2RAD * 45.0f) <= (f32)cos(heading)) {
			dir = 0;
		} else if ((f32)cos(heading) <= (f32)cos(DEG2RAD * 225.0f)) {
			dir = 2;
		} else if (0.0f < (f32)sin(heading)) {
			dir = 1;
		} else {
			dir = 3;
		}
		leticker->_9A0 = leticker->_9A4[dir];
		leticker->_9FC = 10.0f * (f32)leticker->_9A0;
		leticker->_9F8 = 10.0f * (f32)leticker->_99C;
	}
	leticker->vf12C(1, 0, 0, 0, 0.5f);
	leticker->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}
