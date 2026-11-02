#include "game/xobjactor.h"
#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/cpad.h"
#include "game/cscrn.h"
#include "game/xchr.h"
#include "game/infoscrn.h"
#include "game/stagedata.h"
#include "game/ut.h"
#include "game/gmath.h"
#include <sysdolphin/rumble.h>
#include <stdlib.h>

// XObjActor3, the actor most of the stage's small moving things are. which one it is comes
// from the stage table, see GetKind and GetObjFlags

extern "C" {
BOOL HSD_AudioSFXCheck(s32 id);
void HSD_AudioSFXKeyOff(s32 id);
u16 fn_8006E44C(s16 item, s32 kind);
f32 fn_8006697C(f32 x);
u8 fn_800EBDB0(XObj* obj);
BOOL fn_800A0000(XObj* obj);
void fn_80068BAC(void* mgr);
void fn_80068BC0(void* mgr);
void fn_80068BD4(void* mgr);
void fn_80068BE8(void* mgr);
void fn_80068BFC(void* mgr);
void fn_80068C10(void* mgr);
CVec fn_800621E0(XObj* obj);
void HSD_MtxGetScale(Mtx mat, Vec* vec);
void PSMTXRotRad(Mtx m, char axis, f32 rad);
u16 GXGetTexObjWidth(const GXTexObj* obj);
u16 GXGetTexObjHeight(const GXTexObj* obj);
void OSReport(const char* fmt, ...);
void fn_8010904C(XObjActor3* actor);
void fn_801095E8(XObjActor3* actor);
void fn_8010AD88(XObjActor3* actor);
void fn_8010FB1C(XObjActor3* actor);
void fn_8010B394(XObjActor3* actor);
void fn_8010B528(XObjActor3* actor);
void fn_8010AF74(XObjActor3* actor);
void fn_8010B7B4(XObjActor3* actor, u32 frames);

extern s32 lbl_80367A2C[];          // the script's variables
extern AnimSfxEvent lbl_8022C178[]; // haisui_nezumi's anim sound events

s32 lbl_8065B2F0;
s32 lbl_8065B2F4;
}

// the frame the jobj's first anim is at, and the frame it ends at
static inline f32 GetAnimFrame(HSD_JObj* jobj)
{
	HSD_AObj* aobj = GetAObj(jobj);
	return aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
}

static inline f32 GetAnimEndFrame(HSD_JObj* jobj)
{
	HSD_AObj* aobj = GetAObj(jobj);
	return aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
}

// the robot is out of reach, past the front edge or in one of its climbing anims
static inline bool IsPlayerAway(XObjChr* player, Vec pos)
{
	if (pos.z < -4.28f) {
		return true;
	}
	switch (player->mAnimId) {
	case 0x66:
	case 0x67:
	case 0x68:
	case 0x69:
	case 0x6A:
	case 0x6B:
	case 0x6C:
	case 0x6D:
	case 0x6E:
	case 0x6F:
	case 0x70:
	case 0x71:
	case 0x72:
	case 0x73:
		return true;
	}
	return false;
}

// the anims the robot reaches further in
static inline bool IsLongReachAnim(s32 anim)
{
	switch (anim) {
	case 0x4:
	case 0xD:
	case 0x35:
	case 0x79:
	case 0x89:
	case 0x10D:
	case 0x819:
	case 0x898:
		return true;
	}
	return false;
}

// the robot's anims it can't be talked to in
static inline bool IsBusyAnim(s32 anim)
{
	switch (anim) {
	case 0x5:
	case 0x6:
	case 0x7:
	case 0x8:
	case 0x9:
	case 0xA:
	case 0xB:
	case 0x27:
	case 0x28:
	case 0x35:
	case 0x36:
	case 0x37:
	case 0x43:
	case 0x97:
	case 0xA1:
	case 0xED:
	case 0xEE:
	case 0xF6:
	case 0x814:
	case 0x816:
	case 0x817:
	case 0x81F:
	case 0x820:
	case 0x824:
	case 0x879:
	case 0x87A:
	case 0x8AE:
	case 0x8AF:
	case 0x8B7:
	case 0x8C0:
	case 0x8C1:
	case 0x8C2:
	case 0x8C3:
	case 0x8C4:
	case 0x8C5:
		return true;
	}
	return false;
}

// the stage table entry the actor was placed from, the positions in hundredths, the angles in degrees
struct UnkActor3Place {
	s32 mKind;   // _00
	s32 mPos[3]; // _04
	s32 mRot[3]; // _10
};

// hides the items of the kind, the 0x40B ones only
static inline void HideItems(s32 kind)
{
	if (kind >= 0) {
		switch (kind) {
		case 0x40B:
			for (s32 i = 0; i < 0x120; i++) {
				XObj* obj = lbl_8025C400.mXObjs.mPools[22].Get(i);
				if (kind == obj->GetKind()) {
					obj->mStatus.mFlags &= ~0x10000ULL;
				}
			}
			break;
		}
	}
}

// a texture's size on screen at the scale
static inline f32 TexSize(f32 size, f32 scale)
{
	return scale * size;
}

static inline bool IsStage(const char* name)
{
	s32 stage = lbl_8025C400.mStage;
	s32 id    = fn_8006F654(name);
	return id == stage;
}

// the event's object with the id, ids from 10000 up
static inline XObj* FindEvtObj(s16 id)
{
	return lbl_8025C400.mEvt.mData.mScript != NULL ? fn_8007F570(&lbl_8025C400.mEvt.mData, id) : NULL;
}

static inline XObj* GetObj(s16 id)
{
	return lbl_8025C400.mXObjs.mObjects[id];
}

// where the stage table placed the actor
static inline Vec GetPlacePos(const XObjActor3Place* place)
{
	Vec pos = Vec();
	pos.x   = place->mPos[0] / 100.0f;
	pos.y   = place->mPos[1] / 100.0f;
	pos.z   = place->mPos[2] / 100.0f;
	return pos;
}

/**
 * @note Address: 0x8011529C
 * @note Size: 0x4
 */
void XObjActor3::Kind0(u32 frames)
{
}

/**
 * @note Address: 0x801151E0
 * @note Size: 0xBC
 */
void XObjActor3::Kind1(u32 frames)
{
	switch (_B94) {
	case 0:
		break;
	case 1:
		fn_800661E4(&lbl_8025C400.mInfoScrn, lbl_8025C400.mXObjs.mPools[5].Get(_258), 1, 1);
		_B94 = 0;
		break;
	case 2:
		fn_800661E4(&lbl_8025C400.mInfoScrn, lbl_8025C400.mXObjs.mPools[5].Get(_258), 0, 1);
		_B94 = 0;
		break;
	}
}

