#ifndef _GAME_CGAME_SHARED_H
#define _GAME_CGAME_SHARED_H

#include "game/hsd_jobj.h"
#include "game/cparticle.h"
#include "game/cjobjset.h"
#include "game/cgs.h"
#include "game/ccamera.h"
#include "game/clight.h"
#include "game/cobj.h"
#include "game/mm.h"
#include "game/hsd_dobj.h"
#include "game/gmath.h"
#include <dolphin/os.h>
#include <Dolphin/vi.h>
#include <math.h>

// the game core: the CBase and CGame classes that the main loop drives, and the weak copies
// of the inline functions they pull in. the destructors and the interpolation methods are the
// compiler's own copies, renamed in symbols.txt; the copies whose class is still unknown are
// extern "C" stand-ins under their dol address

extern "C" {
void HSD_MtxGetTranslate(Mtx mtx, Vec* translate);
void HSD_VISetBlack(u8 black);
void fn_8006F338(s32 arg0, s32 arg1, s32 arg2);
u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, GXBool mipmap, u8 maxLod);
void PSVECScale(const Vec* src, Vec* dst, f32 scale);
void HSD_VICopyXFBAsync(s32 pri);
void OSResetStopwatch(void* sw);
void OSStartStopwatch(void* sw);
void OSStopStopwatch(void* sw);
s64 OSCheckStopwatch(void* sw);
}

extern "C" {
#include <sysdolphin/controller.h>
}

// game/cgs.h has CGs's dtor out of line, its weak copy is in this file
inline CGs::~CGs()
{
}

// the camera actions. game/ccameraaction.h has them with the interpolations as padding, this
// file destroys them so it spells the members out. names from RTTI

class XObj;
class XObjCamera;
class CMotionControlCamera;

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

// a record in CamAreaState's _1C and _24 lists
struct CamAreaRecord {
	u8 _00[0x4];   // _00
	s32 _04[4][2]; // _04, 4 corners as x z pairs, in hundredths
	u8 _24[0x8];   // _24
	s16 _2C;       // _2C
	u16 _2E;       // _2E, flags
	u8 _30[0x4];   // _30
};

// wraps a CamAreaRecord
class CamAreaInfo {
public:
	CamAreaInfo(CamAreaRecord* record)
	    : _04(record)
	{
	}
	virtual ~CamAreaInfo() { } // _08

	bool Check(u16 mask) const
	{
		u16 flags = _04->_2E & mask;
		return flags != 0;
	}

	// _00 vtable
	CamAreaRecord* _04; // _04
};

// camera area state, class unknown
struct CamAreaState {
	u8 _00[0x10];       // _00
	f32 _10;            // _10
	s32 _14;            // _14
	s32 _18;            // _18
	CamAreaRecord* _1C; // _1C
	s32 _20;            // _20, records in _1C
	CamAreaRecord* _24; // _24
	s32 _28;            // _28, records in _24
};
extern "C" CamAreaState lbl_805A054C;

extern "C" {
void fn_80137EB8(CamAreaState* state);
void fn_801374F4(CamAreaState* state, Vec pos);
}

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

	CCameraActionMap();
	virtual ~CCameraActionMap();
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

	void SetObj(XObjCamera* obj);
};
typedef char CCameraActionAnim_size_check[sizeof(CCameraActionAnim) == 0x48 ? 1 : -1];

extern "C" {
void HSD_PadRumbleOffN(u8 no);
void HSD_PadRumblePauseAll(void);
}

class CPadEx {
public:
	HSD_PadData mQueue;               // _00
	HSD_PadRumbleListData mRumble[4]; // _34
	CPadOne mPad[4];                  // _B4
	// _204 vtable

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

// what every game is built on: the graphics state, font, pads, two cameras, six lights and
// three stopwatches for the frame timings. name from RTTI
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

	// the disc can't be read
	bool IsDiscError() const
	{
		bool error = false;
		if ((mDriveStatus != 0 || mLastDriveStatus != 0) && !mDiskIn) {
			error = true;
		}
		return error;
	}
	// a stage is up and running, nothing stops it
	bool IsRunning() const
	{
		bool running = false;
		if ((mStatus.mFlags & 1) && !(mStatus.mFlags & 0x22)) {
			if (!IsDiscError()) {
				running = true;
			}
		}
		return running;
	}
};

