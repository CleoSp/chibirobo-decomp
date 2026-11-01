#include "game/particle_srt.h"
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

/**
 * @note Address: 0x80019480
 * @note Size: 0x8C
 */
BOOL CThread::Create(void* (*func)(void*), void* param, u32 stackSize, s32 priority, u16 attr)
{
	mStack     = OSAllocFromHeap(__OSCurrHeap, stackSize);
	mStackSize = stackSize;
	if (OSCreateThread(mThread, func, param, (u8*)mStack + mStackSize, mStackSize, priority, attr)) {
		mStackSize = stackSize;
		return TRUE;
	}
	return FALSE;
}

// what has been read ahead into aram, name from RTTI
class ARCacheInfo {
public:
	u32 _00;        // _00
	u32 _04;        // _04
	u8 _08[0x4000]; // _08
	// _4008 vtable

	virtual ~ARCacheInfo(); // _08
};

/**
 * @note Address: 0x8001975C
 * @note Size: 0x48
 */
ARCacheInfo::~ARCacheInfo()
{
}

// CBase and the classes it is built from, as game/cgame.cpp has them. CGs is game/cgs.h's
// with the constructor this file has, so only the parts its constructor sees are here

class CGsScreen : public GXRenderModeObj {
public:
	CGsScreen() { mMode = -1; }

	s32 mMode; // _3C, index into the mode table, -1 before the first set
	// _40 vtable

	virtual ~CGsScreen() { } // _08
};

class CGsRenderMode : public HSD_PEDesc {
public:
	BOOL mPEDirty;     // _0C
	BOOL mCullDirty;   // _10
	BOOL mLineDirty;   // _14
	BOOL mPointDirty;  // _18
	s32 mCullMode;     // _1C
	u8 mLineWidth;     // _20
	s32 mLineTexOffs;  // _24
	u8 mPointSize;     // _28
	s32 mPointTexOffs; // _2C
	// _30 vtable

	virtual ~CGsRenderMode() { } // _08
};

class CGsTevStage : public HSD_TevDesc {
public:
	BOOL mStageDirty;   // _74
	BOOL mNumGensDirty; // _78
	BOOL mTexGenDirty;  // _7C
	u8 mNumTexGens;     // _80
	u32 mTexCoord;      // _84
	u32 mTexGenFunc;    // _88
	u32 mTexGenSrc;     // _8C
	u32 mTexMtx;        // _90
	// _94 vtable

	virtual ~CGsTevStage() { } // _08
};

class CGsChannel : public HSD_Chan {
public:
	BOOL mDirty; // _30
	// _34 vtable

	virtual ~CGsChannel() { } // _08
};

class CGs {
public:
	CGsScreen mScreen;             // _00
	CCamera* mCamera;              // _44
	CLight* mLight;                // _48
	CGsRenderMode mRenderMode;     // _4C
	CGsTevStage mTevStages[2];     // _80
	CGsChannel mChannels[4];       // _1B0
	CGsRenderMode* mCurRenderMode; // _290
	CGsTevStage* mCurTevStage;     // _294
	CGsChannel* mCurChannel;       // _298
	BOOL _29C;                     // _29C
	CFont mFont;                   // _2A0
	CTexObj mTex;                  // _3E0
	CSprite mSprite;               // _4E0
	CLine mLine;                   // _668
	CTriangle mTriangle;           // _7D4
	CCircle mCircle;               // _940
	CTile mTile;                   // _ABC
	CSprite mSprite2;              // _C40
	CCube mCube;                   // _DC8
	COctahedron mOctahedron;       // _F30
	CDodeca mDodeca;               // _1098
	CIcosahedron mIcosahedron;     // _1200
	CSphere mSphere;               // _1368
	CCylinder mCylinder;           // _14D0
	CTorus mTorus;                 // _1638
	s32 _17A0;                     // _17A0
	s32 _17A4;                     // _17A4
	f32 _17A8;                     // _17A8
	f32 _17AC;                     // _17AC
	// _17B0 vtable

	CGs();          // in the dol, 0x8001B9B8
	virtual ~CGs(); // _08
};
typedef char CGs_size_check[sizeof(CGs) == 0x17B4 ? 1 : -1];

class CPadEx {
public:
	HSD_PadData mQueue;               // _00
	HSD_PadRumbleListData mRumble[4]; // _34
	CPadOne mPad[4];                  // _B4
	// _204 vtable

	CPadEx();             // in the dol, 0x80197C8C
	virtual ~CPadEx() { } // _08

	void Init();
	void Update(s32 frames);
};

// an OSStopwatch with the last lap kept, name from RTTI
class CStopwatch {
public:
	u8 mWatch[0x30]; // _00, the OSStopwatch
	s64 mLap;        // _30, OSCheckStopwatch before it was stopped
	// _38 vtable

	virtual ~CStopwatch() { } // _08
};

class CBase {
public:
	CGs mGs;                   // _0000
	u8 _17B4[0x17C0 - 0x17B4]; // _17B4
	CFont mFont;               // _17C0
	CPadEx mPad;               // _1900
	u32 mRetraceCount;         // _1B08, VIGetRetraceCount at the start of the frame
	u32 mFrames;               // _1B0C, retraces since the last frame, at most 4
	CStatus64 mStatus;         // _1B10
	CCamera mCamera[2];        // _1B20
	CLight mLight[6];          // _1E48
	CStopwatch mStopwatch[3];  // _3258, the whole frame, the update and the draw
	u8 mProgressive;           // _3318
	s32 mDriveStatus;          // _331C
	s32 mLastDriveStatus;      // _3320
	s32 mDiskIn;               // _3324
	BOOL mRumblePaused;        // _3328
	s32 mResetTimer;           // _332C, frames the reset combo has been held
	BOOL mReset;               // _3330
	// _3334 vtable

