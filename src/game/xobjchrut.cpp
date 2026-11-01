#include "game/xchr.h"
#include "game/xobjmap.h"
#include "game/cgame.h"
#include "game/coordinfo.h"
#include <math.h>
extern "C" {
#include <dolphin/mtx/vec.h>
#include "game/globals.h"
}

// out of line copies of the small inlines the player code uses, on XObjChr, its actions
// and the objects around it

extern "C" {
u32 CheckStageFlag(s32 stage, u32 mask);
}

// todo: cleanup
static inline bool TestStatus(const XObj* object, u64 mask)
{
	return (u8)((object->mStatus.mFlags & mask) != 0);
}

/**
 * @note Address: 0x800B2004
 * @note Size: 0x8
 */
extern "C" s32 fn_800B2004(const XObj* object)
{
	return object->_784;
}

/**
 * @note Address: 0x800B200C
 * @note Size: 0x8
 */
extern "C" bool fn_800B200C()
{
	return false;
}

/**
 * @note Address: 0x800B2014
 * @note Size: 0x34
 */
extern "C" bool fn_800B2014(const XObj* object)
{
	return TestStatus(object, 0x0000008000000000ULL);
}

/**
 * @note Address: 0x800B2048
 * @note Size: 0x34
 */
extern "C" bool fn_800B2048(const XObj* object)
{
	return TestStatus(object, 0x0000000100000000ULL);
}

/**
 * @note Address: 0x800B207C
 * @note Size: 0xC
 */
extern "C" void fn_800B207C(XObjChrLook* look)
{
	look->mActive = FALSE;
}

/**
 * @note Address: 0x800B2088
 * @note Size: 0x18
 */
extern "C" bool fn_800B2088(const CObjVec* value)
{
	return value->mSpeed.y > 0.0f;
}

/**
 * @note Address: 0x800B20A0
 * @note Size: 0x14
 */
extern "C" s32 fn_800B20A0(const CGame* game, s32 index)
{
	return game->GetPools()[index].mUsed;
}

/**
 * @note Address: 0x800B20B4
 * @note Size: 0x10
 */
extern "C" UnkStageObj* fn_800B20B4(const CoordInfo* info, s32 index)
{
	return info->_00 + index;
}

/**
 * @note Address: 0x800B20C4
 * @note Size: 0x8
 */
extern "C" CoordInfo* fn_800B20C4(XObj* object)
{
	return &object->mCoord;
}

/**
 * @note Address: 0x800B20CC
 * @note Size: 0x3C
 */
extern "C" f32 fn_800B20CC(const Vec* from, const Vec* to)
{
	return atan2(to->x - from->x, to->z - from->z);
}

/**
 * @note Address: 0x800B2108
 * @note Size: 0x20
 */
extern "C" void* fn_800B2108(const CGame* game, s32 pool, s32 index)
{
	const XObjPool* entry = &game->GetPools()[pool];
	return entry->mObjects + index * entry->mStride;
}

/**
 * @note Address: 0x800B2128
 * @note Size: 0xC
 */
extern "C" s32 fn_800B2128(const CGame* game)
{
	return game->mObjectCount;
}

/**
 * @note Address: 0x800B2134
 * @note Size: 0x8
 */
extern "C" f32 fn_800B2134(const XObj* object)
{
	return object->_780;
}

/**
 * @note Address: 0x800B213C
 * @note Size: 0x8
 */
extern "C" void fn_800B213C(XAction* action, s32 state)
{
	action->mState = state;
}

/**
 * @note Address: 0x800B2144
 * @note Size: 0x20
 */
extern "C" void fn_800B2144(XObjChr* chr, s32 state, XObj* target)
{
	chr->mRope.SetState(state, target);
}

/**
 * @note Address: 0x800B2164
 * @note Size: 0x34
 */
extern "C" bool fn_800B2164(const XObj* object)
{
	return TestStatus(object, 0x0000001000000000ULL);
}

/**
 * @note Address: 0x800B2198
 * @note Size: 0x20
 */
extern "C" void fn_800B2198(XObjChr* chr, s32 state, XObj* target)
{
	chr->mLadder.SetState(state, target);
}

/**
 * @note Address: 0x800B21B8
 * @note Size: 0x34
 */
