#ifndef _GAME_SCR_H
#define _GAME_SCR_H

#include "Version.h"
#include "game/cjobj.h"

// the actors the stages place, game/scr*.cpp. class names come from their vtables, nothing
// else has a name in the dol, so the methods are extern "C" under their addresses. offsets
// are usa.
// CXObj, CXObjIntp and XObj are game/cobj.h's and game/obja.h's, copied because some of
// their virtuals take arguments there that the headers don't give them (CXObj's vf64, XObj's
// vfA4 and vfB0), so a file can include only one of these headers

class CObjArchive;
struct CJArchiveDesc;

#if defined(VERSION_JP)
#define CJARCHIVE_DESC_NUM 9
#else
#define CJARCHIVE_DESC_NUM 10
#endif

// see game/cobj.h
class CJArchive {
public:
	CObjArchive* mArchive;                    // _00
	s32 mState;                               // _04
	u32 mFlags;                               // _08
	s32 mResult;                              // _0C
	u32 mAramAddr;                            // _10
	u32 mAramSize;                            // _14
	HSD_Joint* mJoint;                        // _18
	HSD_AnimJoint** mAnimJoint;               // _1C
	HSD_MatAnimJoint** mMatAnimJoint;         // _20
	HSD_ShapeAnimJoint** mShapeAnimJoint;     // _24
	CJArchiveDesc* mDesc[CJARCHIVE_DESC_NUM]; // _28
	s32 mNum[CJARCHIVE_DESC_NUM + 1];         // _50
	// _7C vtable

	virtual ~CJArchive() { }                                                                               // _08 (weak)
	virtual void Clear();                                                                                  // _0C
	virtual BOOL ReadAsync(CObjArchive* archive, const char* path, u32 flags, u32 aramAddr, u32 aramSize); // _10
	virtual BOOL ReadCheck();                                                                              // _14
	virtual void Release();                                                                                // _18
};

// a value with a target and a per frame speed to step it there, see game/cobj.h
class CObjVec : public CVec {
public:
	CVec mTarget; // _10
	CVec mSpeed;  // _20
};

struct CXObjQueueEntry {
	HSD_JObj* mJObj; // _00
	u32 mType;       // _04
	f32 mValue;      // _08
};

// see game/cobj.h
class CXObj : public CJObj {
public:
	u32 mId;                    // _38
	s32 mLoadState;             // _3C
	u32 mModel;                 // _40
	u32 mLoadFlags;             // _44
	CJArchive mArchive;         // _48
	CObjVec mMoveTranslate;     // _C8
	CObjVec mMoveRotate;        // _F8
	CObjVec mMoveScale;         // _128
	s32 mAnimId;                // _158
	f32 mAnimRate;              // _15C
	f32 mLastAnimRate;          // _160
	u32 mColors[2];             // _164
	s32 mQueued;                // _16C
	CXObjQueueEntry mQueue[10]; // _170

