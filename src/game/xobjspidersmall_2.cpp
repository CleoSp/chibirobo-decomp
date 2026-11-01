#include "game/xobjspidersmall_shared.h"

extern "C" {
bool fn_800F4C10(SpiderShot* shot, u8 i, u32 frames);
void fn_800F53F8(u8 on);

// the distances on the ground and in space. their out of line copies, fn_800F841C and
// fn_800F8520, set up the frame before the loads, which these don't, so those stay asm
static inline f32 DistanceXZ(const Vec* a, const Vec* b)
{
	f32 dx = a->x - b->x;
	f32 dy = 0.0f;
	f32 dz = a->z - b->z;
	return sqrtf(dx * dx + dy * dy + dz * dz);
}

static inline f32 Distance(const Vec* a, const Vec* b)
{
	f32 dx = a->x - b->x;
	f32 dy = a->y - b->y;
	f32 dz = a->z - b->z;
	return sqrtf(dx * dx + dy * dy + dz * dz);
}

static inline void SetDead(XObjSpiderSmall* self)
{
	self->mFlags &= ~0x10000ULL;
	self->mStatus.mFlags |= 0x80000;
	self->mStatus.mFlags &= ~0x100000;
}
void fn_800FC930(XObjSpiderSmall* self);

// jumps onto the robot
static inline void Catch(XObjSpiderSmall* self)
{
	self->mFlags |= 0x4000000000000ULL;
	self->_984             = 0.0f;
	self->_99C             = 0.0f;
	self->mMoveTranslate.y = 10.0f + self->_A9C.y;
	self->_A90             = 1.0f;
	self->_A98             = 40.0f;
	self->mStep            = 4;
	self->mStatus.mFlags |= 0x800000;
	lbl_8065B2D8++;
	GAME_PLAYER->_A40 = 0.4f;
	self->SetAnim(5, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}
void fn_800F54B4(XObjSpiderSmall* self);

void fn_800FCF00(XObjSpiderSmall* self)
{
}

// NONMATCHING: register order
// state 1, hit: blinks green, drops an item and waits out the web
void fn_800FC970(XObjSpiderSmall* self, u32 frames)
{
	self->mTimer++;
	if (self->mTimer % 30 < 15) {
		self->SetColor(1, 0, 0x80, 0, 0.0f);
	} else {
		self->SetColor(1, 0, 0, 0, 0.0f);
	}
	switch (self->mStep) {
	case 0:
		if (self->mStatus.Check(0x800000)) {
			fn_800F54B4(self);
		}
		fn_8001E904(GAME_SOUND, 0x127, 0xFF);
		if (!(GAME_FLAGS & 0x100)) {
			fn_800242A8(lbl_8025C400 + 0x965C, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.4f);
		}
		lbl_8037E97C[GAME->mStage].mSpiders[self->_AB4].mFlags |= 0x80000;
		{
			f32 x = self->mMoveTranslate.x;
			f32 y = self->mMoveTranslate.y;
			f32 z = self->mMoveTranslate.z;
			self->mShot.Set(CVec(x, y, z), 3);
		}
		self->mStep++;
		break;
	case 1: {
		self->mStep++;
		self->mTimer = 0;
		fn_800FC930(self);
		Vec pos;
		pos.x = self->mMoveTranslate.x;
		pos.y = self->mMoveTranslate.y;
		pos.z = self->mMoveTranslate.z;
		s32 kind;
		if (IsStage("stage07")) {
			switch (self->_258) {
			case 0:
			case 3:
			case 5:
			case 7:
			case 10:
				kind = 1;
				break;
			case 2:
			case 8:
				kind = 0;
				break;
			case 1:
			case 4:
			case 6:
			case 9:
			default:
				kind = 2;
				break;
			}
		} else if (rand() % 2 > 0) {
			kind = 2;
		} else if (rand() % 4 > 1) {
			kind = 1;
		} else {
			kind = 0;
		}
		if (self->_AA8) {
			kind = 2;
		}
		fn_800EA554(&pos, &self->_9D8, kind);
		break;
	}
	case 2:
		self->mFlags &= ~0x10000ULL;
		if (fn_800F4C10(&self->mShot, self->_A88, frames)) {
			if (self->_A88 != 0xFF) {
				lbl_8037E940[self->_A88] = -1;
				self->_A84               = NULL;
				self->_A88               = 0xFF;
			}
			self->mStep++;
		}
		break;
	case 3:
		break;
	}
}

// NONMATCHING: the web loop swaps r5 and r6
// runs the command an event gave it with vf124
void fn_800FCF04(XObjSpiderSmall* self, u32 frames)
{
	switch (self->_A30[0]) {
	case 0:
		break;
	case 1:
		if (!(self->mStatus.mFlags & 0x8000)) {
			self->SetState(1);
		}
		self->_A30[0] = 0;
		break;
	case 2:
		if (self->mStatus.mFlags & 0x10000) {
			GAME_WORD(0x5D08C) = 1;
		} else {
			GAME_WORD(0x5D08C) = 0;
		}
		self->_A30[0] = 0;
		break;
	case 3:
		if (self->mStatus.mFlags & 0x80000) {
			GAME_WORD(0x5D08C) = 1;
		} else {
			GAME_WORD(0x5D08C) = 0;
		}
		self->_A30[0] = 0;
		break;
	case 4:
		self->mStatus.mFlags |= 0x200000;
		self->_A30[0] = 0;
		break;
	case 5:
		self->mStatus.mFlags |= 0x400000;
		self->_A30[0] = 0;
		break;
	case 6:
		self->mFlags |= 0x4000000000000ULL;
		self->_A30[0] = 0;
		break;
	case 7:
		if (self->_A30[1] == 1) {
			self->mStatus.mFlags &= ~0x2000000;
		} else {
			self->mStatus.mFlags |= 0x2000000;
		}
		self->_A30[0] = 0;
		self->_A30[1] = 0;
		break;
	case 8:
		fn_800FFB00(self);
		fn_800FF168(self);
		self->_A30[0] = 0;
		break;
	case 9:
		self->_9D8.x           = self->_A30[1] / 100.0f;
		self->_9D8.y           = self->_A30[2] / 100.0f;
		self->_9D8.z           = self->_A30[3] / 100.0f;
		self->_A1C.x           = self->_9D8.x;
		self->_A1C.y           = self->_9D8.y;
		self->_A1C.z           = self->_9D8.z;
		self->mMoveTranslate.x = self->_9D8.x;
		self->mMoveTranslate.y = self->_9D8.y;
		self->mMoveTranslate.z = self->_9D8.z;
		self->mMoveTranslate.mTarget.Set(self->mMoveTranslate);
		self->_A30[0] = 0;
		break;
	case 10:
		self->_A08.x  = self->_A30[1] / 100.0f;
		self->_A08.y  = self->_A30[2] / 100.0f;
		self->_A08.z  = self->_A30[3] / 100.0f;
		self->_A14    = self->_A30[4];
		self->_A18    = self->_A30[5];
		self->_A30[0] = 0;
		break;
	case 11:
		self->_984 = 0.0f;
		self->SetState(5);
		self->mShot.mStatus.mFlags |= 1;
		self->mShot._28 = 10;
		self->_A30[0]   = 0;
		break;
	case 12:
		self->mStatus.mFlags |= 0x40000;
		self->mFlags |= 0x10000;
		self->mFlags |= 0x4000000000000ULL;
		self->SetState(6);
		self->mStatus.mFlags |= 0x8000;
		self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 0.0f, 0.0f, 10.0f);
		self->_9A0 = 0.0f;
		self->_9A4 = 0.0f;
		self->_984 = 0.0f;
		for (int i = 0; i < 12; i++) {
			if (lbl_8037E940[i] == -1) {
				self->_A84      = lbl_8037E910[i];
				self->_A88      = i;
				lbl_8037E940[i] = self->_258;
				break;
			}
		}
		self->_A30[0] = 0;
		break;
	case 21:
		if (!(self->mStatus.mFlags & 0x8000) && !(self->mStatus.mFlags & 0x80000) && (self->mFlags & 0x10000)) {
			self->SetState(9);
		}
		self->_A30[0] = 0;
		break;
	case 23:
		if (self->_A30[1] == 1) {
			fn_800F53F8(1);
		} else {
			fn_800F53F8(0);
		}
		self->_A30[0] = 0;
		self->_A30[1] = 0;
		break;
	}
}

// NONMATCHING: the battery clamp recomputes its address in other places
// caught in the web: drains the robot's battery and tints the screen red
void fn_800FD5E0(XObjSpiderSmall* self, u32 frames)
{
	CPad pad(0);
	if ((self->mStatus.mFlags & 2) && !(self->mStatus.mFlags & 0x10)) {
		fn_8001E660(GAME_SOUND, 0x12A);
		self->mStatus.mFlags &= ~2;
		self->SetState(1);
		if (self->_9E4 >= 10.0f) {
			self->_9E8 -= 5;
			if (self->_9E8 <= 0) {
				self->_9E8 = 0;
			}
		} else if (--self->_9E8 <= 0) {
			self->_9E8 = 0;
		}
	}
	{
		UnkSpiderPlayer* player = GAME_PLAYER;
		Vec ppos                = player->mMoveTranslate;
		CVec pos(ppos);
		f32 dist = Distance(&pos, &self->mMoveTranslate);
		if ((self->mStatus.mFlags & 0x800000) && GAME_DEMO == NULL && fn_800A0414(GAME_PLAYER) < 2.0f) {
			if (--self->_A94 <= 0) {
				GAME_PLAYER->UseBattery(3.0f);
				self->_A94 = 30;
				if (lbl_8065B2D4 == 0) {
					self->mStatus.mFlags |= 0x1000000;
					lbl_8065B2D4 = 1;
					fn_80066190(GAME_EVTMGR, 1, 1);
				}
				if (self->mStatus.mFlags & 0x1000000) {
					lbl_8065B2CC[0] = 60;
					lbl_8065B2CC[1] = 0;
				}
			}
		}
		if (self->mStatus.mFlags & 0x1000000) {
			fn_800A0014(player, FALSE, TRUE, 0.0f);
			if (lbl_8065B2CC[0] > 0) {
				if ((lbl_8065B2CC[0] -= frames) < 0) {
					lbl_8065B2CC[0] = 0;
				}
				lbl_8065B2CC[1]++;
				if (lbl_8065B2CC[0] > 0) {
					f32 red = 128.0f * (lbl_8065B2CC[0] / 60.0f);
					player->SetColor(1, red, 0, 0, 0.0f);
					fn_80023EE0(GAME_SCRNCOLOR, 0.0f, red, 0, 0, 0, red, 0, 0, 0);
				} else {
					player->SetColor(1, 0, 0, 0, 0.0f);
					fn_80023EE0(GAME_SCRNCOLOR, 0.0f, 0, 0, 0, 0, 100, 100, 200, 0);
				}
			}
		}
	}
	fn_800FCF04(self, frames);
}
}