// the classes the game objects are built from, only as far as their destructors show them.
// names from RTTI

// an object's script position, game/xobjs.h has it too
class CoordInfo {
public:
	void* mValue; // _00
	// _04 vtable

	virtual ~CoordInfo() { } // _08
};

// an entry of objTable or objTableEx
struct ObjData {
	u8 _00[0x14]; // _00
	s16 mKind;    // _14
};

class AttrInfo {
public:
	ObjData* mData; // _00
	s32* _04;       // _04
	// _08 vtable

	virtual ~AttrInfo() { } // _08

	s32 GetDataKind() const { return mData ? mData->mKind : 0; }
	s16 GetKind() const { return mData ? GetDataKind() : -1; }
};

class CEvtData {
public:
	void* mScript; // _00, the script running
	u8 _04[0x4];   // _04
	// _08 vtable

	virtual ~CEvtData() { } // _08

	u8 _0C[0x20 - 0xC]; // _0C
};

class CStack {
public:
	u8 _00[0xC]; // _00
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

// a CJArchive that keeps its animations

// a game object
class XObj : public CXObjIntp {
public:
	CStatus64 _228;         // _228
	CoordInfo mCoord;       // _238
	AttrInfo mAttr;         // _240
	u8 _24C[0x264 - 0x24C]; // _24C
	CVec _264;              // _264
	u8 _274[0x6F8 - 0x274]; // _274
	f32 _6F8;               // _6F8
	u8 _6FC[0x77C - 0x6FC]; // _6FC
	f32 _77C;               // _77C, a height, nan when there's none
	u8 _780[0x794 - 0x780]; // _780
	CVec _794;              // _794
	CEvt mEvt;              // _7A4
	CAnmQueue mAnmQueue;    // _810
	u8 _898[0x8FC - 0x898]; // _898
	CJAnim mAnim;           // _8FC
	u8 _980[0x988 - 0x980]; // _980

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
	virtual void vf120(s32 pass);                           // _120, 1 before a mirrored draw, 2 after
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
typedef char XObj_size_check[sizeof(XObj) == 0x988 ? 1 : -1];

// the menu cursor objects add nothing to XObj but their vtables
class Cursor : public XObj { };
class MenuCursor : public Cursor { };

// an object with a camera and lights of its own for the title screen
class XObjDemo : public XObj {
public:
	CCamera mCamera;   // _988
	u8 _B1C[0x8];      // _B1C
	CLight mLights[8]; // _B24
};

// the camera the game draws with, it runs the current camera action and eases towards it.
// game/cmotioncontrolcamera.h has the methods
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

	void Init();
	void Update(u32 frames);
	BOOL IsAction(s32 type);
	void ChangeAction(s32 type);
	void ApplyAction();
	void UpdateTarget(XObj* obj);

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

	void EndDraw();
	s32 BeginDraw();
	void Draw(f32 x, f32 y, f32 w, f32 h);
	s32 DrawFrame(f32 x, f32 y, f32 w, f32 h);
	s32 Update();
	void Stop();
	void Start(const char* path, BOOL loop, BOOL alpha, BOOL sound);
};
typedef char CMovie_size_check[sizeof(CMovie) == 0xB8 ? 1 : -1];

// the objects of one kind, all the same size in one block
struct ObjPool {
	u8* mBase;         // _00
	s32 mUsed;         // _04, objects handed out so far
	s32 mCount;        // _08
	const char* mName; // _0C
	s32 mSize;         // _10, size of one object

	XObj* First() { return (XObj*)mBase; }
	XObj* Get(s32 i) { return (XObj*)(mBase + i * mSize); }
};

// the two objects that can bring lights of their own, classes unknown
struct LightOwnerA {
	u8 _00[0x228];          // _00
	CStatus64 _228;         // _228
	u8 _238[0xB84 - 0x238]; // _238
	s32 mLightCount;        // _B84
	CLight mLights[1];      // _B88