	virtual void SetTranslateX(f32 x, s32 force);                                                                     // _0C
	virtual void SetTranslateY(f32 y, s32 force);                                                                     // _10
	virtual void SetTranslateZ(f32 z, s32 force);                                                                     // _14
	virtual void SetScaleX(f32 x, s32 force);                                                                         // _18
	virtual void SetScaleY(f32 y, s32 force);                                                                         // _1C
	virtual void SetScaleZ(f32 z, s32 force);                                                                         // _20
	virtual void SetRotationX(f32 x, s32 force);                                                                      // _24
	virtual void SetRotationY(f32 y, s32 force);                                                                      // _28
	virtual void SetRotationZ(f32 z, s32 force);                                                                      // _2C
	virtual void SetTranslate(const Vec& v, s32 force);                                                               // _30
	virtual void SetTranslate(f32 x, f32 y, f32 z, s32 force);                                                        // _34
	virtual void SetScale(const Vec& v, s32 force);                                                                   // _38
	virtual void SetScale(f32 x, f32 y, f32 z, s32 force);                                                            // _3C
	virtual void SetRotation(const Vec& v, s32 force);                                                                // _40
	virtual void SetRotation(f32 x, f32 y, f32 z, s32 force);                                                         // _44
	virtual void SetAnimRate(f32 rate, u32 mask);                                                                     // _48
	virtual BOOL ReadAsync(CObjArchive* archive, const char* path, u32 model, u32 flags, u32 aramAddr, u32 aramSize); // _4C
	virtual BOOL ReadCheck();                                                                                         // _50
	virtual void Release();                                                                                           // _54
	virtual BOOL StepRotate(u32 frames);                                                                              // _58
	virtual BOOL StepTranslate(u32 frames);                                                                           // _5C
	virtual BOOL StepScale(u32 frames);                                                                               // _60
	virtual void SetMoveTranslate(const Vec* pos, s32 force); // _64, 0x8002657C, puts it there now
	virtual void vf68();                                      // _68
	virtual void vf6C();                                      // _6C
	virtual void vf70();                                      // _70
	virtual void vf74();                                      // _74
	virtual void SetMoveDir(const Vec* dir, f32 speed);       // _78
	virtual void SetRotateY(f32 target, f32 speed);           // _7C
	virtual void SetAnim(s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate, f32 blend,
	                     f32 blendRate); // _80
	virtual void ClearAnim();            // _84
	virtual void UpdateAnim(u32 speed);  // _88
	virtual void ApplyTransform();       // _8C

	void Push(HSD_JObj* jobj, u32 type, f32 value);
};

// a CXObj with a second jobj it blends into, see game/cobj.h
class CXObjIntp : public CXObj {
public:
	CJObj mIntp;    // _1E8
	f32 mBlend;     // _220
	f32 mBlendRate; // _224

	virtual BOOL ReadCheck();               // _50
	virtual void Release();                 // _54
	virtual BOOL StepRotate(u32 frames);    // _58
	virtual BOOL StepTranslate(u32 frames); // _5C
	virtual BOOL StepScale(u32 frames);     // _60
	virtual void SetAnim(s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate, f32 blend,
	                     f32 blendRate); // _80
	virtual void UpdateAnim(u32 speed);  // _88
	virtual void ApplyTransform();       // _8C
};

extern "C" {
void HSD_JObjWalkTree(HSD_JObj* jobj, void (*cb)(HSD_JObj*, void*, u32), void* args);
}

// more jobj.h inlines, the assert lines are from the binary
inline f32 HSD_JObjGetTranslationX(HSD_JObj* jobj)
{
	JOBJ_H_ASSERT(1009, jobj);
	return jobj->translate.x;
}

inline f32 HSD_JObjGetTranslationZ(HSD_JObj* jobj)
{
	JOBJ_H_ASSERT(1033, jobj);
	return jobj->translate.z;
}

// HSD_JObjClearFlags, which the c header has as a function
inline void JObjClearFlags(HSD_JObj* jobj, u32 flags)
{
	JOBJ_H_ASSERT(1199, jobj);
	jobj->flags &= ~flags;
}

#define AOBJ_H_ASSERT(line, cond) ((cond) ? ((void)0) : __assert("aobj.h", line, #cond))

inline f32 HSD_AObjGetCurrFrame(HSD_AObj* aobj)
{
	AOBJ_H_ASSERT(0x95, aobj);
	return aobj->curr_frame;
}

// game/hsd_cobj.h has it too, a file includes only one of the two
inline f32 HSD_AObjGetEndFrame(HSD_AObj* aobj)
{
	AOBJ_H_ASSERT(0xAB, aobj);
	return aobj->end_frame;
}

// first anim object on the jobj, whichever kind it is
static inline HSD_AObj* GetAObj(HSD_JObj* jobj)
{
	HSD_AObj* aobj;
	if (jobj != NULL) {
		if ((aobj = fn_801908F8(jobj)) != NULL) {
			return aobj;
		}
		if ((aobj = fn_801905D4(jobj)) != NULL) {
			return aobj;
		}
		if ((aobj = fn_801902C4(jobj)) != NULL) {
			return aobj;
		}
	}
	return NULL;
}

// a CJArchive that counts the reads it has going, __vt__6CJAnim
class CJAnim : public CJArchive {
public:
	s32 mReading; // _80

