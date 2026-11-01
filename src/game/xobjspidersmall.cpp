#include "game/particle_srt.h"
#include "game/xobjspidersmall_shared.h"

// NONMATCHING: anim and jobj swap r29 and r30
void XObjSpiderSmall::vf148()
{
	if ((mFlags & 1) && (mFlags & 0x10000) && !fn_800665E0(GAME_EVTMGR)) {
		s32 anim = mAnimId;
		mSfx.Play(anim, GetAnimFrame(mJObj), fn_8001DF00(GAME_SOUND, &mMoveTranslate, 0));
	}
}
extern "C" {

bool fn_800F4C10(SpiderShot* shot, u8 i, u32 frames)
{
	if (lbl_8037E910[i] == NULL) {
		return TRUE;
	}
	f32 end = GetAnimEndFrame(lbl_8037E910[i]->mJObj);
	if (end - frames <= GetAnimFrame(lbl_8037E910[i]->mJObj)) {
		return TRUE;
	}
	return FALSE;
}

void fn_800F4DB8(SpiderShot* shot, const CVec& pos)
{
	shot->mStatus.mFlags |= 1;
	shot->mPos = pos;
	shot->_28  = 1;
	if (CanPlaySe()) {
		fn_8001E904(GAME_SOUND, 0x184, 0xFF);
		fn_8001DF00(GAME_SOUND, &shot->mPos, 0);
	}
}

// plays the web's anims, 1 and 2 shoot it, 10 drops it
void fn_800F4F48(SpiderShot* shot, u8 i, u32 frames, BOOL arg)
{
	if (GAME_FLAGS & 0x400) {
		return;
	}
	if (i == 0xFF) {
		shot->_28 = 0;
		shot->mStatus.mFlags &= ~1;
		return;
	}
	if (lbl_8037E910[i] == NULL) {
		shot->_28 = 0;
		shot->mStatus.mFlags &= ~1;
		return;
	}
	if (!(shot->mStatus.mFlags & 1)) {
		return;
	}
	switch (shot->_28) {
	case 0:
		break;
	case 1:
		lbl_8037E910[i]->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		shot->_28++;
		break;
	case 2: {
		f32 end = GetAnimEndFrame(lbl_8037E910[i]->mJObj);
		if (end - frames <= GetAnimFrame(lbl_8037E910[i]->mJObj)) {
			lbl_8037E910[i]->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			shot->_28 = 0;
			shot->mStatus.mFlags &= ~1;
		}
		break;
	}
	case 3:
		lbl_8037E910[i]->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		shot->_28++;
		break;
	case 4: {
		f32 end = GetAnimEndFrame(lbl_8037E910[i]->mJObj);
		if (end - frames <= GetAnimFrame(lbl_8037E910[i]->mJObj)) {
			lbl_8037E910[i]->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			shot->_28 = 0;
			shot->mStatus.mFlags &= ~1;
		}
		break;
	}
	case 10:
		lbl_8037E910[i]->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		shot->_28 = 0;
		shot->mStatus.mFlags &= ~1;
		break;
	}
}

void fn_800F53F8(u8 on)
{
	for (s32 i = 0; i < 0x60; i++) {
		XObj* obj = GAME_SPIDERS->Get(i);
		if ((obj->mFlags & 1) && obj->GetDescKind() == 0x104) {
			if (on) {
				obj->mFlags |= 0x10000;
			} else {
				obj->mFlags &= ~0x10000ULL;
			}
		}
	}
}

s32 fn_800F5588()
{
	s32 n;
	switch (GAME_PLAYER->_D7E8) {
	case 1:
	case 6:
		n = rand() % 10 + 10;
		break;
	case 2:
	case 3:
		n = rand() % 2 + 2;
		break;
	case 4:
	case 8:
		n = rand() % 20 + 20;
		break;
	default:
		n = rand() % 10 + 10;
		break;
	}
	return n;
}

u8 fn_800F5678(XObjSpiderSmall* self)
{
	return (self->mStatus.mFlags >> 6) & 1;
}

u8 fn_800F5684(XObjSpiderSmall* self)
{
	return (self->mStatus.mFlags >> 23) & 1;
}

bool fn_800F5690(XObjSpiderSmall* self)
{
	f32 dx = self->_A1C.x - self->mMoveTranslate.x;
	f32 dy = self->_A1C.y - self->mMoveTranslate.y;
	f32 dz = self->_A1C.z - self->mMoveTranslate.z;
	return sqrtf(dx * dx + dy * dy + dz * dz) <= self->_A28 + self->_998;
}

// the player stands in the spider's area
bool fn_800F57C0(XObjSpiderSmall* self)
{
	Vec pos = GAME_PLAYER->mMoveTranslate;
	f32 y   = pos.y;
	f32 z   = pos.z;
	CVec unused;
	if (y < HSD_JObjGetTranslationY(self->mJObj) - 20.0f || y > 20.0f + HSD_JObjGetTranslationY(self->mJObj)) {
		return false;
	}
	f32 minX = self->_A08.x + self->_998;
	f32 maxX = self->_A08.x + self->_A14 - self->_998;
	f32 minZ = self->_A08.z + self->_998;
	f32 maxZ = self->_A08.z + self->_A18 - self->_998;
	if (minX > pos.x || maxX < pos.x || minZ > z || maxZ < z) {
		return false;
	}
	return true;
}

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

// state 8: chases the robot and spits webs at it
void fn_800F6408(XObjSpiderSmall* self, u32 frames)
{
	CPad pad(0);
	UnkSpiderPlayer* player = GAME_PLAYER;
	Vec ppos                = player->mMoveTranslate;
	Vec prot                = player->mMoveRotate;
	switch (self->mStep) {
	case 0:
		self->_98C = 0.0f;
		if (Distance(&ppos, &self->mMoveTranslate) < 48.0f) {
			self->_9FC.x = ppos.x;
			self->_9FC.y = ppos.y;
			self->_9FC.z = ppos.z;
			self->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mStep++;
		} else {
			self->mStep = 100;
		}
		break;
	case 1: {
		self->_984   = 0.3f;
		self->_9FC.x = ppos.x;
		self->_9FC.y = ppos.y;
		self->_9FC.z = ppos.z;
		self->_980   = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
		f32 dist     = Distance(&self->_9FC, &self->mMoveTranslate);
		if (dist < 20.0f) {
			self->_984 = 0.0f;
			self->SetAnim(3, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mTimer = fn_800F5588();
			self->mStep++;
		} else if (dist >= 64.0f) {
			self->mStep = 100;
		} else if (12.0f + self->_9D8.y < ppos.y || self->_9D8.y - 12.0f > ppos.y) {
			self->mStep = 100;
		}
		break;
	}
	case 2: {
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			if (--self->mTimer < 0) {
				self->_9FC.x = ppos.x;
				self->_9FC.y = ppos.y;
				self->_9FC.z = ppos.z;
				if (Distance(&self->_9FC, &self->mMoveTranslate) >= 20.0f) {
					self->SetAnim(12, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					self->mStatus.mFlags |= 8;
					self->_9FC.x = self->mMoveTranslate.x;
					self->_9FC.y = self->mMoveTranslate.y;
					self->_9FC.z = self->mMoveTranslate.z;
					self->_984   = 0.0f;
					self->ClampSpeed();
					self->_99C = 4.0f;
					self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
					self->mStatus.mFlags &= ~1;
					self->mStep = 22;
				} else {
					f32 size = fn_800A0414(GAME_PLAYER);
					self->SetAnim(4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					self->mStatus.mFlags |= 8;
					self->_A8C   = DEG2RAD * (rand() % 360);
					self->_A9C.x = size * (f32)sin(self->_A8C);
					self->_A9C.y = -rand() % 800 / 100;
					self->_A9C.z = size * (f32)cos(self->_A8C);
					self->_9FC.x = ppos.x + size * (f32)sin(self->_A8C);
					self->_9FC.y = ppos.y + -rand() % 800 / 100;
					self->_9FC.z = ppos.z + size * (f32)cos(self->_A8C);
					self->_984   = DistanceXZ(&self->_9FC, &self->mMoveTranslate) / 20.0f;
					self->ClampSpeed();
					self->_99C = 4.0f;
					self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
					self->mStatus.mFlags &= ~1;
					self->mFlags |= 0x4000000000000ULL;
					self->mStep = 10;
				}
			}
		}
		break;
	}
	case 4: {
		self->_AA8             = 0;
		f32 size               = fn_800A0414(GAME_PLAYER);
		self->mMoveTranslate.x = ppos.x + size * (f32)sin(prot.y + self->_A8C);
		self->mMoveTranslate.y = 10.0f + ppos.y + self->_A9C.y;
		self->mMoveTranslate.z = ppos.z + size * (f32)cos(prot.y + self->_A8C);
		self->_980             = atan2(ppos.x - self->mMoveTranslate.x, ppos.z - self->mMoveTranslate.z);
		if (IsPlayerFree() && (pad.mPad->mTrigger & 0xF0000)) {
			self->_A98 = self->_A98 - self->_A90;
			self->_A90 = 8.0f;
		}
		if (self->_A90 > 1.0f) {
			self->_A90 -= 1.0f;
		}
		if (GAME_DEMO != NULL) {
			self->_9FC.x = ppos.x + size * (f32)sin(prot.y + self->_A8C);
			self->_9FC.y = ppos.y;
			self->_9FC.z = ppos.z + size * (f32)cos(prot.y + self->_A8C);
			fn_800F54B4(self);
			self->SetAnim(6, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 0.0f;
			self->mFlags |= 0x4000000000000ULL;
			self->mStep = 30;
		} else if (Abs(ppos.y - self->_9D8.y) > 10.0f) {
			fn_800F54B4(self);
			self->mStatus.mFlags |= 0x2000;
			self->SetState(7);
		} else if (self->_A98 <= 0.0f || fn_800D4B60(GAME_PLAYER->_A44)) {
			f32 ang;
			f32 dist     = 20.0f + size + rand() % 3000 / 100.0f;
			ang          = PI + (f32)atan2(ppos.x - self->mMoveTranslate.x, ppos.z - self->mMoveTranslate.z);
			self->_9FC.x = ppos.x + dist * (f32)sin(ang);
			self->_9FC.y = ppos.y;
			self->_9FC.z = ppos.z + dist * (f32)cos(ang);
			fn_800F54B4(self);
			self->SetAnim(6, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 1.0f;
			self->mFlags |= 0x4000000000000ULL;
			if (size >= 2.0f) {
				self->_AA8 = 1;
			}
			self->mStep++;
		}
		break;
	}
	case 5: {
		s8 wait = 2;
		self->_99C -= 0.08f * frames;
		if (self->mMoveTranslate.y <= self->_9D8.y + frames) {
			self->_99C             = 0.0f;
			wait                   = 1;
			self->mMoveTranslate.y = self->_9D8.y;
		}
		self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
		if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 0.5f * frames) {
			self->_984 = 0.0f;
			wait--;
		}
		if (wait == 0) {
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mStep++;
		}
		break;
	}
	case 6: {
		self->mTimer++;
		if (self->mTimer % 30 < 15) {
			self->SetColor(1, 0, 0x80, 0, 0.0f);
			self->SetColor(1, 0x80, 0, 0, 0.0f);
		} else {
			self->SetColor(1, 0, 0, 0, 0.0f);
		}
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->SetColor(1, 0, 0, 0, 0.0f);
			self->mTimer = 0;
			if (--self->_9E8 <= 0) {
				self->_9E8 = 0;
			}
			self->mStatus.mFlags &= ~0x8000;
			if (self->_9E8 <= 0) {
				self->SetState(1);
			} else {
				self->mStep = 0;
				self->mStatus.mFlags &= ~8;
				self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				self->mTimer = rand() % 30 + 30;
			}
		}
		break;
	}
	case 10: {
		f32 dist = Distance(&ppos, &self->mMoveTranslate);
		if (dist < 12.0f + fn_800A0414(GAME_PLAYER) - 1.5f) {
			self->mFlags |= 0x4000000000000ULL;
		}
		if (dist < 8.0f + fn_800A0414(GAME_PLAYER) - 1.5f) {
			Catch(self);
		} else {
			if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 2.0f) {
				self->SetAnim(6, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				self->mStep++;
			} else if (self->_99C >= -2.3f) {
				self->_99C -= 0.36f * frames;
			} else {
				self->_99C = -2.3f;
			}
		}
		break;
	}
	case 11: {
		f32 dist = Distance(&ppos, &self->mMoveTranslate);
		if (dist < 12.0f + fn_800A0414(GAME_PLAYER) - 1.5f) {
			self->mFlags |= 0x4000000000000ULL;
		}
		if (dist < 8.0f + fn_800A0414(GAME_PLAYER) - 1.5f) {
			Catch(self);
		} else {
			if (self->_99C >= -2.3f) {
				self->_99C -= 0.36f * frames;
			} else {
				self->_99C = -2.3f;
			}
			if (self->mMoveTranslate.y <= self->_9D8.y + frames) {
				self->_99C             = 0.0f;
				self->mMoveTranslate.y = self->_9D8.y;
				self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				self->_984 = 0.0f;
				self->mStatus.mFlags &= ~8;
				self->mFlags &= ~0x4000000000000ULL;
				self->mStep++;
			}
		}
		break;
	}
	case 12: {
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->mStep = 0;
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mTimer = rand() % 30 + 30;
		}
		break;
	}
	case 22:
		if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 2.0f) {
			self->mStep++;
		} else if (self->_99C >= -2.3f) {
			self->_99C -= 0.36f * frames;
		} else {
			self->_99C = -2.3f;
		}
		break;
	case 23:
		if (self->_99C >= -2.3f) {
			self->_99C -= 0.36f * frames;
		} else {
			self->_99C = -2.3f;
		}
		if (self->mMoveTranslate.y <= self->_9D8.y + frames) {
			self->_99C             = 0.0f;
			self->mMoveTranslate.y = self->_9D8.y;
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 0.0f;
			self->mStatus.mFlags &= ~8;
			self->mFlags &= ~0x4000000000000ULL;
			self->mStep++;
		}
		break;
	case 24: {
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mTimer = rand() % 30 + 30;
			self->mStep  = 0;
		}
		break;
	}
	case 30:
		self->_99C -= 0.08f;
		if (self->mMoveTranslate.y <= self->_9D8.y) {
			self->_99C             = 0.0f;
			self->mMoveTranslate.y = self->_9D8.y;
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mStep = 31;
		}
		break;
	case 31: {
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->Wander(32);
		}
		break;
	}
	case 32:
		self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
		if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 2.0f) {
			self->mStep  = 33;
			self->_984   = 0.0f;
			self->mTimer = rand() % 30 + 30;
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		} else if (GAME_DEMO == NULL) {
			self->mStep = 0;
		}
		break;
	case 33:
		if (GAME_DEMO == NULL) {
			self->mStep = 0;
		}
		break;
	case 100:
		self->_98C = 0.0f;
		self->_984 = 0.0f;
		self->mStatus.mFlags |= 0x8000;
		self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 0.0f, 0.0f, 10.0f);
		self->_9A0 = 1.0f;
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		{
			f32 x = self->mMoveTranslate.x;
			f32 y = self->mMoveTranslate.y;
			f32 z = self->mMoveTranslate.z;
			fn_800F4DB8(&self->mShot, CVec(x, y, z));
		}
		self->mStep++;
		break;
	case 101:
		self->_9A0 -= 0.01f * frames;
		if (self->_9A0 <= 0.0f) {
			self->_9A0   = 0.0f;
			self->mTimer = 210 / frames;
			self->mStep++;
		}
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		break;
	case 102:
		if (fn_800F4C10(&self->mShot, self->_A88, frames)) {
			self->mTimer = rand() % 30 + 30;
			self->mStep++;
		}
		break;
	case 103:
		if (--self->mTimer < 0) {
			self->_AC0--;
			if (12.0f + self->_9D8.y < ppos.y || self->_9D8.y - 12.0f > ppos.y || GAME_DEMO != NULL || self->_AC0 < 0
			    || GAME_FLOAT(0xE5D30) == 0.0f) {
				self->mMoveTranslate.x = self->_9D8.x;
				self->mMoveTranslate.y = self->_9D8.y;
				self->mMoveTranslate.z = self->_9D8.z;
				self->_AC0             = 0;
				self->mTimer           = 0;
				if (self->mStatus.mFlags & 0x40000) {
					SetDead(self);
					if (self->_A88 != 0xFF) {
						lbl_8037E940[self->_A88] = -1;
						self->_A84               = NULL;
						self->_A88               = 0xFF;
					}
					self->mStep = 1000;
				} else {
					self->SetState(5);
					self->mStatus.mFlags &= ~0x40;
					lbl_8037E97C[GAME->mStage].mSpiders[self->_AB4].mFlags &= ~0x40;
				}
			} else {
				f32 size               = fn_800A0414(GAME_PLAYER);
				self->_A8C             = DEG2RAD * (rand() % 360);
				f32 dist               = 10.0f + size;
				self->mMoveTranslate.x = ppos.x + dist * (f32)sin(prot.y + self->_A8C);
				self->mMoveTranslate.y = self->_9D8.y;
				self->mMoveTranslate.z = ppos.z + dist * (f32)cos(prot.y + self->_A8C);
				f32 y, x, z;
				x          = self->mMoveTranslate.x;
				z          = self->mMoveTranslate.z;
				y          = self->mMoveTranslate.y;
				self->_980 = atan2(ppos.x - x, ppos.z - z);
				fn_800F4DB8(&self->mShot, CVec(x, y, z));
				self->mStep++;
			}
		}
		break;
	case 104:
		self->_9A0 += 0.01f * frames;
		if (self->_9A0 >= 1.0f) {
			self->_9A0   = 1.0f;
			self->mTimer = 210 / frames;
			self->mStep++;
		}
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		break;
	case 105:
		if (fn_800F4C10(&self->mShot, self->_A88, frames)) {
			self->mStatus.mFlags &= ~0x8000;
			self->mStep = 0;
		}
		break;
	}
}

