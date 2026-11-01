#include "game/hsd_jobj.h"
#include "game/cparticle.h"
#include "game/xobjactorfamily.h"
#include <stdlib.h>

static inline MtxPtr ActorMatrix(const CJObj& obj)
{
	return HSD_JObjGetMtxPtr(obj.mJObj);
}

void XObjActorPapa::vf158(u32 frames)
{
	CJObj jobj;
	if (mStatus.Check(0x400)) {
		mStatus.mFlags &= ~0x400;
		_AB0 = 0.0f;
	}
	if (mStatus.Check(2)) {
		mStatus.mFlags &= ~2;
		fn_8001E904(GAME_SOUND, rand() % 10 + 0xB7, 0xFF);
	}
	switch (_9E0) {
	case 2:
		Drop();
		switch (_9E4) {
		case 0x1F4:
		case 0x360:
		case 0x3FA:
		case 0x401:
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = GAME_POOL_984C->Get(i);
				if (_9E4 == obj->GetDescKind()) {
					_A84 = obj;
					obj->mFlags |= 0x10000ULL;
					switch (_9E4) {
					case 0x1F4:
						_C8C.mFlags |= 1;
						break;
					case 0x360:
						_C8C.mFlags |= 2;
						break;
					case 0x3FA:
						_C8C.mFlags |= 0x10;
						break;
					case 0x401:
						_C8C.mFlags |= 0x20;
						break;
					}
				}
			}
			break;
		case 0x1D3:
			for (s32 i = 0; i < 15; i++) {
				XObj* obj = GAME_POOL_975C->Get(i);
				if (_9E4 == obj->GetDescKind()) {
					obj->mFlags |= 0x10000ULL;
					_A84 = obj;
					switch (_9E4) {
					case 0x1D3:
						_C8C.mFlags |= 4;
						break;
					}
				}
			}
			break;
		case 0x375:
			for (s32 i = 0; i < 64; i++) {
				XObj* obj = GAME_POOL_9734->Get(i);
				if (_9E4 == obj->GetDescKind()) {
					obj->mFlags |= 0x10000ULL;
					_A84 = obj;
					switch (_9E4) {
					case 0x375:
						_C8C.mFlags |= 8;
						break;
					}
				}
			}
			break;
		default:
			if (_A84 != NULL) {
				HSD_JObj* held = _A84->mJObj;
				HSD_JObjClearUserMtx(held);
				_A84 = NULL;
			}
			break;
		}
		_9E0 = 0;
		break;
	case 3:
		switch (_9E4) {
		case 1: {
			mStatus.mFlags |= 0x100;
			LockPlayer(GAME_PLAYER);
			GAME_PLAYER->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);

			break;
		}
		default: {
			mStatus.mFlags &= ~0x100;
			s32 path[6] = { 0, 2, 2, 0, 0, 0 };
			jobj.Attach(FindJoint(mJObj, 6, path));
			Vec pos;
			pos = GetJObjPos(jobj.mJObj);
			Vec target;
			target.x = pos.x;
			target.y = pos.y;
			target.z = pos.z;
			target.y = mMoveTranslate.y;
			GAME_PLAYER->SetMoveTranslate(&target, 0);
			UnkPlayer* player = GAME_PLAYER;
			player->_980      = 0;
			player->_980 |= 0x20000000;
			player->_980 |= 7;
			player->_12DF4 = 1;
			break;
		}
		}
		_9E0 = 0;
		break;
	case 4:
		fn_800242A8(GAME_QUAKE, _9E4, _9E8[0], _9E8[1], 0.0f, 0.0f, 0.0f, _9E8[2] / 100.0f);
		_9E0 = 0;
		break;
	case 5: {
		void* joint = ((UnkCarried*)this)->GetAnimJoint(_9E4);
		void* mat   = ((UnkCarried*)this)->GetMatAnimJoint(_9E8[0]);
		void* shape = ((UnkCarried*)this)->GetShapeAnimJoint(_9E4);
		SetAnim(0, TRUE, joint, mat, shape, 0.0f, 1.0f, 1.0f, 10.0f);
		mFlags |= 0x0008000000000000ULL;
		_9E0 = 0;
		break;
	}
	case 6:
		_A4C   = _9E4;
		mState = &XObjActorPapa::StatePickUp;
		fn_8010706C(this);
		_9E0 = 0;
		break;
	case 7:
		_A4C = -1;
		if (_A50 != NULL) {
			_A50->Release();
		}
		_9E0 = 0;
		break;
	case 8:
		switch (_9E4) {
		case 1:
			_C94 = 1;
			break;
		default:
			_C94 = 0;
			break;
		}
		_9E0 = 0;
		break;
	}
}
