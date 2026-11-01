#ifndef _GAME_GXA_SHARED_H
#define _GAME_GXA_SHARED_H

#include "game/hsd_jobj.h"
#include "game/cparticle.h"
#include "game/cjobjset.h"
#include "game/hsd_cobj.h"
#include "game/cprimitive.h"
#include "game/cfont.h"
#include "game/ccamera.h"
#include "game/clight.h"
#include "game/cfile.h"
#include "Version.h"
#include "game/mm.h"
#include "Dolphin/OS/OSFastCast.h"

extern "C" {
#include <sysdolphin/controller.h>
#include <sysdolphin/tev.h>
#include <sysdolphin/mobj.h>
#include <sysdolphin/initialize.h>
#include <sysdolphin/archive.h>
}

// the rest of the game core after game/cgame.cpp: CGame's and CBase's constructors, the
// constructors and destructors the compiler writes for the objects they own, and the system
// setup. the functions whose class is still unknown are extern "C" under their dol address

// game/cobj.h's classes, with the constructors CXObj and CObjArchive have here. offsets are
// usa, jp's CJArchive is 8 bytes shorter so CXObj fields after mArchive shift

// HSD archive plus the CFile it's read through, refcounted between CJArchives
class CObjArchive : public HSD_Archive {
public:
	// _00 HSD_Archive
	CFile mFile;   // _44
	s32 mRefCount; // _130
	// _134 vtable

	CObjArchive() { mRefCount = 0; }
	virtual ~CObjArchive() { } // _08 (weak)

	// inlined:
	s32 Parse();
};
typedef char CObjArchive_size_check[sizeof(CObjArchive) == 0x138 ? 1 : -1];

// what the archive's public scene symbol points at
struct CJArchiveModel {
	HSD_Joint* mJoint;                    // _00
	HSD_AnimJoint** mAnimJoint;           // _04
	HSD_MatAnimJoint** mMatAnimJoint;     // _08
	HSD_ShapeAnimJoint** mShapeAnimJoint; // _0C
};

struct CJArchiveDesc {
	u32 _00;    // _00
	void** _04; // _04, null terminated
};

struct CJArchiveScene {
	CJArchiveModel** mModels; // _00
	CJArchiveDesc* _04;       // _04
	CJArchiveDesc** _08;      // _08, 8 of them
	CJArchiveDesc* _0C;       // _0C
};

// japan has one desc list fewer, the last one, which the usa SetupScene fills in wrong
#if defined(VERSION_JP)
#define CJARCHIVE_DESC_NUM 9
#else
#define CJARCHIVE_DESC_NUM 10
#endif

// a step machine that reads one CObjArchive in, then the model and anims out of it
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
	s32 mNum[CJARCHIVE_DESC_NUM + 1];         // _50, the anim count, then one per mDesc list
	// _7C vtable

	virtual ~CJArchive() { }                                                                               // _08 (weak)
	virtual void Clear();                                                                                  // _0C
	virtual BOOL ReadAsync(CObjArchive* archive, const char* path, u32 flags, u32 aramAddr, u32 aramSize); // _10
	virtual BOOL ReadCheck();                                                                              // _14
	virtual void Release();                                                                                // _18

	void SetupScene(s32 model, s32 sceneIndex);
	HSD_AnimJoint* GetAnimJoint(s32 id, s32 depth, const s32* path);
	HSD_MatAnimJoint* GetMatAnimJoint(s32 id, s32 depth, const s32* path);
	HSD_ShapeAnimJoint* GetShapeAnimJoint(s32 id, s32 depth, const s32* path);

	// inlined:
	CJArchiveScene* GetScene(s32 index);
	HSD_JObj* LoadJObj();
	HSD_AnimJoint* GetAnimJointRoot(s32 id);
	HSD_MatAnimJoint* GetMatAnimJointRoot(s32 id);
	HSD_ShapeAnimJoint* GetShapeAnimJointRoot(s32 id);
};
#if defined(VERSION_JP)
typedef char CJArchive_size_check[sizeof(CJArchive) == 0x78 ? 1 : -1];
#else
typedef char CJArchive_size_check[sizeof(CJArchive) == 0x80 ? 1 : -1];
#endif

// a value with a target and a per frame speed to step it there
class CObjVec : public CVec {
public:
	CVec mTarget; // _10
	CVec mSpeed;  // _20

	u8 Step(u32 frames);

