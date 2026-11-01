#include "game/gxb_shared.h"

extern "C" void fn_8001FCBC(s32 id);
extern "C" void fn_8001FCBC(s32 id);

// the screen overlays, CScrnColor and the wipes on it. names from RTTI, the layout is only
// what these functions show
struct CScrnColor {
	f32 mColor[4];  // _00
	f32 mTarget[4]; // _10
	f32 mSpeed[4];  // _20
	void* mVtable;  // _30
	s32 mActive;    // _34
	s32 mMode;      // _38
	f32 _3C;        // _3C
	f32 _40;        // _40
	f32 _44;        // _44
	f32 _48;        // _48
	f32 _4C;        // _4C
	f32 _50;        // _50
	f32 _54;        // _54
	f32 _58;        // _58
	f32 _5C;        // _5C
	f32 _60;        // _60
	f32 mAngle0;    // _64, the robo eyes turn these
	f32 mAngle1;    // _68
};

extern "C" void fn_80023030(CScrnColor* scrn, u32 frames);

// steps one channel towards its target, it stops there
static inline void StepChannel(f32& value, const f32& target, f32& speed, u32 frames)
{
	if (value != target) {
		value = speed * frames + value;
		if (speed > 0.0f && value >= target) {
			value = target;
			speed = 0.0f;
		}
		if (speed < 0.0f && value <= target) {
			value = target;
			speed = 0.0f;
		}
	}
}

// the same for a color without alpha, it flips a sign each step
struct ColorStep {
	CVec mColor;  // _00
	CVec mTarget; // _10
	CVec mSpeed;  // _20
	s32 mSign;    // _30
};

extern "C" int rand(void);

// CGs as the dust draws see it, game/cgs.h has all of it
class CGs {
public:
	u8 _00[0x290];        // _00
	void* mCurRenderMode; // _290
	void* mCurTevStage;   // _294
	void* mCurChannel;    // _298
	BOOL _29C;            // _29C
};

// one dust particle, name from RTTI. the dust kinds derive from it
class CPtclOne {
public:
	s32 mActive;     // _00
	CVec mPos;       // _04
	CVec mDir;       // _14
	CVec mVel;       // _24
	f32 mAngle;      // _34
	f32 mSpin;       // _38
	u32 mColor;      // _3C
	f32 mScale;      // _40
	f32 mScaleSpeed; // _44
	void* mTex;      // _48, a texture, or none for a plain one
	// _4C vtable

	virtual ~CPtclOne() { }          // _08
	virtual void Init();             // _0C
	virtual void Update(u32 frames); // _10
	virtual void Draw(CGs* gs);      // _14

	void Kill()
	{
		mActive = 0;
		mTex    = NULL;
	}
};

// a dust puff that drifts
class CPtclDust3 : public CPtclOne {
public:
	virtual void Update(u32 frames); // _10
	virtual void Draw(CGs* gs);      // _14
};

// a dust puff that falls
class CPtclDust2 : public CPtclOne {
public:
	virtual void Update(u32 frames); // _10
	virtual void Draw(CGs* gs);      // _14
};

// a pool of particles of one kind, laid out every mStride bytes
struct CPtclPool {
	u8* mBuffer; // _00
	s32 mStride; // _04
	s32 mCount;  // _08
	s32 _0C;     // _0C

	CPtclOne* Get(s32 i) { return (CPtclOne*)(mBuffer + i * mStride); }
};
extern "C" {
void fn_80199AB0(void* game, s32 a, s32 b);
void fn_8019B56C(void* renderMode, s32 a, s32 b, s32 c, s32 d);
void fn_8019B464(void* renderMode, s32 a, s32 b, s32 c);
}

#define GAME_GS ((CGs*)lbl_8025C400)

