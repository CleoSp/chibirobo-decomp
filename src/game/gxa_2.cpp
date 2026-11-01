#include "game/gxa_shared.h"

// what has been read ahead into aram, name from RTTI
class ARCacheInfo {
public:
	u32 _00;        // _00
	u32 _04;        // _04
	u8 _08[0x4000]; // _08
	// _4008 vtable

	virtual ~ARCacheInfo(); // _08
};

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
 * @note Address: 0x800195C8
 * @note Size: 0x38
 */
// the five mirrors' static destructor
extern "C" FbMirrorEx lbl_8035FA24[5];
extern "C" void* __dt__10FbMirrorExFv(void* mirror, s16 del);

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

extern "C" u32 lbl_80361E10[8];

// the two words the game keeps over a reset
extern "C" s32* lbl_8065B178;

// a run of frames, stepped round one at a time
struct FrameLoop {
	u8 _00[0x10]; // _00
	s32 mBase;    // _10
	u8 _14;       // _14
	u8 mCount;    // _15
	u8 mFrame;    // _16
};

// two values and whether they count
struct RangePair {
	u8 _00[0x4]; // _00
	f32 mA;      // _04
	f32 mB;      // _08
	s32 mValid;  // _0C, 1 when they do
};

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
