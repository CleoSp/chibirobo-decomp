#include "game/xobjarmy.h"
#include "game/cgame.h"
#include "game/csound.h"
#include <math.h>
extern "C" {
#include <dolphin/mtx/vec.h>
extern s32 lbl_8065B2A4;
}

/**
 * @note Address: 0x800E7530
 * @note Size: 0x2C4
 */
extern "C" void Shoot__8XObjArmyFv(XObjArmy* army)
{
	CVec pos, vel;
	s32 path[6] = { 0, 0, 4, 0, 0, 0 };
	GetJointByPath(army->mJObj, path, 6);
	pos.x   = army->mMoveTranslate.x;
	pos.y   = army->mMoveTranslate.y;
	pos.z   = army->mMoveTranslate.z;
	Vec dir = { 0.0f, 0.0f, 0.0f };
	dir.x   = sin(HSD_JObjGetRotationY(army->mJObj));
	dir.z   = cos(HSD_JObjGetRotationY(army->mJObj));
	vel     = CVec(dir);
	u32 i;
	// retail reads one past the end before checking i
	for (i = 0; army->mShots[i].mActive != 0 && i < 5; ++i) { }
	if (i < 5) {
		u32 quiet               = (army->mFlags.mFlags >> 7) & 1;
		army->mShots[i].mActive = 1;
		army->mShots[i].mLife   = 120;
		army->mShots[i].mPos.x  = pos.x;
		army->mShots[i].mPos.y  = pos.y;
		army->mShots[i].mPos.z  = pos.z;
		army->mShots[i].mPos.y += 6.0f;
		army->mShots[i].mVel.x = vel.x;
		army->mShots[i].mVel.y = vel.y;
		army->mShots[i].mVel.z = vel.z;
		PSVECNormalize(&army->mShots[i].mVel, &army->mShots[i].mVel);
		army->mShots[i].mSpeed = 3.0f;
		army->mShots[i].mPhase = 0;
		if (!quiet) {
			if (lbl_8065B2A4 < 0 || lbl_8065B2A4 >= 4)
				lbl_8065B2A4 = 0;
			fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x110012 + lbl_8065B2A4, 255);
			++lbl_8065B2A4;
		}
	}
}