// state 3: wanders about home, chases the robot and jumps onto it
void fn_800FA4B0(XObjSpiderSmall* self, u32 frames)
{
	CPad pad(0);
	Vec ppos = GAME_PLAYER->mMoveTranslate;
	Vec prot = GAME_PLAYER->mMoveRotate;
	if (!fn_800F57C0(self)) {
		if (--self->_ABC < 0) {
			self->_ABC = 0;
		}
	} else {
		self->_ABC = 180 / frames;
	}
	switch (self->mStep) {
	case 2:
	case 10:
	case 11:
	case 22:
	case 23:
		self->mPoints       = lbl_8065A798[1];
		self->mPointMaskOld = 0;
		self->mPointMask    = 0;
		break;
	default:
		self->mPoints       = lbl_8065A798[0];
		self->mPointMaskOld = 0;
		self->mPointMask    = 0;
		break;
	}
	switch (self->mStep) {
	case 0:
		self->_98C = 0.0f;
		self->mTimer--;
		if (self->mTimer <= 0) {
			self->Wander(20);
			self->mTimer = rand() % 30 + 10;
		} else if (Distance(&ppos, &self->mMoveTranslate) < 64.0f && fn_800F5690(self)) {
			self->_9FC.x = ppos.x;
			self->_9FC.y = ppos.y;
			self->_9FC.z = ppos.z;
			self->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mStep++;
		}
		break;
	case 20:
		self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
		if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 2.0f) {
			self->mStep  = 0;
			self->_984   = 0.0f;
			self->mTimer = rand() % 30 + 30;
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		} else {
			if (--self->mTimer < 0) {
				self->_984 = 0.0f;
				self->SetAnim(3, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				self->mStep = 21;
			}
			if (!self->InRect(self->mMoveTranslate)) {
				self->_9FC.x = self->_9D8.x;
				self->_9FC.y = self->_9D8.y;
				self->_9FC.z = self->_9D8.z;
			}
			if (self->_ABC <= 0) {
				if ((self->mStatus.mFlags & 0x20) && GAME_FLOAT(0xE5D30) != 0.0f) {
					self->_AC0 = rand() % 4 + 3;
					self->SetState(8);
				} else {
					self->SetState(7);
				}
			}
		}
		break;
	case 21: {
		HSD_AObj* aobj = GetAObj(self->mJObj);
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		aobj           = GetAObj(self->mJObj);
		if (end - frames <= (aobj != NULL ? HSD_AObjGetCurrFrame(aobj) : 0.0f)) {
			self->SetAnim(12, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mStatus.mFlags |= 8;
			self->_9FC.x = self->mMoveTranslate.x;
			self->_9FC.y = self->mMoveTranslate.y;
			self->_9FC.z = self->mMoveTranslate.z;
			self->_984   = 0.0f;
			self->ClampSpeed();
			self->mStep++;
			self->_99C = 4.0f;
			self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
			self->mStatus.mFlags &= ~1;
		}
		break;
	}
	case 22:
		if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 2.0f) {
			self->mStep++;
		} else if (self->_99C >= -2.3f) {
			self->_99C -= 0.36f * frames;
		} else {
			self->_99C = -2.3f;
		}
		break;
	case 23:
		if (self->_99C >= -2.3f) {
			self->_99C -= 0.36f * frames;
		} else {
			self->_99C = -2.3f;
		}
		if (self->mMoveTranslate.y <= self->_9D8.y + frames) {
			self->_99C             = 0.0f;
			self->mMoveTranslate.y = self->_9D8.y;
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 0.0f;
			self->mStatus.mFlags &= ~8;
			self->mFlags &= ~0x4000000000000ULL;
			self->mStep++;
		}
		break;
	case 24: {
		HSD_AObj* aobj = GetAObj(self->mJObj);
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		aobj           = GetAObj(self->mJObj);
		if (end - frames <= (aobj != NULL ? HSD_AObjGetCurrFrame(aobj) : 0.0f)) {
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mTimer = rand() % 30 + 30;
			self->mStep  = 0;
		}
		break;
	}
	case 30:
		self->_99C -= 0.08f;
		if (self->mMoveTranslate.y <= self->_9D8.y) {
			self->_99C             = 0.0f;
			self->mMoveTranslate.y = self->_9D8.y;
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mStep = 31;
		}
		break;
	case 31: {
		HSD_AObj* aobj = GetAObj(self->mJObj);
		f32 end        = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
		aobj           = GetAObj(self->mJObj);
		if (end - frames <= (aobj != NULL ? HSD_AObjGetCurrFrame(aobj) : 0.0f)) {
			self->Wander(32);
		}
		break;
	}
	case 32:
		self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
		if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 2.0f) {
			self->mStep  = 33;
			self->_984   = 0.0f;
			self->mTimer = rand() % 30 + 30;
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		} else if (GAME_DEMO == NULL) {
			self->mStep = 0;
		}
		break;
	case 33:
		if (GAME_DEMO == NULL) {
			self->mStep = 0;
		}
		break;
	case 1:
		self->_984   = 0.3f;
		self->_9FC.x = ppos.x;
		self->_9FC.y = ppos.y;
		self->_9FC.z = ppos.z;
		self->_980   = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
		if (Distance(&self->_9FC, &self->mMoveTranslate) < 20.0f) {
			self->_984 = 0.0f;
			self->SetAnim(3, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mTimer = fn_800F5588();
			self->mStep++;
		}
		if (!fn_800F5690(self)) {
			self->Wander(20);
		}
		if (!self->InRect(self->mMoveTranslate)) {
			self->Wander(20);
		}
		break;
	case 2: {
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			if (--self->mTimer < 0) {
				self->_9FC.x = ppos.x;
				self->_9FC.y = ppos.y;
				self->_9FC.z = ppos.z;
				if (Distance(&self->_9FC, &self->mMoveTranslate) >= 20.0f) {
					self->SetAnim(12, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					self->mStatus.mFlags |= 8;
					self->_9FC.x = self->mMoveTranslate.x;
					self->_9FC.y = self->mMoveTranslate.y;
					self->_9FC.z = self->mMoveTranslate.z;
					self->_984   = 0.0f;
					self->ClampSpeed();
					self->_99C = 4.0f;
					self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
					self->mStatus.mFlags &= ~1;
					self->mStep = 22;
				} else {
					f32 size = fn_800A0414(GAME_PLAYER);
					self->SetAnim(4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					self->mStatus.mFlags |= 8;
					self->_A8C   = DEG2RAD * (rand() % 360);
					self->_A9C.x = size * (f32)sin(self->_A8C);
					self->_A9C.y = -rand() % 800 / 100;
					self->_A9C.z = size * (f32)cos(self->_A8C);
					self->_9FC.x = ppos.x + size * (f32)sin(self->_A8C);
					self->_9FC.y = ppos.y + -rand() % 800 / 100;
					self->_9FC.z = ppos.z + size * (f32)cos(self->_A8C);
					self->_984   = DistanceXZ(&self->_9FC, &self->mMoveTranslate) / 20.0f;
					self->ClampSpeed();
					self->_99C = 4.0f;
					self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
					self->mStatus.mFlags &= ~1;
					self->mFlags |= 0x4000000000000ULL;
					self->mStep = 10;
				}
			}
		}
		break;
	}
	case 4: {
		self->_AA8             = 0;
		f32 size               = fn_800A0414(GAME_PLAYER);
		self->mMoveTranslate.x = ppos.x + size * (f32)sin(prot.y + self->_A8C);
		self->mMoveTranslate.y = 10.0f + ppos.y + self->_A9C.y;
		self->mMoveTranslate.z = ppos.z + size * (f32)cos(prot.y + self->_A8C);
		self->_980             = atan2(ppos.x - self->mMoveTranslate.x, ppos.z - self->mMoveTranslate.z);
		if (pad.mPad->mTrigger & 0xF0000) {
			self->_A98 = self->_A98 - self->_A90;
			self->_A90 = 8.0f;
		}
		if (self->_A90 > 1.0f) {
			self->_A90 -= 1.0f;
		}
		if (GAME_DEMO != NULL) {
			self->_9FC.x = ppos.x + size * (f32)sin(prot.y + self->_A8C);
			self->_9FC.y = ppos.y;
			self->_9FC.z = ppos.z + size * (f32)cos(prot.y + self->_A8C);
			fn_800F54B4(self);
			self->SetAnim(6, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 0.0f;
			self->mFlags |= 0x4000000000000ULL;
			self->mStep = 30;
		} else if (Abs(ppos.y - self->_9D8.y) > 10.0f) {
			fn_800F54B4(self);
			self->mStatus.mFlags |= 0x2000;
			self->SetState(7);
		} else if (self->_A98 <= 0.0f || fn_800D4B60(GAME_PLAYER->_A44)) {
			f32 dist     = 20.0f + size + rand() % 3000 / 100.0f;
			f32 ang      = PI + (f32)atan2(ppos.x - self->mMoveTranslate.x, ppos.z - self->mMoveTranslate.z);
			self->_9FC.x = ppos.x + dist * (f32)sin(ang);
			self->_9FC.y = ppos.y;
			self->_9FC.z = ppos.z + dist * (f32)cos(ang);
			fn_800F54B4(self);
			self->SetAnim(6, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 1.0f;
			self->mFlags |= 0x4000000000000ULL;
			if (size >= 2.0f) {
				self->_AA8 = 1;
			}
			self->mStep++;
		}
		break;
	}
	case 5: {
		s8 wait = 2;
		self->_99C -= 0.08f * frames;
		if (self->mMoveTranslate.y <= self->_9D8.y + frames) {
			self->_99C             = 0.0f;
			wait                   = 1;
			self->mMoveTranslate.y = self->_9D8.y;
		}
		if (!self->InRect(self->mMoveTranslate)) {
			self->_9FC.x = self->mMoveTranslate.x;
			self->_9FC.z = self->mMoveTranslate.z;
			self->_984   = 0.0f;
		}
		self->_980 = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
		if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 0.5f * frames) {
			self->_984 = 0.0f;
			wait--;
		}
		if (wait == 0) {
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mStep++;
		}
		break;
	}
	case 6: {
		self->mTimer++;
		if (self->mTimer % 30 < 15) {
			self->SetColor(1, 0, 0x80, 0, 0.0f);
			self->SetColor(1, 0x80, 0, 0, 0.0f);
		} else {
			self->SetColor(1, 0, 0, 0, 0.0f);
		}
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->SetColor(1, 0, 0, 0, 0.0f);
			self->mTimer = 0;
			if (--self->_9E8 <= 0) {
				self->_9E8 = 0;
			}
			self->mStatus.mFlags &= ~0x8000;
			if (self->_9E8 <= 0) {
				self->SetState(1);
			} else {
				self->mStep = 0;
				self->mStatus.mFlags &= ~8;
				self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				self->mTimer = rand() % 30 + 30;
			}
		}
		break;
	}
	case 10: {
		f32 dist = Distance(&ppos, &self->mMoveTranslate);
		if (dist < 12.0f + fn_800A0414(GAME_PLAYER) - 1.5f) {
			self->mFlags |= 0x4000000000000ULL;
		}
		if (dist < 8.0f + fn_800A0414(GAME_PLAYER) - 1.5f) {
			Catch(self);
		} else {
			if (!self->InRect(self->mMoveTranslate)) {
				self->_9FC.x = self->mMoveTranslate.x;
				self->_9FC.z = self->mMoveTranslate.z;
				self->_984   = 0.0f;
			}
			if (DistanceXZ(&self->_9FC, &self->mMoveTranslate) < 2.0f) {
				self->SetAnim(6, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				self->mStep++;
			} else if (self->_99C >= -2.3f) {
				self->_99C -= 0.36f * frames;
			} else {
				self->_99C = -2.3f;
			}
		}
		break;
	}
	case 11: {
		f32 dist = Distance(&ppos, &self->mMoveTranslate);
		if (dist < 12.0f + fn_800A0414(GAME_PLAYER) - 1.5f) {
			self->mFlags |= 0x4000000000000ULL;
		}
		if (dist < 8.0f + fn_800A0414(GAME_PLAYER) - 1.5f) {
			Catch(self);
		} else {
			if (self->_99C >= -2.3f) {
				self->_99C -= 0.36f * frames;
			} else {
				self->_99C = -2.3f;
			}
			if (!self->InRect(self->mMoveTranslate)) {
				self->_984 = 0.0f;
			}
			if (self->mMoveTranslate.y <= self->_9D8.y + frames) {
				self->_99C             = 0.0f;
				self->mMoveTranslate.y = self->_9D8.y;
				self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				self->_984 = 0.0f;
				self->mStatus.mFlags &= ~8;
				self->mFlags &= ~0x4000000000000ULL;
				self->mStep++;
			}
		}
		break;
	}
	case 12: {
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mTimer = rand() % 30 + 30;
			self->mStep  = 0;
		}
		break;
	}
	}
}

void fn_800F54B4(XObjSpiderSmall* self)
{
	self->mStatus.mFlags &= ~0x800000;
	if (self->mStatus.Check(0x1000000)) {
		self->mStatus.mFlags &= ~0x1000000;
		lbl_8065B2D4 = 0;
		GAME_PLAYER->SetColor(1, 0, 0, 0, 0.0f);
		fn_80066190(GAME_EVTMGR, 0, 1);
	}
	if (--lbl_8065B2D8 < 0) {
		lbl_8065B2D8 = 0;
	}
	if (lbl_8065B2D8 == 0) {
		GAME_PLAYER->_A40 = 1.0f;
	}
}

// state 9: falls off the wall, runs a bit and spits a last web
void fn_800F5934(XObjSpiderSmall* self, u32 frames)
{
	UnkSpiderPlayer* player = GAME_PLAYER;
	Vec ppos                = player->mMoveTranslate;
	Vec prot                = player->mMoveRotate;
	switch (self->mStep) {
	case 0:
		if ((self->mStatus.mFlags & 0x800000) || self->mMoveTranslate.y > self->_9D8.y) {
			f32 size     = fn_800A0414(player);
			self->_9FC.x = ppos.x + size * (f32)sin(prot.y + self->_A8C);
			self->_9FC.y = ppos.y;
			self->_9FC.z = ppos.z + size * (f32)cos(prot.y + self->_A8C);
			fn_800F54B4(self);
			self->SetAnim(6, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 0.0f;
			self->mFlags |= 0x4000000000000ULL;
			if (self->_99C >= -2.3f) {
				self->_99C -= 0.36f * frames;
			} else {
				self->_99C = -2.3f;
			}
			self->mStep = 1;
		} else {
			self->SetAnim(2, TRUE, NULL, NULL, NULL, 0.0f, 2.0f, 0.0f, 10.0f);
			self->mTimer = 60 / frames;
			self->mStep  = 10;
		}
		break;
	case 1:
		if (self->_99C >= -2.3f) {
			self->_99C -= 0.36f * frames;
		} else {
			self->_99C = -2.3f;
		}
		if (self->mMoveTranslate.y <= self->_9D8.y + frames) {
			self->_99C             = 0.0f;
			self->mMoveTranslate.y = self->_9D8.y;
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 0.0f;
			self->mStatus.mFlags &= ~8;
			self->mFlags &= ~0x4000000000000ULL;
			self->mStep++;
		}
		break;
	case 2: {
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->_9FC.x = 12.0f + self->_9D8.x - rand() % 2400 / 100.0f;
			self->_9FC.y = self->_9D8.y;
			self->_9FC.z = 12.0f + self->_9D8.z - rand() % 2400 / 100.0f;
			self->SetAnim(2, TRUE, NULL, NULL, NULL, 0.0f, 2.0f, 0.0f, 10.0f);
			self->mTimer = 60 / frames;
			self->mStep  = 10;
		}
		break;
	}
	case 10:
		self->_984   = 1.0f;
		self->_9FC.x = ppos.x;
		self->_9FC.y = ppos.y;
		self->_9FC.z = ppos.z;
		self->_980   = PI + (f32)atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
		Distance(&self->_9FC, &self->mMoveTranslate);
		if (--self->mTimer < 0) {
			self->_98C = 0.0f;
			self->_984 = 0.0f;
			self->mStatus.mFlags |= 0x8000;
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 0.0f, 0.0f, 10.0f);
			self->_9A0 = 1.0f;
			self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
			f32 x = self->mMoveTranslate.x;
			f32 y = self->mMoveTranslate.y;
			f32 z = self->mMoveTranslate.z;
			fn_800F4DB8(&self->mShot, CVec(x, y, z));
			self->mStep++;
		}
		break;
	case 11:
		self->_9A0 -= 0.01f * frames;
		if (self->_9A0 <= 0.0f) {
			self->_9A0   = 0.0f;
			self->mTimer = 210 / frames;
			self->mStep++;
		}
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		break;
	case 12:
		if (fn_800F4C10(&self->mShot, self->_A88, frames)) {
			self->mTimer = 0;
			SetDead(self);
			if (self->_A88 != 0xFF) {
				lbl_8037E940[self->_A88] = -1;
				self->_A84               = NULL;
				self->_A88               = 0xFF;
			}
			self->mStep++;
		}
		break;
	case 13:
		break;
	}
}

