#include "game/xobjactor.h"
#include "game/xchr.h"
#include "game/cscrn.h"
#include "game/gmath.h"
#include "game/heldpropmatrix.h"
#include <math.h>
#include <stdlib.h>

extern "C" {
extern XObjActorPapa::StateFunc sPapaIdleState;
extern XObjActorPapa::StateFunc sPapaPickupState;
}

// takes the controls away from the player
static inline void LockPlayer(XObjChr* player)
{
	player->_980.mFlags = 0;
	player->_980.mFlags |= 0x40000000;
	player->mPad.mReset = FALSE;
}

/**
 * @note Address: 0x8012BDE4
 * @note Size: 0x9BC
 */
#pragma section RX ".text.papacommands"
__declspec(section ".text.papacommands") void XObjActorPapa::ProcessPendingEvents(u32)
{
	CJObj jobj;
	if (_998.Check(0x400)) {
		_998.mFlags &= ~0x400;
		mHeadPitch = 0.0f;
	}
	if (_998.Check(2)) {
		_998.mFlags &= ~2;
		PlayGameSound(&gGame.mSound, rand() % 10 + 0xB7, 0xFF);
	}
	switch (mCmd[0]) {
	case 2:
		Drop();
		switch (mCmd[1]) {
		case 0x1F4:
		case 0x360:
		case 0x3FA:
		case 0x401:
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = gGame.mXObjs.mPools[22].Get(i);
				if (mCmd[1] == obj->GetKind()) {
					mHeldObjects[0] = obj;
					obj->mStatus.mFlags |= 0x10000ULL;
					switch (mCmd[1]) {
					case 0x1F4:
						_C8C.mFlags |= 1;
						break;
					case 0x360:
						_C8C.mFlags |= 2;
						break;
					case 0x3FA:
						_C8C.mFlags |= 0x10;
						break;
					case 0x401:
						_C8C.mFlags |= 0x20;
						break;
					}
				}
			}
			break;
		case 0x1D3:
			for (s32 i = 0; i < 15; i++) {
				XObj* obj = gGame.mXObjs.mPools[10].Get(i);
				if (mCmd[1] == obj->GetKind()) {
					obj->mStatus.mFlags |= 0x10000ULL;
					mHeldObjects[0] = obj;
					switch (mCmd[1]) {
					case 0x1D3:
						_C8C.mFlags |= 4;
						break;
					}
				}
			}
			break;
		case 0x375:
			for (s32 i = 0; i < 64; i++) {
				XObj* obj = gGame.mXObjs.mPools[8].Get(i);
				if (mCmd[1] == obj->GetKind()) {
					obj->mStatus.mFlags |= 0x10000ULL;
					mHeldObjects[0] = obj;
					switch (mCmd[1]) {
					case 0x375:
						_C8C.mFlags |= 8;
						break;
					}
				}
			}
			break;
		default:
			if (mHeldObjects[0] != NULL) {
				HSD_JObj* held = mHeldObjects[0]->mJObj;
				HSD_JObjClearMtxFlags(held);
				mHeldObjects[0] = NULL;
			}
			break;
		}
		mCmd[0] = 0;
		break;
	case 3:
		switch (mCmd[1]) {
		case 1: {
			_998.mFlags |= 0x100;
			LockPlayer(gGame.GetPlayer());
			gGame.GetPlayer()->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);

			break;
		}
		default: {
			_998.mFlags &= ~0x100;
			s32 path[6] = { 0, 2, 2, 0, 0, 0 };
			jobj.Attach(FindJointByPath(mJObj, path, 6));
			Vec pos;
			pos = GetJointPosition(jobj.mJObj);
			Vec target;
			target.x = pos.x;
			target.y = pos.y;
			target.z = pos.z;
			target.y = mMoveTranslate.y;
			gGame.GetPlayer()->SetPos(target, 0);
			XObjChr* player     = gGame.GetPlayer();
			player->_980.mFlags = 0;
			player->_980.mFlags |= 0x20000000;
			player->_980.mFlags |= 7;
			player->mPad.mReset = TRUE;
			break;
		}
		}
		mCmd[0] = 0;
		break;
	case 4:
		StartScreenShake(&gGame.mQuake, mCmd[1], mCmd[2], mCmd[3], 0.0f, 0.0f, 0.0f, mCmd[4] / 100.0f);
		mCmd[0] = 0;
		break;
	case 5: {
		void* joint = GetJointAnimation(mCmd[1]);
		void* mat   = GetMaterialAnimation(mCmd[2]);
		void* shape = GetShapeAnimation(mCmd[1]);
		SetAnim(0, TRUE, joint, mat, shape, 0.0f, 1.0f, 1.0f, 10.0f);
		mStatus.mFlags |= 0x0008000000000000ULL;
		mCmd[0] = 0;
		break;
	}
	case 6:
		_A4C   = mCmd[1];
		mState = sPapaPickupState;
		ResetActorMovementStep(this);
		mCmd[0] = 0;
		break;
	case 7:
		_A4C = -1;
		if (_A50 != NULL) {
			_A50->Release();
		}
		mCmd[0] = 0;
		break;
	case 8:
		switch (mCmd[1]) {
		case 1:
			_C94 = 1;
			break;
		default:
			_C94 = 0;
			break;
		}
		mCmd[0] = 0;
		break;
	}
}

/**
 * @note Address: 0x8012C7A0
 * @note Size: 0xB8
 */
#pragma section RX ".text.papareset"
__declspec(section ".text.papareset") void XObjActorPapa::ResetActorState()
{
	XObjChr* player = gGame.GetPlayer();
	if (player->_980.mFlags & 0x40000000) {
		player->_980.mFlags = 0;
		player->_980.mFlags |= 0x20000000;
		player->_980.mFlags |= 7;
		player->mPad.mReset = TRUE;
	}
	_998.mFlags = 0;
	_C8C.mFlags = 0;
	_980        = 0.0f;
	_984        = 0.0f;
	_988        = 0.0f;
	_C84        = _988;
	_C88        = 0;
	_C94        = 1;
	mState      = sPapaIdleState;
	ResetActorMovementStep(this);
}
