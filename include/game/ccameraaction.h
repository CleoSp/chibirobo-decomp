#ifndef _GAME_CCAMERAACTION_H
#define _GAME_CCAMERAACTION_H

#include "Version.h"
#include "game/ccamera.h"
#include "types.h"

// pal uses the current refresh rate, 50 or 60
#if defined(VERSION_PAL)
extern "C" s32 lbl_80660D4C;
#define FRAME_TIME (1.0f / lbl_80660D4C)
#else
#define FRAME_TIME (1.0f / 60.0f)
#endif

// class names from RTTI, the rest is guessed

// a game object as the camera sees it
class XObj {
public:
	HSD_JObj* mJObj; // _00
	u8 _04[0x30];    // _04
	// _34 vtable

	virtual void vf08();
	virtual void vf0C();
	virtual void vf10();
	virtual void vf14();
	virtual void vf18();
	virtual void vf1C();
	virtual void vf20();
	virtual void vf24();
	virtual void vf28(f32 y, s32 force); // _28, sets the y rotation
	virtual void vf2C();
	virtual void vf30();
	virtual void vf34();
	virtual void vf38();
	virtual void vf3C(f32 x, f32 y, f32 z, s32 force); // _3C, sets the scale
	virtual void vf40();
	virtual void vf44();
	virtual void vf48();
	virtual void vf4C();
	virtual void vf50();
	virtual void vf54();
	virtual void vf58();
	virtual void vf5C();
	virtual void vf60();
	virtual void vf64();
	virtual void vf68(f32 x, f32 y, f32 z, s32 force); // _68, sets the position
	virtual void vf6C();
	virtual void vf70();
	virtual void vf74();
	virtual void vf78();
	virtual void vf7C();
	virtual void vf80();
	virtual void vf84();
	virtual void vf88();
	virtual void vf8C();
	virtual void vf90();
	virtual void vf94();
	virtual void vf98();
	virtual void vf9C();
	virtual void vfA0();
	virtual void vfA4(s32 arg); // _A4
	virtual void vfA8();
	virtual void vfAC();
	virtual void vfB0();
	virtual void vfB4();
	virtual void vfB8();
	virtual void vfBC();
	virtual void vfC0();
	virtual void vfC4();
	virtual void vfC8();
	virtual void vfCC();
	virtual void vfD0();
	virtual void vfD4();
	virtual void vfD8();
	virtual void vfDC();
	virtual void vfE0();
	virtual void vfE4();
	virtual void vfE8();
	virtual Vec GetCameraTarget(); // _EC, where a camera looks at this object
	virtual void vfF0();
	virtual void vfF4();
	virtual void vfF8();
	virtual void vfFC();
	virtual void vf100();
	virtual void vf104();
	virtual void vf108();
	virtual void vf10C(Vec* pos, f32* x, f32* y, f32* z, s32 arg); // _10C, a point near the object on screen
	virtual f32 vf110();                                           // _110
	virtual f32 vf114();                                           // _114

#if defined(VERSION_JP)
	u8 _38[0xC0 - 0x38];    // _38
	CVec mMoveTranslate;    // _C0
	u8 _D0[0x220 - 0xD0];   // _D0
	u64 mFlags;             // _220
	u8 _228[0x23C - 0x228]; // _228
	s32* mKind;             // _23C, _244 in usa
	u8 _240[0x25C - 0x240]; // _240
	Vec _264;               // _25C, _264 in usa
#else
	u8 _38[0xC8 - 0x38];    // _38
	CVec mMoveTranslate;    // _C8, start of CXObj's CObjVec
	u8 _D8[0x228 - 0xD8];   // _D8
	u64 mFlags;             // _228, bit 0 while it's in use, 0x10000 while it's drawn
	u8 _230[0x244 - 0x230]; // _230
	s32* mKind;             // _244
	u8 _248[0x264 - 0x248]; // _248
	Vec _264;               // _264, POS copies the position here
#endif
};

inline s32 XObjKind(const XObj* obj)
{
	return obj->mKind != NULL ? *obj->mKind : -1;
}

// the object that plays camera animations
class XObjCamera : public XObj {
public:
#if defined(VERSION_JP)
	u8 _268[0x970 - 0x268]; // _268
	CCamera mCamera;        // _970, japan's CJArchive is 8 bytes shorter, and something after the flags 8 more
#else
	u8 _270[0x980 - 0x270]; // _270
	CCamera mCamera;        // _980
#endif
};

