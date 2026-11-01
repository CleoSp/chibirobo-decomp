#ifndef _GAME_SCRC_SHARED_H
#define _GAME_SCRC_SHARED_H

#include "game/hsd_jobj.h"
#include "game/cparticle.h"
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

#endif
