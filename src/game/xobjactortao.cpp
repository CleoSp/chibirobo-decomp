#include "game/particle_srt.h"
#include "game/xobjactorfamily.h"

// XObjActorTao, the dog

void XObjActorTao::vf150()
{
	mState = NULL;
}

void XObjActorTao::vf14C(u32 frames)
{
	if (mState) {
		(this->*mState)(frames);
	}
}

XObjActorTao::~XObjActorTao()
{
}

HSD_JObj* XObjActorTao::vf168()
{
	s32 path[4] = { 0, 0, 0, 0 };
	return FindJoint(mJObj, 4, path);
}

CVec XObjActorTao::vf16C()
{
	s32 path[4] = { 0, 0, 0, 0 };
	return fn_8007E7C8(this, 4, path);
}

bool XObjActorTao::IsFree()
{
	if ((mFlags & 1) == 0) {
		return true;
	}
	return !(mStatus.mFlags & 0x2000);
}

s32 XObjActorTao::vf124(s32 argc, s32* argv)
{
	fn_801070A4(this, argc, argv);
	if (argc == 0) {
		return -1;
	}
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
	case 2:
		Drop();
		switch (argv[1]) {
		case 0x102:
			for (s32 i = 0; i < 64; i++) {
				XObj* obj = GAME_POOL_9734->Get(i);
				if (argv[1] == obj->GetDescKind()) {
					_A84 = obj;
					obj->mFlags |= 0x10000ULL;
					switch (argv[1]) {
					case 0x102:
						_C8C.mFlags |= 2;
						break;
					}
				}
			}
			break;
		case 0x159:
			for (s32 i = 0; i < 15; i++) {
				XObj* obj = GAME_POOL_975C->Get(i);
				if (argv[1] == obj->GetDescKind()) {
					_A84 = obj;
					obj->mFlags |= 0x10000ULL;
					_C8C.mFlags |= 1;
				}
			}
			break;
		case 0xF8: {
			XObj* obj;
			_9E8[0] = argv[2];
			u8* data;
			s32 size;
			size  = GAME_POOL_97D4->mSize;
			data  = GAME_POOL_97D4->mData;
			s32 n = -1;
			for (s32 i = 0; i < 21; i++) {
				obj = (XObj*)(data + i * size);
				if (argv[1] == obj->GetDescKind()) {
					if (++n == _9E8[0]) {
						obj->mFlags |= 0x10000ULL;
						_A84 = obj;
						_C8C.mFlags |= 4;
						break;
					}
				}
			}
			break;
		}
		case 0x2A3:
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = GAME_POOL_984C->Get(i);
				if (argv[1] == obj->GetDescKind()) {
					obj->mFlags |= 0x10000ULL;
					_C8C.mFlags |= 8;
					_A84 = obj;
				}
			}
			break;
		case 0x46F:
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = GAME_POOL_984C->Get(i);
				if (argv[1] == obj->GetDescKind()) {
					obj->mFlags |= 0x10000ULL;
					_C8C.mFlags |= 0x20;
					_A84 = obj;
				}
			}
			break;
		default:
			if (_A84 != NULL) {
				HSD_JObjClearUserMtx(_A84->mJObj);
				_A84 = NULL;
			}
			break;
		}
		break;
	case 3:
		_A4C   = argv[1];
		mState = &XObjActorTao::StatePickUp;
		fn_8010706C(this);
		_9E0 = 0;
		break;
	case 4:
		_A4C = -1;
		if (_A50 != NULL) {
			_A50->Release();
		}
		_9E0 = 0;
		break;
	case 5:
		if (argv[1] != 0) {
			_CF4 = 0x82;
			_CF8 = 1;
		} else {
			_CF8 = 0;
			_CF4 = -1;
			if (_CFC != NULL) {
				_CFC->Release();
			}
		}
		_9E0 = 0;
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

void XObjActorTao::StateNone(u32 frames)
{
}

static inline Vec TaoPosition(const CJObj& obj)
{
	return GetJObjPos(obj.mJObj);
}

static inline MtxPtr TaoMatrix(const CJObj& obj)
{
	return HSD_JObjGetMtxPtr(obj.mJObj);
}

// picks up the object _A4C names and holds it in its mouth
void XObjActorTao::StatePickUp(u32 frames)
{
	switch (_990) {
	case 0:
		if (_A50 != NULL) {
			_A50->Release();
		}
		if (_A4C >= 0) {
			_A50 = ReadObj(_A4C);
		} else {
			_A50 = NULL;
		}
		_990++;
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
			s32 path[6] = { 0, 0, 0, 0, 0, 0 };
			jobj.Attach(FindJoint(mJObj, 6, path));
			Vec pos = TaoPosition(jobj);
			((UnkCarried*)_A50)->SetMoveTranslate(pos.x, HSD_JObjGetTranslationY(mJObj), pos.z, 0);
			Vec scale;
			PSMTXCopy(TaoMatrix(jobj), _C94);
			HSD_MtxGetScale(_C94, &scale);
			scale.x = fn_800621E0(_A50).x / scale.x;
			scale.y = fn_800621E0(_A50).y / scale.y;
			scale.z = fn_800621E0(_A50).z / scale.z;
			Mtx scaleMtx;
			Mtx transMtx;
			Mtx m;
			PSMTXScale(scaleMtx, scale.x, scale.y, scale.z);
			PSMTXConcat(_C94, scaleMtx, m);
			PSMTXTrans(transMtx, 0.0f, -10.0f, 0.0f);
			PSMTXConcat(m, transMtx, m);
			HSD_JObj* joint = _A50->mJObj;
			MtxPtr mp       = m;
			HSD_JObjCopyMtx(joint, mp);
			_A50->mFlags &= ~0x0200000000000000ULL;
		}
		break;
	}
}

void XObjActorTao::vf158(u32 frames)
{
	if (mStatus.Check(2)) {
		mStatus.mFlags &= ~2;
		fn_8001E904(GAME_SOUND, 0x15D, 0xFF);
	}
}

void XObjActorTao::vf15C()
{
	mStatus.mFlags = 0;
	_C8C.mFlags    = 0;
	_980           = 0.0f;
	_984           = 0.0f;
	_988           = 0.0f;
	_C84           = _988;
	_C88           = 0;
	mState         = &XObjActorTao::StateNone;
	fn_8010706C(this);
	_9E8[0] = 0;
	_9E8[1] = 0;
	_9E8[2] = 0;
	_9E8[3] = 0;
	_9E8[4] = 0;
	_9E8[5] = 0;
	_9E8[6] = 0;
	_9E8[7] = 0;
	_CF4    = -1;
	_CF8    = 0;
	_CFC    = NULL;
	_CF4    = 0x82;
	_CF8    = 1;
	mStatus.mFlags |= 0x2000;
}
