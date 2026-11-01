#include "game/xobjleticker.h"
#include "game/ccameraaction.h"
#include "game/cgame.h"
#include "game/xchr.h"

/**
 * @note Address: 0x800E54E0
 * @note Size: 0x1A8
 */
extern "C" void fn_800E54E0(XObjLeticker* leticker)
{
	XObj* player    = lbl_8025C400.mPlayer;
	s32 path[2]     = { 0, 9 };
	HSD_JObj* joint = GetJointByPath(leticker->mJObj, path, 2);
	switch (leticker->mMode) {
	case 0:
		break;
	case 10: {
		Vec position;
		position.y = position.z = position.x = 0.0f;
		position.y                           = 0.5f;
		PSMTXMultVec(HSD_JObjGetMtxPtr(joint), &position, &position);
		player->SetPos(position, 0);
		player->SetRotationY(player->mMoveRotate.y - PI / 6, 0);
		break;
	}
	}
}