extern "C" bool fn_800B21B8(const XObj* object)
{
	return TestStatus(object, 0x0000000800000000ULL);
}

/**
 * @note Address: 0x800B21EC
 * @note Size: 0x44
 */
extern "C" void fn_800B21EC(XObjChr* chr)
{
	XObj* target = chr->mLift.mTarget;
	if (target != NULL) {
		target->mStatus.mFlags &= ~0x0400000000000000ULL;
		chr->mLift.mTarget = NULL;
	}
	chr->mLift.mState = 0;
}

/**
 * @note Address: 0x800B2230
 * @note Size: 0x20
 */
extern "C" void fn_800B2230(XObjChr* chr, s32 state, XObj* target)
{
	chr->mHang.SetState(state, target);
}

/**
 * @note Address: 0x800B2250
 * @note Size: 0x34
 */
extern "C" bool fn_800B2250(const XObj* object)
{
	return TestStatus(object, 0x0000000200000000ULL);
}

/**
 * @note Address: 0x800B2284
 * @note Size: 0x10
 */
extern "C" s32 fn_800B2284(const XObj* object, s32 index)
{
	const s32* entries = &object->_25C;
	return entries[index];
}

/**
 * @note Address: 0x800B2294
 * @note Size: 0x34
 */
extern "C" bool fn_800B2294(const XObj* object)
{
	return TestStatus(object, 0x0000002000000000ULL);
}

/**
 * @note Address: 0x800B22C8
 * @note Size: 0x8
 */
extern "C" void fn_800B22C8(XAction* action, s32 state)
{
	action->mState = state;
}

/**
 * @note Address: 0x800B22D0
 * @note Size: 0x28
 */
extern "C" u8 fn_800B22D0(const XObjChrMenu* menu)
{
	return menu->IsOpen();
}

/**
 * @note Address: 0x800B22F8
 * @note Size: 0x48
 */
extern "C" f32 fn_800B22F8()
{
	return (s32)fn_8006EAF0(2) / 100.0f;
}

/**
 * @note Address: 0x800B2340
 * @note Size: 0x48
 */
extern "C" f32 fn_800B2340()
{
	return (s32)fn_8006EAF0(1) / 100.0f;
}

/**
 * @note Address: 0x800B2388
 * @note Size: 0x8
 */
extern "C" void fn_800B2388(CObjVec* value, f32 y)
{
	value->mTarget.y = y;
}

/**
 * @note Address: 0x800B2390
 * @note Size: 0x8
 */
extern "C" bool fn_800B2390()
{
	return true;
}

/**
 * @note Address: 0x800B2398
 * @note Size: 0xE4
 */
extern "C" f32 fn_800B2398(f32 value)
{
	return sqrtf(value);
}

/**
 * @note Address: 0x800B247C
 * @note Size: 0x40
 */
extern "C" f32 fn_800B247C(XObjFloor* const* floors, s32 index)
{
	return DEG2RAD * (*floors)[index]._10;
}

/**
 * @note Address: 0x800B24BC
 * @note Size: 0x8
 */
extern "C" s32 fn_800B24BC(const XAction* action)
{
	return action->mTargetFloor;
}

/**
 * @note Address: 0x800B24C4
 * @note Size: 0x8
 */
extern "C" XObjFloor** fn_800B24C4(XObj* object)
{
	return &object->mFloors;
}

/**
 * @note Address: 0x800B24CC
 * @note Size: 0x8
 */
extern "C" CObjVec* fn_800B24CC(CXObj* object)
{
	return &object->mMoveScale;
}

/**
 * @note Address: 0x800B24D4
 * @note Size: 0x8
 */
extern "C" CObjVec* fn_800B24D4(CXObj* object)
{
	return &object->mMoveRotate;
}

/**
 * @note Address: 0x800B24DC
 * @note Size: 0x28
 */
extern "C" s32 fn_800B24DC(const XObj* object)
{
	return object->GetPool();
}

/**
 * @note Address: 0x800B2504
 * @note Size: 0x14
 */
extern "C" s32 fn_800B2504(const CGame* game, s32 list)
{
	return game->mDrawLists[list].mCount;
}

/**
 * @note Address: 0x800B2518
 * @note Size: 0x24
 */
