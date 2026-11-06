#include "game/xobjspidersmall.h"

extern "C" {
void ResetSpiderCoinBounces(void*);
extern u8 gSpiderCoinBounceManager[4];
extern s32 gSpiderDrainFlashTimers[2];
extern const Vec sSmallSpiderVectorTemplates[] = { { 1, 1, 1 }, {}, {}, { 1, 1, 1 }, {}, {} };
}

#pragma section RX ".text.spiderresetstate"

/**
 * @note Address: 0x800FFB00
 * @note Size: 0x2E0
 */
extern "C" __declspec(section ".text.spiderresetstate") void ResetSmallSpiderState(XObjSpiderSmall* spider)
{
	spider->_980                 = 0.0f;
	spider->_984                 = 0.0f;
	spider->_988                 = 0.0f;
	spider->_98C                 = 0.1f;
	spider->_99C                 = 0.0f;
	spider->_9A0                 = 0.0f;
	spider->_9A4                 = 0.0f;
	spider->mState               = 0;
	spider->mPrevState           = 0;
	spider->mStep                = 0;
	spider->mTimer               = 0;
	spider->mSpiderStatus.mFlags = 0;
	spider->_9C0                 = 0;
	spider->_9C4                 = 1.0f;
	spider->_9C8                 = 1.0f;
	Vec v0                       = { 0.0f, 0.0f, 0.0f };
	spider->mHitPos              = v0;
	Vec v1                       = { 0.0f, 0.0f, 0.0f };
	spider->mHome                = v1;
	Vec v2                       = { 0.0f, 0.0f, 0.0f };
	spider->_9F0                 = v2;
	Vec v3                       = { 0.0f, 0.0f, 0.0f };
	spider->_9FC                 = v3;
	spider->mHitStrength         = 0.0f;
	spider->_9EC                 = 3;
	spider->_9E8                 = spider->_9EC;
	spider->mEvtArgs[0]          = 0;
	spider->mEvtArgs[1]          = 0;
	spider->_A48                 = 0;
	spider->_994                 = 1.0f;
	spider->mStatus.mFlags |= 8;
	spider->mStatus.mFlags |= 0x10000000;
	spider->mFx.mPos.x         = 0.0f;
	spider->mFx.mPos.y         = 0.0f;
	spider->mFx.mPos.z         = 0.0f;
	spider->mFx._10.x          = 0.0f;
	spider->mFx._10.y          = 0.0f;
	spider->mFx._10.z          = 0.0f;
	spider->mFx.mRotY          = 0.0f;
	spider->mFx._24            = 0.0f;
	spider->mFx.mState         = 0;
	spider->mFx._2C            = 0;
	spider->mFx.mStatus.mFlags = 0;
	spider->mRadius            = 16.0f;
	if (spider->mPoolIndex == 0) {
		ResetSpiderCoinBounces(gSpiderCoinBounceManager);
		gSpiderDrainFlashTimers[0] = 0;
		gSpiderDrainFlashTimers[1] = 0;
		gSpiderDrainFlashClaimed   = 0;
		gAttachedSmallSpiderCount  = 0;
	}
	spider->mFxObj        = NULL;
	spider->mFxIndex      = 0xFF;
	spider->_A8C          = 0.0f;
	spider->_A90          = 0.0f;
	spider->_A94          = 0;
	spider->_A98          = 0.0f;
	Vec v4                = { 0.0f, 0.0f, 0.0f };
	spider->_A9C          = v4;
	Vec v5                = { 0.0f, 0.0f, 0.0f };
	spider->mAreaPos      = v5;
	spider->mAreaWidth    = 0.0f;
	spider->mAreaDepth    = 0.0f;
	Vec v6                = { 0.0f, 0.0f, 0.0f };
	spider->mTargetPos    = v6;
	spider->mTargetRadius = 0.0f;
	spider->vf12C(1, 0, 0, 0, 0.0f);
	spider->mAreaGraceTimer = 0;
	spider->_AC0            = 0;
	spider->_AC4            = 0;
	spider->_AA8            = 0;
}
