#include "game/particle_srt.h"
#include "game/xobjactortoy.h"

// XObjActorToy, the toys. which one comes from the stage table, see GetDescKind. kind 0xF
// carries two more XObjs, its parts

void XObjActorToy::vf150()
{
	mState = NULL;
}

void XObjActorToy::vf14C(u32 frames)
{
	if (mState) {
		(this->*mState)(frames);
	}
}

XObjActorToy::~XObjActorToy()
{
}

bool XObjActorToy::fn_8011566C() const
{
	if (!(mFlags & 1)) {
		return true;
	}
	return !mStatus.Check(0x2000);
}

// the texture animation of the jobj's dobj after skip others
static inline HSD_AObj* GetTexAObj(HSD_JObj* jobj, s32 skip)
{
	if (jobj == NULL) {
		return NULL;
	}
	HSD_DObj* dobj = HSD_JObjGetDObj(jobj);
	for (s32 i = 0; i < skip; i++) {
		if (dobj == NULL) {
			return NULL;
		}
		dobj = HSD_DObjGetNext(dobj);
	}
	HSD_MObj* mobj = HSD_DObjGetMObj(dobj);
	if (mobj == NULL) {
		return NULL;
	}
	HSD_TObj* tobj = HSD_MObjGetTObj(mobj);
	if (tobj == NULL) {
		return NULL;
	}
	return tobj->aobj;
}

void XObjActorToy::vf164(f32 frame)
{
	switch (GetDescKind()) {
	case 0x1FC:
		if (mAnimId == 0x3F6) {
			s32 path[3]    = { 0, 2, 0 };
			HSD_AObj* aobj = GetTexAObj(GetJointByPath(mJObj, path, 3), 4);
			frame          = aobj != NULL ? HSD_AObjGetCurrFrame(aobj) : 0.0f;
		}
		if (fn_8001E6C8(GAME_SOUND, 0x10001F)) {
			if (mAnimId != 0x10) {
				fn_8001E660(GAME_SOUND, 0x10001F);
			} else {
				fn_8001E468(GAME_SOUND, 0x10001F, fn_8001DF00(GAME_SOUND, &mMoveTranslate, 0), 0);
			}
		}
		break;
	case 0x159:
		if (fn_8001E6C8(GAME_SOUND, 0xE0008) && mAnimId != 7) {
			fn_8001E660(GAME_SOUND, 0xE0008);
		}
		break;
	}
	fn_80106BE0(this, frame);
}

// one of the toys, see GAME_POOL_975C
static inline XObjActorToy* GetToy(s32 i)
{
	return (XObjActorToy*)GAME_POOL_975C->Get(i);
}

static inline MtxPtr ToyMatrix(const CJObj& obj)
{
	return HSD_JObjGetMtxPtr(obj.mJObj);
}

// the 0x157 toy carries the 0x156 ones along, with what they hold in their hand
void XObjActorToy::vf130()
{
	switch (GetDescKind()) {
	case 0xF:
		break;
	case 0x6F:
		break;
	case 0x110:
		break;
	case 0x117:
		break;
	case 0x156:
		break;
	case 0x157:
		mToyStatus.mFlags |= 2;
		for (s32 i = 0; i < 15; i++) {
			XObjActorToy* toy = GetToy(i);
			if (toy->GetDescKind() == 0x156 && (toy->mToyStatus.mFlags & 1)) {
				toy->SetTranslateX(mMoveTranslate.x, 0);
				toy->SetTranslateZ(mMoveTranslate.z, 0);
				if (toy->_A4C >= 0 && toy->_A50 != NULL) {
					CJObj joint;
					s32 path[4] = { 0, 2, 4, 0 };
					Mtx hand;
					Mtx trans;
					Mtx rot;
					Mtx mtx;
					joint.Attach(GetJointFromRoot(toy->mJObj, 4, path));
					toy->_A50->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					PSMTXCopy(ToyMatrix(joint), hand);
					PSMTXRotRad(rot, 'z', 0.0f);
					PSMTXConcat(hand, rot, mtx);
					PSMTXTrans(trans, 0.0f, 2.0f, 2.0f);
					PSMTXConcat(mtx, trans, mtx);
					HSD_JObj* jobj = toy->_A50->mJObj;
					MtxPtr m       = mtx;
					HSD_JObjCopyMtx(jobj, m);
					if (mFlags & 0x10000) {
						toy->mFlags |= 0x10000ULL;
					} else {
						toy->mFlags &= ~0x10000ULL;
					}
				}
			}
		}
		break;
	case 0x159:
		break;
	case 0x1D3:
		break;
	case 0x1FB:
		break;
	case 0x1FC:
		break;
	case 0x25E:
		break;
	case 0x26C:
		break;
	case 0x26D:
		break;
	case 0x485:
		break;
	}
}

