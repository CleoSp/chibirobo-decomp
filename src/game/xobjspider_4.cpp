#include "game/xobjspider_shared.h"

void XObjSpider::vfC8()
{
}

void XObjSpider::vfCC()
{
}

// NONMATCHING: the joint path and the two points are laid out differently on the stack
// the thread it hangs from, a line from its joint up to the ceiling
void XObjSpider::vfC0()
{
	if (mStatus.mFlags & 0x80) {
		Vec top;
		CJObj joint;
		u32 color = (u32)(90.0f * mThreadAlpha) | 0xFFFFFF00;
		joint.Attach(GetJoint(mJObj, sThreadPath, 1));
		top = GetJObjPos(joint.mJObj);
		CVec pos(top);
		UnkSpiderGame* game = GAME;
		HSD_StateInvalidate(-1);
		UnkSpiderGame* game2 = GAME;
		if (game2->_290 != NULL) {
			UnkSpiderRenderA* a = game->_290;
			if (a->_0C == 0) {
				a->_0C = 1;
			}
			if (a->_10 == 0) {
				a->_10 = 1;
			}
			if (a->_14 == 0) {
				a->_14 = 1;
			}
			if (a->_18 == 0) {
				a->_18 = 1;
			}
		}
		UnkSpiderRenderB* b = game->_294;
		if (b != NULL) {
			if (b->_74 == 0) {
				b->_74 = 1;
			}
			if (b->_78 == 0) {
				b->_78 = 1;
			}
			if (b->_7C == 0) {
				b->_7C = 1;
			}
		}
		UnkSpiderRenderC* c = game->_298;
		if (c != NULL) {
			if (c->_30 == 0) {
				c->_30 = 1;
			}
		}
		UnkSpiderGame* game3 = GAME;
		if (game2->_290 != NULL) {
			fn_8019B278(game3->_290, 1);
		}
		if (game3->_294 != NULL) {
			fn_8019AED0(game3->_294, 1);
		}
		if (game3->_298 != NULL) {
			fn_8019AE94(game3->_298, 1);
		}
		fn_8019AFA0(GAME->_294, 4);
		fn_8019B34C(game2->_290, 0x12, 5);
		fn_801992BC(lbl_8025C400, 0, color, color, pos.x, pos.y, pos.z, top.x, 300.0f, top.z);
		fn_8019B34C(game2->_290, 0xC, 5);
	}
}

// NONMATCHING: argv[1] is read into another register
// the event commands: 100 ties another object to one of its joints, or lets go of all
// of them, anything else is kept for the states to read
extern "C" s32 fn_800F2454(XObjSpider* self, s32 argc, s32* argv)
{
	for (int i = 0; i < argc; i++) { }
	switch (argv[0]) {
	case 100:
		if (argv[1] == 0) {
			self->mStatus.mFlags &= ~0x20000000;
			for (u32 i = 0; i < 8; i++) {
				if (self->mTies[i].mObj != NULL) {
					HSD_JObjClearUserMtx(self->mTies[i].mObj->mJObj);
					self->mTies[i].mObj   = NULL;
					self->mTies[i].mJoint = NULL;
				}
			}
		} else {
			u32 i;
			for (i = 0; self->mTies[i].mObj != NULL && i < 8; i++) { }
			if (i < 8) {
				OSReport("id = %d, argv[1]=%d\n", i);
				self->mStatus.mFlags |= 0x20000000;
				if (argv[1] >= 10000) {
					self->mTies[i].mObj = FindEvtObj(argv[1]);
				} else {
					self->mTies[i].mObj = GetObj(argv[1]);
				}
				if (self->mTies[i].mObj == NULL) {
					OSReport("NULL==============\n");
				}
				s32 path[8];
				s32 depth = argv[2];
				for (int j = 0; j < depth; j++) {
					path[j] = argv[3 + j];
				}
				self->mTies[i].mJoint = GetJointByPath(self->mJObj, path, depth);
				Mtx m;
				XObj* obj = self->mTies[i].mObj;
				PSMTXTrans(m, obj->mMoveTranslate.x, obj->mMoveTranslate.y, obj->mMoveTranslate.z);
				PSMTXConcat(HSD_JObjGetMtxPtr(self->mTies[i].mJoint), m, self->mTies[i].mMtx);
				HSD_JObjCopyMtx(self->mTies[i].mObj->mJObj, self->mTies[i].mMtx);
			}
		}
		break;
	default:
		if (argc < 1) {
			return 0;
		}
		self->mArgs[0] = argv[0];
		if (argc < 2) {
			self->mArgs[1] = 0;
		} else {
			self->mArgs[1] = argv[1];
		}
		if (argc < 3) {
			self->mArgs[2] = 0;
		} else {
			self->mArgs[2] = argv[2];
		}
		if (argc < 4) {
			self->mArgs[3] = 0;
		} else {
			self->mArgs[3] = argv[3];
		}
		if (argc < 5) {
			self->mArgs[4] = 0;
		} else {
			self->mArgs[4] = argv[4];
		}
		if (argc < 6) {
			self->mArgs[5] = 0;
		} else {
			self->mArgs[5] = argv[5];
		}
		break;
	}
	return 0;
}