	CBase();
	virtual ~CBase();                         // _08
	virtual void Init(int argc, char** argv); // _0C
	virtual void InitSystem();                // _10
	virtual BOOL Exec();                      // _14
	virtual void Update(u32 frames);          // _18
	virtual void Draw();                      // _1C
	virtual void vf20();                      // _20
	virtual void vf24();                      // _24
	virtual s32 GetFrameRate();               // _28, retraces per frame
	virtual u32 GetFrames();                  // _2C
	virtual CCamera* GetCamera();             // _30
	virtual CCamera* GetCamera2();            // _34
	virtual CCamera* GetMainCamera();         // _38
	virtual CLight* GetLight();               // _3C
	virtual CLight* GetLight2();              // _40
	virtual CLight* GetLightList();           // _44
	virtual void Reset(u32 code);             // _48
};

/**
 * @note Address: 0x8001B8A0
 * @note Size: 0xDC
 */
CBase::CBase()
{
}

/**
 * @note Address: 0x8001B9B8
 * @note Size: 0x1A4
 */
CGs::CGs()
{
}

// frame buffer effects, names from RTTI. the texture effect keeps its texture inside a part
// of its own
struct FbTexEffImage {
	u8 _00[0x1C]; // _00
	CTexObj mTex; // _1C
};

class CFbTexEff {
public:
	FbTexEffImage mImage; // _00
	u8 _11C[0x4];         // _11C
	// _120 vtable

	virtual void Init();             // _08
	virtual void Reset();            // _0C
	virtual void vf10();             // _10
	virtual void Update(u32 frames); // _14
	virtual void Draw();             // _18
};

// draws the scene mirrored through a camera of its own
class FbMirror : public CFbTexEff {
public:
	s32 _124;               // _124
	CCamera mCamera;        // _128
	u8 _2BC[0x2F0 - 0x2BC]; // _2BC
	s32 _2F0;               // _2F0
};

class FbMirrorEx : public FbMirror {
public:
	~FbMirrorEx();
};

// CGame and the classes it is built from, as game/cgame.cpp has them, with the constructors
// that CGame's constructor below makes the compiler write. names from RTTI

// an object's script position, game/xobjs.h has it too
class CoordInfo {
public:
	CoordInfo() { mValue = NULL; }

	void* mValue; // _00
	// _04 vtable

	virtual ~CoordInfo() { } // _08
};

class AttrInfo {
public:
	AttrInfo() { mData = NULL; }

	void* mData; // _00
	s32* _04;    // _04
	// _08 vtable

	virtual ~AttrInfo() { } // _08
};

class CEvtData {
public:
	CEvtData()
	{
		_00 = 0;
		_04 = 0;
	}

	u32 _00; // _00
	u32 _04; // _04
	// _08 vtable

	virtual ~CEvtData() { } // _08

	u8 _0C[0x20 - 0xC]; // _0C
};

class CStack {
public:
	CStack()
	{
		_00 = 0;
		_08 = 0;
		_04 = 0;
	}

	u32 _00; // _00
	u32 _04; // _04
	u32 _08; // _08
	// _0C vtable

	virtual ~CStack() { } // _08
};

// an event script runner
class CEvt {
public:
	u32 _00;        // _00
	CEvtData mData; // _04
	CStack mStack0; // _24
	CStack mStack1; // _34
	CStack mStack2; // _44
	u8 _54[0x10];   // _54
	// _64 vtable

	virtual ~CEvt() { } // _08

	u8 _68[0x4]; // _68
};
typedef char CEvt_size_check[sizeof(CEvt) == 0x6C ? 1 : -1];

class CAnmQueue {
public:
	u8 _00[0x84]; // _00
	// _84 vtable

	virtual ~CAnmQueue() { } // _08
};

// a CJArchive that keeps its animations, as game/obja.h has it
class CJAnim : public CJArchive {
public:
	s32 mReading; // _80
};

class CShadow;
class CMotionControlCamera;

// a game object
class XObj : public CXObjIntp {
public:
	CStatus64 _228;         // _228
	CoordInfo mCoord;       // _238
	AttrInfo mAttr;         // _240
	u8 _24C[0x264 - 0x24C]; // _24C
	CVec _264;              // _264
	u8 _274[0x794 - 0x274]; // _274
	CVec _794;              // _794
	CEvt mEvt;              // _7A4
	CAnmQueue mAnmQueue;    // _810
	u8 _898[0x8FC - 0x898]; // _898
	CJAnim mAnim;           // _8FC

	virtual void vf90();                                    // _90
	virtual void vf94();                                    // _94
	virtual void vf98();                                    // _98
	virtual void vf9C();                                    // _9C
	virtual void vfA0();                                    // _A0
	virtual void vfA4(s32 arg);                             // _A4
	virtual void vfA8();                                    // _A8
	virtual void vfAC();                                    // _AC
	virtual void vfB0(s32 arg);                             // _B0
	virtual void DrawMirrored(Mtx view);                    // _B4
	virtual void vfB8();                                    // _B8
	virtual void SetShadowAlpha(f32 alpha, const Vec* pos); // _BC
	virtual void vfC0();                                    // _C0
	virtual void vfC4();                                    // _C4
	virtual void vfC8();                                    // _C8
	virtual void vfCC();                                    // _CC
	virtual void CastShadow(CShadow* shadow);               // _D0
	virtual void CastShadow2(CShadow* shadow);              // _D4
	virtual void vfD8();                                    // _D8
	virtual void vfDC(s32 arg0, s32 arg1);                  // _DC
	virtual void vfE0();                                    // _E0
	virtual void vfE4();                                    // _E4
	virtual void vfE8();                                    // _E8
	virtual Vec GetCameraTarget();                          // _EC
	virtual void vfF0();                                    // _F0
	virtual void vfF4();                                    // _F4
	virtual void vfF8();                                    // _F8
	virtual void vfFC();                                    // _FC
	virtual void vf100();                                   // _100
	virtual void vf104();                                   // _104
	virtual f32 vf108();                                    // _108, how far its shadow reaches
	virtual void vf10C();                                   // _10C
	virtual void vf110();                                   // _110
	virtual void vf114();                                   // _114
	virtual void vf118();                                   // _118
	virtual BOOL IsInView(CMotionControlCamera* camera);    // _11C
	virtual void vf120();                                   // _120
	virtual void vf124();                                   // _124
	virtual void vf128();                                   // _128
	virtual void vf12C();                                   // _12C
	virtual void vf130();                                   // _130
	virtual void vf134();                                   // _134
	virtual void vf138();                                   // _138
	virtual void vf13C();                                   // _13C
	virtual void vf140();                                   // _140
	virtual void OnMicVolume(s32 arg, s32 volume);          // _144
};
typedef char XObj_size_check[sizeof(XObj) == 0x980 ? 1 : -1];

