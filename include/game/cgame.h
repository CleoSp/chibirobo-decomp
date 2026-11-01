#ifndef _GAME_CGAME_H
#define _GAME_CGAME_H

#include "types.h"
#include "Version.h"
#include "game/xsound.h"
#include "game/memcard.h"
#include "game/cstatus64.h"

class XObj;
class XObjChr;
class CCamera;
class CLight;
class CInterfaceCameraAction;

// one of CBase's frame timers, class name from RTTI
class CStopwatch {
public:
	u8 mWatch[0x30]; // _00, the sdk's OSStopwatch
	s64 mElapsed;    // _30
	// _38 vtable

	virtual ~CStopwatch(); // _08
};
STATIC_ASSERT(sizeof(CStopwatch) == 0x40);

// the time of day evt's TIME command drives, mode 1 is night
struct EvtTimeState {
	s32 mMode;     // _00
	f32 mTime;     // _04
	f32 mDuration; // _08
	s32 mRunning;  // _0C
};

// one of XOBJS's object pools, xobjs.cpp has the same layout as UnkXObjPool
struct XObjPool {
	u8* mObjects;      // _00
	s32 mUsed;         // _04, objects handed out so far
	s32 mCount;        // _08
	const char* mName; // _0C
	s32 mStride;       // _10, size of one object
};
STATIC_ASSERT(sizeof(XObjPool) == 0x14);

// objects in draw order, CGame keeps three
struct XObjDrawList {
	XObj* mObjects[0x400]; // _0000
	s16 mIndices[0x400];   // _1000, each one's slot in CGame's mObjects
	s32 mCount;            // _1800
};
STATIC_ASSERT(sizeof(XObjDrawList) == 0x1804);

// the system half of the game object, class names from RTTI, layout from __ct__5CBaseFv.
// japan is 0x18 shorter from mFont on, 0x1C from the vtable, europe is 4 longer from
// mGs's camera on and 8 from mStatus. the class members stay untyped, their headers need
// the hsd include path
class CBase {
public:
#if defined(VERSION_JP)
	u8 mGs[0x17A8]; // _0000, a CGs
#elif defined(VERSION_PAL)
	u8 mGs[0x17B8]; // _0000
#else
	u8 mGs[0x17B4]; // _0000
#endif
#if !defined(VERSION_JP)
	f32 mTextRot[3]; // _17B4, cgs.cpp's text rotation
#endif
	u8 mFont[0x140];           // _17C0, _17A8 in japan, a CFont
	u8 mPad[0xB4];             // _1900, CPadEx starts here, its type needs sysdolphin/controller.h
	u8 mPadOne[4][0x54];       // _19B4, its CPadOnes
	u8 _1B04[0x1B08 - 0x1B04]; // _1B04, its vtable
	u32 mLastRetrace;          // _1B08
	u32 mRetraceStep;          // _1B0C, retraces since the last frame, at most 4
	CStatus64 mStatus;         // _1B10, _1AF8 in japan, _1B18 in europe
	u8 mCameras[2][0x194];     // _1B20, CCameras
	u8 mLights[6][0x358];      // _1E48, CLights
	CStopwatch mStopwatch[3];  // _3258, copy, update and draw
#if !defined(VERSION_JP)
	u8 mLanguage; // _3318, 6 is japanese
#endif
	s32 mDvdStatus;     // _331C, _3300 in japan, DVDGetDriveStatus
	s32 mPrevDvdStatus; // _3320
	BOOL mDiskOk;       // _3324, DVDCheckDisk
	BOOL mDvdError;     // _3328, rumble is paused while set
	s32 mResetHold;     // _332C, frames the reset combo is held
	BOOL mReset;        // _3330, stops fn_80132950's read loop when set
	// _3334 vtable

	virtual ~CBase();                               // _08
	virtual void Initialize(int argc, char** argv); // _0C
	virtual void Start();                           // _10
	virtual void BeforeUpdate();                    // _14
	virtual void Update(s32 frames);                // _18
	virtual void Draw();                            // _1C
	virtual void vf20();                            // _20
	virtual void vf24();                            // _24
	virtual s32 FrameInterval();                    // _28
	virtual void vf2C();                            // _2C
	virtual CCamera* vf30();                        // _30
	virtual void vf34();                            // _34
	virtual CCamera* GetCamera();                   // _38, vf30's unless overridden
};

// the main game object, its one instance is lbl_8025C400. the ctor builds members up to
// +0xFF470, way past dtk's 0xA6C30 symbol size. japan is 0x20 shorter from mTpl, 0x48 from
// mInfoScrn, 0xCE8 after the event data and 0xE00 from mMemcard. europe matches usa from mTpl
class CGame : public CBase {
public:
	XObj* GetObject(s32 index) const { return mObjects[index]; }
	// all of XOBJS's pools from the first
	const XObjPool* GetPools() const { return _9694; }