// state 4: jumps around the robot and spits webs at it
void fn_800F9AA0(XObjSpiderSmall* self, u32 frames)
{
	CPad pad(0);
	Vec ppos = GAME_PLAYER->mMoveTranslate;
	Vec prot = GAME_PLAYER->mMoveRotate;
	switch (self->mStep) {
	case 0:
		self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		if (Distance(&ppos, &self->mMoveTranslate) >= 30.0f) {
			self->_984   = 0.0f;
			self->mTimer = 1;
			self->mStep++;
		}
		break;
	case 1:
		if (--self->mTimer < 0) {
			self->_9A0 = 1.0f;
			self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
			f32 x = self->mMoveTranslate.x;
			f32 y = self->mMoveTranslate.y;
			f32 z = self->mMoveTranslate.z;
			fn_800F4DB8(&self->mShot, CVec(x, y, z));
			self->mStep++;
		}
		break;
	case 2:
		self->_9A0 -= 0.01f * frames;
		if (self->_9A0 <= 0.0f) {
			self->_9A0   = 0.0f;
			self->mTimer = 210 / frames;
			self->mStep++;
		}
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		break;
	case 3:
		if (fn_800F4C10(&self->mShot, self->_A88, frames)) {
			self->mTimer = rand() % 30 + 30;
			self->mStep++;
		}
		break;
	case 4:
		if (--self->mTimer < 0) {
			f32 x, z, y, dist;
			f32 size               = fn_800A0414(GAME_PLAYER);
			self->_A8C             = DEG2RAD * (rand() % 360);
			dist                   = 10.0f + size;
			self->mMoveTranslate.x = ppos.x + dist * (f32)sin(prot.y + self->_A8C);
			self->mMoveTranslate.y = ppos.y;
			self->mMoveTranslate.z = ppos.z + dist * (f32)cos(prot.y + self->_A8C);
			x                      = self->mMoveTranslate.x;
			z                      = self->mMoveTranslate.z;
			y                      = self->mMoveTranslate.y;
			self->_980             = atan2(ppos.x - x, ppos.z - z);
			fn_800F4DB8(&self->mShot, CVec(x, y, z));
			self->mStep++;
		}
		break;
	case 5:
		self->_9A0 += 0.01f * frames;
		if (self->_9A0 >= 1.0f) {
			self->_9A0   = 1.0f;
			self->mTimer = 210 / frames;
			self->mStep++;
		}
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		break;
	case 6:
		fn_800F4C10(&self->mShot, self->_A88, frames);
		break;
	}
}

