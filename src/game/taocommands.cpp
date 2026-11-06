#include "game/xobjactor.h"
#include "game/xchr.h"
#include "game/gmath.h"

extern "C" {
void HSD_MtxGetScale(Mtx mtx, Vec* scale);
void PSMTXTrans(Mtx mtx, f32 x, f32 y, f32 z);
CVec GetStagePlacementScale(XObj* object); // the object's scale as it is drawn
}

extern "C" XObjActorTao::StateFunc sTaoPickupState;

#pragma section RX ".text.taocommands"

/**
 * @note Address: 0x8012CA98
 * @note Size: 0x770
 */
__declspec(section ".text.taocommands") s32 XObjActorTao::HandleEventCommand(s32 count, s32* command)
{
	HandleEventCommand__9XObjActorFlPl(this, count, command);
	if (count == 0) {
		return -1;
	}
	switch (command[0]) {
	case 1:
		switch (command[1]) {
		case 0:
			ResetActorLook(this, 0.08726646f);
			break;
		case 1:
			SetActorLookAtPosition(this, &gGame.GetPlayer()->mMoveTranslate, 0.08726646f);
			break;
		case 2:
			if (FindObj(command[2]) != NULL) {
				SetActorLookTarget(this, &FindObj(command[2])->mMoveTranslate, DEG2RAD * command[3]);
			}
			break;
		case 3:
			SetActorLookAngle(this, DEG2RAD * command[2], DEG2RAD * command[3]);
			break;
		}
		break;
	case 2:
		Drop();
		switch (command[1]) {
		case 0x102:
			for (s32 i = 0; i < 64; i++) {
				XObj* obj = gGame.mXObjs.mPools[8].Get(i);
				if (command[1] == obj->GetKind()) {
					mHeldObjects[0] = obj;
					obj->mStatus.mFlags |= 0x10000ULL;
					switch (command[1]) {
					case 0x102:
						_C8C.mFlags |= 2;
						break;
					}
				}
			}
			break;
		case 0x159:
			for (s32 i = 0; i < 15; i++) {
				XObj* obj = gGame.mXObjs.mPools[10].Get(i);
				if (command[1] == obj->GetKind()) {
					mHeldObjects[0] = obj;
					obj->mStatus.mFlags |= 0x10000ULL;
					_C8C.mFlags |= 1;
				}
			}
			break;
		case 0xF8: {
			mCmd[2] = command[2];
			s32 n   = -1;
			for (s32 i = 0; i < 21; i++) {
				XObj* obj = gGame.mXObjs.mPools[16].Get(i);
				if (command[1] == obj->GetKind()) {
					if (++n == mCmd[2]) {
						obj->mStatus.mFlags |= 0x10000ULL;
						mHeldObjects[0] = obj;
						_C8C.mFlags |= 4;
						break;
					}
				}
			}
			break;
		}
		case 0x2A3:
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = gGame.mXObjs.mPools[22].Get(i);
				if (command[1] == obj->GetKind()) {
					obj->mStatus.mFlags |= 0x10000ULL;
					_C8C.mFlags |= 8;
					mHeldObjects[0] = obj;
				}
			}
			break;
		case 0x46F:
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = gGame.mXObjs.mPools[22].Get(i);
				if (command[1] == obj->GetKind()) {
					obj->mStatus.mFlags |= 0x10000ULL;
					_C8C.mFlags |= 0x20;
					mHeldObjects[0] = obj;
				}
			}
			break;
		default:
			if (mHeldObjects[0] != NULL) {
				HSD_JObjClearMtxFlags(mHeldObjects[0]->mJObj);
				mHeldObjects[0] = NULL;
			}
			break;
		}
		break;
	case 3:
		_A4C   = command[1];
		mState = sTaoPickupState;
		ResetActorMovementStep(this);
		mCmd[0] = 0;
		break;
	case 4:
		_A4C = -1;
		if (_A50 != NULL) {
			_A50->Release();
		}
		mCmd[0] = 0;
		break;
	case 5:
		if (command[1] != 0) {
			_CF4 = 0x82;
			_CF8 = 1;
		} else {
			_CF8 = 0;
			_CF4 = -1;
			if (_CFC != NULL) {
				_CFC->Release();
			}
		}
		mCmd[0] = 0;
		break;
	default:
		if (count < 1) {
			return 0;
		}
		mCmd[0] = command[0];
		if (count < 2) {
			mCmd[1] = 0;
		} else {
			mCmd[1] = command[1];
		}
		mCmd[2] = command[2];
		mCmd[3] = command[3];
		mCmd[4] = command[4];
		mCmd[5] = command[5];
		mCmd[6] = command[6];
		mCmd[7] = command[7];
		mCmd[8] = command[8];
		mCmd[9] = command[9];
		break;
	}
	return 0;
}

/**
 * @note Address: 0x8012D208
 * @note Size: 0x664
 */
// picks up the object _A4C names and holds it in its mouth
__declspec(section ".text.taocommands") void XObjActorTao::StatePickUp(u32 frames)
{
	switch (_990) {
	case 0:
		if (_A50 != NULL) {
			_A50->Release();
		}
		if (_A4C >= 0) {
			_A50 = ReadObj(_A4C);
		} else {
			_A50 = NULL;
		}
		_990++;
	case 1:
		if (_A50 != NULL) {
			if (_A50->mStatus.mFlags & 0x0040000000000000ULL) {
				return;
			}
			gGame.mStatus.mFlags |= 0x1000;
		}
		_990++;
	case 2:
		if (_A4C >= 0 && _A50 != NULL) {
			_A50->SetScale(1.0f, 1.0f, 1.0f, 0);
			CJObj jobj;
			s32 path[6] = { 0, 0, 0, 0, 0, 0 };
			jobj.Attach(FindJointByPath(mJObj, path, 6));
			Vec pos = jobj.GetPos();
			_A50->SetPos(pos.x, HSD_JObjGetTranslationY(mJObj), pos.z, 0);
			Vec scale;
			PSMTXCopy(jobj.GetMtx(), _C94);
			HSD_MtxGetScale(_C94, &scale);
			scale.x = GetStagePlacementScale(_A50).x / scale.x;
			scale.y = GetStagePlacementScale(_A50).y / scale.y;
			scale.z = GetStagePlacementScale(_A50).z / scale.z;
			Mtx scaleMtx;
			Mtx transMtx;
			Mtx m;
			PSMTXScale(scaleMtx, scale.x, scale.y, scale.z);
			PSMTXConcat(_C94, scaleMtx, m);
			PSMTXTrans(transMtx, 0.0f, -10.0f, 0.0f);
			PSMTXConcat(m, transMtx, m);
			HSD_JObj* joint = _A50->mJObj;
			MtxPtr mp       = m;
			HSD_JObjCopyMtx(joint, mp);
			_A50->mStatus.mFlags &= ~0x0200000000000000ULL;
		}
		break;
	}
}

/**
 * @note Address: 0x8012D86C
 * @note Size: 0x4
 */
__declspec(section ".text.taocommands") void XObjActorTao::StateNone(u32 frames)
{
}
