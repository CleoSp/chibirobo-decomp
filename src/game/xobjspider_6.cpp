#include "game/particle_srt.h"
#include "game/xobjspider_shared.h"

// puts everything back to how it starts
extern "C" void fn_800F4350(XObjSpider* self)
{
	self->mDir           = 0.0f;
	self->mSpeed         = 0.0f;
	self->_988           = 0.0f;
	self->_98C           = 0.0f;
	self->mTurnSpeed     = 0.1f;
	self->mFallSpeed     = 0.0f;
	self->mState         = 0;
	self->mPrevState     = 0;
	self->mStep          = 0;
	self->mTimer         = 0;
	self->mStatus.mFlags = 0;
	self->mWait          = 0;
	self->_9C0           = 1.0f;
	self->_9C4           = 1.0f;
	Vec v0               = { 0.0f, 0.0f, 0.0f };
	self->mHitPos        = v0;
	Vec v1               = { 0.0f, 0.0f, 0.0f };
	self->mHome          = v1;
	Vec v2               = { 0.0f, 0.0f, 0.0f };
	self->mWallPos       = v2;
	Vec v3               = { 0.0f, 0.0f, 0.0f };
	self->_9FC           = v3;
	Vec v4               = { 0.0f, 0.0f, 0.0f };
	self->_A08           = v4;
	Vec v5               = { 0.0f, 0.0f, 0.0f };
	self->_A14           = v5;
	self->_A38           = 0.0f;
	self->_A40           = 0.0f;
	self->_A44           = 0;
	self->_A48           = 0;
	self->mHitDist       = 0.0f;
	self->mWall          = 0;
	self->mMaxLife       = 3;
	self->mLife          = self->mMaxLife;
	self->mArgs[0]       = 0;
	self->mArgs[1]       = 0;
	self->_A88           = 0;
	self->mThreadAlpha   = 1.0f;
	self->mFlags |= 8;
	self->mPart.mPos.x         = 0.0f;
	self->mPart.mPos.y         = 0.0f;
	self->mPart.mPos.z         = 0.0f;
	self->mPart._10.x          = 0.0f;
	self->mPart._10.y          = 0.0f;
	self->mPart._10.z          = 0.0f;
	self->mPart.mRotY          = 0.0f;
	self->mPart._24            = 0.0f;
	self->mPart.mState         = 0;
	self->mPart._2C            = 0;
	self->mPart.mStatus.mFlags = 0;
	self->mPart.mObj           = NULL;
	self->mRadius              = 20.0f;
	self->mPart2.mObj          = NULL;
	self->mCarry               = 0xFF;
	Vec v6                     = { 0.0f, 0.0f, 0.0f };
	self->mArea                = v6;
	self->mAreaW               = 0.0f;
	self->mAreaD               = 0.0f;
	Vec v7                     = { 0.0f, 0.0f, 0.0f };
	self->_A60                 = v7;
	self->_A6C                 = 0.0f;
	self->_B10                 = 2;
	self->_B14[0]              = 0;
	self->_B14[1]              = 0;
	self->_B14[2]              = 0;
	self->_B14[3]              = 0;
	self->_B24[0]              = 100;
	self->_B24[1]              = 100;
	self->_B24[2]              = 100;
	self->_B54                 = 0;
	self->_B50                 = 0;
	self->_B5C                 = NULL;
	self->_B58                 = 0;
	self->_B60                 = 0;
	for (int i = 0; i < 8; i++) {
		self->mTies[i].mObj   = NULL;
		self->mTies[i].mJoint = NULL;
	}
	if (self->_258 == 0) {
		ResetDrops();
	}
	self->_B94         = 0;
	self->_B98         = 0;
	self->_B9C         = -1;
	self->mRumble      = 0;
	self->mRumbleTimer = 0;
}
