#include "game/hsd_jobj.h"
#include "game/cscrn.h"

extern "C" {
extern void* __vt__9CScrnWipe[];
extern void* __vt__12CScrnWipeTex[];
extern void* __vt__13CScrnRoboeyes[];
extern void* __vt__13CScrnWipeTile[];
extern void* __vt__15CScrnWipeCustom[];
}

/**
 * @note Address: 0x8001B5A4
 * @note Size: 0x6C
 */
// todo: weak copy
extern "C" CScrnRoboeyes* __dt__13CScrnRoboeyesFv(CScrnRoboeyes* wipe, s32 flags)
{
	if (wipe != NULL) {
		*(void***)((u8*)wipe + 0x30) = __vt__13CScrnRoboeyes;
		wipe->CScrnWipe::~CScrnWipe();
		if ((s16)flags > 0) {
			::operator delete(wipe);
		}
	}
	return wipe;
}

/**
 * @note Address: 0x8001B610
 * @note Size: 0x6C
 */
// todo: weak copy
extern "C" CScrnWipeTile* __dt__13CScrnWipeTileFv(CScrnWipeTile* wipe, s32 flags)
{
	if (wipe != NULL) {
		*(void***)((u8*)wipe + 0x30) = __vt__13CScrnWipeTile;
		wipe->CScrnWipe::~CScrnWipe();
		if ((s16)flags > 0) {
			::operator delete(wipe);
		}
	}
	return wipe;
}

/**
 * @note Address: 0x8001B67C
 * @note Size: 0x6C
 */
// todo: weak copy
extern "C" CScrnWipeCustom* __dt__15CScrnWipeCustomFv(CScrnWipeCustom* wipe, s32 flags)
{
	if (wipe != NULL) {
		*(void***)((u8*)wipe + 0x30) = __vt__15CScrnWipeCustom;
		wipe->CScrnWipe::~CScrnWipe();
		if ((s16)flags > 0) {
			::operator delete(wipe);
		}
	}
	return wipe;
}

/**
 * @note Address: 0x8001B6E8
 * @note Size: 0x84
 */
// todo: weak copy
extern "C" CScrnWipeTex* __dt__12CScrnWipeTexFv(CScrnWipeTex* wipe, s32 flags)
{
	if (wipe != NULL) {
		*(void***)((u8*)wipe + 0x30) = __vt__12CScrnWipeTex;
		wipe->mTex.CTexObj::~CTexObj();
		wipe->CScrnWipe::~CScrnWipe();
		if ((s16)flags > 0) {
			::operator delete(wipe);
		}
	}
	return wipe;
}

/**
 * @note Address: 0x8001B76C
 * @note Size: 0x5C
 */
// todo: weak copy
extern "C" CScrnWipe* __dt__9CScrnWipeFv(CScrnWipe* wipe, s32 flags)
{
	if (wipe != NULL) {
		*(void***)((u8*)wipe + 0x30) = __vt__9CScrnWipe;
		wipe->CScrnColor::~CScrnColor();
		if ((s16)flags > 0) {
			::operator delete(wipe);
		}
	}
	return wipe;
}