	// puts it at a value with nowhere to go
	void Reset(f32 x, f32 y, f32 z)
	{
		this->x   = x;
		this->y   = y;
		this->z   = z;
		mTarget.x = x;
		mTarget.y = y;
		mTarget.z = z;
		mSpeed.x  = 0.0f;
		mSpeed.y  = 0.0f;
		mSpeed.z  = 0.0f;
	}
	void Reset(const Vec& v) { Reset(v.x, v.y, v.z); }
	void SetTarget(f32 x, f32 y, f32 z)
	{
		mTarget.x = x;
		mTarget.y = y;
		mTarget.z = z;
	}
	void SetTarget(const Vec& v) { SetTarget(v.x, v.y, v.z); }

	// inlined:
	bool IsMoving() const;
};
typedef char CObjVec_size_check[sizeof(CObjVec) == 0x30 ? 1 : -1];

// a value waiting to be put on a jobj after the next anim step
struct CXObjQueueEntry {
	HSD_JObj* mJObj; // _00
	u32 mType;       // _04, which value, 1 to 0x100, or 0x200 and 0x400 for anim rates
	f32 mValue;      // _08
};

// the game's object, a CJObj with an archive, stepped transforms and a queue of joint values
class CXObj : public CJObj {
public:
	// _00 CJObj, vtable at _34
	u32 mId;                    // _38
	s32 mLoadState;             // _3C, 1 while the archive is being read
	u32 mModel;                 // _40
	u32 mLoadFlags;             // _44
	CJArchive mArchive;         // _48
	CObjVec mMoveTranslate;     // _C8
	CObjVec mMoveRotate;        // _F8
	CObjVec mMoveScale;         // _128
	s32 mAnimId;                // _158
	f32 mAnimRate;              // _15C
	f32 mLastAnimRate;          // _160
	u32 mColors[2];             // _164, the two colors the x_cobj materials blend in
	s32 mQueued;                // _16C
	CXObjQueueEntry mQueue[10]; // _170

	CXObj()
	{
		mMoveTranslate.Reset(0.0f, 0.0f, 0.0f);
		mMoveRotate.Reset(0.0f, 0.0f, 0.0f);
		mMoveScale.Reset(0.0f, 0.0f, 0.0f);
	}

	// CJObj overrides, defined in another file
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
	virtual void vf64();                                                                                              // _64, 0x8002657C
	virtual void vf68();                                                                                              // _68, 0x800263EC
	virtual void vf6C();                                                                                              // _6C, 0x80027B70
	virtual void vf70();                                                                                              // _70, 0x80026938
	virtual void vf74();                                                                                              // _74, 0x800266F8
	virtual void SetMoveDir(const Vec* dir, f32 speed);                                                               // _78
	virtual void SetRotateY(f32 target, f32 speed);                                                                   // _7C
	virtual void SetAnim(s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate, f32 blend,
	                     f32 blendRate); // _80
	virtual void ClearAnim();            // _84
	virtual void UpdateAnim(u32 speed);  // _88
	virtual void ApplyTransform();       // _8C

	void Init(u32 id);
	BOOL Load(CObjArchive* archive, const char* path, u32 model, u32 flags, u32 aramAddr, u32 aramSize);
	void Push(HSD_JObj* jobj, u32 type, f32 value);
	void Attach(HSD_JObj* jobj);
	f32 GetMObjAlpha(HSD_MObj* mobj);

	// inlined:
	HSD_JObj* FindJoint(s32 depth, s32* path);
	void ClearQueue();
};
#if defined(VERSION_JP)
typedef char CXObj_size_check[sizeof(CXObj) == 0x1E0 ? 1 : -1];
#else
typedef char CXObj_size_check[sizeof(CXObj) == 0x1E8 ? 1 : -1];
#endif

// a CXObj with a second jobj it blends into
class CXObjIntp : public CXObj {
public:
	CJObj mIntp;    // _1E8
	f32 mBlend;     // _220, below 1 while the two jobjs are being blended
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
#if defined(VERSION_JP)
typedef char CXObjIntp_size_check[sizeof(CXObjIntp) == 0x220 ? 1 : -1];
#else
typedef char CXObjIntp_size_check[sizeof(CXObjIntp) == 0x228 ? 1 : -1];
#endif

extern "C" {
extern s32 __OSCurrHeap;
void* OSAllocFromHeap(s32 heap, u32 size);
BOOL OSCreateThread(void* thread, void* (*func)(void*), void* param, void* stack, u32 stackSize, s32 priority, u16 attr);
}

class CGame;
extern "C" CGame lbl_8025C400;

typedef void* (*Dtor)(void* obj, s16 del);
extern "C" void __destroy_arr(void* block, Dtor dtor, u32 size, u32 count);

// the camera actions, as game/cgame.cpp spells them out. game/ccameraaction.h has them with
// the interpolations as padding

class XObj;
class XObjCamera;

// moves a float from one value to another over time. Step moves one frame, Update the
// given number of frames, both say whether it's done
class CInterfaceInterpolation {
public:
	// _00 vtable