extern "C" void fn_800B2518(XObj* object, u64 mask)
{
	object->mStatus.mFlags &= ~mask;
}

/**
 * @note Address: 0x800B253C
 * @note Size: 0xC
 */
extern "C" u32 fn_800B253C(const XObjChr* chr, u32 mask)
{
	return chr->_980.mFlags & mask;
}

/**
 * @note Address: 0x800B2548
 * @note Size: 0x1C
 */
extern "C" s32 fn_800B2548(const CGame* game, s32 list, s32 index)
{
	return game->mDrawLists[list].mIndices[index];
}

/**
 * @note Address: 0x800B2564
 * @note Size: 0x1C
 */
extern "C" void fn_800B2564(XObj* object, u64 mask)
{
	object->mStatus.mFlags |= mask;
}

/**
 * @note Address: 0x800B2580
 * @note Size: 0x14
 */
extern "C" f32 fn_800B2580(const XObj* object)
{
	return object->_6FC[object->_274];
}

/**
 * @note Address: 0x800B2594
 * @note Size: 0x8
 */
extern "C" f32 fn_800B2594(const XObj* object)
{
	return object->_6F8;
}

/**
 * @note Address: 0x800B259C
 * @note Size: 0x104
 */
extern "C" f32 fn_800B259C(const Vec* a, const Vec* b)
{
	f32 z = a->z - b->z;
	f32 x = a->x - b->x;
	return sqrtf(x * x + z * z);
}

/**
 * @note Address: 0x800B26A0
 * @note Size: 0x14
 */
extern "C" bool fn_800B26A0(const XAction* action, u32 wanted)
{
	return (u8)(wanted == action->_0C);
}

/**
 * @note Address: 0x800B26B4
 * @note Size: 0x8
 */
extern "C" u32 fn_800B26B4(const CXObj* object)
{
	return object->mId;
}

/**
 * @note Address: 0x800B26BC
 * @note Size: 0x14
 */
extern "C" u64 fn_800B26BC(const XObj* object, u64 mask)
{
	return object->mStatus.mFlags & mask;
}

/**
 * @note Address: 0x800B26D0
 * @note Size: 0x1C
 */
extern "C" XObj* fn_800B26D0(const CGame* game, s32 list, s32 index)
{
	return game->mDrawLists[list].mObjects[index];
}

/**
 * @note Address: 0x800B26EC
 * @note Size: 0x78
 */
extern "C" void fn_800B26EC(CJObj*, HSD_JObj* jobj, Vec* scale)
{
	HSD_JObjGetScale(jobj, scale);
}

/**
 * @note Address: 0x800B2764
 * @note Size: 0x7C
 */
extern "C" HSD_JObj* fn_800B2764(const CJObj* object, s32 depth, s32* path)
{
	return GetJointByPath(object->mJObj, path, depth);
}

/**
 * @note Address: 0x800B27E0
 * @note Size: 0xBC
 */
extern "C" Vec fn_800B27E0(XObjFloor* const* floors, s32 index)
{
	Vec value = lbl_8037827C;
	value.x   = (*floors)[index]._04[0] / 100.0f;
	value.y   = (*floors)[index]._04[1] / 100.0f;
	value.z   = (*floors)[index]._04[2] / 100.0f;
	return value;
}

/**
 * @note Address: 0x800B289C
 * @note Size: 0xBC
 */
extern "C" Vec fn_800B289C(XObjFloor* const* floors, s32 index)
{
	Vec value = lbl_80378288;
	value.x   = (*floors)[index]._14[0] / 100.0f;
	value.y   = (*floors)[index]._14[1] / 100.0f;
	value.z   = (*floors)[index]._14[2] / 100.0f;
	return value;
}

/**
 * @note Address: 0x800B2958
 * @note Size: 0x1C
 */
extern "C" void fn_800B2958(CObjVec* value, const Vec& target)
{
	value->mTarget.Set(target);
}

/**
 * @note Address: 0x800B2974
 * @note Size: 0x10
 */
extern "C" void fn_800B2974(CVec* value, f32 x, f32 y, f32 z)
{
	value->Set(x, y, z);
}

/**
 * @note Address: 0x800B2984
 * @note Size: 0x10
 */