// the menu cursor objects add little to XObj but their vtables
class Cursor : public XObj {
public:
	u8 _980[0x8]; // _980
};
class MenuCursor : public Cursor { };

// an object with a camera and lights of its own for the title screen
class XObjDemo : public XObj {
public:
	u8 _980[0x8];      // _980
	CCamera mCamera;   // _988
	u8 _B1C[0x8];      // _B1C
	CLight mLights[8]; // _B24
};

// what the camera looked like before an action took over
class CCameraMemento {
public:
	virtual ~CCameraMemento() { } // _08

	CVec mEye;                       // _04
	CVec mInterest;                  // _14
	CVec mAngle;                     // _24
	f32 mFov;                        // _34
	f32 mDistance;                   // _38
	CInterfaceCameraAction* mAction; // _3C
};

// the camera the game draws with, it runs the current camera action and eases towards it.
// game/cmotioncontrolcamera.h has the methods
class CMotionControlCamera {
public:
	CCamera mCamera;                       // _00
	CVec mEye;                             // _194
	CVec mInterest;                        // _1A4
	CVec mAngle;                           // _1B4
	f32 mDistance;                         // _1C4
	f32 mFov;                              // _1C8
	s32 mMode;                             // _1CC
	CInterfaceCameraAction* mAction;       // _1D0
	CInterfaceCameraAction* mPrevAction;   // _1D4
	CInterfaceCameraAction* mPushedAction; // _1D8
	s32 _1DC;                              // _1DC
	s32 _1E0;                              // _1E0
	CStatus mFlags;                        // _1E4
	s32 mInterestMode;                     // _1EC
	s32 mEyeMode;                          // _1F0

	virtual ~CMotionControlCamera() { } // _08

	CVec mFollowTarget;      // _1F8
	CVec mFollowValue;       // _208
	CVec mFollowStep;        // _218
	BOOL mFollowDone;        // _228
	f32 mFollowDivide;       // _22C
	CCameraMemento mMemento; // _230
	u8 _270[0x274 - 0x270];  // _270
};
typedef char CMotionControlCamera_size_check[sizeof(CMotionControlCamera) == 0x274 ? 1 : -1];

// the THP movie player, game/cmovie.h has the methods
class CMovie {
public:
	u32 mState;          // _00
	char mPath[0x80];    // _04
	BOOL mLoop;          // _84
	BOOL mAlpha;         // _88
	u32 mVideoWidth;     // _8C, THPVideoInfo
	u32 mVideoHeight;    // _90
	u32 mVideoType;      // _94
	u8 mAudioInfo[0x10]; // _98, THPAudioInfo
	void* mBuffer;       // _A8
	s32 mFrame;          // _AC
	s32 mLastFrame;      // _B0

	virtual ~CMovie() { } // _08
};
typedef char CMovie_size_check[sizeof(CMovie) == 0xB8 ? 1 : -1];

// a menu, its cursor and not much else
struct MenuEx {
	u8 _00[0x28];       // _00
	MenuCursor mCursor; // _28
	// _9B0 vtable

	virtual ~MenuEx() { } // _08
};

// a struct holding a cursor, CMsgFont has one
struct CursorHolder {
	Cursor mCursor; // _00
};

// the screen overlays: a CScrnColor fills the screen, the fades and wipes move it in and
// out. game/gxb.cpp has their methods
class CScrnColor {
public:
	CScrnColor()
	{
		mColor[0]  = 0.0f;
		mColor[1]  = 0.0f;
		mColor[2]  = 0.0f;
		mColor[3]  = 0.0f;
		mTarget[0] = 0.0f;
		mTarget[1] = 0.0f;
		mTarget[2] = 0.0f;
		mTarget[3] = 0.0f;
		mSpeed[0]  = 0.0f;
		mSpeed[1]  = 0.0f;
		mSpeed[2]  = 0.0f;
		mSpeed[3]  = 0.0f;
	}

	f32 mColor[4];  // _00
	f32 mTarget[4]; // _10
	f32 mSpeed[4];  // _20
	// _30 vtable

	virtual ~CScrnColor() { } // _08
};

class CScrnFade : public CScrnColor { };

class CScrnWipe : public CScrnColor {
public:
	u8 _34[0x30]; // _34
};

class CScrnWipeCircle : public CScrnWipe { };
class CScrnWipeCustom : public CScrnWipe { };
class CScrnWipeTile : public CScrnWipe { };

class CScrnWipeTex : public CScrnWipe {
public:
	CTexObj mTex; // _64
};

class CScrnRoboeyes : public CScrnWipe {
public:
	u8 _64[0x8]; // _64
};

// shakes the screen
class CScrnQuake {
public:
	CVec mOffset[3]; // _00
	u32 _30;         // _30
	// _34 vtable

	virtual ~CScrnQuake() { } // _08
};

// the info screen's fade and wipe
class InfoScrn {
public:
	CScrnFade mFade;      // _00
	CScrnWipe mWipe;      // _34
	u8 _98[0x1D8 - 0x98]; // _98
	// _1D8 vtable