	virtual ~CInterfaceInterpolation() { }              // _08
	virtual bool Start(f32 from, f32 to, f32 time) = 0; // _0C
	virtual bool Step(f32 dt)                      = 0; // _10
	virtual bool Update(u32 step, f32 dt)          = 0; // _14
	virtual f32 Get()                              = 0; // _18
};

#define DECLARE_INTERP_METHODS                      \
	virtual bool Start(f32 from, f32 to, f32 time); \
	virtual bool Step(f32 dt);                      \
	virtual bool Update(u32 step, f32 dt);          \
	virtual f32 Get();

class CInterpolationLinear : public CInterfaceInterpolation {
public:
	f32 mValue; // _04
	f32 mSpeed; // _08
	f32 mTime;  // _0C, left to go

	DECLARE_INTERP_METHODS
};

// jumps straight to the end
class CInterpolationNone : public CInterfaceInterpolation {
public:
	f32 mValue; // _04
	f32 mSpeed; // _08
	f32 mTime;  // _0C

	DECLARE_INTERP_METHODS
};

// the ease in out and ease out have their methods in another file
class CInterpolationEaseInOut : public CInterfaceInterpolation {
public:
	u8 _04[0x18]; // _04

	DECLARE_INTERP_METHODS
};

class CInterpolationEaseOut : public CInterfaceInterpolation {
public:
	u8 _04[0x1C]; // _04

	DECLARE_INTERP_METHODS
};

// each step closes the distance by a fraction
class CInterpolationEaseOutDivide : public CInterfaceInterpolation {
public:
	f32 mValue;   // _04
	f32 mTarget;  // _08
	f32 mDivisor; // _0C

	DECLARE_INTERP_METHODS
};

// the same for a vector, one float interpolation per axis
class CInterfaceVectorInterpolation {
public:
	// _00 vtable

	virtual ~CInterfaceVectorInterpolation() { }        // _08
	virtual bool Start(Vec from, Vec to, f32 time) = 0; // _0C
	virtual bool Step(f32 dt)                      = 0; // _10
	virtual bool Update(u32 step, f32 dt)          = 0; // _14
	virtual Vec Get()                              = 0; // _18
};

#define DECLARE_VEC_INTERP_METHODS                  \
	virtual bool Start(Vec from, Vec to, f32 time); \
	virtual bool Step(f32 dt);                      \
	virtual bool Update(u32 step, f32 dt);          \
	virtual Vec Get();

class CVectorLinearInterpolation : public CInterfaceVectorInterpolation {
public:
	CInterpolationLinear mAxes[3]; // _04

	DECLARE_VEC_INTERP_METHODS
};
typedef char CVectorLinearInterpolation_size_check[sizeof(CVectorLinearInterpolation) == 0x34 ? 1 : -1];

class CVectorEaseInOutInterpolation : public CInterfaceVectorInterpolation {
public:
	CInterpolationEaseInOut mAxes[3]; // _04
	BOOL mDone;                       // _58, x and z are there

	DECLARE_VEC_INTERP_METHODS
};
typedef char CVectorEaseInOutInterpolation_size_check[sizeof(CVectorEaseInOutInterpolation) == 0x5C ? 1 : -1];

class CVectorEaseOutInterpolation : public CInterfaceVectorInterpolation {
public:
	CInterpolationEaseOut mAxes[3]; // _04

	DECLARE_VEC_INTERP_METHODS
};
typedef char CVectorEaseOutInterpolation_size_check[sizeof(CVectorEaseOutInterpolation) == 0x64 ? 1 : -1];

class CVectorNoneInterpolation : public CInterfaceVectorInterpolation {
public:
	CInterpolationNone mAxes[3]; // _04