	u32 _3338;          // _3338, _3320 in japan, _3340 in europe
	u8 mArchive[0x108]; // _333C, a CFileArchive
	s32 mStage;         // _3444, _342C in japan, _344C in europe
	s32 mPrevStage;     // _3448
	s32 _344C;          // _344C, room count
#if defined(VERSION_JP) || defined(VERSION_PAL)
	u8 _3450[0x3C80 - 0x3450 - 8]; // _3450
#else
	u8 _3450[0x3C80 - 0x3450]; // _3450
#endif
	u8 mTpl[0x1F0];                  // _3C80, _3C60 in japan, the shared CTpl
	u8 mFade[0x3E90 - 0x3E70];       // _3E70, CScrnFade starts here
	f32 _3E90[4];                    // _3E90, all zero once the fade is done
	u8 _3EA0[0x3EA4 - 0x3EA0];       // _3EA0, its vtable
	u8 mWipeCircle[0x3EF8 - 0x3EA4]; // _3EA4, CScrnWipeCircle starts here
	f32 _3EF8[2];                    // _3EF8, zero once the wipe is done, the same in every wipe
	u8 _3F00[0x3F08 - 0x3F00];       // _3F00
	u8 mWipeTex[0x3F5C - 0x3F08];    // _3F08, CScrnWipeTex starts here
	f32 _3F5C[2];                    // _3F5C
	u8 _3F64[0x3F6C - 0x3F64];       // _3F64
	u8 mWipeTexObj[0x100];           // _3F6C, its CTexObj
	u8 mWipeCustom[0x40C0 - 0x406C]; // _406C, CScrnWipeCustom starts here
	f32 _40C0[2];                    // _40C0
	u8 _40C8[0x40D0 - 0x40C8];       // _40C8
	u8 mWipeTile[0x4124 - 0x40D0];   // _40D0, CScrnWipeTile starts here
	f32 _4124[2];                    // _4124
	u8 _412C[0x4134 - 0x412C];       // _412C
	u8 mWipeTile2[0x4168 - 0x4134];  // _4134, another CScrnWipeTile
	s32 _4168;                       // _4168
	u8 _416C[0x4174 - 0x416C];       // _416C
	f32 _4174;                       // _4174
	u8 _4178[0x417C - 0x4178];       // _4178
	f32 _417C;                       // _417C
	u8 _4180[0x4188 - 0x4180];       // _4180
	f32 _4188[2];                    // _4188
	u8 _4190[0x4198 - 0x4190];       // _4190
	u8 mRoboeyes[0x41F4 - 0x4198];   // _4198, CScrnRoboeyes starts here
	f32 _41F4;                       // _41F4
	u8 _41F8[0x4204 - 0x41F8];       // _41F8
	u8 mOutline[0x64];               // _4204, a CFbOutline, see game/fbeffect.h
	u8 mBlur[0x4290 - 0x4268];       // _4268, CFbBlur starts here
	f32 _4290;                       // _4290, its mAlpha
	f32 _4294;                       // _4294, mTarget
	f32 _4298;                       // _4298, mSpeed
	f32 _429C;                       // _429C, mOffsetX
	f32 _42A0;                       // _42A0, mOffsetY
	u8 _42A4[0x42AC - 0x42A4];       // _42A4
	u8 mSubwin[0x43D0 - 0x42AC];     // _42AC, FbSubwin starts here
	u32 _43D0;                       // _43D0, _43B0 in japan
	u32 _43D4;                       // _43D4
	u8 _43D8[0x43E0 - 0x43D8];       // _43D8
	f32 _43E0;                       // _43E0
	f32 _43E4;                       // _43E4
	f32 _43E8;                       // _43E8
	f32 _43EC;                       // _43EC
	u8 _43F0[0x46BC - 0x43F0];       // _43F0
	u8 mColor[0x46DC - 0x46BC];      // _46BC, CScrnColor starts here
	f32 _46DC[4];                    // _46DC, all zero once it's done
	u8 _46EC[0x46F0 - 0x46EC];       // _46EC
	u8 _46F0[0x472C - 0x46F0];       // _46F0, another CScrnColor
	s32 _472C;                       // _472C, Logo starts here, its state
	u8 _4730[0x4930 - 0x4730];       // _4730
	s32 _4930;                       // _4930, its state
#if defined(VERSION_JP)
	u8 _4934[0x8580 - 0x4934 - 0x28]; // _4934
#else
	u8 _4934[0x8580 - 0x4934]; // _4934
#endif
	s32 _8580;                     // _8580, _8538 in japan
	void* _8584[1];                // _8584, menus, evt indexes it by id
	u8 _8588[0x8F38 - 0x8588];     // _8588
	u8 mInfoScrn[0x9108 - 0x8F38]; // _8F38, _8EF0 in japan, an InfoScrn
	u32 _9108;                     // _9108, InfoScrn's, the game state menus put back
	u8 _910C[0x9114 - 0x910C];     // _910C
	u8 mParticle[0x548];           // _9114, _90CC in japan, a CParticle
	u8 mQuake[0x967C - 0x965C];    // _965C, CScrnQuake starts here
	f32 _967C[3];                  // _967C, its third vector
	u8 _9688[0x9694 - 0x9688];     // _9688
	XObjPool _9694[2];             // _9694, XOBJS starts here
	XObjChr* mPlayer;              // _96BC, _9674 in japan, pool 2's one object, chibi robo
	s32 _96C0;                     // _96C0, the rest of pool 2
	s32 _96C4;                     // _96C4
	const char* _96C8;             // _96C8
	s32 _96CC;                     // _96CC, its stride
	XObjPool mPools[20];           // _96D0, pools 3 to 22
	XObj* mObjects[0x400];         // _9860, _9818 in japan, the object table
	s32 mObjectCount;              // _A860, _A818 in japan, its count
	u8 _A864[0xA870 - 0xA864];     // _A864, XOBJS's vtable at _A86C
	XObjDrawList mDrawLists[3];    // _A870, _A828 in japan
	u8 mObjArchives[0x400][0x138]; // _F07C, CObjArchives
	void* _5D07C;                  // _5D07C, CEvt starts here
	void* _5D080;                  // _5D080, _5D038 in japan, the event's object list
	u8 _5D084[0x5D08C - 0x5D084];  // _5D084
	s32 _5D08C;                    // _5D08C
	u8 _5D090[0x5D0E8 - 0x5D090];  // _5D090, CEvt's vtable at _5D0E0
#if defined(VERSION_JP)
	u8 mEvtData[2][0x43FD0]; // _5D0E8, CEvtData, japan's are 0x650 shorter
#else
	u8 mEvtData[2][0x44620]; // _5D0E8, CEvtData
#endif
	EvtTimeState mTimeState;                  // _E5D28, _E5040 in japan
	u8 mCamera[0xE5DCC - 0xE5D38];            // _E5D38, _E5050 in japan, the game's CMotionControlCamera starts here
	f32 _E5DCC[3];                            // _E5DCC, its CCamera's mEye
	u8 _E5DD8[0xE5DDC - 0xE5DD8];             // _E5DD8
	f32 _E5DDC[3];                            // _E5DDC, mInterest, where sounds are heard from
	u8 _E5DE8[0xE5DEC - 0xE5DE8];             // _E5DE8
	f32 _E5DEC[3];                            // _E5DEC, mAngle
	u8 _E5DF8[0xE5FAC - 0xE5DF8];             // _E5DF8
	u8 mCameraActions[0x1E40];                // _E5FAC, _E52C4 in japan, the CCameraActions the camera picks from
	CInterfaceCameraAction* mCameraTable[12]; // _E7DEC, _E7104 in japan, CCameraActions's table, see CAMACTION
	u8 _E7E1C[0xE7E20 - 0xE7E1C];             // _E7E1C, CCameraActions's vtable
	CLight* _E7E20;                           // _E7E20, _E7138 in japan, head of the light list, LIGHT walks it
	XSound mSound;                            // _E7E24, _E713C in japan
#if defined(VERSION_JP)
	u8 _E8208[0xE8400 - 0xE8208 - 0x118]; // _E8208
#else
	u8 _E8208[0xE8400 - 0xE8208]; // _E8208
#endif
	Memcard mMemcard;             // _E8400, _E7600 in japan
	u8 _FEE24[0xFF200 - 0xFEE24]; // _FEE24
#if defined(VERSION_PAL)
	f32 mPlayTime; // _FF200, frames played
#else
	s32 mPlayTime; // _FF200, _FE400 in japan, frames played
#endif
	u8 _FF204[0xFF3A4 - 0xFF204]; // _FF204
	u8 mMovie[0xB8];              // _FF3A4, a CMovie
	u32 mMovieFlags;              // _FF45C, the Start flags
	f32 mMovieRect[4];            // _FF460, _FE660 in japan, where the movie draws

	virtual ~CGame(); // _08
};

#if defined(VERSION_JP)
STATIC_ASSERT(sizeof(CBase) == 0x3320);
STATIC_ASSERT(sizeof(CGame) == 0xFE670);
#elif defined(VERSION_PAL)
STATIC_ASSERT(sizeof(CBase) == 0x3340);
STATIC_ASSERT(sizeof(CGame) == 0xFF470);
#else
STATIC_ASSERT(sizeof(CBase) == 0x3338);
STATIC_ASSERT(sizeof(CGame) == 0xFF470);
#endif

extern "C" CGame lbl_8025C400;

extern "C" {
void fn_8000B394(CGame* game, s32 stage, s32 arg);
}

#endif