extern "C" s32 fn_80115F18(XObjActorToy* self, s32 argc, s32* argv)
{
	fn_801070A4(self, argc, argv);
	if (argc < 1) {
		return 0;
	}
	self->_9E0 = argv[0];
	if (argc < 2) {
		self->_9E4 = 0;
	} else {
		self->_9E4 = argv[1];
	}
	for (int i = 0; i < 8; i++) {
		self->_9E8[i] = argv[i + 2];
	}
	return 0;
}

void XObjActorToy::vfC8()
{
	fn_80107600(this);
	if (mPart0.mFlags & 0x10000) {
		mPart0.vfC8();
	}
	if (mPart1.mFlags & 0x10000) {
		mPart1.vfC8();
	}
}

void XObjActorToy::Release()
{
	mPart0.Release();
	mPart1.Release();
	fn_800911C0(this);
	mLoadStep = 0;
	_1F90     = 0;
}

BOOL XObjActorToy::vf9C(void* info)
{
	switch (GetDescKind()) {
	case 0xF:
		switch (mLoadStep) {
		case 0:
			if (!fn_80107F18(this, info)) {
				break;
			}
			mLoadStep++;
		case 1:
			if (!mPart0.vf9C(&CoordInfo(GetSpawnData(0x2A)))) {
				break;
			}
			mPart0.mFlags &= ~0x10000ULL;
			mPart0.SetMoveTranslate(&mMoveTranslate, 0);
			mLoadStep++;
		case 2:
			if (!mPart1.vf9C(&CoordInfo(GetSpawnData(0x2B)))) {
				break;
			}
			mPart1.mFlags &= ~0x10000ULL;
			mPart1.SetMoveTranslate(&mMoveTranslate, 0);
			mLoadStep++;
		}
		if (mLoadStep > 2) {
			mLoadStep = 0;
			return TRUE;
		}
		return FALSE;
	}
	return fn_80107F18(this, info) != FALSE;
}

extern "C" const u8 lbl_8022C7E0[];

void XObjActorToy::Reset()
{
	const u8* data = lbl_8022C7E0;
	fn_801081F0(this);
	mLoadStep = 0;
	_1F90     = 0;
	mPart0.Reset();
	mPart1.Reset();
	switch (GetDescKind()) {
	case 0x6F:
		mSe.mTable = (const UnkActorSeEntry*)(data + 0x374);
		break;
	case 0x110:
		mSe.mTable = (const UnkActorSeEntry*)(data + 0x68);
		break;
	case 0x117:
		mSe.mTable = (const UnkActorSeEntry*)(data + 0x50C);
		break;
	case 0x159:
		mSe.mTable = (const UnkActorSeEntry*)(data + 0x4DC);
		break;
	case 0x1FB:
		mSe.mTable = (const UnkActorSeEntry*)(data + 0x62C);
		break;
	case 0x1FC:
		mSe.mTable = (const UnkActorSeEntry*)(data + 0x3EC);
		break;
	case 0x25E:
		mSe.mTable = (const UnkActorSeEntry*)(data + 0x74C);
		break;
	case 0x317:
		mSe.mTable = (const UnkActorSeEntry*)(data + 0x68C);
		break;
	case 0x26C:
	case 0x485:
		mSe.mTable = (const UnkActorSeEntry*)(data + 0x4AC);
		break;
	default:
		mSe.mTable = NULL;
		break;
	}
}

void XObjActorToy::vf94()
{
	fn_80108014(this);
	mLoadStep = 0;
	_1F90     = 0;
	mPart0.vf94();
	mPart1.vf94();
}

