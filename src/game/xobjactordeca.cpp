#include "game/particle_srt.h"
#include "game/xobjactorfamily.h"

// XObjActorDeca

void XObjActorDeca::vf150()
{
	mState = NULL;
}

void XObjActorDeca::vf14C(u32 frames)
{
	if (mState) {
		(this->*mState)(frames);
	}
}

XObjActorDeca::~XObjActorDeca()
{
}

HSD_JObj* XObjActorDeca::vf168()
{
	s32 path[3] = { 0, 3, 0 };
	return FindJoint(mJObj, 3, path);
}

CVec XObjActorDeca::vf16C()
{
	s32 path[3] = { 0, 3, 0 };
	return fn_8007E7C8(this, 3, path);
}

Vec XObjActorDeca::GetCameraTarget()
{
	if (_C90.mFlags & 4) {
		CJObj jobj;
		s32 path[2] = { 0, 3 };
		jobj.Attach(FindJoint(mJObj, 2, path));
		Vec pos;
		pos = GetJObjPos(jobj.mJObj);
		Vec target;
		target = pos;
		target.y -= 0.5f;
		return target;
	}
	return fn_80107084(this);
}

s32 XObjActorDeca::vf124(s32 argc, s32* argv)
{
	UnkPlayer* player = GAME_PLAYER;
	CJObj jobj;
	CVec unused;
	fn_801070A4(this, argc, argv);
	switch (argv[0]) {
	case 1:
		switch (argv[1]) {
		case 0:
			fn_8010771C(this, 0.08726646f);
			break;
		case 1:
			fn_801076FC(this, &GAME_PLAYER->mMoveTranslate, 0.08726646f);
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
		if (_9E4 == 0xD0) {
			_A2C = (s32)FindObj(_9E8[0]);
		}
		switch (_9E0) {
		case 2:
			switch (_9E4) {
			case 0:
				_C90.mFlags |= 1;
				break;
			case 1:
				_C90.mFlags &= ~1;
				break;
			}
			_9E0 = 0;
			break;
		case 3:
			fn_8001E904(GAME_SOUND, 0x50014, 0xFF);
			switch (_9E4) {
			case 0:
				_C90.mFlags |= 2;
				_CA4 = 3.1415927f;
				break;
			case 1:
				_C90.mFlags |= 2;
				_CA4 = 0.0f;
				break;
			}
			_9E0 = 0;
			break;
		case 4:
			_A4C   = _9E4;
			mState = &XObjActorDeca::StatePickUp;
			fn_8010706C(this);
			_9E0 = 0;
			break;
		case 5:
			_A4C = -1;
			if (_A50 != NULL) {
				_A50->Release();
			}
			_9E0 = 0;
			break;
		case 6:
			if (_9E4 == 1) {
				fn_800660E8(GAME_EVTMGR, 1, 1);
			} else {
				fn_800660E8(GAME_EVTMGR, 0, 1);
			}
			_9E0 = 0;
			break;
		case 7:
			switch (_9E4) {
			case 1:
				mStatus.mFlags |= 0x100;
				player->_980 = 0;
				player->_980 |= 0x40000000;
				player->_12DF4 = 0;
				break;
			default: {
				mStatus.mFlags &= ~0x100;
				s32 path[5] = { 0, 6, 0, 0, 1 };
				jobj.Attach(FindJoint(mJObj, 5, path));
				Vec pos;
				pos = GetJObjPos(jobj.mJObj);
				CVec target(pos);
				target.y = 0.0f;
				player->SetMoveTranslate(&target, 0);
				player->_980 = 0;
				player->_980 |= 0x20000000;
				player->_980 |= 7;
				player->_12DF4 = 1;
				HSD_JObj* pj   = player->mJObj;
				HSD_JObjClearUserMtx(pj);
				break;
			}
			}
			_9E0 = 0;
			break;
		case 8:
			switch (_9E4) {
			case 1:
				_C90.mFlags |= 4;
				break;
			default:
				_C90.mFlags &= ~4;
				break;
			}
			_9E0 = 0;
			break;
		case 9:
			if (_A50 != NULL) {
				((UnkCarried*)_A50)->SetAnim(_9E4, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
			_9E0 = 0;
			break;
		}
		break;
	}
	return 0;
}

// picks up the object _A4C names, then holds it
// NONMATCHING: two registers
void XObjActorDeca::StatePickUp(u32 frames)
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
		XObjActorPapa* papa = GAME_PAPA;
		if ((papa->mFlags & 1) && !papa->IsFree()) {
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
			s32 path[2] = { 0, 5 };
			jobj.Attach(FindJoint(mJObj, 2, path));
			((UnkCarried*)_A50)->SetAnim(0, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			Mtx m;
			Mtx transMtx;
			Mtx hand;
			PSMTXCopy(HSD_JObjGetMtxPtr(jobj.mJObj), m);
			PSMTXTrans(transMtx, 0.0f, -3.0f, -4.0f);
			PSMTXConcat(m, transMtx, hand);
			HSD_JObj* joint = _A50->mJObj;
			MtxPtr mp       = hand;
			HSD_JObjCopyMtx(joint, mp);

			_A50->mFlags &= ~0x0200000000000000ULL;
			if (mFlags & 0x10000) {
				_A50->mFlags |= 0x10000ULL;
			} else {
				_A50->mFlags &= ~0x10000ULL;
			}
			_A50->mFlags &= ~0x0200000000000000ULL;
		}
		mStatus.mFlags &= ~0x2000;
		mState = &XObjActorDeca::StateHold;
		fn_8010706C(this);
		break;
	}
}

static inline MtxPtr DecaMatrix(const CJObj& obj)
{
	return HSD_JObjGetMtxPtr(obj.mJObj);
}

// keeps what it carries in its hand
void XObjActorDeca::StateHold(u32 frames)
{
	if (_A4C >= 0 && _A50 != NULL) {
		((UnkCarried*)_A50)->SetScale(1.0f, 1.0f, 1.0f, 0);
		CJObj jobj;
		s32 path[2] = { 0, 5 };
		jobj.Attach(FindJoint(mJObj, 2, path));
		Mtx m;
		Mtx transMtx;
		Mtx hand;
		PSMTXCopy(DecaMatrix(jobj), m);
		PSMTXTrans(transMtx, 0.0f, -3.0f, -4.0f);
		PSMTXConcat(m, transMtx, hand);
		HSD_JObj* joint = _A50->mJObj;
		MtxPtr mp       = hand;
		HSD_JObjCopyMtx(joint, mp);
		if (mFlags & 0x10000) {
			_A50->mFlags |= 0x10000ULL;
		} else {
			_A50->mFlags &= ~0x10000ULL;
		}
		_A50->mFlags &= ~0x0200000000000000ULL;
	}
}

void XObjActorDeca::UpdateAnim(u32 speed)
{
	CXObjIntp::UpdateAnim(speed);
}

void XObjActorDeca::Update(u32 frames)
{
	fn_80107DE0(this, frames);
}

void XObjActorDeca::vf158(u32 frames)
{
	if (mStatus.Check(2)) {
		mStatus.mFlags &= ~2;
	}
}

void XObjActorDeca::vf15C()
{
	mStatus.mFlags = 0;
	_C90.mFlags    = 0;
	_C98.mFlags    = 0;
	_980           = 0.0f;
	_984           = 0.0f;
	_988           = 0.0f;
	_C84           = _988;
	_C88           = 0.0f;
	_C8C           = 0;
	_CA0           = 3.1415927f;
	_CA4           = 3.1415927f;
	mState         = &XObjActorDeca::StateHold;
	fn_8010706C(this);
	_9E0 = 0;
	_9E4 = 0;
	for (int i = 0; i < 16; i++) {
		_9E8[i] = 0;
	}
	_A28 = 0;
	_A2C = 0;
}