	virtual ~InfoScrn() { } // _08
};

// the logo shown at boot
class Logo {
public:
	s32 mState;             // _00
	CTpl mTpl;              // _04
	u8 _1F4[0x200 - 0x1F4]; // _1F4
	// _200 vtable

	virtual ~Logo() { } // _08
};

// the sound player. game/sound.cpp has it as CSound, these are the fields this file uses
class CSound {
public:
	virtual ~CSound() { } // _08

	u32 _04; // _04
};

class XSound : public CSound {
public:
	CFile mFile;            // _08
	u8 _F4[0x2B4 - 0xF4];   // _F4
	s32 _2B4;               // _2B4
	u8 _2B8[0x2C8 - 0x2B8]; // _2B8
	s32 _2C8;               // _2C8
};

// the memory card, a CCard plus the save data
class CCard {
public:
	u8 _00[0xA020]; // _00
	// _A020 vtable

	virtual ~CCard() { } // _08
};

class Memcard {
public:
	u8 _00[0x20];               // _00
	CCard mCard;                // _20
	u8 _A044[0x16A20 - 0xA044]; // _A044
	// _16A20 vtable

	virtual ~Memcard() { } // _08
};

class CMsgData {
public:
	CMsgData()
	{
		mRead = NULL;
		_894  = 0;
	}

	u8 _00[0x800];          // _00
	u8* mRead;              // _800
	u8 _804[0x894 - 0x804]; // _804
	u32 _894;               // _894
	// _898 vtable

	virtual ~CMsgData() { } // _08
};

// a character of the message font, a texture and the sprite it's drawn with
struct MsgChar {
	CTexObj mTex;    // _00
	CSprite mSprite; // _100
	u8 _288[0x24];   // _288
};
typedef char MsgChar_size_check[sizeof(MsgChar) == 0x2AC ? 1 : -1];

struct MsgTex {
	CTexObj mTex; // _00
};

// the message font
class CMsgFont {
public:
	CMsgFont()
	{
		_43C80 = 0;
		_43CEC = -1;
		_43D0C = -1;
	}

	u8 _00[0x8];                  // _00
	MsgTex _08;                   // _08
	u8 _108[0x120 - 0x108];       // _108
	CursorHolder mCursor[1];      // _120
	u8 _AA8[0x4];                 // _AA8
	MsgChar mChars[400];          // _AAC
	CTexObj mTexes[5];            // _4376C
	u8 _43C6C[0x43C80 - 0x43C6C]; // _43C6C
	s32 _43C80;                   // _43C80
	u8 _43C84[0x43CEC - 0x43C84]; // _43C84
	s32 _43CEC;                   // _43CEC
	u8 _43CF0[0x43D0C - 0x43CF0]; // _43CF0
	s32 _43D0C;                   // _43D0C
	u8 _43D10[0x43D1C - 0x43D10]; // _43D10
	// _43D1C vtable

	virtual ~CMsgFont() { } // _08
};

// a message window, CGame has two
class CMsg {
public:
	u32 _00;                      // _00
	CMsgData mData;               // _04
	CMsgFont mFont;               // _8A0
	u8 _445C0[0x44618 - 0x445C0]; // _445C0
	// _44618 vtable

	virtual ~CMsg() { } // _08

	u32 _4461C; // _4461C
};
typedef char CMsgData_size_check[sizeof(CMsgData) == 0x89C ? 1 : -1];
typedef char CursorHolder_size_check[sizeof(CursorHolder) == 0x988 ? 1 : -1];
typedef char CMsgFont_size_check[sizeof(CMsgFont) == 0x43D20 ? 1 : -1];
typedef char CMsg_size_check[sizeof(CMsg) == 0x44620 ? 1 : -1];

// the title screen
class Title {
public:
	s32 mState;                // _00
	u8 _04[0x24];              // _04
	Cursor mCursor;            // _28
	XObjDemo mDemo;            // _9B0
	u8 _2F98[0x2FB8 - 0x2F98]; // _2F98
	CTpl mTpl;                 // _2FB8
	CTexObj mTex[6];           // _31A8
	u8 _37A8[0x3C48 - 0x37A8]; // _37A8
	// _3C48 vtable

	virtual ~Title() { } // _08
};

// the small window the screen is drawn into from a second camera
class FbSubwin : public CFbTexEff {
public:
	BOOL mOpen;      // _124
	CStatus _128;    // _128
	f32 mAlpha;      // _130, eases to 1 while it's open and back to 0
	u8 _134[0x10];   // _134
	CCamera mCamera; // _144
};
typedef char FbSubwin_size_check[sizeof(FbSubwin) == 0x2D8 ? 1 : -1];

// the frame buffer effects CGame owns, game/fbeffect.h has them with their dtors out of line
class CFbOutline {
public:
	u8 _00[0x60]; // _00
	// _60 vtable

	virtual ~CFbOutline() { } // _08
};

class CFbBlur {
public:
	u8 _00[0x40]; // _00
	// _40 vtable

	virtual ~CFbBlur() { } // _08
};

// a screen color with a texture to draw it through
struct ScrnTex {
	u32 _00;           // _00
	CScrnColor mColor; // _04
	CTexObj mTex;      // _38
};

// the objects of one kind, all the same size in one block
struct ObjPool {
	u8* mBase;         // _00
	s32 mUsed;         // _04, objects handed out so far
	s32 mCount;        // _08
	const char* mName; // _0C
	s32 mSize;         // _10, size of one object
};

// the objects: a pool per kind and the list of the ones in use, name from RTTI
class XOBJS {
public:
	ObjPool mPools[0x17];  // _000
	XObj* mObjects[0x400]; // _1CC
	s32 mObjectCount;      // _11CC
	s32 _11D0;             // _11D0
	s32 _11D4;             // _11D4
	// _11D8 vtable

	virtual ~XOBJS() { } // _08
};