// NONMATCHING: r9 and r10 swap around the web sound
// state 5: rises back up on its thread
void fn_800F9350(XObjSpiderSmall* self, u32 frames)
{
	CJObj obj;
	Vec ppos = GAME_PLAYER->mMoveTranslate;
	CPad pad(0);
	switch (self->mStep) {
	case 0:
		self->mStatus.mFlags |= 0x8000;
		self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 0.0f, 0.0f, 10.0f);
		self->_9A0 = 0.0f;
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		DistanceXZ(&ppos, &self->mMoveTranslate);
		if (fn_800F57C0(self) && GAME_DEMO == NULL) {
			f32 x = self->mMoveTranslate.x;
			f32 y = self->mMoveTranslate.y;
			f32 z = self->mMoveTranslate.z;
			fn_800F4DB8(&self->mShot, CVec(x, y, z));
			self->mStep++;
		}
		break;
	case 1:
		self->_9A0 += 0.01f * frames;
		if (self->_9A0 >= 1.0f) {
			self->_9A0 = 1.0f;
			self->mStep++;
			self->mTimer = 210 / frames;
		}
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		break;
	case 2:
		if (fn_800F4C10(&self->mShot, self->_A88, frames)) {
			self->mStatus.mFlags &= ~0x8000;
			self->SetState(3);
			self->mStatus.mFlags |= 0x40;
			lbl_8037E97C[GAME->mStage].mSpiders[self->_AB4].mFlags |= 0x40;
			self->_ABC = 180 / frames;
			switch (GAME->mStage) {
			case 3:
			case 7:
			case 9:
				break;
			default:
				if (rand() % 4 == 0) {
					self->mStatus.mFlags |= 0x20;
				}
				break;
			}
		}
		break;
	}
}