void XObjSpider::vf138(const Vec* pos, f32 f)
{
	if (mStatus.mFlags & 2) {
		return;
	}
	if (mStatus.mFlags & 0x10) {
		return;
	}
	if (mStatus.mFlags & 0x8000) {
		return;
	}
	if (fn_8009C1BC((u8*)GAME->mChr + 0x12DFC, 2) < 2) {
		return;
	}
	fn_8008A6C0(this, pos, f);
	mStatus.mFlags |= 2;
	Vec v    = { 0.0f, 0.0f, 0.0f };
	v        = *pos;
	mHitPos  = v;
	mHitDist = f;
}

s32 XObjSpider::vf100(s32 a, s32 b)
{
	mStatus.mFlags |= 1;
	return b;
}

// NONMATCHING: the return temp and the target sit the other way on the stack
// shoots the part from its joint, ahead of it or at the robot
extern "C" void fn_800F0B7C(XObjSpider* self)
{
	CJObj joint;
	s32 path[8] = { 0, 0, 0, 0, 1, 1, 0, 0 };
	joint.Attach(GetJoint(self->mJObj, path, 8));
	CVec pos(GetJObjPos(joint.mJObj));
	switch (self->Get25C()) {
	case 0:
		self->mShotTo.x = pos.x + 72.0f * (f32)sin(self->mDir);
		self->mShotTo.y = pos.y;
		self->mShotTo.z = pos.z + 72.0f * (f32)cos(self->mDir);
		break;
	case 12:
	case 13:
		self->mShotTo.x = pos.x + 72.0f * (f32)sin(HALF_PI + self->mDir);
		self->mShotTo.y = pos.y + 72.0f * (f32)cos(HALF_PI + self->mDir);
		self->mShotTo.z = pos.z;
		break;
	default:
		Vec chr       = GAME->mChr->mMoveTranslate;
		self->mShotTo = chr;
		break;
	}
	self->_A44 = 1;
	self->_A48 = 10;
	self->mPart.mStatus.mFlags |= 1;
	self->mPart.mPos.x = pos.x;
	self->mPart.mPos.y = pos.y;
	self->mPart.mPos.z = pos.z;
	XObj* obj          = (XObj*)(GAME_POOL2->mData + self->_258 * GAME_POOL2->mSize);
	CVec to(self->mShotTo);
	self->mPart.mRotY  = atan2(to.y - self->mPart.mPos.y, to.x - self->mPart.mPos.x);
	self->mPart.mObj   = obj;
	self->mPart.mState = 1;
	fn_8001E904(GAME_SOUND, 0x12A, 0xFF);
}

