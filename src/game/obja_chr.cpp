#include "game/cobj.h"
#include "game/xobjs.h"
#include "game/gmath.h"
extern "C" {
#include <dolphin/mtx/vec.h>
}
#include "game/cvec.h"
#include <math.h>

// XObjChr, the robot, and the menus it opens (DrsMenu, AtcMenu, OctaMenu). nothing here has
// a name in the dol, so everything is extern "C" under its address. most of the small
// functions after the huge update functions are out of line copies of inlines they stopped
// inlining once they got too big

extern "C" {
void PSVECScale(Vec* src, Vec* dst, f32 scale);
s32 fn_8006E9AC(s32 port);
s32 fn_8006EAF0(s32 port);
BOOL fn_800D4FE8(const struct UnkChrSub* timer);
u32 CheckStageFlag(s32 stage, u32 mask);
}

// the objects the robot works with keep 64 bits of flags
struct UnkActorId {
	u8 _00[0x14]; // _00
	s16 mId;      // _14
};

struct UnkChrSub {
	u8 _00[4]; // _00
};

struct UnkActor {
	u8 _000[0x228];     // _000
	u64 mFlags;         // _228
	u8 _230[0x8];       // _230
	UnkChrSub mSub238;  // _238
	u8 _23C[0x4];       // _23C
	UnkActorId* mIdObj; // _240
};

// puts a CObjVec at a value and its target there too, the speed stays
static inline void SetNow(CObjVec* vec, f32 x, f32 y, f32 z)
{
	vec->x         = x;
	vec->y         = y;
	vec->z         = z;
	vec->mTarget.x = x;
	vec->mTarget.y = y;
	vec->mTarget.z = z;
}

// the y angle that faces from one point to another
static inline f32 GetAngle(const Vec& from, const Vec& to)
{
	return atan2(to.x - from.x, to.z - from.z);
}

static inline s16 GetActorId(UnkActorId* obj)
{
	if (obj != NULL) {
		return obj->mId;
	}
	return 0;
}

// a placement table, 0x20 byte entries in whole hundredths
struct UnkPlaceEntry {
	s32 _00;      // _00
	s32 mPos[3];  // _04
	s32 mRotY;    // _10, degrees
	s32 mPos2[3]; // _14
};

struct UnkPlaceTable {
	UnkPlaceEntry* mEntries; // _00
};

// the robot's state machine, a state and its previous one
struct UnkState {
	s32 _00;    // _00
	s32 mState; // _04
};

struct UnkHolder {
	u8 _000[0x784]; // _000
	s32 mId;        // _784
};

struct UnkLink {
	u8 _00[0xC];      // _00
	s32 mKind;        // _0C
	u8 _10[0x10];     // _10
	UnkHolder* mHold; // _20
	s32 _24;          // _24
	s32 mHoldId;      // _28
	u8 _2C[0x10];     // _2C
	UnkChrSub mSub3C; // _3C
	u8 _40[0x8];      // _40
	f32 _48;          // _48
};

struct UnkTimer {
	u8 _00[0x24]; // _00
	f32 mTime;    // _24
};

struct UnkPair {
	s32 _000;       // _000
	u8 _004[0x294]; // _004
	s32 _298;       // _298
};

struct UnkChrState {
	u8 _000[0x980]; // _000
	u32 mFlags;     // _980
};

struct UnkBig {
	u8 _00000[0x5D080]; // _00000
	s32 _5D080;         // _5D080
	u8 _5D084[0x8ADA0]; // _5D084
	u8 _E7E24[0x4];     // _E7E24
};

struct UnkChrSlot {
	u8 _00[0x54]; // _00
};