extern "C" void fn_800B2984(CObjVec* value, f32 x, f32 y, f32 z)
{
	value->mSpeed.Set(x, y, z);
}

/**
 * @note Address: 0x800B2994
 * @note Size: 0x24
 */
extern "C" void fn_800B2994(Vec* value, f32 scale)
{
	PSVECScale(value, value, scale);
}

/**
 * @note Address: 0x800B29B8
 * @note Size: 0x8
 */
extern "C" bool fn_800B29B8()
{
	return true;
}

/**
 * @note Address: 0x800B29C0
 * @note Size: 0x8
 */
extern "C" bool fn_800B29C0()
{
	return true;
}

/**
 * @note Address: 0x800B29C8
 * @note Size: 0x2C
 */
extern "C" u8 fn_800B29C8(const XActCopter* act)
{
	return act->mState == 0 || act->mState == 6 || act->mState == 9;
}

/**
 * @note Address: 0x800B29F4
 * @note Size: 0x8
 */
extern "C" bool fn_800B29F4()
{
	return true;
}

/**
 * @note Address: 0x800B29FC
 * @note Size: 0x8
 */
extern "C" bool fn_800B29FC()
{
	return true;
}

/**
 * @note Address: 0x800B2A04
 * @note Size: 0x24
 */
extern "C" u8 fn_800B2A04(const XActRope* act)
{
	return act->mState == 0 || act->mState == 10;
}

/**
 * @note Address: 0x800B2A28
 * @note Size: 0x24
 */
extern "C" u8 fn_800B2A28(const XActLadder* act)
{
	return act->mState == 0 || act->mState == 11;
}

/**
 * @note Address: 0x800B2A4C
 * @note Size: 0x24
 */
extern "C" u8 fn_800B2A4C(const XActJump* act)
{
	return act->mState == 0 || act->mState == 4;
}

/**
 * @note Address: 0x800B2A70
 * @note Size: 0x10
 */
extern "C" bool fn_800B2A70(const XActHang* act)
{
	return (u8)(act->mState == 0);
}

/**
 * @note Address: 0x800B2A80
 * @note Size: 0x8
 */
extern "C" bool fn_800B2A80()
{
	return true;
}

/**
 * @note Address: 0x800B2A88
 * @note Size: 0x34
 */
extern "C" CVec* fn_800B2A88(CVec* value)
{
	PSVECNormalize(value, value);
	return value;
}

struct XObjMapHit {
	s32 mTriangle; // _00
	s32 mTrigon;   // _04
	f32 mHeight;   // _08
};

/**
 * @note Address: 0x800B2ABC
 * @note Size: 0xF4
 */
extern "C" bool fn_800B2ABC(XObjMap* map, Vec* pos, XObjMapHit* hit, Vec* tri)
{
	f32 limit = 0.2f + pos->y;
	s32 start;
	for (start = 0; start < map->mTrigonCount; ++start) {
		if (map->mTrigon[map->mTrigonOrder[start]].mMinY <= limit)
			goto found;
	}
	start = 0;
found:
	s32 count = map->mTrigonCount;
	for (s32 i = 0; i < count; ++i) {
		s32 index    = map->mTrigonOrder[(start + i) % count];
		s32 triangle = map->mTrigon[index].Check(pos, &hit->mHeight, tri);
		if (triangle >= 0) {
			hit->mTrigon   = index;
			hit->mTriangle = triangle;
			return true;
		}
	}
	return false;
}

/**
 * @note Address: 0x800B2BB0
 * @note Size: 0x20
 */
extern "C" void fn_800B2BB0(XObjChr* chr, s32 state, BOOL restart)
{
	chr->mTumble.mState = state;
	if (restart) {
		chr->mTumble.mFade.mActive = TRUE;
		chr->mTumble.mFade.mAmount = 0.0f;
	}
}

extern "C" s32 lbl_8065B260;

/**
 * @note Address: 0x800B2BD0
 * @note Size: 0x12C
 */
extern "C" void fn_800B2BD0(XObjPlug* plug, BOOL resetJoint)
{
	plug->_D3C = 0;
	if (resetJoint) {
		s32 path       = lbl_8065B260;
		HSD_JObj* jobj = GetJointByPath(plug->mJObj, &path, 1);
		HSD_JObjSetRotationX(jobj, 0.0f);
	}
}