/**
 * @note Address: 0x801146F8
 * @note Size: 0xC8
 */
void XObjActor3::Kind4(u32 frames)
{
	_B88++;
	f32 s     = sin(0.017453292f * ((_B88 * 6) % 360));
	f32 swing = 0.25f;
	if (GetKind() == 0x437) {
		mMoveRotate.x = s * swing;
	} else {
		mMoveRotate.z = s * swing;
	}
}

/**
 * @note Address: 0x801146F4
 * @note Size: 0x4
 */
void XObjActor3::Kind5(u32 frames)
{
}

/**
 * @note Address: 0x8011463C
 * @note Size: 0xB8
 */
// NONMATCHING: register allocation
void XObjActor3::Kind6(u32 frames)
{
	s16 id;
	s16 num;
	s32 i;
	s32 count = 0;
	switch (_B94) {
	case 0:
		break;
	case 1:
		for (i = 0; i < 0x100; i++) {
			InventoryItem* items = lbl_8025C400.GetPlayer()->mParams.mInventory.mItems;
			id                   = items[i].mId;
			num                  = items[i].mCount;
			if (fn_8006E44C(id, 4) && id >= 0 && num > 0) {
				count++;
			}
		}
		lbl_8025C400.mEvt._10 = count;
		_B94                  = 0;
		break;
	}
}

/**
 * @note Address: 0x80114060
 * @note Size: 0x4E0
 */
// holds the 0x40B item at its first joint while flag 0x100 is up
// NONMATCHING: the loop and the joint's vectors take each other's registers
void XObjActor3::Kind8(u32 frames)
{
	Mtx m;
	Mtx t;
	Mtx a;
	CJObj joint;
	if (_B8C.mFlags & 0x100) {
		CVec pos;
		s32 path[1] = { 0 };
		joint.Attach(GetJointFromRoot(mJObj, 1, path));
		pos      = GetJointPosition(joint.mJObj);
		s32 kind = -1;
		if (_B8C.mFlags & 0x100) {
			kind = 0x40B;
		}
		if (kind >= 0) {
			for (s32 i = 0; i < 0x120; i++) {
				XObj* obj = lbl_8025C400.mXObjs.mPools[22].Get(i);
				if (kind == obj->GetKind()) {
					PSMTXCopy(HSD_JObjGetMtxPtr(joint.mJObj), m);
					PSMTXTrans(t, 0.0f, -14.0f, 0.0f);
					PSMTXConcat(m, t, a);
					HSD_JObj* jobj = obj->mJObj;
					MtxPtr mp      = a;
					HSD_JObjCopyMtx(jobj, mp);
					if (mStatus.mFlags & 0x10000) {
						obj->mStatus.mFlags |= 0x10000ULL;
					} else {
						obj->mStatus.mFlags &= ~0x10000ULL;
					}
				}
			}
		}
	}
}

/**
 * @note Address: 0x80114540
 * @note Size: 0xFC
 */
void XObjActor3::Kind7(u32 frames)
{
	switch (_B94) {
	case 0:
		break;
	case 1:
		if (_B98 != 0) {
			fn_80068BC0(&lbl_8025C400.mInfoScrn);
		} else {
			fn_80068BAC(&lbl_8025C400.mInfoScrn);
		}
		_B94 = 0;
		break;
	case 2:
		if (_B98 != 0) {
			fn_80068C10(&lbl_8025C400.mInfoScrn);
			fn_80068BE8(&lbl_8025C400.mInfoScrn);
			fn_80068BC0(&lbl_8025C400.mInfoScrn);
		} else {
			fn_80068BFC(&lbl_8025C400.mInfoScrn);
			fn_80068BD4(&lbl_8025C400.mInfoScrn);
			fn_80068BAC(&lbl_8025C400.mInfoScrn);
		}
		_B94 = 0;
		break;
	}
}

/**
 * @note Address: 0x801115FC
 * @note Size: 0x47C
 */
// falls from above the robot, knocks it down on a hit, blinks out on the floor
void XObjActor3::Kind10(u32 frames)
{
	XObjChr* player = lbl_8025C400.GetPlayer();
	Vec pos         = player->mMoveTranslate;
	switch (_B80) {
	case 0:
		if (_B8C.mFlags & 4) {
			break;
		}
		mMoveTranslate.x = pos.x - (rand() % 3600) / 100.0f + 18.0f;
		mMoveTranslate.y = 64.0f;
		mMoveTranslate.z = pos.z - (rand() % 3600) / 100.0f + 18.0f;
		mStatus.mFlags |= 0x10000;
		mStatus.mFlags |= 4;
		_B70 = 4.0f;
		_B88 = rand() % 30 + 30;
		_B80++;
		break;
	case 1:
		if (--_B88 < 0) {
			_B80++;
		}
		break;
	case 2:
		f32 dx   = pos.x - mMoveTranslate.x;
		f32 dy   = pos.y - mMoveTranslate.y;
		f32 dz   = pos.z - mMoveTranslate.z;
		f32 dist = sqrtf(dx * dx + dy * dy + dz * dz);
		if (dist < 8.0f && !fn_800A0000(player) && !fn_800665E0(&lbl_8025C400.mInfoScrn)) {
			player->vf138(pos, 10.0f);
			_B88 = 30;
			_B80 = 3;
			break;
		}
		if (_B70 >= -2.3f) {
			_B70 -= 0.36f * frames;
		} else {
			_B70 = -2.3f;
		}
		mMoveTranslate.y += _B70;
		if (mMoveTranslate.y < 0.0f) {
			mMoveTranslate.y = 0.0f;
			_B88             = 30;
			_B80             = 3;
		}
		break;
	case 3:
		if ((_B88 & 3) < 2) {
			mStatus.mFlags |= 0x10000;
		} else {
			mStatus.mFlags &= ~0x10000ULL;
		}
		if (--_B88 < 0) {
			mStatus.mFlags &= ~0x10000ULL;
			mStatus.mFlags &= ~4ULL;
			_B8C.mFlags |= 4;
			_B80 = 0;
		}
		break;
	}
}

/**
 * @note Address: 0x8011132C
 * @note Size: 0x2D0
 */
