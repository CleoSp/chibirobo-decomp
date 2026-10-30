#include "game/cjobj.h"
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

// one controller port, game/cpad.h has it but that header clashes with ut.h's CPad
class CPadOne {
public:
	u8 mPort;            // _00
	u32 mButton;         // _04
	u32 mDirButton;      // _08
	u32 mLastButton;     // _0C
	u32 mLastDirButton;  // _10
	u32 mTrigger;        // _14
	u32 mDirTrigger;     // _18
	u32 mRepeat;         // _1C
	u32 mDirRepeat;      // _20
	s32 mRepeatTimer[4]; // _24
	f32 mStickX;         // _34
	f32 mStickY;         // _38
	f32 mSubStickX;      // _3C
	f32 mSubStickY;      // _40
	f32 mAnalogL;        // _44
	f32 mAnalogR;        // _48
	BOOL mConnected;     // _4C
	// _50 vtable

	virtual ~CPadOne() { }           // _08
	virtual void Init(u8 port);      // _0C
	virtual void Clear();            // _10
	virtual void Update(s32 frames); // _14
};

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

// a 64 bit CStatus, name from RTTI
class CStatus64 {
public:
	u64 mFlags; // _00
	// _08 vtable

	virtual ~CStatus64() { } // _08
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
	u8 _00[0x8]; // _00
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
class CJAnim : public CJArchive {
public:
	u8 _80[0xC]; // _80
};

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
// out. names from RTTI
class CScrnColor {
public:
	u8 _00[0x30]; // _00
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

// a particle emitter, CParticle keeps 32
class CGenerator {
public:
	u8 _00[0x8]; // _00
	// _08 vtable

	virtual ~CGenerator() { } // _08
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
	u8 _00[0x20];               // _00
	CCard mCard;                // _20
	u8 _A044[0x16A20 - 0xA044]; // _A044
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
	u8 _108[0x120 - 0x108];       // _108
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

// reads one particle bank and keeps the generators made from it, game/cparticle.h has the
// methods
class CParticle {
public:
	s32 mReadStep;              // _00
	CFile mFile[4];             // _04
	u8 _3B4[0x10];              // _3B4
	CGenerator mGenerators[32]; // _3C4
	// _544 vtable

	virtual ~CParticle() { } // _08
};
typedef char CParticle_size_check[sizeof(CParticle) == 0x548 ? 1 : -1];

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
	CMsg mCMsgs[2];                            // _5D0E8
	u8 _E5D28[0xE5D38 - 0xE5D28];              // _E5D28
	CMotionControlCamera mMotionControlCamera; // _E5D38
	CCameraActions mCameraActions;             // _E5FAC
	CLight* mLightList;                        // _E7E20
	XSound mSound;                             // _E7E24
	u8 _E7F18[0xE8400 - 0xE7F18];              // _E7F18
	GameMemcard mMemcard;                      // _E8400
	u8 _FEE24[0xFF3A4 - 0xFEE24];              // _FEE24
	GameMovie mMovie;                          // _FF3A4
	BOOL mMovieSound;                          // _FF45C
	f32 _FF460[4];                             // _FF460

	virtual ~CGame();

	bool Is472C() const { return mLogo.mState >= 1 && mLogo.mState < 15; }
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

/**
 * @note Address: 0x80007880
 * @note Size: 0x6BC
 */
CGame::~CGame()
{
}

/**
 * @note Address: 0x80007F3C
 * @note Size: 0x114
 */
CBase::~CBase()
{
}

/**
 * @note Address: 0x8000A4B8
 * @note Size: 0x68
 */
extern "C" s32 fn_8000A4B8(CGame* game)
{
	bool slow = false;
	if (game->Is472C() || game->Is4930()) {
		slow = true;
	}
	return slow ? 1 : 2;
}

/**
 * @note Address: 0x8000A520
 * @note Size: 0xC
 */
extern "C" void* fn_8000A520(CGame* game)
{
	return &game->mMotionControlCamera;
}

/**
 * @note Address: 0x8000A52C
 * @note Size: 0xC
 */
extern "C" CLight* fn_8000A52C(CGame* game)
{
	return game->mLightList;
}

/**
 * @note Address: 0x8000A538
 * @note Size: 0x8
 */
extern "C" u32 fn_8000A538(CBase* base)
{
	return base->mFrames;
}

/**
 * @note Address: 0x8000A540
 * @note Size: 0x8
 */
extern "C" CCamera* fn_8000A540(CBase* base)
{
	return &base->mCamera[0];
}

/**
 * @note Address: 0x8000A548
 * @note Size: 0x8
 */
extern "C" CCamera* fn_8000A548(CBase* base)
{
	return &base->mCamera[1];
}

/**
 * @note Address: 0x8000A550
 * @note Size: 0x8
 */
extern "C" CLight* fn_8000A550(CBase* base)
{
	return &base->mLight[0];
}

/**
 * @note Address: 0x8000A558
 * @note Size: 0x8
 */
extern "C" CLight* fn_8000A558(CBase* base)
{
	return &base->mLight[4];
}

/**
 * @note Address: 0x8000BE38
 * @note Size: 0x560
 */
// draws the movie, centred at its own size unless _FF460 says otherwise. with alpha it's drawn
// three times over
extern "C" void fn_8000BE38(CGame* game)
{
	if (game->mMovie.mMovie.mState != 0) {
		f32 w   = fpclassify(game->_FF460[2]) == FP_NAN ? (f32)game->mMovie.mMovie.mVideoWidth : game->_FF460[2];
		f32 h   = fpclassify(game->_FF460[3]) == FP_NAN ? (f32)game->mMovie.mMovie.mVideoHeight : game->_FF460[3];
		f32 x   = fpclassify(game->_FF460[0]) == FP_NAN ? (640.0f - w) / 2 : game->_FF460[0];
		f32 y   = fpclassify(game->_FF460[1]) == FP_NAN ? (448.0f - h) / 2 : game->_FF460[1];
		CGs* gs = &lbl_8025C400.mGs;
		fn_8019B648(gs->mCurRenderMode, 1);
		fn_8019B5D0(gs->mCurRenderMode, 0);
		fn_8019B464(gs->mCurRenderMode, 1, 7, 0);
		if (game->mMovie.mMovie.mAlpha) {
			fn_8019B56C(gs->mCurRenderMode, 1, 4, 5, 15);
			fn_8019B504(gs->mCurRenderMode, 4, 0x20, 0, 3, 255);
			SetupAll(FALSE);
			if (game->mMovie.mMovie.BeginDraw() >= 0) {
				for (s32 i = 0; i < 3; i++) {
					game->mMovie.mMovie.Draw(x, y, w, h);
				}
				game->mMovie.mMovie.EndDraw();
			}
		} else {
			fn_8019B56C(gs->mCurRenderMode, 0, 4, 5, 15);
			fn_8019B504(gs->mCurRenderMode, 7, 0, 0, 7, 255);
			SetupAll(FALSE);
			game->mMovie.mMovie.DrawFrame(x, y, w, h);
		}
		fn_8019B504(gs->mCurRenderMode, 4, 0, 0, 3, 255);
		InvalidateAll();
	}
}

extern "C" {
void fn_8019B3E8(CGsRenderMode* mode, u8 enable, u8 value);
void fn_80198614(CGs* gs, f32 x0, f32 y0, f32 z0, u32 color0, f32 x1, f32 y1, f32 z1, u32 color1, f32 x2, f32 y2, f32 z2, u32 color2,
                 f32 x3, f32 y3, f32 z3, u32 color3);
}

// the player, only the objects it points at
struct UnkPlayer : public XObj {
	u8 _988[0x9E0 - 0x988]; // _988
	XObj* mShadowObj;       // _9E0, the one its shadow height comes from
	u8 _9E4[0xA64 - 0x9E4]; // _9E4
	XObj* _A64;             // _A64
	u8 _A68[0xA94 - 0xA68]; // _A68
	XObj* _A94;             // _A94
	u8 _A98[0xB84 - 0xA98]; // _A98
	XObj* _B84;             // _B84
	u8 _B88[0xBC8 - 0xB88]; // _B88
	XObj* _BC8;             // _BC8
	u8 _BCC[0xC08 - 0xBCC]; // _BCC
	XObj* _C08;             // _C08

