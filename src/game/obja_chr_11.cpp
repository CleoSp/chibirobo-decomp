#include "game/obja_chr_shared.h"

extern "C" {

// turns the joint to an angle and lets go of what it looked at
static inline void TurnTo(CJointRot* rot, f32 angle)
{
	rot->mMode   = 2;
	rot->mTarget = angle;
	rot->mLookAt = NULL;
}

// turns the joint to an angle, it flips when it gets there
static inline void LookTo(CJointRot* rot, f32 angle)
{
	rot->mMode   = 1;
	rot->mTarget = angle;
}

static inline void PairMove(UnkMenuPair* pair, s32 dir)
{
	if (dir == 0) {
		fn_8001E904(GAME_E7E24, 0x86, 0xFF);
		if (pair->mMenu0._280 != 3) {
			pair->_544 = pair->_548;
		}
	} else if (dir > 0) {
		fn_8001E904(GAME_E7E24, 0x88, 0xFF);
	} else if (dir < 0) {
		fn_8001E904(GAME_E7E24, 0x86, 0xFF);
		if (pair->mMenu0._280 != 3) {
			pair->_544 = pair->_548;
		}
	}
	pair->mMenu0.vf10(dir);
	pair->mMenu1.vf10(dir);
}

// the picked item glows brighter and darker
static inline void AddPulse(s32& r, s32& g, s32& b, const OctaMenu* menu, s32 i)
{
	r += (s32)(25.0f * (f32)sin(1.2f * menu->mEntries[i].mTime) + 25.0f);
	g += (s32)(25.0f * (f32)sin(1.2f * menu->mEntries[i].mTime) + 25.0f);
	b += (s32)(25.0f * (f32)sin(1.2f * menu->mEntries[i].mTime) + 25.0f);
}

// puts the joint at an angle and keeps it there
static inline void SetTurn(CJointRot* rot, f32 angle)
{
	rot->mMode   = 1;
	rot->mTarget = angle;
	rot->mAngle  = angle;
}

static inline void OctaOpen(OctaMenu* menu, s32 a)
{
	if (menu->mOpen == 0) {
		menu->_16C = menu->mOwner->mMoveRotate.y;
		SetTurn(&menu->mOwner->mJointRots[0], 0.0f);
		SetTurn(&menu->mOwner->mJointRots[1], -1.52f);
	}
	menu->mOpen     = 1;
	menu->mSelected = a;
	if (menu->mSelected != 0) {
		menu->_00C = 1.0f;
	}
	Vec ofs = { 0.0f, 8.8f, 0.0f };
	menu->mOwner->vf10C(&ofs, &menu->mScreenX, &menu->mScreenY, NULL, 0);
	for (int i = 0; i < 8; i++) {
		menu->mEntries[i].mTime  = DEG2RAD * (rand() % 360);
		menu->mEntries[i].mSpin  = DEG2RAD * (rand() % 360);
		menu->mEntries[i].mScale = menu->mSelected != 0 ? 0.3f : 0.5f;
	}
	menu->mCursor = -1;
	menu->_170    = 0;
	for (int i = 0; i < 22; i++) {
		menu->mSparks[i].mAngle = DEG2RAD * (rand() % 360);
		menu->mSparks[i].mSpin  = DEG2RAD * (rand() % 360);
	}
	menu->mOwner->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
}

// the robot carries the item
static inline BOOL HasItem(UnkMenuOwner* chr, s16 id)
{
	return fn_800DCD30(&chr->mItems, id) < 0 ? FALSE : TRUE;
}

// stops every shot the robot has flying
static inline void StopShots(UnkMenuOwner* chr)
{
	UnkShot* shot = chr->mShots;
	for (int i = 0; i < 10; i++, shot++) {
		fn_800C2D18(shot);
	}
}

// the CPadOne under the CPad
static inline UnkPadOne* GetPad(const CPad& pad)
{
	return (UnkPadOne*)pad.mPad;
}

// how the head turns for each cursor, from no cursor through the eight items
static f32 sMenuTurnY[9] = { 0.0f, -20.0f, 0.0f, 20.0f, 20.0f, 20.0f, 0.0f, -20.0f, -20.0f };
static f32 sMenuTurnX[9] = { -20.0f, -20.0f, -30.0f, -20.0f, -10.0f, 0.0f, 10.0f, 0.0f, -10.0f };

static inline void SlotsSetValue(UnkSlots* slots, s32 id, s32 value)
{
	s32 i = slots->Find(id);
	if (i >= 0) {
		slots->mSlots[i].mValue = value;
	}
}

void fn_8009CEA0(UnkSlots* slots);
void fn_8009CEA0(UnkSlots* slots);

// these two come after their caller too
// puts a slot's value, slots it doesn't have are left alone
static inline void SetSlot(UnkSlots* slots, s32 id, bool on)
{
	s32 i = slots->Find(id);
	if (i >= 0) {
		slots->mSlots[i].mValue = on;
	}
}

// the command line's picks, cgame.cpp reads it, the slots are 1 more than the pick
struct UnkCmdLine {
	s32 mStage;    // _00
	s32 mSlot;     // _04
	u8 _08_0 : 2;  // _08
	u8 mPick1 : 2; // _08
	u8 mPick2 : 2; // _08
	u8 mPick3 : 2; // _08
	u8 _09[0x17];  // _09
};
extern "C" {
extern UnkCmdLine lbl_8020FBC0;
s32 fn_8006E940(s32 index);
BOOL fn_8006EC34(void);
}

// the kind of floor under what the robot stands on or holds, 0 for none
static inline u8 GetHoldKind(UnkMenuOwner* chr)
{
	s32 kind = 0;
	if (chr->_AC4 == 5 && chr->mHoldAE0 != NULL) {
		kind = chr->mHoldAE0->mPoints[0]._02;
	} else if (chr->_BAC == 5 && chr->mHoldBC8 != NULL) {
		kind = chr->mHoldBC8->mPoints[0]._02;
	} else if (chr->_B68 == 5 && chr->mHoldB84 != NULL) {
		kind = chr->mHoldB84->mPoints[0]._02;
	} else if (chr->mHeld[0] >= 0) {
		XObj* obj = GAME_OBJS[chr->mHeld[0]];
		f32 best  = NAN;
		for (int j = 0; obj->mPoints[j].mKind != -1; j++) {
			if (obj->IsPointOn(j)) {
				if ((fpclassify(best) == FP_NAN || obj->mPointDist[j] > best) && obj->mPointDist[j] <= 1.0f + chr->mMoveTranslate.y) {
					kind = obj->mPoints[j]._02;
					best = obj->mPointDist[j];
				}
			}
		}
	}
	return kind;
}
BOOL fn_8009FEDC(const UnkMenuOwner* obj);

// the robot is in a pose where it talks over everything
static inline bool IsChrTalking(const UnkMenuOwner* chr)
{
	bool ret = false;
	if (fn_8009FEDC(chr)) {
		if (chr->_158A4 != 0) {
			ret = true;
		}
	}
	return ret;
}

static inline bool IsChrTalkingNow(const UnkMenuOwner* chr)
{
	bool ret = false;
	if ((GAME_FLAGS & 0x40000000) && IsChrTalking(chr)) {
		ret = true;
	}
	return ret;
}

static inline bool IsChrSpeaking(const UnkMenuOwner* chr)
{
	bool ret = false;
	if (chr->_C44 == 4 || chr->_C44 == 5 || chr->_C44 == 6) {
		ret = true;
	}
	return ret;
}

static inline bool IsMuted(const UnkMenuOwner* chr)
{
	bool ret = false;
	if (IsChrTalkingNow(chr) && IsChrSpeaking(chr)) {
		ret = true;
	}
	return ret;
}

// stops walking and turning and drops what it was carrying around
static inline void StopMove(UnkMenuOwner* owner)
{
	owner->mSpeeds[0].Set(0.0f, 0.0f, 0.0f);
	owner->mSpeeds[1].Set(0.0f, 0.0f, 0.0f);
	owner->mSpeeds[2].Set(0.0f, 0.0f, 0.0f);
	owner->mMoveRotate.Set(owner->mMoveRotate.mTarget);
	if (owner->_BEC != 0) {
		fn_800C5A00(&owner->_BE8);
		owner->_BEC = 0;
		owner->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		owner->_A2C = 3;
	}
}
extern "C" {
void HSD_JObjReqAnim(HSD_JObj* jobj, f32 frame);
}

// the data the jobj's anim plays from
static inline u8* GetAnimData(HSD_JObj* jobj)
{
	HSD_FObj* fobj = jobj->aobj != NULL ? jobj->aobj->fobj : NULL;
	return fobj != NULL ? fobj->ad_head : NULL;
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
	return GetId(obj);
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

// what a floor search found
struct UnkFloorHit {
	s32 mTri;    // _00
	s32 mTrigon; // _04
	f32 mY;      // _08
};

// XObjMap as far as the floor search goes, see game/obja_map.cpp
struct UnkMapObj {
	u8 _000[0x980];       // _000
	s32 mTrigonCount;     // _980
	s32 mOrder[16];       // _984
	CTrigon mTrigons[16]; // _9C4

	// the first trigon by height that reaches up to y
	s32 FindStart(f32 y) const
	{
		for (int i = 0; i < mTrigonCount; i++) {
			if (mTrigons[mOrder[i]].mMinY <= y) {
				return i;
			}
		}
		return 0;
	}
};

BOOL fn_800B2ABC(const UnkMapObj* map, const Vec* pos, UnkFloorHit* hit, Vec* tri)
{
	s32 start = map->FindStart(0.2f + pos->y);
	for (int i = 0; i < map->mTrigonCount; i++) {
		s32 trigon = map->mOrder[(start + i) % map->mTrigonCount];
		int found  = ((CTrigon*)&map->mTrigons[trigon])->Check((Vec*)pos, &hit->mY, tri);
		if (found >= 0) {
			hit->mTrigon = trigon;
			hit->mTri    = found;
			return TRUE;
		}
	}
	return FALSE;
}

void fn_800B2BB0(UnkChr* chr, s32 value, BOOL reset)
{
	chr->_C44 = value;
	if (reset) {
		chr->_C6C = 1;
		chr->_C7C = 0.0f;
	}
}

// the game lets the robot idle, it is in control and nothing is going on
static inline bool CanIdle()
{
	bool ret = false;
	if ((GAME_FLAGS & 1) && !(GAME_FLAGS & 0x22) && !((UnkGame*)lbl_8025C400)->IsBusy()) {
		ret = true;
	}
	return ret;
}

// the robot's timer ran out
static inline bool IsTimeUp(const UnkMenuOwner* chr)
{
	bool ret = false;
	if (chr->_A48 == 4 && fn_800D4FE8(&chr->mTimer)) {
		ret = true;
	}
	return ret;
}

// the robot plays its way into the room, the other way round when it came back
static inline void EnterRoomAnim(UnkMenuOwner* chr, s16 anim)
{
	if (chr->_95DC == 0) {
		chr->_A48 = 0x14;
		chr->_A8C = anim;
	} else {
		chr->_A48 = 0x18;
		chr->_A8C = anim;
	}
}

// the anim archive each room needs
static const char* sRoomAnims[9] = {
	NULL, "cb_robo_6.dat", "cb_robo_7.dat", "cb_robo_8.dat", "cb_robo_9.dat", "cb_robo_10.dat", "cb_robo_11.dat", "cb_robo_12.dat", NULL,
};

// steps the robot from the room it is in to the one it asked for, it stands still meanwhile
void fn_800B31B8(UnkMenuOwner* chr, f32* speed)
{
	if (chr->_95D4 >= 0) {
		switch (chr->_95D8) {
		case 0:
			if (IsTimeUp(chr)) {
				if (chr->mTarget != NULL) {
					chr->mTarget->mFlags &= ~0x0400000000000000ULL;
					chr->mTarget = NULL;
				}
				chr->_A48 = 0;
			}
			switch (chr->_95D0) {
			case 1:
				chr->_C8C = 2;
				break;
			case 2:
				chr->_CC4 = 9;
				fn_800C17BC(&chr->mPopperFx);
				break;
			case 3:
				chr->_943C = 2;
				break;
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				chr->_A48 = 0x13;
				break;
			default:
				if (fn_800D4F0C(&chr->mTimer) || fn_800D4E30(&chr->mTimer) || fn_800D4D54(&chr->mTimer) || fn_800D4C78(&chr->mTimer)) {
					chr->_A48 = 0x13;
				}
				break;
			}
			chr->_95D8 = 10;
		case 10:
			if (chr->_C8C != 0 || chr->_CC4 != 0 || chr->_943C != 0 || chr->_A48 != 0) {
				break;
			}
			if (chr->mAnimSets[0].mLoading == 0) {
				chr->_95D0 = chr->_95D4;
				chr->_95D8 = 15;
				chr->mAnimSets[0].Release();
			}
		case 15: {
			BOOL done;
			if (sRoomAnims[chr->_95D0] != NULL) {
				if (fn_8008A38C(&chr->mAnimSets[0], sRoomAnims[chr->_95D0])) {
					chr->SetAnim(0x11, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					done = TRUE;
				} else {
					done = FALSE;
				}
			} else {
				done = TRUE;
			}
			if (!done) {
				break;
			}
			chr->_95D8 = 20;
		}
		case 20:
			switch (chr->_95D4) {
			case 1:
				chr->_C8C = 1;
				break;
			case 2:
				chr->_CC4 = 1;
				break;
			case 3:
				chr->_943C = 1;
				break;
			case 4:
				EnterRoomAnim(chr, 0x10);
				break;
			case 5:
				EnterRoomAnim(chr, 0x15);
				break;
			case 6:
				EnterRoomAnim(chr, 0x16);
				break;
			case 7:
				EnterRoomAnim(chr, 0x38);
				break;
			case 8:
				chr->_95D0 = 0;
				break;
			}
			chr->_95D8 = 30;
			chr->_95DC = 0;
			break;
		case 30:
			if (chr->_C8C == 0 && chr->_CC4 == 0 && chr->_943C == 0) {
				chr->_95D4 = -1;
				chr->_95D8 = 0;
				chr->_95DC = 0;
				chr->_95E0 = chr->_95E4;
				chr->_95E4 = 0;
			}
			break;
		}
		*speed = 0.0f;
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

BOOL fn_800B3064(UnkLink* link, s32 kind)
{
	return link->mHold->mPoints[link->mHoldId].mKind == kind;
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

bool fn_800B4F28(const CVec* vec, Vec v)
{
	return vec->x != v.x || vec->y != v.y || vec->z != v.z;
}

Vec fn_800B4F68(void* self, HSD_JObj* jobj)
{
	Vec v;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(jobj), &v);
	return v;
}

void fn_800B501C(UnkState* state, s32 value)
{
	state->mState = value;
}

void fn_800B5024(UnkState* state, s32 value)
{
	state->mState = value;
}

void fn_800B502C(UnkState* state, s32 value)
{
	state->mState = value;
}

void fn_800B5034(UnkState* state, s32 value)
{
	state->mState = value;
}

void fn_800B503C(UnkLink* link, s32 value)
{
	link->_24 = value;
}

void fn_800B5044(UnkState* state, s32 value)
{
	state->mState = value;
}

// the disc is being read or retried
static inline u8 IsDiskBusy(const UnkChr* chr)
{
	return (u8)((chr->_331C || chr->_3320) && chr->_3324 == 0);
}

u8 fn_800B517C(UnkChr* chr)
{
	return (u8)((chr->mFlags2 & 1) && !(chr->mFlags2 & 0x22) && !IsDiskBusy(chr));
}

void fn_800B556C(UnkMenuOwner* owner)
{
	owner->RequestRoom(0);
}

void fn_800B504C(UnkBig* game, s32 pos, void* obj, s16 a, s16 b)
{
	if (!IsMuted(game->mChr)) {
		fn_8007EEA4(game->mSound, pos, obj, a, b);
	}
}

static inline void InitSpinAngles(UnkMenuOwner& owner)
{
	for (int i = 0; i < 2; i++) {
		owner.mSpinner.mSpins[i].mAngle.x = DEG2RAD * (rand() % 360);
		owner.mSpinner.mSpins[i].mAngle.y = DEG2RAD * (rand() % 360);
		owner.mSpinner.mSpins[i].mAngle.z = 0.5f;
	}
}

void fn_800B51FC(UnkMenuOwner* chr, s32 a, s32 b)
{
	if (chr->mSpinner.mOn == 0) {
		chr->mSpinner.mOn       = 1;
		chr->mSpinner._0008     = a;
		chr->mSpinner.mRotY     = chr->mSpinner.mOwner->mMoveRotate.y;
		chr->mSpinner.mSelected = b;
		InitSpinAngles(*chr);
		fn_8001E904(GAME_E7E24, 0x86, 0xFF);
		chr->mSpinner.mOwner->SetAnim(0x5D, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		GAME_FLAGS |= 0x800;
	}
}

void fn_800B53A0(UnkMenuOwner* owner, s32 dir)
{
	PairMove(&owner->mPair, dir);
}

void fn_800B558C(UnkMenuOwner* owner);

// opens the two menus, the one picked last gets the cursor
static inline void OpenMenus(UnkMenuOwner* owner)
{
	GAME_FLAGS |= 0x400;
	fn_8001E904(GAME_E7E24, 0x86, 0xFF);
	if (owner->mPair._544 == 0) {
		owner->mPair.mMenu0.vf0C(0);
		owner->mPair.mMenu1.vf0C(1);
	} else {
		owner->mPair.mMenu0.vf0C(1);
		owner->mPair.mMenu1.vf0C(0);
	}
}

void fn_800B54A4(UnkMenuOwner* owner, s32 sel)
{
	if (sel >= 0) {
		owner->mPair._548 = owner->mPair._544;
		owner->mPair._544 = sel;
	}
	if (owner->mPair.IsOpen()) {
		if (owner->mPair._544 == 0) {
			owner->mPair.mMenu0.Select(FALSE);
			owner->mPair.mMenu1.Select(TRUE);
		} else {
			owner->mPair.mMenu0.Select(TRUE);
			owner->mPair.mMenu1.Select(FALSE);
		}
		fn_8001E904(GAME_E7E24, 0x86, 0xFF);
	} else {
		fn_800B558C(owner);
	}
}

void fn_800B558C(UnkMenuOwner* owner)
{
	if (owner->_95D4 != -1) {
		return;
	}
	if (owner->mReq.mBusy != 0) {
		return;
	}
	if (!owner->mPair.IsActive()) {
		StopMove(owner);
		OpenMenus(owner);
	} else if (owner->mPair.IsClosing()) {
		OpenMenus(owner);
	}
}

// the robot is in one of its work poses
static inline bool IsWorking(const UnkMenuOwner* chr)
{
	return chr->_BAC == 1 || chr->_BAC == 2 || chr->_BAC == 3 || chr->_BAC == 4 || chr->_BAC == 5 || chr->_BAC == 6 || chr->_BAC == 7
	    || chr->_BAC == 8;
}
CVec fn_800B3190(f32 x, f32 y, f32 z)
{
	return CVec(x, y, z);
}
}
