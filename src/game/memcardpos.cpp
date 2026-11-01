#include <dolphin/mtx/vec.h>
#include "game/savedata.h"
#include "game/cgame.h"
#include "game/xchr.h"
#include "game/cmotioncontrolcamera.h"
#include "game/xobjchr.h"

extern "C" void fn_8014127C(CInterfaceCameraAction* action);

/**
 * @note Address: 0x80087A4C
 * @note Size: 0x23C
 */
extern "C" void fn_80087A4C(Memcard* card)
{
	SaveData* save = &card->mFile.mData;
	XObj* player   = lbl_8025C400.mPlayer;
	player->SetPos(save->mPos, 0);
	lbl_8025C400.mPlayer->SetRotationY(save->mRotY, 0);
	fn_800B5E1C(lbl_8025C400.mPlayer);
	fn_8009FCC4(lbl_8025C400.mPlayer);
	if (save->mHeader.mVersion <= 0x10100 || (save->mHeader.mVersion & 2)) {
		CMotionControlCamera* camera = (CMotionControlCamera*)&lbl_8025C400.mCamera;
		s32 action                   = save->mCameraAction;
		if (action >= 0 && action < CAMACTION_NUM) {
			camera->ChangeAction(action);
			camera->mCamera.SetEye(save->mCameraEye);
			camera->mCamera.SetInterest(save->mCameraInterest);
			camera->mCamera.SetAngle(save->mCameraAngle);
			camera->mCamera.SetDistance(save->mCameraDistance);
			camera->mCamera.SetFov(save->mCameraFov);
			CCameraMemento memento;
			CInterfaceCameraAction* map = lbl_8025C400.mCameraTable[CAMACTION_MAP];
			memento.SetEye(save->mCameraEye);
			memento.SetInterest(save->mCameraInterest);
			memento.SetAngle(save->mCameraAngle);
			memento.mDistance = save->mCameraDistance;
			memento.mFov      = save->mCameraFov;
			memento.mAction   = lbl_8025C400.mCameraTable[save->mCameraAction];
			fn_8014127C(map);
			camera->SetMemento(&memento);
		}
	}
}