	XObj* GetShadowObj()
	{
		if (mShadowObj) {
			return mShadowObj;
		}
		return NULL;
	}
};

// the current stage's settings
struct UnkStageInfo {
	u8 _00[0x6];        // _00
	s16 mCameraMode;    // _06
	u8 mMirrorFade;     // _08
	u8 _09[0x10 - 0x9]; // _09
	u8 mFlags;          // _10, bit 0 keeps the shadows at full strength
};

struct UnkStageRef {
	UnkStageInfo* mInfo; // _00
};

extern "C" {
UnkStageRef* fn_8006F4B8(void);
void JObjClearMObjFlags(HSD_JObj* jobj, u32 flags);
void HSD_JObjWalkTree(HSD_JObj* jobj, void* cb, void* args);
void HSD_CObjSetInterest(HSD_CObj* cobj, Vec* interest);
void HSD_CObjSetEyePosition(HSD_CObj* cobj, Vec* eye);
void HSD_ShadowDeleteObject(HSD_Shadow* shadow, HSD_JObj* jobj);
void fn_8000CBF4(HSD_JObj* jobj, f32* limits);
}

// an object that is drawn into the shadow, or onto it with the mask
static inline bool IsShadowObj(XObj* obj, u64 mask)
{
	bool on = false;
	if ((obj->_228.mFlags & 1) && (obj->_228.mFlags & 0x10000) && (obj->_228.mFlags & mask)) {
		on = true;
	}
	return on;
}

/**
 * @note Address: 0x8000C450
 * @note Size: 0x6FC
 */
// NONMATCHING: the shadow object and the first loop's registers rotate
// renders the shadow of the first light that casts one, from above the main camera's
// interest. the player's shadow object gets the height of its highest joint under the player
extern "C" void fn_8000C450(CGame* game)
{
	UnkPlayer* player;
	XObj* target;
	s32 i;
	XObj* obj;
	if (game->mStatus.mFlags & 1) {
		player        = (UnkPlayer*)lbl_8025C400.mXObjs.mPools[2].First();
		target        = player->GetShadowObj();
		f32 limits[2] = { 1.0f + player->mMoveTranslate.y, NAN };
		for (i = 0; i < game->_D874; i++) {
			obj = game->_C074[i];
			if (obj->_228.mFlags & 2) {
				if (obj == target) {
					limits[1] = NAN;
					if (obj->mJObj != NULL) {
						HSD_JObjWalkTree(obj->mJObj, fn_8000CBF4, limits);
					}
				} else if (obj->mJObj != NULL) {
					HSD_JObjWalkTree(obj->mJObj, JObjClearMObjFlags, (void*)0x4000000);
				}
			}
		}

		for (CLight* light = game->mLightList; light != NULL; light = (CLight*)light->next) {
			CShadow* shadow = &light->mShadow;
			if (shadow->IsActive()) {
				HSD_CObj* cobj = shadow->mShadow->camera;
				Vec interest   = game->GetMainCamera()->mInterest;
				Vec eye;
				eye.x = interest.x;
				eye.z = interest.z;
				eye.y = 200.0f + interest.y;
				HSD_CObjSetInterest(cobj, &interest);
				HSD_CObjSetEyePosition(cobj, &eye);
				HSD_ShadowDeleteObject(shadow->mShadow, NULL);
				shadow->InitRect();
				for (s32 i = 0; i < game->_D874; i++) {
					XObj* obj = game->_C074[i];
					if (IsShadowObj(obj, 4)) {
						obj->CastShadow(shadow);
					}
					if ((fn_8006F4B8()->mInfo->mFlags & 1) && obj->mAttr.GetKind() == 2) {
						obj->CastShadow2(shadow);
					}
				}
				Vec center = game->GetMainCamera()->mInterest;
				shadow->AddRect(&center, 100.0f, -100.0f, -100.0f, 100.0f);
				game->mGs.mCamera = shadow->StartRender();
				if (fn_8019997C(game)) {
					fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 3, 0);
					fn_80199AB0(&lbl_8025C400.mGs, 0, 0);
					fn_8019B360(lbl_8025C400.mGs.mCurRenderMode, 2);
					SetupAll(TRUE);
					for (s32 i = 0; i < game->_D874; i++) {
						XObj* obj = game->_C074[i];
						if (IsShadowObj(obj, 8)) {
							f32 alpha;
							if (fn_8006F4B8()->mInfo->mFlags & 1) {
								alpha = 1.0f;
							} else {
								CCamera* camera = game->GetMainCamera();
								f32 dz          = camera->mInterest.z - obj->mMoveTranslate.z;
								f32 dx          = camera->mInterest.x - obj->mMoveTranslate.x;
								alpha           = (100.0f - sqrtf(dx * dx + dz * dz)) / 100.0f;
								if (alpha < 0.0f) {
									alpha = 0.0f;
								}
								if (alpha > 0.8f) {
									alpha = 0.8f;
								}
							}
							obj->SetShadowAlpha(sin(3.1415927f * alpha * 0.5f), &obj->mMoveTranslate);
						}
					}
					fn_80199944(&game->mGs);
				}
				shadow->EndRender();
				break;
			}
		}
		InvalidateAll();
	}
}

/**
 * @note Address: 0x8000D118
 * @note Size: 0x5EC
 */
// NONMATCHING: the second loop's registers rotate
// clears the screen's alpha from the second camera, draws the 0x40 objects and the two the
// player is busy with into it, then sets the 0x40 objects back up for the main pass
extern "C" void fn_8000D118(CGame* game)
{
	XObj* obj;
	s32 i;
	UnkPlayer* player;
	if ((game->mStatus.mFlags & 1) && (game->mStatus.mFlags & 0x4000000000)) {
		game->mGs.mCamera = game->GetCamera2();
		if (fn_8019997C(game)) {
			InvalidateAll();
			fn_80199AB0(&lbl_8025C400.mGs, 4, 0);
			fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 0, 4, 5, 15);
			fn_8019B648(lbl_8025C400.mGs.mCurRenderMode, 0);
			fn_8019B5D0(lbl_8025C400.mGs.mCurRenderMode, 1);
			fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 0, 7, 0);
			fn_8019B3E8(lbl_8025C400.mGs.mCurRenderMode, 0, 0);
			SetupAll(TRUE);
			Vec quad[4] = { { 0.0f, 0.0f, 0.0f }, { 640.0f, 0.0f, 0.0f }, { 640.0f, 448.0f, 0.0f }, { 0.0f, 448.0f, 0.0f } };
			fn_80198614(&lbl_8025C400.mGs, quad[0].x, quad[0].y, quad[0].z, 4, quad[1].x, quad[1].y, quad[1].z, 4, quad[2].x, quad[2].y,
			            quad[2].z, 4, quad[3].x, quad[3].y, quad[3].z, 4);
			fn_8019B648(lbl_8025C400.mGs.mCurRenderMode, 1);
			InvalidateAll();
			fn_80199944(&game->mGs);
		}

		game->mGs.mCamera = game->GetMainCamera();
		if (fn_8019997C(game)) {
			XObj* targets[2] = { NULL, NULL };
			player           = (UnkPlayer*)game->mXObjs.mPools[2].First();
			if (targets[0] == NULL) {
				targets[0] = player->_A94;
			}
			if (targets[1] == NULL) {
				targets[1] = player->_A64;
			}
			if (targets[1] == NULL) {
				targets[1] = player->_C08;
			}
			if (targets[1] == NULL) {
				targets[1] = player->_B84;
			}
			if (targets[1] == NULL) {
				targets[1] = player->_BC8;
			}
			for (i = 0; i < game->_D874; i++) {
				obj = game->_C074[i];
				if (obj->_228.mFlags & 0x40) {
					obj->vfDC(1, 0);
					obj->vfAC();
				} else if ((targets[0] == obj || targets[1] == obj) && !(obj->_228.mFlags & 0x800000)) {
					obj->vfAC();
				}
			}
			fn_80199944(&game->mGs);
		}

		for (i = 0; i < game->_D874; i++) {
			obj = game->_C074[i];
			if (obj->_228.mFlags & 0x40) {
				if (obj->_228.mFlags & 0x800000) {
					obj->vfDC(1, 8);
				} else {
					obj->vfDC(1, 4);
				}
			}
		}
	}
}

/**
 * @note Address: 0x8000CBF4
 * @note Size: 0x1A8
 */
// a joint walk: finds the highest joint at or under the limit in limits[0] and keeps it in
// limits[1], nan to start, and marks the materials of those joints
extern "C" void fn_8000CBF4(HSD_JObj* jobj, f32* limits)
{
	Vec pos;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(jobj), &pos);
	BOOL under = pos.y <= limits[0];
	if (under) {
		if (fpclassify(limits[1]) == FP_NAN) {
			limits[1] = pos.y;
		} else if (limits[1] < pos.y) {
			limits[1] = pos.y;
		}
	}
	for (HSD_DObj* dobj = HSD_JObjGetDObj(jobj); dobj != NULL; dobj = HSD_DObjGetNext(dobj)) {
		HSD_MObj* mobj = HSD_DObjGetMObj(dobj);
		if (mobj != NULL) {
			if (under) {
				HSD_MObjSetFlags(mobj, 0x4000000);
			} else {
				HSD_MObjClearFlags(mobj, 0x4000000);
			}
		}
	}
}

/**
 * @note Address: 0x8000CDA0
 * @note Size: 0x28C
 */
// CGame::vf20, draws the objects in _C074 from the main camera, kind 3 ones first
extern "C" void fn_8000CDA0(CGame* game)
{
	if (game->mStatus.mFlags & 1) {
		s32 i;
		game->mGs.mCamera = game->GetMainCamera();
		if (fn_8019997C(game)) {
			for (i = 0; i < game->_D874; i++) {
				if (game->_C074[i]->mAttr.GetKind() == 3) {
					game->_C074[i]->vfAC();
				}
			}
			fn_80199944(&game->mGs);
		}
		game->mGs.mCamera = game->GetMainCamera();
		if (fn_8019997C(game)) {
			for (i = 0; i < game->_D874; i++) {
				if (game->_C074[i]->mAttr.GetKind() != 3) {
					game->_C074[i]->vfAC();
				}
			}
			fn_8006F338(2, 0, 0);
			fn_80199944(&game->mGs);
		}
		InvalidateAll();
	}
}

/**
 * @note Address: 0x8000D05C
 * @note Size: 0x2C
 */
extern "C" CCamera* fn_8000D05C(CBase* base)
{
	return base->GetCamera();
}

/**
 * @note Address: 0x8001552C
 * @note Size: 0x8
 */
extern "C" BOOL fn_8001552C(CBase* base)
{
	return TRUE;
}

/**
 * @note Address: 0x800153B4
 * @note Size: 0x4
 */
extern "C" void fn_800153B4(CBase* base)
{
}

/**
 * @note Address: 0x800153B8
 * @note Size: 0x4
 */
extern "C" void fn_800153B8(CBase* base)
{
}

/**
 * @note Address: 0x80015634
 * @note Size: 0x4
 */
extern "C" void fn_80015634(CBase* base, int argc, char** argv)
{
}

/**
 * @note Address: 0x80015638
 * @note Size: 0x8
 */
extern "C" s32 fn_80015638(CBase* base)
{
	return 1;
}

/**
 * @note Address: 0x80015640
 * @note Size: 0x4
 */
extern "C" void fn_80015640(CBase* base)
{
}

/**
 * @note Address: 0x80019310
 * @note Size: 0x2C
 */
extern "C" CLight* fn_80019310(CBase* base)
{
	return base->GetLight();
}

extern "C" {
s32 DVDGetDriveStatus(void);
BOOL DVDCheckDisk(void);
void fn_801CDC34(void);
void AXQuit(void);
void AIReset(void);
void* HSD_VISetUserGXDrawDoneCallback(void* cb);
void GXDrawDone(void);
void GXAbortFrame(void);
void OSResetSystem(int reset, u32 code, BOOL forceMenu);
}

/**
 * @note Address: 0x800153BC
 * @note Size: 0x170
 */