// state 7: spits a web and shrinks away
void fn_800F8634(XObjSpiderSmall* self, u32 frames)
{
	CJObj obj;
	CPad pad(0);
	switch (self->mStep) {
	case 0:
		self->_98C = 0.0f;
		self->_984 = 0.0f;
		self->mStatus.mFlags |= 0x8000;
		self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 0.0f, 0.0f, 10.0f);
		self->_9A0 = 1.0f;
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		{
			f32 x = self->mMoveTranslate.x;
			f32 y = self->mMoveTranslate.y;
			f32 z = self->mMoveTranslate.z;
			fn_800F4DB8(&self->mShot, CVec(x, y, z));
		}
		self->mStep++;
		break;
	case 1:
		self->_9A0 -= 0.01f * frames;
		if (self->_9A0 <= 0.0f) {
			self->_9A0 = 0.0f;
			self->mStep++;
			self->mTimer = 210 / frames;
		}
		self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
		break;
	case 2:
		if (self->mStatus.mFlags & 0x40000) {
			if (fn_800F4C10(&self->mShot, self->_A88, frames)) {
				self->mStep++;
				self->mTimer = 0;
				SetDead(self);
				if (self->_A88 != 0xFF) {
					lbl_8037E940[self->_A88] = -1;
					self->_A84               = NULL;
					self->_A88               = 0xFF;
				}
			}
		} else if (fn_800F4C10(&self->mShot, self->_A88, frames)) {
			if (self->mStatus.mFlags & 0x2000) {
				self->mMoveTranslate.x = self->_9D8.x;
				self->mMoveTranslate.y = self->_9D8.y;
				self->mMoveTranslate.z = self->_9D8.z;
				self->mStatus.mFlags &= ~0x2000;
			}
			self->SetState(5);
			self->mStatus.mFlags &= ~0x40;
			self->mStatus.mFlags &= ~0x20;
			lbl_8037E97C[GAME->mStage].mSpiders[self->_AB4].mFlags &= ~0x40;
		}
		break;
	case 3:
		break;
	}
}