void XObjActorToy::Init(u32 id)
{
	fn_801083F0(this, id);
	mLoadStep = 0;
	_1F90     = 0;
	mPart0.Init(id);
	mPart1.Init(id);
}

extern "C" void fn_8011E8DC(XObjActorToy* self)
{
	switch (self->_9E0) {
	case 1:
		fn_800242A8(GAME_QUAKE, self->_9E4, self->_9E8[0], self->_9E8[1], 0.0f, 0.0f, 0.0f, self->_9E8[2] / 100.0f);
		self->_9E0 = 0;
		break;
	}
}

extern "C" void fn_8011E9AC(XObjActorToy* self)
{
	switch (self->_9E0) {
	case 0:
		self->_9E0 = 0;
		break;
	case 1:
		fn_800242A8(GAME_QUAKE, self->_9E4, self->_9E8[0], self->_9E8[1], 0.0f, 0.0f, 0.0f, self->_9E8[2] / 100.0f);
		self->_9E0 = 0;
		break;
	case 2:
		switch (self->_9E4) {
		case 0:
			self->mToyStatus.mFlags |= 1;
			self->_9E0 = 0;
			break;
		case 1:
			self->mToyStatus.mFlags &= ~1;
			self->_9E0 = 0;
			break;
		}
		break;
	}
}

extern "C" void fn_8011EAEC(XObjActorToy* self)
{
	switch (self->_9E0) {
	case 0:
		break;
	case 1:
		self->_A4C = self->_9E4;
		self->SetState(&XObjActorToy::StateA);
		self->_9E0 = 0;
		break;
	case 2:
		self->_A4C = -1;
		if (self->_A50 != NULL) {
			self->_A50->Release();
		}
		self->_9E0 = 0;
		break;
	}
}

extern "C" void fn_8011EB98(XObjActorToy* self)
{
	switch (self->_9E0) {
	case 0:
		break;
	case 1:
		self->mToyStatus.mFlags |= 8;
		self->_9E0 = 0;
		break;
	case 2:
		if (self->mFlags & 0x10000) {
			lbl_80367A2C[self->_9E4] = 1;
		}
		self->_9E0 = 0;
		break;
	}
}

extern "C" void fn_8011EC18(XObjActorToy* self)
{
	switch (self->_9E0) {
	case 1:
		self->_9E0 = 0;
		break;
	case 2:
		if (self->mMoveTranslate.y <= 2.0f) {
			lbl_80367A2C[self->_9E4] = 1;
		}
		self->_9E0 = 0;
		break;
	case 3:
		if (self->mToyStatus.mFlags & 0x10) {
			lbl_80367A2C[self->_9E4] = 1;
		}
		self->_9E0 = 0;
		break;
	case 4:
		self->_A4C = self->_9E4;
		self->SetState(&XObjActorToy::StateA);
		self->_9E0 = 0;
		break;
	case 5:
		self->_A4C = -1;
		if (self->_A50 != NULL) {
			self->_A50->Release();
		}
		self->_9E0 = 0;
		break;
	}
}

extern "C" void fn_8011ED48(XObjActorToy* self)
{
	switch (self->_9E0) {
	case 0:
		break;
	case 1:
		switch (self->_9E4) {
		case 0: {
			self->mToyStatus.mFlags &= ~1;
			self->_984 = 0.0f;
			self->_990 = 0;
			CVec pos(self->mMoveTranslate.x, self->mMoveTranslate.y, self->mMoveTranslate.z);
			self->SetMoveDir(&pos, 0.0f);
			break;
		}
		case 1:
			self->mToyStatus.mFlags |= 1;
			self->_9C8.x = self->_9E8[0] / 100.0f;
			self->_9C8.y = self->_9E8[1] / 100.0f;
			self->_9C8.z = self->_9E8[2] / 100.0f;
			self->_990   = 1;
			break;
		}
		self->_9E0 = 0;
		break;
	}
}