	virtual void Clear();   // _0C
	virtual void Release(); // _18
};

struct XObjPoint;

// the stage table entry an object was placed from
struct XObjDesc {
	s32 mKind; // _00, which object
};

// what the object was placed from
struct XObjInfo {
	u8 _00[0x14]; // _00
	s16 mId;      // _14
	s16 mKind;    // _16
};

static inline s16 GetInfoKind(XObjInfo* info)
{
	if (info != NULL) {
		return info->mKind;
	}
	return 0;
}

// a color stepping to a target, 0 to 255 per channel
struct XObjColor {
	Vec mColor;  // _00
	Vec mTarget; // _0C
	Vec mSpeed;  // _18
};

struct XObjMsg {
	s32 mId; // _00
	s32 _04; // _04
};

struct XObjEvt {
	u8 _00[0x4];  // _00
	void* mEvt;   // _04
	u8 _08[0x60]; // _08
};

// __vt__4XObj, the object every placed thing in the house is
class XObj : public CXObjIntp {
public:
	u64 mFlags;             // _228
	u8 _230[0x8];           // _230
	void* _238;             // _238
	u8 _23C[0x4];           // _23C
	XObjInfo* mInfo;        // _240
	XObjDesc* mDesc;        // _244
	u8 _248[0x4];           // _248
	XObjPoint* mPoints;     // _24C
	u32 mPointMask;         // _250
	u32 mPointMaskOld;      // _254
	s32 _258;               // _258
	u8 _25C[0x6FC - 0x25C]; // _25C
	f32 mPointDist[32];     // _6FC
	f32 _77C;               // _77C
	f32 mNearDist;          // _780
	s32 mNearPoint;         // _784
	s32 _788;               // _788
	s32 _78C;               // _78C
	f32 _790;               // _790
	f32 _794;               // _794
	f32 _798;               // _798
	f32 _79C;               // _79C
	u8 _7A0[0x4];           // _7A0
	XObjEvt mEvt;           // _7A4
	s32 _80C;               // _80C
	s32 mMsgCount;          // _810
	XObjMsg mMsgs[16];      // _814
	f32 _894;               // _894
	f32 _898;               // _898
	f32 _89C;               // _89C
	u8 _8A0[0x10];          // _8A0
	u32 _8B0;               // _8B0
	XObjColor mColors[2];   // _8B4
	CJAnim mAnim;           // _8FC

	virtual void Init(u32 id);                                   // _90
	virtual void vf94();                                         // _94
	virtual void Reset();                                        // _98
	virtual BOOL vf9C(void* info);                               // _9C
	virtual void vfA0();                                         // _A0
	virtual void Update(u32 frames);                             // _A4
	virtual void vfA8();                                         // _A8
	virtual void vfAC();                                         // _AC
	virtual void vfB0(s32 arg);                                  // _B0
	virtual void vfB4();                                         // _B4
	virtual void vfB8();                                         // _B8
	virtual void vfBC(Vec* pos, f32 scale);                      // _BC
	virtual void vfC0();                                         // _C0
	virtual void vfC4();                                         // _C4
	virtual void vfC8();                                         // _C8
	virtual void vfCC();                                         // _CC
	virtual void vfD0();                                         // _D0
	virtual void vfD4();                                         // _D4
	virtual void vfD8(void* args);                               // _D8
	virtual void vfDC(s32 a, u8 b);                              // _DC
	virtual void vfE0(f32 alpha);                                // _E0
	virtual void vfE4(f32 alpha);                                // _E4
	virtual void vfE8();                                         // _E8
	virtual Vec GetCameraTarget();                               // _EC
	virtual void vfF0();                                         // _F0
	virtual void vfF4();                                         // _F4
	virtual void vfF8();                                         // _F8
	virtual void vfFC();                                         // _FC
	virtual s32 vf100(s32 a, s32 b);                             // _100
	virtual void vf104();                                        // _104
	virtual void vf108();                                        // _108
	virtual void vf10C();                                        // _10C
	virtual void vf110();                                        // _110
	virtual void vf114();                                        // _114
	virtual void vf118();                                        // _118
	virtual void vf11C();                                        // _11C
	virtual void vf120(s32 mode);                                // _120
	virtual void vf124();                                        // _124
	virtual s32 GetSoundPos();                                   // _128
	virtual void SetColor(s32 i, u8 r, u8 g, u8 b, f32 seconds); // _12C
	virtual void vf130();                                        // _130
	virtual BOOL vf134();                                        // _134
	virtual void vf138(const Vec* pos, f32 f);                   // _138
	virtual BOOL vf13C(s32 a, s32 b);                            // _13C
	virtual BOOL vf140(s32 a, s32 b);                            // _140
	virtual void vf144();                                        // _144