// state 6: drops down on its thread and lands
void fn_800F8DB8(XObjSpiderSmall* self, u32 frames)
{
	CJObj obj;
	CPad pad(0);
	switch (self->mStep) {
	case 0:
		self->mStatus.mFlags |= 0x8000;
		self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 0.0f, 0.0f, 10.0f);
		self->_9A0 = 0.0f;
		self->_9A4 = 0.0f;
		if (self->mStatus.mFlags & 0x40000) {
			self->mTimer = 2 / frames;
			self->mStep++;
		}
		break;
	case 1:
		if (--self->mTimer < 0) {
			self->_984 = 0.0f;
			self->_9A4 = 1.0f;
			self->SetAnim(12, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mStatus.mFlags |= 8;
			self->_9FC.x = self->mMoveTranslate.x;
			self->_9FC.y = self->mMoveTranslate.y;
			self->_9FC.z = self->mMoveTranslate.z;
			self->_984   = 0.0f;
			self->_99C   = 4.0f;
			self->_980   = atan2(self->_9FC.x - self->mMoveTranslate.x, self->_9FC.z - self->mMoveTranslate.z);
			self->mStatus.mFlags &= ~1;
			self->mStep++;
		}
		break;
	case 2:
		if (self->_99C >= -2.3f) {
			self->_99C -= 0.36f * frames;
		} else {
			self->_99C = -2.3f;
		}
		if (self->mMoveTranslate.y <= self->_9D8.y + frames) {
			self->_99C             = 0.0f;
			self->mMoveTranslate.y = self->_9D8.y;
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->_984 = 0.0f;
			self->mStatus.mFlags &= ~8;
			self->mFlags &= ~0x4000000000000ULL;
			self->mStep++;
		}
		break;
	case 3: {
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mTimer = rand() % 30 + 30;
			self->mStatus.mFlags &= ~0x8000;
			self->SetState(3);
			self->mFlags &= ~0x4000000000000ULL;
			self->mStatus.mFlags |= 0x40;
			self->mStep = 0;
		}
		break;
	}
	}
	fn_80068C8C(&self->_9A0, &self->_9A4, 0.08f * frames);
	self->SetScale(self->_9A0, self->_9A0, self->_9A0, 0);
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
