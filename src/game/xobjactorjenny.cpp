#include "game/particle_srt.h"
#include "game/xobjactorjenny.h"

// XObjActorJenny

extern "C" CVec fn_8007E7C8(XObj* obj, s32 depth, s32* path); // where the joint is

// the objects placed in the house by id
struct UnkObjTable {
	void* _00; // _00
};
#define GAME_OBJTABLE ((UnkObjTable*)(lbl_8025C400 + 0x5D080))
#define GAME_PLAYER   (*(UnkPlayer**)(lbl_8025C400 + 0x96BC))

extern "C" XObj* fn_8007F570(UnkObjTable* table, s16 id);

static inline XObj* FindObj(s16 id)
{
	UnkObjTable* table = GAME_OBJTABLE;
	if (table->_00 != NULL) {
		return fn_8007F570(table, id);
	}
	return NULL;
}

// the robot, what Jenny looks at
class UnkPlayer : public XObj {
public:
	u32 _980;                 // _980
	u8 _984[0x12DF4 - 0x984]; // _984
	s32 _12DF4;               // _12DF4
};

// more of the game object's pools, see game/scr.h
#define GAME_POOL_9734 ((UnkObjPool*)(lbl_8025C400 + 0x9734)) // 64 of them
#define GAME_POOL_984C ((UnkObjPool*)(lbl_8025C400 + 0x984C)) // 288 of them

static inline u32 TestFlag(const CStatus& status, u32 mask)
{
	return status.mFlags & mask;
}

// the XObjActor turns, game/xobjactorlook.cpp, the id there is what the actor turns to
extern "C" {
void fn_801076B0(XObjActor* self, Vec* target, f32 rate);
void fn_801076FC(XObjActor* self, Vec* target, f32 rate);
}

void XObjActorJenny::vf150()
{
	mState = NULL;
}

void XObjActorJenny::vf14C(u32 frames)
{
	if (mState) {
		(this->*mState)(frames);
	}
}

XObjActorJenny::~XObjActorJenny()
{
}

HSD_JObj* XObjActorJenny::vf168()
{
	s32 path[3] = { 0, 3, 0 };
	return GetJointFromRoot(mJObj, 3, path);
}

CVec XObjActorJenny::vf16C()
{
	s32 path[3] = { 0, 3, 0 };
	return fn_8007E7C8(this, 3, path);
}

f32 XObjActorJenny::vf17C()
{
	switch (mAnimId) {
	case 0x412:
	case 0x413:
	case 0x414:
		return 0.34906584f;
	}
	return -0.34906584f;
}

f32 XObjActorJenny::vf178()
{
	switch (mAnimId) {
	case 0x412:
	case 0x413:
	case 0x414:
		return 1.0471976f;
	}
	return 0.17453292f;
}

void XObjActorJenny::StateIdle(u32 frames)
{
}

extern "C" s32 fn_8012305C(XObjActorJenny* self, s32 argc, s32* argv)
{
	fn_801070A4(self, argc, argv);
	switch (argv[0]) {
	case 1:
		switch (argv[1]) {
		case 0:
			fn_8010771C(self, 0.08726646f);
			break;
		case 1:
			fn_801076B0(self, &GAME_PLAYER->mMoveTranslate, 0.08726646f);
			break;
		case 2:
			if (FindObj(argv[2]) != NULL) {
				fn_801076FC(self, &FindObj(argv[2])->mMoveTranslate, 0.017453292f * argv[3]);
			}
			break;
		case 3:
			fn_801076DC(self, 0.017453292f * argv[2], 0.017453292f * argv[3]);
			break;
		}
		break;
	default:
		if (argc < 1) {
			return 0;
		}
		self->_9E0 = argv[0];
		if (argc < 2) {
			self->_9E4 = 0;
		} else {
			self->_9E4 = argv[1];
		}
		for (int i = 0; i < 8; i++) {
			self->_9E8[i] = argv[i + 2];
		}
		if (self->_9E4 == 0xD0) {
			self->_A2C = (s32)FindObj(self->_9E8[0]);
		}
		break;
	}
	return 0;
}

// NONMATCHING: the paths sit at other .rodata offsets, the start of the file isn't decompiled
void XObjActorJenny::UpdateAnim(u32 speed)
{
	CXObjIntp::UpdateAnim(speed);
	CJObj jobj;
	if (_C90.mFlags & 8) {
		s32 path[7] = { 0, 3, 2, 0, 0, 1, 0 };
		jobj.Attach(GetJointFromRoot(mJObj, 7, path));
		HSD_JObjClearFlags(jobj.mJObj, 0x10);
	} else {
		s32 path[7] = { 0, 3, 2, 0, 0, 1, 0 };
		jobj.Attach(GetJointFromRoot(mJObj, 7, path));
		HSD_JObjSetFlags(jobj.mJObj, 0x10);
	}
}

// lets go of what she carries
void XObjActorJenny::Drop()
{
	s32 kind = -1;
	if (TestFlag(_C98, 1)) {
		_C98.mFlags &= ~1;
		kind = 0x3D;
	} else if (TestFlag(_C98, 2)) {
		_C98.mFlags &= ~2;
		kind = 0x3E;
	} else if (TestFlag(_C98, 4)) {
		_C98.mFlags &= ~4;
		kind = 0x3F;
	} else if (TestFlag(_C98, 8)) {
		_C98.mFlags &= ~8;
		kind = 0x3B;
	} else if (TestFlag(_C98, 0x10)) {
		_C98.mFlags &= ~0x10;
		kind = 0x401;
	}
	if (kind < 0) {
		return;
	}
	switch (kind) {
	case 0x3B:
	case 0x3D:
	case 0x3E:
	case 0x3F:
		for (s32 i = 0; i < 64; i++) {
			XObj* obj = GAME_POOL_9734->Get(i);
			if (kind == obj->GetDescKind()) {
				obj->mFlags &= ~0x10000ULL;
			}
		}
		break;
	case 0x401:
		for (s32 i = 0; i < 288; i++) {
			XObj* obj = GAME_POOL_984C->Get(i);
			if (kind == obj->GetDescKind()) {
				obj->mFlags &= ~0x10000ULL;
			}
		}
		break;
	}
}

void XObjActorJenny::vf15C()
{
	UnkPlayer* player = GAME_PLAYER;
	if (player->_980 & 0x40000000) {
		player->_980 = 0;
		player->_980 |= 0x20000000;
		player->_980 |= 7;
		player->_12DF4 = 1;
	}
	mStatus.mFlags = 0;
	_C90.mFlags    = 0;
	_C98.mFlags    = 0;
	_980           = 0.0f;
	_984           = 0.0f;
	_988           = 0.0f;
	_C84           = _988;
	_C88           = 0.0f;
	_C8C           = 0;
	_CA0           = 0;
	mState         = &XObjActorJenny::StateIdle;
	fn_8010706C(this);
	_9E0 = 0;
	_9E4 = 0;
	for (int i = 0; i < 16; i++) {
		_9E8[i] = 0;
	}
	_A28 = 0;
	_A2C = 0;
	_C90.mFlags &= ~8;
}
