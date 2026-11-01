#include "game/xobjarmy.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__8XObjArmy[];
XObjArmyShot* fn_800E69F4(XObjArmyShot* shot, s32 del);
}

/**
 * @note Address: 0x800E6950
 * @note Size: 0xA4
 */
// todo: weak copy
extern "C" XObjArmy* fn_800E6950(XObjArmy* army, s32 del)
{
	if (army != NULL) {
		*(void***)((u8*)army + 0x34) = __vt__8XObjArmy;
		__destroy_arr(army->mShots, (ConstructorDestructor*)fn_800E69F4, sizeof(XObjArmyShot), ARRAY_SIZE(army->mShots));
		army->_9A8.CVec::~CVec();
		army->mFlags.CStatus::~CStatus();
		fn_8000877C((XObj*)army, 0);
		if ((s16)del > 0)
			::operator delete(army);
	}
	return army;
}

/**
 * @note Address: 0x800E69F4
 * @note Size: 0x68
 */
// todo: weak copy
extern "C" XObjArmyShot* fn_800E69F4(XObjArmyShot* shot, s32 del)
{
	if (shot != NULL) {
		shot->mVel.CVec::~CVec();
		shot->mPos.CVec::~CVec();
		if ((s16)del > 0)
			::operator delete(shot);
	}
	return shot;
}
