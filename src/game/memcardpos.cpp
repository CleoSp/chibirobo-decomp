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
extern "C" void RestoreSavedPlayerAndCamera(Memcard* card)
{
	SaveData* save = &card->mFile.mData;
	XObj* player   = gGame.GetPlayer();
	player->SetPos(save->mPos, 0);
	gGame.GetPlayer()->SetRotationY(save->mRotY, 0);
	ResetPlayerHeadOffset(gGame.GetPlayer());
	fn_8009FCC4(gGame.GetPlayer());
	if (save->mHeader.mVersion <= 0x10100 || (save->mHeader.mVersion & 2)) {
		CMotionControlCamera* camera = &gGame.mMotionControlCamera;
		s32 action                   = save->mCameraAction;
		if (action >= 0 && action < CAMACTION_NUM) {
			camera->ChangeAction(action);
			camera->mCamera.SetEye(save->mCameraEye);
			camera->mCamera.SetInterest(save->mCameraInterest);
			camera->mCamera.SetAngle(save->mCameraAngle);
			camera->mCamera.SetDistance(save->mCameraDistance);
			camera->mCamera.SetFov(save->mCameraFov);
			CCameraMemento memento;
			CInterfaceCameraAction* map = gGame.mCameraActions.mTable[CAMACTION_MAP];
			memento.SetEye(save->mCameraEye);
			memento.SetInterest(save->mCameraInterest);
			memento.SetAngle(save->mCameraAngle);
			memento.mDistance = save->mCameraDistance;
			memento.mFov      = save->mCameraFov;
			memento.mAction   = gGame.mCameraActions.mTable[save->mCameraAction];
			fn_8014127C(map);
			camera->SetMemento(&memento);
		}
	}
}