// the player, entry 0 of the object table. offsets are US
struct CameraPlayer {
	u8 _00[0xF8]; // _00
	CVec mRot;    // _F8
#if defined(VERSION_JP)
	// 0x10 shorter in japan somewhere in here
	u8 _108[0x930]; // _108
#else
	u8 _108[0x940]; // _108
#endif
	s32 mState;      // _A48
	u8 _A4C[0x78];   // _A4C
	s32 _AC4;        // _AC4
	u8 _AC8[0x4C];   // _AC8
	s32 _B14;        // _B14
	u8 _B18[0x50];   // _B18
	s32 _B68;        // _B68
	u8 _B6C[0x40];   // _B6C
	s32 _BAC;        // _BAC
	u8 _BB0[0x3C];   // _BB0
	s32 _BEC;        // _BEC
	u8 _BF0[0x54];   // _BF0
	s32 _C44;        // _C44
	u8 _C48[0x8E60]; // _C48
	f32 _9AA8;       // _9AA8
	f32 _9AAC;       // _9AAC
};

// moves a vector from one value to another over time
class CInterfaceVectorInterpolation {
public:
	// _00 vtable

	virtual ~CInterfaceVectorInterpolation() { }        // _08
	virtual void Start(Vec from, Vec to, f32 time) = 0; // _0C
	virtual void vf10()                            = 0; // _10
	virtual bool Update(u32 step, f32 dt)          = 0; // _14, TRUE once it's done
	virtual Vec Get()                              = 0; // _18
};

// the same for one float
class CInterfaceInterpolation {
public:
	// _00 vtable

	virtual ~CInterfaceInterpolation() { }              // _08
	virtual void Start(f32 from, f32 to, f32 time) = 0; // _0C
	virtual void vf10()                            = 0; // _10
	virtual bool Update(u32 step, f32 dt)          = 0; // _14
	virtual f32 Get()                              = 0; // _18
};

#define DECLARE_VEC_INTERP(name, size)                  \
	class name : public CInterfaceVectorInterpolation { \
	public:                                             \
		u8 _04[size - 4];                               \
		virtual ~name()                                 \
		{                                               \
		}                                               \
		virtual void Start(Vec from, Vec to, f32 time); \
		virtual void vf10();                            \
		virtual bool Update(u32 step, f32 dt);          \
		virtual Vec Get();                              \
	};
#define DECLARE_INTERP(name, size)                      \
	class name : public CInterfaceInterpolation {       \
	public:                                             \
		u8 _04[size - 4];                               \
		virtual ~name()                                 \
		{                                               \
		}                                               \
		virtual void Start(f32 from, f32 to, f32 time); \
		virtual void vf10();                            \
		virtual bool Update(u32 step, f32 dt);          \
		virtual f32 Get();                              \
	};

DECLARE_VEC_INTERP(CVectorLinearInterpolation, 0x34)
DECLARE_VEC_INTERP(CVectorEaseInOutInterpolation, 0x5C)
DECLARE_VEC_INTERP(CVectorEaseOutInterpolation, 0x64)
DECLARE_VEC_INTERP(CVectorNoneInterpolation, 0x34)
DECLARE_VEC_INTERP(CVectorEaseOutDivideInterpolation, 0x34)
DECLARE_INTERP(CInterpolationLinear, 0x10)
DECLARE_INTERP(CInterpolationEaseInOut, 0x1C)
DECLARE_INTERP(CInterpolationEaseOut, 0x20)
DECLARE_INTERP(CInterpolationNone, 0x10)
DECLARE_INTERP(CInterpolationEaseOutDivide, 0x10)

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
	virtual void SetCamera(CCamera* camera) { } // _18, takes eye, interest, angle, distance and fov
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

	// set the mode word along with the vector
	void SetEye(const Vec& eye, u32 mode)
	{
		_38 = mode;
		SetEye(eye);
	}
	void SetInterest(const Vec& interest, u32 mode)
	{
		_38 = mode;
		SetInterest(interest);
	}
};

// base of the area, map and reset actions. fields from _40 on differ per subclass
class CCameraActionAreaExM : public CInterfaceCameraAction {
public:
	virtual void vf64(); // _64
	virtual f32 vf68();  // _68
	virtual void vf6C(); // _6C
	virtual void vf70(); // _70
	virtual void vf74(); // _74
	virtual void vf78(); // _78
	virtual void vf7C(); // _7C
};

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
	u8 _8C[0xBC - 0x8C];                    // _8C
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
	u8 _E14[0xE64 - 0xE14];                 // _E14
	CInterfaceInterpolation* _E64;          // _E64
	CInterpolationEaseOutDivide _E68;       // _E68
	u8 _E78[0xEA8 - 0xE78];                 // _E78
	s32 _EA8;                               // _EA8
	s32 _EAC;                               // _EAC
	s32 _EB0;                               // _EB0
	u8 _EB4[0xF28 - 0xEB4];                 // _EB4
	CInterfaceVectorInterpolation* _F28;    // _F28
	CVectorEaseOutDivideInterpolation _F2C; // _F2C
	u8 _F60[0xF68 - 0xF60];                 // _F60
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
STATIC_ASSERT(sizeof(CCameraActionMap) == 0x11EC);