// the robot, a CXObjIntp underneath. offsets are usa
struct UnkChr {
	HSD_JObj* mJObj;      // _000
	u8 _004[0xB0];        // _004
	UnkChrSub mSubB4;     // _0B4
	u8 _0B8[0x194];       // _0B8
	UnkChrSub mSub24C;    // _24C
	u8 _250[0xC];         // _250
	u32 _25C[2];          // _25C
	UnkChrSub mSub264;    // _264
	u8 _268[0xC];         // _268
	s32 mIndex;           // _274
	u8 _278[0x480];       // _278
	f32 _6F8;             // _6F8
	f32 _6FC[2];          // _6FC
	u8 _704[0x78];        // _704
	f32 _77C;             // _77C
	f32 _780;             // _780
	s32 mId;              // _784
	u8 _788[0x128];       // _788
	u32 mFlags;           // _8B0
	u8 _8B4[0x138];       // _8B4
	UnkChrSub mSub9EC;    // _9EC
	u8 _9F0[0x50];        // _9F0
	f32 _A40;             // _A40
	UnkChrSub mTimer;     // _A44
	s32 mTimerState;      // _A48
	u8 _A4C[0x18];        // _A4C
	UnkActor* mTarget;    // _A64
	u8 _A68[0x5C];        // _A68
	s32 mStateAC4;        // _AC4
	u8 _AC8[0x18];        // _AC8
	UnkHolder* mHoldAE0;  // _AE0
	u8 _AE4[0x4];         // _AE4
	s32 mHoldIdAE8;       // _AE8
	u8 _AEC[0x7C];        // _AEC
	s32 mStateB68;        // _B68
	u8 _B6C[0x18];        // _B6C
	UnkHolder* mHoldB84;  // _B84
	u8 _B88[0x4];         // _B88
	s32 mHoldIdB8C;       // _B8C
	u8 _B90[0x1C];        // _B90
	s32 mStateBAC;        // _BAC
	u8 _BB0[0x18];        // _BB0
	UnkHolder* mHoldBC8;  // _BC8
	u8 _BCC[0x4];         // _BCC
	s32 mHoldIdBD0;       // _BD0
	u8 _BD4[0x70];        // _BD4
	s32 _C44;             // _C44
	u8 _C48[0x24];        // _C48
	s32 _C6C;             // _C6C
	u8 _C70[0xC];         // _C70
	f32 _C7C;             // _C7C
	u8 _C80[0xBC];        // _C80
	s32 _D3C;             // _D3C
	u8 _D40[0xC74];       // _D40
	UnkChrSlot mSlots[4]; // _19B4
	u8 _1B04[0xC];        // _1B04
	u64 mFlags2;          // _1B10
};

// the game object behind lbl_8025C400, the CGs is only its start
struct UnkGame {
	u8 _0000[0x3444]; // _0000
	s32 mStage;       // _3444
};

// a table of rooms, each with a list of points and a grid
struct UnkRoomList {
	u8* mData;   // _00
	s32 mCount;  // _04
	u8 _08[0x8]; // _08
	s32 mStride; // _10
};

struct UnkRoom {
	s32 _0000[0x400]; // _0000
	s16 _1000[0x400]; // _1000
	s32 _1800;        // _1800
};

struct UnkMap {
	u8 _0000[0x8580];      // _0000
	s32 _8580;             // _8580
	u8 _8584[0x104C];      // _8584
	s32 _95D0;             // _95D0
	s32 _95D4;             // _95D4
	u8 _95D8[0xBC];        // _95D8
	UnkRoomList mLists[4]; // _9694
	u8 _96E4[0x17C];       // _96E4
	u32 _9860[0x400];      // _9860
	s32 _A860;             // _A860
	u8 _A864[0xC];         // _A864
	UnkRoom mRooms[1];     // _A870
};

struct UnkStageTable {
	UnkStageObj* mObjs; // _00
};

// ---- functions ----

