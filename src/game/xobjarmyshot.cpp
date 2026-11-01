#include "game/xobjarmy.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/xchr.h"
extern "C" {
#include <dolphin/mtx/vec.h>
}

/**
 * @note Address: 0x800E6B70
 * @note Size: 0x308
 */
extern "C" void fn_800E6B70(XObjArmyShot* shot)
{
	if (!shot->mActive)
		return;
	CVec target;
	switch (shot->mPhase) {
	case 0: {
		if (--shot->mLife == 0)
			shot->mActive = 0;
		shot->Move();
		f32 height;
		if (lbl_8065B2AC != NULL) {
			target.x = lbl_8065B2AC->mMoveTranslate.x;
			target.y = lbl_8065B2AC->mMoveTranslate.y;
			target.z = lbl_8065B2AC->mMoveTranslate.z;
			height   = 28.0f;
		} else {
			XObjChr* player = lbl_8025C400.mPlayer;
			target.x        = player->mMoveTranslate.x;
			target.y        = player->mMoveTranslate.y;
			target.z        = player->mMoveTranslate.z;
			height          = player->GetAttachment() != 6 ? 10.0f : 28.0f;
		}
		f32 dy = target.y - shot->mPos.y;
		if (dy < 0.0f)
			dy = -dy;
		if (dy < height) {
			target.y = shot->mPos.y;
			if (PSVECDistance(&shot->mPos, &target) < 5.0f) {
				Vec hit = shot->mPos;
				fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x110029, 255);
				lbl_8065B2B0.mFlags |= 1;
				lbl_8025C400.mPlayer->vf138(hit, 10.0f);
				shot->mVel.x = -1.0f * shot->mVel.x;
				shot->mVel.y = -1.0f;
				shot->mVel.z = -1.0f * shot->mVel.z;
				shot->mSpeed = 0.5f;
				++shot->mPhase;
			}
		}
		break;
	}
	case 1:
		shot->Move();
		shot->mVel.y += -0.15f;
		if (shot->mPos.y <= 1.5f) {
			shot->mVel.y = 0.0f;
			shot->mSpeed = 0.3f;
			shot->mLife  = 10;
			++shot->mPhase;
		}
		break;
	case 2:
		if (--shot->mLife == 0)
			shot->mActive = 0;
		shot->Move();
		break;
	}
}