	DECLARE_VEC_INTERP_METHODS
};

class CVectorEaseOutDivideInterpolation : public CInterfaceVectorInterpolation {
public:
	CInterpolationEaseOutDivide mAxes[3]; // _04
	BOOL mDone;                           // _34, x and z are there

	DECLARE_VEC_INTERP_METHODS
};
typedef char CVectorEaseOutDivideInterpolation_size_check[sizeof(CVectorEaseOutDivideInterpolation) == 0x38 ? 1 : -1];

class CInterfaceCameraAction {
public:
	CVec mEye;      // _00
	CVec mInterest; // _10
	CVec mAngle;    // _20
	f32 mDistance;  // _30
	f32 mFov;       // _34
	u32 _38;        // _38
	// _3C vtable

	virtual ~CInterfaceCameraAction() { }       // _08
	virtual void PreviousUpdate() { }           // _0C
	virtual void Update(u32 step) = 0;          // _10
	virtual void vf14() { }                     // _14
	virtual void SetCamera(CCamera* camera) { } // _18
	virtual void Init() { }                     // _1C
	virtual void vf20() { }                     // _20
	virtual void vf24() { }                     // _24
	virtual void vf28() { }                     // _28
	virtual void SetEye(f32 x, f32 y, f32 z)    // _2C
	{
		mEye.x = x;
		mEye.y = y;
		mEye.z = z;
	}
	virtual void SetInterest(f32 x, f32 y, f32 z) // _30
	{
		mInterest.x = x;
		mInterest.y = y;
		mInterest.z = z;
	}
	virtual void SetAngle(f32 x, f32 y, f32 z) // _34
	{
		mAngle.x = x;
		mAngle.y = y;
		mAngle.z = z;
	}
	virtual void SetEye(const Vec& eye) { SetEye(eye.x, eye.y, eye.z); }                               // _38
	virtual void SetInterest(const Vec& interest) { SetInterest(interest.x, interest.y, interest.z); } // _3C
	virtual void SetAngle(const Vec& angle) { SetAngle(angle.x, angle.y, angle.z); }                   // _40
	virtual void SetDistance(f32 distance) { mDistance = distance; }                                   // _44
	virtual void SetFov(f32 fov) { mFov = fov; }                                                       // _48
	virtual void SetAngleZ(f32 z) { mAngle.z = z; }                                                    // _4C
	virtual u32 vf50() { return 0; }                                                                   // _50
	virtual u32 Get_38() { return _38; }                                                               // _54
	virtual f32 vf58();                                                                                // _58
	virtual u32 vf5C() { return 0; }                                                                   // _5C
	virtual XObj* GetTarget() { return NULL; }                                                         // _60
};

// base of the area, map and reset actions, the methods it adds ask the area state
class CCameraActionAreaExM : public CInterfaceCameraAction {
public:
	virtual void vf64();          // _64
	virtual f32 vf68();           // _68
	virtual s32 vf6C();           // _6C
	virtual void vf70(s32 value); // _70
	virtual void vf74(Vec pos);   // _74
	virtual BOOL vf78(s32 index); // _78
	virtual BOOL vf7C(s32 index); // _7C
};

// the area action, CCameraActions holds it at _40
class CCameraActionArea : public CCameraActionAreaExM {
public:
	u8 _40[0x70 - 0x40];                    // _40
	CVectorEaseInOutInterpolation _70;      // _70
	CVectorLinearInterpolation _CC;         // _CC
	CVectorEaseInOutInterpolation _100;     // _100
	CVectorEaseOutInterpolation _15C;       // _15C
	CVectorEaseOutDivideInterpolation _1C0; // _1C0
	u8 _1F8[0x20C - 0x1F8];                 // _1F8
	CInterpolationLinear _20C;              // _20C
	CInterpolationEaseInOut _21C;           // _21C
	CInterpolationEaseOut _238;             // _238
	CInterpolationEaseOutDivide _258;       // _258
	u8 _268[0x294 - 0x268];                 // _268

