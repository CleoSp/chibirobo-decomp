#include "game/xobjactor.h"
#include "game/xchr.h"
#include <stdlib.h>

// XObjActor3's update

extern "C" {
CVec fn_800621E0(XObj* obj);
void HSD_MtxGetScale(Mtx mat, Vec* vec);
void fn_8010B394(XObjActor3* actor);
void fn_8010B7B4(XObjActor3* actor, u32 frames);
}

/**
 * @note Address: 0x80109E80
 * @note Size: 0x680
 */
// the per-kind script commands, the kind's own function, the carried objects
extern "C" void fn_80109E80(XObjActor3* actor, u32 frames)
{
	switch (actor->GetItem()) {
	case 8:
		actor->_B8C.mFlags |= 4;
		break;
	case 9:
		actor->vf148();
		if (lbl_8025C400.mStatus.mFlags & 0x400) {
			return;
		}
		break;
	case 10:
		if (lbl_8025C400.mStatus.mFlags & 0x400) {
			return;
		}
		switch (actor->_B94) {
		case 1:
			actor->_B8C.mFlags &= ~4;
			actor->_B94 = 0;
			actor->_B98 = 0;
			break;
		}
		break;
	case 11:
	case 12:
		if (lbl_8025C400.mStatus.mFlags & 0x400) {
			return;
		}
		switch (actor->_B94) {
		case 1:
			if (actor->_B98 != 0) {
				actor->_B8C.mFlags &= ~4;
			} else {
				actor->_B8C.mFlags |= 4;
				actor->_B80 = 0;
			}
			actor->_B94 = 0;
			actor->_B98 = 0;
			break;
		case 2:
			actor->_B80 = 2;
			actor->_B78 = actor->_B9C / 100.0f;
			actor->_B94 = 0;
			actor->_B98 = 0;
			break;
		}
		break;
	case 13:
		switch (actor->mRumble) {
		case 0:
			break;
		case 1:
			if (lbl_8025C400.mStatus.mFlags & 0x800000000ULL) {
				CPadOne* rumble = &lbl_8025C400.mPad.mPad[0];
				if (rumble->mConnected) {
					HSD_PadRumbleOn(rumble->mPort);
				}
				actor->mRumbleTimer = 24 / frames;
				actor->mRumble++;
			} else {
				actor->mRumble = 0;
			}
			break;
		case 2:
			if (--actor->mRumbleTimer < 0) {
				CPadOne* rumble = &lbl_8025C400.mPad.mPad[0];
				if (rumble->mConnected) {
					HSD_PadRumbleOffN(rumble->mPort);
				}
				if (rumble->mConnected) {
					HSD_PadRumbleRemove(rumble->mPort);
				}
				actor->mRumble = 0;
			}
			break;
		}
		if (lbl_8025C400.mStatus.mFlags & 0x400) {
			return;
		}
		switch (actor->_B94) {
		case 1:
			actor->_B8C.mFlags &= ~4;
			actor->_B94 = 0;
			actor->_B98 = 0;
			break;
		}
		break;
	case 14:
		if (lbl_8025C400.mStatus.mFlags & 0x400) {
			return;
		}
		fn_8010B394(actor);
		break;
	}
	if (actor->_B8C.mFlags & 0x10) {
		return;
	}
	(actor->*lbl_8022C330[actor->GetItem()])(frames);
	if (!(actor->_B8C.mFlags & 4)) {
		actor->SetRotateY(actor->_B7C, 0.2f);
		actor->SetPos(actor->mMoveTranslate, 0);
	}
	fn_80090ED4(actor, frames);
	if (actor->GetKind() == 0x31B) {
		fn_80024D84(&actor->mDust.mPtcl, frames);
	}
	if (actor->GetKind() == 0x2C3) {
		fn_8010B7B4(actor, frames);
	}
	if (actor->_B8C.mFlags & 2) {
		for (u32 i = 0; i < 8; i++) {
			if (actor->mParts[i].mJoint != NULL && actor->mParts[i].mObj != NULL) {
				Mtx t;
				XObj* obj = actor->mParts[i].mObj;
				PSMTXTrans(t, obj->mMoveTranslate.x, obj->mMoveTranslate.y, obj->mMoveTranslate.z);
				PSMTXCopy(HSD_JObjGetMtxPtr(actor->mParts[i].mJoint), actor->mParts[i].mMtx);
				Vec scale;
				HSD_MtxGetScale(actor->mParts[i].mMtx, &scale);
				scale.x = fn_800621E0(actor->mParts[i].mObj).x / scale.x;
				scale.y = fn_800621E0(actor->mParts[i].mObj).y / scale.y;
				scale.z = fn_800621E0(actor->mParts[i].mObj).z / scale.z;
				Mtx s;
				PSMTXScale(s, scale.x, scale.y, scale.z);
				Mtx m;
				PSMTXConcat(actor->mParts[i].mMtx, s, m);
				PSMTXConcat(m, t, m);
				HSD_JObj* joint = actor->mParts[i].mObj->mJObj;
				MtxPtr mp       = m;
				HSD_JObjCopyMtx(joint, mp);
			}
		}
	}
}
