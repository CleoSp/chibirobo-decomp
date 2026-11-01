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
