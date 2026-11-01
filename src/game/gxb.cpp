#include "game/cjobjset.h"
#include "game/cfile.h"
#include "game/ctpl.h"
#include "Version.h"
extern "C" {
#include <sysdolphin/archive.h>
#include <sysdolphin/mobj.h>
}
#include "game/clight.h"
#include <math.h>
extern "C" {
#include <dolphin/mtx/vec.h>
}

// game/cobj.h's classes, with the signatures of CXObj's move setters that the house objects
// call filled in
// class names from RTTI, ReadAsync/ReadCheck/Init from halt strings.
// offsets are usa, jp's CJArchive is 8 bytes shorter so CXObj fields after mArchive shift

// HSD archive plus the CFile it's read through, refcounted between CJArchives
class CObjArchive : public HSD_Archive {
public:
	// _00 HSD_Archive
	CFile mFile;   // _44
	s32 mRefCount; // _130
	// _134 vtable

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
	virtual void SetMoveTranslate(const Vec& v, s32 force);                                                           // _64, 0x8002657C
	virtual void SetMoveTranslate(f32 x, f32 y, f32 z, s32 force);                                                    // _68, 0x800263EC
	virtual void SetMoveScale(f32 scale, s32 force);                                                                  // _6C, 0x80027B70
	virtual void SetMoveRotate(const Vec& v, s32 force);                                                              // _70, 0x80026938
	virtual void SetMoveRotate(f32 x, f32 y, f32 z, s32 force);                                                       // _74, 0x800266F8
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

// turns one joint of a CXObj towards an angle, or towards another CXObj
class CJointRot {
public:
	s16 mMode;        // _00, 0 idle, 2 runs once, other values flip sign when the turn ends
	u16 mAxis;        // _02
	f32 mAngle;       // _04
	f32 mTarget;      // _08
	f32 mSpeed;       // _0C
	CXObj* mObj;      // _10
	HSD_JObj* mJoint; // _14
	CXObj* mLookAt;   // _18
	// _1C vtable

	virtual ~CJointRot() { } // _08 (weak)

	bool Update(u32 frames);
	void Init(CXObj* obj, s32 depth, s32* path, char axis);
};
typedef char CJointRot_size_check[sizeof(CJointRot) == 0x20 ? 1 : -1];

// game functions 0x8001F8D8-0x80031004: the end of the sound object, a frame fade, the dust
// particles and the house objects. names of most are lost, so they are extern "C"
// stand-ins under their dol address

struct CSound;
extern "C" {
void HSD_SynthSFXBankClear(s32 bank);
s32 HSD_SynthSFXNewBank(u32 size);
void HSD_JObjSetDPtclCallback(void (*cb)(s32 id));
void InitAudio(CSound* sound, u32 aramSize, u32 voices, u32 streams);
BOOL fn_8001E904(CSound* sound, s32 sfx, u8 vol);
}

// 16 slots of names waiting to load
struct SfxNames {
	u8 _00[0x10]; // _00
	struct Slot {
		const char* _00; // _00, name
		s32 _04;         // _04
		s32 _08;         // _08, load result
		u8 _0C;          // _0C, loaded flag
	} _10[16];           // _10, _2E0 in CSound
	s32 _110;            // _110, pending count, _3E0 in CSound

	void Clear()
	{
		for (u32 i = 0; i < 16; i++) {
			Slot* slot = &_10[i];
			slot->_00  = NULL;
			slot->_08  = -1;
			slot->_0C  = 0;
		}
		_110 = 0;
	}
};

// the sound object in CGame at +0xE7E24, as game/sound.cpp has it
struct CSound {
	u8 _00[0x4];            // _00
	s32 mLoadStep;          // _04
	CFile mFile;            // _08
	s32 _F4;                // _F4, the resident sfx bank
	u8 _F8;                 // _F8, set once the resident bank is in
	u8 _F9[0x100 - 0xF9];   // _F9
	s32 _100;               // _100, the stage sfx bank
	u8 _104[0x2D0 - 0x104]; // _104
	SfxNames mNames;        // _2D0

