#include "game/hsd_jobj.h"
#include "game/cparticle.h"
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
extern "C" {
void fn_801076B0(XObjActor* self, Vec* target, f32 rate);
void fn_801076FC(XObjActor* self, Vec* target, f32 rate);
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