/**
 * @note Address: 0x800B2CFC
 * @note Size: 0xC
 */
extern "C" XSound* fn_800B2CFC(CGame* game)
{
	return &game->mSound;
}

/**
 * @note Address: 0x800B2D08
 * @note Size: 0x24
 */
extern "C" u32 fn_800B2D08(const CGame* game, u32 mask)
{
	return CheckStageFlag(game->mStage, mask);
}

/**
 * @note Address: 0x800B2D2C
 * @note Size: 0x14
 */
extern "C" bool fn_800B2D2C(const XActRadar* act, s32 wanted)
{
	return (u8)(wanted == act->mState);
}

/**
 * @note Address: 0x800B2D40
 * @note Size: 0x14
 */
extern "C" bool fn_800B2D40(const XActPopper* act, s32 wanted)
{
	return (u8)(wanted == act->mState);
}

/**
 * @note Address: 0x800B2D54
 * @note Size: 0x8
 */
extern "C" CVec* fn_800B2D54(XObjPlug* plug)
{
	return &plug->mCord._6C;
}

/**
 * @note Address: 0x800B2D5C
 * @note Size: 0x8
 */
extern "C" bool fn_800B2D5C()
{
	return true;
}

/**
 * @note Address: 0x800B2D64
 * @note Size: 0x8
 */
extern "C" bool fn_800B2D64()
{
	return true;
}

/**
 * @note Address: 0x800B2D6C
 * @note Size: 0x54
 */
extern "C" u8 fn_800B2D6C(const XActCopter* act)
{
	s32 state = act->mState;
	return (state == 0 || state == 3 || state == 4 || state == 7 || state == 8 || state == 5 || state == 6 || state == 9);
}

/**
 * @note Address: 0x800B2DC0
 * @note Size: 0x24
 */
extern "C" u8 fn_800B2DC0(const XActTumble* act)
{
	return (act->mState != 8 && act->mState != 9);
}

/**
 * @note Address: 0x800B2DE4
 * @note Size: 0x34
 */
extern "C" u8 fn_800B2DE4(const XActRope* act)
{
	s32 state = act->mState;
	return (state == 0 || state == 9 || state == 10 || state == 11);
}

/**
 * @note Address: 0x800B2E18
 * @note Size: 0x2C
 */
extern "C" u8 fn_800B2E18(const XActLadder* act)
{
	s32 state = act->mState;
	return (state == 0 || state == 10 || state == 11);
}

/**
 * @note Address: 0x800B2E44
 * @note Size: 0x3C
 */
extern "C" u8 fn_800B2E44(const XActJump* act)
{
	s32 state = act->mState;
	return (state == 0 || state == 2 || state == 3 || state == 4 || state == 17);
}

/**
 * @note Address: 0x800B2E80
 * @note Size: 0x24
 */
extern "C" u8 fn_800B2E80(const XActHang* act)
{
	return (act->mState == 0 || act->mState == 10);
}

/**
 * @note Address: 0x800B2EA4
 * @note Size: 0xC
 */
extern "C" s32 fn_800B2EA4(const CGame* game)
{
	return game->_8580;
}

/**
 * @note Address: 0x800B2EB0
 * @note Size: 0x8
 */
extern "C" u32 fn_800B2EB0(const XObjPlug* plug)
{
	return plug->_D3C;
}

/**
 * @note Address: 0x800B2EB8
 * @note Size: 0x18
 */
extern "C" void* fn_800B2EB8(const CGame* game, s32 index)
{
	s32 stride  = game->mPools[0].mStride;
	u8* entries = game->mPools[0].mObjects;
	return entries + index * stride;
}

/**
 * @note Address: 0x800B2ED0
 * @note Size: 0x8
 */
extern "C" void fn_800B2ED0(CObjVec* value, f32 y)
{
	value->mSpeed.y = y;
}

/**
 * @note Address: 0x800B2ED8
 * @note Size: 0x28
 */
extern "C" void fn_800B2ED8(CObjVec* value, const Vec& v)
{
	f32 x = v.x, y = v.y, z = v.z;
	value->x         = x;
	value->y         = y;
	value->z         = z;
	value->mTarget.x = x;
	value->mTarget.y = y;
	value->mTarget.z = z;
}