// NONMATCHING: the matrix pointers and the tie loop take other registers
// reads the object it holds up and keeps it on its joint, and moves the tied objects
// along with their joints
extern "C" void fn_800F1068(XObjSpider* self, u32 frames)
{
	switch (self->_B60) {
	case 0:
		break;
	case 1:
		if (!IsFree__12XObjActorTaoFv(GAME->_97AC)) {
			break;
		}
		if (self->_B5C != NULL) {
			self->_B5C->Release();
		}
		if (self->_B58 >= 0) {
			self->_B5C = ReadObj(self->_B58);
		} else {
			self->_B5C = NULL;
		}
		self->_B60++;
	case 2:
		if (self->_B5C != NULL) {
			if (self->_B5C->mFlags & 0x0040000000000000ULL) {
				break;
			}
			GAME->mFlags |= 0x1000;
		}
		self->_B60++;
	case 3:
		if (self->_B58 < 0 || self->_B5C == NULL) {
			break;
		}
		self->_B5C->SetScale(1.0f, 1.0f, 1.0f, 0);
		CJObj joint;
		joint.Attach(GetJoint(self->mJObj, sHoldPath, 1));
		PSMTXCopy(HSD_JObjGetMtxPtr(joint.mJObj), self->_B64);
		Vec scale;
		HSD_MtxGetScale(self->_B64, &scale);
		scale.x = fn_800621E0(self->_B5C).x / scale.x;
		scale.y = fn_800621E0(self->_B5C).y / scale.y;
		scale.z = fn_800621E0(self->_B5C).z / scale.z;
		Mtx s;
		PSMTXScale(s, scale.x, scale.y, scale.z);
		Mtx t;
		Mtx m;
		PSMTXConcat(self->_B64, s, m);
		PSMTXTrans(t, 0.0f, 5.0f, 0.0f);
		PSMTXConcat(m, t, m);
		HSD_JObjCopyMtx(self->_B5C->mJObj, m);
		self->_B5C->mFlags &= ~0x0200000000000000ULL;
		break;
	}
	if (self->mStatus.mFlags & 0x20000000) {
		for (u32 i = 0; i < 8; i++) {
			if (self->mTies[i].mJoint != NULL && self->mTies[i].mObj != NULL) {
				Mtx m;
				XObj* obj = self->mTies[i].mObj;
				PSMTXTrans(m, obj->mMoveTranslate.x, obj->mMoveTranslate.y, obj->mMoveTranslate.z);
				PSMTXConcat(HSD_JObjGetMtxPtr(self->mTies[i].mJoint), m, self->mTies[i].mMtx);
				HSD_JObjCopyMtx(self->mTies[i].mObj->mJObj, self->mTies[i].mMtx);
			}
		}
	}
}

// the hits: it loses life when the robot hits it, more for a hard hit, then blinks red
// for a while. and when the robot touches it, the robot gets hit
extern "C" void fn_800F1B10(XObjSpider* self, u32 frames)
{
	switch (self->_B94) {
	case 0:
		if ((self->mStatus.mFlags & 2) && !(self->mStatus.mFlags & 0x10)) {
			fn_8001E660(GAME_SOUND, 0x12A);
			self->mStatus.mFlags &= ~2;
			if (self->mHitDist >= fn_8006EBC8(0x1F) / 100.0f) {
				self->mLife -= 5;
				if (self->mLife <= 0) {
					self->mLife = 0;
				}
			} else if (--self->mLife <= 0) {
				self->mLife = 0;
			}
			if (self->mLife <= 0) {
				self->mSpeed = 0.0f;
				self->SetState(3);
				self->mStatus.mFlags |= 0x10;
			} else {
				self->_B94 = 1;
				self->_B98 = 0;
				self->mStatus.mFlags |= 0x10;
			}
		}
		break;
	case 1:
		if (self->mTimer % 30 < 15) {
			self->SetColor(1, 0x80, 0, 0, 0.0f);
		} else {
			self->SetColor(1, 0, 0, 0, 0.0f);
		}
		self->_B98++;
		if (self->_B98 > 20) {
			self->mStatus.mFlags &= ~0x10;
			self->SetColor(1, 0, 0, 0, 0.0f);
			self->_B98 = 0;
			self->_B94 = 0;
		}
		break;
	}
	Vec chr = GAME->mChr->mMoveTranslate;
	if (self->mWall == 3) {
		f32 dx = chr.x - self->_A14.x;
		f32 dy = 8.0f + chr.y - self->_A14.y;
		f32 dz = chr.z - chr.z;
		if (sqrtf(dx * dx + dy * dy + dz * dz) < 8.0f && (self->mStatus.mFlags & 0x800)) {
			GAME->mChr->vf138(&chr, 7.0f);
			HitCarried(self);
		}
	} else {
		f32 dx = chr.x - self->_A14.x;
		f32 dy = 0.0f;
		f32 dz = chr.z - self->_A14.z;
		f32 y  = chr.y;
		if (sqrtf(dx * dx + dy * dy + dz * dz) < 2.0f && (self->mStatus.mFlags & 0x800) && Abs(y - self->mMoveTranslate.y) < 4.0f) {
			GAME->mChr->vf138(&chr, 7.0f);
			HitCarried(self);
		}
	}
	fn_800F1730(self, frames);
	fn_800F0090(self, frames);
}

