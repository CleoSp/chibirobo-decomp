#include "game/scrc_shared.h"

extern "C" {

void fn_8010904C(XObjActor3* self);
void fn_801095E8(XObjActor3* self);
void fn_8010AD88(XObjActor3* self);
void fn_8010FB1C(XObjActor3* self);
void fn_8010B394(XObjActor3* self);
void fn_8010B528(XObjActor3* self);
void fn_8010AF74(XObjActor3* self);
void fn_8010B7B4(XObjActor3* self, u32 frames);
void fn_80024D84(CPtcl* ptcl, u32 frames);
void fn_8010FB1C(XObjActor3* self);
void fn_8010B528(XObjActor3* self);
void fn_8010B7B4(XObjActor3* self, u32 frames);
void fn_8010B394(XObjActor3* self);
void fn_8010AD88(XObjActor3* self);
void fn_8010AF74(XObjActor3* self);

// the per-kind script commands, the kind's own function, the carried objects
void fn_80109E80(XObjActor3* self, u32 frames)
{
	switch (self->GetInfoKind()) {
	case 8:
		self->mStatus.mFlags |= 4;
		break;
	case 9:
		self->vf148();
		if (GAME->mFlags & 0x400) {
			return;
		}
		break;
	case 10:
		if (GAME->mFlags & 0x400) {
			return;
		}
		switch (self->_B94[0]) {
		case 1:
			self->mStatus.mFlags &= ~4;
			self->_B94[0] = 0;
			self->_B94[1] = 0;
			break;
		}
		break;
	case 11:
	case 12:
		if (GAME->mFlags & 0x400) {
			return;
		}
		switch (self->_B94[0]) {
		case 1:
			if (self->_B94[1] != 0) {
				self->mStatus.mFlags &= ~4;
			} else {
				self->mStatus.mFlags |= 4;
				self->_B80 = 0;
			}
			self->_B94[0] = 0;
			self->_B94[1] = 0;
			break;
		case 2:
			self->_B80    = 2;
			self->_B78    = self->_B94[2] / 100.0f;
			self->_B94[0] = 0;
			self->_B94[1] = 0;
			break;
		}
		break;
	case 13:
		switch (self->mRumble) {
		case 0:
			break;
		case 1:
			if (GAME->mFlags & 0x800000000ULL) {
				GAME->StartRumble();
				self->mRumbleTimer = 24 / frames;
				self->mRumble++;
			} else {
				self->mRumble = 0;
			}
			break;
		case 2:
			if (--self->mRumbleTimer < 0) {
				UnkActor3Game* game = GAME;
				game->StopRumble();
				self->mRumble = 0;
			}
			break;
		}
		if (GAME->mFlags & 0x400) {
			return;
		}
		switch (self->_B94[0]) {
		case 1:
			self->mStatus.mFlags &= ~4;
			self->_B94[0] = 0;
			self->_B94[1] = 0;
			break;
		}
		break;
	case 14:
		if (GAME->mFlags & 0x400) {
			return;
		}
		fn_8010B394(self);
		break;
	}
	if (self->mStatus.mFlags & 0x10) {
		return;
	}
	(self->*lbl_8022C330[self->GetInfoKind()])(frames);
	if (!(self->mStatus.mFlags & 4)) {
		self->SetRotateY(self->_B7C, 0.2f);
		self->SetMoveTranslate(&self->mMoveTranslate, 0);
	}
	fn_80090ED4(self, frames);
	if (self->GetDescKind() == 0x31B) {
		fn_80024D84(&self->mPtcl.mPtcl, frames);
	}
	if (self->GetDescKind() == 0x2C3) {
		fn_8010B7B4(self, frames);
	}
	if (self->mStatus.mFlags & 2) {
		for (u32 i = 0; i < 8; i++) {
			if (self->mSlots[i].mJoint != NULL && self->mSlots[i].mObj != NULL) {
				Mtx t;
				XObj* obj = self->mSlots[i].mObj;
				PSMTXTrans(t, obj->mMoveTranslate.x, obj->mMoveTranslate.y, obj->mMoveTranslate.z);
				PSMTXCopy(HSD_JObjGetMtxPtr(self->mSlots[i].mJoint), self->mSlots[i].mMtx);
				Vec scale;
				HSD_MtxGetScale(self->mSlots[i].mMtx, &scale);
				scale.x = fn_800621E0(self->mSlots[i].mObj).x / scale.x;
				scale.y = fn_800621E0(self->mSlots[i].mObj).y / scale.y;
				scale.z = fn_800621E0(self->mSlots[i].mObj).z / scale.z;
				Mtx s;
				PSMTXScale(s, scale.x, scale.y, scale.z);
				Mtx m;
				PSMTXConcat(self->mSlots[i].mMtx, s, m);
				PSMTXConcat(m, t, m);
				HSD_JObj* joint = self->mSlots[i].mObj->mJObj;
				MtxPtr mp       = m;
				HSD_JObjCopyMtx(joint, mp);
			}
		}
	}
}
void fn_8010904C(XObjActor3* self);
}