	virtual void Update(u32 step); // _10
};
typedef char CCameraActionArea_size_check[sizeof(CCameraActionArea) == 0x294 ? 1 : -1];

// an entry in CCameraActionMap's _570 and _9A0 lists
struct CamMapEntry {
	Vec _00; // _00, a position
	s32 _0C; // _0C
	s32 _10; // _10
	Vec _14; // _14
};

// the map action, CCameraActions holds it at _99C
class CCameraActionMap : public CCameraActionAreaExM {
public:
	s32 _40;                                // _40
	XObj* _44;                              // _44, the object the camera looks at
	CVec _48;                               // _48
	f32 _58;                                // _58
	f32 _5C;                                // _5C
	CVec _60;                               // _60
	CVec _70;                               // _70
	f32 _80;                                // _80
	f32 _84;                                // _84
	f32 _88;                                // _88
	CObjVec _8C;                            // _8C
	f32 _BC;                                // _BC, turn speed
	f32 _C0;                                // _C0, wrapped target yaw
	f32 _C4;                                // _C4
	CInterpolationEaseOutDivide _C8;        // _C8
	CVec _D8;                               // _D8
	u8 _E8[0x4EC - 0xE8];                   // _E8
	s32 _4EC;                               // _4EC
	s32 _4F0;                               // _4F0
	s32 _4F4;                               // _4F4
	s32 _4F8;                               // _4F8
	u8 _4FC[0x570 - 0x4FC];                 // _4FC
	CamMapEntry _570[32];                   // _570
	s32 _970;                               // _970, count of _570
	CamMapEntry* _974;                      // _974
	s32 _978;                               // _978
	CamMapEntry* _97C;                      // _97C
	s32 _980;                               // _980
	Vec _984;                               // _984, one end of the picked side
	Vec _990;                               // _990, the other end
	s32 _99C;                               // _99C, 1 or -1, which way round they are
	CamMapEntry _9A0[32];                   // _9A0
	s32 _DA0;                               // _DA0, count of _9A0
	s32 _DA4;                               // _DA4
	s32 _DA8;                               // _DA8
	s32 _DAC;                               // _DAC
	s32 _DB0;                               // _DB0
	CInterfaceVectorInterpolation* _DB4;    // _DB4
	CVectorEaseInOutInterpolation _DB8;     // _DB8
	CInterpolationEaseInOut _E14;           // _E14
	u8 _E30[0x4];                           // _E30
	CInterpolationEaseInOut _E34;           // _E34
	u8 _E50[0xE64 - 0xE50];                 // _E50
	CInterfaceInterpolation* _E64;          // _E64
	CInterpolationEaseOutDivide _E68;       // _E68
	u8 _E78[0x4];                           // _E78
	CInterpolationEaseInOut _E7C;           // _E7C
	u8 _E98[0xEA8 - 0xE98];                 // _E98
	s32 _EA8;                               // _EA8
	s32 _EAC;                               // _EAC
	s32 _EB0;                               // _EB0
	u8 _EB4[0xF28 - 0xEB4];                 // _EB4
	CInterfaceVectorInterpolation* _F28;    // _F28
	CVectorEaseOutDivideInterpolation _F2C; // _F2C
	u8 _F64[0xF68 - 0xF64];                 // _F64
	CVec _F68;                              // _F68
	CVec _F78;                              // _F78
	CVec _F88;                              // _F88
	CVec _F98;                              // _F98
	CVec _FA8;                              // _FA8
	CVec _FB8;                              // _FB8
	u8 _FC8[0x11D8 - 0xFC8];                // _FC8
	CStatus _11D8;                          // _11D8, printed as CAMSTAT
	CStatus _11E0;                          // _11E0, printed as CAMSTATPRE
	BOOL _11E8;                             // _11E8

	CCameraActionMap();            // in the dol, 0x8001A45C
	virtual ~CCameraActionMap();   // in the dol, 0x80009F20
	virtual void Update(u32 step); // _10
};
typedef char CCameraActionMap_size_check[sizeof(CCameraActionMap) == 0x11EC ? 1 : -1];

// puts the camera back behind the player
class CCameraActionReset : public CCameraActionAreaExM {
public:
	XObj* mTarget;                                 // _40
	s32 mMode;                                     // _44, picks the interpolators, 3 is none
	f32 mTime;                                     // _48
	BOOL _4C;                                      // _4C
	s32 _50;                                       // _50
	s32 mState;                                    // _54
	s32 _58;                                       // _58
	CInterfaceVectorInterpolation* mEyeInterp;     // _5C
	CInterfaceInterpolation* mFovInterp;           // _60
	CVectorLinearInterpolation mEyeLinear;         // _64
	CVectorEaseInOutInterpolation mEyeEaseInOut;   // _98
	CVectorEaseOutInterpolation mEyeEaseOut;       // _F4
	CVectorNoneInterpolation mEyeNone;             // _158
	CInterfaceVectorInterpolation* mEyeInterps[4]; // _18C, by mode
	CInterpolationLinear mFovLinear;               // _19C
	CInterpolationEaseInOut mFovEaseInOut;         // _1AC
	CInterpolationEaseOut mFovEaseOut;             // _1C8
	CInterpolationNone mFovNone;                   // _1E8
	CInterfaceInterpolation* mFovInterps[4];       // _1F8

