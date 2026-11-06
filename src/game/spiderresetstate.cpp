#include "game/xobjspider.h"
#include "game/cgame.h"
#include "game/xobjflags.h"

extern "C" {
extern CoinBounce gSpiderCoinBounces[11];
extern s32 gSpiderCoinBounceCount;
extern const Vec sSpiderVectorTemplates[] = { { 1, 1, 1 }, {}, {}, { 1, 1, 1 }, {}, {} };
}

#pragma section RX ".text.spiderresetstate"

// takes all the coins back
static inline void ResetCoins()
{
	gSpiderCoinBounceCount = 0;
	for (int i = 0; i < 11; i++) {
		gSpiderCoinBounces[i].mCoin  = NULL;
		gSpiderCoinBounces[i].mPhase = 0;
		gSpiderCoinBounces[i].mCount = 0;
	}
	for (int i = 0; i < 96; i++) {
		XObjCoin* coin = (XObjCoin*)gGame.mXObjs.mPools[7].Get(i);
		if (coin->GetKind() == 0x104) {
			coin->mBouncing = 0;
		}
	}
}

/**
 * @note Address: 0x800F4350
 * @note Size: 0x4C8
 */
// puts everything back to how it starts
extern "C" __declspec(section ".text.spiderresetstate") void ResetSpiderState(XObjSpider* spider)
{
	spider->mDir          = 0.0f;
	spider->mSpeed        = 0.0f;
	spider->_988          = 0.0f;
	spider->_98C          = 0.0f;
	spider->mTurnSpeed    = 0.1f;
	spider->mFallSpeed    = 0.0f;
	spider->mState        = 0;
	spider->mPrevState    = 0;
	spider->mPhase        = 0;
	spider->mCounter      = 0;
	spider->mFlags.mFlags = 0;
	spider->mWait         = 0;
	spider->_9C0          = 1.0f;
	spider->_9C4          = 1.0f;
	Vec hitPos            = { 0.0f, 0.0f, 0.0f };
	spider->mHitPos       = hitPos;
	Vec home              = { 0.0f, 0.0f, 0.0f };
	spider->mHome         = home;
	Vec wallPos           = { 0.0f, 0.0f, 0.0f };
	spider->mWallPos      = wallPos;
	Vec target            = { 0.0f, 0.0f, 0.0f };
	spider->mTarget       = target;
	Vec v08               = { 0.0f, 0.0f, 0.0f };
	spider->mShotOrigin   = v08;
	Vec v14               = { 0.0f, 0.0f, 0.0f };
	spider->mShotPosition = v14;
	spider->mShotRange    = 0.0f;
	spider->mShotProgress = 0.0f;
	spider->mShotState    = 0;
	spider->_A48          = 0;
	spider->mHitStrength  = 0.0f;
	spider->mWall         = 0;
	spider->mMaxHits      = 3;
	spider->mHits         = spider->mMaxHits;
	spider->mCommand      = 0;
	spider->mArgs[0]      = 0;
	spider->_A88          = 0;
	spider->mThreadAlpha  = 1.0f;
	spider->mStatus.mFlags |= 8;
	spider->mCarry.mPos.x        = 0.0f;
	spider->mCarry.mPos.y        = 0.0f;
	spider->mCarry.mPos.z        = 0.0f;
	spider->mCarry._10.x         = 0.0f;
	spider->mCarry._10.y         = 0.0f;
	spider->mCarry._10.z         = 0.0f;
	spider->mCarry.mAngle        = 0.0f;
	spider->mCarry._24           = 0.0f;
	spider->mCarry.mState        = 0;
	spider->mCarry._2C           = 0;
	spider->mCarry.mFlags.mFlags = 0;
	spider->mCarry.mObj          = NULL;
	spider->mRadius              = 20.0f;
	spider->_AC8.mObj            = NULL;
	spider->mSlot                = 255;
	Vec areaPos                  = { 0.0f, 0.0f, 0.0f };
	spider->mAreaPos             = areaPos;
	spider->mAreaWidth           = 0.0f;
	spider->mAreaDepth           = 0.0f;
	Vec startPos                 = { 0.0f, 0.0f, 0.0f };
	spider->mStartPos            = startPos;
	spider->_A6C                 = 0.0f;
	spider->_B10                 = 2;
	spider->_B14[0]              = 0;
	spider->_B14[1]              = 0;
	spider->_B14[2]              = 0;
	spider->_B14[3]              = 0;
	spider->_B24[0]              = 100;
	spider->_B24[1]              = 100;
	spider->_B24[2]              = 100;
	spider->_B54                 = 0;
	spider->_B50                 = 0;
	spider->mEffect              = NULL;
	spider->mEffectId            = 0;
	spider->mEffectOn            = 0;
	for (int i = 0; i < 8; i++) {
		spider->mTies[i].mObj   = NULL;
		spider->mTies[i].mJoint = NULL;
	}
	if (spider->mPoolIndex == 0) {
		ResetCoins();
	}
	spider->mHitState    = 0;
	spider->mHitTimer    = 0;
	spider->_B9C         = -1;
	spider->mRumble      = 0;
	spider->mRumbleTimer = 0;
}
