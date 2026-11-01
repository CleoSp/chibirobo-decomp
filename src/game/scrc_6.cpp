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

void XObjActor3::Kind0(u32 frames)
{
}

void XObjActor3::Kind1(u32 frames)
{
	switch (_B94[0]) {
	case 0:
		break;
	case 1:
		fn_800661E4(GAME_EVTMGR, GAME_ACTORS->Get(_258), 1, 1);
		_B94[0] = 0;
		break;
	case 2:
		fn_800661E4(GAME_EVTMGR, GAME_ACTORS->Get(_258), 0, 1);
		_B94[0] = 0;
		break;
	}
}

void XObjActor3::Kind4(u32 frames)
{
	_B88++;
	f32 s     = sin(0.017453292f * ((_B88 * 6) % 360));
	f32 swing = 0.25f;
	if (GetDescKind() == 0x437) {
		mMoveRotate.x = s * swing;
	} else {
		mMoveRotate.z = s * swing;
	}
}

void XObjActor3::Kind5(u32 frames)
{
}

// NONMATCHING: register allocation
void XObjActor3::Kind6(u32 frames)
{
	s16 id;
	s16 num;
	s32 i;
	s32 count = 0;
	switch (_B94[0]) {
	case 0:
		break;
	case 1:
		for (i = 0; i < 0x100; i++) {
			UnkPlayerItem* items = GAME_PLAYER->mItems;
			id                   = items[i].mId;
			num                  = items[i].mNum;
			if (fn_8006E44C(id, 4) && id >= 0 && num > 0) {
				count++;
			}
		}
		GAME_ITEMCOUNT = count;
		_B94[0]        = 0;
		break;
	}
}

// holds the 0x40B item at its first joint while flag 0x100 is up
// NONMATCHING: the loop and the joint's vectors take each other's registers
void XObjActor3::Kind8(u32 frames)
{
	Mtx m;
	Mtx t;
	Mtx a;
	CJObj joint;
	if (mStatus.mFlags & 0x100) {
		CVec pos;
		s32 path[1] = { 0 };
		joint.Attach(GetJointFromRoot(mJObj, 1, path));
		pos      = GetJObjPos(joint.mJObj);
		s32 kind = -1;
		if (mStatus.mFlags & 0x100) {
			kind = 0x40B;
		}
		if (kind >= 0) {
			for (s32 i = 0; i < 0x120; i++) {
				XObj* obj = GAME_ITEMS->Get(i);
				if (kind == obj->GetDescKind()) {
					PSMTXCopy(HSD_JObjGetMtxPtr(joint.mJObj), m);
					PSMTXTrans(t, 0.0f, -14.0f, 0.0f);
					PSMTXConcat(m, t, a);
					HSD_JObj* jobj = obj->mJObj;
					MtxPtr mp      = a;
					HSD_JObjCopyMtx(jobj, mp);
					if (mFlags & 0x10000) {
						obj->mFlags |= 0x10000ULL;
					} else {
						obj->mFlags &= ~0x10000ULL;
					}
				}
			}
		}
	}
}

void XObjActor3::Kind7(u32 frames)
{
	switch (_B94[0]) {
	case 0:
		break;
	case 1:
		if (_B94[1] != 0) {
			fn_80068BC0(GAME_EVTMGR);
		} else {
			fn_80068BAC(GAME_EVTMGR);
		}
		_B94[0] = 0;
		break;
	case 2:
		if (_B94[1] != 0) {
			fn_80068C10(GAME_EVTMGR);
			fn_80068BE8(GAME_EVTMGR);
			fn_80068BC0(GAME_EVTMGR);
		} else {
			fn_80068BFC(GAME_EVTMGR);
			fn_80068BD4(GAME_EVTMGR);
			fn_80068BAC(GAME_EVTMGR);
		}
		_B94[0] = 0;
		break;
	}
}