	virtual void PreviousUpdate();                // _0C
	virtual void Update(u32 step);                // _10
	virtual void SetCamera(CCamera* camera);      // _18
	virtual void Init();                          // _1C
	virtual f32 vf58() { return 4.0f; }           // _58
	virtual u32 vf5C() { return _58; }            // _5C
	virtual XObj* GetTarget() { return mTarget; } // _60
};
typedef char CCameraActionReset_size_check[sizeof(CCameraActionReset) == 0x208 ? 1 : -1];

// a camera that looks along the z axis at the player from a fixed distance
class CCameraActionScroll : public CInterfaceCameraAction {
public:
	BOOL mStart;                               // _40
	f32 mFarDistance;                          // _44
	f32 mNearDistance;                         // _48
	f32 mScrollFov;                            // _4C
	CInterfaceInterpolation* mDistanceInterp;  // _50
	CInterpolationEaseOutDivide mDistanceEase; // _54
	u32 _64;                                   // _64
	s32 mZoom;                                 // _68, 0 near, 1 far

	virtual void Update(u32 step);           // _10
	virtual void vf14();                     // _14
	virtual void SetCamera(CCamera* camera); // _18
	virtual u32 vf5C() { return _64; }       // _5C
};
typedef char CCameraActionScroll_size_check[sizeof(CCameraActionScroll) == 0x6C ? 1 : -1];

// behind the player at an angle off its facing
class CCameraActionLadr : public CInterfaceCameraAction {
public:
	f32 mAngle; // _40

	virtual void Update(u32 step);      // _10
	virtual void vf14();                // _14
	virtual u32 Get_38() { return 11; } // _54
	virtual Vec GetCameraTarget();      // _64
};

// just looks at the player
class CCameraActionDefault : public CInterfaceCameraAction {
public:
	virtual void Update(u32 step); // _10
};

// follows the player from behind on a damped spring
class CCameraActionChase : public CInterfaceCameraAction {
public:
	XObj* mTarget;     // _40
	f32 mRestDistance; // _44
	f32 mDamping;      // _48
	CVec mLastPos;     // _4C
	CVec mPos;         // _5C
	u32 mMode;         // _6C

	virtual void Update(u32 step);           // _10
	virtual void vf14();                     // _14
	virtual void SetCamera(CCamera* camera); // _18
	virtual void Init();                     // _1C
	virtual u32 Get_38() { return mMode; }   // _54
};
typedef char CCameraActionChase_size_check[sizeof(CCameraActionChase) == 0x70 ? 1 : -1];

// the camera event scripts drive
class CCameraActionEvt : public CInterfaceCameraAction {
public:
	XObj* mTarget;                                      // _40
	CInterfaceVectorInterpolation* mEyeInterp;          // _44
	CInterfaceVectorInterpolation* mInterestInterp;     // _48
	CInterfaceInterpolation* mDistanceInterp;           // _4C
	CInterfaceInterpolation* mFovInterp;                // _50
	CInterfaceVectorInterpolation* mAngleInterp;        // _54
	CStatus mAxes;                                      // _58
	u32 _60;                                            // _60
	CVectorLinearInterpolation mEyeLinear;              // _64
	CVectorEaseInOutInterpolation mEyeEaseInOut;        // _98
	CVectorEaseOutInterpolation mEyeEaseOut;            // _F4
	CVectorNoneInterpolation mEyeNone;                  // _158
	CInterfaceVectorInterpolation* mEyeInterps[4];      // _18C
	CVectorLinearInterpolation mInterestLinear;         // _19C
	CVectorEaseInOutInterpolation mInterestEaseInOut;   // _1D0
	CVectorEaseOutInterpolation mInterestEaseOut;       // _22C
	CVectorNoneInterpolation mInterestNone;             // _290
	CInterfaceVectorInterpolation* mInterestInterps[4]; // _2C4
	CInterpolationLinear mFovLinear;                    // _2D4
	CInterpolationEaseInOut mFovEaseInOut;              // _2E4
	CInterpolationEaseOut mFovEaseOut;                  // _300
	CInterpolationNone mFovNone;                        // _320
	CInterfaceInterpolation* mFovInterps[4];            // _330
	CInterpolationLinear mDistanceLinear;               // _340
	CInterpolationEaseInOut mDistanceEaseInOut;         // _350
	CInterpolationEaseOut mDistanceEaseOut;             // _36C
	CInterpolationNone mDistanceNone;                   // _38C
	CInterfaceInterpolation* mDistanceInterps[4];       // _39C
	CVectorLinearInterpolation mAngleLinear;            // _3AC
	CVectorEaseInOutInterpolation mAngleEaseInOut;      // _3E0
	CVectorEaseOutInterpolation mAngleEaseOut;          // _43C
	CVectorNoneInterpolation mAngleNone;                // _4A0
	CInterfaceVectorInterpolation* mAngleInterps[4];    // _4D4

