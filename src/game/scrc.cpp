#include "game/particle_srt.h"
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

// the event commands: 100 carries another object at one of its joints, or lets go of all
// of them, anything else is kept for the kind's function to read
// NONMATCHING: argv[1] and the 0x40B item loop take other registers, as in game/xobjspider
s32 fn_80108794(XObjActor3* self, s32 argc, s32* argv)
{
	switch (argv[0]) {
	case 100:
		if (argv[1] == 0) {
			self->mStatus.mFlags &= ~2;
			for (u32 i = 0; i < 8; i++) {
				if (self->mSlots[i].mObj != NULL) {
					HSD_JObjClearUserMtx(self->mSlots[i].mObj->mJObj);
					self->mSlots[i].mObj   = NULL;
					self->mSlots[i].mJoint = NULL;
				}
			}
		} else {
			u32 i;
			for (i = 0; self->mSlots[i].mObj != NULL && i < 8; i++) { }
			if (i < 8) {
				OSReport("id = %d, argv[1]=%d\n", i);
				self->mStatus.mFlags |= 2;
				if (argv[1] >= 10000) {
					self->mSlots[i].mObj = FindEvtObj(argv[1]);
				} else {
					self->mSlots[i].mObj = GetObj(argv[1]);
				}
				if (self->mSlots[i].mObj == NULL) {
					OSReport("NULL===============\n");
				}
				s32 path[8];
				s32 depth = argv[2];
				for (s32 j = 0; j < depth; j++) {
					path[j] = argv[3 + j];
				}
				self->mSlots[i].mJoint = GetJointByPath(self->mJObj, path, depth);
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
		break;
	default:
		self->_B94[0]  = argv[0];
		self->_B94[1]  = argv[1];
		self->_B94[2]  = argv[2];
		self->_B94[3]  = argv[3];
		self->_B94[4]  = argv[4];
		self->_B94[5]  = argv[5];
		self->_B94[6]  = argv[6];
		self->_B94[7]  = argv[7];
		self->_B94[8]  = argv[8];
		self->_B94[9]  = argv[9];
		self->_B94[10] = 0;
		switch (self->GetInfoKind()) {
		case 9:
			fn_8010B528(self);
			break;
		case 0x13:
			fn_8010AF74(self);
			break;
		case 8:
			switch (self->_B94[0]) {
			case 0:
				break;
			case 1:
				s32 kind = -1;
				if (self->mStatus.Check(0x100)) {
					kind = 0x40B;
					self->mStatus.mFlags &= ~0x100;
				}
				HideItems(kind);
				switch (self->_B94[1]) {
				case 0x40B:
					for (s32 i = 0; i < 0x120; i++) {
						XObj* obj = GAME_ITEMS->Get(i);
						if (self->_B94[1] == obj->GetDescKind()) {
							self->mHeld = obj;
							obj->mFlags |= 0x10000ULL;
							switch (self->_B94[1]) {
							case 0x40B:
								self->mStatus.mFlags |= 0x100;
								obj->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
								break;
							}
						}
					}
					break;
				default:
					if (self->mHeld != NULL) {
						HSD_JObjClearUserMtx(self->mHeld->mJObj);
						self->mHeld = NULL;
					}
					break;
				}
				self->_B94[0] = 0;
				break;
			}
			break;
		}
		break;
	}
	return 0;
}
void fn_8010904C(XObjActor3* self);

void fn_80108728(XObjActor3* self, const Vec* pos, f32 f)
{
	if (self->GetInfoKind() == 1) {
		fn_8008A6C0(self, pos, f);
	} else if (!self->mStatus.Check(1)) {
		self->mStatus.mFlags |= 1;
	}
}

void fn_80108694(XObjActor3* self, s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate, f32 blend,
                 f32 blendRate)
{
	fn_800902A4(self, id, force, jointAnim, matAnim, shapeAnim, frame, rate, blend, blendRate);
	if (self->GetDescKind() == 0x31B) {
		if (self->mAnimId == 7) {
			self->mSfx = fn_8001E904(GAME_SOUND, 0xB0000, 0xFF);
		} else if (HSD_AudioSFXCheck(self->mSfx)) {
			HSD_AudioSFXKeyOff(self->mSfx);
		}
	}
}
} // extern "C"

// NONMATCHING: register allocation
void XObjActor3::vf148()
{
	if ((mFlags & 1) && (mFlags & 0x10000) && _258 == 3) {
		s32 id    = mAnimId;
		f32 frame = GetAnimFrame(mJObj);
		mSe.Play(id, fn_8001DF00(GAME_SOUND, &mMoveTranslate, 0), frame);
	}
}
