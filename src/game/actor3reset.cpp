#include "game/xobjactor.h"

extern "C" {
extern AnimSfxEvent gHaisuiNezumiAnimSfxEvents[];
extern s32 gSpiderBossVolleySlot;
extern s32 gActor3Kind15ActivationCounter;
void ResetActor3Fields(XObjActor3* actor);
}

/**
 * @note Address: 0x8010AD00
 * @note Size: 0x20
 */
extern "C" void vf94__10XObjActor3Fv(XObjActor3* actor)
{
	ResetActor3Fields(actor);
}

/**
 * @note Address: 0x8010AD20
 * @note Size: 0x34
 */
extern "C" void ResetObjectState__10XObjActor3Fv(XObjActor3* actor)
{
	ResetObjectState__4XObjFv(actor);
	ResetActor3Fields(actor);
}

/**
 * @note Address: 0x8010AD54
 * @note Size: 0x34
 */
extern "C" void InitializeObject__10XObjActor3FUl(XObjActor3* actor, u32 id)
{
	InitializeObject__4XObjFUl(actor, id);
	ResetActor3Fields(actor);
}

/**
 * @note Address: 0x8010AD88
 * @note Size: 0x1EC
 */
extern "C" void ResetActor3Fields(XObjActor3* actor)
{
	actor->_B70        = 0.0f;
	actor->_B74        = 0.0f;
	actor->_B78        = 0.0f;
	actor->_B7C        = 0.0f;
	actor->_B80        = 0;
	actor->_B88        = 0;
	actor->_B8C.mFlags = 0;
	actor->_B8C.mFlags |= 4;
	actor->mEventCommand = 0;
	actor->mEventArg1    = 0;
	actor->mEventArg2    = 0;
	actor->mEventArg3    = 0;
	actor->_BA4[0]       = 0;
	actor->_BA4[1]       = 0;
	actor->_BA4[2]       = 0;
	actor->_BA4[3]       = 0;
	actor->_BA4[4]       = 0;
	actor->_BA4[5]       = 0;
	actor->_BA4[6]       = 0;
	actor->_BC0          = 0.0f;
	actor->_BC4          = 0.0f;
	actor->_BC8          = 0.0f;
	Vec zero             = { 0.0f, 0.0f, 0.0f };
	actor->_BCC          = zero.x;
	actor->_BD0          = zero.y;
	actor->_BD4          = zero.z;
	for (int i = 0; i < 8; i++) {
		actor->mParts[i].Clear();
	}
	if (actor->mPoolIndex == 0) {
		gSpiderBossVolleySlot          = 0;
		gActor3Kind15ActivationCounter = 0;
	}
	actor->vf12C(1, 0, 0, 0, 0.0f);
	actor->_BDC.x             = 0.0f;
	actor->_BDC.y             = 0.0f;
	actor->_BDC.z             = 0.0f;
	actor->_BEC               = 0;
	actor->mTimerPanelAlpha   = 0.0f;
	actor->mCountdownFrames   = 0.0f;
	actor->mCountdownRunning  = 0;
	actor->mCounterFadeMode   = 0;
	actor->mCounterPanelAlpha = 0.0f;
	actor->mSpiderCount       = 0;
	actor->_C08               = 0;
	if (actor->GetKind() == 0x31B) {
		InitParticlePool(&actor->mDust.mPtcl, actor->mDust.mPtcls, 0x50, 0x32);
		actor->mAnimSfx.mEvents = gHaisuiNezumiAnimSfxEvents;
	}
	actor->_1DC0[0]     = 0;
	actor->_1DC0[1]     = 0;
	actor->_1DC0[2]     = 0;
	actor->_1DC0[3]     = 0;
	actor->_1DC0[4]     = 0;
	actor->_1DC0[5]     = 0;
	actor->mRumble      = 0;
	actor->mRumbleTimer = 0;
	actor->mHeld        = NULL;
	actor->_1DE4        = 0;
	actor->mSe          = -1;
}