// the object archives loaded so far
struct ObjArchiveTable {
	CObjArchive mArchives[0x400]; // _00
};

// the movie, the card and two of the screen colors sit in structs of their own
struct GameMovie {
	CMovie mMovie; // _00
};

struct GameMemcard {
	Memcard mCard; // _00
};

struct GameScrnColor {
	CScrnColor mColor; // _00
};

// the game: the title, the menus, the object table, the event scripts and the camera on top
// of CBase. name from RTTI
class CGame : public CBase {
public:
	u32 _3338;                                 // _3338
	CFileArchive mArchive;                     // _333C
	s32 mStage;                                // _3444
	u8 _3448[0x3C80 - 0x3448];                 // _3448
	CTpl mTpl;                                 // _3C80
	CScrnFade mFade;                           // _3E70
	CScrnWipeCircle mWipeCircle;               // _3EA4
	CScrnWipeTex mWipeTex;                     // _3F08
	CScrnWipeCustom mWipeCustom;               // _406C
	CScrnWipeTile mWipeTile0;                  // _40D0
	CScrnWipeTile mWipeTile1;                  // _4134
	CScrnRoboeyes mRoboeyes;                   // _4198
	CFbOutline mOutline;                       // _4204
	CFbBlur mBlur;                             // _4268
	FbSubwin mSubwin;                          // _42AC
	ScrnTex mScrnTex;                          // _4584
	GameScrnColor mColor0;                     // _46BC
	GameScrnColor mColor1;                     // _46F0
	u8 _4724[0x472C - 0x4724];                 // _4724
	Logo mLogo;                                // _472C
	Title mTitle;                              // _4930
	MenuEx mMenu;                              // _8580
	InfoScrn mInfoScrn;                        // _8F38
	CParticle mParticle;                       // _9114
	CScrnQuake mQuake;                         // _965C
	XOBJS mXObjs;                              // _9694
	XObj* mDrawList[0x400];                    // _A870
	u16 mDrawIndex[0x400];                     // _B870, each one's index in mObjects
	s32 mDrawCount;                            // _C070
	XObj* _C074[0x600];                        // _C074
	s32 _D874;                                 // _D874, count of _C074
	XObj* _D878[0x600];                        // _D878, the ones mirrors show
	s32 _F078;                                 // _F078, count of _D878
	ObjArchiveTable mArchives;                 // _F07C
	CEvt mEvt;                                 // _5D07C
	CMsg mMsgs[2];                             // _5D0E8
	u8 _E5D28[0xE5D38 - 0xE5D28];              // _E5D28
	CMotionControlCamera mMotionControlCamera; // _E5D38
	CCameraActions mCameraActions;             // _E5FAC
	CLight* mLightList;                        // _E7E20
	XSound mSound;                             // _E7E24
	u8 _E80F0[0xE8400 - 0xE80F0];              // _E80F0
	GameMemcard mMemcard;                      // _E8400
	u8 _FEE24[0xFF3A4 - 0xFEE24];              // _FEE24
	GameMovie mMovie;                          // _FF3A4
	BOOL mMovieSound;                          // _FF45C
	f32 _FF460[4];                             // _FF460

	CGame();
	virtual ~CGame();
};
typedef char CGame_size_check[sizeof(CGame) == 0xFF470 ? 1 : -1];

/**
 * @note Address: 0x800197A4
 * @note Size: 0x6B8
 */
CGame::CGame()
{
}

// the light sets and cameras the scenes start from. each one sets up the objects it gets and
// hands them back

/**
 * @note Address: 0x8001CEFC
 * @note Size: 0xD4
 */
extern "C" CLight* fn_8001CEFC(CLight* lights)
{
	CLight* light;

	lights[0].Init(4, FALSE);
	lights[0].mColor.r = 0x80;
	lights[0].mColor.g = 0x80;
	lights[0].mColor.b = 0x80;
	lights[0].mColor.a = 0xFF;
	HSD_LObjSetColor(&lights[0], lights[0].mColor);

	lights[0].next = &lights[1];
	light          = &lights[1];
	light->Init(5, FALSE);
	light->SetPosition(1.0f, -1.0f, 1.0f);
	light->mColor.r = 0xFF;
	light->mColor.g = 0xFF;
	light->mColor.b = 0xFF;
	light->mColor.a = 0xFF;
	HSD_LObjSetColor(light, light->mColor);
	light->next = NULL;
	return lights;
}

/**
 * @note Address: 0x8001CFD0
 * @note Size: 0x1D0
 */
extern "C" CLight* fn_8001CFD0(CLight* lights)
{
	CLight* light;

	lights[0].Init(4, FALSE);
	lights[0].mColor.r = 200;
	lights[0].mColor.g = 200;
	lights[0].mColor.b = 200;
	lights[0].mColor.a = 0xFF;
	HSD_LObjSetColor(&lights[0], lights[0].mColor);

	lights[0].next = &lights[1];
	light          = &lights[1];
	light->Init(5, TRUE);
	light->SetPosition(-2.0f, 20.0f, 2.0f);
	light->mColor.r = 200;
	light->mColor.g = 200;
	light->mColor.b = 200;
	light->mColor.a = 0xFF;
	HSD_LObjSetColor(light, light->mColor);

	light->next = NULL;

	light = &lights[2];
	light->Init(6, FALSE);
	light->SetPosition(0.0f, 20.0f, -20.0f);
	light->mColor.r = 0;
	light->mColor.g = 0xFF;
	light->mColor.b = 0xFF;
	light->mColor.a = 0xFF;
	HSD_LObjSetColor(light, light->mColor);

	light->next = NULL;

	light = &lights[3];
	light->Init(7, FALSE);
	light->SetPosition(0.0f, 20.0f, 0.0f);
	light->SetInterest(0.0f, 0.0f, 20.0f);
	light->mColor.r = 0xFF;
	light->mColor.g = 0;
	light->mColor.b = 0;
	light->mColor.a = 0xFF;
	HSD_LObjSetColor(light, light->mColor);
	light->next = NULL;
	return lights;
}