// a vertex of the shadow's rim
struct UnkSpiderVtx {
	Vec mPos;   // _00
	u32 mColor; // _0C
};

// the bgm playing, none while one is queued
static inline s32 GetBgm()
{
	return GAME_WORD(0xE80E8) <= 0 ? GAME_WORD(0xE80D8) : 0;
}

// the player stands in a live spider's area
static inline bool IsPlayerNear()
{
	Vec pos = GAME_PLAYER->mMoveTranslate;
	for (int i = 0; i < 16; i++) {
		XObjSpiderSmall* spider = (XObjSpiderSmall*)GAME_POOL->Get(i);
		if ((spider->mFlags & 1) && !spider->IsDead() && (spider->mFlags & 0x10000) && !(spider->mStatus.mFlags & 0x2000000)) {
			if (pos.y < HSD_JObjGetTranslationY(spider->mJObj) - 20.0f || pos.y > 20.0f + HSD_JObjGetTranslationY(spider->mJObj)) {
				continue;
			}
			f32 minX = 1.5f + spider->_A08.x;
			f32 maxX = spider->_A08.x + spider->_A14 - 1.5f;
			f32 minZ = 1.5f + spider->_A08.z;
			f32 maxZ = spider->_A08.z + spider->_A18 - 1.5f;
			if (minX > pos.x || maxX < pos.x || minZ > pos.z || maxZ < pos.z) {
				continue;
			}
			return true;
		}
	}
	return false;
}

static inline void SetObjVec(CObjVec& v, const Vec& pos)
{
	v.Set(pos);
	v.mTarget.Set(pos);
}

// finds the stage's webs, the objects of kind 0x1BC
static inline void FindWebs()
{
	for (int i = 0; i < 12; i++) {
		lbl_8037E910[i] = NULL;
		lbl_8037E940[i] = -1;
	}
	lbl_8065B2E0 = 0;
	lbl_8065B2DC = NULL;
	s32 n        = 0;
	for (s32 i = 0; lbl_8065B2DC == NULL && i < GAME->mObjectCount; i++) {
		XObj* obj = GAME->mObjects[i];
		if ((obj->mFlags & 1) && *(s32*)obj->_238 == 0x1BC) {
			lbl_8037E910[n] = obj;
			lbl_8037E910[n]->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			n++;
			if (n >= 12) {
				break;
			}
		}
	}
}
extern "C" void fn_800FFB00(XObjSpiderSmall* self);