	CLight* GetLights() { return mLightCount > 0 ? mLights : NULL; }
};

struct LightOwnerB {
	u8 _00[0x228];          // _00
	CStatus64 _228;         // _228
	u8 _238[0x980 - 0x238]; // _238
	s32 mLightCount;        // _980
	CLight mLights[1];      // _984

	CLight* GetLights() { return mLightCount > 0 ? mLights : NULL; }
};

// a menu, its cursor and not much else
struct MenuEx {
	s32 mOpen;          // _00
	void* _04;          // _04
	void* _08;          // _08, the menu that is up
	u8 _0C[0x28 - 0xC]; // _0C
	MenuCursor mCursor; // _28
	// _9B0 vtable

	virtual ~MenuEx() { } // _08
};

// a struct holding a cursor, CMsgFont has one
struct CursorHolder {
	Cursor mCursor; // _00
};

// the screen overlays: a CScrnColor fills the screen, the fades and wipes move it in and
// out. names from RTTI
class CScrnColor {
public:
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

	// these two are CScrnColor's own, see game/gxb.cpp
	virtual void vf0C();             // _0C
	virtual void Update(u32 frames); // _10
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

// the sound player, game/xsound.h has the fields it uses
class CSound {
public:
	virtual ~CSound() { } // _08

	u32 _04; // _04
};

class XSound : public CSound {
public:
	CFile mFile; // _08
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
	u8 _00[0x20];                 // _00
	CCard mCard;                  // _20
	u8 _A044[0x16A08 - 0xA044];   // _A044
	BOOL mBusy;                   // _16A08, a card task is running
	u8 _16A0C[0x16A20 - 0x16A0C]; // _16A0C
	// _16A20 vtable

	virtual ~Memcard() { } // _08
};

class CMsgData {
public:
	u8 _00[0x898]; // _00
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

struct MsgTexes {
	CTexObj mTex[5]; // _00
};

// the message font
class CMsgFont {
public:
	u8 _00[0x8];                  // _00
	MsgTex _08;                   // _08
	u8 _108[0x118 - 0x108];       // _108
	GXColor mColor;               // _118
	u8 _11C[0x120 - 0x11C];       // _11C
	CursorHolder mCursor[1];      // _120
	u8 _AA8[0x4];                 // _AA8
	MsgChar mChars[400];          // _AAC
	MsgTexes mTexes;              // _4376C
	u8 _43C6C[0x43D1C - 0x43C6C]; // _43C6C
	// _43D1C vtable

	virtual ~CMsgFont() { } // _08
};

// a message window, CGame has two. its vtable is unnamed
class CMsg {
public:
	u32 _00;                      // _00
	CMsgData mData;               // _04
	CMsgFont mFont;               // _8A0
	u8 _445C0[0x44618 - 0x445C0]; // _445C0
	// _44618 vtable

	virtual ~CMsg() { } // _08

	void SetColor(GXColor color) { mFont.mColor = color; }

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

// the frame buffer effects that copy the screen into a texture. names from RTTI
// the texture an effect copies into
struct FbTexBuffer {
	void* mBuffer;    // _00
	u16 mWidth;       // _04
	u16 mHeight;      // _06
	GXTexFmt mFormat; // _08
	s32 _0C;          // _0C
	f32 _10;          // _10
	f32 _14;          // _14
	u8 _18;           // _18, copies at half size
	CTexObj mTex;     // _1C
};

class CFbTexEff : public FbTexBuffer {
public:
	BOOL _11C; // _11C
	// _120 vtable

	virtual void Init();             // _08
	virtual void Reset();            // _0C
	virtual void vf10();             // _10
	virtual void Update(u32 frames); // _14
	virtual void Draw();             // _18
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

// a mirror, drawn from a camera of its own into a 256 square texture
class FbMirror : public CFbTexEff {
public:
	HSD_JObj* mJObj; // _124, the mirror's surface
	CCamera mCamera; // _128
	BOOL mVisible;   // _2BC, the eye is in front of it
	Mtx mViewMtx;    // _2C0
	XObj* mOwner;    // _2F0, the mirror is off while this is gone or hidden