// NONMATCHING: the pad pointer and the counter swap registers
// CBase::Update, keeps the rumble off while the disc can't be read and watches for the reset
// button or B X start held for a second
extern "C" void fn_800153BC(CBase* base, u32 frames)
{
	base->mLastDriveStatus = base->mDriveStatus;
	base->mDriveStatus     = DVDGetDriveStatus();
	base->mDiskIn          = DVDCheckDisk();
	switch (base->mDriveStatus) {
	case -1:
	case 4:
	case 5:
	case 6:
	case 11:
		if (base->mDriveStatus != base->mLastDriveStatus) {
			CPadEx* pad         = &base->mPad;
			base->mRumblePaused = TRUE;
			for (s32 i = 0; i < 4; i++) {
				if (pad->mPad[i].mConnected) {
					HSD_PadRumbleOffN(pad->mPad[i].mPort);
				}
			}
			HSD_PadRumblePauseAll();
		}
		break;
	case 0:
		if (base->mLastDriveStatus != 0 && base->mRumblePaused) {
			base->mRumblePaused = FALSE;
			fn_801CDC34();
		}
		break;
	}

	if (!base->mReset) {
		if (HSD_PadGetResetSwitch()) {
			base->mResetTimer = 0;
			base->mReset      = TRUE;
		} else if ((base->mPad.mPad[0].mButton & 0xFFFF) == 0x1600) {
			if ((base->mResetTimer += base->mFrames) > 60) {
				base->mResetTimer = 0;
				base->mReset      = TRUE;
			}
		} else if (!base->mReset) {
			base->mResetTimer = 0;
		}
	}
}

/**
 * @note Address: 0x800152A0
 * @note Size: 0x60
 */
// stops the sound and the video and resets
inline void ResetSystem(u32 code)
{
	AXQuit();
	AIReset();
	HSD_VISetUserGXDrawDoneCallback(NULL);
	GXDrawDone();
	GXAbortFrame();
	VISetBlack(TRUE);
	VIFlush();
	VIWaitForRetrace();
	OSResetSystem(code, 0, 0);
}

// CBase::Reset
extern "C" void fn_800152A0(CBase* base, u32 code)
{
	ResetSystem(code);
}

extern "C" void GXProject(f32 x, f32 y, f32 z, const Mtx mtx, const f32* pm, const f32* vp, f32* sx, f32* sy, f32* sz);

/**
 * @note Address: 0x80015300
 * @note Size: 0xB4
 */
// where a point ends up on screen through a camera, the main one by default
extern "C" void fn_80015300(CGame* game, const Vec& pos, f32* x, f32* y, f32* z, CCamera* camera)
{
	camera = camera != NULL ? camera : &lbl_8025C400.mMotionControlCamera.mCamera;
	f32 sx, sy, sz;
	GXProject(pos.x, pos.y, pos.z, camera->mViewMtx, camera->mProjection, camera->mViewport, &sx, &sy, &sz);
	if (x != NULL) {
		*x = sx;
	}
	if (y != NULL) {
		*y = sy;
	}
	if (z != NULL) {
		*z = sz;
	}
}

// a menu, only as far as its inline methods show it. name from RTTI
class CMenu {
public:
	s32 mState;             // _00
	u8 _04[0x8];            // _04
	BOOL mOpen;             // _0C
	u8 _10[0x138 - 0x10];   // _10
	s32 _138;               // _138
	s32 _13C;               // _13C
	u8 _140[0x148 - 0x140]; // _140
	// _148 vtable

	virtual ~CMenu();     // _08
	virtual void vf0C();  // _0C
	virtual void vf10();  // _10
	virtual void vf14();  // _14
	virtual void vf18();  // _18
	virtual void Open();  // _1C
	virtual void vf20();  // _20
	virtual void vf24();  // _24
	virtual void vf28();  // _28
	virtual void Reset(); // _2C
};

/**
 * @note Address: 0x80010CD0
 * @note Size: 0x48
 */
// CMenu::Open
extern "C" void fn_80010CD0(CMenu* menu)
{
	menu->mState = 1;
	menu->Reset();
	menu->mOpen = TRUE;
}

/**
 * @note Address: 0x80010D18
 * @note Size: 0x10
 */
// CMenu::Reset
extern "C" void fn_80010D18(CMenu* menu)
{
	menu->_138 = 0;
	menu->_13C = 0;
}

/**
 * @note Address: 0x80015644
 * @note Size: 0x1C8
 */
void main(int argc, char** argv)
{
	((CBase*)&lbl_8025C400)->Init(argc, argv);
	((CBase*)&lbl_8025C400)->InitSystem();
	HSD_VISetBlack(0);
	while (true) {
		VIWaitForRetrace();
		lbl_8025C400.mFrames       = VIGetRetraceCount() - lbl_8025C400.mRetraceCount;
		lbl_8025C400.mRetraceCount = VIGetRetraceCount();
		if (lbl_8025C400.mFrames > 4) {
			lbl_8025C400.mFrames = 4;
		}
		OSResetStopwatch(&lbl_8025C400.mStopwatch[0]);
		OSStartStopwatch(&lbl_8025C400.mStopwatch[0]);
		OSResetStopwatch(&lbl_8025C400.mStopwatch[1]);
		OSStartStopwatch(&lbl_8025C400.mStopwatch[1]);
		OSResetStopwatch(&lbl_8025C400.mStopwatch[2]);
		OSStartStopwatch(&lbl_8025C400.mStopwatch[2]);
		HSD_VICopyXFBAsync(0);
		lbl_8025C400.mStopwatch[0].mLap = OSCheckStopwatch(&lbl_8025C400.mStopwatch[0]);
		OSStopStopwatch(&lbl_8025C400.mStopwatch[0]);
		lbl_8025C400.mPad.Update(lbl_8025C400.mFrames);
		((CBase*)&lbl_8025C400)->Exec();
		((CBase*)&lbl_8025C400)->Update(lbl_8025C400.mFrames);
		lbl_8025C400.mStopwatch[1].mLap = OSCheckStopwatch(&lbl_8025C400.mStopwatch[1]);
		OSStopStopwatch(&lbl_8025C400.mStopwatch[1]);
		((CBase*)&lbl_8025C400)->Draw();
		lbl_8025C400.mStopwatch[2].mLap = OSCheckStopwatch(&lbl_8025C400.mStopwatch[2]);
		OSStopStopwatch(&lbl_8025C400.mStopwatch[2]);
		s32 rate = ((CBase*)&lbl_8025C400)->GetFrameRate();
		u32 wait = rate * 60 / 60 - 1;
		while (VIGetRetraceCount() - lbl_8025C400.mRetraceCount < wait) { }
	}
}

/**
 * @note Address: 0x8001580C
 * @note Size: 0x50
 */
extern "C" void OSReport(const char* fmt, ...)
{
}

/**
 * @note Address: 0x80015EF0
 * @note Size: 0x8
 */
f32 CInterfaceCameraAction::vf58()
{
	return 12.0f;
}

// CJObj's weak copies, everything in it is inline

/**
 * @note Address: 0x80017244
 * @note Size: 0xC4
 */
extern "C" void fn_80017244(CJObj* jobj, f32 x, s32 force)
{
	jobj->CJObj::SetTranslateX(x, force);
}

/**
 * @note Address: 0x80017308
 * @note Size: 0xC4
 */
extern "C" void fn_80017308(CJObj* jobj, f32 y, s32 force)
{
	jobj->CJObj::SetTranslateY(y, force);
}

/**
 * @note Address: 0x800173CC
 * @note Size: 0xC4
 */
extern "C" void fn_800173CC(CJObj* jobj, f32 z, s32 force)
{
	jobj->CJObj::SetTranslateZ(z, force);
}

/**
 * @note Address: 0x80017490
 * @note Size: 0xC4
 */
extern "C" void fn_80017490(CJObj* jobj, f32 x, s32 force)
{
	jobj->CJObj::SetScaleX(x, force);
}

/**
 * @note Address: 0x80017554
 * @note Size: 0xC4
 */
extern "C" void fn_80017554(CJObj* jobj, f32 y, s32 force)
{
	jobj->CJObj::SetScaleY(y, force);
}

/**
 * @note Address: 0x80017618
 * @note Size: 0xC4
 */
extern "C" void fn_80017618(CJObj* jobj, f32 z, s32 force)
{
	jobj->CJObj::SetScaleZ(z, force);
}

/**
 * @note Address: 0x800176DC
 * @note Size: 0x19C
 */
extern "C" void fn_800176DC(CJObj* jobj, f32 x, s32 force)
{
	jobj->CJObj::SetRotationX(x, force);
}

/**
 * @note Address: 0x80017878
 * @note Size: 0x19C
 */
extern "C" void fn_80017878(CJObj* jobj, f32 y, s32 force)
{
	jobj->CJObj::SetRotationY(y, force);
}

/**
 * @note Address: 0x80017A14
 * @note Size: 0x19C
 */
extern "C" void fn_80017A14(CJObj* jobj, f32 z, s32 force)
{
	jobj->CJObj::SetRotationZ(z, force);
}

/**
 * @note Address: 0x80017BB0
 * @note Size: 0x16C
 */
extern "C" void fn_80017BB0(CJObj* jobj, f32 x, f32 y, f32 z, s32 force)
{
	jobj->CJObj::SetTranslate(x, y, z, force);
}

/**
 * @note Address: 0x80017D1C
 * @note Size: 0x14C
 */
extern "C" void fn_80017D1C(CJObj* jobj, const Vec& v, s32 force)
{
	jobj->CJObj::SetTranslate(v, force);
}

/**
 * @note Address: 0x80017E68
 * @note Size: 0x168
 */
extern "C" void fn_80017E68(CJObj* jobj, f32 x, f32 y, f32 z, s32 force)
{
	jobj->CJObj::SetScale(x, y, z, force);
}

/**
 * @note Address: 0x80017FD0
 * @note Size: 0x148
 */
extern "C" void fn_80017FD0(CJObj* jobj, const Vec& v, s32 force)
{
	jobj->CJObj::SetScale(v, force);
}

/**
 * @note Address: 0x80018118
 * @note Size: 0x21C
 */
