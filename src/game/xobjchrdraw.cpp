#include "game/xchr.h"
#include "game/cgame.h"
#include "game/cmotioncontrolcamera.h"
#include "game/cparticle.h"
#include "game/hsd_cobj.h"

// a particle an anim frame table asks for
struct PlayerFrameParticle {
	s32 mCode;      // _00, -1 for none
	Vec mTranslate; // _04
	Vec mScale;     // _10
	Vec mRotate;    // _1C
};
STATIC_ASSERT(sizeof(PlayerFrameParticle) == 0x28);

extern "C" {
void PSMTXRotRad(Mtx mtx, char axis, f32 angle);
PlayerFrameParticle fn_800A7E1C(XObj* object, f32* frame, const void* table);
extern const u8 lbl_80226324[];
extern const u8 lbl_802276D4[];
}

// XObjChr::vfB8, nothing up close to the camera
/**
 * @note Address: 0x800A77D4
 * @note Size: 0xC0
 */
extern "C" void fn_800A77D4(XObjChr* player)
{
	CMotionControlCamera* camera = (CMotionControlCamera*)lbl_8025C400.mCamera;
	if (camera->GetMapDistance() < 1.0f)
		return;
	if (player->mStatus.mFlags & 4ULL) {
		if (lbl_8025C400.mStatus.mFlags & 0x0040000000000000ULL) {
			XObj* plug = &player->mPlug;
			plug->vfB8();
		}
	} else {
		fn_80090864(player);
	}
}

// XObjChr::vfB4
/**
 * @note Address: 0x800A7894
 * @note Size: 0x198
 */
extern "C" void fn_800A7894(XObjChr* player, Mtx view)
{
	CMotionControlCamera* camera = (CMotionControlCamera*)lbl_8025C400.mCamera;
	if (camera->GetMapDistance() < 1.0f)
		return;
	if (lbl_8025C400.mStatus.mFlags & 0x100ULL)
		fn_800A30EC(player, 0);
	player->vfB0(view);
	if (lbl_8025C400.mStatus.mFlags & 0x100ULL)
		fn_800A30EC(player, 1);
	if (lbl_8025C400.mStatus.mFlags & 0x0040000000000000ULL)
		player->mPlug.vfB4(view);
	player->mCopterObj.vfB4(view);
	player->mPopperObj.vfB4(view);
	player->mRadarObj.vfB4(view);
	player->mOXObj.vfB4(view);
}

// XObjChr::vfB0, himself then the attachments, the popper turned to face the camera on flag 0x100
/**
 * @note Address: 0x800A7A2C
 * @note Size: 0x218
 */
extern "C" void fn_800A7A2C(XObjChr* player, Mtx view)
{
	CMotionControlCamera* camera = (CMotionControlCamera*)lbl_8025C400.mCamera;
	if (camera->GetMapDistance() < 1.0f)
		return;
	fn_800908C0(player, view);
	if ((lbl_8025C400.mStatus.mFlags & 0x100ULL) && player->mLift.mState == 4 && fn_800D4FE8(&player->mLift)) {
		// no plug in this lift state
	} else if (lbl_8025C400.mStatus.mFlags & 0x0040000000000000ULL) {
		player->mPlug.vfB0(view);
	}
	player->mCopterObj.vfB0(view);
	if (lbl_8025C400.mStatus.mFlags & 0x100ULL) {
		Mtx rotation, combined;
		PSMTXRotRad(rotation, 'x', -camera->mCamera.mAngle.x);
		if (view != NULL) {
			PSMTXConcat(rotation, view, combined);
		} else {
			CCamera* current = ((CBase*)&lbl_8025C400)->GetCamera();
			PSMTXConcat(rotation, current->mViewMtx, combined);
		}
		player->mPopperObj.vfB0(combined);
	} else {
		player->mPopperObj.vfB0(view);
	}
	player->mRadarObj.vfB0(view);
	player->mOXObj.vfB0(view);
}

// XObjAttachment::vfB0
/**
 * @note Address: 0x800A7C44
 * @note Size: 0x2C
 */
extern "C" void fn_800A7C44(XObjAttachment* object, Mtx view)
{
	if (object->mVisible != 0)
		fn_800908C0(object, view);
}

// steps the anim frame and fires the particles the frame tables ask for
/**
 * @note Address: 0x800A7C70
 * @note Size: 0x1AC
 */
extern "C" void fn_800A7C70(XObjChr* player)
{
	player->_94FC                = player->_94F8;
	HSD_AObj* aobj               = GetAObj(player->mJObj);
	player->_94F8                = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
	PlayerFrameParticle particle = fn_800A7E1C(player, &player->_94F8, lbl_80226324);
	if (particle.mCode >= 0 && !(lbl_8025C400.mStatus.mFlags & 0x100ULL)) {
		((CParticle*)lbl_8025C400.mParticle)
		    ->CreateSRT((u32)particle.mCode | 0x10000, 1, particle.mTranslate, particle.mScale, particle.mRotate, FALSE);
	}
	if (player->mCopterObj.mVisible != 0)
		fn_800A7E1C(&player->mCopterObj, NULL, lbl_802276D4);
}