// hides the item the toy dropped, the _C84 bits say which
extern "C" void fn_8011E3A4(XObjActorToy* self)
{
	s32 id = -1;
	if (self->_C84.Check(1)) {
		id = 0x3D;
		self->_C84.mFlags &= ~1;
	} else if (self->_C84.Check(2)) {
		id = 0x3E;
		self->_C84.mFlags &= ~2;
	} else if (self->_C84.Check(4)) {
		id = 0x3F;
		self->_C84.mFlags &= ~4;
	} else if (self->_C84.Check(8)) {
		id = 0x3B;
		self->_C84.mFlags &= ~8;
	} else if (self->_C84.Check(0x80)) {
		id = 0x21D;
		self->_C84.mFlags &= ~0x80;
	} else if (self->_C84.Check(0x10)) {
		id = 0x21E;
		self->_C84.mFlags &= ~0x10;
	} else if (self->_C84.Check(0x40)) {
		id = 0x341;
		self->_C84.mFlags &= ~0x40;
	}
	if (id < 0) {
		return;
	}
	switch (id) {
	case 0x3B:
	case 0x3D:
	case 0x3E:
	case 0x3F:
	case 0x21D:
	case 0x21E:
		for (s32 i = 0; i < 64; i++) {
			XObj* obj = GAME_POOL_9734->Get(i);
			if (id == obj->GetDescKind()) {
				obj->mFlags &= ~0x10000ULL;
			}
		}
		break;
	case 0x341:
		for (s32 i = 0; i < 288; i++) {
			XObj* obj = GAME_POOL_984C->Get(i);
			if (id == obj->GetDescKind()) {
				obj->mFlags &= ~0x10000ULL;
			}
		}
		break;
	}
}

extern "C" void fn_8011E688(XObjActorToy* self)
{
	switch (self->_9E0) {
	case 0:
		break;
	case 1:
		self->_A4C = self->_9E4;
		self->SetState(&XObjActorToy::StateA);
		self->_9E0 = 0;
		break;
	case 2:
		self->_A4C = -1;
		if (self->_A50 != NULL) {
			self->_A50->Release();
		}
		self->_9E0 = 0;
		break;
	case 3:
		fn_8011E3A4(self);
		switch (self->_9E4) {
		case 0x341:
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = GAME_POOL_984C->Get(i);
				if (self->_9E4 == obj->GetDescKind()) {
					self->_A84 = obj;
					obj->mFlags |= 0x10000ULL;
					switch (self->_9E4) {
					case 0x341:
						self->_C84.mFlags |= 0x40;
						break;
					}
				}
			}
			break;
		default:
			if (self->_A84 != NULL) {
				HSD_JObjClearUserMtx(self->_A84->mJObj);
				self->_A84 = NULL;
			}
			break;
		}
		self->_9E0 = 0;
		break;
	}
}

extern "C" {
void fn_80119774(XObjActorToy* self, u32 frames);
void fn_80119C24(XObjActorToy* self, u32 frames);
void fn_8011A050(XObjActorToy* self, u32 frames);
void fn_8011A390(XObjActorToy* self, u32 frames);
void fn_8011B7EC(XObjActorToy* self, u32 frames);
void fn_8011BE44(XObjActorToy* self, u32 frames);
void fn_8011C44C(XObjActorToy* self, u32 frames);
void fn_8011C834(XObjActorToy* self, u32 frames);
void fn_8011CF94(XObjActorToy* self, u32 frames);
}

// NONMATCHING: the loop counter and the pool base swap registers
void XObjActorToy::vf148(u32 frames)
{
	fn_80107750(this, frames);
	switch (GetDescKind()) {
	case 0xF:
		fn_8011CF94(this, frames);
		break;
	case 0x6F:
		fn_8011C834(this, frames);
		break;
	case 0x110:
		break;
	case 0x117:
		fn_8011C44C(this, frames);
		break;
	case 0x156:
		if (mToyStatus.mFlags & 1) {
			for (s32 i = 0; i < 15; i++) {
				if (GetToy(i)->GetDescKind() == 0x157) {
					Vec pos          = GetToy(i)->mMoveTranslate;
					mMoveTranslate.x = pos.x;
					mMoveTranslate.y = pos.y;
					mMoveTranslate.z = pos.z;
				}
			}
		}
		break;
	case 0x158:
		break;
	case 0x159:
		fn_8011BE44(this, frames);
		break;
	case 0x16C:
		break;
	case 0x1D3:
		break;
	case 0x1FB:
		fn_8011B7EC(this, frames);
		break;
	case 0x1FC:
		fn_8011A390(this, frames);
		break;
	case 0x26C:
	case 0x485:
		fn_8011A050(this, frames);
		break;
	case 0x26D:
		fn_80119C24(this, frames);
		break;
	case 0x25E:
		fn_80119774(this, frames);
		break;
	}
}