/**
 * @note Address: 0x8001D1D0
 * @note Size: 0xCC
 */
extern "C" CCamera* fn_8001D1D0(CCamera* camera, GXRenderModeObj* rmode)
{
	camera->Init(0.0f, 10000.0f, rmode);
	camera->SetScale(1.0f, 1.0f, 1.0f);
	camera->SetEye(0.0f, 0.0f, 5000.0f);
	camera->SetInterest(0.0f, 0.0f, 0.0f);
	camera->CalcAngle();
	camera->CalcDistance();
	return camera;
}

/**
 * @note Address: 0x8001D2D0
 * @note Size: 0xF4
 */
extern "C" CCamera* fn_8001D2D0(CCamera* camera)
{
	camera->Init(1.0f, 80000.0f, 30.0f, (GXRenderModeObj*)&lbl_8025C400);
	camera->SetScale(1.0f, 1.0f, 1.0f);
	camera->SetInterest(-4.2f, 0.0f, -64.0f);
	camera->SetAngle(0.41887903f, -3.1415927f, 0.0f);
	camera->SetDistance(400.0f);
	camera->SetFov(18.0f);
	camera->CalcEye();
	return camera;
}

/**
 * @note Address: 0x8001D3D4
 * @note Size: 0xEC
 */
extern "C" CCamera* fn_8001D3D4(CCamera* camera, GXRenderModeObj* rmode)
{
	camera->Init(1.0f, 80000.0f, 30.0f, rmode);
	camera->SetScale(1.0f, 1.0f, 1.0f);
	camera->SetInterest(-4.2f, 0.0f, -64.0f);
	camera->SetAngle(0.41887903f, -3.1415927f, 0.0f);
	camera->SetDistance(400.0f);
	camera->SetFov(18.0f);
	camera->CalcEye();
	return camera;
}

// the destructors of the objects the game keeps outside CGame

// two files read ahead into aram, name from RTTI
class ARPreCache {
public:
	u32 _00;      // _00
	CFile mFile0; // _04
	CFile mFile1; // _F0
	u8 _1DC[0x8]; // _1DC
	// _1E4 vtable

	virtual ~ARPreCache(); // _08
};

/**
 * @note Address: 0x80019554
 * @note Size: 0x74
 */
ARPreCache::~ARPreCache()
{
}

/**
 * @note Address: 0x800195C8
 * @note Size: 0x38
 */
// the five mirrors' static destructor
extern "C" FbMirrorEx lbl_8035FA24[5];
extern "C" void* __dt__10FbMirrorExFv(void* mirror, s16 del);
extern "C" void fn_800195C8(void)
{
	__destroy_arr(lbl_8035FA24, __dt__10FbMirrorExFv, sizeof(FbMirrorEx), 5);
}

/**
 * @note Address: 0x80019600
 * @note Size: 0x100
 */
FbMirrorEx::~FbMirrorEx()
{
}

/**
 * @note Address: 0x8001BC34
 * @note Size: 0xC8
 */
// sets the mirror's camera up for its 256 square texture
extern "C" void fn_8001BC34(FbMirrorEx* mirror)
{
	mirror->_124 = 0;
	mirror->mCamera.Init(0.1f, 32768.0f, 0.5f, -0.5f, -0.5f, 0.5f, (GXRenderModeObj*)&lbl_8025C400);
	mirror->mCamera.SetViewport(0.0f, 256.0f, 0.0f, 256.0f);
	mirror->mCamera.SetScissor(0, 0x100, 0, 0x100);
	mirror->Reset();
	mirror->_2F0 = 0;
}

// the progressive scan question at boot, kept between resets. class unknown
struct ResetState {
	s32 _00; // _00, the step
	s32 _04; // _04, the scan mode to switch to, -1 for none
	f32 _08; // _08, frames left
	f32 _0C; // _0C, the question's alpha
	s32 _10; // _10, it was asked before the reset
	s32 _14; // _14, interlaced was picked
};
extern "C" ResetState lbl_80210730;

/**
 * @note Address: 0x8001C89C
 * @note Size: 0x28
 */
extern "C" void fn_8001C89C(void)
{
	lbl_80210730._00 = 0;
	lbl_80210730._04 = -1;
	lbl_80210730._08 = 0.0f;
	lbl_80210730._10 = 0;
	lbl_80210730._14 = 0;
}

// the pad as the progressive question reads it
struct ResetPad {
	u8 _00[0x4];  // _00
	u32 mButton;  // _04
	u8 _08[0xC];  // _08
	u32 mTrigger; // _14
	u32 mRepeat;  // _18
};

extern "C" {
u32 VIGetDTVStatus(void);
u32 OSGetResetCode(void);
u32 OSGetProgressiveMode(void);
void OSSetProgressiveMode(u32 mode);
void fn_8019B850(void* gs, s32 a, s32 b, s32 c);
}
extern "C" s32* lbl_8065B178;

/**
 * @note Address: 0x8001C2C4
 * @note Size: 0x590
 */
