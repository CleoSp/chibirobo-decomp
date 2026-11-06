#include "game/xobjactor.h"
#include "game/xchr.h"
#include "game/gmath.h"

extern "C" {
void HSD_MtxGetScale(Mtx mtx, Vec* scale);
void PSMTXRotRad(Mtx mtx, char axis, f32 angle);
void PSMTXTrans(Mtx mtx, f32 x, f32 y, f32 z);
CVec GetStagePlacementScale(XObj* object); // the object's scale as it is drawn
}

#pragma section RX ".text.papapickup"

/**
 * @note Address: 0x8012955C
 * @note Size: 0x210
 */
__declspec(section ".text.papapickup") s32 XObjActorPapa::HandleEventCommand(s32 count, s32* command)
{
	HandleEventCommand__9XObjActorFlPl(this, count, command);
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
 * @note Address: 0x8012976C
 * @note Size: 0x76C
 */
// picks up the object _A4C names and holds it in his hand
__declspec(section ".text.papapickup") void XObjActorPapa::StatePickUp(u32 frames)
{
	switch (_990) {
	case 0: {
		XObjActorTao* tao = ((XObjActorTao*)gGame.mXObjs.mPools[14].mObjects);
		if ((tao->mStatus.mFlags & 1) && !tao->IsFree()) {
			return;
		}
		bool busy = false;
		for (s32 i = 0; i < 15; i++) {
			XObjActorToy* toy = (XObjActorToy*)gGame.mXObjs.mPools[10].Get(i);
			if (toy->GetKind() == 0x159) {
				if (!(toy->mStatus.mFlags & 1)) {
					break;
				}
				if (!toy->IsFree()) {
					busy = true;
					break;
				}
			}
		}
		if (busy) {
			return;
		}
		if (_A50 != NULL) {
			_A50->Release();
		}
		if (_A4C >= 0) {
			_A50 = ReadObj(_A4C);
		} else {
			_A50 = NULL;
		}
		_998.mFlags |= 0x2000;
		_990++;
	}
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
			s32 path[6] = { 0, 2, 2, 0, 0, 0 };
			jobj.Attach(FindJointByPath(mJObj, path, 6));
			Vec pos;
			pos = GetJointPosition(jobj.mJObj);
			Vec scale;
			scale.x = pos.x;
			scale.y = pos.y;
			scale.z = pos.z;
			_A50->SetPos(pos.x, pos.y + 2.0f, pos.z, 0);
			PSMTXCopy(jobj.GetMtx(), _A54);
			HSD_MtxGetScale(_A54, &scale);
			scale.x = GetStagePlacementScale(_A50).x / scale.x;
			scale.y = GetStagePlacementScale(_A50).y / scale.y;
			scale.z = GetStagePlacementScale(_A50).z / scale.z;
			Mtx scaleMtx;
			Mtx rotMtx;
			Mtx transMtx;
			Mtx m;
			PSMTXScale(scaleMtx, scale.x, scale.y, scale.z);
			PSMTXConcat(_A54, scaleMtx, m);
			PSMTXRotRad(rotMtx, 'z', 1.5707964f);
			PSMTXConcat(m, rotMtx, m);
			PSMTXTrans(transMtx, -5.5f, 3.5f, 0.0f);
			PSMTXConcat(m, transMtx, m);
			HSD_JObj* joint = _A50->mJObj;
			MtxPtr mp       = m;
			HSD_JObjCopyMtx(joint, mp);

			_A50->mStatus.mFlags &= ~0x0200000000000000ULL;
		}
		_998.mFlags &= ~0x2000;
		break;
	}
}

/**
 * @note Address: 0x80129ED8
 * @note Size: 0x4
 */
__declspec(section ".text.papapickup") void XObjActorPapa::StateNone(u32 frames)
{
}
