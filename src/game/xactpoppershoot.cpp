#include "game/xact.h"
#include "game/cparticle.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/globals.h"

extern "C" void fn_800C21A0(XActPopperShot* shot);

/**
 * @note Address: 0x800C2A14
 * @note Size: 0x304
 */
// fires a shot, kind 1 is the small one
extern "C" void fn_800C2A14(XActPopperShot* shot, const Vec* pos, s32 kind, void* attachment, f32 first, f32 second, f32 third, f32 fourth,
                            f32 fifth, f32 strength)
{
	shot->mActive = TRUE;
	shot->mState  = 0;
	shot->mPos    = CVec(*pos);
	shot->_14     = first;
	shot->_18     = second;
	shot->_1C     = fourth;
	shot->mPhase  = 0.0f;
	shot->_24     = fifth;
	shot->_28     = third;
	for (u32 i = 0; i < 10; ++i) {
		shot->mHistory[i].x = shot->mPos.x;
		shot->mHistory[i].y = shot->mPos.y;
		shot->mHistory[i].z = shot->mPos.z;
	}
	shot->mRing[0].mPos.x  = pos->x;
	shot->mRing[0].mPos.y  = pos->y;
	shot->mRing[0].mPos.z  = pos->z;
	shot->mRing[0].mAmount = 0.9f;
	shot->mRing[0].mRadius = 100.0f;
	shot->mRing[0].mColor  = 0x00FF0096;
	shot->mStrength        = strength;
	shot->mKind            = kind;
	if (attachment != NULL)
		shot->mAttachment = attachment;
	else
		shot->mAttachment = NULL;

	CParticle* particles = (CParticle*)lbl_8025C400.mParticle;
	shot->mGen           = particles->Create((kind == 1 ? 0x4E : 0x4F) | 0x10000, 0, shot->mJoint, TRUE);
	if (strength >= (s32)fn_8006EBC8(31) / 100.0f) {
		shot->mChargeGen = particles->Create(0x10050, 0, shot->mJoint, TRUE);
	}
	fn_8001E904((CSound*)&lbl_8025C400.mSound,
	            strength >= (s32)fn_8006EBC8(31) / 100.0f ? (kind == 1 ? 0x193 : 0x196) : (kind == 1 ? 0xB : 0x195), 255);
	fn_800C21A0(shot);
}
