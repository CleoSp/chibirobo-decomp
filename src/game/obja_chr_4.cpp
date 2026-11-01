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

// the stick or the dpad moves the cursor, a picks, b closes. 1 picked, -1 closed
// NONMATCHING: the dol loads the turn table into r3 and the robot into r4, here they swap
s32 fn_8009B9E0(OctaMenu* menu, BOOL active)
{
	if (menu->mOpen > 0 && menu->mSelected == 0) {
		CPad pad(0);
		s32 cursor;
		if (menu->_280 == 3) {
			u32 dir = active ? GetPad(pad)->mDirRepeat & 0xF : 0;
			if (dir & 1) {
				if (--menu->_288 < 0) {
					menu->_288 = 6;
				}
				fn_8001E904(GAME_E7E24, 0x87, 0xFF);
			} else if (dir & 2) {
				if (++menu->_288 >= 7) {
					menu->_288 = 0;
				}
				fn_8001E904(GAME_E7E24, 0x87, 0xFF);
			}
			if (menu->mSelected != 0) {
				cursor = menu->mCursor = -1;
			} else {
				cursor = menu->mCursor = (menu->_288 + 3) % 7;
			}
		} else {
			u32 stick = active ? GetPad(pad)->mButton & 0xF0000 : 0;
			if (stick == 0x50000) {
				cursor = 0;
			} else if (stick == 0x10000) {
				cursor = 1;
			} else if (stick == 0x90000) {
				cursor = 2;
			} else if (stick == 0x80000) {
				cursor = 3;
			} else if (stick == 0xA0000) {
				cursor = 4;
			} else if (stick == 0x20000) {
				cursor = 5;
			} else if (stick == 0x60000) {
				cursor = 6;
			} else if (stick == 0x40000) {
				cursor = 7;
			} else {
				cursor = menu->mCursor;
			}
		}
		if (menu->_280 != 3) {
			LookTo(&menu->mOwner->mJointRots[0], DEG2RAD * sMenuTurnY[cursor + 1]);
			if (fpclassify(menu->mHeadTurn) == FP_NAN) {
				LookTo(&menu->mOwner->mJointRots[1], DEG2RAD * sMenuTurnX[cursor + 1] + -1.52f);
			} else {
				LookTo(&menu->mOwner->mJointRots[1], menu->mHeadTurn);
			}
		}
		if (menu->mCursor != cursor) {
			fn_8001E904(GAME_E7E24, 0x87, 0xFF);
		}
		menu->mCursor = cursor;
		if (active && menu->mFade >= 1.0f) {
			if (GetPad(pad)->mTrigger & 0x100) {
				if (menu->mCursor < 0) {
					fn_8001E904(GAME_E7E24, 0x85, 0xFF);
				} else if (menu->mEntries[menu->mCursor].mCount == 0) {
					fn_8001E904(GAME_E7E24, 0x85, 0xFF);
				} else {
					menu->vf1C();
					StopShots(menu->mOwner);
					return 1;
				}
			} else if (GetPad(pad)->mTrigger & 0x200) {
				return -1;
			}
		}
	}
	return 0;
}

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

// the disc is being read or retried
static inline u8 IsDiskBusy(const UnkChr* chr)
{
	return (u8)((chr->_331C || chr->_3320) && chr->_3324 == 0);
}

static inline void InitSpinAngles(UnkMenuOwner& owner)
{
	for (int i = 0; i < 2; i++) {
		owner.mSpinner.mSpins[i].mAngle.x = DEG2RAD * (rand() % 360);
		owner.mSpinner.mSpins[i].mAngle.y = DEG2RAD * (rand() % 360);
		owner.mSpinner.mSpins[i].mAngle.z = 0.5f;
	}
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
void fn_800B558C(UnkMenuOwner* owner);

// the robot is in one of its work poses
static inline bool IsWorking(const UnkMenuOwner* chr)
{
	return chr->_BAC == 1 || chr->_BAC == 2 || chr->_BAC == 3 || chr->_BAC == 4 || chr->_BAC == 5 || chr->_BAC == 6 || chr->_BAC == 7
	    || chr->_BAC == 8;
}
}