static inline s16 ToyItemId(const UnkPlayerItem& item)
{
	return item.mId;
}

static inline s16 ToyItemNum(const UnkPlayerItem& item)
{
	return item.mNum;
}

extern "C" void fn_8011EE8C(XObjActorToy* self)
{
	s16 id;
	s16 num;
	s32 count;
	switch (self->_9E0) {
	case 0:
		break;
	case 1:
		switch (self->_9E4) {
		case 0:
			self->mToyStatus.mFlags &= ~1;
			break;
		case 1:
			self->mToyStatus.mFlags |= 1;
			break;
		}
		self->_9E0 = 0;
		break;
	case 2:
		fn_8011E3A4(self);
		switch (self->_9E4) {
		case 0x3B:
		case 0x3D:
		case 0x3E:
		case 0x3F:
		case 0x21D:
		case 0x21E:
			for (s32 i = 0; i < 64; i++) {
				XObj* obj = GAME_POOL_9734->Get(i);
				if (self->_9E4 == obj->GetDescKind()) {
					obj->mFlags |= 0x10000ULL;
					self->_A84 = obj;
					switch (self->_9E4) {
					case 0x3B:
						self->_C84.mFlags |= 8;
						break;
					case 0x3D:
						self->_C84.mFlags |= 1;
						break;
					case 0x3E:
						self->_C84.mFlags |= 2;
						break;
					case 0x3F:
						self->_C84.mFlags |= 4;
						break;
					case 0x21D:
						self->_C84.mFlags |= 0x80;
						break;
					case 0x21E:
						self->_C84.mFlags |= 0x10;
						break;
					}
				}
			}
			break;
		default:
			if (self->_A84 != NULL) {
				HSD_JObjClearUserMtx(self->_A84->mJObj);
				self->_A84 = NULL;
			}
			break;
		}
		self->_9E0 = 0;
		break;
	case 3:
		lbl_80367A2C[self->_9E4] = fn_80106B50();
		self->_9E0               = 0;
		break;
	case 4:
		count = 0;
		for (s32 i = 0; i < 0x100; i++) {
			UnkPlayerItem* items = GAME_PLAYER->mItems;
			num                  = ToyItemNum(items[i]);
			id                   = ToyItemId(items[i]);
			if (fn_8006E44C(id, 4) && id >= 0 && num > 0) {
				count++;
			}
		}
		GAME_ITEMCOUNT = count;
		self->_9E0     = 0;
		break;
	case 5:
		self->_A4C = self->_9E4;
		self->SetState(&XObjActorToy::StateA);
		self->_9E0 = 0;
		break;
	case 6:
		self->_A4C = -1;
		if (self->_A50 != NULL) {
			self->_A50->Release();
		}
		self->_9E0 = 0;
		break;
	case 7:
		if (self->_9E4) {
			self->mParts[0].mId    = self->_9E8[0];
			self->mParts[0].mState = 1;
		} else {
			for (u32 j = 0; j < 4; j++) {
				if (self->mParts[j].mObj != NULL) {
					self->mParts[j].mObj->Release();
				}
				self->mParts[j].mId    = -1;
				self->mParts[j].mState = 0;
				self->mParts[j].mObj   = NULL;
			}
		}
		self->_9E0 = 0;
		break;
	case 8:
		lbl_80367A2C[self->_9E4] = fn_80106AC0();
		self->_9E0               = 0;
		break;
	}
}

// toys 0x26C and 0x485
extern "C" void fn_8011A050(XObjActorToy* self, u32 frames)
{
	CJObj joint;
	switch (self->_990) {
	case 0:
		if (self->_9A4 > 0) {
			self->_990++;
		}
		break;
	case 1: {
		s32 path[4] = { 0, 2, 4, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 4, path));
		joint.SetTranslateY(-self->_9A8, 0);
		if (1.0f == self->_9A8) {
			self->_990 = 0;
		}
		break;
	}
	}
}

