#include "game/particle_srt.h"
#include "game/scr.h"
#include "game/ctpl.h"

// XObjActor3, the actor most of the stage's small moving things are. which one it is comes
// from the stage table, see GetDescKind and GetInfoKind. the class name is from its vtable,
// nothing else has a name in the dol, so everything is extern "C" under its address

#include "game/gmath.h"
#include <stdlib.h>

extern "C" {
BOOL HSD_AudioSFXCheck(s32 id);
void HSD_AudioSFXKeyOff(s32 id);
u16 fn_8006E44C(s16 item, s32 kind);
f32 fn_8006697C(f32 x);
u8 fn_800EBDB0(XObj* obj);
BOOL fn_800A0000(XObj* obj);
void fn_800661E4(void* mgr, XObj* obj, s32 on, s32 b);
void fn_80068BAC(void* mgr);
void fn_80068BC0(void* mgr);
void fn_80068BD4(void* mgr);
void fn_80068BE8(void* mgr);
void fn_80068BFC(void* mgr);
void fn_80068C10(void* mgr);
void HSD_PadRumbleOn(u8 no);
void HSD_PadRumbleOffN(u8 no);
void HSD_PadRumbleRemove(u8 no);
CVec fn_800621E0(XObj* obj);
void HSD_MtxGetScale(Mtx mat, Vec* vec);
void PSMTXScale(Mtx mtx, f32 x, f32 y, f32 z);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void HSD_MtxGetTranslate(Mtx mat, Vec* vec);
void PSMTXRotRad(Mtx m, char axis, f32 rad);
s32 fn_8006F654(const char* name);
void fn_8019AFA0(void* tev, s32 mode);
void fn_80197800(void* gs, CTexObj* tex, u32 c0, u32 c1, u32 c2, u32 c3, f32 x, f32 y, f32 z, f32 w, f32 h, f32 a, f32 b, f32 c);
u16 GXGetTexObjWidth(const GXTexObj* obj);
u16 GXGetTexObjHeight(const GXTexObj* obj);
XObj* fn_8007F570(void* list, s16 id);
void OSReport(const char* fmt, ...);
}

// the game object, only the pad the actors rumble and the flags
struct UnkActor3Game {
	u8 _0000[0x19B4];          // _0000
	u8 mPadNo;                 // _19B4, the pad that rumbles
	u8 _19B5[0x4B];            // _19B5
	BOOL mRumbleOn;            // _1A00
	u8 _1A04[0x1B10 - 0x1A04]; // _1A04
	u64 mFlags;                // _1B10

	void StartRumble()
	{
		if (mRumbleOn) {
			HSD_PadRumbleOn(mPadNo);
		}
	}
	void StopRumble()
	{
		if (mRumbleOn) {
			HSD_PadRumbleOffN(mPadNo);
		}
		if (mRumbleOn) {
			HSD_PadRumbleRemove(mPadNo);
		}
	}
};
#define GAME ((UnkActor3Game*)lbl_8025C400)

// one slot of the robot's item list
struct UnkPlayerItem {
	s16 mId;  // _00
	s16 mNum; // _02
};

// chibi robo, only the item list
class UnkPlayer : public XObj {
public:
	u32 _0980;                  // _0980
	u8 _0984[0x95D4 - 0x984];   // _0984
	s32 _95D4;                  // _95D4
	u8 _95D8[0x9688 - 0x95D8];  // _95D8
	UnkPlayerItem mItems[256];  // _9688
	u8 _9A88[0x12DF4 - 0x9A88]; // _9A88
	s32 _12DF4;                 // _12DF4
};

// the robot is out of reach, past the front edge or in one of its climbing anims
static inline bool IsPlayerAway(UnkPlayer* player, Vec pos)
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
#define GAME_PLAYER    (*(UnkPlayer**)(lbl_8025C400 + 0x96BC))
#define GAME_ITEMCOUNT (*(s32*)(lbl_8025C400 + 0x5D08C))
#define GAME_HOLDER    (*(XObj**)(lbl_8025C400 + 0x9810))     // what the kind 0xB and 0xC actors sit on
#define GAME_ITEMS     ((UnkObjPool*)(lbl_8025C400 + 0x984C)) // 288 of them
#define GAME_5D080     (*(void**)(lbl_8025C400 + 0x5D080))
#define GAME_OBJS      ((XObj**)(lbl_8025C400 + 0x9860))
#define GAME_STAGE     (*(s32*)(lbl_8025C400 + 0x3444))
#define GS_TEV         (*(void**)(lbl_8025C400 + 0x294))
#define GAME_DAYTIME   (*(f32*)(lbl_8025C400 + 0xE5D2C)) // the time of day, 0 to 1

// the stage table entry the actor was placed from, the positions in hundredths, the angles in degrees
struct UnkActor3Place {
	s32 mKind;   // _00
	s32 mPos[3]; // _04
	s32 mRot[3]; // _10
};

// the joint at the end of a path of child and next steps
static inline HSD_JObj* GetJointFromRoot(HSD_JObj* jobj, s32 depth, s32* path)
{
	if (jobj == NULL) {
		return NULL;
	}
	for (s32 i = 0; i < depth; i++) {
		jobj = HSD_JObjGetChild(jobj);
		for (s32 j = 0; j < *path; j++) {
			if ((jobj = HSD_JObjGetNext(jobj)) == NULL) {
				return NULL;
			}
		}
		path++;
	}
	return jobj;
}

// the jobj's place in the world
static inline Vec GetJObjPos(HSD_JObj* jobj)
{
	Vec v;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(jobj), &v);
	return v;
}

// hands the matrix back to the srt values
static inline void HSD_JObjClearUserMtx(HSD_JObj* jobj)
{
	JOBJ_H_ASSERT(1199, jobj);
	jobj->flags &= ~0x03800000;
	HSD_JObjSetMtxDirty(jobj);
}