// whether the robot stands where the actor can reach it
extern "C" BOOL fn_8011132C(XObjActor3* actor)
{
	XObjChr* player = lbl_8025C400.GetPlayer();
	Vec pos         = player->mMoveTranslate;
	if (!(actor->_B8C.mFlags & 4)) {
		if (IsPlayerAway(player, pos)) {
			return FALSE;
		}
		if (pos.y >= actor->mMoveTranslate.y - 16.0f && pos.y <= actor->mMoveTranslate.y + 16.0f) {
			if (actor->_B7C == 1.5707964f) {
				if (actor->mMoveTranslate.x < pos.x && pos.x < actor->_BC4) {
					return TRUE;
				}
				if (actor->mMoveTranslate.x > pos.x && pos.x > actor->_BC0) {
					if (IsLongReachAnim(player->mAnimId)) {
						if (actor->mMoveTranslate.x > pos.x && pos.x > actor->mMoveTranslate.x - 32.0f) {
							return TRUE;
						}
					} else {
						if (actor->mMoveTranslate.x > pos.x && pos.x > actor->mMoveTranslate.x - 12.0f) {
							return TRUE;
						}
					}
				}
			} else {
				if (actor->mMoveTranslate.x > pos.x && pos.x > actor->_BC0) {
					return TRUE;
				}
				if (actor->mMoveTranslate.x < pos.x && pos.x < actor->_BC4) {
					if (IsLongReachAnim(player->mAnimId)) {
						if (actor->mMoveTranslate.x < pos.x && pos.x < actor->mMoveTranslate.x + 32.0f) {
							return TRUE;
						}
					} else {
						if (actor->mMoveTranslate.x < pos.x && pos.x < actor->mMoveTranslate.x + 12.0f) {
							return TRUE;
						}
					}
				}
			}
		}
	}
	return FALSE;
}

/**
 * @note Address: 0x8010FE68
 * @note Size: 0x12C
 */
// NONMATCHING: register allocation
extern "C" void fn_8010FE68(s32 n)
{
	s32 count = 0;
	u8* data;
	s32 size;
	size = lbl_8025C400.mXObjs.mPools[5].mStride;
	data = lbl_8025C400.mXObjs.mPools[5].mObjects;
	for (s32 i = 0; i < 48; i++) {
		XObjActor3* obj = (XObjActor3*)(data + i * size);
		if (XObj::GetDataItem(obj->mAttr._00) == 0xB) {
			if (count == n && (obj->mStatus.mFlags & 1)) {
				if (obj->_B80 == 0) {
					obj->_B8C.mFlags &= ~4;
				}
				return;
			}
			count++;
		}
	}
}

/**
 * @note Address: 0x8010FDA0
 * @note Size: 0xC8
 */
// starts the n-th kind 0xB actor
// NONMATCHING: register allocation
extern "C" void fn_8010FDA0(s32 n)
{
	u8* data;
	XObjActor3* obj;
	s32 size;
	s32 count = 0;
	s32 i;
	size = lbl_8025C400.mXObjs.mPools[5].mStride;
	data = lbl_8025C400.mXObjs.mPools[5].mObjects;
	for (i = 0; i < 48; i++) {
		obj = (XObjActor3*)(data + i * size);
		if (XObj::GetDataItem(obj->mAttr._00) == 0xB) {
			if (count == n && (obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 0x10000)) {
				fn_8010FB1C(obj);
				return;
			}
			count++;
		}
	}
}

/**
 * @note Address: 0x8010FB1C
 * @note Size: 0x284
 */
// jumps the actor onto its holder, at its angle and distance around it
extern "C" void fn_8010FB1C(XObjActor3* actor)
{
	XObj* holder = lbl_8025C400.mXObjs.mPools[19].Get(0);
	if ((holder->mStatus.mFlags & 1) && (holder->mStatus.mFlags & 0x10000) && actor->_B80 == 1) {
		f32 s          = sin(actor->_BC8 + HSD_JObjGetRotationY(holder->mJObj));
		f32 x          = actor->_BD8 * s;
		f32 y          = actor->_BD0;
		f32 c          = cos(actor->_BC8 + HSD_JObjGetRotationY(holder->mJObj));
		f32 z          = actor->_BD8 * c;
		actor->_B80    = 2;
		actor->_B78    = 6.0f;
		HSD_JObj* jobj = actor->mJObj;
		HSD_JObjClearMtxFlags(jobj);
		actor->mMoveTranslate.x = x + HSD_JObjGetTranslationX(holder->mJObj);
		actor->mMoveTranslate.y = y + 8.0f + 10.0f;
		actor->mMoveTranslate.z = z + HSD_JObjGetTranslationZ(holder->mJObj);
		actor->SetPos(actor->mMoveTranslate, 0);
		actor->_B7C = (f32)atan2(holder->mMoveTranslate.x - actor->mMoveTranslate.x, holder->mMoveTranslate.z - actor->mMoveTranslate.z)
		            + 3.1415927f;
		fn_8001E904(&lbl_8025C400.mSound, 0x160021, 0xFF);
	}
}

/**
 * @note Address: 0x8010F208
 * @note Size: 0x110
 */
// NONMATCHING: register allocation
extern "C" void fn_8010F208(void)
{
	XObjActor3* obj;
	u8* data;
	s32 size;
	size = lbl_8025C400.mXObjs.mPools[5].mStride;
	data = lbl_8025C400.mXObjs.mPools[5].mObjects;
	for (s32 i = 0; i < 48; i++) {
		obj = (XObjActor3*)(data + i * size);
		if (XObj::GetDataItem(obj->mAttr._00) == 0xC && (obj->mStatus.mFlags & 1)) {
			if (obj->_B80 == 0) {
				obj->_B8C.mFlags &= ~4;
			}
			return;
		}
	}
}

/**
 * @note Address: 0x8010EF70
 * @note Size: 0x298
 */
// jumps the first kind 0xC actor onto the holder's middle
// NONMATCHING: register allocation
extern "C" void fn_8010EF70(void)
{
	u8* data;
	s32 size;
	size = lbl_8025C400.mXObjs.mPools[5].mStride;
	data = lbl_8025C400.mXObjs.mPools[5].mObjects;
	for (s32 i = 0; i < 48; i++) {
		XObjActor3* obj = (XObjActor3*)(data + i * size);
		if (XObj::GetDataItem(obj->mAttr._00) == 0xC && (obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 0x10000)) {
			XObj* holder = (XObj*)lbl_8025C400.mXObjs.mPools[19].mObjects;
			if ((holder->mStatus.mFlags & 1) && (holder->mStatus.mFlags & 0x10000) && obj->_B80 == 1) {
				obj->_B80      = 2;
				obj->_B78      = 6.0f;
				HSD_JObj* jobj = obj->mJObj;
				HSD_JObjClearMtxFlags(jobj);
				f32 ofs               = 0.0f; // the holder's middle, where fn_8010FB1C goes round it
				obj->mMoveTranslate.x = ofs + HSD_JObjGetTranslationX(holder->mJObj);
				obj->mMoveTranslate.y = 18.0f;
				obj->mMoveTranslate.z = ofs + HSD_JObjGetTranslationZ(holder->mJObj);
				obj->SetPos(obj->mMoveTranslate, 0);
				obj->_B7C = (f32)atan2(holder->mMoveTranslate.x - obj->mMoveTranslate.x, holder->mMoveTranslate.z - obj->mMoveTranslate.z)
				          + 3.1415927f;
				obj->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				fn_8001E904(&lbl_8025C400.mSound, 0x160021, 0xFF);
			}
			return;
		}
	}
}

