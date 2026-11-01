#include "game/particle_srt.h"
#include "game/xobjactorfamily.h"
#include <stdlib.h>

static inline MtxPtr ActorMatrix(const CJObj& obj)
{
	return HSD_JObjGetMtxPtr(obj.mJObj);
}
// XObjActorPapa, Mr. Sanderson

void XObjActorPapa::vf150()
{
	mState = NULL;
}

void XObjActorPapa::vf14C(u32 frames)
{
	if (mState) {
		(this->*mState)(frames);
	}
}

XObjActorPapa::~XObjActorPapa()
{
}

HSD_JObj* XObjActorPapa::vf168()
{
	s32 path[3] = { 0, 2, 0 };
	return FindJoint(mJObj, 3, path);
}

CVec XObjActorPapa::vf16C()
{
	s32 path[3] = { 0, 2, 0 };
	return fn_8007E7C8(this, 3, path);
}

bool XObjActorPapa::IsFree()
{
	if ((mFlags & 1) == 0) {
		return true;
	}
	return !(mStatus.mFlags & 0x2000);
}

f32 XObjActorPapa::vf17C()
{
	return -0.17453292f;
}

f32 XObjActorPapa::vf178()
{
	return 0.17453292f;
}

s32 XObjActorPapa::vf124(s32 argc, s32* argv)
{
	fn_801070A4(this, argc, argv);
	switch (argv[0]) {
	case 1:
		switch (argv[1]) {
		case 0:
			fn_8010771C(this, 0.08726646f);
			break;
		case 1:
			fn_801076B0(this, &GAME_PLAYER->mMoveTranslate, 0.08726646f);
			break;
		case 2:
			if (FindObj(argv[2]) != NULL) {
				fn_801076FC(this, &FindObj(argv[2])->mMoveTranslate, 0.017453292f * argv[3]);
			}
			break;
		case 3:
			fn_801076DC(this, 0.017453292f * argv[2], 0.017453292f * argv[3]);
			break;
		}
		break;
	default:
		if (argc < 1) {
			return 0;
		}
		_9E0 = argv[0];
		if (argc < 2) {
			_9E4 = 0;
		} else {
			_9E4 = argv[1];
		}
		_9E8[0] = argv[2];
		_9E8[1] = argv[3];
		_9E8[2] = argv[4];
		_9E8[3] = argv[5];
		_9E8[4] = argv[6];
		_9E8[5] = argv[7];
		_9E8[6] = argv[8];
		_9E8[7] = argv[9];
		break;
	}
	return 0;
}

// picks up the object _A4C names and holds it in his hand
// NONMATCHING: one register
void XObjActorPapa::StatePickUp(u32 frames)
{
	switch (_990) {
	case 0: {
		XObjActorTao* tao = GAME_TAO;
		if ((tao->mFlags & 1) && !tao->IsFree()) {
			return;
		}
		bool busy = false;
		for (s32 i = 0; i < 15; i++) {
			XObj* obj = GAME_POOL_975C->Get(i);
			if (obj->GetDescKind() == 0x159) {
				if (!(obj->mFlags & 1)) {
					break;
				}
				if (!fn_8011566C(obj)) {
					busy = true;
					break;
				}
			}
		}
		if (busy) {
			return;
		}
		if (_A50 != NULL) {
			_A50->Release();
		}
		if (_A4C >= 0) {
			_A50 = ReadObj(_A4C);
		} else {
			_A50 = NULL;
		}
		mStatus.mFlags |= 0x2000;
		_990++;
	}
	case 1:
		if (_A50 != NULL) {
			if (_A50->mFlags & 0x0040000000000000ULL) {
				return;
			}
			GAME_FLAGS |= 0x1000;
		}
		_990++;
	case 2:
		if (_A4C >= 0 && _A50 != NULL) {
			((UnkCarried*)_A50)->SetScale(1.0f, 1.0f, 1.0f, 0);
			CJObj jobj;
			s32 path[6] = { 0, 2, 2, 0, 0, 0 };
			jobj.Attach(FindJoint(mJObj, 6, path));
			Vec pos;
			pos = GetJObjPos(jobj.mJObj);
			Vec scale;
			scale.x = pos.x;
			scale.y = pos.y;
			scale.z = pos.z;
			((UnkCarried*)_A50)->SetMoveTranslate(pos.x, pos.y + 2.0f, pos.z, 0);
			PSMTXCopy(ActorMatrix(jobj), (MtxPtr)_A54);
			HSD_MtxGetScale((MtxPtr)_A54, &scale);
			scale.x = fn_800621E0(_A50).x / scale.x;
			scale.y = fn_800621E0(_A50).y / scale.y;
			scale.z = fn_800621E0(_A50).z / scale.z;
			Mtx scaleMtx;
			Mtx rotMtx;
			Mtx transMtx;
			Mtx m;
			PSMTXScale(scaleMtx, scale.x, scale.y, scale.z);
			PSMTXConcat((MtxPtr)_A54, scaleMtx, m);
			PSMTXRotRad(rotMtx, 'z', 1.5707964f);
			PSMTXConcat(m, rotMtx, m);
			PSMTXTrans(transMtx, -5.5f, 3.5f, 0.0f);
			PSMTXConcat(m, transMtx, m);
			HSD_JObj* joint = _A50->mJObj;
			MtxPtr mp       = m;
			HSD_JObjCopyMtx(joint, mp);

			_A50->mFlags &= ~0x0200000000000000ULL;
		}
		mStatus.mFlags &= ~0x2000;
		break;
	}
}

void XObjActorPapa::StateNone(u32 frames)
{
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

void XObjActorPapa::vf15C()
{
	UnkPlayer* player = GAME_PLAYER;
	if (player->_980 & 0x40000000) {
		player->_980 = 0;
		player->_980 |= 0x20000000;
		player->_980 |= 7;
		player->_12DF4 = 1;
	}
	mStatus.mFlags = 0;
	_C8C.mFlags    = 0;
	_980           = 0.0f;
	_984           = 0.0f;
	_988           = 0.0f;
	_C84           = _988;
	_C88           = 0;
	_C94           = 1;
	mState         = &XObjActorPapa::StateNone;
	fn_8010706C(this);
}