// an entry in a CamAreaSlot list
struct CamAreaEntry {
	u8 _00[0x20]; // _00
	Vec _20;      // _20, an angle
	u8 _2C[0xC];  // _2C
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

// wraps a CamAreaRecord, name from RTTI
class CamAreaInfo {
public:
	CamAreaInfo(CamAreaRecord* record)
	    : _04(record)
	{
	}
	virtual ~CamAreaInfo() { } // _08

	bool Is_2C_1() const { return _04->_2C == 1; }

	// _00 vtable
	CamAreaRecord* _04; // _04
};

struct CamAreaSlot {
	CamAreaEntry* _00; // _00
	u8 _04[0x4];       // _04
	s32 _08;           // _08, entries in _00
	u8 _0C[0x10];      // _0C
};

// camera area state, class unknown
struct CamAreaState {
	u8 _00[0x4];            // _00
	s32 _04;                // _04
	s32 _08;                // _08
	s32 _0C;                // _0C
	f32 _10;                // _10
	s32 _14;                // _14
	s32 _18;                // _18
	CamAreaRecord* _1C;     // _1C
	s32 _20;                // _20, records in _1C
	CamAreaRecord* _24;     // _24
	s32 _28;                // _28, records in _24
	u8 _2C[0x2034 - 0x2C];  // _2C
	CamAreaSlot _2034[256]; // _2034, count unknown, 256 fills out the 0x3C34 object
};
extern "C" CamAreaState lbl_805A054C;

// puts the camera back behind the player, see ccameraactionreset.cpp
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

	void SetEye_Interp(s32 mode, const Vec& eye, f32 time)
	{
		if (mode == 3 || time <= 0.0f) {
			mEyeInterp = NULL;
			SetEye(eye);
			return;
		}
		mEyeInterp = mEyeInterps[mode];
		if (mEyeInterp != NULL) {
			mEyeInterp->Start(mEye, eye, time);
		}
	}
	void SetFov_Interp(s32 mode, f32 fov, f32 time)
	{
		if (mode == 3 || time <= 0.0f) {
			mFovInterp = NULL;
			SetFov(fov);
			return;
		}
		mFovInterp = mFovInterps[mode];
		if (mFovInterp != NULL) {
			mFovInterp->Start(mFov, fov, time);
		}
	}
};
STATIC_ASSERT(sizeof(CCameraActionReset) == 0x208);

// a camera that looks along the z axis at the player from a fixed distance, the dpad or
// c stick swaps between a near and a far one. see ccameraactionscroll.cpp
class CCameraActionScroll : public CInterfaceCameraAction {
public:
	BOOL mStart;                               // _40, the next update places the camera from scratch
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

	void SetZoomDistance(f32 distance)
	{
		mDistanceInterp = NULL;
		SetDistance(distance);
	}
	void StartZoom(f32 distance)
	{
		mDistanceInterp = &mDistanceEase;
		if (mDistanceInterp != NULL) {
			mDistanceInterp->Start(mDistance, distance, 15.0f);
		}
	}
};
STATIC_ASSERT(sizeof(CCameraActionScroll) == 0x6C);

// behind the player at an angle off its facing
class CCameraActionLadr : public CInterfaceCameraAction {
public:
	f32 mAngle; // _40, added to the player's y rotation

	virtual void Update(u32 step);      // _10
	virtual void vf14();                // _14
	virtual u32 Get_38() { return 11; } // _54
	virtual Vec GetCameraTarget();      // _64, the player

	void CalcEye(Vec* eye, f32 angle);
};
STATIC_ASSERT(sizeof(CCameraActionLadr) == 0x44);

// just looks at the player, the camera keeps everything else
class CCameraActionDefault : public CInterfaceCameraAction {
public:
	virtual void Update(u32 step); // _10
};
STATIC_ASSERT(sizeof(CCameraActionDefault) == 0x40);

