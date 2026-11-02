#include "game/xobjspiderboss.h"

struct XObjSpiderBossStates {
	XObjSpiderBoss::StateFunc mFuncs[5]; // _00
};
typedef char XObjSpiderBossStates_size_check[sizeof(XObjSpiderBossStates) == 0x3C ? 1 : -1];

// the tu's data block, the code reaches it from lbl_8022B8F0
struct XObjSpiderBossData {
	u8 _000[0x250];                 // _000
	XObjSpiderBossStates mDefaults; // _250
	XObjSpiderBossStates mStates;   // _28C, lbl_8022BB7C
};

extern "C" {
extern XObjSpiderBossData lbl_8022B8F0;
}

// todo: cleanup
static inline void ResetState(XObjSpiderBoss* spider)
{
	fn_8010660C(spider);
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

/**
 * @note Address: 0x8010697C
 * @note Size: 0x98
 */
// todo: the tu's __sinit, the table it copies is other units'
extern "C" void fn_8010697C()
{
	lbl_8022B8F0.mStates = lbl_8022B8F0.mDefaults;
}

__declspec(section ".ctors") extern void (*const spiderBossInit)() = fn_8010697C;