extern "C" {

void fn_800B1B0C(UnkLink* link, s32 value)
{
	link->_24 = value;
}

void fn_800B1B14(UnkState* state, s32 value)
{
	state->mState = value;
}

void fn_800B1B1C(UnkState* state, s32 value)
{
	state->mState = value;
}

BOOL fn_800B1B24(const UnkChr* chr)
{
	return chr->mTimerState == 4 && fn_800D4FE8(&chr->mTimer);
}

void fn_800B1B70(UnkLink* link, UnkHolder* hold)
{
	link->mHold   = hold;
	link->mHoldId = link->mHold->mId;
}

f32 fn_800B1B84(void)
{
	return fn_8006E9AC(2) / 100.0f;
}

f32 fn_800B1BCC(void)
{
	return fn_8006E9AC(1) / 100.0f;
}

f32 fn_800B1C14(void)
{
	return fn_8006E9AC(0) / 100.0f;
}

f32 fn_800B1C5C(const UnkPadStatus* pad)
{
	f32 x = pad->_34;
	f32 y = pad->_38;
	return sqrtf(x * x + y * y);
}

BOOL fn_800B1D50(UnkActor* obj)
{
	return (obj->mFlags & 0x400000000ULL) != 0;
}

UnkChrSub* fn_800B1D84(UnkChr* chr)
{
	return &chr->mSubB4;
}

void fn_800B1DDC(UnkState* state, s32 value)
{
	state->mState = value;
}

void fn_800B1DE4(s32* value)
{
	*value = 1;
}

u32 fn_800B1DF0(UnkChr* chr, u32 mask)
{
	return chr->mFlags & mask;
}

u32 fn_800B1DFC(UnkChr* chr)
{
	return chr->mFlags;
}

BOOL fn_800B1E04(void)
{
	return FALSE;
}

BOOL fn_800B1E0C(void* self, const Vec& pos, CXObj* obj)
{
	f32 angle = GetAngle(pos, obj->mMoveTranslate);
	angle     = WrapAngle(angle - obj->mMoveRotate.y);
	return angle >= 2.0943952f && angle <= 4.1887903f;
}

f32 fn_800B1EB8(UnkChr* chr)
{
	return chr->_77C;
}

s32 fn_800B2004(UnkChr* chr)
{
	return chr->mId;
}

BOOL fn_800B200C(void)
{
	return FALSE;
}

BOOL fn_800B2014(UnkActor* obj)
{
	return (obj->mFlags & 0x8000000000ULL) != 0;
}

BOOL fn_800B2048(UnkActor* obj)
{
	return (obj->mFlags & 0x100000000ULL) != 0;
}

void fn_800B207C(s32* value)
{
	*value = 0;
}

BOOL fn_800B2088(UnkTimer* timer)
{
	return timer->mTime > 0.0f;
}

s32 fn_800B20A0(UnkMap* map, s32 list)
{
	return map->mLists[list].mCount;
}

UnkStageObj* fn_800B20B4(UnkStageTable* table, s32 i)
{
	return &table->mObjs[i];
}

UnkChrSub* fn_800B20C4(UnkActor* obj)
{
	return &obj->mSub238;
}

f32 fn_800B20CC(const Vec& from, const Vec& to)
{
	return GetAngle(from, to);
}

void* fn_800B2108(UnkMap* map, s32 list, s32 i)
{
	return map->mLists[list].mData + i * map->mLists[list].mStride;
}

s32 fn_800B2128(UnkMap* map)
{
	return map->_A860;
}

f32 fn_800B2134(UnkChr* chr)
{
	return chr->_780;
}

void fn_800B213C(UnkState* state, s32 value)
{
	state->mState = value;
}

void fn_800B2144(UnkChr* chr, s32 state, UnkHolder* hold)
{
	chr->mStateBAC = state;
	if (hold != NULL) {
		chr->mHoldBC8   = hold;
		chr->mHoldIdBD0 = chr->mHoldBC8->mId;
	}
}

BOOL fn_800B2164(UnkActor* obj)
{
	return (obj->mFlags & 0x1000000000ULL) != 0;
}

void fn_800B2198(UnkChr* chr, s32 state, UnkHolder* hold)
{
	chr->mStateB68 = state;
	if (hold != NULL) {
		chr->mHoldB84   = hold;
		chr->mHoldIdB8C = chr->mHoldB84->mId;
	}
}

BOOL fn_800B21B8(UnkActor* obj)
{
	return (obj->mFlags & 0x800000000ULL) != 0;
}

void fn_800B21EC(UnkChr* chr)
{
	if (chr->mTarget != NULL) {
		chr->mTarget->mFlags &= ~0x0400000000000000ULL;
		chr->mTarget = NULL;
	}
	chr->mTimerState = 0;
}

void fn_800B2230(UnkChr* chr, s32 state, UnkHolder* hold)
{
	chr->mStateAC4 = state;
	if (hold != NULL) {
		chr->mHoldAE0   = hold;
		chr->mHoldIdAE8 = chr->mHoldAE0->mId;
	}
}

BOOL fn_800B2250(UnkActor* obj)
{
	return (obj->mFlags & 0x200000000ULL) != 0;
}

u32 fn_800B2284(UnkChr* chr, s32 i)
{
	return chr->_25C[i];
}

BOOL fn_800B2294(UnkActor* obj)
{
	return (obj->mFlags & 0x2000000000ULL) != 0;
}

void fn_800B22C8(UnkState* state, s32 value)
{
	state->mState = value;
}

BOOL fn_800B22D0(UnkPair* pair)
{
	return pair->_000 != 0 || pair->_298 != 0;
}

f32 fn_800B22F8(void)
{
	return fn_8006EAF0(2) / 100.0f;
}

f32 fn_800B2340(void)
{
	return fn_8006EAF0(1) / 100.0f;
}

void fn_800B2388(CObjVec* vec, f32 y)
{
	vec->mTarget.y = y;
}

BOOL fn_800B2390(void)
{
	return TRUE;
}

f32 fn_800B2398(f32 x)
{
	return sqrtf(x);
}

f32 fn_800B247C(const UnkPlaceTable& table, s32 i)
{
	return DEG2RAD * table.mEntries[i].mRotY;
}

s32 fn_800B24BC(UnkLink* link)
{
	return link->mHoldId;
}

UnkChrSub* fn_800B24C4(UnkChr* chr)
{
	return &chr->mSub24C;
}

CObjVec* fn_800B24CC(CXObj* obj)
{
	return &obj->mMoveScale;
}

CObjVec* fn_800B24D4(CXObj* obj)
{
	return &obj->mMoveRotate;
}

s16 fn_800B24DC(UnkActor* obj)
{
	if (obj->mIdObj != NULL) {
		return GetActorId(obj->mIdObj);
	}
	return -1;
}

s32 fn_800B2504(UnkMap* map, s32 room)
{
	return map->mRooms[room]._1800;
}

void fn_800B2518(UnkActor* obj, u64 mask)
{
	obj->mFlags &= ~mask;
}

u32 fn_800B253C(UnkChrState* state, u32 mask)
{
	return state->mFlags & mask;
}

s16 fn_800B2548(UnkMap* map, s32 room, s32 i)
{
	return map->mRooms[room]._1000[i];
}

void fn_800B2564(UnkActor* obj, u64 mask)
{
	obj->mFlags |= mask;
}

f32 fn_800B2580(UnkChr* chr)
{
	return chr->_6FC[chr->mIndex];
}

f32 fn_800B2594(UnkChr* chr)
{
	return chr->_6F8;
}

f32 fn_800B259C(const Vec& a, const Vec& b)
{
	f32 dx = a.x - b.x;
	f32 dz = a.z - b.z;
	return sqrtf(dx * dx + dz * dz);
}

BOOL fn_800B26A0(UnkLink* link, s32 value)
{
	return link->mKind == value;
}

s32 fn_800B26B4(CXObj* obj)
{
	return obj->mId;
}

u64 fn_800B26BC(UnkActor* obj, u64 mask)
{
	return obj->mFlags & mask;
}

s32 fn_800B26D0(UnkMap* map, s32 room, s32 i)
{
	return map->mRooms[room]._0000[i];
}

void fn_800B26EC(void* self, HSD_JObj* jobj, Vec* scale)
{
	HSD_JObjGetScale(jobj, scale);
}

HSD_JObj* fn_800B2764(CXObj* obj, s32 depth, s32* path)
{
	return GetJointByPath(obj->mJObj, path, depth);
}

Vec fn_800B27E0(const UnkPlaceTable& table, s32 i)
{
	Vec pos = { table.mEntries[i].mPos[0] / 100.0f, table.mEntries[i].mPos[1] / 100.0f, table.mEntries[i].mPos[2] / 100.0f };
	return pos;
}

Vec fn_800B289C(const UnkPlaceTable& table, s32 i)
{
	Vec pos = { table.mEntries[i].mPos2[0] / 100.0f, table.mEntries[i].mPos2[1] / 100.0f, table.mEntries[i].mPos2[2] / 100.0f };
	return pos;
}

void fn_800B2958(CObjVec* vec, const Vec& target)
{
	vec->SetTarget(target);
}

void fn_800B2974(CVec* vec, f32 x, f32 y, f32 z)
{
	vec->Set(x, y, z);
}

void fn_800B2984(CObjVec* vec, f32 x, f32 y, f32 z)
{
	vec->mSpeed.Set(x, y, z);
}

void fn_800B2994(CVec* vec, f32 scale)
{
	PSVECScale(vec, vec, scale);
}

BOOL fn_800B29B8(void)
{
	return TRUE;
}

BOOL fn_800B29C0(void)
{
	return TRUE;
}

BOOL fn_800B29C8(UnkState* state)
{
	return state->mState == 0 || state->mState == 6 || state->mState == 9;
}

BOOL fn_800B29F4(void)
{
	return TRUE;
}

BOOL fn_800B29FC(void)
{
	return TRUE;
}

BOOL fn_800B2A04(UnkState* state)
{
	return state->mState == 0 || state->mState == 10;
}

BOOL fn_800B2A28(UnkState* state)
{
	return state->mState == 0 || state->mState == 11;
}

BOOL fn_800B2A4C(UnkState* state)
{
	return state->mState == 0 || state->mState == 4;
}

BOOL fn_800B2A70(UnkState* state)
{
	return state->mState == 0;
}

BOOL fn_800B2A80(void)
{
	return TRUE;
}

CVec* fn_800B2A88(CVec* vec)
{
	return &vec->Normalize();
}

void fn_800B2BB0(UnkChr* chr, s32 value, BOOL reset)
{
	chr->_C44 = value;
	if (reset) {
		chr->_C6C = 1;
		chr->_C7C = 0.0f;
	}
}

void fn_800B2BD0(UnkChr* chr, BOOL reset)
{
	chr->_D3C = 0;
	if (reset) {
		s32 path[] = { 0 };
		HSD_JObjSetRotationX(GetJointByPath(chr->mJObj, path, 1), 0.0f);
	}
}

void* fn_800B2CFC(UnkBig* big)
{
	return &big->_E7E24;
}

u32 fn_800B2D08(const UnkGame* game, u32 mask)
{
	return CheckStageFlag(game->mStage, mask);
}

BOOL fn_800B2D2C(UnkState* state, s32 value)
{
	return value == state->mState;
}

BOOL fn_800B2D40(UnkState* state, s32 value)
{
	return value == state->mState;
}

UnkChrSub* fn_800B2D54(UnkChr* chr)
{
	return &chr->mSub9EC;
}

BOOL fn_800B2D5C(void)
{
	return TRUE;
}

BOOL fn_800B2D64(void)
{
	return TRUE;
}

BOOL fn_800B2D6C(UnkState* state)
{
	return state->mState == 0 || state->mState == 3 || state->mState == 4 || state->mState == 7 || state->mState == 8 || state->mState == 5
	    || state->mState == 6 || state->mState == 9;
}

BOOL fn_800B2DC0(UnkState* state)
{
	return state->mState != 8 && state->mState != 9;
}

BOOL fn_800B2DE4(UnkState* state)
{
	return state->mState == 0 || state->mState == 9 || state->mState == 10 || state->mState == 11;
}

BOOL fn_800B2E18(UnkState* state)
{
	return state->mState == 0 || state->mState == 10 || state->mState == 11;
}

BOOL fn_800B2E44(UnkState* state)
{
	return state->mState == 0 || state->mState == 2 || state->mState == 3 || state->mState == 4 || state->mState == 17;
}

BOOL fn_800B2E80(UnkState* state)
{
	return state->mState == 0 || state->mState == 10;
}

s32 fn_800B2EA4(UnkMap* map)
{
	return map->_8580;
}

s32 fn_800B2EB0(UnkChr* chr)
{
	return chr->_D3C;
}

void* fn_800B2EB8(UnkMap* map, s32 i)
{
	return map->mLists[3].mData + i * map->mLists[3].mStride;
}

void fn_800B2ED0(UnkTimer* timer, f32 time)
{
	timer->mTime = time;
}

void fn_800B2ED8(CObjVec* vec, const Vec& v)
{
	SetNow(vec, v.x, v.y, v.z);
}

u32 fn_800B2F00(UnkState* state, u32 mask)
{
	return state->mState & mask;
}

UnkChrSlot* fn_800B2F0C(UnkChr* chr, u8 i)
{
	UnkChrSlot* slots = chr->mSlots;
	return &slots[i];
}

void fn_800B2F20(CObjVec* vec, f32 z)
{
	vec->z         = z;
	vec->mTarget.z = z;
}

void fn_800B2F2C(CObjVec* vec, f32 x)
{
	vec->x         = x;
	vec->mTarget.x = x;
}

BOOL fn_800B2F38(UnkState* state, s32 value)
{
	return value == state->mState;
}

BOOL fn_800B2F4C(UnkMap* map, s32 value)
{
	return value == (map->_95D4 >= 0 ? map->_95D4 : map->_95D0);
}

BOOL fn_800B2F74(UnkBig* big)
{
	return big->_5D080 != 0;
}

BOOL fn_800B2F8C(UnkState* state, s32 value)
{
	return value == state->mState;
}

f32 fn_800B2FA0(Vec* a, Vec* b)
{
	return PSVECDistance(a, b);
}

void fn_800B2FC0(void)
{
}

u64 fn_800B2FC4(UnkChr* chr, u64 mask)
{
	return chr->mFlags2 & mask;
}

void fn_800B2FD8(CObjVec* vec, f32 y)
{
	vec->y         = y;
	vec->mTarget.y = y;
}

s32 fn_800B2FE4(CXObj* obj)
{
	return obj->mAnimId;
}

void fn_800B2FEC(CObjVec* vec, f32 x, f32 y, f32 z)
{
	SetNow(vec, x, y, z);
}

void fn_800B3008(CVec* vec, f32 z)
{
	vec->z = z;
}

void fn_800B3010(CVec* vec, f32 x)
{
	vec->x = x;
}

f32 fn_800B3018(f32 angle)
{
	return WrapAngle(angle);
}

s32 fn_800B304C(UnkLink* link)
{
	return link->mKind;
}

f32 fn_800B3054(UnkLink* link)
{
	return link->_48;
}

UnkChrSub* fn_800B305C(UnkLink* link)
{
	return &link->mSub3C;
}

void fn_800B3088(CObjVec* vec, f32 z)
{
	vec->mSpeed.z = z;
}

void fn_800B3090(CObjVec* vec, f32 x)
{
	vec->mSpeed.x = x;
}

CVec* fn_800B3098(CObjVec* vec)
{
	return &vec->mSpeed;
}

f32 fn_800B30A0(CVec* vec)
{
	return vec->Length();
}

CVec* fn_800B30C0(CObjVec* vec)
{
	return &vec->mTarget;
}

void fn_800B30C8(CObjVec* vec, f32 z)
{
	vec->mTarget.z = z;
}

void fn_800B30D0(CObjVec* vec, f32 x)
{
	vec->mTarget.x = x;
}

f32 fn_800B30D8(f32 x)
{
	return cos(x);
}

f32 fn_800B30FC(UnkChr* chr)
{
	return chr->_A40;
}

f32 fn_800B3104(f32 x)
{
	return sin(x);
}

UnkHolder* fn_800B3128(UnkLink* link)
{
	return link->mHold;
}

BOOL fn_800B3130(UnkState* state, s32 value)
{
	return value == state->mState;
}

UnkChrSub* fn_800B3144(UnkChr* chr)
{
	return &chr->mSub264;
}

CObjVec* fn_800B314C(CXObj* obj)
{
	return &obj->mMoveTranslate;
}

u32 fn_800B3154(UnkMap* map, s32 i)
{
	return map->_9860[i];
}

BOOL fn_800B3168(UnkLink* link)
{
	return link->mHold != NULL;
}

BOOL fn_800B317C(UnkState* state, s32 value)
{
	return value == state->mState;
}

u32 fn_800B31AC(UnkChrState* state, u32 mask)
{
	return state->mFlags & mask;
}
} // extern "C"