// does what the last event command said
extern "C" void fn_800F1730(XObjSpider* self, u32 frames)
{
	switch (self->mArgs[0]) {
	case 0:
		break;
	case 1:
		self->SetState(3);
		self->mArgs[0] = 0;
		break;
	case 2:
		if (self->mStatus.mFlags & 0x10000) {
			GAME_5D08C = 1;
		} else {
			GAME_5D08C = 0;
		}
		self->mArgs[0] = 0;
		break;
	case 3:
		if (self->mStatus.mFlags & 0x80000) {
			GAME_5D08C = 1;
		} else {
			GAME_5D08C = 0;
		}
		self->mArgs[0] = 0;
		break;
	case 4:
		self->mStatus.mFlags |= 0x200000;
		self->mArgs[0] = 0;
		break;
	case 5:
		self->mStatus.mFlags |= 0x400000;
		self->mArgs[0] = 0;
		break;
	case 6:
		self->mFlags |= 0x100000;
		self->mArgs[0] = 0;
		break;
	case 7:
		if (self->mArgs[1] == 1) {
			self->mStatus.mFlags &= ~0x8000000;
		} else {
			self->mStatus.mFlags |= 0x8000000;
		}
		self->mArgs[0] = 0;
		self->mArgs[1] = 0;
		break;
	case 8:
		fn_800F38F4(self);
		self->mArgs[0] = 0;
		break;
	case 9:
		self->mHome.x          = self->mArgs[1] / 100.0f;
		self->mHome.y          = self->mArgs[2] / 100.0f;
		self->mHome.z          = self->mArgs[3] / 100.0f;
		self->_A60.x           = self->mHome.x;
		self->_A60.y           = self->mHome.y;
		self->_A60.z           = self->mHome.z;
		self->mMoveTranslate.x = self->mHome.x;
		self->mMoveTranslate.y = (f32) * (s32*)&self->_25C[4] + self->mHome.y; // _260, the height it hangs at
		self->mMoveTranslate.z = self->mHome.z;
		self->mMoveTranslate.mTarget.Set(self->mMoveTranslate.x, self->mMoveTranslate.y, self->mMoveTranslate.z);
		self->mArgs[0] = 0;
		break;
	case 10:
		self->mArea.x  = self->mArgs[1] / 100.0f;
		self->mArea.y  = self->mArgs[2] / 100.0f;
		self->mArea.z  = self->mArgs[3] / 100.0f;
		self->mAreaW   = self->mArgs[4];
		self->mAreaD   = self->mArgs[5];
		self->mArgs[0] = 0;
		break;
	case 11:
		self->mArgs[0] = 0;
		break;
	case 12:
		self->mArgs[0] = 0;
		break;
	case 13:
		self->mStatus.mFlags |= 0x10000000;
		self->mArgs[0] = 0;
		break;
	case 14:
		self->_B58     = self->mArgs[1];
		self->_B60     = 1;
		self->mArgs[0] = 0;
		break;
	case 15:
		self->_B60 = 0;
		self->_B58 = -1;
		if (self->_B5C != NULL) {
			self->_B5C->Release();
		}
		self->mArgs[0] = 0;
		break;
	case 16:
		self->mArgs[0] = 0;
		break;
	case 17:
		self->mArgs[0] = 0;
		break;
	case 18:
		self->mArgs[0] = 0;
		break;
	case 19:
		self->SetState(5);
		self->mArgs[0] = 0;
		break;
	}
}