// the progressive scan question: asked when the cable allows it and the console is already
// in progressive mode or B is held, the answer goes to the scan mode and the saved setting
extern "C" BOOL fn_8001C2C4(u32 frames)
{
	CPad pad(0);

	switch (lbl_80210730._00) {
	case 0:
		lbl_80210730._00 = 100;
		break;
	case 100:
		if (VIGetDTVStatus() != 0) {
			if ((OSGetResetCode() >> 31) && lbl_8065B178[0]) {
				lbl_80210730._00 = 10;
				lbl_80210730._10 = lbl_8065B178[0];
				lbl_80210730._14 = lbl_8065B178[1];
				break;
			}
			BOOL ask = FALSE;
			if (OSGetProgressiveMode() == 1) {
				ask = TRUE;
			} else if (((ResetPad*)pad.mPad)->mButton & 0x200) {
				ask = TRUE;
			}
			if (ask) {
				lbl_80210730._08 = 600.0f;
				lbl_80210730._00 = 1;
				lbl_80210730._0C = 0.0f;
			} else {
				lbl_80210730._00 = 0;
				OSSetProgressiveMode(0);
				lbl_8065B178[0] = TRUE;
				lbl_8065B178[1] = TRUE;
			}
		} else {
			lbl_80210730._00 = 0;
			OSSetProgressiveMode(0);
			lbl_8065B178[0] = TRUE;
			lbl_8065B178[1] = TRUE;
		}
		break;
	case 1: {
		if (lbl_80210730._0C < 1.0f && (lbl_80210730._0C += 0.11111111f * frames) > 1.0f) {
			lbl_80210730._0C = 1.0f;
		}
		BOOL done = FALSE;
		if ((lbl_80210730._08 -= frames) <= 0.0f) {
			done = TRUE;
		} else if (((ResetPad*)pad.mPad)->mTrigger & 0x100) {
			done = TRUE;
		} else if (((ResetPad*)pad.mPad)->mRepeat & 1) {
			lbl_80210730._14 = FALSE;
		} else if (((ResetPad*)pad.mPad)->mRepeat & 2) {
			lbl_80210730._14 = TRUE;
		}
		if (done) {
			lbl_80210730._00 = 10;
		}
		break;
	}
	case 10:
		if (lbl_80210730._0C > 0.0f && (lbl_80210730._0C -= 0.11111111f * frames) < 0.0f) {
			lbl_80210730._0C = 0.0f;
		}
		if (lbl_80210730._0C == 0.0f) {
			if (!lbl_80210730._14) {
				lbl_80210730._00 = 2;
				lbl_80210730._08 = 2.0f;
				HSD_VISetBlack(TRUE);
			} else {
				lbl_80210730._00 = 4;
				lbl_80210730._04 = lbl_80210730._14 ? FALSE : TRUE;
				lbl_80210730._08 = 600.0f;
			}
		}
		break;
	case 2:
		if ((lbl_80210730._08 -= frames) <= 0.0f) {
			lbl_80210730._00 = 3;
			lbl_80210730._04 = lbl_80210730._14 ? FALSE : TRUE;
			lbl_80210730._08 = 120.0f;
		}
		break;
	case 3:
		if ((lbl_80210730._08 -= frames) <= 0.0f) {
			lbl_80210730._00 = 4;
			lbl_80210730._08 = 600.0f;
			HSD_VISetBlack(FALSE);
		}
		break;
	case 4:
		if ((OSGetResetCode() >> 31) && lbl_80210730._10) {
			lbl_80210730._00 = 40;
		} else {
			if (lbl_80210730._0C < 1.0f && (lbl_80210730._0C += 0.11111111f * frames) > 1.0f) {
				lbl_80210730._0C = 1.0f;
			}
			BOOL done = FALSE;
			if ((lbl_80210730._08 -= frames) <= 0.0f) {
				done = TRUE;
			} else if (((ResetPad*)pad.mPad)->mTrigger & 0x100) {
				done = TRUE;
			}
			if (done) {
				lbl_80210730._00 = 40;
			}
		}
		break;
	case 40:
		if (lbl_80210730._0C > 0.0f && (lbl_80210730._0C -= 0.11111111f * frames) < 0.0f) {
			lbl_80210730._0C = 0.0f;
		}
		if (lbl_80210730._0C == 0.0f) {
			lbl_80210730._00 = 0;
		}
		break;
	}

	if (lbl_80210730._04 >= 0) {
		if (lbl_80210730._04 != 0) {
			OSSetProgressiveMode(1);
			fn_8019B850(&lbl_8025C400, 8, 0, 16);
		} else {
			OSSetProgressiveMode(0);
		}
		lbl_80210730._04 = -1;
		lbl_8065B178[0]  = TRUE;
		lbl_8065B178[1]  = lbl_80210730._14;
	}
	return !lbl_80210730._00;
}

// the system setup the game runs before anything else

extern "C" {
void srand(u32 seed);
u32 OSGetTick(void);
void* OSInitAlloc(void* arenaStart, void* arenaEnd, int maxHeaps);
u32 OSGetResetCode(void);
void OSSetSaveRegion(void* start, void* end);
void DVDSetAutoFatalMessaging(BOOL enable);
BOOL DVDChangeDir(char* dirName);
u32 ARInit(u32* stackIndexAddr, u32 numEntries);
void ARQInit(void);
void HSD_VIDrawDoneXFB(s32 idx);
}

/**
 * @note Address: 0x8001D4C0
 * @note Size: 0x2C
 */
extern "C" void fn_8001D4C0(void)
{
	DVDSetAutoFatalMessaging(TRUE);
	DVDChangeDir((char*)"/qp/");
}

extern "C" u32 lbl_80361E10[8];

/**
 * @note Address: 0x8001D4EC
 * @note Size: 0x30
 */
extern "C" void fn_8001D4EC(void)
{
	ARInit(lbl_80361E10, 8);
	ARQInit();
}

/**
 * @note Address: 0x8001D8A0
 * @note Size: 0x20
 */
extern "C" void fn_8001D8A0(s32 idx)
{
	HSD_VIDrawDoneXFB(idx);
}

// the two words the game keeps over a reset
extern "C" s32* lbl_8065B178;

/**
 * @note Address: 0x8001D8C0
 * @note Size: 0xE8
 */