	bool IsUsed() const { return mJObj ? true : false; }
	bool IsOff()
	{
		bool off = false;
		if (mOwner == NULL || (mOwner->_228.mFlags & 0x1000000000000)) {
			off = true;
		}
		return off;
	}
};
typedef char FbMirror_size_check[sizeof(FbMirror) == 0x2F4 ? 1 : -1];
extern "C" FbMirror lbl_8035FA24[5];

// every camera action and the table the camera picks them from. a class with its vtable at
// _1E70, its dtor is spelled out below
struct CCameraActions {
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

	virtual ~CCameraActions() { } // _08
};
typedef char CCameraActions_size_check[sizeof(CCameraActions) == 0x1E74 ? 1 : -1];

// the frame buffer effects CGame owns, game/fbeffect.h has them with their dtors out of line
class CFbOutline {
public:
	u8 _00[0x28];    // _00
	GXColor mColor0; // _28
	u8 _2C[0x4];     // _2C
	GXColor mColor1; // _30
	GXColor mColor2; // _34
	GXColor mColor3; // _38
	f32 _3C;         // _3C
	u8 _40[0x1C];    // _40
	f32 _5C;         // _5C
	// _60 vtable

	virtual ~CFbOutline() { } // _08

	void Update(s32 frames);
	void SetGlow(u8 r, u8 g, u8 b, u8 a)
	{
		GXColor color = { r, g, b, 4 };
		mColor0       = color;
		mColor0.a     = a;
	}
	void SetTexColor(u8 r, u8 g, u8 b)
	{
		GXColor color = { r, g, b, 0x80 };
		mColor1       = color;
	}
	void SetColor2(u8 r, u8 g, u8 b, u8 a)
	{
		GXColor color = { r, g, b, a };
		mColor2       = color;
	}
	void SetColor3(u8 r, u8 g, u8 b, u8 a)
	{
		GXColor color = { r, g, b, a };
		mColor3       = color;
	}
};

class CFbBlur {
public:
	u8 _00[0x28]; // _00
	f32 _28;      // _28
	f32 _2C;      // _2C
	u8 _30[0x10]; // _30
	// _40 vtable

	virtual ~CFbBlur() { } // _08

	void Update(u32 frames);
};

// a screen color with a texture to draw it through
struct ScrnTex {
	f32 mAngle;        // _00
	CScrnColor mColor; // _04
	CTexObj mTex;      // _38
};

// the objects: a pool per kind and the list of the ones in use, name from RTTI. game/xobjs.cpp
// has it as UnkXObjList
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

// the movie, the card and two of the screen colors sit in structs of their own, CGame's dtor
// checks each twice
struct GameMovie {
	CMovie mMovie; // _00
};

struct GameMemcard {
	Memcard mCard; // _00
};

struct GameScrnColor {
	CScrnColor mColor; // _00
};

// one letter of the stage name that pops in when a stage starts
struct StageNameChar {
	const char* mText; // _00, the letter, one or two bytes
	f32 mScale;        // _04, grows to 1 and shrinks away
	f32 mWait;         // _08, frames until it shrinks
	f32 mBob;          // _0C, the angle it bobs up and down by
	f32 mTilt;         // _10, the angle it rocks by
};

// the stage name, each letter on its own over a bubble
struct StageName {
	BOOL mActive;             // _00
	s32 mCount;               // _04
	u32 mColor;               // _08, the letters' color, the alpha comes from their scale
	StageNameChar mChars[20]; // _0C
};
typedef char StageName_size_check[sizeof(StageName) == 0x19C ? 1 : -1];

// the game: the title, the menus, the object table, the event scripts and the camera on top
// of CBase. name from RTTI
class CGame : public CBase {
public:
	s32 _3338;                                 // _3338
	CFileArchive mArchive;                     // _333C
	s32 mStage;                                // _3444
	s32 _3448;                                 // _3448, the stage before
	s32 mStageCount;                           // _344C
	u8 _3450[0x3C80 - 0x3450];                 // _3450
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
	s32 _4724;                                 // _4724
	f32 _4728;                                 // _4728, the outline's width, the c stick moves it
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
	CMsg mCMsgs[2];                            // _5D0E8
	s32 _E5D28;                                // _E5D28
	f32 _E5D2C;                                // _E5D2C, the idle timer, fills to 1
	f32 _E5D30;                                // _E5D30, how many frames it takes, 0 for never
	BOOL _E5D34;                               // _E5D34, the idle script is due
	CMotionControlCamera mMotionControlCamera; // _E5D38
	CCameraActions mCameraActions;             // _E5FAC
	CLight* mLightList;                        // _E7E20
	XSound mSound;                             // _E7E24
	u8 _E7F18[0xE8400 - 0xE7F18];              // _E7F18
	GameMemcard mMemcard;                      // _E8400
	u8 _FEE24[0xFF000 - 0xFEE24];              // _FEE24
	BOOL mMemcardLoaded;                       // _FF000, the stage came from the card
	u8 _FF004[0xFF200 - 0xFF004];              // _FF004
	s32 mPlayTime;                             // _FF200, frames played, it stops just short of 100 hours
	u8 _FF204[0x4];                            // _FF204
	StageName mStageName;                      // _FF208
	GameMovie mMovie;                          // _FF3A4
	BOOL mMovieSound;                          // _FF45C
	f32 _FF460[4];                             // _FF460