/**
 * @note Address: 0x800B2F00
 * @note Size: 0xC
 */
extern "C" u32 fn_800B2F00(const CPadOne* pad, u32 mask)
{
	return mask & pad->mButton;
}

/**
 * @note Address: 0x800B2F0C
 * @note Size: 0x14
 */
extern "C" CPadOne* fn_800B2F0C(CGame* game, u8 port)
{
	return (CPadOne*)game->mPadOne[port];
}

/**
 * @note Address: 0x800B2F20
 * @note Size: 0xC
 */
extern "C" void fn_800B2F20(CObjVec* value, f32 z)
{
	value->z         = z;
	value->mTarget.z = z;
}

/**
 * @note Address: 0x800B2F2C
 * @note Size: 0xC
 */
extern "C" void fn_800B2F2C(CObjVec* value, f32 x)
{
	value->x         = x;
	value->mTarget.x = x;
}

/**
 * @note Address: 0x800B2F38
 * @note Size: 0x14
 */
extern "C" bool fn_800B2F38(const XActLift* act, s32 wanted)
{
	return (u8)(wanted == act->mState);
}

/**
 * @note Address: 0x800B2F4C
 * @note Size: 0x28
 */
extern "C" bool fn_800B2F4C(const XObjChr* chr, s32 wanted)
{
	return (u8)(wanted == chr->GetAttachment());
}

/**
 * @note Address: 0x800B2F74
 * @note Size: 0x18
 */
extern "C" bool fn_800B2F74(const CGame* game)
{
	return game->_5D080 != NULL;
}

/**
 * @note Address: 0x800B2F8C
 * @note Size: 0x14
 */
extern "C" bool fn_800B2F8C(const XActTumble* act, s32 wanted)
{
	return (u8)(wanted == act->mState);
}

/**
 * @note Address: 0x800B2FA0
 * @note Size: 0x20
 */
extern "C" f32 fn_800B2FA0(const Vec* a, const Vec* b)
{
	return PSVECDistance((Vec*)a, (Vec*)b);
}

/**
 * @note Address: 0x800B2FC0
 * @note Size: 0x4
 */
extern "C" void fn_800B2FC0()
{
}

/**
 * @note Address: 0x800B2FC4
 * @note Size: 0x14
 */
extern "C" u64 fn_800B2FC4(const CGame* game, u64 mask)
{
	return mask & game->mStatus.mFlags;
}

/**
 * @note Address: 0x800B2FD8
 * @note Size: 0xC
 */
extern "C" void fn_800B2FD8(CObjVec* value, f32 y)
{
	value->y         = y;
	value->mTarget.y = y;
}

/**
 * @note Address: 0x800B2FE4
 * @note Size: 0x8
 */
extern "C" s32 fn_800B2FE4(const CXObj* object)
{
	return object->mAnimId;
}

/**
 * @note Address: 0x800B2FEC
 * @note Size: 0x1C
 */
extern "C" void fn_800B2FEC(CObjVec* value, f32 x, f32 y, f32 z)
{
	value->x         = x;
	value->y         = y;
	value->z         = z;
	value->mTarget.x = x;
	value->mTarget.y = y;
	value->mTarget.z = z;
}

/**
 * @note Address: 0x800B3008
 * @note Size: 0x8
 */
extern "C" void fn_800B3008(CVec* value, f32 z)
{
	value->z = z;
}

/**
 * @note Address: 0x800B3010
 * @note Size: 0x8
 */
extern "C" void fn_800B3010(CVec* value, f32 x)
{
	value->x = x;
}

/**
 * @note Address: 0x800B3018
 * @note Size: 0x34
 */
extern "C" f32 fn_800B3018(f32 angle)
{
	angle = WrapAngle(angle);
	return angle;
}

/**
 * @note Address: 0x800B304C
 * @note Size: 0x8
 */
extern "C" u32 fn_800B304C(const XAction* action)
{
	return action->_0C;
}

/**
 * @note Address: 0x800B3054
 * @note Size: 0x8
 */
extern "C" f32 fn_800B3054(const XActHang* act)
{
	return act->_48;
}