extern "C" void fn_8001D8C0(void)
{
	OSInitFastCast();
	srand(OSGetTick());
	void* lo = OSGetArenaLo();
	void* hi = OSGetArenaHi();
	OSSetArenaLo(OSInitAlloc(lo, hi, 1));
	HSD_SetInitParameter(HSD_INIT_FIFO_SIZE, 0x40000);
	HSD_SetInitParameter(HSD_INIT_XFB_MAX_NUM, 3);
	HSD_Init();
	mm::Init();
	lbl_8065B178 = (s32*)OSAllocFromHeap(__OSCurrHeap, 8);
	if (!(OSGetResetCode() >> 31)) {
		lbl_8065B178[0] = 0;
		lbl_8065B178[1] = 1;
	}
	OSSetSaveRegion(lbl_8065B178, lbl_8065B178 + 2);
}

/**
 * @note Address: 0x8001D9A8
 * @note Size: 0x8
 */
extern "C" s32 fn_8001D9A8(u8* obj)
{
	return *(s32*)(obj + 0x44);
}

/**
 * @note Address: 0x8001DB44
 * @note Size: 0x4
 */
extern "C" void fn_8001DB44(void* obj)
{
}

// a run of frames, stepped round one at a time
struct FrameLoop {
	u8 _00[0x10]; // _00
	s32 mBase;    // _10
	u8 _14;       // _14
	u8 mCount;    // _15
	u8 mFrame;    // _16
};

/**
 * @note Address: 0x8001DB48
 * @note Size: 0x48
 */
extern "C" s32 fn_8001DB48(void* obj, FrameLoop* loop)
{
	if (loop->mCount == 0) {
		return loop->mBase;
	}
	loop->mFrame++;
	if (loop->mCount <= loop->mFrame) {
		loop->mFrame = 0;
	}
	return loop->mBase + loop->mFrame;
}

/**
 * @note Address: 0x8001DB90
 * @note Size: 0x8
 */
extern "C" s32 fn_8001DB90(void* obj)
{
	return 0;
}

// two values and whether they count
struct RangePair {
	u8 _00[0x4]; // _00
	f32 mA;      // _04
	f32 mB;      // _08
	s32 mValid;  // _0C, 1 when they do
};

/**
 * @note Address: 0x8001DB98
 * @note Size: 0x78
 */
extern "C" BOOL fn_8001DB98(void* obj, RangePair* pair, f32 min, f32 max)
{
	f32 a = pair->mA;
	f32 b = pair->mB;

	switch (pair->mValid) {
	case 1:
		if (0.0f <= a && min <= a && a < max) {
			return TRUE;
		}
		if (0.0f <= b && min <= b && b < max) {
			return TRUE;
		}
		break;
	}
	return FALSE;
}

/**
 * @note Address: 0x8001DC10
 * @note Size: 0x10
 */
extern "C" void fn_8001DC10(u8* obj)
{
	obj[0x1A1] = 0;
	obj[0x1A0] = 0;
}

/**
 * @note Address: 0x8001A45C
 * @note Size: 0x190
 */
CCameraActionMap::CCameraActionMap()
{
}

/**
 * @note Address: 0x80019E5C
 * @note Size: 0x488
 */
CCameraActions::CCameraActions()
{
}

// the sound object
#define SOUND() ((XSound*)((u8*)&lbl_8025C400 + 0xE7E24))

extern "C" {
BOOL fn_801E62A8(s32 sfx);
void fn_801E9FA4(void);
BOOL fn_8001E904(XSound* sound, s32 sfx, u8 vol);
}

// ids with the high half 0xFFFF but not -1 mean every sfx, see game/sound.cpp
static inline bool IsAllSfx(s32 sfx)
{
	if (sfx == -1) {
		return false;
	}
	return (sfx & 0xFFFF0000) == 0xFFFF0000;
}

// CSound's stop, fn_8001E660 in the dol
static inline BOOL StopSfx(XSound* sound, s32 sfx)
{
	if (IsAllSfx(sfx)) {
		sound->_2B4 = 0;
		sound->_2C8 = 0;
		fn_801E9FA4();
		return TRUE;
	}
	return fn_801E62A8(sfx);
}

// one sfx an animation plays between two of its frames
struct AnimSfx {
	s32 mAnim;  // _00, -1 ends the table
	f32 mStart; // _04
	f32 mEnd;   // _08
	s32 mType;  // _0C, 1 for a frame range
	s32 mSfx;   // _10
	u8 _14;     // _14
	u8 mCount;  // _15, more than one steps through that many ids from mSfx
	u8 mStep;   // _16
	u8 _17;     // _17
};

// plays the sfx of an object's animations as the frames go by, class unknown
class AnimSfxPlayer {
public:
	AnimSfx* mTable; // _00
	s32 mAnim;       // _04
	f32 mFrame;      // _08
	// _0C vtable

	virtual ~AnimSfxPlayer();                               // _08
	virtual void vf0C();                                    // _0C
	virtual BOOL IsInRange(AnimSfx* sfx, f32 from, f32 to); // _10
	virtual BOOL IsLooped(AnimSfx* sfx);                    // _14
	virtual s32 GetSfx(AnimSfx* sfx);                       // _18
};

/**
 * @note Address: 0x8001D9B0
 * @note Size: 0x194
 */
extern "C" void fn_8001D9B0(AnimSfxPlayer* player, s32 anim, f32 frame, u8 vol)
{
	s32 prev = player->mAnim;

	if (prev != anim) {
		player->mAnim  = anim;
		player->mFrame = 0.0f;
	}
	AnimSfx* sfx;
	for (s32 i = 0; player->mTable != NULL && (sfx = &player->mTable[i])->mAnim >= 0; i++) {
		if (sfx->mAnim == anim || sfx->mAnim == prev) {
			if (prev != anim && sfx->mAnim == prev && player->IsLooped(sfx)) {
				StopSfx(SOUND(), player->GetSfx(sfx));
			}
			if (sfx->mAnim == anim && player->IsInRange(sfx, player->mFrame, frame)) {
				fn_8001E904(SOUND(), player->GetSfx(sfx), vol);
			}
		}
	}
	player->mFrame = frame;
}