	virtual ~CGame();

	bool Is472C() const { return mLogo.mState >= 1 && mLogo.mState < 15; }
	void SetStage(s32 stage)
	{
		_3448  = mStage;
		mStage = stage;
	}
	bool Is4930() const { return mTitle.mState >= 1 && mTitle.mState < 23; }
};
typedef char CGame_size_check[sizeof(CGame) == 0xFF470 ? 1 : -1];

extern "C" CGame lbl_8025C400;

extern "C" {
MtxPtr fn_8019997C(CBase* base);
void fn_80199944(CGs* gs);
void fn_8019B648(CGsRenderMode* mode, u8 enable);
void fn_8019B5D0(CGsRenderMode* mode, u8 enable);
void fn_8019B56C(CGsRenderMode* mode, s32 type, s32 src, s32 dst, s32 op);
void fn_8019B504(CGsRenderMode* mode, s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
void fn_8019B464(CGsRenderMode* mode, u8 enable, s32 func, u8 update);
void fn_8019B360(CGsRenderMode* mode, s32 cull);
void fn_8019B278(CGsRenderMode* mode, BOOL force);
void fn_8019AFA0(CGsTevStage* stage, s32 mode);
void fn_8019AED0(CGsTevStage* stage, BOOL force);
void fn_8019AE94(CGsChannel* chan, BOOL force);
void fn_80199AB0(CGs* gs, s32 chan, u8 enable);
void GXSetTexCopySrc(u16 left, u16 top, u16 wd, u16 ht);
void GXSetTexCopyDst(u16 wd, u16 ht, GXTexFmt fmt, GXBool mipmap);
void GXCopyTex(void* dest, GXBool clear);
void GXPixModeSync(void);
}

// sends all of the current render, tev and channel state
static inline void SetupAll(BOOL force)
{
	CGs* gs = &lbl_8025C400.mGs;
	if (gs->mCurRenderMode != NULL) {
		fn_8019B278(gs->mCurRenderMode, force);
	}
	if (gs->mCurTevStage != NULL) {
		fn_8019AED0(gs->mCurTevStage, force);
	}
	if (gs->mCurChannel != NULL) {
		fn_8019AE94(gs->mCurChannel, force);
	}
}

// forgets all of the state after drawing behind its back
static inline void InvalidateAll()
{
	CGs* gs = &lbl_8025C400.mGs;
	HSD_StateInvalidate(HSD_STATE_ALL);
	if (gs->mCurRenderMode != NULL) {
		gs->mCurRenderMode->Invalidate();
	}
	if (gs->mCurTevStage != NULL) {
		gs->mCurTevStage->Invalidate();
	}
	if (gs->mCurChannel != NULL) {
		if (!gs->mCurChannel->mDirty) {
			gs->mCurChannel->mDirty = TRUE;
		}
	}
}

#endif
