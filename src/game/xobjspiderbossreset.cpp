#include "game/xobjspiderboss.h"

// todo: cleanup
static inline void ResetState(XObjSpiderBoss* spider)
{
	ResetSpiderBossState(spider);
	spider->mStep      = 0;
	spider->mTimer     = 0;
	spider->mPrevState = spider->mState;
	spider->mState     = 0;
	spider->mSpiderStatus.mFlags |= 0x04000000;
}

/**
 * @note Address: 0x801068D4
 * @note Size: 0x50
 */
extern "C" void vf94__14XObjSpiderBossFv(XObjSpiderBoss* spider)
{
	ResetState(spider);
}

/**
 * @note Address: 0x80106924
 * @note Size: 0x58
 */
extern "C" void vf90__14XObjSpiderBossFUl(XObjSpiderBoss* spider, u32 id)
{
	vf90__4XObjFUl(spider, id);
	ResetState(spider);
}

XObjSpiderBoss::StateFunc gSpiderBossStateHandlers[5] = {
	&XObjSpiderBoss::State0, &XObjSpiderBoss::State1, &XObjSpiderBoss::State2, &XObjSpiderBoss::State3, &XObjSpiderBoss::State4,
};
