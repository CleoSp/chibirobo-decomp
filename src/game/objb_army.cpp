#include "game/objb_army_shared.h"

/**
 * @note Address: 0x800E6E78
 * @note Size: 0x158
 */
void XObjArmy::vf138(const Vec* pos, f32 f)
{
	if (!(mStatus.mFlags & 1)) {
		_994 = 0.0f;
		fn_8001E904(GAME_SOUND, 0x83, 0xFF);
		if (_994 <= 0.0f) {
			if (fn_8006EBC8(0x1F) / 100.0f <= f || PSVECDistance(&mMoveTranslate, &GAME_PLAYER->mMoveTranslate) < 20.0f) {
				mStep  = 0;
				mState = &XObjArmy::State7DDC;
			} else {
				mStep  = 0;
				mState = &XObjArmy::State81A0;
			}
		} else {
			mStep  = 0;
			mState = &XObjArmy::State8564;
		}
	}
}
extern "C" {

/**
 * @note Address: 0x800E6FD0
 * @note Size: 0x560
 */
s32 fn_800E6FD0(XObjArmy* obj, s32 argc, s32* args)
{
	if (argc < 1) {
		return -1;
	}
	switch (args[0]) {
	case 0:
		sB2A8  = 0;
		sMin.x = args[1] / 100.0f;
		sMin.y = args[2] / 100.0f;
		sMin.z = args[3] / 100.0f;
		sMax.x = args[4] / 100.0f;
		sMax.y = args[5] / 100.0f;
		sMax.z = args[6] / 100.0f;
		break;
	case 1:
		switch (args[1]) {
		case 0:
			return sB2A8;
		case 5:
			return sStatus.mFlags & 1;
		default:
			return sB2A8;
		}
	case 2:
		switch (args[1]) {
		case 0: {
			Vec pos;
			pos.x = sMin.x + (sMax.x - sMin.x) * HSD_Randf();
			pos.y = sMin.y + (sMax.y - sMin.y) * HSD_Randf();
			pos.z = sMin.z + (sMax.z - sMin.z) * HSD_Randf();
			obj->SetMoveTranslate(&pos, 0);
			obj->mFlags |= 0x10000;
			obj->SetMode(1);
			break;
		}
		case 1:
			obj->Attack();
			break;
		case 2:
			obj->mStep  = 0;
			obj->mState = NULL;
			break;
		case 3:
			if (argc >= 2) {
				if (args[2] >= 0) {
					sTarget = GAME_OBJTABLE[args[2]];
				} else {
					sTarget = NULL;
				}
			}
			break;
		case 4:
			if (argc >= 3) {
				if (args[2]) {
					obj->mStatus.mFlags &= ~0x80;
				} else {
					obj->mStatus.mFlags |= 0x80;
				}
			} else {
				obj->mStatus.mFlags &= ~0x80;
			}
			break;
		case 5:
			sStatus.mFlags &= ~1;
			break;
		case 6:
			if (argc >= 3) {
				if (args[2]) {
					sStatus.mFlags &= ~2;
				} else {
					sStatus.mFlags |= 2;
				}
			} else {
				sStatus.mFlags |= 2;
			}
			break;
		}
		break;
	case 3:
		obj->mGuardPos.x = args[1] / 100.0f;
		obj->mGuardPos.y = args[2] / 100.0f;
		obj->mGuardPos.z = args[3] / 100.0f;
		obj->SetMode(2);
		break;
	case 100:
		break;
	case 0x65:
		if (args[1] == 0) {
			obj->mStatus.mFlags &= ~0x100;
		} else {
			obj->mStatus.mFlags |= 0x100;
		}
		break;
	default:
		break;
	}
	return 0;
}
}

// Initialize a shot from the soldier's position and firing direction.
static inline void FireShot(ArmyShot* shot, const CVec& pos, const CVec& dir, u8 quiet)
{
	shot->mActive = 1;
	shot->mLife   = 120;
	shot->mPos    = pos;
	shot->mPos.y += 6.0f;
	shot->mVel = dir;
	PSVECNormalize(&shot->mVel, &shot->mVel);
	shot->mSpeed = 3.0f;
	shot->mPhase = 0;
	if (!quiet) {
		if (sB2A4 < 0 || sB2A4 >= 4) {
			sB2A4 = 0;
		}
		fn_8001E904(GAME_SOUND, sB2A4 + 0x110012, 0xFF);
		sB2A4++;
	}
}