// toy 0x25E, keeps the 0x341 objects it holds in its hand
// NONMATCHING: register allocation
extern "C" void fn_80119774(XObjActorToy* self, u32 frames)
{
	CJObj joint;
	if (self->_C84.mFlags & 0x40) {
		static const s32 path[1] = { 0 };
		CVec pos;
		joint.Attach(GetJointByPath(self->mJObj, (s32*)path, 1));
		pos    = CVec(GetJObjPos(joint.mJObj));
		s32 id = -1;
		if (self->_C84.mFlags & 0x40) {
			id = 0x341;
		}
		if (id >= 0) {
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = GAME_POOL_984C->Get(i);
				if (id == obj->GetDescKind()) {
					MtxPtr mtx = HSD_JObjGetMtxPtr(joint.mJObj);
					HSD_JObjCopyMtx(obj->mJObj, mtx);
					if (self->mFlags & 0x10000) {
						obj->mFlags |= 0x10000ULL;
					} else {
						obj->mFlags &= ~0x10000ULL;
					}
				}
			}
		}
	}
}

static inline f32 ToyTranslationX(const CJObj& obj)
{
	return HSD_JObjGetTranslationX(obj.mJObj);
}

static inline f32 ToyTranslationY(const CJObj& obj)
{
	return HSD_JObjGetTranslationY(obj.mJObj);
}

static inline f32 ToyTranslationZ(const CJObj& obj)
{
	return HSD_JObjGetTranslationZ(obj.mJObj);
}

static inline f32 ToyScaleX(const CJObj& obj)
{
	return HSD_JObjGetScaleX(obj.mJObj);
}

static inline f32 ToyScaleY(const CJObj& obj)
{
	return HSD_JObjGetScaleY(obj.mJObj);
}

static inline f32 ToyScaleZ(const CJObj& obj)
{
	return HSD_JObjGetScaleZ(obj.mJObj);
}

static inline f32 ToyRotationY(const CJObj& obj)
{
	return HSD_JObjGetRotationY(obj.mJObj);
}

void XObjActorToy::vfAC()
{
	fn_80107620(this);
	if (mPart0.mFlags & 0x10000) {
		mPart0.CJObj::SetTranslate(ToyTranslationX(*this), ToyTranslationY(*this), ToyTranslationZ(*this), 1);
		mPart0.CJObj::SetScale(ToyScaleX(*this), ToyScaleY(*this), ToyScaleZ(*this), 1);
		mPart0.CJObj::SetRotationY(ToyRotationY(*this), 1);
		if (mJObj->flags & 8) {
			HSD_JObjSetFlags(mPart0.mJObj, 8);
		} else {
			HSD_JObjClearFlags(mPart0.mJObj, 8);
		}
		mPart0.vfAC();
	}
	if (mPart1.mFlags & 0x10000) {
		mPart1.CJObj::SetTranslate(ToyTranslationX(*this), ToyTranslationY(*this), ToyTranslationZ(*this), 1);
		mPart1.CJObj::SetScale(ToyScaleX(*this), ToyScaleY(*this), ToyScaleZ(*this), 1);
		mPart1.CJObj::SetRotationY(ToyRotationY(*this), 1);
		if (mJObj->flags & 8) {
			HSD_JObjSetFlags(mPart1.mJObj, 8);
		} else {
			HSD_JObjClearFlags(mPart1.mJObj, 8);
		}
		mPart1.vfAC();
	}
}

// toy 0x26D, spins a joint once round
extern "C" void fn_80119C24(XObjActorToy* self, u32 frames)
{
	CJObj joint;
	switch (self->_990) {
	case 0:
		break;
	case 1:
		self->_990++;
		self->_988 = 0.0f;
		break;
	case 2: {
		self->_988 += 0.2f;
		s32 path[2] = { 0, 3 };
		joint.Attach(GetJointFromRoot(self->mJObj, 2, path));
		if (self->_988 >= 6.2831855f) {
			self->_990 = 0;
			self->_988 = 0.0f;
		}
		joint.SetRotationY(self->_988 + HSD_JObjGetRotationY(self->mJObj), 0);
		break;
	}
	}
}