/**
 * @note Address: 0x8010D390
 * @note Size: 0x2CC
 */
// NONMATCHING: register allocation
extern "C" void fn_8010D390(s32 mode)
{
	if (mode == 0) {
		for (s32 i = 0; i < 48; i++) {
			XObjActor3* obj = (XObjActor3*)lbl_8025C400.mXObjs.mPools[5].Get(i);
			if (obj->GetKind() == 0x3E0 && (obj->mStatus.mFlags & 1)) {
				obj->_B8C.mFlags &= ~4;
				obj->_B80    = 0;
				lbl_8065B2F0 = 0;
			}
		}
	}
	if (mode == 2) {
		for (s32 i = 0; i < 48; i++) {
			XObjActor3* obj = (XObjActor3*)lbl_8025C400.mXObjs.mPools[5].Get(i);
			if (obj->GetKind() == 0x3E0 && (obj->mStatus.mFlags & 1) && obj->_B8C.Check(4)) {
				obj->_B8C.mFlags &= ~4;
				obj->_B80    = 20;
				lbl_8065B2F0 = 0;
			}
		}
	} else {
		u8* data;
		s32 size;
		size = lbl_8025C400.mXObjs.mPools[5].mStride;
		data = lbl_8025C400.mXObjs.mPools[5].mObjects;
		for (s32 i = 0; i < 48; i++) {
			XObjActor3* obj = (XObjActor3*)(data + i * size);
			if (obj->GetKind() == 0x3E0) {
				if (!(obj->mStatus.mFlags & 1)) {
					return;
				}
				if (obj->_B8C.Check(4)) {
					obj->_B8C.mFlags &= ~4;
					obj->_B80 = 10;
					return;
				}
			}
		}
	}
}

/**
 * @note Address: 0x8010D114
 * @note Size: 0x27C
 */
void XObjActor3::Kind14(u32 frames)
{
	switch (_BEC) {
	case 0:
		_BEC = 4;
		_BF0 = 0.0f;
		break;
	case 1:
		_BF0 = 1.0f;
		break;
	case 2:
		_BF0 = 0.0f;
		break;
	case 3:
		_BF0 += 0.08f * frames;
		if (_BF0 >= 1.0f) {
			_BF0 = 1.0f;
		}
		break;
	case 4:
		_BF0 -= 0.08f * frames;
		if (_BF0 < 0.0f) {
			_BF0 = 0.0f;
		}
		break;
	}
	if (_BF8) {
		_BF4 -= frames;
	}
	switch (_BFC) {
	case 0:
		_BFC = 4;
		_C00 = 0.0f;
		break;
	case 1:
		_C00 = 1.0f;
		break;
	case 2:
		_C00 = 0.0f;
		break;
	case 3:
		_C00 += 0.08f * frames;
		if (_C00 >= 1.0f) {
			_C00 = 1.0f;
		}
		break;
	case 4:
		_C00 -= 0.08f * frames;
		if (_C00 < 0.0f) {
			_C00 = 0.0f;
		}
		break;
	}
	s32 count = 0;
	for (s32 i = 0; i < 16; i++) {
		XObj* obj = lbl_8025C400.mXObjs.mPools[17].Get(i);
		if ((obj->mStatus.mFlags & 0x10000) && !fn_800EBDB0(obj)) {
			count++;
		}
	}
	_C04 = count;
}

/**
 * @note Address: 0x8010CE80
 * @note Size: 0x294
 */