static inline void SetMoveTranslate(CXObj* obj, f32 x, f32 y, f32 z, s32 force)
{
	obj->mMoveTranslate.x         = x;
	obj->mMoveTranslate.y         = y;
	obj->mMoveTranslate.z         = z;
	obj->mMoveTranslate.mTarget.x = x;
	obj->mMoveTranslate.mTarget.y = y;
	obj->mMoveTranslate.mTarget.z = z;
	if (obj->mJObj) {
		obj->CJObj::SetTranslate(x, y, z, force);
	}
}

// the object every placed thing in the house is, only as far as these functions need it.
// game/obja.h has more of it
class XObj : public CXObjIntp {
public:
	u8 _228[0x80C - 0x228]; // _228
	s32 _80C;               // _80C
	u8 _810[0x8FC - 0x810]; // _810
	CJArchive mAnim;        // _8FC, the CJAnim with the animations
	s32 mAnimReading;       // _97C

	virtual void Init(u32 id);  // _90
	virtual void vf94();        // _94
	virtual void vf98();        // _98
	virtual void vf9C();        // _9C
	virtual void vfA0();        // _A0
	virtual void vfA4(s32 arg); // _A4
	virtual void vfA8();        // _A8
	virtual void vfAC();        // _AC
	virtual void vfB0(s32 arg); // _B0, draws it
};

// an object with a camera and lights of its own out of its archive, for the title and the
// demos. name from RTTI
class XObjDemo : public XObj {
public:
	s32 mState;        // _980, 0 off, 1 running, 2 until the screen fade is done
	BOOL mWatchAnim;   // _984
	CCamera mCamera;   // _988
	s32 mCameraAnim;   // _B1C
	s32 mLightCount;   // _B20
	CLight mLights[8]; // _B24
	s32 mLightAnim[8]; // _25E4
};
extern "C" {
BOOL fn_80091C80(XObj* obj);
void fn_800911C0(XObj* obj);
void fn_800928E8(XObj* obj);
}

#define GAME_CAMERA ((CCamera*)(lbl_8025C400 + 0x1B20))
#define GAME_LIGHT  ((CLight*)(lbl_8025C400 + 0x1E48))

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" u8 lbl_8025C400[];

// the boot logos, read from one tpl. each fades in, waits and fades out, the first one can
// be skipped with a button. name from RTTI, see Logo in game/cgame.cpp
struct Logo {
	s32 mState;    // _00
	CTpl mTpl;     // _04
	f32 mAlpha;    // _1F4
	f32 mAngle;    // _1F8, turns the first logo's shine
	f32 mTimer;    // _1FC
	void* mVtable; // _200

	void Reset()
	{
		mAlpha = 0.0f;
		mAngle = 0.0f;
		mTimer = 0.0f;
	}
	bool IsActive() const { return mState >= 1 && mState < 15; }
	TPLDescriptor* GetDesc(s32 i) { return &((TPLPalette*)mTpl.mBuffer)->descriptorArray[i]; }
};

