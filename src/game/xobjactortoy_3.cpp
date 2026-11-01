#include "game/hsd_jobj.h"
#include "game/cparticle.h"
#include "game/xobjactortoy.h"

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

// one of the toys, see GAME_POOL_975C
static inline XObjActorToy* GetToy(s32 i)
{
	return (XObjActorToy*)GAME_POOL_975C->Get(i);
}

static inline MtxPtr ToyMatrix(const CJObj& obj)
{
	return HSD_JObjGetMtxPtr(obj.mJObj);
}

extern "C" const u8 lbl_8022C7E0[];

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
extern "C" void fn_8011A050(XObjActorToy* self, u32 frames);
extern "C" void fn_80119774(XObjActorToy* self, u32 frames);

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
extern "C" void fn_80119C24(XObjActorToy* self, u32 frames);
extern "C" void fn_8011A390(XObjActorToy* self, u32 frames);
