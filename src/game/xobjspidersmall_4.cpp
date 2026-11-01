#include "game/xobjspidersmall_shared.h"

extern "C" {
void fn_800F4F48(SpiderShot* shot, u8 i, u32 frames, BOOL arg);

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
void fn_800FD5E0(XObjSpiderSmall* self, u32 frames);
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

// NONMATCHING: register allocation
void XObjSpiderSmall::Update(u32 frames)
{
	CPad pad(0);
	bool music = true;
	if (IsStage("stage00") || IsStage("stage19")) {
		music = false;
	}
	if (_258 == 0 && music && !IsStage("stage07")) {
		switch (_AC4) {
		case 0:
			if (IsPlayerNear() && GAME_DEMO == NULL && GetBgm() != -0xFFF4) {
				_AAC = GetBgm();
				_AB0 = *(u8*)(lbl_8025C400 + 0xE80F0);
				fn_8001E904(GAME_SOUND, -0xFFF4, 0xFF);
				_AC4++;
			}
			break;
		case 1:
			if (fn_800665E0(GAME_EVTMGR)) {
				if (fn_80034E4C(0x9E) > 0) {
					if (fn_8001E6C8(GAME_SOUND, -0xFFF4)) {
						fn_8001E660(GAME_SOUND, -0xFFF4);
					}
					_AC4 = 2;
				} else {
					if (fn_8001E6C8(GAME_SOUND, -0xFFF4)) {
						fn_8001E660(GAME_SOUND, -0xFFF4);
					}
					_AC4 = 0;
				}
			} else if (!IsPlayerNear() || GAME_DEMO != NULL) {
				if (_AAC != 0) {
					fn_8001E904(GAME_SOUND, _AAC, _AB0);
					_AC4 = 0;
				} else if (GetBgm() == -0xFFF4) {
					fn_8001E660(GAME_SOUND, -0xFFF4);
					_AC4 = 0;
				}
			}
			break;
		case 2:
			if (!IsPlayerNear() || GAME_DEMO != NULL) {
				if (_AAC != 0) {
					fn_8001E904(GAME_SOUND, _AAC, _AB0);
					_AC4 = 0;
				} else if (GetBgm() == -0xFFF4) {
					fn_8001E660(GAME_SOUND, -0xFFF4);
					_AC4 = 0;
				}
			}
			break;
		}
	}
	if (GAME_FLAGS & 0x400) {
		return;
	}
	if (_258 == 0) {
		fn_800EA780(lbl_8065B2C8, frames);
	}
	if (!(mFlags & 0x10000) && mState != 1) {
		return;
	}
	vf148();
	fn_800FD5E0(this, frames);
	if (!(mStatus.mFlags & 0x2000000)) {
		(this->*lbl_8022B230[mState])(frames);
		CVec dir;
		if (mStatus.mFlags & 0x100) {
			dir.x = 0.0f;
			dir.z = 0.0f;
			dir.y = _984;
			SetRotateY(_980, _98C);
		} else {
			if (mStatus.mFlags & 0x10) {
				mMoveTranslate.y += _99C;
			} else {
				mMoveTranslate.y = _9D8.y;
			}
			dir.x = _984 * (f32)sin(_980);
			dir.z = _984 * (f32)cos(_980);
			dir.y = 0.0f;
			SetRotateY(_980, _98C);
		}
		SetObjVec(mMoveTranslate, mMoveTranslate + dir * frames);
		SetMoveTranslate(&mMoveTranslate, 0);
	}
	fn_80090ED4(this, frames);
	fn_800F4F48(&mShot, _A88, frames, FALSE);
}

void XObjSpiderSmall::Release()
{
	if (fn_8001E6C8(GAME_SOUND, -0xFFF4)) {
		fn_8001E660(GAME_SOUND, -0xFFF4);
	}
	fn_800911C0(this);
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

void XObjSpiderSmall::Reset()
{
	fn_800928E8(this);
	mSfx.mTable = lbl_8022B170;
	_AAC        = GAME_WORD(0xE80E8) <= 0 ? GAME_WORD(0xE80D8) : 0;
	if (_258 == 0) {
		FindWebs();
	}
}