// toy 0x1FC, pushes two of its joints out and back
// NONMATCHING: the path templates sit at other .rodata offsets and retail inlines the CJObj
// constructor
extern "C" void fn_8011A390(XObjActorToy* self, u32 frames)
{
	CJObj joint;
	static f32 sTime = 0.0f;
	static Vec sPos0;
	static Vec sPos1;
	switch (self->_990) {
	case 1: {
		s32 path0[4] = { 0, 2, 0, 1 };
		joint.Attach(GetJointFromRoot(self->mJObj, 4, path0));
		sPos0.x      = HSD_JObjGetTranslationX(joint.mJObj);
		sPos0.y      = HSD_JObjGetTranslationY(joint.mJObj);
		sPos0.z      = HSD_JObjGetTranslationZ(joint.mJObj);
		s32 path1[5] = { 0, 2, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 5, path1));
		sPos1.x = HSD_JObjGetTranslationX(joint.mJObj);
		sPos1.y = HSD_JObjGetTranslationY(joint.mJObj);
		sPos1.z = HSD_JObjGetTranslationZ(joint.mJObj);
		self->_990++;
		break;
	}
	case 2: {
		sTime += 0.16f;
		if (sTime >= 1.0f) {
			self->_990++;
		}
		s32 path0[4] = { 0, 2, 0, 1 };
		joint.Attach(GetJointFromRoot(self->mJObj, 4, path0));
		joint.SetTranslateZ(sPos0.z + 16.0f * sTime, 0);
		s32 path1[5] = { 0, 2, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 5, path1));
		joint.SetTranslateZ(sPos1.z + 16.0f * sTime, 0);
		break;
	}
	case 3: {
		s32 path0[4] = { 0, 2, 0, 1 };
		joint.Attach(GetJointFromRoot(self->mJObj, 4, path0));
		joint.SetTranslateZ(sPos0.z + 16.0f * sTime, 0);
		s32 path1[5] = { 0, 2, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 5, path1));
		joint.SetTranslateZ(sPos1.z + 16.0f * sTime, 0);
		sTime -= 0.16f;
		if (sTime <= 0.0f) {
			self->_990   = 0;
			s32 path2[4] = { 0, 2, 0, 1 };
			joint.Attach(GetJointFromRoot(self->mJObj, 4, path2));
			joint.SetTranslateX(sPos0.x, 0);
			joint.SetTranslateY(sPos0.y, 0);
			joint.SetTranslateZ(sPos0.z, 0);
			s32 path3[5] = { 0, 2, 0, 0, 0 };
			joint.Attach(GetJointFromRoot(self->mJObj, 5, path3));
			joint.SetTranslateX(sPos1.x, 0);
			joint.SetTranslateY(sPos1.y, 0);
			joint.SetTranslateZ(sPos1.z, 0);
			sTime = 0.0f;
		}
		break;
	}
	}
	if (self->mToyStatus.mFlags & 1) {
		s32 path0[4] = { 0, 0, 0, 1 };
		joint.Attach(GetJointFromRoot(self->mJObj, 4, path0));
		HSD_JObjSetFlags(joint.mJObj, 0x10);
		s32 path1[5] = { 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 5, path1));
		HSD_JObjSetFlags(joint.mJObj, 0x10);
		s32 path2[6] = { 0, 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 6, path2));
		HSD_JObjSetFlags(joint.mJObj, 0x10);
		s32 path3[7] = { 0, 0, 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 7, path3));
		HSD_JObjSetFlags(joint.mJObj, 0x10);
	} else {
		s32 path0[4] = { 0, 0, 0, 1 };
		joint.Attach(GetJointFromRoot(self->mJObj, 4, path0));
		HSD_JObjClearFlags(joint.mJObj, 0x10);
		s32 path1[5] = { 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 5, path1));
		HSD_JObjClearFlags(joint.mJObj, 0x10);
		s32 path2[6] = { 0, 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 6, path2));
		HSD_JObjClearFlags(joint.mJObj, 0x10);
		s32 path3[7] = { 0, 0, 0, 0, 0, 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 7, path3));
		HSD_JObjClearFlags(joint.mJObj, 0x10);
	}
}