	virtual void Update(u32 step);           // _10
	virtual void SetCamera(CCamera* camera); // _18
	virtual void Init();                     // _1C
};
typedef char CCameraActionEvt_size_check[sizeof(CCameraActionEvt) == 0x4E4 ? 1 : -1];

// the room camera
class CCameraActionScroll2 : public CInterfaceCameraAction {
public:
	Vec* mCenterPtr;     // _40
	Vec mCenter;         // _44
	f32 mMinX;           // _50
	f32 mMaxX;           // _54
	f32 mMinZ;           // _58
	f32 mMaxZ;           // _5C
	f32 mMaxFov;         // _60
	f32 mMinFov;         // _64
	f32 mYaw;            // _68
	Vec mOffset;         // _6C
	Vec mStart;          // _78
	BOOL mTurned;        // _84
	BOOL mFromStage;     // _88
	u8 mSavedInfo[0x20]; // _8C

	virtual void Update(u32 step);      // _10
	virtual void vf14();                // _14
	virtual void vf20();                // _20
	virtual void vf28() { }             // _28
	virtual u32 Get_38() { return 11; } // _54
};
typedef char CCameraActionScroll2_size_check[sizeof(CCameraActionScroll2) == 0xAC ? 1 : -1];

// looks around from the player's eyes
class CCameraActionEye : public CInterfaceCameraAction {
public:
	CObjVec mLook;  // _40
	CVec mOverride; // _70

	virtual void Update(u32 step);           // _10
	virtual void vf14();                     // _14
	virtual void Init() { }                  // _1C
	virtual u32 Get_38() { return 0x20019; } // _54
};
typedef char CCameraActionEye_size_check[sizeof(CCameraActionEye) == 0x80 ? 1 : -1];

// plays a camera animation
class CCameraActionAnim : public CInterfaceCameraAction {
public:
	CCamera* mCamera; // _40
	XObjCamera* mObj; // _44

	virtual void Update(u32 step);           // _10
	virtual void SetCamera(CCamera* camera); // _18
	virtual void Init();                     // _1C
};
typedef char CCameraActionAnim_size_check[sizeof(CCameraActionAnim) == 0x48 ? 1 : -1];

// every camera action and the table the camera picks them from, CGame holds it at
// +0xE5FAC
class CCameraActions {
public:
	CCameraActionDefault mDefault;        // _0000
	CCameraActionArea mArea;              // _0040
	CCameraActionChase mChase;            // _02D4
	CCameraActionEvt mEvt;                // _0344
	CCameraActionScroll2 mScroll2;        // _0828
	CCameraActionEye mEye;                // _08D4
	CCameraActionAnim mAnim;              // _0954
	CCameraActionMap mMap;                // _099C
	CCameraActionLadr mLadr;              // _1B88
	CCameraActionReset mReset;            // _1BCC
	CCameraActionScroll mScroll;          // _1DD4
	CInterfaceCameraAction* mActions[12]; // _1E40
	// _1E70 vtable

	CCameraActions();
	virtual ~CCameraActions(); // _08
};

// an OSThread with its own stack, name from RTTI
class CThread {
public:
	u8 mThread[0x318]; // _000, the OSThread
	void* mStack;      // _318
	u32 mStackSize;    // _31C
	// _320 vtable

	virtual ~CThread() { }                                                                         // _08
	virtual BOOL Create(void* (*func)(void*), void* param, u32 stackSize, s32 priority, u16 attr); // _0C
};

#endif