#define GAME_PAD_TRIGGER (*(u32*)(lbl_8025C400 + 0x19C8))
extern "C" {
void fn_8019B648(void* renderMode, u8 enable);
u16 GXGetTexObjWidth(const GXTexObj* obj);
u16 GXGetTexObjHeight(const GXTexObj* obj);
void fn_8019B5D0(void* renderMode, u8 enable);
void fn_8019B504(void* renderMode, s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
void fn_8019AFA0(void* tevStage, s32 mode);
void fn_80197800(CGs* gs, void* tex, f32 x, f32 y, f32 z, f32 w, f32 h, f32 rx, f32 ry, f32 rz, u32 c0, u32 c1, u32 c2, u32 c3);
}

/**
 * @note Address: 0x80027A7C
 * @note Size: 0x1C
 */
// a jobj holder with a vector after it, class unknown
struct JObjVec {
	HSD_JObj* mJObj;     // _00
	u8 _04[0x1F4 - 0x4]; // _04
	f32 _1F4;            // _1F4
	f32 _1F8;            // _1F8
	f32 _1FC;            // _1FC
};
extern "C" {
void fn_800902A4(XObj* obj);
void fn_8008A38C(void* anim);
}

// animation ids from 1000 up are in the object's own anim archive once that is read, the
// rest are in its model archive. the last three digits pick the animation

static inline s32 GetAnimBank(XObj* obj, s32 id)
{
	bool own = id >= 1000;
	if (own) {
		own = obj->mAnim.mArchive != NULL;
	}
	return own ? id / 1000 : 0;
}

static inline HSD_AnimJoint* GetAnimJoint(CJArchive* archive, s32 id)
{
	if (archive->mAnimJoint != NULL) {
		return archive->mAnimJoint[id % 1000];
	}
	return NULL;
}

static inline HSD_MatAnimJoint* GetMatAnimJoint(CJArchive* archive, s32 id)
{
	if (archive->mMatAnimJoint != NULL) {
		return archive->mMatAnimJoint[id % 1000];
	}
	return NULL;
}

static inline HSD_ShapeAnimJoint* GetShapeAnimJoint(CJArchive* archive, s32 id)
{
	if (archive->mShapeAnimJoint != NULL) {
		return archive->mShapeAnimJoint[id % 1000];
	}
	return NULL;
}

// an object that keeps turning round its z axis, class unknown
class XObjSpin : public XObj {
public:
	f32 mAngle; // _980
	f32 mSpeed; // _984, per frame
};
extern "C" {
void fn_80090ED4(XObj* obj, u32 frames);
void fn_800929C4(XObj* obj);
}

// the screen fade's color, it is done once it is all zero
#define GAME_FADE_COLOR ((f32*)(lbl_8025C400 + 0x3E90))
extern "C" {
HSD_AObj* fn_801902C4(HSD_JObj* jobj);
HSD_AObj* fn_801905D4(HSD_JObj* jobj);
HSD_AObj* fn_801908F8(HSD_JObj* jobj);
}

// the first animation on the tree, joint ones before material and shape ones
static inline HSD_AObj* GetFirstAObj(HSD_JObj* jobj)
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

// the player's empty and forwarding handlers

struct Player;
extern "C" void fn_8002BD0C(Player* title);

/**
 * @note Address: 0x8002C3E8
 * @note Size: 0x4
 */
extern "C" void fn_8002C3E8(void* player)
{
}

/**
 * @note Address: 0x8002C3EC
 * @note Size: 0x4
 */
extern "C" void fn_8002C3EC(void* player)
{
}

// one sparkle on the title screen
struct TitleSpark {
	s32 _00; // _00
	s32 _04; // _04
	f32 mX;  // _08
	f32 mY;  // _0C
	f32 _10; // _10
	f32 _14; // _14
};

// one letter of a name that pops in, see StageName in game/cgame.cpp
struct StageNameChar {
	const char* mText; // _00, the letter, one or two bytes
	f32 mScale;        // _04, grows to 1 and shrinks away
	f32 mWait;         // _08, frames until it shrinks
	f32 mBob;          // _0C, the angle it bobs up and down by
	f32 mTilt;         // _10, the angle it rocks by
};

// a name, each letter on its own over a bubble
struct StageName {
	BOOL mActive;             // _00
	s32 mCount;               // _04
	u32 mColor;               // _08, the letters' color, the alpha comes from their scale
	StageNameChar mChars[20]; // _0C
};

// the title screen, see Title in game/gxa.cpp. only the fields these handlers touch,
// offsets are usa
struct Player {
	s32 _00;                   // _00
	s32 mState;                // _04
	s32 _08;                   // _08
	s32 _0C;                   // _0C
	f32 _10;                   // _10
	f32 _14;                   // _14
	f32 _18;                   // _18
	s32 _1C;                   // _1C
	s32 _20;                   // _20
	u8 _24[0x28 - 0x24];       // _24
	XObj mCursor;              // _28
	f32 _9A8;                  // _9A8
	f32 _9AC;                  // _9AC
	XObjDemo mDemo;            // _9B0
	u8 _2FB4[0x37A8 - 0x2FB4]; // _2FB4
	s32 _37A8;                 // _37A8
	f32 _37AC;                 // _37AC
	f32 _37B0;                 // _37B0
	f32 _37B4;                 // _37B4
	f32 _37B8;                 // _37B8
	f32 _37BC;                 // _37BC
	f32 _37C0;                 // _37C0
	f32 _37C4;                 // _37C4
	f32 _37C8;                 // _37C8
	f32 _37CC;                 // _37CC
	s32 _37D0;                 // _37D0
	s32 _37D4;                 // _37D4
	s32 _37D8;                 // _37D8
	u8 _37DC[0x37E0 - 0x37DC]; // _37DC
	u8 _37E0;                  // _37E0
	u8 _37E1;                  // _37E1
	u8 _37E2;                  // _37E2
	u8 _37E3;                  // _37E3
	f32 _37E4;                 // _37E4
	f32 _37E8;                 // _37E8
	s32 mPrevState;            // _37EC
	s32 mStep;                 // _37F0
	s32 _37F4;                 // _37F4
	s32 _37F8;                 // _37F8
	s32 _37FC;                 // _37FC
	u8 _3800[0x3804 - 0x3800]; // _3800
	s32 _3804;                 // _3804
	u8 _3808;                  // _3808
	u8 _3809;                  // _3809
	u8 _380A[0x380C - 0x380A]; // _380A
	s32 _380C;                 // _380C
	f32 _3810;                 // _3810
	f32 _3814;                 // _3814
	u8 _3818;                  // _3818, rumble on
	u8 _3819;                  // _3819, mono sound
	u8 _381A[0x3820 - 0x381A]; // _381A
	s32 _3820;                 // _3820, frames the rumble test has left
	TitleSpark mSparks[16];    // _3824
	u8 _39A4[0x39B4 - 0x39A4]; // _39A4
	StageName mName;           // _39B4, the title's name letters
	u8 _3B50[0x3C44 - 0x3B50]; // _3B50
	s32 _3C44;                 // _3C44

	void SetState(s32 state)
	{
		mPrevState = mState;
		mState     = state;
		mStep      = 0;
		_37CC      = -32.0f;
	}
};

// what the title shows of each of the three save files, class unknown
struct SaveInfo {
	u32 mUsed;    // _00, 1 when there is a file
	s32 _04;      // _04
	f32 _08;      // _08
	f32 _0C;      // _0C
	s32 _10;      // _10
	s32 _14;      // _14
	s32 _18;      // _18
	s32 _1C;      // _1C
	s32 mHours;   // _20
	s32 mMinutes; // _24
	s32 _28;      // _28
	s32 _2C;      // _2C
	u8 _30[0x10]; // _30
	s32 _40;      // _40
	u8 _44;       // _44
	u8 _45[0x3];  // _45
};
extern "C" SaveInfo lbl_80361E8C[3];
extern "C" bool fn_8003D7C4(SaveInfo* info);
extern "C" void fn_8003D7DC(void* obj);

// the card was taken out: back to the card check with the things reset
static inline void OnCardRemoved(Player* title)
{
	title->_37D8 = -3;
	title->SetState(9);
	for (s32 i = 0; i < 3; i++) {
		fn_8003D7DC(&lbl_80361E8C[i]);
	}
}

// moves a vector by another one passed by value
static inline void AddVec(CVec& vec, Vec add)
{
	PSVECAdd(&vec, &add, &vec);
}

// where the dust comes from, class unknown
struct PtclSource {
	CVec mPos; // _00
	CVec mDir; // _10
	CVec mVel; // _20
	BOOL mOn;  // _30
};

extern "C" void fn_80030CE8(Player* title);

extern "C" void fn_80089890(void* obj, s32 arg, s32 flag);

// the memory card's step and its last result, in CGame
#define CARD_STEP   (*(s32*)(lbl_8025C400 + 0xE8400))
#define CARD_RESULT (*(s32*)(lbl_8025C400 + 0xE8438))
#define CARD_FREE0  (*(s32*)(lbl_8025C400 + 0xFEE00))
#define CARD_FREE1  (*(s32*)(lbl_8025C400 + 0xFEE04))

extern "C" BOOL fn_8001E660(CSound* sound, s32 sfx);
#define GAME_9108 (*(s32*)(lbl_8025C400 + 0x9108))

extern "C" void fn_80068B98(void* obj);

// the pad state CPad points at, with the repeat bits game/ut.h leaves out
struct PadState {
	u8 mPort;       // _00
	u8 _01[0x13];   // _01
	u32 mTrigger;   // _14
	u8 _18[0x4];    // _18
	u32 mRepeat;    // _1C, the stick directions folded in
	u8 _20[0x2C];   // _20
	s32 mConnected; // _4C
};

extern "C" void fn_8008A1F4(void* obj, s32 a, s32 b, s32 flag);

static inline PadState* GetPadState(CPad& pad)
{
	return (PadState*)pad.mPad;
}

// moves the yes or no cursor with left and right
static inline void MoveYesNo(Player* title, CPad& pad)
{
	if (GetPadState(pad)->mRepeat & 0x40001) {
		if (title->_37FC > 0) {
			title->_37FC--;
			fn_8001E904(SOUND(), 0x87, 0xFF);
		}
	} else if (GetPadState(pad)->mRepeat & 0x80002) {
		if (title->_37FC < 1) {
			title->_37FC++;
			fn_8001E904(SOUND(), 0x87, 0xFF);
		}
	}
}

// CBase's 64 bit status flags
#define GAME_STATUS (*(u64*)(lbl_8025C400 + 0x1B10))
extern "C" {
void fn_8001E00C(CSound* sound, BOOL mono);
void HSD_PadRumbleOn(u8 no);
void HSD_PadRumbleOffN(u8 no);
void HSD_PadRumbleRemove(u8 no);
}
extern "C" {
const char* fn_8006DBF0(s32 id);
f32 fn_801968BC(void* game, const char* text, s32 a, f32 size);
void fn_80196610(void* game, u32 color, const char* text, f32 x, f32 y, f32 z, f32 w, f32 h, ...);
void fn_800652C0(u32 color0, u32 color1, f32 x, f32 y, f32 w, f32 h, f32 sx, f32 sy, f32 sz);
}

// how far the first animation on the cursor is through, from 0 to 1
static inline f32 GetAnimProgress(Player* title)
{
	HSD_AObj* aobj = GetFirstAObj(title->mCursor.mJObj);
	f32 frame      = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
	aobj           = GetFirstAObj(title->mCursor.mJObj);
	return frame / (aobj != NULL ? HSD_AObjGetEndFrame(aobj) : 0.0f);
}

class CFont;
extern "C" {
int strncmp(const char* a, const char* b, unsigned long n);
CTexObj* fn_8019E8F4(CFont* font, const char* str, s32* width);
}
extern "C" f32 lbl_8021195C[4];
extern "C" f32 lbl_8021196C[4];

// the font as the name draws see it, game/cfont.h has it
class CFont {
public:
	CFont() { _24 = NULL; }

	u8 _00[0x24]; // _00
	void* _24;    // _24
	u8 _28[0x14]; // _28
	CTexObj mTex; // _3C
	// _13C vtable

	virtual ~CFont() { } // _08 (weak)
};

#define GAME_TPL (*(CTpl*)(lbl_8025C400 + 0x3C80))

extern "C" void fn_80088E78(void* obj, s32 a, s32 flag);
extern "C" {
void fn_8019AFA0(void* tevStage, s32 mode);
void fn_80197800(CGs* gs, void* tex, f32 x, f32 y, f32 z, f32 w, f32 h, f32 rx, f32 ry, f32 rz, u32 c0, u32 c1, u32 c2, u32 c3);
void fn_80198258(CGs* gs, s32 type, f32 x, f32 y, f32 z, f32 w, f32 h, f32 rx, f32 ry, f32 rz, u32 color);
}

// a sprite in the world, textured or plain
static inline void DrawSprite(CGs* gs, void* tex, const Vec& pos, f32 w, f32 h, const Vec& rot, u32 color)
{
	fn_80197800(gs, tex, pos.x, pos.y, pos.z, w, h, rot.x, rot.y, rot.z, color, color, color, color);
}

static inline void DrawRect(CGs* gs, const Vec& pos, f32 w, f32 h, const Vec& rot, u32 color)
{
	fn_80198258(gs, 8, pos.x, pos.y, pos.z, w, h, rot.x, rot.y, rot.z, color);
}

extern "C" void fn_8001E630(CSound* sound, s32 track);
extern "C" void fn_80030CE8(Player* title);

// the card that was formatted went away again: back to the start
static inline void OnFormattedCardGone(Player* title)
{
	title->mPrevState = title->mState;
	title->mState     = 25;
	title->mStep      = 0;
	title->_00        = 11;
	fn_8001E904(SOUND(), 0x88, 0xFF);
	title->_37BC = -200.0f;
	title->_37CC = 0.0f;
	title->_37E8 = 0.0f;
	for (u32 i = 0; i < 20; i++) {
		title->mName.mChars[i].mWait = 0.0f;
	}
	title->_37A8 = 0;
	title->_3809 = 0;
	title->_3808 = 0;
}
extern "C" void fn_8002BD0C(Player* title);

extern "C" void fn_80089C90(void* obj, s32 a, s32 b, s32 flag);

// one save file's header as the card code read it, class unknown
struct CardFile {
	s32 mUsed;           // _00
	s32 _04;             // _04
	u8 _08[0x2C - 0x8];  // _08
	u32 mSaveTime;       // _2C, in seconds
	u8 _30[0x7C - 0x30]; // _30
	s32 _7C;             // _7C
	s32 _80;             // _80
	u8 _84[0x8C - 0x84]; // _84
	s32 _8C;             // _8C
	s32 _90;             // _90
	s32 _94;             // _94
	f32 _98;             // _98
	f32 _9C;             // _9C
	s32 _A0;             // _A0
	s32 _A4;             // _A4
	s32 _A8;             // _A8
	u8 _AC[0xC8 - 0xAC]; // _AC
	s32 mPlayTime;       // _C8, in frames
};

// the memory card state in CGame, class unknown
struct CardState {
	s32 mStep;             // _00
	u8 _04[0x16200 - 0x4]; // _04
	CardFile mFiles[3];    // _16200
};
#define CARD ((CardState*)(lbl_8025C400 + 0xE8400))
extern "C" void fn_80023030(CScrnColor* scrn, u32 frames);

// the os time functions, game/ has no header for them
typedef s64 OSTime;
struct OSCalendarTime {
	s32 sec;  // _00
	s32 min;  // _04
	s32 hour; // _08
	s32 mday; // _0C
	s32 mon;  // _10
	s32 year; // _14
	s32 wday; // _18
	s32 yday; // _1C
	s32 msec; // _20
	s32 usec; // _24
};
extern "C" {
void OSTicksToCalendarTime(OSTime ticks, OSCalendarTime* td);
OSTime OSCalendarTimeToTicks(OSCalendarTime* td);
s32 fn_80035184(OSTime a, OSTime b, OSTime c);
BOOL fn_80086F24(CardFile* file);
}

#define OS_TIMER_CLOCK (*(u32*)0x800000F8 / 4)