	// the stage table's id for the object, -1 when it wasn't placed from one
	s32 GetDescKind() const
	{
		if (mDesc != NULL) {
			return mDesc->mKind;
		}
		return -1;
	}
	s16 GetInfoKind() const
	{
		if (mInfo != NULL) {
			return ::GetInfoKind(mInfo);
		}
		return -1;
	}
};
#if !defined(VERSION_JP)
typedef char XObj_size_check[sizeof(XObj) == 0x980 ? 1 : -1];
#endif

// the XObj methods the actors call directly
extern "C" {
void fn_800902A4(XObj* obj, s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate, f32 blend,
                 f32 blendRate);                    // SetAnim
void fn_80090864(XObj* obj);                        // vfB8
void fn_8009047C(XObj* obj);                        // vfC8
void fn_8009056C(XObj* obj);                        // vfBC
void fn_80090ED4(XObj* obj, u32 frames);            // Update
void fn_800911C0(XObj* obj);                        // Release
BOOL fn_80091C80(XObj* obj, void* info);            // vf9C, loads the model
void fn_800928E8(XObj* obj);                        // Reset
void fn_800929C4(XObj* obj, u32 id);                // Init
void fn_8008A6C0(XObj* obj, const Vec* pos, f32 f); // vf138
}

// the sound player, at +0xE7E24 in the game object
struct CSound;

extern "C" {
s32 fn_8001DF00(CSound* sound, Vec* pos, s32 kind);
s32 fn_8001E904(CSound* sound, s32 sfx, u8 vol);
}

extern "C" u8 lbl_8025C400[];
#define GAME_SOUND  ((CSound*)(lbl_8025C400 + 0xE7E24))
#define GAME_EVTMGR (lbl_8025C400 + 0x8F38)

extern "C" BOOL fn_800665E0(void* mgr); // true while an event runs

// a pool of objects of one size, in the game object
struct UnkObjPool {
	u8* mData;   // _00
	u8 _04[0xC]; // _04
	s32 mSize;   // _10

	XObj* Get(s32 i) { return (XObj*)(mData + i * mSize); }
};
#define GAME_ACTORS ((UnkObjPool*)(lbl_8025C400 + 0x96F8)) // 48 of them
#define GAME_POOL2  ((UnkObjPool*)(lbl_8025C400 + 0x97E8)) // 16 of them
#define GAME_POOL   ((UnkObjPool*)(lbl_8025C400 + 0x97FC)) // 16 of them

// one entry of an actor's sound table, the sound an animation plays at a frame
struct UnkActorSeEntry {
	s32 mAnim;    // _00
	f32 mFrame;   // _04
	u8 _08[0x10]; // _08
};

// plays an actor's sounds along its animation
class UnkActorSe {
public:
	const UnkActorSeEntry* mTable; // _00
	u8 _04[0x8];                   // _04
	// _0C vtable

	virtual ~UnkActorSe();                         // _08
	virtual void Play(s32 id, s32 vol, f32 frame); // _0C
};

// the frame the jobj's first animation is at
static inline f32 GetAnimFrame(HSD_JObj* jobj)
{
	HSD_AObj* aobj = GetAObj(jobj);
	return aobj != NULL ? HSD_AObjGetCurrFrame(aobj) : 0.0f;
}

// and the frame it ends at
static inline f32 GetAnimEndFrame(HSD_JObj* jobj)
{
	HSD_AObj* aobj = GetAObj(jobj);
	return aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f;
}

extern "C" BOOL fn_8001E6C8(CSound* sound, s32 sfx); // whether the sound is playing

#endif