extern "C" void fn_80018118(CJObj* jobj, f32 x, f32 y, f32 z, s32 force)
{
	Quaternion q;

	if (force == 0) {
		Vec v = { x, y, z };
		// retail tests the cached translation here
		if (!(jobj->mTranslate != v)) {
			return;
		}
	}
	jobj->mRotate.x = x;
	jobj->mRotate.y = y;
	jobj->mRotate.z = z;
	if (jobj->mJObj->flags & USE_QUATERNION) {
		EulerToQuat(&jobj->mRotate, &q);
		HSD_JObjSetRotation(jobj->mJObj, &q);
	} else {
		HSD_JObjSetRotation(jobj->mJObj, (Quaternion*)&jobj->mRotate);
	}
}

/**
 * @note Address: 0x80018334
 * @note Size: 0x1FC
 */
extern "C" void fn_80018334(CJObj* jobj, const Vec& v, s32 force)
{
	if (force == 0 && !(jobj->mRotate != v)) {
		return;
	}
	jobj->mRotate.x = v.x;
	jobj->mRotate.y = v.y;
	jobj->mRotate.z = v.z;
	if (jobj->mJObj->flags & USE_QUATERNION) {
		Quaternion q;
		EulerToQuat(&jobj->mRotate, &q);
		HSD_JObjSetRotation(jobj->mJObj, &q);
	} else {
		HSD_JObjSetRotation(jobj->mJObj, (Quaternion*)&jobj->mRotate);
	}
}

/**
 * @note Address: 0x80018530
 * @note Size: 0x80
 */
extern "C" void fn_80018530(CJObj* jobj, f32 rate, u32 mask)
{
	jobj->CJObj::SetAnimRate(rate, mask);
}

/**
 * @note Address: 0x8001585C
 * @note Size: 0x1C
 */
bool CInterpolationNone::Start(f32 from, f32 to, f32 time)
{
	mTime  = 1.0f;
	mValue = to;
	mSpeed = 0.0f;
	return true;
}

/**
 * @note Address: 0x80015878
 * @note Size: 0x24
 */
bool CInterpolationNone::Step(f32 dt)
{
	mTime = 1.0f;
	return mTime <= 0.0f;
}

/**
 * @note Address: 0x8001589C
 * @note Size: 0x2C
 */
bool CInterpolationNone::Update(u32 step, f32 dt)
{
	return Step(dt);
}

/**
 * @note Address: 0x800158C8
 * @note Size: 0x8
 */
f32 CInterpolationNone::Get()
{
	return mValue;
}

/**
 * @note Address: 0x800158D0
 * @note Size: 0xCC
 */
bool CVectorNoneInterpolation::Start(Vec from, Vec to, f32 time)
{
	if (time < 0.0f) {
		return false;
	}
	mAxes[0].Start(from.x, to.x, time);
	mAxes[1].Start(from.y, to.y, time);
	mAxes[2].Start(from.z, to.z, time);
	return true;
}

/**
 * @note Address: 0x8001599C
 * @note Size: 0xB4
 */
