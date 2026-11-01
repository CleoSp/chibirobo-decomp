#include "game/xobjspidersmall_shared.h"

extern "C" {

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

void XObjSpiderSmall::vf94()
{
	_A2C = -1;
	fn_800FFB00(this);
	SetState(0);
	mStatus.mFlags |= 0x4000000;
	_AAC = -1;
	_AB4 = -1;
	if (_258 == 0) {
		for (s32 i = 0; i < 0x27; i++) {
			lbl_8037E97C[i].Clear();
		}
	}
	_AB8 = GAME_WORD(0xE5D28);
}

void XObjSpiderSmall::Init(u32 id)
{
	fn_800929C4(this, id);
	_A2C = -1;
	fn_800FFB00(this);
	SetState(0);
	mStatus.mFlags |= 0x4000000;
	_AAC = -1;
	_AB4 = -1;
	if (_258 == 0) {
		for (s32 i = 0; i < 0x27; i++) {
			lbl_8037E97C[i].Clear();
		}
	}
	_AB8 = GAME_WORD(0xE5D28);
}

// NONMATCHING: its Vec constants sit 0x18 lower in .rodata
extern "C" void fn_800FFB00(XObjSpiderSmall* self)
{
	self->_980           = 0.0f;
	self->_984           = 0.0f;
	self->_988           = 0.0f;
	self->_98C           = 0.1f;
	self->_99C           = 0.0f;
	self->_9A0           = 0.0f;
	self->_9A4           = 0.0f;
	self->mState         = 0;
	self->mPrevState     = 0;
	self->mStep          = 0;
	self->mTimer         = 0;
	self->mStatus.mFlags = 0;
	self->_9C0           = 0;
	self->_9C4           = 1.0f;
	self->_9C8           = 1.0f;
	Vec v0               = { 0.0f, 0.0f, 0.0f };
	self->_9CC           = v0;
	Vec v1               = { 0.0f, 0.0f, 0.0f };
	self->_9D8           = v1;
	Vec v2               = { 0.0f, 0.0f, 0.0f };
	self->_9F0           = v2;
	Vec v3               = { 0.0f, 0.0f, 0.0f };
	self->_9FC           = v3;
	self->_9E4           = 0.0f;
	self->_9EC           = 3;
	self->_9E8           = self->_9EC;
	self->_A30[0]        = 0;
	self->_A30[1]        = 0;
	self->_A48           = 0;
	self->_994           = 1.0f;
	self->mFlags |= 8;
	self->mFlags |= 0x10000000;
	self->mShot.mPos.x         = 0.0f;
	self->mShot.mPos.y         = 0.0f;
	self->mShot.mPos.z         = 0.0f;
	self->mShot.mTarget.x      = 0.0f;
	self->mShot.mTarget.y      = 0.0f;
	self->mShot.mTarget.z      = 0.0f;
	self->mShot._20            = 0.0f;
	self->mShot._24            = 0.0f;
	self->mShot._28            = 0;
	self->mShot._2C            = 0;
	self->mShot.mStatus.mFlags = 0;
	self->_998                 = 16.0f;
	if (self->_258 == 0) {
		fn_800EA99C(lbl_8065B2C8);
		lbl_8065B2CC[0] = 0;
		lbl_8065B2CC[1] = 0;
		lbl_8065B2D4    = 0;
		lbl_8065B2D8    = 0;
	}
	self->_A84 = NULL;
	self->_A88 = 0xFF;
	self->_A8C = 0.0f;
	self->_A90 = 0.0f;
	self->_A94 = 0;
	self->_A98 = 0.0f;
	Vec v4     = { 0.0f, 0.0f, 0.0f };
	self->_A9C = v4;
	Vec v5     = { 0.0f, 0.0f, 0.0f };
	self->_A08 = v5;
	self->_A14 = 0.0f;
	self->_A18 = 0.0f;
	Vec v6     = { 0.0f, 0.0f, 0.0f };
	self->_A1C = v6;
	self->_A28 = 0.0f;
	self->SetColor(1, 0, 0, 0, 0.0f);
	self->_ABC = 0;
	self->_AC0 = 0;
	self->_AC4 = 0;
	self->_AA8 = 0;
}