/**
 * @note Address: 0x800B305C
 * @note Size: 0x8
 */
extern "C" Vec* fn_800B305C(XActHang* act)
{
	return &act->_3C;
}

/**
 * @note Address: 0x800B3064
 * @note Size: 0x24
 */
extern "C" bool fn_800B3064(const XAction* action, s32 wanted)
{
	return (u8)(action->mTarget->mFloors[action->mTargetFloor].mId == wanted);
}

/**
 * @note Address: 0x800B3088
 * @note Size: 0x8
 */
extern "C" void fn_800B3088(CObjVec* value, f32 z)
{
	value->mSpeed.z = z;
}

/**
 * @note Address: 0x800B3090
 * @note Size: 0x8
 */
extern "C" void fn_800B3090(CObjVec* value, f32 x)
{
	value->mSpeed.x = x;
}

/**
 * @note Address: 0x800B3098
 * @note Size: 0x8
 */
extern "C" CVec* fn_800B3098(CObjVec* value)
{
	return &value->mSpeed;
}

/**
 * @note Address: 0x800B30A0
 * @note Size: 0x20
 */
extern "C" f32 fn_800B30A0(const Vec* value)
{
	return PSVECMag((Vec*)value);
}

/**
 * @note Address: 0x800B30C0
 * @note Size: 0x8
 */
extern "C" CVec* fn_800B30C0(CObjVec* value)
{
	return &value->mTarget;
}

/**
 * @note Address: 0x800B30C8
 * @note Size: 0x8
 */
extern "C" void fn_800B30C8(CObjVec* value, f32 z)
{
	value->mTarget.z = z;
}

/**
 * @note Address: 0x800B30D0
 * @note Size: 0x8
 */
extern "C" void fn_800B30D0(CObjVec* value, f32 x)
{
	value->mTarget.x = x;
}

/**
 * @note Address: 0x800B30D8
 * @note Size: 0x24
 */
extern "C" f32 fn_800B30D8(f32 angle)
{
	return cos(angle);
}

/**
 * @note Address: 0x800B30FC
 * @note Size: 0x8
 */
extern "C" f32 fn_800B30FC(const XObjChr* chr)
{
	return chr->_A40;
}

/**
 * @note Address: 0x800B3104
 * @note Size: 0x24
 */
extern "C" f32 fn_800B3104(f32 angle)
{
	return sin(angle);
}

/**
 * @note Address: 0x800B3128
 * @note Size: 0x8
 */
extern "C" XObj* fn_800B3128(const XAction* action)
{
	return action->mTarget;
}

/**
 * @note Address: 0x800B3130
 * @note Size: 0x14
 */
extern "C" bool fn_800B3130(const XActHang* act, s32 wanted)
{
	return (u8)(wanted == act->mState);
}

/**
 * @note Address: 0x800B3144
 * @note Size: 0x8
 */
extern "C" CVec* fn_800B3144(XObj* object)
{
	return &object->_264;
}

/**
 * @note Address: 0x800B314C
 * @note Size: 0x8
 */
extern "C" CObjVec* fn_800B314C(CXObj* object)
{
	return &object->mMoveTranslate;
}

/**
 * @note Address: 0x800B3154
 * @note Size: 0x14
 */
extern "C" XObj* fn_800B3154(const CGame* game, s32 index)
{
	return game->mObjects[index];
}

/**
 * @note Address: 0x800B3168
 * @note Size: 0x14
 */
extern "C" bool fn_800B3168(const XAction* action)
{
	return action->mTarget != NULL;
}

/**
 * @note Address: 0x800B317C
 * @note Size: 0x14
 */
extern "C" bool fn_800B317C(const XActPush* act, s32 wanted)
{
	return (u8)(wanted == act->mState);
}

/**
 * @note Address: 0x800B3190
 * @note Size: 0x1C
 */
extern "C" CVec fn_800B3190(f32 x, f32 y, f32 z)
{
	return CVec(x, y, z);
}

/**
 * @note Address: 0x800B31AC
 * @note Size: 0xC
 */
extern "C" u32 fn_800B31AC(const XObjChr* chr, u32 mask)
{
	return mask & chr->_980.mFlags;
}