// falls from above the robot, knocks it down on a hit, blinks out on the floor
void XObjActor3::Kind10(u32 frames)
{
	UnkPlayer* player = GAME_PLAYER;
	Vec pos           = player->mMoveTranslate;
	switch (_B80) {
	case 0:
		if (mStatus.mFlags & 4) {
			break;
		}
		mMoveTranslate.x = pos.x - (rand() % 3600) / 100.0f + 18.0f;
		mMoveTranslate.y = 64.0f;
		mMoveTranslate.z = pos.z - (rand() % 3600) / 100.0f + 18.0f;
		mFlags |= 0x10000;
		mFlags |= 4;
		_B70 = 4.0f;
		_B88 = rand() % 30 + 30;
		_B80++;
		break;
	case 1:
		if (--_B88 < 0) {
			_B80++;
		}
		break;
	case 2:
		f32 dx   = pos.x - mMoveTranslate.x;
		f32 dy   = pos.y - mMoveTranslate.y;
		f32 dz   = pos.z - mMoveTranslate.z;
		f32 dist = sqrtf(dx * dx + dy * dy + dz * dz);
		if (dist < 8.0f && !fn_800A0000(player) && !fn_800665E0(GAME_EVTMGR)) {
			player->vf138(&pos, 10.0f);
			_B88 = 30;
			_B80 = 3;
			break;
		}
		if (_B70 >= -2.3f) {
			_B70 -= 0.36f * frames;
		} else {
			_B70 = -2.3f;
		}
		mMoveTranslate.y += _B70;
		if (mMoveTranslate.y < 0.0f) {
			mMoveTranslate.y = 0.0f;
			_B88             = 30;
			_B80             = 3;
		}
		break;
	case 3:
		if ((_B88 & 3) < 2) {
			mFlags |= 0x10000;
		} else {
			mFlags &= ~0x10000ULL;
		}
		if (--_B88 < 0) {
			mFlags &= ~0x10000ULL;
			mFlags &= ~4ULL;
			mStatus.mFlags |= 4;
			_B80 = 0;
		}
		break;
	}
}

// whether the robot stands where the actor can reach it
BOOL fn_8011132C(XObjActor3* self)
{
	UnkPlayer* player = GAME_PLAYER;
	Vec pos           = player->mMoveTranslate;
	if (!(self->mStatus.mFlags & 4)) {
		if (IsPlayerAway(player, pos)) {
			return FALSE;
		}
		if (pos.y >= self->mMoveTranslate.y - 16.0f && pos.y <= self->mMoveTranslate.y + 16.0f) {
			if (self->_B7C == 1.5707964f) {
				if (self->mMoveTranslate.x < pos.x && pos.x < self->_BC4) {
					return TRUE;
				}
				if (self->mMoveTranslate.x > pos.x && pos.x > self->_BC0) {
					if (IsLongReachAnim(player->mAnimId)) {
						if (self->mMoveTranslate.x > pos.x && pos.x > self->mMoveTranslate.x - 32.0f) {
							return TRUE;
						}
					} else {
						if (self->mMoveTranslate.x > pos.x && pos.x > self->mMoveTranslate.x - 12.0f) {
							return TRUE;
						}
					}
				}
			} else {
				if (self->mMoveTranslate.x > pos.x && pos.x > self->_BC0) {
					return TRUE;
				}
				if (self->mMoveTranslate.x < pos.x && pos.x < self->_BC4) {
					if (IsLongReachAnim(player->mAnimId)) {
						if (self->mMoveTranslate.x < pos.x && pos.x < self->mMoveTranslate.x + 32.0f) {
							return TRUE;
						}
					} else {
						if (self->mMoveTranslate.x < pos.x && pos.x < self->mMoveTranslate.x + 12.0f) {
							return TRUE;
						}
					}
				}
			}
		}
	}
	return FALSE;
}
void fn_8010FB1C(XObjActor3* self);
void fn_8010B528(XObjActor3* self);
void fn_8010B7B4(XObjActor3* self, u32 frames);
void fn_8010B394(XObjActor3* self);
void fn_8010AD88(XObjActor3* self);
void fn_8010AF74(XObjActor3* self);
void fn_8010904C(XObjActor3* self);
}