	// drops every name waiting to load and empties the stage bank
	void ClearStageBank()
	{
		mNames.Clear();
		HSD_SynthSFXBankClear(_100);
	}
};

extern "C" u8 lbl_8025C400[];
#define SOUND() ((CSound*)(lbl_8025C400 + 0xE7E24))

/**
 * @note Address: 0x8001F8D8
 * @note Size: 0xA4
 */
// drops every name waiting to load and empties the stage bank
extern "C" void fn_8001F8D8(CSound* sound)
{
	sound->ClearStageBank();
}

extern "C" void fn_8001FCBC(s32 id);

/**
 * @note Address: 0x8001FC54
 * @note Size: 0x68
 */
extern "C" void fn_8001FC54(CSound* sound)
{
	InitAudio(sound, 0x700000, 0x60, 0x20);
	sound->_F4       = HSD_SynthSFXNewBank(0x480000);
	sound->_100      = HSD_SynthSFXNewBank(0x280000);
	sound->mLoadStep = 0;
	sound->_F8       = 0;
	HSD_JObjSetDPtclCallback(fn_8001FCBC);
}

/**
 * @note Address: 0x8001FCBC
 * @note Size: 0x38
 */
// the particle callback, plays the sfx a particle asks for
extern "C" void fn_8001FCBC(s32 id)
{
	fn_8001E904(SOUND(), id, 0xFF);
}

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

/**
 * @note Address: 0x80021474
 * @note Size: 0x8C
 */
// the robo eyes wipe's update, it turns its two rings while it runs
extern "C" void fn_80021474(CScrnColor* scrn, u32 frames)
{
	fn_80023030(scrn, frames);
	if (scrn->mActive) {
		scrn->mAngle0 += 0.02617994f * frames;
		scrn->mAngle1 += 0.10471976f * frames;
	}
}

/**
 * @note Address: 0x80021500
 * @note Size: 0x70
 */
extern "C" void fn_80021500(CScrnColor* scrn)
{
	scrn->mActive = 0;
	scrn->_3C = scrn->_40 = 0.0f;
	scrn->_44 = scrn->_48 = 0.0f;
	scrn->_4C = scrn->_50 = 0.0f;
	scrn->_54 = scrn->_58 = 0.0f;
	scrn->mColor[0]       = 0.0f;
	scrn->mColor[1]       = 0.0f;
	scrn->mColor[2]       = 0.0f;
	scrn->mColor[3]       = 0.0f;
	scrn->mTarget[0]      = 0.0f;
	scrn->mTarget[1]      = 0.0f;
	scrn->mTarget[2]      = 0.0f;
	scrn->mTarget[3]      = 0.0f;
	scrn->mSpeed[0]       = 0.0f;
	scrn->mSpeed[1]       = 0.0f;
	scrn->mSpeed[2]       = 0.0f;
	scrn->mSpeed[3]       = 0.0f;
	scrn->_5C = scrn->_60 = 0.0f;
	scrn->mAngle0         = 0.0f;
	scrn->mAngle1         = 0.0f;
}

/**
 * @note Address: 0x800238A4
 * @note Size: 0x68
 */
extern "C" void fn_800238A4(CScrnColor* scrn)
{
	scrn->mActive = 0;
	scrn->_3C = scrn->_40 = 0.0f;
	scrn->_44 = scrn->_48 = 0.0f;
	scrn->_4C = scrn->_50 = 0.0f;
	scrn->_54 = scrn->_58 = 0.0f;
	scrn->mColor[0]       = 0.0f;
	scrn->mColor[1]       = 0.0f;
	scrn->mColor[2]       = 0.0f;
	scrn->mColor[3]       = 0.0f;
	scrn->mTarget[0]      = 0.0f;
	scrn->mTarget[1]      = 0.0f;
	scrn->mTarget[2]      = 0.0f;
	scrn->mTarget[3]      = 0.0f;
	scrn->mSpeed[0]       = 0.0f;
	scrn->mSpeed[1]       = 0.0f;
	scrn->mSpeed[2]       = 0.0f;
	scrn->mSpeed[3]       = 0.0f;
	scrn->_5C = scrn->_60 = 0.0f;
}

/**
 * @note Address: 0x80023D74
 * @note Size: 0x16C
 */
// NONMATCHING
// fades to a color over the given seconds, at once for zero
extern "C" void fn_80023D74(CScrnColor* scrn, f32 time, u8 r, u8 g, u8 b, u8 a)
{
	scrn->mTarget[0] = r;
	scrn->mTarget[1] = g;
	scrn->mTarget[2] = b;
	scrn->mTarget[3] = a;
	if (time == 0.0f) {
		scrn->mColor[0] = r;
		scrn->mColor[1] = g;
		scrn->mColor[2] = b;
		scrn->mColor[3] = a;
		scrn->mSpeed[0] = 0.0f;
		scrn->mSpeed[1] = 0.0f;
		scrn->mSpeed[2] = 0.0f;
		scrn->mSpeed[3] = 0.0f;
	} else {
		f32 frames      = 60.0f * time;
		scrn->mSpeed[0] = (r - scrn->mColor[0]) / frames;
		scrn->mSpeed[1] = (g - scrn->mColor[1]) / frames;
		scrn->mSpeed[2] = (b - scrn->mColor[2]) / frames;
		scrn->mSpeed[3] = (a - scrn->mColor[3]) / frames;
	}
}

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

/**
 * @note Address: 0x80023B38
 * @note Size: 0x23C
 */
extern "C" void fn_80023B38(CScrnColor* scrn, u32 frames)
{
	StepChannel(scrn->mColor[0], scrn->mTarget[0], scrn->mSpeed[0], frames);
	StepChannel(scrn->mColor[1], scrn->mTarget[1], scrn->mSpeed[1], frames);
	StepChannel(scrn->mColor[2], scrn->mTarget[2], scrn->mSpeed[2], frames);
	StepChannel(scrn->mColor[3], scrn->mTarget[3], scrn->mSpeed[3], frames);
}

// the same for a color without alpha, it flips a sign each step
struct ColorStep {
	CVec mColor;  // _00
	CVec mTarget; // _10
	CVec mSpeed;  // _20
	s32 mSign;    // _30
};

/**
 * @note Address: 0x800240EC
 * @note Size: 0x1BC
 */
extern "C" void fn_800240EC(ColorStep* step, u32 frames)
{
	StepChannel(step->mColor.x, step->mTarget.x, step->mSpeed.x, frames);
	StepChannel(step->mColor.y, step->mTarget.y, step->mSpeed.y, frames);
	StepChannel(step->mColor.z, step->mTarget.z, step->mSpeed.z, frames);
	step->mSign = -step->mSign;
}

/**
 * @note Address: 0x800242A8
 * @note Size: 0x184
 */
// starts a step from one color to another over the given seconds, at once for zero. the
// channels are kept positive
extern "C" void fn_800242A8(ColorStep* step, f32 r0, f32 g0, f32 b0, f32 r1, f32 g1, f32 b1, f32 time)
{
	step->mColor.x  = r0;
	step->mColor.y  = g0;
	step->mColor.z  = b0;
	step->mColor.x  = Abs(step->mColor.x);
	step->mColor.y  = Abs(step->mColor.y);
	step->mColor.z  = Abs(step->mColor.z);
	step->mTarget.x = r1;
	step->mTarget.y = g1;
	step->mTarget.z = b1;
	step->mTarget.x = Abs(step->mTarget.x);
	step->mTarget.y = Abs(step->mTarget.y);
	step->mTarget.z = Abs(step->mTarget.z);
	if (time == 0.0f) {
		step->mColor.x = step->mTarget.x;
		step->mColor.y = step->mTarget.y;
		step->mColor.z = step->mTarget.z;
		step->mSpeed.x = 0.0f;
		step->mSpeed.y = 0.0f;
		step->mSpeed.z = 0.0f;
	} else {
		step->mSpeed = (step->mTarget - step->mColor) / (60.0f * time);
	}
	step->mSign = 1;
}

/**
 * @note Address: 0x8002442C
 * @note Size: 0x74
 */
CVec CVec::operator/(f32 s) const
{
	CVec quotient = *this;
	quotient.x /= s;
	quotient.y /= s;
	quotient.z /= s;
	return quotient;
}

/**
 * @note Address: 0x800244A0
 * @note Size: 0x84
 */
CVec CVec::operator-(Vec v) const
{
	CVec difference = *this;
	PSVECSubtract(&difference, &v, &difference);
	return difference;
}

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

/**
 * @note Address: 0x800245F0
 * @note Size: 0x10
 */
void CPtclOne::Init()
{
	Kill();
}

/**
 * @note Address: 0x80024CA0
 * @note Size: 0xE0
 */
extern "C" void fn_80024CA0(CPtclPool* pool)
{
	fn_80199AB0(lbl_8025C400, 0, 0);
	fn_8019B56C(GAME_GS->mCurRenderMode, 1, 4, 5, 15);
	fn_8019B464(GAME_GS->mCurRenderMode, 1, 3, 0);
	GAME_GS->_29C = TRUE;
	for (s32 i = 0; i < pool->mCount; i++) {
		CPtclOne* ptcl = pool->Get(i);
		if (ptcl->mActive) {
			ptcl->Draw(GAME_GS);
		}
	}
	GAME_GS->_29C = FALSE;
}

/**
 * @note Address: 0x80024D80
 * @note Size: 0x4
 */
void CPtclOne::Draw(CGs* gs)
{
}

/**
 * @note Address: 0x80024D84
 * @note Size: 0x84
 */
extern "C" void fn_80024D84(CPtclPool* pool, u32 frames)
{
	for (s32 i = 0; i < pool->mCount; i++) {
		CPtclOne* ptcl = pool->Get(i);
		if (ptcl->mActive) {
			ptcl->Update(frames);
		}
	}
}

/**
 * @note Address: 0x80024E08
 * @note Size: 0x4
 */
void CPtclOne::Update(u32 frames)
{
}

/**
 * @note Address: 0x80024E0C
 * @note Size: 0x44
 */
extern "C" void fn_80024E0C(CPtclPool* pool)
{
	for (s32 i = 0; i < pool->mCount; i++) {
		CPtclOne* ptcl = pool->Get(i);
		if (ptcl->mActive) {
			ptcl->Kill();
		}
	}
}

/**
 * @note Address: 0x80024FAC
 * @note Size: 0x84
 */
extern "C" void fn_80024FAC(CPtclPool* pool, u8* buffer, s32 stride, s32 count)
{
	pool->mBuffer = buffer;
	pool->mStride = stride;
	pool->mCount  = count;
	pool->_0C     = 0;
	for (s32 i = 0; i < count; i++) {
		pool->Get(i)->Init();
	}
}

// CXObj's overrides of the CJObj setters. each puts the value on the stepped transform with
// nowhere left to go, then on the jobj when there is one. the scale's y goes on negated

/**
 * @note Address: 0x80025150
 * @note Size: 0xD8
 */
extern "C" void fn_80025150(CXObj* obj, f32 y, s32 force)
{
	y                         = -y;
	obj->mMoveScale.y         = y;
	obj->mMoveScale.mTarget.y = y;
	if (obj->mJObj) {
		obj->CJObj::SetScaleY(y, force);
	}
}

/**
 * @note Address: 0x80025228
 * @note Size: 0x190
 */
extern "C" void fn_80025228(CXObj* obj, f32 scale, s32 force)
{
	f32 x = scale;
	f32 y = -scale;
	f32 z = scale;

	obj->mMoveScale.x         = x;
	obj->mMoveScale.y         = y;
	obj->mMoveScale.z         = z;
	obj->mMoveScale.mTarget.x = x;
	obj->mMoveScale.mTarget.y = y;
	obj->mMoveScale.mTarget.z = z;
	if (obj->mJObj) {
		if (force == 0) {
			Vec v     = { x, y, z };
			bool same = !(obj->mScale != v);
			if (same) {
				return;
			}
		}
		obj->mScale.x = x;
		obj->mScale.y = y;
		obj->mScale.z = z;
		HSD_JObjSetScale(obj->mJObj, &obj->mScale);
	}
}

/**
 * @note Address: 0x800253B8
 * @note Size: 0x190
 */
extern "C" void fn_800253B8(CXObj* obj, f32 x, f32 y, f32 z, s32 force)
{
	y = -y;

	obj->mMoveScale.x         = x;
	obj->mMoveScale.y         = y;
	obj->mMoveScale.z         = z;
	obj->mMoveScale.mTarget.x = x;
	obj->mMoveScale.mTarget.y = y;
	obj->mMoveScale.mTarget.z = z;
	if (obj->mJObj) {
		if (force == 0) {
			Vec v     = { x, y, z };
			bool same = !(obj->mScale != v);
			if (same) {
				return;
			}
		}
		obj->mScale.x = x;
		obj->mScale.y = y;
		obj->mScale.z = z;
		HSD_JObjSetScale(obj->mJObj, &obj->mScale);
	}
}

/**
 * @note Address: 0x80025548
 * @note Size: 0x19C
 */
extern "C" void fn_80025548(CXObj* obj, const Vec& scale, s32 force)
{
	f32 x = scale.x;
	f32 y = -scale.y;
	f32 z = scale.z;

	obj->mMoveScale.x         = x;
	obj->mMoveScale.y         = y;
	obj->mMoveScale.z         = z;
	obj->mMoveScale.mTarget.x = x;
	obj->mMoveScale.mTarget.y = y;
	obj->mMoveScale.mTarget.z = z;
	if (obj->mJObj) {
		if (force == 0) {
			Vec v     = { x, y, z };
			bool same = !(obj->mScale != v);
			if (same) {
				return;
			}
		}
		obj->mScale.x = x;
		obj->mScale.y = y;
		obj->mScale.z = z;
		HSD_JObjSetScale(obj->mJObj, &obj->mScale);
	}
}

/**
 * @note Address: 0x80025AA4
 * @note Size: 0xD8
 */
extern "C" void fn_80025AA4(CXObj* obj, f32 x, s32 force)
{
	obj->mMoveTranslate.x         = x;
	obj->mMoveTranslate.mTarget.x = x;
	if (obj->mJObj) {
		obj->CJObj::SetTranslateX(x, force);
	}
}

/**
 * @note Address: 0x80025B7C
 * @note Size: 0xD8
 */
extern "C" void fn_80025B7C(CXObj* obj, f32 y, s32 force)
{
	obj->mMoveTranslate.y         = y;
	obj->mMoveTranslate.mTarget.y = y;
	if (obj->mJObj) {
		obj->CJObj::SetTranslateY(y, force);
	}
}

/**
 * @note Address: 0x80025C54
 * @note Size: 0xD8
 */
extern "C" void fn_80025C54(CXObj* obj, f32 z, s32 force)
{
	obj->mMoveTranslate.z         = z;
	obj->mMoveTranslate.mTarget.z = z;
	if (obj->mJObj) {
		obj->CJObj::SetTranslateZ(z, force);
	}
}

/**
 * @note Address: 0x80025D2C
 * @note Size: 0xD8
 */
extern "C" void fn_80025D2C(CXObj* obj, f32 x, s32 force)
{
	obj->mMoveScale.x         = x;
	obj->mMoveScale.mTarget.x = x;
	if (obj->mJObj) {
		obj->CJObj::SetScaleX(x, force);
	}
}

/**
 * @note Address: 0x80025E04
 * @note Size: 0xD8
 */
extern "C" void fn_80025E04(CXObj* obj, f32 z, s32 force)
{
	obj->mMoveScale.z         = z;
	obj->mMoveScale.mTarget.z = z;
	if (obj->mJObj) {
		obj->CJObj::SetScaleZ(z, force);
	}
}

/**
 * @note Address: 0x80025EDC
 * @note Size: 0x1B0
 */
extern "C" void fn_80025EDC(CXObj* obj, f32 x, s32 force)
{
	obj->mMoveRotate.x         = x;
	obj->mMoveRotate.mTarget.x = x;
	if (obj->mJObj) {
		obj->CJObj::SetRotationX(x, force);
	}
}

/**
 * @note Address: 0x8002608C
 * @note Size: 0x1B0
 */
extern "C" void fn_8002608C(CXObj* obj, f32 y, s32 force)
{
	obj->mMoveRotate.y         = y;
	obj->mMoveRotate.mTarget.y = y;
	if (obj->mJObj) {
		obj->CJObj::SetRotationY(y, force);
	}
}

/**
 * @note Address: 0x8002623C
 * @note Size: 0x1B0
 */
extern "C" void fn_8002623C(CXObj* obj, f32 z, s32 force)
{
	obj->mMoveRotate.z         = z;
	obj->mMoveRotate.mTarget.z = z;
	if (obj->mJObj) {
		obj->CJObj::SetRotationZ(z, force);
	}
}

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

/**
 * @note Address: 0x800263EC
 * @note Size: 0x190
 */
extern "C" void fn_800263EC(CXObj* obj, f32 x, f32 y, f32 z, s32 force)
{
	obj->mMoveTranslate.x         = x;
	obj->mMoveTranslate.y         = y;
	obj->mMoveTranslate.z         = z;
	obj->mMoveTranslate.mTarget.x = x;
	obj->mMoveTranslate.mTarget.y = y;
	obj->mMoveTranslate.mTarget.z = z;
	if (obj->mJObj) {
		if (force == 0) {
			Vec v     = { x, y, z };
			bool same = !(obj->mTranslate != v);
			if (same) {
				return;
			}
		}
		obj->mTranslate.x = x;
		obj->mTranslate.y = y;
		obj->mTranslate.z = z;
		HSD_JObjSetTranslate(obj->mJObj, &obj->mTranslate);
	}
}

/**
 * @note Address: 0x8002657C
 * @note Size: 0x17C
 */
extern "C" void fn_8002657C(CXObj* obj, const Vec& v, s32 force)
{
	obj->mMoveTranslate.Set(v);
	obj->mMoveTranslate.mTarget.Set(v);
	if (obj->mJObj) {
		obj->CJObj::SetTranslate(v, force);
	}
}

/**
 * @note Address: 0x800266F8
 * @note Size: 0x240
 */
extern "C" void fn_800266F8(CXObj* obj, f32 x, f32 y, f32 z, s32 force)
{
	obj->mMoveRotate.x         = x;
	obj->mMoveRotate.y         = y;
	obj->mMoveRotate.z         = z;
	obj->mMoveRotate.mTarget.x = x;
	obj->mMoveRotate.mTarget.y = y;
	obj->mMoveRotate.mTarget.z = z;
	if (obj->mJObj) {
		if (force == 0) {
			Vec v = { x, y, z };
			// tests the cached translation, as CJObj's does
			bool same = !(obj->mTranslate != v);
			if (same) {
				return;
			}
		}
		obj->mRotate.x = x;
		obj->mRotate.y = y;
		obj->mRotate.z = z;
		obj->UpdateRotation();
	}
}

/**
 * @note Address: 0x80026938
 * @note Size: 0x22C
 */
extern "C" void fn_80026938(CXObj* obj, const Vec& v, s32 force)
{
	obj->mMoveRotate.Set(v);
	obj->mMoveRotate.mTarget.Set(v);
	if (obj->mJObj) {
		obj->CJObj::SetRotation(v, force);
	}
}

// the same setters again on an object that keeps its scale's y as it is

/**
 * @note Address: 0x80027A98
 * @note Size: 0xD8
 */
extern "C" void fn_80027A98(CXObj* obj, f32 y, s32 force)
{
	obj->mMoveScale.y         = y;
	obj->mMoveScale.mTarget.y = y;
	if (obj->mJObj) {
		obj->CJObj::SetScaleY(y, force);
	}
}

/**
 * @note Address: 0x80027B70
 * @note Size: 0x18C
 */
extern "C" void fn_80027B70(CXObj* obj, f32 scale, s32 force)
{
	f32 x = scale;
	f32 y = scale;
	f32 z = scale;

	obj->mMoveScale.x         = x;
	obj->mMoveScale.y         = y;
	obj->mMoveScale.z         = z;
	obj->mMoveScale.mTarget.x = x;
	obj->mMoveScale.mTarget.y = y;
	obj->mMoveScale.mTarget.z = z;
	if (obj->mJObj) {
		if (force == 0) {
			Vec v     = { x, y, z };
			bool same = !(obj->mScale != v);
			if (same) {
				return;
			}
		}
		obj->mScale.x = x;
		obj->mScale.y = y;
		obj->mScale.z = z;
		HSD_JObjSetScale(obj->mJObj, &obj->mScale);
	}
}

/**
 * @note Address: 0x80027CFC
 * @note Size: 0x18C
 */
extern "C" void fn_80027CFC(CXObj* obj, f32 x, f32 y, f32 z, s32 force)
{
	obj->mMoveScale.x         = x;
	obj->mMoveScale.y         = y;
	obj->mMoveScale.z         = z;
	obj->mMoveScale.mTarget.x = x;
	obj->mMoveScale.mTarget.y = y;
	obj->mMoveScale.mTarget.z = z;
	if (obj->mJObj) {
		if (force == 0) {
			Vec v     = { x, y, z };
			bool same = !(obj->mScale != v);
			if (same) {
				return;
			}
		}
		obj->mScale.x = x;
		obj->mScale.y = y;
		obj->mScale.z = z;
		HSD_JObjSetScale(obj->mJObj, &obj->mScale);
	}
}

/**
 * @note Address: 0x80027E88
 * @note Size: 0x178
 */
extern "C" void fn_80027E88(CXObj* obj, const Vec& scale, s32 force)
{
	obj->mMoveScale.Set(scale);
	obj->mMoveScale.mTarget.Set(scale);
	if (obj->mJObj) {
		obj->CJObj::SetScale(scale, force);
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

/**
 * @note Address: 0x80027528
 * @note Size: 0x554
 */
// NONMATCHING: sprintf's format and buffer are set up in another order
// Logo::Update, reads the logos in, then fades each of them in and out
extern "C" void fn_80027528(Logo* logo, u32 frames)
{
	char path[64];
	switch (logo->mState) {
	case 1:
		logo->mState = 2;
	case 2:
		sprintf(path, "tpl/logo%s.tpl", "_us");
		if (!logo->mTpl.ReadEx(path, 0, 0)) {
			break;
		}
		logo->mTpl.CloseHandle();
		if ((s32)((TPLPalette*)logo->mTpl.mBuffer)->numDescriptors > 3) {
			logo->mState = 3;
		} else {
			logo->mState = 6;
		}
		logo->Reset();
	case 3:
		if (logo->mAlpha < 1.0f && (logo->mAlpha += 0.033333335f * frames) > 1.0f) {
			logo->mAlpha = 1.0f;
		}
		if (logo->mAlpha != 1.0f) {
			break;
		}
		logo->mState = 4;
	case 4:
		logo->mTimer += frames;
		if (logo->mTimer >= 90.0f) {
			if ((logo->mAngle += 0.05235988f * frames) >= 3.1415927f) {
				logo->mAngle = 0.0f;
			}
			BOOL done = logo->mTimer >= 3600.0f ? TRUE : GAME_PAD_TRIGGER != 0;
			if (!done) {
				break;
			}
		} else {
			break;
		}
		logo->mState = 5;
	case 5:
		if (logo->mAlpha > 0.0f && (logo->mAlpha -= 0.033333335f * frames) < 0.0f) {
			logo->mAlpha = 0.0f;
		}
		if (logo->mAlpha == 0.0f) {
			logo->mState = 6;
			logo->Reset();
		}
		break;
	case 6:
		if (logo->mAlpha < 1.0f && (logo->mAlpha += 0.033333335f * frames) > 1.0f) {
			logo->mAlpha = 1.0f;
		}
		if (logo->mAlpha != 1.0f) {
			break;
		}
		logo->mState = 7;
	case 7:
		if ((logo->mTimer += frames) >= 60.0f) {
			logo->mState = 8;
		} else {
			break;
		}
	case 8:
		if (logo->mAlpha > 0.0f && (logo->mAlpha -= 0.033333335f * frames) < 0.0f) {
			logo->mAlpha = 0.0f;
		}
		if (logo->mAlpha == 0.0f) {
			logo->mState = 9;
			logo->Reset();
		}
		break;
	case 9:
		if (logo->mAlpha < 1.0f && (logo->mAlpha += 0.033333335f * frames) > 1.0f) {
			logo->mAlpha = 1.0f;
		}
		if (logo->mAlpha != 1.0f) {
			break;
		}
		logo->mState = 10;
	case 10:
		if ((logo->mTimer += frames) >= 60.0f) {
			logo->mState = 11;
		} else {
			break;
		}
	case 11:
		if (logo->mAlpha > 0.0f && (logo->mAlpha -= 0.033333335f * frames) < 0.0f) {
			logo->mAlpha = 0.0f;
		}
		if (logo->mAlpha == 0.0f) {
			logo->mState = 12;
			logo->Reset();
		}
		break;
	case 12:
		if (logo->mAlpha < 1.0f && (logo->mAlpha += 0.033333335f * frames) > 1.0f) {
			logo->mAlpha = 1.0f;
		}
		if (logo->mAlpha != 1.0f) {
			break;
		}
		logo->mState = 13;
	case 13:
		if ((logo->mTimer += frames) >= 60.0f) {
			logo->mState = 14;
		} else {
			break;
		}
	case 14:
		if (logo->mAlpha > 0.0f && (logo->mAlpha -= 0.033333335f * frames) < 0.0f) {
			logo->mAlpha = 0.0f;
		}
		if (logo->mAlpha == 0.0f) {
			logo->mState = 15;
			logo->Reset();
			logo->mTpl.Close();
		}
		break;
	case 15:
		break;
	}
}

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
 * @note Address: 0x80026CB0
 * @note Size: 0x878
 */
// Logo::Draw, the current logo centred on the screen at its alpha. the first one has a shine
// that comes and goes with the angle
extern "C" void fn_80026CB0(Logo* logo)
{
	if (logo->IsActive() && logo->mAlpha > 0.0f) {
		fn_80199AB0(lbl_8025C400, 0, 0);
		fn_8019B648(GAME_GS->mCurRenderMode, 1);
		fn_8019B5D0(GAME_GS->mCurRenderMode, 0);
		fn_8019B504(GAME_GS->mCurRenderMode, 4, 0, 0, 3, 255);
		fn_8019B56C(GAME_GS->mCurRenderMode, 1, 4, 5, 15);
		fn_8019B464(GAME_GS->mCurRenderMode, 0, 7, 0);
		fn_8019AFA0(GAME_GS->mCurTevStage, 0);
		CTexObj tex;
		// declared up front, it decides their registers
		f32 h2;
		f32 shineH;
		f32 y;
		f32 shineW;
		f32 h1;
		f32 shine;
		if (logo->mState == 3 || logo->mState == 4 || logo->mState == 5) {
			tex       = logo->mTpl.mTex.Init(logo->GetDesc(3), 0);
			u16 w     = GXGetTexObjWidth(&tex);
			h1        = GXGetTexObjHeight(&tex);
			u32 color = (u32)(255.0f * logo->mAlpha) | 0xFFFFFF00;
			fn_80197800(GAME_GS, &tex, 320.0f, 224.0f, 0.0f, w, h1, 0.0f, 0.0f, 0.0f, color, color, color, color);

			tex    = logo->mTpl.mTex.Init(logo->GetDesc(4), 0);
			shineW = GXGetTexObjWidth(&tex);
			shineH = GXGetTexObjHeight(&tex);
			y      = shineH / 2.0f + 359.0f;
			f32 s  = sin(logo->mAngle);
			shine  = s;
			if (s < 0.0f) {
				shine = 0.0f;
			}
			color = (u32)(255.0f * logo->mAlpha * shine) | 0xFFFFFF00;
			fn_80197800(GAME_GS, &tex, 320.0f, y, 0.0f, shineW, shineH, 0.0f, 0.0f, 0.0f, color, color, color, color);
		}
		if (logo->mState == 6 || logo->mState == 7 || logo->mState == 8) {
			tex       = logo->mTpl.mTex.Init(logo->GetDesc(0), 0);
			u16 w     = GXGetTexObjWidth(&tex);
			h2        = GXGetTexObjHeight(&tex);
			u32 color = (u32)(255.0f * logo->mAlpha) | 0xDC000000;
			fn_80197800(GAME_GS, &tex, 320.0f, h2 / 2.0f + 154.0f, 0.0f, w, h2, 0.0f, 0.0f, 0.0f, color, color, color, color);
		}
		if (logo->mState == 9 || logo->mState == 10 || logo->mState == 11) {
			tex       = logo->mTpl.mTex.Init(logo->GetDesc(1), 0);
			u16 w     = GXGetTexObjWidth(&tex);
			u16 h     = GXGetTexObjHeight(&tex);
			u32 color = (u32)(255.0f * logo->mAlpha) | 0xFFFFFF00;
			fn_80197800(GAME_GS, &tex, 320.0f, 224.0f, 0.0f, w, h, 0.0f, 0.0f, 0.0f, color, color, color, color);
		}
		if (logo->mState == 12 || logo->mState == 13 || logo->mState == 14) {
			tex       = logo->mTpl.mTex.Init(logo->GetDesc(2), 0);
			u16 w     = GXGetTexObjWidth(&tex);
			u16 h     = GXGetTexObjHeight(&tex);
			u32 color = (u32)(255.0f * logo->mAlpha) | 0xFFFFFF00;
			fn_80197800(GAME_GS, &tex, 320.0f, 224.0f, 0.0f, w, h, 0.0f, 0.0f, 0.0f, color, color, color, color);
		}
		fn_8019AFA0(GAME_GS->mCurTevStage, 4);
	}
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

extern "C" void fn_80027A7C(JObjVec* obj)
{
	obj->mJObj = NULL;
	obj->_1F4  = 0.0f;
	obj->_1F8  = 0.0f;
	obj->_1FC  = 0.0f;
}

/**
 * @note Address: 0x80028000
 * @note Size: 0xB4
 */
extern "C" void fn_80028000(XObjDemo* demo)
{
	CCamera* camera;
	CLight* light;

	if (demo->mState) {
		if (demo->mArchive.mDesc[0] != NULL) {
			camera = &demo->mCamera;
		} else {
			camera = GAME_CAMERA;
		}
		if (demo->mArchive.mDesc[1] != NULL) {
			light = &demo->mLights[0];
		} else {
			light = GAME_LIGHT;
		}
		light->SetCurrent(camera);
		if (camera->SetCurrent()) {
			demo->vfB0(0);
			camera->EndCurrent();
		}
	}
}

/**
 * @note Address: 0x80028368
 * @note Size: 0xA4
 */
extern "C" void fn_80028368(XObjDemo* demo)
{
	if (demo->mArchive.mDesc[0] != NULL) {
		demo->mCamera.Release();
	}
	for (s32 i = 0; i < demo->mLightCount; i++) {
		if (demo->mArchive.mDesc[i + 1] != NULL) {
			demo->mLights[i].Release();
		}
	}
	fn_800911C0(demo);
}

/**
 * @note Address: 0x8002840C
 * @note Size: 0xD4
 */
extern "C" BOOL fn_8002840C(XObjDemo* demo)
{
	CLight* last;

	if (fn_80091C80(demo)) {
		if (demo->mArchive.mDesc[0] != NULL) {
			demo->mCamera.Init((HSD_CObjDesc*)demo->mArchive.mDesc[0]->_00);
		}
		last = NULL;
		for (s32 i = 0; i < 8; i++) {
			if (demo->mArchive.mDesc[i + 1] != NULL) {
				demo->mLights[i].Init((HSD_LightDesc*)demo->mArchive.mDesc[i + 1]->_00, FALSE);
				demo->mLightCount++;
				if (last != NULL) {
					last->next = &demo->mLights[i];
				}
				last = &demo->mLights[i];
			}
		}
		if (last != NULL) {
			last->next = NULL;
		}
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x800284E0
 * @note Size: 0x64
 */
extern "C" void fn_800284E0(XObjDemo* demo)
{
	fn_800928E8(demo);
	demo->mState      = 0;
	demo->mWatchAnim  = FALSE;
	demo->mCameraAnim = -1;
	demo->mLightCount = 0;
	for (s32 i = 0; i < 8; i++) {
		demo->mLightAnim[i] = -1;
	}
}

// small virtuals of the house objects

extern "C" {
void fn_800902A4(XObj* obj);
void fn_8008A38C(void* anim);
}

/**
 * @note Address: 0x800256E4
 * @note Size: 0x38
 */
extern "C" void fn_800256E4(XObj* obj)
{
	fn_800902A4(obj);
	HSD_JObjSetFlags(obj->mJObj, CLASSICAL_SCALE);
}

/**
 * @note Address: 0x8002571C
 * @note Size: 0x4
 */
extern "C" void fn_8002571C(XObj* obj)
{
}

/**
 * @note Address: 0x80025720
 * @note Size: 0x24
 */
extern "C" void fn_80025720(XObj* obj)
{
	fn_8008A38C(&obj->mAnim);
}

/**
 * @note Address: 0x80025744
 * @note Size: 0x4
 */
extern "C" void fn_80025744(XObj* obj)
{
}

/**
 * @note Address: 0x80025748
 * @note Size: 0x1C
 */
extern "C" Vec fn_80025748(XObj* obj)
{
	return obj->mMoveTranslate;
}

/**
 * @note Address: 0x80025A40
 * @note Size: 0x8
 */
extern "C" s32 fn_80025A40(XObj* obj, s32 a, s32 b)
{
	return b;
}

/**
 * @note Address: 0x80025A48
 * @note Size: 0x50
 */
// NONMATCHING
extern "C" s32 fn_80025A48(XObj* obj, s32 count)
{
	for (s32 i = 0; i < count; i++) { }
	return 0;
}

/**
 * @note Address: 0x80025A98
 * @note Size: 0x8
 */
extern "C" s32 fn_80025A98(XObj* obj)
{
	return obj->_80C;
}

/**
 * @note Address: 0x80025AA0
 * @note Size: 0x4
 */
extern "C" void fn_80025AA0(XObj* obj)
{
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

/**
 * @note Address: 0x80025764
 * @note Size: 0xF4
 */
extern "C" HSD_AnimJoint* fn_80025764(XObj* obj, s32 id)
{
	if (GetAnimBank(obj, id) != 0) {
		return GetAnimJoint(&obj->mAnim, id % 1000);
	}
	return GetAnimJoint(&obj->mArchive, id);
}

/**
 * @note Address: 0x80025858
 * @note Size: 0xF4
 */
extern "C" HSD_MatAnimJoint* fn_80025858(XObj* obj, s32 id)
{
	if (GetAnimBank(obj, id) != 0) {
		return GetMatAnimJoint(&obj->mAnim, id % 1000);
	}
	return GetMatAnimJoint(&obj->mArchive, id);
}

/**
 * @note Address: 0x8002594C
 * @note Size: 0xF4
 */
extern "C" HSD_ShapeAnimJoint* fn_8002594C(XObj* obj, s32 id)
{
	if (GetAnimBank(obj, id) != 0) {
		return GetShapeAnimJoint(&obj->mAnim, id % 1000);
	}
	return GetShapeAnimJoint(&obj->mArchive, id);
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

/**
 * @note Address: 0x80026B64
 * @note Size: 0x38
 */
extern "C" void fn_80026B64(XObj* obj)
{
	if (obj->mJObj != NULL) {
		HSD_JObjDispAll(obj->mJObj, NULL, 7, 0);
	}
}

/**
 * @note Address: 0x80026B9C
 * @note Size: 0x90
 */
extern "C" void fn_80026B9C(XObjSpin* obj, u32 frames)
{
	obj->mAngle = fmod(obj->mSpeed * frames + obj->mAngle, 6.2831855f);
	obj->SetRotationZ(obj->mAngle, 0);
	fn_80090ED4(obj, frames);
}

/**
 * @note Address: 0x80026C2C
 * @note Size: 0x4C
 */
extern "C" BOOL fn_80026C2C(XObj* obj)
{
	if (fn_80091C80(obj)) {
		HSD_JObjSetFlags(obj->mJObj, CLASSICAL_SCALE);
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x80026C78
 * @note Size: 0x38
 */
extern "C" void fn_80026C78(XObjSpin* obj)
{
	fn_800929C4(obj);
	obj->mAngle = obj->mSpeed = 0.0f;
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

/**
 * @note Address: 0x800280B4
 * @note Size: 0x244
 */
// NONMATCHING
extern "C" void fn_800280B4(XObjDemo* demo, u32 frames)
{
	if (demo->mState == 0) {
		return;
	}
	if (demo->mState == 2) {
		bool done = false;
		if (0.0f == GAME_FADE_COLOR[0] && 0.0f == GAME_FADE_COLOR[1] && 0.0f == GAME_FADE_COLOR[2] && 0.0f == GAME_FADE_COLOR[3]) {
			done = true;
		}
		if (done) {
			demo->mState = 0;
			return;
		}
	}
	fn_80090ED4(demo, frames);
	if (demo->mArchive.mDesc[0] != NULL && demo->mCameraAnim >= 0) {
		demo->mCamera.SetAnimRate(frames);
		demo->mCamera.Anim();
	}
	for (s32 i = 0; i < demo->mLightCount; i++) {
		if (demo->mArchive.mDesc[i + 1] != NULL && demo->mLightAnim[i] >= 0) {
			demo->mLights[i].SetAnimRate(frames);
			demo->mLights[i].Anim();
		}
	}
	if (demo->mWatchAnim) {
		if (GetFirstAObj(demo->mJObj)->flags & 0x44000000) {
			demo->mState = 1;
			demo->SetMoveTranslate(demo->mMoveTranslate, 0);
			demo->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
	}
}

// the player's empty and forwarding handlers

struct Player;
extern "C" void fn_8002BD0C(Player* title);

/**
 * @note Address: 0x8002BB7C
 * @note Size: 0x4
 */
extern "C" void fn_8002BB7C(void* player)
{
}

/**
 * @note Address: 0x8002BCC8
 * @note Size: 0x4
 */
extern "C" void fn_8002BCC8(void* player)
{
}

/**
 * @note Address: 0x8002BCCC
 * @note Size: 0x20
 */
extern "C" void fn_8002BCCC(Player* title)
{
	fn_8002BD0C(title);
}

/**
 * @note Address: 0x8002BCEC
 * @note Size: 0x20
 */
extern "C" void fn_8002BCEC(Player* title)
{
	fn_8002BD0C(title);
}

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

/**
 * @note Address: 0x8002D188
 * @note Size: 0x4
 */
extern "C" void fn_8002D188(void* player)
{
}

/**
 * @note Address: 0x8002D4EC
 * @note Size: 0x4
 */
extern "C" void fn_8002D4EC(void* player)
{
}

/**
 * @note Address: 0x8002DCF8
 * @note Size: 0x4
 */
extern "C" void fn_8002DCF8(void* player)
{
}

/**
 * @note Address: 0x8002E8CC
 * @note Size: 0x4
 */
extern "C" void fn_8002E8CC(void* player)
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

/**
 * @note Address: 0x8002C1B8
 * @note Size: 0x118
 */
extern "C" void fn_8002C1B8(Player* player)
{
	CPad pad(0);

	if (!player->_37E0) {
		OnCardRemoved(player);
		player->_3C44 = 0;
		return;
	}
	if (pad.mPad->mTrigger & 0x100) {
		player->SetState(5);
		player->_37FC = 1;
		fn_8001E904(SOUND(), 0x88, 0xFF);
		player->_3C44 = 0;
	}
}

/**
 * @note Address: 0x8002C2D0
 * @note Size: 0x118
 */
extern "C" void fn_8002C2D0(Player* player)
{
	CPad pad(0);

	if (!player->_37E0) {
		OnCardRemoved(player);
		player->_3C44 = 0;
		return;
	}
	if (pad.mPad->mTrigger & 0x100) {
		player->SetState(5);
		player->_37FC = 1;
		fn_8001E904(SOUND(), 0x88, 0xFF);
		player->_3C44 = 0;
	}
}

// moves a vector by another one passed by value
static inline void AddVec(CVec& vec, Vec add)
{
	PSVECAdd(&vec, &add, &vec);
}

/**
 * @note Address: 0x800247D8
 * @note Size: 0x15C
 */
// a dust particle grows, then shrinks away while it drifts
void CPtclDust3::Update(u32 frames)
{
	if (!mActive) {
		return;
	}
	if (mScaleSpeed < 0.0f) {
		if ((mScale -= mScaleSpeed * frames) >= 1.0f) {
			mScaleSpeed = -mScaleSpeed;
		}
	}
	if (mScaleSpeed > 0.0f) {
		if ((mScale -= mScaleSpeed * frames) <= 0.0f) {
			mActive = 0;
			mScale  = 0.0f;
			return;
		}
	}
	AddVec(mPos, mVel * 1.0f * frames);
	mAngle += mSpin;
}

/**
 * @note Address: 0x80024B1C
 * @note Size: 0x184
 */
// the same, falling as it goes
void CPtclDust2::Update(u32 frames)
{
	if (!mActive) {
		return;
	}
	if (mScaleSpeed < 0.0f) {
		if ((mScale -= mScaleSpeed * frames) >= 1.0f) {
			mScaleSpeed = -mScaleSpeed;
		}
	}
	if (mScaleSpeed > 0.0f) {
		if ((mScale -= mScaleSpeed * frames) <= 0.0f) {
			mActive = 0;
			mScale  = 0.0f;
			return;
		}
	}
	mVel.y -= 0.05f * frames;
	AddVec(mPos, mVel * 1.0f * frames);
	mAngle += mSpin;
}

// where the dust comes from, class unknown
struct PtclSource {
	CVec mPos; // _00
	CVec mDir; // _10
	CVec mVel; // _20
	BOOL mOn;  // _30
};

/**
 * @note Address: 0x80024524
 * @note Size: 0x34
 */
extern "C" void fn_80024524(PtclSource* src)
{
	src->mPos.Set(0.0f, 0.0f, 0.0f);
	src->mDir.Set(0.0f, 0.0f, 0.0f);
	src->mVel.Set(0.0f, 0.0f, 0.0f);
	src->mOn = TRUE;
}

/**
 * @note Address: 0x80024E50
 * @note Size: 0x15C
 */
extern "C" CPtclOne* fn_80024E50(CPtclPool* pool, const Vec& pos, const Vec& dir, const Vec& vel, u32 color)
{
	for (s32 i = 0; i < pool->mCount; i++) {
		CPtclOne* ptcl = pool->Get(i);
		if (!ptcl->mActive) {
			ptcl->mActive     = 1;
			ptcl->mPos        = CVec(pos);
			ptcl->mVel        = CVec(vel);
			ptcl->mDir        = CVec(dir);
			ptcl->mAngle      = fmod((f32)rand(), 3.1415927f);
			ptcl->mSpin       = 0.0f;
			ptcl->mColor      = color;
			ptcl->mScale      = 1.0f;
			ptcl->mScaleSpeed = 0.02f;
			ptcl->mTex        = NULL;
			return ptcl;
		}
	}
	return NULL;
}

extern "C" void fn_80030CE8(Player* title);

/**
 * @note Address: 0x80030EE4
 * @note Size: 0x120
 */
// scatters the sparkles over the 640 by 480 screen
extern "C" void fn_80030EE4(Player* title)
{
	fn_80030CE8(title);
	title->mDemo.Init(0);
	for (s32 i = 0; i < 16; i++) {
		title->mSparks[i]._00 = 0;
		title->mSparks[i]._04 = 0;
		title->mSparks[i].mX  = rand() % 640;
		title->mSparks[i].mY  = rand() % 480;
		title->mSparks[i]._10 = 0.0f;
		title->mSparks[i]._14 = 0.0f;
	}
	title->_3C44 = 0;
}

extern "C" void fn_80089890(void* obj, s32 arg, s32 flag);

/**
 * @note Address: 0x8002D18C
 * @note Size: 0x148
 */
extern "C" void fn_8002D18C(Player* title)
{
	CPad pad(0);

	if (title->_37E2 == 1 || title->_37E0 == 1) {
		for (s32 i = 0; i < 3; i++) {
			fn_8003D7DC(&lbl_80361E8C[i]);
		}
		fn_80089890(lbl_8025C400 + 0xE8400, title->_3804, 1);
		title->mPrevState = title->mState;
		title->mState     = 1;
		title->mStep      = 0;
		fn_8001E904(SOUND(), 0x88, 0xFF);
		title->_37CC = 0.0f;
		return;
	}
	if (pad.mPad->mTrigger & 0x100) {
		title->SetState(5);
		title->_37FC = 1;
		fn_8001E904(SOUND(), 0x88, 0xFF);
	}
}

// the memory card's step and its last result, in CGame
#define CARD_STEP   (*(s32*)(lbl_8025C400 + 0xE8400))
#define CARD_RESULT (*(s32*)(lbl_8025C400 + 0xE8438))
#define CARD_FREE0  (*(s32*)(lbl_8025C400 + 0xFEE00))
#define CARD_FREE1  (*(s32*)(lbl_8025C400 + 0xFEE04))

/**
 * @note Address: 0x8002CCF4
 * @note Size: 0x14C
 */
// waits for the card check, then goes on to the state its result asks for
extern "C" void fn_8002CCF4(Player* title, u32 frames)
{
	s32 result;

	title->_37F4 -= frames;
	if (title->_37F4 < 0) {
		title->_37F4 = 0;
	}
	switch (CARD_STEP) {
	case 0:
		title->_37D0      = CARD_FREE1;
		title->_37D4      = CARD_FREE0;
		title->mPrevState = title->mState;
		title->mState     = 13;
		title->mStep      = 0;
		title->_37E8      = 1.0f;
		break;
	case 4:
		break;
	case 1:
	case 2:
	case 3:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		result       = CARD_RESULT;
		title->_37D8 = result;
		switch (result) {
		case -128:
		case -14:
		case -13:
		case -12:
		case -11:
		case -10:
		case -9:
		case -8:
		case -7:
		case -6:
		case -5:
		case -4:
		case -3:
		case -2:
		case -1:
		case 0:
			if (result == -6 || result == -13) {
				title->mPrevState = title->mState;
				title->mState     = 19;
				title->mStep      = 0;
				title->_37E1      = 1;
			} else {
				title->mPrevState = title->mState;
				title->mState     = 21;
				title->mStep      = 0;
				title->_37E1      = 1;
			}
			title->_37E8 = 1.0f;
			break;
		default:
			title->mPrevState = title->mState;
			title->mState     = 14;
			title->mStep      = 0;
			title->_37E8      = 1.0f;
			title->_37CC      = -32.0f;
			break;
		}
		break;
	}
}

extern "C" BOOL fn_8001E660(CSound* sound, s32 sfx);
#define GAME_9108 (*(s32*)(lbl_8025C400 + 0x9108))

/**
 * @note Address: 0x8002BB80
 * @note Size: 0x148
 */
extern "C" void fn_8002BB80(Player* title, u32 frames)
{
	switch (title->mStep) {
	case 0:
		title->_37F4 -= frames;
		if (title->_37F4 < 0) {
			title->_37F4 = 0;
			title->mStep++;
		}
		break;
	case 1:
		for (u32 i = 0; i < 20; i++) {
			title->mName.mChars[i].mWait = 0.0f;
		}
		title->_37BC      = -200.0f;
		title->_37CC      = 0.0f;
		title->_37E8      = 0.0f;
		title->_37FC      = 0;
		title->_00        = 13;
		title->mPrevState = title->mState;
		title->mState     = 25;
		title->mStep      = 0;
		fn_8001E904(SOUND(), 0x88, 0xFF);
		fn_8001E660(SOUND(), 0xFFFF0003);
		GAME_9108 = title->_37F8;
		break;
	}
}

extern "C" void fn_80068B98(void* obj);

/**
 * @note Address: 0x8002D2D4
 * @note Size: 0x218
 */
// waits for the card to be saved to, then goes on to the state its result asks for
extern "C" void fn_8002D2D4(Player* title)
{
	s32 step = CARD_STEP;
	s32 result;

	switch (step) {
	case 0:
		for (u32 i = 0; i < 20; i++) {
			title->mName.mChars[i].mWait = 0.0f;
		}
		fn_8001E660(SOUND(), 0xFFFF0003);
		title->_37BC = -200.0f;
		title->_37E8 = 0.0f;
		GAME_9108    = title->_37F8;
		fn_80068B98(lbl_8025C400 + 0x8F38);
		title->mPrevState = title->mState;
		title->mState     = 25;
		title->mStep      = 0;
		title->_00        = 13;
		title->_0C        = 20000;
		break;
	case 4:
		break;
	case 1:
	case 2:
	case 3:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		result       = CARD_RESULT;
		title->_37D8 = result;
		switch (result) {
		case -128:
		case -14:
		case -13:
		case -12:
		case -11:
		case -10:
		case -9:
		case -8:
		case -7:
		case -6:
		case -5:
		case -4:
		case -3:
		case -2:
		case -1:
		case 0:
			if (step == 2 && (result == -6 || result == -13)) {
				title->mPrevState = title->mState;
				title->mState     = 11;
				title->mStep      = 0;
				title->_37FC      = 1;
				title->_37E1      = 1;
			} else if (result == -3 || result == -5) {
				title->mPrevState = title->mState;
				title->mState     = 22;
				title->mStep      = 0;
				title->_37E1      = 1;
			} else {
				title->mPrevState = title->mState;
				title->mState     = 14;
				title->mStep      = 0;
				title->_37CC      = -32.0f;
			}
			title->_37E8 = 1.0f;
			break;
		default:
			title->mPrevState = title->mState;
			title->mState     = 14;
			title->mStep      = 0;
			title->_37E8      = 1.0f;
			title->_37CC      = -32.0f;
			break;
		}
		break;
	}
}

/**
 * @note Address: 0x8002CB40
 * @note Size: 0x1B4
 */
extern "C" void fn_8002CB40(Player* title, u32 frames)
{
	CPad pad(0);

	if (!title->_37E0) {
		OnCardRemoved(title);
		return;
	}
	switch (title->mStep) {
	case 0:
		title->_37F4 -= frames;
		if (title->_37F4 < 0) {
			title->_37F4 = 0;
			title->mStep++;
			fn_8001E904(SOUND(), 0x88, 0xFF);
		}
		break;
	case 1:
		if (pad.mPad->mTrigger & 0x100) {
			for (s32 i = 0; i < 3; i++) {
				fn_8003D7DC(&lbl_80361E8C[i]);
			}
			fn_80089890(lbl_8025C400 + 0xE8400, title->_3804, 1);
			title->mPrevState = title->mState;
			title->mState     = 1;
			title->mStep      = 0;
			fn_8001E904(SOUND(), 0x88, 0xFF);
			return;
		}
		break;
	}
}

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

/**
 * @note Address: 0x8002DCFC
 * @note Size: 0x240
 */
// the yes or no question, left and right pick, A takes it
extern "C" void fn_8002DCFC(Player* title)
{
	CPad pad(0);
	PadState* state;

	if (!title->_37E0) {
		OnCardRemoved(title);
		return;
	}
	state = (PadState*)pad.mPad;
	if (state->mRepeat & 0x40001) {
		if (title->_37FC > 0) {
			title->_37FC--;
			fn_8001E904(SOUND(), 0x87, 0xFF);
			return;
		}
	} else if (state->mRepeat & 0x80002) {
		if (title->_37FC < 1) {
			title->_37FC++;
			fn_8001E904(SOUND(), 0x87, 0xFF);
			return;
		}
	}
	if (state->mTrigger & 0x100) {
		switch (title->_37FC) {
		case 0:
			fn_8008A1F4(lbl_8025C400 + 0xE8400, title->_3804, title->_37F8, 1);
			title->mPrevState = title->mState;
			title->mState     = 15;
			title->mStep      = 0;
			title->_37F4      = 90;
			fn_8001E904(SOUND(), 0x88, 0xFF);
			title->_37CC = -32.0f;
			return;
		case 1:
			title->mPrevState = title->mState;
			title->mState     = 2;
			title->mStep      = 0;
			title->_37FC      = 0;
			fn_8001E904(SOUND(), 0x86, 0xFF);
			return;
		}
	}
}

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

/**
 * @note Address: 0x80029168
 * @note Size: 0x5F8
 */
// the options: up and down pick rumble, sound or back, A changes the line, B goes back. the
// rumble test runs the motor for 24 frames
extern "C" void fn_80029168(Player* title, u32 frames)
{
	CPad pad(0);

	title->mCursor.vfA4(frames);
	title->_1C += frames;
	if (title->_1C >= 60.0f) {
		title->_1C = 60;
		title->_18 = fmod(title->_18 + frames * 9, 360.0);
	}
	if (!GetPadState(pad)->mConnected && title->_3818) {
		title->_3818 = FALSE;
		GAME_STATUS &= ~0x800000000ULL;
	}

	switch (title->_08) {
	case 0:
		if (GetPadState(pad)->mRepeat & 0x10008) {
			if (title->_380C > 0) {
				title->_380C--;
				fn_8001E904(SOUND(), 0x87, 0xFF);
				title->_18 = 45.0f;
			}
		} else if (GetPadState(pad)->mRepeat & 0x20004) {
			if (title->_380C < 2) {
				title->_380C++;
				fn_8001E904(SOUND(), 0x87, 0xFF);
				title->_18 = 45.0f;
			}
		}
		if (GetPadState(pad)->mTrigger & 0x100) {
			switch (title->_380C) {
			case 0:
				if (GetPadState(pad)->mConnected) {
					if (title->_3818) {
						title->_3818 = FALSE;
						GAME_STATUS &= ~0x800000000ULL;
					} else {
						title->_3818 = TRUE;
						GAME_STATUS |= 0x800000000ULL;
						if (GetPadState(pad)->mConnected) {
							HSD_PadRumbleOn(GetPadState(pad)->mPort);
						}
						title->_08   = 1;
						title->_3820 = 24;
					}
					fn_8001E904(SOUND(), 0x88, 0xFF);
				} else {
					fn_8001E904(SOUND(), 0x85, 0xFF);
				}
				break;
			case 1:
				if (title->_3819) {
					title->_3819 = FALSE;
					GAME_STATUS &= ~0x1000000000ULL;
					fn_8001E00C(SOUND(), FALSE);
				} else {
					title->_3819 = TRUE;
					GAME_STATUS |= 0x1000000000ULL;
					fn_8001E00C(SOUND(), TRUE);
				}
				fn_8001E904(SOUND(), 0x88, 0xFF);
				break;
			case 2:
				title->_08 = 2;
				title->mCursor.SetAnim(1, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				title->_18 = 90.0f;
				fn_8001E904(SOUND(), 0x88, 0xFF);
				break;
			}
		} else if (GetPadState(pad)->mTrigger & 0x200) {
			title->_08 = 2;
			title->mCursor.SetAnim(1, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			title->_18 = 90.0f;
			fn_8001E904(SOUND(), 0x88, 0xFF);
		}
		break;
	case 1:
		if (--title->_3820 < 0) {
			title->_08 = 0;
			if (GetPadState(pad)->mConnected) {
				HSD_PadRumbleOffN(GetPadState(pad)->mPort);
			}
			if (GetPadState(pad)->mConnected) {
				HSD_PadRumbleRemove(GetPadState(pad)->mPort);
			}
		}
		break;
	case 2:
		HSD_AObj* aobj = GetFirstAObj(title->mCursor.mJObj);
		if ((aobj->flags & 0x44000000) || title->mCursor.mAnimId == 0 || title->mCursor.mArchive.mNum[0] < 2) {
			title->mCursor.SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			title->_18 = 0.0f;
			title->_00 = 11;
			fn_8001E904(SOUND(), 0x88, 0xFF);
			title->_3814 = 0.0f;
			title->_08   = 0;
			title->_380C = 0;
			title->_37BC = -200.0f;
			title->_37CC = 0.0f;
			title->_37E8 = 0.0f;
			for (u32 i = 0; i < 20; i++) {
				title->mName.mChars[i].mWait = 0.0f;
			}
		}
		break;
	}
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

/**
 * @note Address: 0x80029760
 * @note Size: 0x69C
 */
// NONMATCHING
// draws the options: a window, its title and the three lines, the picked one lit once the
// window is open. the back line bobs while the cursor plays its anim
extern "C" void fn_80029760(Player* title)
{
	f32 scale = title->_3810;
	if (scale > 0.0f) {
		f32 alpha  = 255.0f * scale;
		f32 h      = 224.0f * scale;
		s32 a      = alpha;
		u32 window = (u32)alpha | 0xFFFFFF00;
		f32 top;
		fn_800652C0((u32)(160.0f * scale), window, 320.0f, top = 224.0f - h / 2.0f, 320.0f * scale, h, 1.0f, 1.0f, 1.0f);

		u32 lit    = (u8)a | 0xFFFFFF00;
		u32 normal = (u8)alpha | 0xFFFFFF00;
		fn_8006DBF0(0x60);
		f32 titleSize    = 24.0f * title->_3810;
		const char* text = fn_8006DBF0(0x60);
		fn_80196610(lbl_8025C400, normal, text, 320.0f - fn_801968BC(lbl_8025C400, text, 1, titleSize) / 2.0f, titleSize + top, 0.0f,
		            titleSize, titleSize);

		f32 size = 24.0f * title->_3810;
		bool sel = false;
		if (title->_380C == 0 && title->_1C >= 60.0f) {
			sel = true;
		}
		u32 color = normal;
		if (sel) {
			color = lit;
		}
		text  = fn_8006DBF0(0x10);
		f32 x = 320.0f - fn_801968BC(lbl_8025C400, text, 1, size) / 2.0f - 8.0f;
		f32 y;
		fn_80196610(lbl_8025C400, color, text, x - fn_801968BC(lbl_8025C400, text, 1, size) / 2.0f, y = size + (4.0f * (size / 2.0f) + top),
		            0.0f, size, size);
		text = title->_3818 ? fn_8006DBF0(0x11) : fn_8006DBF0(0x12);
		x    = 8.0f + (fn_801968BC(lbl_8025C400, text, 1, size) / 2.0f + 320.0f);
		fn_80196610(lbl_8025C400, normal, text, x - fn_801968BC(lbl_8025C400, text, 1, size) / 2.0f, y, 0.0f, size, size);

		sel = false;
		if (title->_380C == 1 && title->_1C >= 60.0f) {
			sel = true;
		}
		color = normal;
		if (sel) {
			color = lit;
		}
		text = fn_8006DBF0(0x13);
		x    = 320.0f - fn_801968BC(lbl_8025C400, text, 1, size) / 2.0f - 8.0f;
		fn_80196610(lbl_8025C400, color, text, x - fn_801968BC(lbl_8025C400, text, 1, size) / 2.0f,
		            y = size + (4.0f * (2.0f * (size / 2.0f)) + top), 0.0f, size, size);
		text = title->_3819 ? fn_8006DBF0(0x14) : fn_8006DBF0(0x15);
		x    = 8.0f + (fn_801968BC(lbl_8025C400, text, 1, size) / 2.0f + 320.0f);
		fn_80196610(lbl_8025C400, normal, text, x - fn_801968BC(lbl_8025C400, text, 1, size) / 2.0f, y, 0.0f, size, size);

		f32 t        = title->mCursor.mAnimId == 1 ? GetAnimProgress(title) : 0.0f;
		f32 bob      = title->_380C == 2 ? 2.0f * (f32)sin(4.712389f * t) : 0.0f;
		f32 backSize = 24.0f + bob;
		sel          = false;
		if (title->_380C == 2 && title->_1C >= 60.0f) {
			sel = true;
		}
		if (sel) {
			normal = lit;
		}
		text = fn_8006DBF0(0x16);
		fn_80196610(lbl_8025C400, normal, text, 320.0f - fn_801968BC(lbl_8025C400, text, 1, size) / 2.0f,
		            (backSize - size) / 2.0f + ((2.0f * size + (4.0f * (3.0f * (size / 2.0f)) + top)) - backSize), 0.0f, size, size);
	}
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

/**
 * @note Address: 0x80028BAC
 * @note Size: 0x5BC
 */
// NONMATCHING: the loop's float registers are assigned in another order
// draws the title's name: a bubble per letter, then the letter with a shadow on four sides
extern "C" void fn_80028BAC(Player* title)
{
	if (title->mName.mActive) {
		fn_8019B56C(GAME_GS->mCurRenderMode, 1, 4, 5, 15);
		fn_80199AB0(lbl_8025C400, 0, 0);
		fn_8019B464(GAME_GS->mCurRenderMode, 1, 7, 0);
		fn_8019AFA0(GAME_GS->mCurTevStage, 0);
		CTexObj bubble = GAME_TPL.mTex.Init(&((TPLPalette*)GAME_TPL.mBuffer)->descriptorArray[108], 0);
		f32 zero       = 0.0f;
		u32 color      = title->mName.mColor & 0xFFFFFF00;
		for (s32 i = 0; i < title->mName.mCount; i++) {
			f32 scale = title->mName.mChars[i].mScale;
			if (scale <= zero || strncmp(title->mName.mChars[i].mText, " ", 1) == 0) {
				continue;
			}
			CFont font;
			s32 width;
			CTexObj letter = *fn_8019E8F4(&font, title->mName.mChars[i].mText, &width);
			f32 bob;
			f32 tilt;
			f32 swell;
			f32 pop;
			f32 x;
			f32 rot;
			f32 y;
			bob       = 4.0f * (f32)sin(title->mName.mChars[i].mBob);
			tilt      = sin(0.7f * title->mName.mChars[i].mTilt);
			swell     = sin(3.1415927f * scale);
			pop       = 0.2f * swell + (f32)sin(1.5707964f * scale);
			x         = 24.0f + (48.0f * i + (320.0f - 48.0f * title->mName.mCount / 2.0f));
			u32 alpha = 255.0f * scale;
			rot       = -(0.017453292f * (10.0f * tilt));
			y         = 64.0f + bob;
			fn_80197800(GAME_GS, &bubble, x, y, 0.0f, 20.0f * (2.0f * pop), 20.0f * (2.0f * pop), zero, -zero, rot, alpha | 0xFFFFFF00,
			            alpha | 0xFFFFFF00, alpha | 0xFFFFFF00, alpha | 0xFFFFFF00);
			f32 size   = 20.0f * (1.7f * pop);
			f32 shrink = width / 24.0f * size;
			f32 off    = size - shrink;
			for (s32 j = 0; j < 4; j++) {
				fn_80197800(GAME_GS, &letter, off / 2.0f + (x + lbl_8021195C[j]), bob + (64.0f + lbl_8021196C[j]), 0.0f, size, size, zero,
				            -zero, rot, alpha, alpha, alpha, alpha);
			}
			fn_80197800(GAME_GS, &letter, off / 2.0f + x, y, 0.0f, size, size, zero, -zero, rot, color | alpha, color | alpha,
			            color | alpha, color | alpha);
		}
		fn_8019AFA0(GAME_GS->mCurTevStage, 4);
		fn_8019B464(GAME_GS->mCurRenderMode, 0, 7, 0);
		fn_80199AB0(lbl_8025C400, 0, 0);
	}
}

extern "C" void fn_80088E78(void* obj, s32 a, s32 flag);

/**
 * @note Address: 0x8002CE40
 * @note Size: 0x348
 */
// asked twice whether to make a new save file
extern "C" void fn_8002CE40(Player* title)
{
	CPad pad(0);

	if (!title->_37E0) {
		OnCardRemoved(title);
		return;
	}
	switch (title->mStep) {
	case 0:
		MoveYesNo(title, pad);
		if (GetPadState(pad)->mTrigger & 0x100) {
			if (title->_37FC == 0) {
				fn_8001E904(SOUND(), 0x88, 0xFF);
				title->_37FC = 1;
				title->mStep++;
			} else {
				title->mPrevState = title->mState;
				title->mState     = 19;
				title->mStep      = 0;
				fn_8001E904(SOUND(), 0x88, 0xFF);
				title->_3C44 = 1;
			}
			return;
		}
		break;
	case 1:
		MoveYesNo(title, pad);
		if (GetPadState(pad)->mTrigger & 0x100) {
			if (title->_37FC == 0) {
				fn_80088E78(lbl_8025C400 + 0xE8400, title->_3804, 1);
				title->mPrevState = title->mState;
				title->mState     = 12;
				title->mStep      = 0;
				title->_37F4      = 90;
				fn_8001E904(SOUND(), 0x88, 0xFF);
			} else {
				title->mPrevState = title->mState;
				title->mState     = 19;
				title->mStep      = 0;
				fn_8001E904(SOUND(), 0x88, 0xFF);
				title->_3C44 = 1;
			}
			return;
		}
		break;
	}
}

/**
 * @note Address: 0x8002D4F0
 * @note Size: 0x304
 */
// asked whether to save over the file, unless the card went away under it
extern "C" void fn_8002D4F0(Player* title)
{
	CPad pad(0);

	if ((!title->_37E0 || !title->_3809) && title->_37A8 == 1) {
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
		return;
	}
	if (!title->_37E0) {
		OnCardRemoved(title);
		return;
	}
	if (GetPadState(pad)->mRepeat & 0x40001) {
		if (title->_37FC > 0) {
			title->_37FC--;
			fn_8001E904(SOUND(), 0x87, 0xFF);
			return;
		}
	} else if (GetPadState(pad)->mRepeat & 0x80002) {
		if (title->_37FC < 1) {
			title->_37FC++;
			fn_8001E904(SOUND(), 0x87, 0xFF);
			return;
		}
	}
	if (GetPadState(pad)->mTrigger & 0x100) {
		switch (title->_37FC) {
		case 0:
			fn_8008A1F4(lbl_8025C400 + 0xE8400, title->_3804, title->_37F8, 1);
			title->mPrevState = title->mState;
			title->mState     = 15;
			title->mStep      = 0;
			title->_37F4      = 90;
			fn_8001E904(SOUND(), 0x88, 0xFF);
			title->_37CC = -32.0f;
			break;
		case 1:
			title->mPrevState = title->mState;
			title->mState     = 2;
			title->mStep      = 0;
			title->_37FC      = 0;
			fn_8001E904(SOUND(), 0x86, 0xFF);
			break;
		}
	}
}

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

/**
 * @note Address: 0x80024600
 * @note Size: 0x1D8
 */
// NONMATCHING
// a dust puff, a sprite that swells and fades with its scale
void CPtclDust3::Draw(CGs* gs)
{
	if (!mActive) {
		return;
	}
	f32 w     = 2.0f * (mDir.x * (f32)sin(0.8f * (3.1415927f * mScale)));
	f32 h     = 2.0f * (mDir.y * (f32)sin(0.8f * (3.1415927f * mScale)));
	u32 color = (mColor & 0xFFFFFF00) | (u8)((mColor & 0xFF) * mScale);
	if (mTex != NULL) {
		fn_8019AFA0(gs->mCurTevStage, 0);
		Vec rot = { 0.0f, 0.0f, mAngle };
		DrawSprite(gs, mTex, mPos, w, h, rot, color);
		fn_8019AFA0(gs->mCurTevStage, 4);
	} else {
		Vec rot = { 0.0f, 0.0f, mAngle };
		DrawRect(gs, mPos, w, h, rot, color);
	}
}

extern "C" void fn_8001E630(CSound* sound, s32 track);

/**
 * @note Address: 0x80030CE8
 * @note Size: 0x1FC
 */
extern "C" void fn_80030CE8(Player* title)
{
	title->_00    = 0;
	title->mState = 0;
	title->_0C    = -1;
	title->_10    = 0.0f;
	title->_14    = 0.0f;
	title->_18    = 90.0f;
	title->_1C    = 0;
	title->_20    = 0;
	title->mCursor.Init(0);
	title->_9A8       = 0.0f;
	title->_9AC       = 0.0f;
	title->_37A8      = 0;
	title->_37F8      = 0;
	title->_37FC      = 0;
	title->_37E4      = 0.0f;
	title->_37E8      = 0.0f;
	title->mPrevState = 0;
	title->mStep      = 0;
	title->_37F4      = 0;
	title->_37B0      = -10.0f;
	title->_37B4      = -200.0f;
	title->_37B8      = -10.0f;
	title->_37BC      = -200.0f;
	title->_37C0      = 0.0f;
	title->_37C4      = 0.0f;
	title->_37C8      = 0.0f;
	title->_37CC      = 0.0f;
	title->_380C      = 0;
	title->_08        = 0;
	title->_3810      = 0.0f;
	title->_3814      = 0.0f;

	CPad pad(0);
	if (GetPadState(pad)->mConnected) {
		if (GAME_STATUS & 0x800000000ULL) {
			title->_3818 = TRUE;
		} else {
			title->_3818 = FALSE;
		}
	} else {
		title->_3818 = FALSE;
	}
	if (GAME_STATUS & 0x1000000000ULL) {
		title->_3819 = TRUE;
	} else {
		title->_3819 = FALSE;
	}
	title->_3804 = 0;
	for (s32 i = 0; i < 3; i++) {
		fn_8003D7DC(&lbl_80361E8C[i]);
	}
	title->_37AC = 0.0f;
	title->_3809 = 0;
	title->_3808 = 0;
	fn_8001E630(SOUND(), 1);
	title->_3C44 = 0;
}

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

/**
 * @note Address: 0x8002C738
 * @note Size: 0x408
 */
// shows what is wrong with the card until A is pressed or the card changes
extern "C" void fn_8002C738(Player* title)
{
	CPad pad(0);
	s32 error = title->_37D8;

	switch (error) {
	case -3:
		if (title->_37E2 == 1) {
			for (s32 i = 0; i < 3; i++) {
				fn_8003D7DC(&lbl_80361E8C[i]);
			}
			fn_80089890(lbl_8025C400 + 0xE8400, title->_3804, 1);
			title->mPrevState = title->mState;
			title->mState     = 1;
			title->mStep      = 0;
			fn_8001E904(SOUND(), 0x88, 0xFF);
			title->_37CC = 0.0f;
			return;
		}
		if (title->_37E0 == 1) {
			for (s32 i = 0; i < 3; i++) {
				fn_8003D7DC(&lbl_80361E8C[i]);
			}
			fn_80089890(lbl_8025C400 + 0xE8400, title->_3804, 1);
			title->mPrevState = title->mState;
			title->mState     = 1;
			title->mStep      = 0;
			fn_8001E904(SOUND(), 0x88, 0xFF);
			title->_37CC = 0.0f;
			return;
		}
		break;
	case -2:
		if (!title->_37E2) {
			OnCardRemoved(title);
			return;
		}
		break;
	case -128:
		if ((!title->_37E0 || !title->_3809) && title->_37A8 == 1) {
			OnFormattedCardGone(title);
			return;
		}
		break;
	default:
		if (!title->_37E0) {
			OnCardRemoved(title);
			return;
		}
		break;
	}
	if (GetPadState(pad)->mTrigger & 0x100) {
		switch (error) {
		case -9:
			title->mPrevState = title->mState;
			title->mState     = 18;
			title->mStep      = 0;
			title->_37CC      = 0.0f;
			break;
		case -128:
			title->SetState(6);
			break;
		default:
			title->SetState(5);
			title->_37FC = 1;
			break;
		}
		fn_8001E904(SOUND(), 0x88, 0xFF);
	}
}

/**
 * @note Address: 0x8002D7F4
 * @note Size: 0x504
 */
// asked whether to go on without saving
extern "C" void fn_8002D7F4(Player* title)
{
	CPad pad(0);

	switch (title->_37D8) {
	case -3:
		if (title->_37E0 == 1) {
			for (s32 i = 0; i < 3; i++) {
				fn_8003D7DC(&lbl_80361E8C[i]);
			}
			fn_80089890(lbl_8025C400 + 0xE8400, title->_3804, 1);
			title->_37CC      = 0.0f;
			title->mPrevState = title->mState;
			title->mState     = 1;
			title->mStep      = 0;
			fn_8001E904(SOUND(), 0x88, 0xFF);
			return;
		}
		break;
	case -2:
		if (!title->_37E2) {
			OnCardRemoved(title);
			return;
		}
		break;
	default:
		if (!title->_37E0) {
			OnCardRemoved(title);
			return;
		}
		break;
	}
	MoveYesNo(title, pad);
	if (GetPadState(pad)->mTrigger & 0x100) {
		if (title->_37FC == 0 && (GetPadState(pad)->mTrigger & 0x100)) {
			for (u32 i = 0; i < 20; i++) {
				title->mName.mChars[i].mWait = 0.0f;
			}
			title->_37BC      = -200.0f;
			title->_37CC      = 0.0f;
			title->_37E8      = 0.0f;
			title->_37FC      = 0;
			title->_00        = 13;
			title->mPrevState = title->mState;
			title->mState     = 25;
			title->mStep      = 0;
			fn_8001E904(SOUND(), 0x88, 0xFF);
			fn_8001E660(SOUND(), 0xFFFF0003);
			GAME_9108 = title->_37F8;
			return;
		}
		if (title->_37FC == 1 && (GetPadState(pad)->mTrigger & 0x100)) {
			if (title->_37E1 == 1) {
				title->mPrevState = title->mState;
				title->mState     = 19;
				title->mStep      = 0;
				title->_37CC      = 0.0f;
			} else if (title->_37D8 == 0) {
				title->mPrevState = title->mState;
				title->mState     = 2;
				title->mStep      = 0;
				title->_37FC      = 0;
			} else if (title->_37D8 == -3) {
				title->mPrevState = title->mState;
				title->mState     = 25;
				title->mStep      = 0;
				title->_00        = 11;
				fn_8001E904(SOUND(), 0x88, 0xFF);
				title->_37BC = -200.0f;
				title->_37E8 = 0.0f;
				title->_37FC = 0;
				title->_37CC = 0.0f;
				for (u32 i = 0; i < 20; i++) {
					title->mName.mChars[i].mWait = 0.0f;
				}
			} else {
				for (s32 i = 0; i < 3; i++) {
					fn_8003D7DC(&lbl_80361E8C[i]);
				}
				fn_80089890(lbl_8025C400 + 0xE8400, title->_3804, 1);
				title->mPrevState = title->mState;
				title->mState     = 1;
				title->mStep      = 0;
				title->_37CC      = 0.0f;
			}
			fn_8001E904(SOUND(), 0x88, 0xFF);
			return;
		}
	}
}

/**
 * @note Address: 0x8002BD0C
 * @note Size: 0x4AC
 */
// the card check's message, it waits for A or for the card to change
extern "C" void fn_8002BD0C(Player* title)
{
	CPad pad(0);
	s32 error = title->_37D8;

	if (error == -3) {
		if (title->_37A8 == 1) {
			if (title->_37E0) {
				OnFormattedCardGone(title);
				return;
			}
		} else if (title->_37E0 == 1) {
			for (s32 i = 0; i < 3; i++) {
				fn_8003D7DC(&lbl_80361E8C[i]);
			}
			fn_80089890(lbl_8025C400 + 0xE8400, title->_3804, 1);
			title->mPrevState = title->mState;
			title->mState     = 1;
			title->mStep      = 0;
			fn_8001E904(SOUND(), 0x88, 0xFF);
			return;
		}
	} else if (title->_37A8 == 1) {
		if (!title->_37E0 || !title->_3809) {
			OnFormattedCardGone(title);
			return;
		}
	} else if (!title->_37E0) {
		OnCardRemoved(title);
		return;
	}
	if (GetPadState(pad)->mTrigger & 0x100) {
		if (title->_37A8 == 1) {
			OnFormattedCardGone(title);
			return;
		}
		if (error == -3) {
			OnCardRemoved(title);
		} else if (error == -5) {
			title->mPrevState = title->mState;
			title->mState     = 19;
			title->mStep      = 0;
			title->_37E1      = 1;
		} else {
			title->SetState(5);
			title->_37FC = 1;
		}
		fn_8001E904(SOUND(), 0x88, 0xFF);
	}
}

extern "C" void fn_80089C90(void* obj, s32 a, s32 b, s32 flag);

/**
 * @note Address: 0x8002DF3C
 * @note Size: 0x638
 */
// picks one of the three save files, A starts or loads it, B goes back
extern "C" void fn_8002DF3C(Player* title)
{
	CPad pad(0);

	if ((!title->_37E0 || !title->_3809) && title->_37A8 == 1) {
		OnFormattedCardGone(title);
		return;
	}
	if (!title->_37E0) {
		OnCardRemoved(title);
		return;
	}
	title->_37CC = 0.0f;
	if (GetPadState(pad)->mRepeat & 0x40001) {
		title->_37F8--;
		if (title->_37F8 < 0) {
			title->_37F8 = 2;
		}
		fn_8001E904(SOUND(), 0x87, 0xFF);
	} else if (GetPadState(pad)->mRepeat & 0x80002) {
		title->_37F8++;
		if (title->_37F8 > 2) {
			title->_37F8 = 0;
		}
		fn_8001E904(SOUND(), 0x87, 0xFF);
	}
	if (GetPadState(pad)->mTrigger & 0x100) {
		switch (title->_37A8) {
		case 0:
			if (lbl_80361E8C[title->_37F8].mUsed == 1) {
				title->mPrevState = title->mState;
				title->mState     = 3;
				title->mStep      = 0;
				title->_37FC      = 1;
				fn_8001E904(SOUND(), 0x88, 0xFF);
			} else if (title->_37D0 >= 6 && title->_37D4 > 0) {
				fn_8008A1F4(lbl_8025C400 + 0xE8400, title->_3804, title->_37F8, 1);
				title->mPrevState = title->mState;
				title->mState     = 15;
				title->mStep      = 0;
				title->_37F4      = 90;
				title->_37CC      = -32.0f;
				fn_8001E904(SOUND(), 0x88, 0xFF);
			} else {
				title->_37D8      = -9;
				title->mPrevState = title->mState;
				title->mState     = 14;
				title->mStep      = 0;
				fn_8001E904(SOUND(), 0x88, 0xFF);
				title->_37CC = -32.0f;
			}
			return;
		case 1:
			if (lbl_80361E8C[title->_37F8].mUsed == 1) {
				if (!fn_8003D7C4(&lbl_80361E8C[title->_37F8])) {
					title->_37D8      = -128;
					title->mPrevState = title->mState;
					title->mState     = 14;
					title->mStep      = 0;
					fn_8001E904(SOUND(), 0x88, 0xFF);
					title->_37CC = 0.0f;
					return;
				}
				if (lbl_80361E8C[title->_37F8]._28 == 0) {
					for (u32 i = 0; i < 20; i++) {
						title->mName.mChars[i].mWait = 0.0f;
					}
					title->_37BC      = -200.0f;
					title->_37E8      = 0.0f;
					title->_37FC      = 0;
					title->_00        = 13;
					title->mPrevState = title->mState;
					title->mState     = 25;
					title->mStep      = 0;
					fn_8001E904(SOUND(), 0x88, 0xFF);
					fn_8001E660(SOUND(), 0xFFFF0003);
					GAME_9108 = title->_37F8;
					return;
				}
				fn_80089C90(lbl_8025C400 + 0xE8400, title->_3804, title->_37F8, 1);
				title->mPrevState = title->mState;
				title->mState     = 8;
				title->mStep      = 0;
				fn_8001E904(SOUND(), 0x88, 0xFF);
			} else {
				fn_8001E904(SOUND(), 0x86, 0xFF);
			}
			return;
		}
	} else if (GetPadState(pad)->mTrigger & 0x200) {
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
	}
}

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

/**
 * @note Address: 0x8002C3F0
 * @note Size: 0x348
 */
// NONMATCHING
// waits for the card to read the file picked, then shows what is in it
extern "C" void fn_8002C3F0(Player* title, u32 frames)
{
	CardState* card = CARD;
	s32 step        = card->mStep;
	CardFile* file;
	s32 result;

	title->_37F4 -= frames;
	if (title->_37F4 < 0) {
		title->_37F4 = 0;
	}
	switch (step) {
	case 0:
		title->_37D0 = CARD_FREE1;
		title->_37D4 = CARD_FREE0;
		file         = &card->mFiles[title->_37F8];
		if (file->mUsed != 0) {
			lbl_80361E8C[title->_37F8].mUsed    = 1;
			lbl_80361E8C[title->_37F8]._04      = file->_90;
			lbl_80361E8C[title->_37F8]._08      = file->_98;
			lbl_80361E8C[title->_37F8]._0C      = file->_9C;
			lbl_80361E8C[title->_37F8]._10      = file->_A0;
			lbl_80361E8C[title->_37F8]._14      = file->_8C;
			lbl_80361E8C[title->_37F8]._18      = file->_A4;
			lbl_80361E8C[title->_37F8]._1C      = file->_94;
			lbl_80361E8C[title->_37F8].mHours   = file->mPlayTime / 216000;
			lbl_80361E8C[title->_37F8].mMinutes = (file->mPlayTime / 3600) % 60;
			lbl_80361E8C[title->_37F8]._28      = file->_7C;
			lbl_80361E8C[title->_37F8]._2C      = file->_80;
			lbl_80361E8C[title->_37F8]._40      = 0;
			if (file->_A8 > 0) {
				lbl_80361E8C[title->_37F8]._44 = 1;
			} else {
				lbl_80361E8C[title->_37F8]._44 = 0;
			}
		}
		title->mPrevState = title->mState;
		title->mState     = 24;
		title->mStep      = 0;
		break;
	case 4:
		break;
	case 1:
	case 2:
	case 3:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		result       = CARD_RESULT;
		title->_37D8 = result;
		switch (result) {
		case -128:
		case -14:
		case -13:
		case -12:
		case -11:
		case -10:
		case -9:
		case -8:
		case -7:
		case -6:
		case -5:
		case -4:
		case -3:
		case -2:
		case -1:
		case 0:
			if (step == 2 && (result == -6 || result == -13)) {
				title->mPrevState = title->mState;
				title->mState     = 11;
				title->mStep      = 0;
				title->_37FC      = 1;
				title->_37E1      = 1;
			} else if (result == -3 || result == -5) {
				title->mPrevState = title->mState;
				title->mState     = 20;
				title->mStep      = 0;
				title->_37E1      = 1;
			} else {
				title->mPrevState = title->mState;
				title->mState     = 14;
				title->mStep      = 0;
				title->_37CC      = -32.0f;
			}
			title->_37E8 = 1.0f;
			break;
		default:
			title->mPrevState = title->mState;
			title->mState     = 14;
			title->mStep      = 0;
			title->_37E8      = 1.0f;
			title->_37CC      = -32.0f;
			break;
		}
		break;
	}
}

/**
 * @note Address: 0x800236EC
 * @note Size: 0x1B8
 */
// starts a wipe from one rect to another over the given seconds, in one color
extern "C" void fn_800236EC(CScrnColor* scrn, f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2, f32 time, u8 r, u8 g, u8 b, u8 a, s32 mode)
{
	scrn->mActive = 1;
	scrn->mMode   = mode;
	scrn->_3C     = x0;
	scrn->_40     = y0;
	scrn->_44     = x1;
	scrn->_48     = y1;
	scrn->_4C     = x2;
	scrn->_50     = y2;
	if (0.0f == time) {
		scrn->_44 = x2;
		scrn->_48 = y2;
		scrn->_54 = scrn->_58 = 0.0f;
	} else {
		scrn->_54 = (x2 - x1) / (60.0f * time);
		scrn->_58 = (y2 - y1) / (60.0f * time);
	}
	scrn->mColor[0]  = r;
	scrn->mColor[1]  = g;
	scrn->mColor[2]  = b;
	scrn->mColor[3]  = a;
	scrn->mTarget[0] = r;
	scrn->mTarget[1] = g;
	scrn->mTarget[2] = b;
	scrn->mTarget[3] = a;
	scrn->mSpeed[0]  = r;
	scrn->mSpeed[1]  = g;
	scrn->mSpeed[2]  = b;
	scrn->mSpeed[3]  = a;
	if (scrn->mMode) {
		bool still = false;
		if (0.0f == scrn->_54 && 0.0f == scrn->_58) {
			still = true;
		}
		if (still) {
			scrn->mActive = 0;
		}
	}
}

/**
 * @note Address: 0x80023EE0
 * @note Size: 0x20C
 */
// fades from one color to another over the given seconds, at once for zero
extern "C" void fn_80023EE0(CScrnColor* scrn, f32 time, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1, u8 g1, u8 b1, u8 a1)
{
	scrn->mColor[0]  = r0;
	scrn->mColor[1]  = g0;
	scrn->mColor[2]  = b0;
	scrn->mColor[3]  = a0;
	scrn->mTarget[0] = r1;
	scrn->mTarget[1] = g1;
	scrn->mTarget[2] = b1;
	scrn->mTarget[3] = a1;
	if (time == 0.0f) {
		scrn->mColor[0] = r1;
		scrn->mColor[1] = g1;
		scrn->mColor[2] = b1;
		scrn->mColor[3] = a1;
		scrn->mSpeed[0] = 0.0f;
		scrn->mSpeed[1] = 0.0f;
		scrn->mSpeed[2] = 0.0f;
		scrn->mSpeed[3] = 0.0f;
	} else {
		scrn->mSpeed[0] = ((f32)r1 - (f32)r0) / (60.0f * time);
		scrn->mSpeed[1] = ((f32)g1 - (f32)g0) / (60.0f * time);
		scrn->mSpeed[2] = ((f32)b1 - (f32)b0) / (60.0f * time);
		scrn->mSpeed[3] = ((f32)a1 - (f32)a0) / (60.0f * time);
	}
}

/**
 * @note Address: 0x80023438
 * @note Size: 0x2B4
 */
// starts a wipe from one rect to another while fading from one color to another
extern "C" void fn_80023438(CScrnColor* scrn, f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2, f32 time, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1,
                            u8 g1, u8 b1, u8 a1, s32 mode)
{
	scrn->mActive = 1;
	scrn->mMode   = mode;
	scrn->_3C     = x0;
	scrn->_40     = y0;
	scrn->_44     = x1;
	scrn->_48     = y1;
	scrn->_4C     = x2;
	scrn->_50     = y2;
	if (0.0f == time) {
		scrn->_44 = x2;
		scrn->_48 = y2;
	} else {
		scrn->_54 = (x2 - x1) / (60.0f * time);
		scrn->_58 = (y2 - y1) / (60.0f * time);
	}
	scrn->mColor[0]  = r0;
	scrn->mColor[1]  = g0;
	scrn->mColor[2]  = b0;
	scrn->mColor[3]  = a0;
	scrn->mTarget[0] = r1;
	scrn->mTarget[1] = g1;
	scrn->mTarget[2] = b1;
	scrn->mTarget[3] = a1;
	if (time == 0.0f) {
		scrn->mColor[0] = r1;
		scrn->mColor[1] = g1;
		scrn->mColor[2] = b1;
		scrn->mColor[3] = a1;
		scrn->mSpeed[0] = 0.0f;
		scrn->mSpeed[1] = 0.0f;
		scrn->mSpeed[2] = 0.0f;
		scrn->mSpeed[3] = 0.0f;
	} else {
		scrn->mSpeed[0] = ((f32)r1 - (f32)r0) / (60.0f * time);
		scrn->mSpeed[1] = ((f32)g1 - (f32)g0) / (60.0f * time);
		scrn->mSpeed[2] = ((f32)b1 - (f32)b0) / (60.0f * time);
		scrn->mSpeed[3] = ((f32)a1 - (f32)a0) / (60.0f * time);
	}
	if (scrn->mMode) {
		bool still = false;
		if (0.0f == scrn->_54 && 0.0f == scrn->_58) {
			still = true;
		}
		if (still) {
			scrn->mActive = 0;
		}
	}
}

/**
 * @note Address: 0x80023030
 * @note Size: 0x408
 */
// a wipe's update: it turns, moves its rect and fades its color, and stops once the rect
// has stopped when it was asked to
extern "C" void fn_80023030(CScrnColor* scrn, u32 frames)
{
	if (!scrn->mActive) {
		return;
	}
	if (0.0f != scrn->_60) {
		scrn->_5C = fmod(scrn->_60 * frames + scrn->_5C, 6.2831855f);
	}
	StepChannel(scrn->_44, scrn->_4C, scrn->_54, frames);
	StepChannel(scrn->_48, scrn->_50, scrn->_58, frames);
	StepChannel(scrn->mColor[0], scrn->mTarget[0], scrn->mSpeed[0], frames);
	StepChannel(scrn->mColor[1], scrn->mTarget[1], scrn->mSpeed[1], frames);
	StepChannel(scrn->mColor[2], scrn->mTarget[2], scrn->mSpeed[2], frames);
	StepChannel(scrn->mColor[3], scrn->mTarget[3], scrn->mSpeed[3], frames);
	if (scrn->mMode) {
		bool still = false;
		if (0.0f == scrn->_54 && 0.0f == scrn->_58) {
			still = true;
		}
		if (still) {
			scrn->mActive = 0;
		}
	}
}

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

/**
 * @note Address: 0x8002E574
 * @note Size: 0x358
 */
// NONMATCHING
// waits for the card to list its files, then fills in the three save infos and picks the
// newest file
extern "C" void fn_8002E574(Player* title, u32 frames)
{
	s32 step = CARD->mStep;
	s32 result;

	title->_37F4 -= frames;
	if (title->_37F4 < 0) {
		title->_37F4 = 0;
	}
	switch (step) {
	case 0: {
		OSTime times[3];
		OSCalendarTime cal;
		CardFile* file;
		SaveInfo* info;
		for (s32 i = 0; i < 3; i++) {
			file = &CARD->mFiles[i];
			if (file->mUsed != 0) {
				info = &lbl_80361E8C[i];
				OSTicksToCalendarTime((OSTime)file->mSaveTime * OS_TIMER_CLOCK, &cal);
				info->mUsed    = 1;
				info->_04      = file->_90;
				info->_08      = file->_98;
				info->_0C      = file->_9C;
				info->_10      = file->_A0;
				info->_14      = file->_8C;
				info->_18      = file->_A4;
				info->_1C      = file->_94;
				info->mHours   = file->mPlayTime / 216000;
				info->mMinutes = (file->mPlayTime / 3600) % 60;
				info->_28      = file->_7C;
				info->_2C      = file->_80;
				if (file->_A8 > 0) {
					info->_44 = 1;
				} else {
					info->_44 = 0;
				}
				times[i] = OSCalendarTimeToTicks(&cal);
				if (file->_04 != 0 || !fn_80086F24(file)) {
					info->_40 = 1;
				} else {
					info->_40 = 0;
				}
			}
		}
		title->_37D0      = CARD_FREE1;
		title->_37D4      = CARD_FREE0;
		title->mPrevState = title->mState;
		title->mState     = 2;
		title->mStep      = 0;
		if (title->_37A8 == 0) {
			title->_37F8 = 0;
		} else {
			title->_37F8 = fn_80035184(times[0], times[1], times[2]);
		}
		title->_37E8 = 1.0f;
		break;
	}
	case 4:
		break;
	case 1:
	case 2:
	case 3:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		result       = CARD_RESULT;
		title->_37D8 = result;
		switch (result) {
		case -128:
		case -14:
		case -13:
		case -12:
		case -11:
		case -10:
		case -9:
		case -8:
		case -7:
		case -6:
		case -5:
		case -4:
		case -3:
		case -2:
		case -1:
		case 0:
			if (result == -6 || result == -13) {
				title->mPrevState = title->mState;
				title->mState     = 11;
				title->mStep      = 0;
				title->_37FC      = 1;
			} else if (result == -5) {
				title->mPrevState = title->mState;
				title->mState     = 19;
				title->mStep      = 0;
				title->_37E1      = 1;
			} else {
				title->mPrevState = title->mState;
				title->mState     = 14;
				title->mStep      = 0;
				title->_37CC      = -32.0f;
			}
			title->_37E8 = 1.0f;
			break;
		default:
			title->mPrevState = title->mState;
			title->mState     = 14;
			title->mStep      = 0;
			title->_37E8      = 1.0f;
			title->_37CC      = -32.0f;
			break;
		}
		break;
	}
}
