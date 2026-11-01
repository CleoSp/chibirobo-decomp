#include "game/particle_srt.h"
#include "game/xobjactorfamily.h"

// XObjActorMama, Mrs. Sanderson

void XObjActorMama::vf150()
{
	mState = NULL;
}

void XObjActorMama::vf14C(u32 frames)
{
	if (mState) {
		(this->*mState)(frames);
	}
}

XObjActorMama::~XObjActorMama()
{
}

HSD_JObj* XObjActorMama::vf168()
{
	s32 path[4] = { 0, 4, 0, 0 };
	return FindJoint(mJObj, 4, path);
}

CVec XObjActorMama::vf16C()
{
	s32 path[4] = { 0, 4, 0, 0 };
	return fn_8007E7C8(this, 4, path);
}

f32 XObjActorMama::vf17C()
{
	return -0.17453292f;
}

f32 XObjActorMama::vf178()
{
	return 0.17453292f;
}

f32 XObjActorMama::vf174()
{
	return -0.6981317f;
}

f32 XObjActorMama::vf170()
{
	return 0.6981317f;
}

s32 XObjActorMama::vf124(s32 argc, s32* argv)
{
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
		break;
	}
	return 0;
}

void XObjActorMama::StateNone(u32 frames)
{
}

// lets go of what she carries
void XObjActorMama::Drop()
{
	s32 kind = -1;
	if (TestFlag(_C8C, 4)) {
		_C8C.mFlags &= ~4;
		kind = 0x7D;
	} else if (TestFlag(_C8C, 1)) {
		_C8C.mFlags &= ~1;
		kind = 0x40;
	} else if (TestFlag(_C8C, 2)) {
		_C8C.mFlags &= ~2;
		kind = 0x5B;
	} else if (TestFlag(_C8C, 8)) {
		_C8C.mFlags &= ~8;
		kind = 0x196;
	} else if (TestFlag(_C8C, 0x10)) {
		_C8C.mFlags &= ~0x10;
		kind = 0x1A1;
	} else if (TestFlag(_C8C, 0x20)) {
		_C8C.mFlags &= ~0x20;
		kind = 0x45B;
	}
	if (kind < 0) {
		return;
	}
	switch (kind) {
	case 0x40:
		for (s32 i = 0; i < 64; i++) {
			XObj* obj = GAME_POOL_9734->Get(i);
			if (kind == obj->GetDescKind()) {
				obj->mFlags &= ~0x10000ULL;
			}
		}
		break;
	case 0x5B:
	case 0x7D:
	case 0x196:
	case 0x45B:
		for (s32 i = 0; i < 288; i++) {
			XObj* obj = GAME_POOL_984C->Get(i);
			if (kind == obj->GetDescKind()) {
				obj->mFlags &= ~0x10000ULL;
			}
		}
		break;
	case 0x1A1:
		for (s32 i = 0; i < 288; i++) {
			XObj* obj = GAME_POOL_984C->Get(i);
			if (obj->GetDescKind() == 0x1A0) {
				obj->mFlags &= ~0x10000ULL;
			} else if (kind == obj->GetDescKind()) {
				obj->mFlags &= ~0x10000ULL;
			}
		}
		break;
	}
}

void XObjActorMama::Update(u32 frames)
{
	fn_80107DE0(this, frames);
}

void XObjActorMama::vf15C()
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
	mState         = &XObjActorMama::StateNone;
	fn_8010706C(this);
}