// hides the items of the kind, the 0x40B ones only
static inline void HideItems(s32 kind)
{
	if (kind >= 0) {
		switch (kind) {
		case 0x40B:
			for (s32 i = 0; i < 0x120; i++) {
				XObj* obj = GAME_ITEMS->Get(i);
				if (kind == obj->GetDescKind()) {
					obj->mFlags &= ~0x10000ULL;
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
	s32 stage = GAME_STAGE;
	s32 id    = fn_8006F654(name);
	return id == stage;
}

// the event's object with the id, ids from 10000 up
static inline XObj* FindEvtObj(s16 id)
{
	return GAME_5D080 != NULL ? fn_8007F570(&GAME_5D080, id) : NULL;
}

static inline XObj* GetObj(s16 id)
{
	return GAME_OBJS[id];
}

// where the stage table placed the actor
static inline Vec GetPlacePos(const UnkActor3Place* place)
{
	Vec pos = Vec();
	pos.x   = place->mPos[0] / 100.0f;
	pos.y   = place->mPos[1] / 100.0f;
	pos.z   = place->mPos[2] / 100.0f;
	return pos;
}

// a particle set, name from its vtable
class CPtcl {
public:
	u8 _00[0x10]; // _00
	// _10 vtable

	virtual ~CPtcl() { } // _08
};

// one dust particle, its constructor and destructor are in another file
class CPtclDust3 {
public:
	u8 _00[0x4C]; // _00
	// _4C vtable

	virtual ~CPtclDust3(); // _08
};

// the set and the 50 particles it runs
struct UnkActor3Ptcl {
	CPtcl mPtcl;          // _000
	CPtclDust3 mDust[50]; // _014
};

extern "C" {
void fn_80024CA0(CPtcl* ptcl);
void fn_80024FAC(CPtcl* ptcl, void* particles, u32 size, u32 count);
}

// one of the actor's 8 slots, an object it carries at one of its joints, 0x68 bytes
struct UnkActor3Slot {
	HSD_JObj* mJoint; // _00
	XObj* mObj;       // _04
	u8 _08[0x30];     // _08
	Mtx mMtx;         // _38

	void Clear()
	{
		mObj   = NULL;
		mJoint = NULL;
	}
};

class XObjActor3 : public XObj {
public:
	CTpl mTpl;               // _980
	f32 _B70;                // _B70
	f32 _B74;                // _B74
	f32 _B78;                // _B78
	f32 _B7C;                // _B7C
	s32 _B80;                // _B80
	u8 _B84[0x4];            // _B84
	s32 _B88;                // _B88
	CStatus mStatus;         // _B8C
	s32 _B94[11];            // _B94
	f32 _BC0;                // _BC0
	f32 _BC4;                // _BC4
	f32 _BC8;                // _BC8
	Vec _BCC;                // _BCC
	f32 _BD8;                // _BD8
	CVec _BDC;               // _BDC
	s32 _BEC;                // _BEC
	f32 _BF0;                // _BF0
	f32 _BF4;                // _BF4
	u8 _BF8;                 // _BF8
	s32 _BFC;                // _BFC
	f32 _C00;                // _C00
	s32 _C04;                // _C04
	u8 _C08;                 // _C08
	UnkActor3Ptcl mPtcl;     // _C0C
	CTexObj mTex[2];         // _1BC0
	s32 _1DC0[6];            // _1DC0
	s32 mRumble;             // _1DD8
	s32 mRumbleTimer;        // _1DDC
	XObj* mHeld;             // _1DE0, the 0x40B object kind 8 holds
	s32 _1DE4;               // _1DE4
	s32 mSfx;                // _1DE8
	UnkActorSe mSe;          // _1DEC
	UnkActor3Slot mSlots[8]; // _1DFC

	virtual void vf148(); // _148

	const UnkActor3Place* GetPlace() const { return (const UnkActor3Place*)_238; }

	// what the actor does each frame, by GetInfoKind, Update calls them through lbl_8022C330
	void Kind0(u32 frames);  // 0x8011529C
	void Kind1(u32 frames);  // 0x801151E0
	void Kind2(u32 frames);  // 0x80114CC8
	void Kind3(u32 frames);  // 0x801147C0
	void Kind4(u32 frames);  // 0x801146F8
	void Kind5(u32 frames);  // 0x801146F4
	void Kind6(u32 frames);  // 0x8011463C
	void Kind7(u32 frames);  // 0x80114540
	void Kind8(u32 frames);  // 0x80114060
	void Kind9(u32 frames);  // 0x80111A78
	void Kind10(u32 frames); // 0x801115FC
	void Kind11(u32 frames); // 0x8010FF94
	void Kind12(u32 frames); // 0x8010F318
	void Kind13(u32 frames); // 0x8010D65C
	void Kind14(u32 frames); // 0x8010D114
	void Kind15(u32 frames); // 0x8010CE80
	void Kind16(u32 frames); // 0x8010C304
	void Kind17(u32 frames); // 0x8010BF7C
	void Kind18(u32 frames); // 0x8010BC0C
	void Kind19(u32 frames); // 0x8010BC08
};

// the script's variables
extern "C" s32 lbl_80367A2C[];

// the sound table of kind 0x31B
extern "C" const UnkActorSeEntry lbl_8022C178[];

// what each kind does, by GetInfoKind
typedef void (XObjActor3::*XObjActor3Kind)(u32 frames);
static XObjActor3Kind lbl_8022C330[20] = {
	&XObjActor3::Kind0,  &XObjActor3::Kind1,  &XObjActor3::Kind2,  &XObjActor3::Kind3,  &XObjActor3::Kind4,
	&XObjActor3::Kind5,  &XObjActor3::Kind6,  &XObjActor3::Kind7,  &XObjActor3::Kind8,  &XObjActor3::Kind9,
	&XObjActor3::Kind10, &XObjActor3::Kind11, &XObjActor3::Kind12, &XObjActor3::Kind13, &XObjActor3::Kind14,
	&XObjActor3::Kind15, &XObjActor3::Kind16, &XObjActor3::Kind17, &XObjActor3::Kind18, &XObjActor3::Kind19,
};

extern "C" {

s32 lbl_8065B2F0;
s32 lbl_8065B2F4;

void fn_8010904C(XObjActor3* self);
void fn_801095E8(XObjActor3* self);
void fn_8010AD88(XObjActor3* self);
void fn_8010FB1C(XObjActor3* self);
void fn_8010B394(XObjActor3* self);
void fn_8010B528(XObjActor3* self);
void fn_8010AF74(XObjActor3* self);
void fn_8010B7B4(XObjActor3* self, u32 frames);
void fn_80024D84(CPtcl* ptcl, u32 frames);

void XObjActor3::Kind0(u32 frames)
{
}

void XObjActor3::Kind1(u32 frames)
{
	switch (_B94[0]) {
	case 0:
		break;
	case 1:
		fn_800661E4(GAME_EVTMGR, GAME_ACTORS->Get(_258), 1, 1);
		_B94[0] = 0;
		break;
	case 2:
		fn_800661E4(GAME_EVTMGR, GAME_ACTORS->Get(_258), 0, 1);
		_B94[0] = 0;
		break;
	}
}

void XObjActor3::Kind4(u32 frames)
{
	_B88++;
	f32 s     = sin(0.017453292f * ((_B88 * 6) % 360));
	f32 swing = 0.25f;
	if (GetDescKind() == 0x437) {
		mMoveRotate.x = s * swing;
	} else {
		mMoveRotate.z = s * swing;
	}
}

void XObjActor3::Kind5(u32 frames)
{
}

// NONMATCHING: register allocation
void XObjActor3::Kind6(u32 frames)
{
	s16 id;
	s16 num;
	s32 i;
	s32 count = 0;
	switch (_B94[0]) {
	case 0:
		break;
	case 1:
		for (i = 0; i < 0x100; i++) {
			UnkPlayerItem* items = GAME_PLAYER->mItems;
			id                   = items[i].mId;
			num                  = items[i].mNum;
			if (fn_8006E44C(id, 4) && id >= 0 && num > 0) {
				count++;
			}
		}
		GAME_ITEMCOUNT = count;
		_B94[0]        = 0;
		break;
	}
}

// holds the 0x40B item at its first joint while flag 0x100 is up
// NONMATCHING: the loop and the joint's vectors take each other's registers
void XObjActor3::Kind8(u32 frames)
{
	Mtx m;
	Mtx t;
	Mtx a;
	CJObj joint;
	if (mStatus.mFlags & 0x100) {
		CVec pos;
		s32 path[1] = { 0 };
		joint.Attach(GetJointFromRoot(mJObj, 1, path));
		pos      = GetJObjPos(joint.mJObj);
		s32 kind = -1;
		if (mStatus.mFlags & 0x100) {
			kind = 0x40B;
		}
		if (kind >= 0) {
			for (s32 i = 0; i < 0x120; i++) {
				XObj* obj = GAME_ITEMS->Get(i);
				if (kind == obj->GetDescKind()) {
					PSMTXCopy(HSD_JObjGetMtxPtr(joint.mJObj), m);
					PSMTXTrans(t, 0.0f, -14.0f, 0.0f);
					PSMTXConcat(m, t, a);
					HSD_JObj* jobj = obj->mJObj;
					MtxPtr mp      = a;
					HSD_JObjCopyMtx(jobj, mp);
					if (mFlags & 0x10000) {
						obj->mFlags |= 0x10000ULL;
					} else {
						obj->mFlags &= ~0x10000ULL;
					}
				}
			}
		}
	}
}

void XObjActor3::Kind7(u32 frames)
{
	switch (_B94[0]) {
	case 0:
		break;
	case 1:
		if (_B94[1] != 0) {
			fn_80068BC0(GAME_EVTMGR);
		} else {
			fn_80068BAC(GAME_EVTMGR);
		}
		_B94[0] = 0;
		break;
	case 2:
		if (_B94[1] != 0) {
			fn_80068C10(GAME_EVTMGR);
			fn_80068BE8(GAME_EVTMGR);
			fn_80068BC0(GAME_EVTMGR);
		} else {
			fn_80068BFC(GAME_EVTMGR);
			fn_80068BD4(GAME_EVTMGR);
			fn_80068BAC(GAME_EVTMGR);
		}
		_B94[0] = 0;
		break;
	}
}

// falls from above the robot, knocks it down on a hit, blinks out on the floor
void XObjActor3::Kind10(u32 frames)
{
	UnkPlayer* player = GAME_PLAYER;
	Vec pos           = player->mMoveTranslate;
	switch (_B80) {
	case 0:
		if (mStatus.mFlags & 4) {
			break;
		}
		mMoveTranslate.x = pos.x - (rand() % 3600) / 100.0f + 18.0f;
		mMoveTranslate.y = 64.0f;
		mMoveTranslate.z = pos.z - (rand() % 3600) / 100.0f + 18.0f;
		mFlags |= 0x10000;
		mFlags |= 4;
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
		if (dist < 8.0f && !fn_800A0000(player) && !fn_800665E0(GAME_EVTMGR)) {
			player->vf138(&pos, 10.0f);
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
			mFlags |= 0x10000;
		} else {
			mFlags &= ~0x10000ULL;
		}
		if (--_B88 < 0) {
			mFlags &= ~0x10000ULL;
			mFlags &= ~4ULL;
			mStatus.mFlags |= 4;
			_B80 = 0;
		}
		break;
	}
}

// whether the robot stands where the actor can reach it
BOOL fn_8011132C(XObjActor3* self)
{
	UnkPlayer* player = GAME_PLAYER;
	Vec pos           = player->mMoveTranslate;
	if (!(self->mStatus.mFlags & 4)) {
		if (IsPlayerAway(player, pos)) {
			return FALSE;
		}
		if (pos.y >= self->mMoveTranslate.y - 16.0f && pos.y <= self->mMoveTranslate.y + 16.0f) {
			if (self->_B7C == 1.5707964f) {
				if (self->mMoveTranslate.x < pos.x && pos.x < self->_BC4) {
					return TRUE;
				}
				if (self->mMoveTranslate.x > pos.x && pos.x > self->_BC0) {
					if (IsLongReachAnim(player->mAnimId)) {
						if (self->mMoveTranslate.x > pos.x && pos.x > self->mMoveTranslate.x - 32.0f) {
							return TRUE;
						}
					} else {
						if (self->mMoveTranslate.x > pos.x && pos.x > self->mMoveTranslate.x - 12.0f) {
							return TRUE;
						}
					}
				}
			} else {
				if (self->mMoveTranslate.x > pos.x && pos.x > self->_BC0) {
					return TRUE;
				}
				if (self->mMoveTranslate.x < pos.x && pos.x < self->_BC4) {
					if (IsLongReachAnim(player->mAnimId)) {
						if (self->mMoveTranslate.x < pos.x && pos.x < self->mMoveTranslate.x + 32.0f) {
							return TRUE;
						}
					} else {
						if (self->mMoveTranslate.x < pos.x && pos.x < self->mMoveTranslate.x + 12.0f) {
							return TRUE;
						}
					}
				}
			}
		}
	}
	return FALSE;
}

// NONMATCHING: register allocation
void fn_8010FE68(s32 n)
{
	s32 count = 0;
	u8* data;
	s32 size;
	size = GAME_ACTORS->mSize;
	data = GAME_ACTORS->mData;
	for (s32 i = 0; i < 48; i++) {
		XObjActor3* obj = (XObjActor3*)(data + i * size);
		if (GetInfoKind(obj->mInfo) == 0xB) {
			if (count == n && (obj->mFlags & 1)) {
				if (obj->_B80 == 0) {
					obj->mStatus.mFlags &= ~4;
				}
				return;
			}
			count++;
		}
	}
}

// starts the n-th kind 0xB actor
// NONMATCHING: register allocation
void fn_8010FDA0(s32 n)
{
	u8* data;
	XObjActor3* obj;
	s32 size;
	s32 count = 0;
	s32 i;
	size = GAME_ACTORS->mSize;
	data = GAME_ACTORS->mData;
	for (i = 0; i < 48; i++) {
		obj = (XObjActor3*)(data + i * size);
		if (GetInfoKind(obj->mInfo) == 0xB) {
			if (count == n && (obj->mFlags & 1) && (obj->mFlags & 0x10000)) {
				fn_8010FB1C(obj);
				return;
			}
			count++;
		}
	}
}

// jumps the actor onto its holder, at its angle and distance around it
void fn_8010FB1C(XObjActor3* self)
{
	XObj* holder = GAME_HOLDER;
	if ((holder->mFlags & 1) && (holder->mFlags & 0x10000) && self->_B80 == 1) {
		f32 s          = sin(self->_BC8 + HSD_JObjGetRotationY(holder->mJObj));
		f32 x          = self->_BD8 * s;
		f32 y          = self->_BCC.y;
		f32 c          = cos(self->_BC8 + HSD_JObjGetRotationY(holder->mJObj));
		f32 z          = self->_BD8 * c;
		self->_B80     = 2;
		self->_B78     = 6.0f;
		HSD_JObj* jobj = self->mJObj;
		JObjClearFlags(jobj, 0x3800000);
		HSD_JObjSetMtxDirty(jobj);
		self->mMoveTranslate.x = x + HSD_JObjGetTranslationX(holder->mJObj);
		self->mMoveTranslate.y = y + 8.0f + 10.0f;
		self->mMoveTranslate.z = z + HSD_JObjGetTranslationZ(holder->mJObj);
		self->SetMoveTranslate(&self->mMoveTranslate, 0);
		self->_B7C
		    = (f32)atan2(holder->mMoveTranslate.x - self->mMoveTranslate.x, holder->mMoveTranslate.z - self->mMoveTranslate.z) + 3.1415927f;
		fn_8001E904(GAME_SOUND, 0x160021, 0xFF);
	}
}

// NONMATCHING: register allocation
void fn_8010F208(void)
{
	XObjActor3* obj;
	u8* data;
	s32 size;
	size = GAME_ACTORS->mSize;
	data = GAME_ACTORS->mData;
	for (s32 i = 0; i < 48; i++) {
		obj = (XObjActor3*)(data + i * size);
		if (GetInfoKind(obj->mInfo) == 0xC && (obj->mFlags & 1)) {
			if (obj->_B80 == 0) {
				obj->mStatus.mFlags &= ~4;
			}
			return;
		}
	}
}

// jumps the first kind 0xC actor onto the holder's middle
// NONMATCHING: register allocation
void fn_8010EF70(void)
{
	u8* data;
	s32 size;
	size = GAME_ACTORS->mSize;
	data = GAME_ACTORS->mData;
	for (s32 i = 0; i < 48; i++) {
		XObjActor3* obj = (XObjActor3*)(data + i * size);
		if (GetInfoKind(obj->mInfo) == 0xC && (obj->mFlags & 1) && (obj->mFlags & 0x10000)) {
			XObj* holder = GAME_HOLDER;
			if ((holder->mFlags & 1) && (holder->mFlags & 0x10000) && obj->_B80 == 1) {
				obj->_B80      = 2;
				obj->_B78      = 6.0f;
				HSD_JObj* jobj = obj->mJObj;
				JObjClearFlags(jobj, 0x3800000);
				HSD_JObjSetMtxDirty(jobj);
				f32 ofs               = 0.0f; // the holder's middle, where fn_8010FB1C goes round it
				obj->mMoveTranslate.x = ofs + HSD_JObjGetTranslationX(holder->mJObj);
				obj->mMoveTranslate.y = 18.0f;
				obj->mMoveTranslate.z = ofs + HSD_JObjGetTranslationZ(holder->mJObj);
				obj->SetMoveTranslate(&obj->mMoveTranslate, 0);
				obj->_B7C = (f32)atan2(holder->mMoveTranslate.x - obj->mMoveTranslate.x, holder->mMoveTranslate.z - obj->mMoveTranslate.z)
				          + 3.1415927f;
				obj->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				fn_8001E904(GAME_SOUND, 0x160021, 0xFF);
			}
			return;
		}
	}
}

// NONMATCHING: register allocation
void fn_8010D390(s32 mode)
{
	if (mode == 0) {
		for (s32 i = 0; i < 48; i++) {
			XObjActor3* obj = (XObjActor3*)GAME_ACTORS->Get(i);
			if (obj->GetDescKind() == 0x3E0 && (obj->mFlags & 1)) {
				obj->mStatus.mFlags &= ~4;
				obj->_B80    = 0;
				lbl_8065B2F0 = 0;
			}
		}
	}
	if (mode == 2) {
		for (s32 i = 0; i < 48; i++) {
			XObjActor3* obj = (XObjActor3*)GAME_ACTORS->Get(i);
			if (obj->GetDescKind() == 0x3E0 && (obj->mFlags & 1) && obj->mStatus.Check(4)) {
				obj->mStatus.mFlags &= ~4;
				obj->_B80    = 20;
				lbl_8065B2F0 = 0;
			}
		}
	} else {
		u8* data;
		s32 size;
		size = GAME_ACTORS->mSize;
		data = GAME_ACTORS->mData;
		for (s32 i = 0; i < 48; i++) {
			XObjActor3* obj = (XObjActor3*)(data + i * size);
			if (obj->GetDescKind() == 0x3E0) {
				if (!(obj->mFlags & 1)) {
					return;
				}
				if (obj->mStatus.Check(4)) {
					obj->mStatus.mFlags &= ~4;
					obj->_B80 = 10;
					return;
				}
			}
		}
	}
}

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
		XObj* obj = GAME_POOL2->Get(i);
		if ((obj->mFlags & 0x10000) && !fn_800EBDB0(obj)) {
			count++;
		}
	}
	_C04 = count;
}

void XObjActor3::Kind15(u32 frames)
{
	switch (_B80) {
	case 0:
		if (mStatus.mFlags & 4) {
			break;
		}
		_B80++;
		SetAnim(1, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		if (++lbl_8065B2F4 >= 10) {
			lbl_8065B2F4 = 0;
		}
		if (!fn_8001E6C8(GAME_SOUND, 0x127)) {
			fn_8001E904(GAME_SOUND, 0x127, 0xFF);
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
		mStatus.mFlags |= 4;
		_B80 = 0;
		break;
	}
}

// puts every waiting 0x43A actor at the point
// NONMATCHING: register allocation
void fn_8010CDDC(f32 x, f32 y, f32 z)
{
	XObjActor3* obj;
	for (s32 i = 0; i < 48; i++) {
		obj = (XObjActor3*)GAME_ACTORS->Get(i);
		switch (obj->GetDescKind()) {
		case 0x43A:
			if ((obj->mFlags & 1) && obj->mStatus.Check(4)) {
				obj->mStatus.mFlags &= ~4;
				obj->mMoveTranslate.x = x;
				obj->mMoveTranslate.y = y;
				obj->mMoveTranslate.z = z;
			}
			break;
		}
	}
}

// plays anim n on the first kind 0x10 actor and sizes it for it
void fn_8010C0F0(s32 n)
{
	u8* data;
	s32 size;
	size = GAME_ACTORS->mSize;
	data = GAME_ACTORS->mData;
	for (s32 i = 0; i < 48; i++) {
		XObjActor3* obj = (XObjActor3*)(data + i * size);
		if (GetInfoKind(obj->mInfo) == 0x10 && (obj->mFlags & 1)) {
			obj->SetAnim(n, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			obj->mStatus.mFlags &= ~4;
			obj->mStatus.mFlags &= ~0x10;
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

// puts the first kind 0x10 actor back to its rest pose, small
void fn_8010BF80(void)
{
	u8* data;
	s32 size;
	size = GAME_ACTORS->mSize;
	data = GAME_ACTORS->mData;
	for (s32 i = 0; i < 48; i++) {
		XObjActor3* obj = (XObjActor3*)(data + i * size);
		if (GetInfoKind(obj->mInfo) == 0x10 && (obj->mFlags & 1)) {

			obj->_B80 = 0;
			obj->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			obj->mStatus.mFlags |= 4;
			obj->mStatus.mFlags |= 0x10;
			obj->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			obj->mStatus.mFlags &= ~4;
			obj->mStatus.mFlags &= ~0x10;
			obj->SetScale(0.1f, 0.1f, 0.1f, 0);
			obj->_B80 = 0;
			return;
		}
	}
}

// slides out to the left from about ten to noon and back in the afternoon
void XObjActor3::Kind18(u32 frames)
{
	f32 time = GAME_DAYTIME;
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

void XObjActor3::Kind17(u32 frames)
{
}

void XObjActor3::Kind19(u32 frames)
{
}

void fn_8010B528(XObjActor3* self)
{
	switch (self->_B94[0]) {
	case 1:
		if (self->_B94[1] != 0) {
			self->mStatus.mFlags &= ~4;
			self->_BC0 = self->_B94[2] / 100.0f;
			self->_BC4 = self->_B94[3] / 100.0f;
		} else {
			self->mStatus.mFlags |= 4;
			self->_BC0 = 0.0f;
			self->_BC4 = 0.0f;
			self->_B80 = 0;
		}
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	case 2:
		if ((self->mStatus.mFlags & 0x80) && !(self->mStatus.mFlags & 4)) {
			lbl_80367A2C[self->_B94[1]] = 1;
		}
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	case 3:
		self->mStatus.mFlags &= ~0x80;
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	case 4:
		UnkPlayer* player = GAME_PLAYER;
		if (!IsBusyAnim(player->mAnimId) && player->_95D4 < 0) {
			lbl_80367A2C[self->_B94[1]] = 1;
		}
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	}
}

// carries the robot while it rides the actor, at the actor's first joint
// NONMATCHING: the robot and the joint's vectors take each other's registers
void fn_8010B7B4(XObjActor3* self, u32 frames)
{
	CJObj joint;
	UnkPlayer* player = GAME_PLAYER;
	if ((self->mStatus.mFlags & 0x40) && (player->_0980 & 0x40000000)) {
		Mtx scaleMtx;
		Mtx rotMtx;
		Mtx transMtx;
		Mtx mtx;
		Mtx m;
		s32 path[2] = { 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 2, path));
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

// the script's commands to the actor, _B94 is the command and _B98 its argument
void fn_8010B394(XObjActor3* self)
{
	switch (self->_B94[0]) {
	case 0:
		break;
	case 1:
		if (self->_B94[1] != 0) {
			self->_BEC = 3;
		} else {
			self->_BEC = 4;
		}
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	case 2:
		if (self->_B94[1] != 0) {
			self->_BF8 = 1;
		} else {
			self->_BF8 = 0;
		}
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	case 3:
		self->_BF4    = 60.0f * (self->_B94[1] / 100.0f);
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	case 4:
		lbl_80367A2C[self->_B94[1]] = fn_8006697C(self->_BF4);
		self->_B94[0]               = 0;
		self->_B94[1]               = 0;
		break;
	case 5:
		if (self->_B94[1] != 0) {
			self->_BFC = 3;
		} else {
			self->_BFC = 4;
		}
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	case 6:
		self->_C04    = self->_B94[1];
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	case 7:
		lbl_80367A2C[self->_B94[1]] = self->_C04;
		self->_B94[0]               = 0;
		self->_B94[1]               = 0;
		break;
	}
}

void fn_8010AD88(XObjActor3* self)
{
	self->_B70           = 0.0f;
	self->_B74           = 0.0f;
	self->_B78           = 0.0f;
	self->_B7C           = 0.0f;
	self->_B80           = 0;
	self->_B88           = 0;
	self->mStatus.mFlags = 0;
	self->mStatus.mFlags |= 4;
	self->_B94[0]  = 0;
	self->_B94[1]  = 0;
	self->_B94[2]  = 0;
	self->_B94[3]  = 0;
	self->_B94[4]  = 0;
	self->_B94[5]  = 0;
	self->_B94[6]  = 0;
	self->_B94[7]  = 0;
	self->_B94[8]  = 0;
	self->_B94[9]  = 0;
	self->_B94[10] = 0;
	self->_BC0     = 0.0f;
	self->_BC4     = 0.0f;
	self->_BC8     = 0.0f;
	Vec zero       = { 0.0f, 0.0f, 0.0f };
	self->_BCC     = zero;
	for (int i = 0; i < 8; i++) {
		self->mSlots[i].Clear();
	}
	if (self->_258 == 0) {
		lbl_8065B2F0 = 0;
		lbl_8065B2F4 = 0;
	}
	self->SetColor(1, 0, 0, 0, 0.0f);
	self->_BDC.x = 0.0f;
	self->_BDC.y = 0.0f;
	self->_BDC.z = 0.0f;
	self->_BEC   = 0;
	self->_BF0   = 0.0f;
	self->_BF4   = 0.0f;
	self->_BF8   = 0;
	self->_BFC   = 0;
	self->_C00   = 0.0f;
	self->_C04   = 0;
	self->_C08   = 0;
	if (self->GetDescKind() == 0x31B) {
		fn_80024FAC(&self->mPtcl.mPtcl, self->mPtcl.mDust, 0x50, 0x32);
		self->mSe.mTable = lbl_8022C178;
	}
	self->_1DC0[0]     = 0;
	self->_1DC0[1]     = 0;
	self->_1DC0[2]     = 0;
	self->_1DC0[3]     = 0;
	self->_1DC0[4]     = 0;
	self->_1DC0[5]     = 0;
	self->mRumble      = 0;
	self->mRumbleTimer = 0;
	self->mHeld        = NULL;
	self->_1DE4        = 0;
	self->mSfx         = -1;
}

// the robot gets on the actor at its first joint, or off it
// NONMATCHING: the robot and the joint's vectors take each other's registers
void fn_8010AF74(XObjActor3* self)
{
	CJObj joint;
	UnkPlayer* player = GAME_PLAYER;
	switch (self->_B94[0]) {
	case 1:
		switch (self->_B94[1]) {
		case 1:
			self->mStatus.mFlags |= 0x40;
			player->_0980 = 0;
			player->_0980 |= 0x40000000;
			player->_12DF4 = 0;
			break;
		default:
			self->mStatus.mFlags &= ~0x40;
			s32 path[2] = { 0, 0 };
			joint.Attach(GetJointFromRoot(self->mJObj, 2, path));
			CVec pos(GetJObjPos(joint.mJObj));
			player->SetMoveTranslate(&pos, 0);
			player->_0980 = 0;
			player->_0980 |= 0x20000000;
			player->_0980 |= 7;
			player->_12DF4 = 1;
			HSD_JObjClearUserMtx(player->mJObj);
			break;
		}
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	}
}

// the event commands: 100 carries another object at one of its joints, or lets go of all
// of them, anything else is kept for the kind's function to read
// NONMATCHING: argv[1] and the 0x40B item loop take other registers, as in game/xobjspider
s32 fn_80108794(XObjActor3* self, s32 argc, s32* argv)
{
	switch (argv[0]) {
	case 100:
		if (argv[1] == 0) {
			self->mStatus.mFlags &= ~2;
			for (u32 i = 0; i < 8; i++) {
				if (self->mSlots[i].mObj != NULL) {
					HSD_JObjClearUserMtx(self->mSlots[i].mObj->mJObj);
					self->mSlots[i].mObj   = NULL;
					self->mSlots[i].mJoint = NULL;
				}
			}
		} else {
			u32 i;
			for (i = 0; self->mSlots[i].mObj != NULL && i < 8; i++) { }
			if (i < 8) {
				OSReport("id = %d, argv[1]=%d\n", i);
				self->mStatus.mFlags |= 2;
				if (argv[1] >= 10000) {
					self->mSlots[i].mObj = FindEvtObj(argv[1]);
				} else {
					self->mSlots[i].mObj = GetObj(argv[1]);
				}
				if (self->mSlots[i].mObj == NULL) {
					OSReport("NULL===============\n");
				}
				s32 path[8];
				s32 depth = argv[2];
				for (s32 j = 0; j < depth; j++) {
					path[j] = argv[3 + j];
				}
				self->mSlots[i].mJoint = GetJointByPath(self->mJObj, path, depth);
				Mtx t;
				XObj* obj = self->mSlots[i].mObj;
				PSMTXTrans(t, obj->mMoveTranslate.x, obj->mMoveTranslate.y, obj->mMoveTranslate.z);
				PSMTXCopy(HSD_JObjGetMtxPtr(self->mSlots[i].mJoint), self->mSlots[i].mMtx);
				Vec scale;
				HSD_MtxGetScale(self->mSlots[i].mMtx, &scale);
				scale.x = fn_800621E0(self->mSlots[i].mObj).x / scale.x;
				scale.y = fn_800621E0(self->mSlots[i].mObj).y / scale.y;
				scale.z = fn_800621E0(self->mSlots[i].mObj).z / scale.z;
				Mtx s;
				PSMTXScale(s, scale.x, scale.y, scale.z);
				Mtx m;
				PSMTXConcat(self->mSlots[i].mMtx, s, m);
				PSMTXConcat(m, t, m);
				HSD_JObj* joint = self->mSlots[i].mObj->mJObj;
				MtxPtr mp       = m;
				HSD_JObjCopyMtx(joint, mp);
			}
		}
		break;
	default:
		self->_B94[0]  = argv[0];
		self->_B94[1]  = argv[1];
		self->_B94[2]  = argv[2];
		self->_B94[3]  = argv[3];
		self->_B94[4]  = argv[4];
		self->_B94[5]  = argv[5];
		self->_B94[6]  = argv[6];
		self->_B94[7]  = argv[7];
		self->_B94[8]  = argv[8];
		self->_B94[9]  = argv[9];
		self->_B94[10] = 0;
		switch (self->GetInfoKind()) {
		case 9:
			fn_8010B528(self);
			break;
		case 0x13:
			fn_8010AF74(self);
			break;
		case 8:
			switch (self->_B94[0]) {
			case 0:
				break;
			case 1:
				s32 kind = -1;
				if (self->mStatus.Check(0x100)) {
					kind = 0x40B;
					self->mStatus.mFlags &= ~0x100;
				}
				HideItems(kind);
				switch (self->_B94[1]) {
				case 0x40B:
					for (s32 i = 0; i < 0x120; i++) {
						XObj* obj = GAME_ITEMS->Get(i);
						if (self->_B94[1] == obj->GetDescKind()) {
							self->mHeld = obj;
							obj->mFlags |= 0x10000ULL;
							switch (self->_B94[1]) {
							case 0x40B:
								self->mStatus.mFlags |= 0x100;
								obj->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
								break;
							}
						}
					}
					break;
				default:
					if (self->mHeld != NULL) {
						HSD_JObjClearUserMtx(self->mHeld->mJObj);
						self->mHeld = NULL;
					}
					break;
				}
				self->_B94[0] = 0;
				break;
			}
			break;
		}
		break;
	}
	return 0;
}

void fn_8010AD54(XObjActor3* self, u32 id)
{
	fn_800929C4(self, id);
	fn_8010AD88(self);
}

void fn_8010AD20(XObjActor3* self)
{
	fn_800928E8(self);
	fn_8010AD88(self);
}

void fn_8010AD00(XObjActor3* self)
{
	fn_8010AD88(self);
}

void fn_8010A500(XObjActor3* self)
{
	switch (self->GetDescKind()) {
	case 0x149:
	case 0x42B:
		self->mTpl.Close();
		break;
	}
	fn_800911C0(self);
}

// the per-kind script commands, the kind's own function, the carried objects
void fn_80109E80(XObjActor3* self, u32 frames)
{
	switch (self->GetInfoKind()) {
	case 8:
		self->mStatus.mFlags |= 4;
		break;
	case 9:
		self->vf148();
		if (GAME->mFlags & 0x400) {
			return;
		}
		break;
	case 10:
		if (GAME->mFlags & 0x400) {
			return;
		}
		switch (self->_B94[0]) {
		case 1:
			self->mStatus.mFlags &= ~4;
			self->_B94[0] = 0;
			self->_B94[1] = 0;
			break;
		}
		break;
	case 11:
	case 12:
		if (GAME->mFlags & 0x400) {
			return;
		}
		switch (self->_B94[0]) {
		case 1:
			if (self->_B94[1] != 0) {
				self->mStatus.mFlags &= ~4;
			} else {
				self->mStatus.mFlags |= 4;
				self->_B80 = 0;
			}
			self->_B94[0] = 0;
			self->_B94[1] = 0;
			break;
		case 2:
			self->_B80    = 2;
			self->_B78    = self->_B94[2] / 100.0f;
			self->_B94[0] = 0;
			self->_B94[1] = 0;
			break;
		}
		break;
	case 13:
		switch (self->mRumble) {
		case 0:
			break;
		case 1:
			if (GAME->mFlags & 0x800000000ULL) {
				GAME->StartRumble();
				self->mRumbleTimer = 24 / frames;
				self->mRumble++;
			} else {
				self->mRumble = 0;
			}
			break;
		case 2:
			if (--self->mRumbleTimer < 0) {
				UnkActor3Game* game = GAME;
				game->StopRumble();
				self->mRumble = 0;
			}
			break;
		}
		if (GAME->mFlags & 0x400) {
			return;
		}
		switch (self->_B94[0]) {
		case 1:
			self->mStatus.mFlags &= ~4;
			self->_B94[0] = 0;
			self->_B94[1] = 0;
			break;
		}
		break;
	case 14:
		if (GAME->mFlags & 0x400) {
			return;
		}
		fn_8010B394(self);
		break;
	}
	if (self->mStatus.mFlags & 0x10) {
		return;
	}
	(self->*lbl_8022C330[self->GetInfoKind()])(frames);
	if (!(self->mStatus.mFlags & 4)) {
		self->SetRotateY(self->_B7C, 0.2f);
		self->SetMoveTranslate(&self->mMoveTranslate, 0);
	}
	fn_80090ED4(self, frames);
	if (self->GetDescKind() == 0x31B) {
		fn_80024D84(&self->mPtcl.mPtcl, frames);
	}
	if (self->GetDescKind() == 0x2C3) {
		fn_8010B7B4(self, frames);
	}
	if (self->mStatus.mFlags & 2) {
		for (u32 i = 0; i < 8; i++) {
			if (self->mSlots[i].mJoint != NULL && self->mSlots[i].mObj != NULL) {
				Mtx t;
				XObj* obj = self->mSlots[i].mObj;
				PSMTXTrans(t, obj->mMoveTranslate.x, obj->mMoveTranslate.y, obj->mMoveTranslate.z);
				PSMTXCopy(HSD_JObjGetMtxPtr(self->mSlots[i].mJoint), self->mSlots[i].mMtx);
				Vec scale;
				HSD_MtxGetScale(self->mSlots[i].mMtx, &scale);
				scale.x = fn_800621E0(self->mSlots[i].mObj).x / scale.x;
				scale.y = fn_800621E0(self->mSlots[i].mObj).y / scale.y;
				scale.z = fn_800621E0(self->mSlots[i].mObj).z / scale.z;
				Mtx s;
				PSMTXScale(s, scale.x, scale.y, scale.z);
				Mtx m;
				PSMTXConcat(self->mSlots[i].mMtx, s, m);
				PSMTXConcat(m, t, m);
				HSD_JObj* joint = self->mSlots[i].mObj->mJObj;
				MtxPtr mp       = m;
				HSD_JObjCopyMtx(joint, mp);
			}
		}
	}
}

void fn_80109E44(XObjActor3* self)
{
	if (!(self->mStatus.mFlags & 0x10)) {
		self->vfB0(0);
	}
}

// kind 0xE on stage02: the panel and the count of _C04 in two digits over it, faded by _C00
// NONMATCHING: the colours, the texture matrix pointer and the loop count take other registers
void fn_8010904C(XObjActor3* self)
{
	f32 alpha = self->_C00;
	if (alpha <= 0.0f) {
		return;
	}
	if (!IsStage("stage02")) {
		return;
	}
	if (!self->mTpl.mBound) {
		return;
	}
	CTexObj tex;
	tex       = self->mTpl.mTex.Init(&((TPLPalette*)self->mTpl.mBuffer)->descriptorArray[9], 0);
	f32 scale = 1.0f;
	f32 w     = scale * GXGetTexObjWidth(&tex);
	u16 h     = GXGetTexObjHeight(&tex);
	fn_8019AFA0(GS_TEV, 0);
	fn_80197800(lbl_8025C400, &tex, -1, -1, -1, -1, 320.0f, 40.0f, 0.0f, w, TexSize(h, 1.0f), 0.0f, 0.0f, 0.0f);
	fn_8019AFA0(GS_TEV, 4);
	tex    = self->mTpl.mTex.Init(&((TPLPalette*)self->mTpl.mBuffer)->descriptorArray[1], 0);
	f32 dw = 1.4f * TexSize(GXGetTexObjWidth(&tex) / 11, 1.0f);
	f32 x;
	f32 dh    = 1.4f * TexSize(GXGetTexObjHeight(&tex), 1.0f);
	x         = 110.0f - (1 - self->_C04 / 10) * 12;
	f32 count = self->_C04;
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
			fn_8019AFA0(GS_TEV, 0);
			fn_80197800(lbl_8025C400, &tex, c0, c0, c1, c1, 7.0f * dw + (x - i * (dw - 8.0f)) - 32.0f, 76.0f, 0.0f, dw, dh, 0.0f, 0.0f,
			            0.0f);
			fn_8019AFA0(GS_TEV, 4);
		}
	}
}

void fn_80109DD8(XObjActor3* self)
{
	switch (self->GetInfoKind()) {
	case 0xE:
		fn_801095E8(self);
		fn_8010904C(self);
		break;
	}
}

void fn_80109008(XObjActor3* self)
{
	if (self->GetDescKind() == 0x31B) {
		fn_80024CA0(&self->mPtcl.mPtcl);
	}
}

void fn_80109004(XObjActor3* self)
{
}

void fn_80108728(XObjActor3* self, const Vec* pos, f32 f)
{
	if (self->GetInfoKind() == 1) {
		fn_8008A6C0(self, pos, f);
	} else if (!self->mStatus.Check(1)) {
		self->mStatus.mFlags |= 1;
	}
}

void fn_80108694(XObjActor3* self, s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate, f32 blend,
                 f32 blendRate)
{
	fn_800902A4(self, id, force, jointAnim, matAnim, shapeAnim, frame, rate, blend, blendRate);
	if (self->GetDescKind() == 0x31B) {
		if (self->mAnimId == 7) {
			self->mSfx = fn_8001E904(GAME_SOUND, 0xB0000, 0xFF);
		} else if (HSD_AudioSFXCheck(self->mSfx)) {
			HSD_AudioSFXKeyOff(self->mSfx);
		}
	}
}

} // extern "C"

// NONMATCHING: register allocation
void XObjActor3::vf148()
{
	if ((mFlags & 1) && (mFlags & 0x10000) && _258 == 3) {
		s32 id    = mAnimId;
		f32 frame = GetAnimFrame(mJObj);
		mSe.Play(id, fn_8001DF00(GAME_SOUND, &mMoveTranslate, 0), frame);
	}
}