void XObjActor3::Kind15(u32 frames)
{
	switch (_B80) {
	case 0:
		if (_B8C.mFlags & 4) {
			break;
		}
		_B80++;
		SetAnim(1, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		if (++lbl_8065B2F4 >= 10) {
			lbl_8065B2F4 = 0;
		}
		if (!fn_8001E6C8(&lbl_8025C400.mSound, 0x127)) {
			fn_8001E904(&lbl_8025C400.mSound, 0x127, 0xFF);
		}
		break;
	case 1:
		f32 end = GetAnimEndFrame(mJObj);
		if (end - frames <= GetAnimFrame(mJObj)) {
			_B80++;
			SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
		break;
	case 2:
		_B8C.mFlags |= 4;
		_B80 = 0;
		break;
	}
}

/**
 * @note Address: 0x8010CDDC
 * @note Size: 0xA4
 */
// puts every waiting 0x43A actor at the point
// NONMATCHING: register allocation
extern "C" void fn_8010CDDC(f32 x, f32 y, f32 z)
{
	XObjActor3* obj;
	for (s32 i = 0; i < 48; i++) {
		obj = (XObjActor3*)lbl_8025C400.mXObjs.mPools[5].Get(i);
		switch (obj->GetKind()) {
		case 0x43A:
			if ((obj->mStatus.mFlags & 1) && obj->_B8C.Check(4)) {
				obj->_B8C.mFlags &= ~4;
				obj->mMoveTranslate.x = x;
				obj->mMoveTranslate.y = y;
				obj->mMoveTranslate.z = z;
			}
			break;
		}
	}
}

/**
 * @note Address: 0x8010C0F0
 * @note Size: 0x214
 */
// plays anim n on the first kind 0x10 actor and sizes it for it
extern "C" void fn_8010C0F0(s32 n)
{
	u8* data;
	s32 size;
	size = lbl_8025C400.mXObjs.mPools[5].mStride;
	data = lbl_8025C400.mXObjs.mPools[5].mObjects;
	for (s32 i = 0; i < 48; i++) {
		XObjActor3* obj = (XObjActor3*)(data + i * size);
		if (XObj::GetDataItem(obj->mAttr._00) == 0x10 && (obj->mStatus.mFlags & 1)) {
			obj->SetAnim(n, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			obj->_B8C.mFlags &= ~4;
			obj->_B8C.mFlags &= ~0x10;
			switch (n) {
			case 0:
				obj->SetScale(0.1f, 0.1f, 0.1f, 0);
				obj->_B80 = 0;
				break;
			case 1:
				obj->SetScale(1.4f, 1.4f, 1.4f, 0);
				obj->_B80 = 1;
				break;
			case 2:
				obj->SetScale(1.0f, 1.0f, 1.0f, 0);
				obj->_B80 = 2;
				break;
			case 3:
				obj->SetScale(1.0f, 1.0f, 1.0f, 0);
				obj->_B80 = 1;
				break;
			case 4:
				obj->SetScale(1.0f, 1.0f, 1.0f, 0);
				obj->_B80 = 4;
				break;
			}
			return;
		}
	}
}

/**
 * @note Address: 0x8010BF80
 * @note Size: 0x170
 */
// puts the first kind 0x10 actor back to its rest pose, small
extern "C" void fn_8010BF80(void)
{
	u8* data;
	s32 size;
	size = lbl_8025C400.mXObjs.mPools[5].mStride;
	data = lbl_8025C400.mXObjs.mPools[5].mObjects;
	for (s32 i = 0; i < 48; i++) {
		XObjActor3* obj = (XObjActor3*)(data + i * size);
		if (XObj::GetDataItem(obj->mAttr._00) == 0x10 && (obj->mStatus.mFlags & 1)) {

			obj->_B80 = 0;
			obj->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			obj->_B8C.mFlags |= 4;
			obj->_B8C.mFlags |= 0x10;
			obj->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			obj->_B8C.mFlags &= ~4;
			obj->_B8C.mFlags &= ~0x10;
			obj->SetScale(0.1f, 0.1f, 0.1f, 0);
			obj->_B80 = 0;
			return;
		}
	}
}

/**
 * @note Address: 0x8010BC0C
 * @note Size: 0x370
 */
// slides out to the left from about ten to noon and back in the afternoon
void XObjActor3::Kind18(u32 frames)
{
	f32 time = lbl_8025C400.mTimeState.mTime;
	switch (_B80) {
	case 0:
		if (time >= 0.42f && time < 0.5f) {
			_B80++;
		}
		break;
	case 1:
		mMoveTranslate.x -= 0.2f * frames;
		if (mMoveTranslate.x <= GetPlacePos(GetPlace()).x - 10.0f) {
			mMoveTranslate.x = GetPlacePos(GetPlace()).x - 10.0f;
			_B80++;
		}
		break;
	case 2:
		if (time >= 0.5f) {
			_B80++;
		}
		break;
	case 3:
		mMoveTranslate.x += 0.2f * frames;
		if (mMoveTranslate.x >= GetPlacePos(GetPlace()).x) {
			mMoveTranslate.x = GetPlacePos(GetPlace()).x;
			_B80             = 0;
		}
		break;
	}
}

/**
 * @note Address: 0x8010BF7C
 * @note Size: 0x4
 */
void XObjActor3::Kind17(u32 frames)
{
}

/**
 * @note Address: 0x8010BC08
 * @note Size: 0x4
 */
void XObjActor3::Kind19(u32 frames)
{
}

/**
 * @note Address: 0x8010B528
 * @note Size: 0x28C
 */
extern "C" void fn_8010B528(XObjActor3* actor)
{
	switch (actor->_B94) {
	case 1:
		if (actor->_B98 != 0) {
			actor->_B8C.mFlags &= ~4;
			actor->_BC0 = actor->_B9C / 100.0f;
			actor->_BC4 = actor->_BA0 / 100.0f;
		} else {
			actor->_B8C.mFlags |= 4;
			actor->_BC0 = 0.0f;
			actor->_BC4 = 0.0f;
			actor->_B80 = 0;
		}
		actor->_B94 = 0;
		actor->_B98 = 0;
		break;
	case 2:
		if ((actor->_B8C.mFlags & 0x80) && !(actor->_B8C.mFlags & 4)) {
			lbl_80367A2C[actor->_B98] = 1;
		}
		actor->_B94 = 0;
		actor->_B98 = 0;
		break;
	case 3:
		actor->_B8C.mFlags &= ~0x80;
		actor->_B94 = 0;
		actor->_B98 = 0;
		break;
	case 4:
		XObjChr* player = lbl_8025C400.GetPlayer();
		if (!IsBusyAnim(player->mAnimId) && player->mAttachmentRequest < 0) {
			lbl_80367A2C[actor->_B98] = 1;
		}
		actor->_B94 = 0;
		actor->_B98 = 0;
		break;
	}
}

/**
 * @note Address: 0x8010B7B4
 * @note Size: 0x454
 */
// carries the robot while it rides the actor, at the actor's first joint
// NONMATCHING: the robot and the joint's vectors take each other's registers
extern "C" void fn_8010B7B4(XObjActor3* actor, u32 frames)
{
	CJObj joint;
	XObjChr* player = lbl_8025C400.GetPlayer();
	if ((actor->_B8C.mFlags & 0x40) && (player->_980.mFlags & 0x40000000)) {
		Mtx scaleMtx;
		Mtx rotMtx;
		Mtx transMtx;
		Mtx mtx;
		Mtx m;
		s32 path[2] = { 0, 0 };
		joint.Attach(GetJointFromRoot(actor->mJObj, 2, path));
		PSMTXCopy(HSD_JObjGetMtxPtr(joint.mJObj), m);
		Vec scale;
		HSD_MtxGetScale(m, &scale);
		scale.x = fn_800621E0(player).x / scale.x;
		scale.y = fn_800621E0(player).y / scale.y;
		scale.z = fn_800621E0(player).z / scale.z;
		PSMTXScale(scaleMtx, scale.x, scale.y, scale.z);
		PSMTXConcat(m, scaleMtx, mtx);
		PSMTXRotRad(rotMtx, 'y', 0.0f);
		PSMTXConcat(mtx, rotMtx, mtx);
		PSMTXTrans(transMtx, 0.0f, 3.7f, 30.0f);
		PSMTXConcat(mtx, transMtx, mtx);
		HSD_JObj* jobj = player->mJObj;
		MtxPtr mp      = mtx;
		HSD_JObjCopyMtx(jobj, mp);
	}
}

/**
 * @note Address: 0x8010B394
 * @note Size: 0x194
 */
// the script's commands to the actor, _B94 is the command and _B98 its argument
extern "C" void fn_8010B394(XObjActor3* actor)
{
	switch (actor->_B94) {
	case 0:
		break;
	case 1:
		if (actor->_B98 != 0) {
			actor->_BEC = 3;
		} else {
			actor->_BEC = 4;
		}
		actor->_B94 = 0;
		actor->_B98 = 0;
		break;
	case 2:
		if (actor->_B98 != 0) {
			actor->_BF8 = 1;
		} else {
			actor->_BF8 = 0;
		}
		actor->_B94 = 0;
		actor->_B98 = 0;
		break;
	case 3:
		actor->_BF4 = 60.0f * (actor->_B98 / 100.0f);
		actor->_B94 = 0;
		actor->_B98 = 0;
		break;
	case 4:
		lbl_80367A2C[actor->_B98] = fn_8006697C(actor->_BF4);
		actor->_B94               = 0;
		actor->_B98               = 0;
		break;
	case 5:
		if (actor->_B98 != 0) {
			actor->_BFC = 3;
		} else {
			actor->_BFC = 4;
		}
		actor->_B94 = 0;
		actor->_B98 = 0;
		break;
	case 6:
		actor->_C04 = actor->_B98;
		actor->_B94 = 0;
		actor->_B98 = 0;
		break;
	case 7:
		lbl_80367A2C[actor->_B98] = actor->_C04;
		actor->_B94               = 0;
		actor->_B98               = 0;
		break;
	}
}

/**
 * @note Address: 0x8010AD88
 * @note Size: 0x1EC
 */
extern "C" void fn_8010AD88(XObjActor3* actor)
{
	actor->_B70        = 0.0f;
	actor->_B74        = 0.0f;
	actor->_B78        = 0.0f;
	actor->_B7C        = 0.0f;
	actor->_B80        = 0;
	actor->_B88        = 0;
	actor->_B8C.mFlags = 0;
	actor->_B8C.mFlags |= 4;
	actor->_B94    = 0;
	actor->_B98    = 0;
	actor->_B9C    = 0;
	actor->_BA0    = 0;
	actor->_BA4[0] = 0;
	actor->_BA4[1] = 0;
	actor->_BA4[2] = 0;
	actor->_BA4[3] = 0;
	actor->_BA4[4] = 0;
	actor->_BA4[5] = 0;
	actor->_BA4[6] = 0;
	actor->_BC0    = 0.0f;
	actor->_BC4    = 0.0f;
	actor->_BC8    = 0.0f;
	Vec zero       = { 0.0f, 0.0f, 0.0f };
	actor->_BCC    = zero.x;
	actor->_BD0    = zero.y;
	actor->_BD4    = zero.z;
	for (int i = 0; i < 8; i++) {
		actor->mParts[i].Clear();
	}
	if (actor->_258 == 0) {
		lbl_8065B2F0 = 0;
		lbl_8065B2F4 = 0;
	}
	actor->vf12C(1, 0, 0, 0, 0.0f);
	actor->_BDC.x = 0.0f;
	actor->_BDC.y = 0.0f;
	actor->_BDC.z = 0.0f;
	actor->_BEC   = 0;
	actor->_BF0   = 0.0f;
	actor->_BF4   = 0.0f;
	actor->_BF8   = 0;
	actor->_BFC   = 0;
	actor->_C00   = 0.0f;
	actor->_C04   = 0;
	actor->_C08   = 0;
	if (actor->GetKind() == 0x31B) {
		fn_80024FAC(&actor->mDust.mPtcl, actor->mDust.mPtcls, 0x50, 0x32);
		actor->mAnimSfx.mEvents = lbl_8022C178;
	}
	actor->_1DC0[0]     = 0;
	actor->_1DC0[1]     = 0;
	actor->_1DC0[2]     = 0;
	actor->_1DC0[3]     = 0;
	actor->_1DC0[4]     = 0;
	actor->_1DC0[5]     = 0;
	actor->mRumble      = 0;
	actor->mRumbleTimer = 0;
	actor->mHeld        = NULL;
	actor->_1DE4        = 0;
	actor->mSe          = -1;
}

/**
 * @note Address: 0x8010AF74
 * @note Size: 0x420
 */
// the robot gets on the actor at its first joint, or off it
// NONMATCHING: the robot and the joint's vectors take each other's registers
extern "C" void fn_8010AF74(XObjActor3* actor)
{
	CJObj joint;
	XObjChr* player = lbl_8025C400.GetPlayer();
	switch (actor->_B94) {
	case 1:
		switch (actor->_B98) {
		case 1:
			actor->_B8C.mFlags |= 0x40;
			player->_980.mFlags = 0;
			player->_980.mFlags |= 0x40000000;
			player->mPadReset = 0;
			break;
		default:
			actor->_B8C.mFlags &= ~0x40;
			s32 path[2] = { 0, 0 };
			joint.Attach(GetJointFromRoot(actor->mJObj, 2, path));
			CVec pos(GetJointPosition(joint.mJObj));
			player->SetPos(pos, 0);
			player->_980.mFlags = 0;
			player->_980.mFlags |= 0x20000000;
			player->_980.mFlags |= 7;
			player->mPadReset = 1;
			HSD_JObjClearMtxFlags(player->mJObj);
			break;
		}
		actor->_B94 = 0;
		actor->_B98 = 0;
		break;
	}
}

/**
 * @note Address: 0x80108794
 * @note Size: 0x870
 */
// the event commands: 100 carries another object at one of its joints, or lets go of all
// of them, anything else is kept for the kind's function to read
// NONMATCHING: argv[1] and the 0x40B item loop take other registers, as in game/xobjspider
extern "C" s32 fn_80108794(XObjActor3* actor, s32 argc, s32* argv)
{
	switch (argv[0]) {
	case 100:
		if (argv[1] == 0) {
			actor->_B8C.mFlags &= ~2;
			for (u32 i = 0; i < 8; i++) {
				if (actor->mParts[i].mObj != NULL) {
					HSD_JObjClearMtxFlags(actor->mParts[i].mObj->mJObj);
					actor->mParts[i].mObj   = NULL;
					actor->mParts[i].mJoint = NULL;
				}
			}
		} else {
			u32 i;
			for (i = 0; actor->mParts[i].mObj != NULL && i < 8; i++) { }
			if (i < 8) {
				OSReport("id = %d, argv[1]=%d\n", i);
				actor->_B8C.mFlags |= 2;
				if (argv[1] >= 10000) {
					actor->mParts[i].mObj = FindEvtObj(argv[1]);
				} else {
					actor->mParts[i].mObj = GetObj(argv[1]);
				}
				if (actor->mParts[i].mObj == NULL) {
					OSReport("NULL===============\n");
				}
				s32 path[8];
				s32 depth = argv[2];
				for (s32 j = 0; j < depth; j++) {
					path[j] = argv[3 + j];
				}
				actor->mParts[i].mJoint = GetJointByPath(actor->mJObj, path, depth);
				Mtx t;
				XObj* obj = actor->mParts[i].mObj;
				PSMTXTrans(t, obj->mMoveTranslate.x, obj->mMoveTranslate.y, obj->mMoveTranslate.z);
				PSMTXCopy(HSD_JObjGetMtxPtr(actor->mParts[i].mJoint), actor->mParts[i].mMtx);
				Vec scale;
				HSD_MtxGetScale(actor->mParts[i].mMtx, &scale);
				scale.x = fn_800621E0(actor->mParts[i].mObj).x / scale.x;
				scale.y = fn_800621E0(actor->mParts[i].mObj).y / scale.y;
				scale.z = fn_800621E0(actor->mParts[i].mObj).z / scale.z;
				Mtx s;
				PSMTXScale(s, scale.x, scale.y, scale.z);
				Mtx m;
				PSMTXConcat(actor->mParts[i].mMtx, s, m);
				PSMTXConcat(m, t, m);
				HSD_JObj* joint = actor->mParts[i].mObj->mJObj;
				MtxPtr mp       = m;
				HSD_JObjCopyMtx(joint, mp);
			}
		}
		break;
	default:
		actor->_B94    = argv[0];
		actor->_B98    = argv[1];
		actor->_B9C    = argv[2];
		actor->_BA0    = argv[3];
		actor->_BA4[0] = argv[4];
		actor->_BA4[1] = argv[5];
		actor->_BA4[2] = argv[6];
		actor->_BA4[3] = argv[7];
		actor->_BA4[4] = argv[8];
		actor->_BA4[5] = argv[9];
		actor->_BA4[6] = 0;
		switch (actor->GetItem()) {
		case 9:
			fn_8010B528(actor);
			break;
		case 0x13:
			fn_8010AF74(actor);
			break;
		case 8:
			switch (actor->_B94) {
			case 0:
				break;
			case 1:
				s32 kind = -1;
				if (actor->_B8C.Check(0x100)) {
					kind = 0x40B;
					actor->_B8C.mFlags &= ~0x100;
				}
				HideItems(kind);
				switch (actor->_B98) {
				case 0x40B:
					for (s32 i = 0; i < 0x120; i++) {
						XObj* obj = lbl_8025C400.mXObjs.mPools[22].Get(i);
						if (actor->_B98 == obj->GetKind()) {
							actor->mHeld = obj;
							obj->mStatus.mFlags |= 0x10000ULL;
							switch (actor->_B98) {
							case 0x40B:
								actor->_B8C.mFlags |= 0x100;
								obj->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
								break;
							}
						}
					}
					break;
				default:
					if (actor->mHeld != NULL) {
						HSD_JObjClearMtxFlags(actor->mHeld->mJObj);
						actor->mHeld = NULL;
					}
					break;
				}
				actor->_B94 = 0;
				break;
			}
			break;
		}
		break;
	}
	return 0;
}

/**
 * @note Address: 0x8010AD54
 * @note Size: 0x34
 */
extern "C" void fn_8010AD54(XObjActor3* actor, u32 id)
{
	fn_800929C4(actor, id);
	fn_8010AD88(actor);
}

/**
 * @note Address: 0x8010AD20
 * @note Size: 0x34
 */
extern "C" void fn_8010AD20(XObjActor3* actor)
{
	fn_800928E8(actor);
	fn_8010AD88(actor);
}

/**
 * @note Address: 0x8010AD00
 * @note Size: 0x20
 */
extern "C" void fn_8010AD00(XObjActor3* actor)
{
	fn_8010AD88(actor);
}

/**
 * @note Address: 0x8010A500
 * @note Size: 0x74
 */
extern "C" void fn_8010A500(XObjActor3* actor)
{
	switch (actor->GetKind()) {
	case 0x149:
	case 0x42B:
		actor->_980.Close();
		break;
	}
	fn_800911C0(actor);
}

/**
 * @note Address: 0x80109E80
 * @note Size: 0x680
 */
// the per-kind script commands, the kind's own function, the carried objects
extern "C" void fn_80109E80(XObjActor3* actor, u32 frames)
{
	switch (actor->GetItem()) {
	case 8:
		actor->_B8C.mFlags |= 4;
		break;
	case 9:
		actor->vf148();
		if (lbl_8025C400.mStatus.mFlags & 0x400) {
			return;
		}
		break;
	case 10:
		if (lbl_8025C400.mStatus.mFlags & 0x400) {
			return;
		}
		switch (actor->_B94) {
		case 1:
			actor->_B8C.mFlags &= ~4;
			actor->_B94 = 0;
			actor->_B98 = 0;
			break;
		}
		break;
	case 11:
	case 12:
		if (lbl_8025C400.mStatus.mFlags & 0x400) {
			return;
		}
		switch (actor->_B94) {
		case 1:
			if (actor->_B98 != 0) {
				actor->_B8C.mFlags &= ~4;
			} else {
				actor->_B8C.mFlags |= 4;
				actor->_B80 = 0;
			}
			actor->_B94 = 0;
			actor->_B98 = 0;
			break;
		case 2:
			actor->_B80 = 2;
			actor->_B78 = actor->_B9C / 100.0f;
			actor->_B94 = 0;
			actor->_B98 = 0;
			break;
		}
		break;
	case 13:
		switch (actor->mRumble) {
		case 0:
			break;
		case 1:
			if (lbl_8025C400.mStatus.mFlags & 0x800000000ULL) {
				CPadOne* rumble = &lbl_8025C400.mPad.mPad[0];
				if (rumble->mConnected) {
					HSD_PadRumbleOn(rumble->mPort);
				}
				actor->mRumbleTimer = 24 / frames;
				actor->mRumble++;
			} else {
				actor->mRumble = 0;
			}
			break;
		case 2:
			if (--actor->mRumbleTimer < 0) {
				CPadOne* rumble = &lbl_8025C400.mPad.mPad[0];
				if (rumble->mConnected) {
					HSD_PadRumbleOffN(rumble->mPort);
				}
				if (rumble->mConnected) {
					HSD_PadRumbleRemove(rumble->mPort);
				}
				actor->mRumble = 0;
			}
			break;
		}
		if (lbl_8025C400.mStatus.mFlags & 0x400) {
			return;
		}
		switch (actor->_B94) {
		case 1:
			actor->_B8C.mFlags &= ~4;
			actor->_B94 = 0;
			actor->_B98 = 0;
			break;
		}
		break;
	case 14:
		if (lbl_8025C400.mStatus.mFlags & 0x400) {
			return;
		}
		fn_8010B394(actor);
		break;
	}
	if (actor->_B8C.mFlags & 0x10) {
		return;
	}
	(actor->*lbl_8022C330[actor->GetItem()])(frames);
	if (!(actor->_B8C.mFlags & 4)) {
		actor->SetRotateY(actor->_B7C, 0.2f);
		actor->SetPos(actor->mMoveTranslate, 0);
	}
	fn_80090ED4(actor, frames);
	if (actor->GetKind() == 0x31B) {
		fn_80024D84(&actor->mDust.mPtcl, frames);
	}
	if (actor->GetKind() == 0x2C3) {
		fn_8010B7B4(actor, frames);
	}
	if (actor->_B8C.mFlags & 2) {
		for (u32 i = 0; i < 8; i++) {
			if (actor->mParts[i].mJoint != NULL && actor->mParts[i].mObj != NULL) {
				Mtx t;
				XObj* obj = actor->mParts[i].mObj;
				PSMTXTrans(t, obj->mMoveTranslate.x, obj->mMoveTranslate.y, obj->mMoveTranslate.z);
				PSMTXCopy(HSD_JObjGetMtxPtr(actor->mParts[i].mJoint), actor->mParts[i].mMtx);
				Vec scale;
				HSD_MtxGetScale(actor->mParts[i].mMtx, &scale);
				scale.x = fn_800621E0(actor->mParts[i].mObj).x / scale.x;
				scale.y = fn_800621E0(actor->mParts[i].mObj).y / scale.y;
				scale.z = fn_800621E0(actor->mParts[i].mObj).z / scale.z;
				Mtx s;
				PSMTXScale(s, scale.x, scale.y, scale.z);
				Mtx m;
				PSMTXConcat(actor->mParts[i].mMtx, s, m);
				PSMTXConcat(m, t, m);
				HSD_JObj* joint = actor->mParts[i].mObj->mJObj;
				MtxPtr mp       = m;
				HSD_JObjCopyMtx(joint, mp);
			}
		}
	}
}

/**
 * @note Address: 0x80109E44
 * @note Size: 0x3C
 */
extern "C" void fn_80109E44(XObjActor3* actor)
{
	if (!(actor->_B8C.mFlags & 0x10)) {
		actor->vfB0(0);
	}
}

/**
 * @note Address: 0x8010904C
 * @note Size: 0x59C
 */
// kind 0xE on stage02: the panel and the count of _C04 in two digits over it, faded by _C00
// NONMATCHING: the colours, the texture matrix pointer and the loop count take other registers
extern "C" void fn_8010904C(XObjActor3* actor)
{
	f32 alpha = actor->_C00;
	if (alpha <= 0.0f) {
		return;
	}
	if (!IsStage("stage02")) {
		return;
	}
	if (!actor->_980.mBound) {
		return;
	}
	CTexObj tex;
	tex       = actor->_980.mTex.Init(&((TPLPalette*)actor->_980.mBuffer)->descriptorArray[9], 0);
	f32 scale = 1.0f;
	f32 w     = scale * GXGetTexObjWidth(&tex);
	u16 h     = GXGetTexObjHeight(&tex);
	fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
	fn_80197800(&lbl_8025C400.mGs, &tex, -1, -1, -1, -1, 320.0f, 40.0f, 0.0f, w, TexSize(h, 1.0f), 0.0f, 0.0f, 0.0f);
	fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
	tex    = actor->_980.mTex.Init(&((TPLPalette*)actor->_980.mBuffer)->descriptorArray[1], 0);
	f32 dw = 1.4f * TexSize(GXGetTexObjWidth(&tex) / 11, 1.0f);
	f32 x;
	f32 dh    = 1.4f * TexSize(GXGetTexObjHeight(&tex), 1.0f);
	x         = 110.0f - (1 - actor->_C04 / 10) * 12;
	f32 count = actor->_C04;
	PSMTXScale(tex.mMtx[1], 0.09090909f, 1.0f, 1.0f);
	tex.mMtxDirty = TRUE;
	s32 i         = 0;
	u32 a         = 255.0f * alpha;
	u32 c0;
	u32 c1 = a | 0x1F33FF00;
	c0     = a | 0xFFFFFF00;
	for (; i < 2; i++) {
		f32 p = 1.0f;
		if (i > 0) {
			for (s32 k = 0; k < i; k++) {
				p *= 10.0f;
			}
		}
		s32 d = count / p;
		if (i == 0 || d != 0) {
			d %= 10;
			PSMTXTrans(tex.mMtx[0], d * 0.09090909f, 0.0f, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
			fn_80197800(&lbl_8025C400.mGs, &tex, c0, c0, c1, c1, 7.0f * dw + (x - i * (dw - 8.0f)) - 32.0f, 76.0f, 0.0f, dw, dh, 0.0f, 0.0f,
			            0.0f);
			fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
		}
	}
}

/**
 * @note Address: 0x80109DD8
 * @note Size: 0x6C
 */
extern "C" void fn_80109DD8(XObjActor3* actor)
{
	switch (actor->GetItem()) {
	case 0xE:
		fn_801095E8(actor);
		fn_8010904C(actor);
		break;
	}
}

/**
 * @note Address: 0x80109008
 * @note Size: 0x44
 */
extern "C" void fn_80109008(XObjActor3* actor)
{
	if (actor->GetKind() == 0x31B) {
		fn_80024CA0(&actor->mDust.mPtcl);
	}
}

/**
 * @note Address: 0x80109004
 * @note Size: 0x4
 */
extern "C" void fn_80109004(XObjActor3* actor)
{
}

/**
 * @note Address: 0x80108728
 * @note Size: 0x6C
 */
extern "C" void fn_80108728(XObjActor3* actor, const Vec* pos, f32 f)
{
	if (actor->GetItem() == 1) {
		fn_8008A6C0(actor, pos, f);
	} else if (!actor->_B8C.Check(1)) {
		actor->_B8C.mFlags |= 1;
	}
}

/**
 * @note Address: 0x80108694
 * @note Size: 0x94
 */
extern "C" void fn_80108694(XObjActor3* actor, s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate,
                            f32 blend, f32 blendRate)
{
	fn_800902A4(actor, id, force, jointAnim, matAnim, shapeAnim, frame, rate, blend, blendRate);
	if (actor->GetKind() == 0x31B) {
		if (actor->mAnimId == 7) {
			actor->mSe = fn_8001E904(&lbl_8025C400.mSound, 0xB0000, 0xFF);
		} else if (HSD_AudioSFXCheck(actor->mSe)) {
			HSD_AudioSFXKeyOff(actor->mSe);
		}
	}
}

/**
 * @note Address: 0x80108540
 * @note Size: 0x154
 */
// NONMATCHING: register allocation
void XObjActor3::vf148()
{
	if ((mStatus.mFlags & 1) && (mStatus.mFlags & 0x10000) && _258 == 3) {
		s32 id    = mAnimId;
		f32 frame = GetAnimFrame(mJObj);
		mAnimSfx.Update(id, frame, fn_8001DF00(&lbl_8025C400.mSound, &mMoveTranslate, 0));
	}
}

/**
 * @note Address: 0x80108424
 * @note Size: 0x11C
 */
XObjActor3::~XObjActor3()
{
}