bool CVectorNoneInterpolation::Step(f32 dt)
{
	bool x = mAxes[0].Step(dt);
	bool y = mAxes[1].Step(dt);
	bool z = mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x80015A50
 * @note Size: 0x2C
 */
bool CVectorNoneInterpolation::Update(u32 step, f32 dt)
{
	return Step(dt);
}

/**
 * @note Address: 0x80015A7C
 * @note Size: 0xBC
 */
Vec CVectorNoneInterpolation::Get()
{
	return CVec(mAxes[0].Get(), mAxes[1].Get(), mAxes[2].Get());
}

/**
 * @note Address: 0x80015F08
 * @note Size: 0xCC
 */
bool CVectorLinearInterpolation::Start(Vec from, Vec to, f32 time)
{
	if (time < 0.0f) {
		return false;
	}
	mAxes[0].Start(from.x, to.x, time);
	mAxes[1].Start(from.y, to.y, time);
	mAxes[2].Start(from.z, to.z, time);
	return true;
}

/**
 * @note Address: 0x80015FD4
 * @note Size: 0x30
 */
bool CInterpolationLinear::Start(f32 from, f32 to, f32 time)
{
	if (time < 0.0f) {
		return false;
	}
	mTime  = time;
	mValue = from;
	mSpeed = (to - from) / time;
	return true;
}

/**
 * @note Address: 0x80016004
 * @note Size: 0xB4
 */
bool CVectorLinearInterpolation::Step(f32 dt)
{
	bool x = mAxes[0].Step(dt);
	bool y = mAxes[1].Step(dt);
	bool z = mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x800160B8
 * @note Size: 0x38
 */
bool CInterpolationLinear::Step(f32 dt)
{
	mTime -= dt;
	mValue += mSpeed * dt;
	return mTime <= 0.0f;
}

/**
 * @note Address: 0x800160F0
 * @note Size: 0x2C
 */
bool CVectorLinearInterpolation::Update(u32 step, f32 dt)
{
	return Step(dt);
}

/**
 * @note Address: 0x8001611C
 * @note Size: 0xBC
 */
Vec CVectorLinearInterpolation::Get()
{
	return CVec(mAxes[0].Get(), mAxes[1].Get(), mAxes[2].Get());
}

/**
 * @note Address: 0x800161D8
 * @note Size: 0x8
 */
f32 CInterpolationLinear::Get()
{
	return mValue;
}

/**
 * @note Address: 0x80016264
 * @note Size: 0xD8
 */
bool CVectorEaseInOutInterpolation::Start(Vec from, Vec to, f32 time)
{
	if (time <= 0.0f) {
		return false;
	}
	mAxes[0].Start(from.x, to.x, time);
	mAxes[1].Start(from.y, to.y, time);
	mAxes[2].Start(from.z, to.z, time);
	mDone = FALSE;
	return true;
}

/**
 * @note Address: 0x8001633C
 * @note Size: 0xE0
 */
bool CVectorEaseInOutInterpolation::Update(u32 step, f32 dt)
{
	bool x = mAxes[0].Update(step, dt);
	bool y = mAxes[1].Update(step, dt);
	bool z = mAxes[2].Update(step, dt);
	if (x && z) {
		mDone = TRUE;
	}
	return x && y && z;
}

/**
 * @note Address: 0x8001641C
 * @note Size: 0xB4
 */
bool CVectorEaseInOutInterpolation::Step(f32 dt)
{
	bool x = mAxes[0].Step(dt);
	bool y = mAxes[1].Step(dt);
	bool z = mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x800164D0
 * @note Size: 0xBC
 */
Vec CVectorEaseInOutInterpolation::Get()
{
	return CVec(mAxes[0].Get(), mAxes[1].Get(), mAxes[2].Get());
}

/**
 * @note Address: 0x80016610
 * @note Size: 0xD0
 */
bool CVectorEaseOutInterpolation::Start(Vec from, Vec to, f32 time)
{
	if (time <= 0.0f) {
		return false;
	}
	mAxes[0].Start(from.x, to.x, time);
	mAxes[1].Start(from.y, to.y, time);
	mAxes[2].Start(from.z, to.z, time);
	return true;
}

/**
 * @note Address: 0x800166E0
 * @note Size: 0xC8
 */
bool CVectorEaseOutInterpolation::Update(u32 step, f32 dt)
{
	bool x = mAxes[0].Update(step, dt);
	bool y = mAxes[1].Update(step, dt);
	bool z = mAxes[2].Update(step, dt);
	return x && y && z;
}

/**
 * @note Address: 0x800167A8
 * @note Size: 0xB4
 */
bool CVectorEaseOutInterpolation::Step(f32 dt)
{
	bool x = mAxes[0].Step(dt);
	bool y = mAxes[1].Step(dt);
	bool z = mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x8001685C
 * @note Size: 0xBC
 */
Vec CVectorEaseOutInterpolation::Get()
{
	return CVec(mAxes[0].Get(), mAxes[1].Get(), mAxes[2].Get());
}

/**
 * @note Address: 0x800169E4
 * @note Size: 0xD8
 */
bool CVectorEaseOutDivideInterpolation::Start(Vec from, Vec to, f32 time)
{
	if (time <= 0.0f) {
		return false;
	}
	mAxes[0].Start(from.x, to.x, time);
	mAxes[1].Start(from.y, to.y, time);
	mAxes[2].Start(from.z, to.z, time);
	mDone = FALSE;
	return true;
}

/**
 * @note Address: 0x80016ABC
 * @note Size: 0x2C
 */
bool CInterpolationEaseOutDivide::Start(f32 from, f32 to, f32 time)
{
	if (time <= 0.0f) {
		return false;
	}
	mValue   = from;
	mTarget  = to;
	mDivisor = time;
	return true;
}

/**
 * @note Address: 0x80016AE8
 * @note Size: 0xE0
 */
bool CVectorEaseOutDivideInterpolation::Update(u32 step, f32 dt)
{
	bool x = mAxes[0].Update(step, dt);
	bool y = mAxes[1].Update(step, dt);
	bool z = mAxes[2].Update(step, dt);
	if (x && z) {
		mDone = TRUE;
	}
	return x && y && z;
}

/**
 * @note Address: 0x80016BC8
 * @note Size: 0x84
 */
bool CInterpolationEaseOutDivide::Update(u32 step, f32 dt)
{
	bool done = false;
	for (u32 i = 0; i < step; i++) {
		done = Step(dt);
		if (done == true) {
			break;
		}
	}
	return done;
}

/**
 * @note Address: 0x80016C4C
 * @note Size: 0x58
 */
bool CInterpolationEaseOutDivide::Step(f32 dt)
{
	f32 last = mValue;
	if (mDivisor > 0.0f) {
		mValue = (last * (mDivisor - 1.0f) + mTarget) / mDivisor;
	}
	if ((f32)fabs(mValue - last) < 0.001f) {
		return true;
	}
	return false;
}

/**
 * @note Address: 0x80016CA4
 * @note Size: 0xB4
 */
bool CVectorEaseOutDivideInterpolation::Step(f32 dt)
{
	bool x = mAxes[0].Step(dt);
	bool y = mAxes[1].Step(dt);
	bool z = mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x80016D58
 * @note Size: 0xBC
 */
Vec CVectorEaseOutDivideInterpolation::Get()
{
	return CVec(mAxes[0].Get(), mAxes[1].Get(), mAxes[2].Get());
}

/**
 * @note Address: 0x80016E14
 * @note Size: 0x8
 */
f32 CInterpolationEaseOutDivide::Get()
{
	return mValue;
}

/**
 * @note Address: 0x80016EA0
 * @note Size: 0x2C
 */
bool CInterpolationLinear::Update(u32 step, f32 dt)
{
	return Step(dt);
}

/**
 * @note Address: 0x80009F20
 * @note Size: 0x340
 */
CCameraActionMap::~CCameraActionMap()
{
}

// the microphone in each memory card slot, class unknown. the game only uses slot B
struct UnkSlot {
	s32 _00;       // _00, -3 once the mic is pulled out
	u32 mDeviceID; // _04
	s32 mState;    // _08, 0 probing, 1 mounting, 2 waiting for the mount, 3 mounted
	void* mWork;   // _0C, the mount's work area
};
extern "C" UnkSlot lbl_8020FC3C[];
extern "C" s32 lbl_80659ECC; // the slot the mic in use is in, -1 for none

extern "C" {
s32 MICProbeEx(s32 chan);
s32 MICMountAsync(s32 chan, void* work, s32 size, void (*detach)(s32), void (*attach)(s32, s32));
s32 MICGetResultCode(s32 chan);
s32 MICGetDeviceID(s32 chan, u32* id);
s32 MICUnmount(s32 chan);
s32 M2SGetActiveChannel(void);
s32 M2SGetSamplesLeft(void);
void M2SGetSamples(s16** samples, s32 count);
void M2SAdvanceBuffer(s32 count);
void* memset(void* dst, int val, u32 size);
void DCFlushRange(void* addr, u32 size);
extern void* __OSCurrHeap;
void* OSAllocFromHeap(void* heap, u32 size);
void OSFreeToHeap(void* heap, void* ptr);
}

/**
 * @note Address: 0x8000A858
 * @note Size: 0x18
 */
extern "C" void fn_8000A858(s32 index)
{
	lbl_8020FC3C[index]._00 = -3;
}

/**
 * @note Address: 0x8000A6D8
 * @note Size: 0x180
 */
// NONMATCHING: the work area isn't read back for the mount
// mounts the mic in slot B and keeps it mounted
extern "C" void fn_8000A6D8(void)
{
	UnkSlot* slot = &lbl_8020FC3C[1];
	s32 result;
	switch (slot->mState) {
	case 0:
		if (MICProbeEx(1) == 0) {
			slot->mState = 1;
		}
		break;
	case 1:
		slot->mWork = OSAllocFromHeap(__OSCurrHeap, 0x3000);
		switch (result = MICMountAsync(1, slot->mWork, 0x3000, fn_8000A858, NULL)) {
		case 0:
			slot->mState = 2;
			break;
		case -1:
			break;
		default:
			goto failed;
		}
		break;
	case 2:
		switch (result = MICGetResultCode(1)) {
		case 0:
			MICGetDeviceID(1, &slot->mDeviceID);
			slot->_00    = 0;
			slot->mState = 3;
			break;
		case -1:
			break;
		default:
			goto failed;
		}
		break;
	case 3:
		if (slot->_00 != 0) {
			MICUnmount(1);
			OSFreeToHeap(__OSCurrHeap, slot->mWork);
			slot->mWork  = NULL;
			slot->mState = 0;
		}
		break;
	}
	return;

failed:
	switch (result) {
	case -2:
		OSFreeToHeap(__OSCurrHeap, slot->mWork);
		slot->mWork  = NULL;
		slot->mState = 0;
		break;
	case -3:
		OSFreeToHeap(__OSCurrHeap, slot->mWork);
		slot->mWork  = NULL;
		slot->mState = 0;
		break;
	}
}

extern "C" s32 MICReset(s32 chan);

/**
 * @note Address: 0x8000FA40
 * @note Size: 0x8C
 */
// CGame::Reset, resets the mic first if one is in use
extern "C" void fn_8000FA40(CGame* game, u32 code)
{
	if (lbl_80659ECC >= 0 && lbl_8020FC3C[lbl_80659ECC].mDeviceID == 0) {
		MICReset(lbl_80659ECC);
	}
	ResetSystem(code);
}

extern "C" s32 lbl_8020FC5C[3];
extern "C" s32 lbl_8020FC68[3];
extern "C" s32 lbl_8065B158;
extern "C" s32 lbl_8065B15C;
extern "C" s16* lbl_8065B160[2];
extern "C" u8 lbl_8065B168;
extern "C" u32 lbl_8065B16C[2];
extern "C" void (*lbl_8065B174)(void);

/**
 * @note Address: 0x8000A560
 * @note Size: 0x118
 */
// NONMATCHING: next and count swap registers
// pulls the next samples from the mic into the buffer, a faster step when more are waiting,
// and keeps the loudest ones
extern "C" void fn_8000A560(s16* buffer)
{
	memset(buffer, 0, 0x280);
	u32* pos = &lbl_8065B16C[M2SGetActiveChannel()];
	s32 left = M2SGetSamplesLeft();
	if (left < 0) {
		*pos = 0;
	} else if (left >= lbl_8020FC68[0]) {
		s32 step;
		if (left < lbl_8020FC68[1]) {
			step = lbl_8020FC5C[0];
		} else if (left < lbl_8020FC68[2]) {
			step = lbl_8020FC5C[1];
		} else {
			step = lbl_8020FC5C[2];
		}
		u32 next  = *pos + step * 159;
		s32 count = next >> 16;
		s16* samples;
		M2SGetSamples(&samples, count + 4);
		s16* sample = samples;
		for (s32 i = 0; i < count + 4; i++) {
			s32 value = *sample;
			if (value > lbl_8065B15C) {
				lbl_8065B15C = value;
			}
			if (value > 1000 && value > lbl_8065B158) {
				lbl_8065B158 = value;
			}
			sample++;
		}
		next += step;
		*pos = next & 0xFFFF;
		M2SAdvanceBuffer(next >> 16);
	}
}

extern "C" {
u32 CheckStageFlag(s32 stage, u32 mask);
BOOL M2SSetActiveChannel(s32 chan);
BOOL M2SSetMode(s32 mode);
BOOL M2SSetPrerecordSamples(s32 milliseconds);
void M2SOpen(void);
void M2SClose(void);
void M2SStart(void);
void M2SStop(void);
s32 MICSetGain(s32 chan, s32 gain);
s32 MICStart(s32 chan);
s32 MICGetButton(s32 chan, u32* button);
s64 OSGetTime(void);
}
extern "C" BOOL lbl_8065B148;
extern "C" s64 lbl_8065B150;

// os.h's, which this file can't include
#define OSMillisecondsToTicks(msec) ((msec) * ((*(u32*)0x800000F8 / 4) / 1000))

// whether the stage takes the mic, which ended up true either way
static inline BOOL IsMicStage()
{
	if (CheckStageFlag(lbl_8025C400.mStage, 2)) {
		return TRUE;
	}
	return TRUE;
}

/**
 * @note Address: 0x8000A870
 * @note Size: 0x31C
 */
// the mic: opens it in whichever slot has one, records while its button is held and a little
// after, and hands the loudest sample to every object
extern "C" void fn_8000A870(void)
{
	if (lbl_8025C400.mStatus.mFlags & 1) {
		fn_8000A6D8();
		if (IsMicStage()) {
			if (lbl_80659ECC < 0) {
				if (lbl_8020FC3C[0]._00 == 0) {
					lbl_80659ECC = 0;
				} else if (lbl_8020FC3C[1]._00 == 0) {
					lbl_80659ECC = 1;
				}
				if (lbl_80659ECC >= 0 && lbl_8020FC3C[lbl_80659ECC].mDeviceID == 0) {
					M2SSetActiveChannel(lbl_80659ECC);
					M2SSetMode(3);
					M2SSetPrerecordSamples(400);
					M2SOpen();
					MICSetGain(lbl_80659ECC, 0);
					MICStart(lbl_80659ECC);
				}
			} else if (lbl_8020FC3C[lbl_80659ECC]._00 != 0) {
				M2SClose();
				lbl_80659ECC = -1;
			}
		}

		if (lbl_80659ECC >= 0 && lbl_8020FC3C[lbl_80659ECC].mDeviceID == 0) {
			u32 button;
			MICGetButton(lbl_80659ECC, &button);
			if (button & 0x10) {
				if (!lbl_8065B148 && lbl_8065B150 == 0) {
					M2SStart();
					lbl_8065B148 = TRUE;
				}
			} else if (lbl_8065B148 && lbl_8065B150 == 0) {
				lbl_8065B150 = OSGetTime();
			}
			if (OSGetTime() - lbl_8065B150 > OSMillisecondsToTicks(100) && lbl_8065B150 != 0 && lbl_8065B148) {
				M2SStop();
				lbl_8065B148 = FALSE;
				lbl_8065B150 = 0;
			}
		}

		if (lbl_8065B158 != 0) {
			if ((lbl_8025C400.mStatus.mFlags & 1) && !(lbl_8025C400.mStatus.mFlags & 2)) {
				for (s32 i = 0; i < lbl_8025C400.mDrawCount; i++) {
					if (lbl_8025C400.mDrawList[i]->_228.mFlags & 1) {
						lbl_8025C400.mDrawList[i]->OnMicVolume(-1, lbl_8065B158);
					}
				}
			}
			lbl_8065B158 = 0;
		}
	}
}

/**
 * @note Address: 0x8000A678
 * @note Size: 0x60
 */
extern "C" void fn_8000A678(void)
{
	lbl_8065B174();
	fn_8000A560(lbl_8065B160[lbl_8065B168]);
	DCFlushRange(lbl_8065B160[lbl_8065B168], 0x280);
	lbl_8065B168 ^= 1;
}

/**
 * @note Address: 0x8000AB8C
 * @note Size: 0x4
 */
extern "C" void fn_8000AB8C(void)
{
}

extern "C" {
void OSPanic(const char* file, int line, const char* message, ...);
BOOL OSCreateThread(void* thread, void* (*func)(void*), void* param, void* stack, u32 stackSize, s32 priority, u16 attr);
s32 OSResumeThread(void* thread);
}

// the thread the movie plays on and its stack
struct MovieThread {
	u8 mThread[0x318]; // _000, an OSThread
	void* mStack;      // _318
	u32 mStackSize;    // _31C
	u8 _320[0x8];      // _320
};
extern "C" MovieThread lbl_80360AE8;
extern "C" u8 lbl_80360E10[0x1000];

/**
 * @note Address: 0x8000AC64
 * @note Size: 0xD0
 */
// NONMATCHING: count and result swap registers
// UpdateMovie, the movie thread: steps the movie once a retrace until it's done
extern "C" void* fn_8000AC64(void* arg)
{
	u32 count     = VIGetRetraceCount();
	CMovie* movie = &((CGame*)arg)->mMovie.mMovie;
	do {
		while (count == VIGetRetraceCount()) {
			VIWaitForRetrace();
		}
		count = VIGetRetraceCount();
		if (movie->mState != 0) {
			s32 result = movie->Update();
			if (result < 0) {
				movie->Stop();
				OSPanic("game.cpp", 3832, "%s%s", fn_801911B4("error on play movie! - %d\n", result), __FUNCTION__);
			}
		}
	} while (movie->mState != 0);
	return NULL;
}

/**
 * @note Address: 0x8000ABB8
 * @note Size: 0xAC
 */
// NONMATCHING: the flags are stored before the alpha bit is taken
// starts a movie and the thread that plays it, bit 0 of the flags plays it with alpha
extern "C" void fn_8000ABB8(CGame* game, const char* path, BOOL loop, u32 flags)
{
	game->mMovieSound = flags;
	game->mMovie.mMovie.Start(path, loop, flags & 1, TRUE);
	game->_FF460[0] = game->_FF460[1] = game->_FF460[2] = game->_FF460[3] = NAN;
	lbl_80360AE8.mStack                                                   = lbl_80360E10;
	lbl_80360AE8.mStackSize                                               = sizeof(lbl_80360E10);
	OSCreateThread(lbl_80360AE8.mThread, fn_8000AC64, game, lbl_80360E10 + sizeof(lbl_80360E10), sizeof(lbl_80360E10), 16, 1);
	OSResumeThread(lbl_80360AE8.mThread);
}

// what the command line can set
struct CmdLineArgs {
	s32 mStage;   // _00
	s32 mSlot;    // _04, the card slot, 0 for A
	u8 _08[0x18]; // _08
};
extern "C" CmdLineArgs lbl_8020FBC0;
extern "C" s32 fn_8006F654(const char* name);
extern "C" {
char* strtok(char* str, const char* delim);
int strcmp(const char* a, const char* b);
}

/**
 * @note Address: 0x80013580
 * @note Size: 0xCC
 */
// reads /stage:name and /slot:letter off the command line
extern "C" void fn_80013580(CGame* game, int argc, char** argv)
{
	for (s32 i = 1; i < argc; i++) {
		switch (argv[i][0]) {
		case '/':
			char* name = strtok(argv[i] + 1, ":");
			if (strcmp(name, "stage") == 0) {
				lbl_8020FBC0.mStage = fn_8006F654(strtok(NULL, " "));
			} else if (strcmp(name, "slot") == 0) {
				lbl_8020FBC0.mSlot = *strtok(NULL, " ") - 'A';
			}
			break;
		}
	}
}

/**
 * @note Address: 0x8000AB90
 * @note Size: 0x28
 */
extern "C" void fn_8000AB90(CGame* game)
{
	game->mMovie.mMovie.Stop();
}

/**
 * @note Address: 0x8000CBC0
 * @note Size: 0x8
 */
extern "C" f32 fn_8000CBC0(void)
{
	return 40.0f;
}

/**
 * @note Address: 0x8000CBC8
 * @note Size: 0x2C
 */
extern "C" void fn_8000CBC8(CJObj* obj, CShadow* shadow)
{
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)obj->mJObj);
}

/**
 * @note Address: 0x8000CD9C
 * @note Size: 0x4
 */
extern "C" void fn_8000CD9C(void)
{
}

/**
 * @note Address: 0x8000D02C
 * @note Size: 0x30
 */
extern "C" void fn_8000D02C(XObj* obj)
{
	obj->vfB0(0);
}

/**
 * @note Address: 0x8000D088
 * @note Size: 0x88
 */
extern "C" f32 fn_8000D088(XObj* obj)
{
	if (fpclassify(obj->_77C) == FP_NAN) {
		return 0.0f;
	}
	return obj->_77C - obj->mMoveTranslate.y;
}

/**
 * @note Address: 0x8000D110
 * @note Size: 0x8
 */
extern "C" f32 fn_8000D110(XObj* obj)
{
	return obj->_6F8;
}

/**
 * @note Address: 0x8000DA50
 * @note Size: 0x2C
 */
extern "C" void fn_8000DA50(XObj* obj, s32 arg)
{
	obj->vfB0(arg);
}

/**
 * @note Address: 0x8000CB4C
 * @note Size: 0x74
 */
extern "C" void fn_8000CB4C(XObj* obj, CShadow* shadow)
{
	f32 size = obj->vf108();
	Vec pos  = obj->mMoveTranslate;
	shadow->AddRect(&pos, size, -size, -size, size);
}

/**
 * @note Address: 0x80012A7C
 * @note Size: 0x10
 */
extern "C" void fn_80012A7C(void* obj, s32* count)
{
	(*count)++;
}

extern "C" {
s32 fn_801E4380(s32 chan, void* arg, s32 result);
s32 fn_801E4C58(s32 chan, void* arg, s32 result);
}

/**
 * @note Address: 0x8001364C
 * @note Size: 0x34
 */
extern "C" s32 fn_8001364C(s32 chan, void* arg, s32 result)
{
	if (chan == 0 && result == 0x48) {
		chan = 1;
	}
	return fn_801E4380(chan, arg, result);
}

/**
 * @note Address: 0x80013680
 * @note Size: 0x34
 */
extern "C" s32 fn_80013680(s32 chan, void* arg, s32 result)
{
	if (chan == 0 && result == 0x48) {
		chan = 1;
	}
	return fn_801E4C58(chan, arg, result);
}

/**
 * @note Address: 0x8001357C
 * @note Size: 0x4
 */
extern "C" void fn_8001357C(void)
{
}

/**
 * @note Address: 0x800151DC
 * @note Size: 0x4
 */
extern "C" void fn_800151DC(void)
{
}

/**
 * @note Address: 0x8000C398
 * @note Size: 0xB8
 */
extern "C" void fn_8000C398(CGame* game)
{
	if (game->mStatus.mFlags & 1) {
		for (s32 i = 0; i < game->mXObjs.mObjectCount; i++) {
			XObj* obj = game->mXObjs.mObjects[i];
			if (obj->_228.mFlags & 1) {
				obj->vfC8();
			}
		}
		fn_8006F338(4, 0, 0);
	}
}

// the area action's queries of the area state

extern "C" {
void fn_8001146C(CGame* game);
void fn_80011270(CGame* game);
BOOL fn_80132B98(XOBJS* objs);
void* fn_8006F3CC(void);
void* fn_8006F3BC(void);
void fn_80141754(CInterfaceCameraAction* action);
void fn_800236EC(CScrnRoboeyes* eyes, s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9,
                 f32 arg10, f32 arg11);
void fn_801BA3AC(HSD_MObj* mobj, u8 r, u8 g, u8 b);
HSD_TObj* HSD_MObjGetTObj(HSD_MObj* mobj);
}

static inline HSD_JObj* GetChild(HSD_JObj* jobj)
{
	return jobj == NULL ? NULL : jobj->child;
}

// the last joint down the first children
static inline HSD_JObj* GetLeaf(HSD_JObj* jobj)
{
	while (GetChild(jobj) != NULL) {
		jobj = GetChild(jobj);
	}
	return jobj;
}

/**
 * @note Address: 0x80011638
 * @note Size: 0x5A0
 */
// NONMATCHING: the stage's camera mode and the second loop's registers rotate
// the step after the objects are read: points the camera and its actions at them, lists them
// and sets up a mirror for each object that has one, at most five
extern "C" BOOL fn_80011638(CGame* game)
{
	FbMirror* mirror;
	s32 mirrors;
	XObj* obj;
	s32 mode;
	s32 i;
	HSD_JObj* jobj;
	if (fn_80132B98(&game->mXObjs)) {
		mode                                                                    = fn_8006F4B8()->mInfo->mCameraMode;
		((CCameraActionScroll*)lbl_8025C400.mCameraActions.mActions[11])->mZoom = 1;
		if (fn_8006F3CC() && fn_8006F3BC()) {
			game->mCameraActions.mActions[1]->vf14();
		}
		if (fn_8006F3CC() && fn_8006F3BC()) {
			((CCameraActionAreaExM*)game->mCameraActions.mActions[1])->vf64();
		}
		((CCameraActionAnim*)game->mCameraActions.mActions[6])->SetObj((XObjCamera*)game->mXObjs.mPools[0].First());
		game->mMotionControlCamera.Init();
		if (mode == 1 || mode == 7) {
			game->mMotionControlCamera.ChangeAction(7);
			fn_80141754(game->mCameraActions.mActions[7]);
			game->mMotionControlCamera.ApplyAction();
		} else {
			game->mMotionControlCamera.ChangeAction(mode);
		}
		for (i = 0; i < game->mXObjs.mObjectCount; i++) {
			game->mMotionControlCamera.UpdateTarget(game->mXObjs.mObjects[i]);
		}

		if (game->mStatus.mFlags & 0x100) {
			game->mStatus.mFlags &= ~0x300;
			fn_800236EC(&game->mRoboeyes, 0, 0, 0, 0, 1, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		}
		fn_8001146C(game);
		fn_80011270(game);

		game->mStatus.mFlags &= ~0x2000000000;
		mirror = lbl_8035FA24;
		game->mStatus.mFlags &= ~0x8000000000;
		game->mStatus.mFlags &= ~0x100000000000;
		mirrors = 0;
		for (i = 0; i < game->mXObjs.mObjectCount; i++) {
			obj = game->mXObjs.mObjects[i];
			if (obj->_228.mFlags & 1) {
				if (!(game->mStatus.mFlags & 0x2000000000) && (obj->_228.mFlags & 0x20000000) && !(obj->_228.mFlags & 0x400000000000000)) {
					game->mStatus.mFlags |= 0x2000000000;
				}
				if ((obj->_228.mFlags & 0x400) && mirrors < 5) {
					game->mStatus.mFlags |= 0x100000000000;
					jobj = obj->mJObj;
					if (jobj != NULL) {
						mirror->mOwner   = obj;
						jobj             = GetLeaf(jobj);
						mirror->mJObj    = jobj;
						mirror->mVisible = FALSE;
						mirror->mWidth   = 256;
						mirror->mHeight  = 256;
						mirror->mFormat  = GX_TF_RGB565;
						mirror->_0C      = 0;
						mirror->_10      = 0.0f;
						mirror->_14      = 0.0f;
						mirror->_18      = 0;
						mirror->mBuffer  = mm::Alloc(GXGetTexBufferSize(256, 256, GX_TF_RGB565, GX_FALSE, 0), "(mirror)");
						mirror->_11C     = TRUE;
						mirror->mTex.Init(mirror->mBuffer, mirror->mWidth, mirror->mHeight, mirror->mFormat, GX_CLAMP, GX_CLAMP, GX_FALSE);
						for (HSD_DObj* dobj = HSD_JObjGetDObj(jobj); dobj != NULL; dobj = HSD_DObjGetNext(dobj)) {
							HSD_MObj* mobj = HSD_DObjGetMObj(dobj);
							if (mobj != NULL) {
								fn_801BA3AC(mobj, 255, 255, 255);
								HSD_TObj* tobj = HSD_MObjGetTObj(mobj);
								if (tobj != NULL) {
									tobj->imagedesc = (HSD_ImageDesc*)mirror;
								}
							}
						}
						mirror++;
						mirrors++;
					}
				}
				if (!(game->mStatus.mFlags & 0x8000000000) && (obj->_228.mFlags & 0x80000000)) {
					game->mStatus.mFlags |= 0x8000000000;
				}
			}
		}

		if (game->mStatus.mFlags & 0x2000000000) {
			f32 fade  = fn_8006F4B8()->mInfo->mMirrorFade / 100.0f;
			XObj* obj = game->mXObjs.mPools[3].First();
			if ((obj->_228.mFlags & 1) && fade > 0.0f && fade < 1.0f) {
				obj->vfE4();
			}
		}
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x80010D28
 * @note Size: 0x548
 */
// NONMATCHING: register allocation, the counters and loop pointers come out in other registers
// sorts the objects into the ones to draw and the ones mirrors show, those of pool 3 first,
// then the plain ones, then the 0xA00000 ones, then the 0x40 ones. objects out of view are only
// mirrored
extern "C" void fn_80010D28(CGame* game)
{
	s32 shown;
	XObj* obj;
	CMotionControlCamera* camera;
	s32 i;
	s32 mirrored;
	if (game->mStatus.mFlags & 0x8000000000) {
		camera = &game->mMotionControlCamera;
		for (i = 0; i < game->mXObjs.mObjectCount; i++) {
			obj = game->mXObjs.mObjects[i];
			if ((obj->_228.mFlags & 1) && (obj->_228.mFlags & 0x10000)) {
				if (!(game->mStatus.mFlags & 0x100) && (obj->_228.mFlags & 0x80000000) && obj->IsInView(camera)) {
					obj->_228.mFlags |= 0x1000000000000;
				} else {
					obj->_228.mFlags &= ~0x1000000000000;
				}
			}
		}
	}

	mirrored = 0;
	shown    = 0;
	for (i = 0; i < game->mXObjs.mPools[3].mUsed; i++) {
		obj = game->mXObjs.mPools[3].Get(i);
		if ((obj->_228.mFlags & 1) && (obj->_228.mFlags & 0x10000) && !(obj->_228.mFlags & 0xA00040)) {
			if (!(obj->_228.mFlags & 0x1000000000000)) {
				game->_C074[shown++] = obj;
			}
			game->_D878[mirrored++] = obj;
		}
	}
	for (i = 0; i < game->mXObjs.mObjectCount; i++) {
		obj = game->mXObjs.mObjects[i];
		if (obj->mAttr.GetKind() != 3 && (obj->_228.mFlags & 1) && (obj->_228.mFlags & 0x10000) && !(obj->_228.mFlags & 0xA00040)) {
			if (!(obj->_228.mFlags & 0x1000000000000)) {
				game->_C074[shown++] = obj;
			}
			game->_D878[mirrored++] = obj;
		}
	}
	for (i = 0; i < game->mXObjs.mObjectCount; i++) {
		obj = game->mXObjs.mObjects[i];
		if (obj->mAttr.GetKind() != 3 && (obj->_228.mFlags & 1) && (obj->_228.mFlags & 0x10000) && (obj->_228.mFlags & 0xA00000)
		    && !(obj->_228.mFlags & 0x40)) {
			if (!(obj->_228.mFlags & 0x1000000000000)) {
				game->_C074[shown++] = obj;
			}
			game->_D878[mirrored++] = obj;
		}
	}
	for (i = 0; i < game->mXObjs.mObjectCount; i++) {
		obj = game->mXObjs.mObjects[i];
		if (obj->mAttr.GetKind() != 3 && (obj->_228.mFlags & 1) && (obj->_228.mFlags & 0x10000) && (obj->_228.mFlags & 0x40)) {
			if (!(obj->_228.mFlags & 0x1000000000000)) {
				game->_C074[shown++] = obj;
			}
			game->_D878[mirrored++] = obj;
		}
	}
	game->_D874 = shown;
	game->_F078 = mirrored;
}

/**
 * @note Address: 0x80011270
 * @note Size: 0x1FC
 */
// NONMATCHING: the object and the loop pointers get other volatile registers
// lists the objects to draw: those with flag 0x4000 first, then the rest, then the two
// special ones
extern "C" void fn_80011270(CGame* game)
{
	s32 count = 0;
	s32 i;
	for (i = 0; i < game->mXObjs.mObjectCount; i++) {
		if (game->mXObjs.mObjects[i] != game->mXObjs.mPools[2].First() && game->mXObjs.mObjects[i] != game->mXObjs.mPools[0].First()
		    && (game->mXObjs.mObjects[i]->_228.mFlags & 1) && (game->mXObjs.mObjects[i]->_228.mFlags & 0x4000)) {
			game->mDrawList[count]  = game->mXObjs.mObjects[i];
			game->mDrawIndex[count] = i;
			count++;
		}
	}
	for (i = 0; i < game->mXObjs.mObjectCount; i++) {
		if (game->mXObjs.mObjects[i] != game->mXObjs.mPools[2].First() && game->mXObjs.mObjects[i] != game->mXObjs.mPools[0].First()
		    && (game->mXObjs.mObjects[i]->_228.mFlags & 1) && !(game->mXObjs.mObjects[i]->_228.mFlags & 0x4000)) {
			game->mDrawList[count]  = game->mXObjs.mObjects[i];
			game->mDrawIndex[count] = i;
			count++;
		}
	}
	if (game->mXObjs.mPools[2].First()->_228.mFlags & 1) {
		game->mDrawList[count++] = game->mXObjs.mPools[2].First();
	}
	if (game->mXObjs.mPools[0].First()->_228.mFlags & 1) {
		game->mDrawList[count++] = game->mXObjs.mPools[0].First();
	}
	game->mDrawCount = count;
}

/**
 * @note Address: 0x8001146C
 * @note Size: 0x1A0
 */
// NONMATCHING: the light count isn't read again for the second GetLights
// picks the lights to draw with: an object's own when it has some, else the game's. only the
// object lights cast shadows then
extern "C" void fn_8001146C(CGame* game)
{
	game->mLightList = NULL;
	LightOwnerA* a   = (LightOwnerA*)game->mXObjs.mPools[3].mBase;
	if ((a->_228.mFlags & 1) && a->GetLights() != NULL) {
		game->mLightList = a->GetLights();
	} else {
		LightOwnerB* b = (LightOwnerB*)game->mXObjs.mPools[1].mBase;
		if (b->_228.mFlags & 1) {
			game->mLightList = b->GetLights();
		}
	}

	if (game->mLightList != NULL) {
		CLight* light;
		for (light = game->mLightList; light != NULL; light = (CLight*)light->next) {
			if (light->mShadow.IsActive()) {
				HSD_CObjSetProjectionType(light->mShadow.mShadow->camera, 3);
			}
		}
		for (light = game->mLight; light != NULL; light = (CLight*)light->next) {
			if (light->mShadow.IsActive()) {
				light->mShadow.SetActive(FALSE);
			}
		}
	} else {
		game->mLightList = game->mLight;
	}
}

/**
 * @note Address: 0x8001160C
 * @note Size: 0x28
 */
void CCameraActionAreaExM::vf64()
{
	fn_80137EB8(&lbl_805A054C);
}

/**
 * @note Address: 0x80015C54
 * @note Size: 0x10
 */
f32 CCameraActionAreaExM::vf68()
{
	return lbl_805A054C._10;
}

/**
 * @note Address: 0x80015C64
 * @note Size: 0x10
 */
s32 CCameraActionAreaExM::vf6C()
{
	return lbl_805A054C._14;
}

/**
 * @note Address: 0x80015C74
 * @note Size: 0x10
 */
void CCameraActionAreaExM::vf70(s32 value)
{
	lbl_805A054C._14 = value;
}

/**
 * @note Address: 0x80015C84
 * @note Size: 0x44
 */
void CCameraActionAreaExM::vf74(Vec pos)
{
	fn_801374F4(&lbl_805A054C, pos);
}

/**
 * @note Address: 0x80015CC8
 * @note Size: 0x64
 */
BOOL CCameraActionAreaExM::vf78(s32 index)
{
	bool set = CamAreaInfo(&lbl_805A054C._24[index]).Check(4);
	if (set) {
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x80015D74
 * @note Size: 0x64
 */
BOOL CCameraActionAreaExM::vf7C(s32 index)
{
	bool set = CamAreaInfo(&lbl_805A054C._1C[index]).Check(0x80);
	if (set) {
		return FALSE;
	}
	return TRUE;
}

/**
 * @note Address: 0x8000F93C
 * @note Size: 0x84
 */
CVec CVec::operator+(Vec v) const
{
	CVec sum = *this;
	PSVECAdd(&sum, &v, &sum);
	return sum;
}

/**
 * @note Address: 0x8000F9C0
 * @note Size: 0x80
 */
CVec CVec::operator*(f32 s) const
{
	CVec product = *this;
	PSVECScale(&product, &product, s);
	return product;
}

extern "C" void fn_8001D3D4(CCamera* camera, CBase* base);

/**
 * @note Address: 0x80014A64
 * @note Size: 0x144
 */
// FbMirror::Draw, sets up the state the mirror's surface is drawn with
extern "C" void fn_80014A64(FbMirror* mirror)
{
	if (mirror->mJObj != NULL) {
		CGs* gs = &lbl_8025C400.mGs;
		SetupAll(TRUE);
		fn_80199AB0(&lbl_8025C400.mGs, 4, 0);
		fn_8019B56C(gs->mCurRenderMode, 0, 4, 5, 15);
		fn_8019B504(gs->mCurRenderMode, 7, 0, 0, 7, 255);
		fn_8019B464(gs->mCurRenderMode, 1, 3, 1);
		fn_8019B648(gs->mCurRenderMode, 1);
		fn_8019B5D0(gs->mCurRenderMode, 0);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
		fn_8019B360(gs->mCurRenderMode, 1);
		fn_8019B360(gs->mCurRenderMode, 2);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
		fn_8019B504(gs->mCurRenderMode, 4, 0, 0, 3, 255);
	}
}

// copies what was drawn into the effect's texture
static inline void CopyToTexture(CFbTexEff* effect)
{
	fn_8019B648(lbl_8025C400.mGs.mCurRenderMode, 1);
	fn_8019B5D0(lbl_8025C400.mGs.mCurRenderMode, 1);
	fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 0, 7, 1);
	SetupAll(TRUE);
	if (effect->mBuffer != NULL) {
		u16 width, height;
		if (effect->_18) {
			width  = effect->mWidth << 1;
			height = effect->mHeight << 1;
		} else {
			width  = effect->mWidth;
			height = effect->mHeight;
		}
		GXSetTexCopySrc(0, 0, width, height);
		GXSetTexCopyDst(effect->mWidth, effect->mHeight, effect->mFormat, effect->_18);
		GXCopyTex(effect->mBuffer, GX_TRUE);
		GXPixModeSync();
		InvalidateAll();
	}
}

/**
 * @note Address: 0x800146D0
 * @note Size: 0x1F0
 */
// FbSubwin::vf10
extern "C" void fn_800146D0(FbSubwin* subwin)
{
	CopyToTexture(subwin);
}

/**
 * @note Address: 0x80014BA8
 * @note Size: 0x1FC
 */
// FbMirror::vf10, copies what was drawn from the mirror camera into the texture
extern "C" void fn_80014BA8(FbMirror* mirror)
{
	if (mirror->mVisible) {
		CopyToTexture(mirror);
	}
}

/**
 * @note Address: 0x8000D704
 * @note Size: 0x34C
 */
// NONMATCHING: the mirror, its view and the object list pointer rotate registers
// draws what each mirror shows into its texture
extern "C" void fn_8000D704(CGame* game)
{
	MtxPtr view;
	FbMirror* mirror;
	s32 i;
	s32 j;
	XObj* obj;
	if ((game->mStatus.mFlags & 1) && (game->mStatus.mFlags & 0x100000000000)) {
		for (i = 0; i < 5; i++) {
			mirror = &lbl_8035FA24[i];
			if (mirror->mJObj != NULL && mirror->mVisible && !mirror->IsOff()) {
				game->mGs.mCamera = &mirror->mCamera;
				if (fn_8019997C(game)) {
					view = mirror->mViewMtx;
					for (j = 0; j < game->_F078; j++) {
						obj = game->_D878[j];
						if (!(obj->_228.mFlags & 0x200000) && (obj->_228.mFlags & 0x200000000000)) {
							obj->DrawMirrored(view);
						}
					}
					fn_80199944(&game->mGs);
					if (mirror->mVisible) {
						CopyToTexture(mirror);
					}
				}
			}
		}
	}
}

/**
 * @note Address: 0x80014DA4
 * @note Size: 0x2BC
 */
// NONMATCHING: the surface check is a compare where the original tests a bool
// FbMirror::Update, puts the mirror camera at the main camera's eye reflected in the surface,
// looking at the surface through a frustum that just fits it
extern "C" void fn_80014DA4(FbMirror* mirror, u32 frames)
{
	HSD_JObj* jobj = mirror->mJObj;
	if (jobj != NULL) {
		Mtx mtx;
		Mtx view;
		Vec eye;
		Vec pos;
		Vec diff;
		Vec x;
		Vec y;
		Vec normal;

		PSMTXCopy(HSD_JObjGetMtxPtr(jobj), mtx);
		x.x = mtx[0][0];
		x.y = mtx[1][0];
		x.z = mtx[2][0];
		y.x = mtx[0][1];
		y.y = mtx[1][1];
		y.z = mtx[2][1];
		PSVECScale(&x, &x, -1.0f);
		PSVECCrossProduct(&x, &y, &normal);
		PSVECNormalize(&normal, &normal);
		mtx[0][0] = x.x;
		mtx[1][0] = x.y;
		mtx[2][0] = x.z;
		mtx[0][2] = normal.x;
		mtx[1][2] = normal.y;
		mtx[2][2] = normal.z;

		CMotionControlCamera* main = &lbl_8025C400.mMotionControlCamera;
		eye.x                      = main->mCamera.mEye.x;
		eye.y                      = main->mCamera.mEye.y;
		eye.z                      = main->mCamera.mEye.z;
		HSD_MtxGetTranslate(mtx, &pos);
		PSVECSubtract(&pos, &eye, &diff);
		f32 dist = PSVECDotProduct(&normal, &diff);
		if (dist <= 0.0f) {
			mirror->mVisible = FALSE;
		} else {
			PSVECScale(&normal, &diff, 2.0f * dist);
			PSVECAdd(&diff, &eye, &diff);
			mtx[0][3] = diff.x;
			mtx[1][3] = diff.y;
			mtx[2][3] = diff.z;
			PSMTXInverse(mtx, view);
			PSMTXMultVec(view, &pos, &pos);
			mirror->mCamera.SetNear(0.1f - pos.z);
			mirror->mCamera.SetFar(32768.0f + (0.1f - pos.z));
			mirror->mCamera.SetFrustum(0.5f + pos.y, -0.5f + pos.y, -0.5f + pos.x, 0.5f + pos.x);
			mirror->mCamera.SetEye(diff);
			mirror->mCamera.SetMtx(view);
			PSMTXCopy(view, mirror->mViewMtx);
			mirror->mVisible = TRUE;
		}
	}
}

/**
 * @note Address: 0x800151E0
 * @note Size: 0xC0
 */
// FbMirror::Init
extern "C" void fn_800151E0(FbMirror* mirror)
{
	mirror->mJObj = NULL;
	mirror->mCamera.Init(0.1f, 32768.0f, 0.5f, -0.5f, -0.5f, 0.5f, &lbl_8025C400.mGs.mScreen);
	mirror->mCamera.SetViewport(0.0f, 256.0f, 0.0f, 256.0f);
	mirror->mCamera.SetScissor(0, 256, 0, 256);
	mirror->Reset();
}

/**
 * @note Address: 0x80018CCC
 * @note Size: 0x4
 */
extern "C" void fn_80018CCC(CFbTexEff* effect)
{
}

/**
 * @note Address: 0x80018CD0
 * @note Size: 0x4
 */
extern "C" void fn_80018CD0(CFbTexEff* effect)
{
}

/**
 * @note Address: 0x80018CD4
 * @note Size: 0x4
 */
extern "C" void fn_80018CD4(CFbTexEff* effect)
{
}

/**
 * @note Address: 0x80018CD8
 * @note Size: 0x4
 */
extern "C" void fn_80018CD8(CFbTexEff* effect, u32 frames)
{
}

/**
 * @note Address: 0x80018CDC
 * @note Size: 0x4
 */
extern "C" void fn_80018CDC(CFbTexEff* effect)
{
}

/**
 * @note Address: 0x80014978
 * @note Size: 0xEC
 */
extern "C" void fn_80014978(FbSubwin* subwin)
{
	subwin->mWidth  = 320;
	subwin->mHeight = 224;
	subwin->mFormat = GX_TF_RGB565;
	subwin->_0C     = 0;
	subwin->_10     = 0.0f;
	subwin->_14     = 0.0f;
	subwin->_18     = 0;
	subwin->mBuffer = mm::Alloc(GXGetTexBufferSize(320, 224, GX_TF_RGB565, GX_FALSE, 0), "(subwin)");
	subwin->_11C    = TRUE;
	subwin->mTex.Init(subwin->mBuffer, subwin->mWidth, subwin->mHeight, subwin->mFormat, GX_CLAMP, GX_CLAMP, GX_FALSE);
	fn_8001D3D4(&subwin->mCamera, &lbl_8025C400);
	HSD_RectS16 viewport = { 0, 320, 0, 224 };
	HSD_CObjSetViewport(&subwin->mCamera, &viewport);
	subwin->Reset();
}

/**
 * @note Address: 0x80014964
 * @note Size: 0x14
 */
extern "C" void fn_80014964(FbSubwin* subwin)
{
	subwin->mOpen  = FALSE;
	subwin->mAlpha = 0.0f;
}

/**
 * @note Address: 0x800148C0
 * @note Size: 0xA4
 */
extern "C" void fn_800148C0(FbSubwin* subwin, u32 frames)
{
	if (subwin->mOpen) {
		if (subwin->mAlpha < 1.0f) {
			if ((subwin->mAlpha += frames * (1.0f / 18.0f)) >= 1.0f) {
				subwin->mAlpha = 1.0f;
			}
		}
	} else {
		if (subwin->mAlpha > 0.0f) {
			if ((subwin->mAlpha -= frames * (1.0f / 18.0f)) <= 0.0f) {
				subwin->mAlpha = 0.0f;
			}
		}
	}
}

/**
 * @note Address: 0x800136B4
 * @note Size: 0xD0
 */
extern "C" void fn_800136B4(FbSubwin* subwin, const Vec& interest, f32 angleY, f32 angleX, f32 distance, f32 fov)
{
	subwin->mCamera.SetInterest(interest);
	subwin->mCamera.SetAngleY(angleY);
	subwin->mCamera.SetAngleX(angleX);
	subwin->mCamera.SetDistance(distance);
	subwin->mCamera.SetFov(fov);
	subwin->mCamera.CalcEye();
}