// follows the player from behind on a damped spring, the stick or dpad turns and zooms it
class CCameraActionChase : public CInterfaceCameraAction {
public:
	XObj* mTarget;     // _40
	f32 mRestDistance; // _44
	f32 mDamping;      // _48
	CVec mLastPos;     // _4C
	CVec mPos;         // _5C, the point behind the target the eye is pulled to
	u32 mMode;         // _6C

	virtual void Update(u32 step);           // _10
	virtual void vf14();                     // _14
	virtual void SetCamera(CCamera* camera); // _18
	virtual void Init();                     // _1C
	virtual u32 Get_38() { return mMode; }   // _54
};
STATIC_ASSERT(sizeof(CCameraActionChase) == 0x70);

// the camera event scripts drive, every part eases to where the script says
class CCameraActionEvt : public CInterfaceCameraAction {
public:
	XObj* mTarget;                                      // _40, the interest follows it while nothing eases it
	CInterfaceVectorInterpolation* mEyeInterp;          // _44
	CInterfaceVectorInterpolation* mInterestInterp;     // _48
	CInterfaceInterpolation* mDistanceInterp;           // _4C
	CInterfaceInterpolation* mFovInterp;                // _50
	CInterfaceVectorInterpolation* mAngleInterp;        // _54
	CStatus mAxes;                                      // _58, which angles SetAngle_Interp moves
	u32 _60;                                            // _60
	CVectorLinearInterpolation mEyeLinear;              // _64
	CVectorEaseInOutInterpolation mEyeEaseInOut;        // _98
	CVectorEaseOutInterpolation mEyeEaseOut;            // _F4
	CVectorNoneInterpolation mEyeNone;                  // _158
	CInterfaceVectorInterpolation* mEyeInterps[4];      // _18C, by mode, 3 sets it at once
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

	void SetAngle_Interp(s32 mode, const Vec& angle, u32 axes, u32 type, BOOL relative, f32 time);
	void SetFov_Interp(s32 mode, f32 fov, f32 time);
	void SetInterest_Interp(s32 mode, const Vec& interest, f32 time);
	void SetInterest_Interp(s32 mode, XObj* target, f32 time);
	void SetEye_Interp(s32 mode, const Vec& eye, f32 time);
};
STATIC_ASSERT(sizeof(CCameraActionEvt) == 0x4E4);

// how far the room camera can pan, at its widest and its narrowest fov
struct Scroll2Limits {
	f32 mWideMinX;   // _00
	f32 mWideMaxX;   // _04
	f32 mWideMinZ;   // _08
	f32 mWideMaxZ;   // _0C
	f32 mNarrowMinX; // _10
	f32 mNarrowMaxX; // _14
	f32 mNarrowMinZ; // _18
	f32 mNarrowMaxZ; // _1C
	f32 mMaxFov;     // _20
	f32 mMinFov;     // _24
	f32 mMidFov;     // _28, z narrows only below it
};

// the room camera, it pans over the room with the stick and zooms with the c stick
class CCameraActionScroll2 : public CInterfaceCameraAction {
public:
	Vec* mCenterPtr;     // _40, always mCenter
	Vec mCenter;         // _44
	f32 mMinX;           // _50
	f32 mMaxX;           // _54
	f32 mMinZ;           // _58
	f32 mMaxZ;           // _5C
	f32 mMaxFov;         // _60
	f32 mMinFov;         // _64
	f32 mYaw;            // _68
	Vec mOffset;         // _6C, eye minus interest
	Vec mStart;          // _78, the interest it started from
	BOOL mTurned;        // _84, the camera is turned a quarter round x in room 11
	BOOL mFromStage;     // _88
	u8 mSavedInfo[0x20]; // _8C

	virtual void Update(u32 step);      // _10
	virtual void vf14();                // _14
	virtual void vf20();                // _20, draws the markers over the room's objects
	virtual void vf28() { }             // _28
	virtual u32 Get_38() { return 11; } // _54

	void SaveCamInfo(const void* info);
	Scroll2Limits* GetLimits(s32 room);
	void ShowObjs();
	void HideObjs();
	void ClearHideList();
	BOOL AddHideObj(s32 id);
};
STATIC_ASSERT(sizeof(CCameraActionScroll2) == 0xAC);

// plays a camera animation, copying an XObjCamera's camera every frame
class CCameraActionAnim : public CInterfaceCameraAction {
public:
	CCamera* mCamera; // _40
	XObjCamera* mObj; // _44

	virtual void Update(u32 step);           // _10
	virtual void SetCamera(CCamera* camera); // _18
	virtual void Init();                     // _1C

	void SetObj(XObjCamera* obj);
};
STATIC_ASSERT(sizeof(CCameraActionAnim) == 0x48);

#endif
