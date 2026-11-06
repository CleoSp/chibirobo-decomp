#include "Version.h"
extern "C" {
extern const char sCarJObjHeader[];
extern const char sCarJObjExpression[];
}
#define JOBJ_H_ASSERT_JOINT(line, joint) ((joint) ? ((void)0) : __assert(sCarJObjHeader, line, sCarJObjExpression))
#include "game/hsd_jobj.h"
#include "rel/stage02/cameraaction.h"

extern "C" {
void PSMTXCopy(Mtx src, Mtx dst);
void PSMTXConcat(Mtx a, Mtx b, Mtx ab);
void PSMTXScale(Mtx m, f32 xS, f32 yS, f32 zS);
void PSMTXTrans(Mtx m, f32 xT, f32 yT, f32 zT);
void PSMTXMultVec(Mtx m, Vec* src, Vec* dst);
}

// CGame, accessed by offset
extern "C" u8 gGame[];
#define GAME_AT(off) (gGame + (off))

// loaded model object, vtable at 0x34
class XObj {
public:
	HSD_JObj* jobj; // _00
	u8 _04[0x30];   // _04
	virtual void vf08();
	virtual void vf0C();
	virtual void vf10();
	virtual void vf14();
	virtual void vf18();
	virtual void vf1C();
	virtual void vf20();
	virtual void vf24();
	virtual void vf28();
	virtual void vf2C();
	virtual void vf30();
	virtual void vf34();
	virtual void vf38();
	virtual void vf3C();
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
	virtual void vf68();
	virtual void vf6C();
	virtual void vf70();
	virtual void vf74();
	virtual void vf78();
	virtual void vf7C();
	virtual void SetAnim(s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate, f32 blend,
	                     f32 blendRate);
#if defined(VERSION_JP)
	u8 _38[0x88]; // _38
#else
	u8 _38[0x90]; // _38
#endif
	u8 _C8[0x90];   // _C8 (JP: _C0)
	s32 _158;       // _158 (JP: _150)
	u8 _15C[0xCC];  // _15C (JP: _154)
	u64 _228;       // _228 (JP: _220)
	u8 _230[0x8];   // _230 (JP: _228)
	s32* _238;      // _238 (JP: _230)
	u8 _23C[0x5D4]; // _23C (JP: _234)
	s32 _810;       // _810 (JP: _808)
	struct {
		s32 type; // _00
		s16 _04;  // _04
		s16 _06;  // _06
	} _814[16];   // _814 (JP: _80C)
};

namespace Car {

class CCameraActionCar : public CInterfaceCameraAction {
public:
	CCameraActionCar()
	{
		_40   = NULL;
		_44.x = _44.y = _44.z = 0.0f;
		_50.x = _50.y = _50.z = 0.0f;
	}
	virtual ~CCameraActionCar();
	virtual void Update();
	virtual s32 GetMode();
	virtual s32 GetFlags();
	virtual f32 GetEaseDivisor();

	void SetTarget(Vec pos) { _50 = pos; }
	void SetOffset(Vec pos) { _44 = pos; }

	MtxPtr _40;  // _40
	Vec _44;     // _44
	Vec _50;     // _50
	u8 _5C[0xC]; // _5C
};

class CarRoot {
public:
	virtual void Update(u32 arg);
	virtual void Brake();
	virtual void Start();

	void Register(XObj* a, XObj* b, XObj* c);

	XObj* _04;          // _04
	XObj* _08;          // _08
	XObj* _0C;          // _0C
	f32 mFrame;         // _10
	f32 mPreviousFrame; // _14
	f32 mPlaybackRate;  // _18
	Mtx _1C;            // _1C
	Mtx _4C;            // _4C
	u8 mBraking;        // _7C
	u8 mStarted;        // _7D
};

class PlayerCarRoot : public CarRoot {
public:
	virtual void Update(u32 arg);
	virtual void Start();
	virtual void Clear();

	f32 mBrakeFrame; // _80
};

// prints "Car::Init"
void Init(void);

} // namespace Car

struct RoboView {
#if defined(VERSION_JP)
	u8 _00[0x970];
#else
	u8 _00[0x980];
#endif
	u32 mFlags;
#if defined(VERSION_JP)
	u8 _974[0x12CF4 - 0x974];
#else
	u8 _984[0x12DF4 - 0x984];
#endif
	BOOL mPadReset;
};

#if defined(VERSION_JP)
#define GAME_OBJ()              (*(RoboView**)GAME_AT(0x9674))
#define MOTION_CAMERA()         ((CMotionControlCamera*)GAME_AT(0xE5050))
#define DEFAULT_CAMERA_ACTION() (*(CInterfaceCameraAction**)GAME_AT(0xE5224))
#define SOUND()                 GAME_AT(0xE713C)
#else
#define GAME_OBJ()              (*(RoboView**)GAME_AT(0x96BC))
#define MOTION_CAMERA()         ((CMotionControlCamera*)GAME_AT(0xE5D38))
#define DEFAULT_CAMERA_ACTION() (*(CInterfaceCameraAction**)GAME_AT(0xE5F0C))
#define SOUND()                 GAME_AT(0xE7E24)
#endif

extern "C" {
extern const char sCarAObjHeader[];
extern const char sCarAObjExpression[];
extern const char sCarInitMessage[];
extern const char sCarRegisterMessage[];
extern const char sCarTerminateMessage[];
extern const float sCarZero;
extern const float sCarAnimBlendRate;
extern const float sCarFrameHundredths;
extern const float sCarTrailHeight;
extern const float sCarTargetHeight;
extern const float sCarTargetDepth;
extern const float sCarCameraDepth;
extern const float sCarModelScale;
extern const float sCarAttachmentX;
extern const float sCarAttachmentY;
extern const float sCarAttachmentZ;
extern const float sCarBrakeRateDivisor;
extern const float sCarCameraFov;
extern const float sCarCameraEaseDivisor;
}
Car::CCameraActionCar carCamera;

// see game/cmotioncontrolcamera.h
class CMotionControlCamera {
public:
	void ChangeAction(CInterfaceCameraAction* action);
};

Car::PlayerCarRoot playerCar;
Car::CarRoot racerCar;
u8 carsStarted;
Vec carCameraTrail[32];
u32 carCameraTrailIndex;
u8* carParams;
u8* carSoundCues;

extern "C" {
s32 IsGameSoundPlaying(void* p, s32 id);
void PlayGameSound(void* p, s32 id, s32 v);
void StopGameSfx(void* p, s32 id);
void FadeOutGameSfx(void* p, s32 a, s32 b);

void RestoreDefaultCamera(void);
void Terminate(void);
s32 GetPlayerCarFrame(void);
s32 GetPlayerCarResult(void);
void SetCarSoundCues(u8* data);
void SetCarParams(u8* data);
void DrawCarHud(void* arg);
void UpdateCars(u32 arg);
void StartCars(void);
void StartCarCamera(void);
s32 RegisterCar(XObj* a, XObj* b, XObj* c);
}

// little endian ints out of a loaded data file
inline s32 ReadS32LE(u8* p)
{
	if (p != NULL) {
		return p[0] + p[1] * 0x100 + p[2] * 0x10000 + p[3] * 0x1000000;
	}
	return 0;
}

// todo: copied inline, clean up
inline f32 HSD_AObjGetEndFrame(HSD_AObj* aobj)
{
	((aobj) ? ((void)0) : __assert(sCarAObjHeader, 171, sCarAObjExpression));
	return aobj->end_frame;
}

inline void Car::CarRoot::Register(XObj* a, XObj* b, XObj* c)
{
#if !defined(VERSION_PAL)
	OSReport(sCarRegisterMessage);
#endif
	_04 = a;
	_08 = b;
	_0C = c;
	a->_228 |= 0x10000;
	_08->_228 &= ~0x10000ULL;
	mFrame         = 0.0f;
	mPreviousFrame = 0.0f;
	mPlaybackRate  = 1.0f;
	mBraking       = 0;
	mStarted       = 0;
}

// inline virtuals in the original, so direct calls get inlined
inline void CarRoot_Brake(Car::CarRoot* car)
{
	s32 i;
	XObj* obj;

	car->mBraking = 1;
	car->_04->SetAnim(2, 0, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, sCarAnimBlendRate);
	obj = car->_04;
	// takes the first free slot in the request ring
	for (i = 0; i < 16; i++) {
		s32 idx = (obj->_810 + i) % 16;
		if (obj->_814[idx].type == -1) {
			obj->_814[idx].type = 3;
			obj->_814[idx]._04  = 0;
			obj->_814[idx]._06  = 100;
			break;
		}
	}
}

inline void CarRoot_Start(Car::CarRoot* car)
{
	car->mStarted = 1;
}

inline void PlayerCarRoot_Start(Car::PlayerCarRoot* car)
{
	car->mStarted = 1;
	PlayGameSound(SOUND(), 0x2000D, 0xFF);
}

inline void RoboView_ResetFlags(RoboView* game)
{
	game->mFlags = 0;
	game->mFlags |= 0x20000000;
	game->mFlags |= 7;
	game->mPadReset = 1;
}

// body of Car::Init, inlined into Terminate
inline void Car_Init(void)
{
#if !defined(VERSION_PAL)
	OSReport(sCarInitMessage);
#endif
	playerCar._08         = NULL;
	playerCar._04         = NULL;
	playerCar.mBrakeFrame = sCarZero;
	racerCar._08          = NULL;
	racerCar._04          = NULL;
	carsStarted           = 0;
	carCameraTrailIndex   = 0;
	carParams             = NULL;
}

/**
 * @note Address: stage02 .text 0x6E5C
 * @note Size: 0xCC
 */
#pragma section RX ".text.carcontrol"
extern "C" __declspec(section ".text.carcontrol") void RestoreDefaultCamera(void)
{
	HSD_JObjClearMtxFlags(((XObj*)GAME_OBJ())->jobj);
	MOTION_CAMERA()->ChangeAction(DEFAULT_CAMERA_ACTION());
}

/**
 * @note Address: stage02 .text 0x6F28
 * @note Size: 0x3DC
 */
extern "C" __declspec(section ".text.carcontrol") void Terminate(void)
{
#if !defined(VERSION_PAL)
	OSReport(sCarTerminateMessage);
#endif
	HSD_JObjClearMtxFlags(playerCar._04->jobj);
	if (playerCar._0C != NULL) {
		HSD_JObjClearMtxFlags(playerCar._0C->jobj);
	}
	playerCar.mStarted = 0;
	playerCar.mBraking = 0;
	HSD_JObjClearMtxFlags(racerCar._04->jobj);
	if (racerCar._0C != NULL) {
		HSD_JObjClearMtxFlags(racerCar._0C->jobj);
	}
	racerCar.mStarted = 0;
	racerCar.mBraking = 0;
	RoboView_ResetFlags(GAME_OBJ());
	HSD_JObjClearMtxFlags(((XObj*)GAME_OBJ())->jobj);
	MOTION_CAMERA()->ChangeAction(DEFAULT_CAMERA_ACTION());
	Car_Init();
}

// Terminate
/*
    stwu     r1, -0x20(r1)
    mflr     r0
    lis      r3, lbl_5_rodata_244@ha
    stw      r0, 0x24(r1)
    addi     r3, r3, lbl_5_rodata_244@l
    stw      r31, 0x1c(r1)
    stw      r30, 0x18(r1)
    stw      r29, 0x14(r1)
    crclr    cr1eq
    bl       OSReport
    lis      r3, playerCar@ha
    addi     r31, r3, playerCar@l
    lwz      r3, 0x4(r31)
    lwz      r30, 0x0(r3)
    cmplwi   r30, 0x0
    bne      .L_00006F80
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_23C@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x4af
    addi     r5, r5, lbl_5_rodata_23C@l
    bl       __assert
.L_00006F80:
    lwz      r0, 0x14(r30)
    cmplwi   r30, 0x0
    rlwinm   r0, r0, 0, 9, 5
    stw      r0, 0x14(r30)
    beq      .L_00006FDC
    bne      .L_00006FB0
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_23C@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x25d
    addi     r5, r5, lbl_5_rodata_23C@l
    bl       __assert
.L_00006FB0:
    lwz      r4, 0x14(r30)
    li       r3, 0x0
    rlwinm.  r0, r4, 0, 8, 8
    bne      .L_00006FCC
    rlwinm.  r0, r4, 0, 25, 25
    beq      .L_00006FCC
    li       r3, 0x1
.L_00006FCC:
    clrlwi.  r0, r3, 24
    bne      .L_00006FDC
    mr       r3, r30
    bl       HSD_JObjSetMtxDirtySub
.L_00006FDC:
    lis      r3, playerCar@ha
    addi     r3, r3, playerCar@l
    lwz      r3, 0xc(r3)
    cmplwi   r3, 0x0
    beq      .L_00007070
    lwz      r30, 0x0(r3)
    cmplwi   r30, 0x0
    bne      .L_00007014
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_23C@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x4af
    addi     r5, r5, lbl_5_rodata_23C@l
    bl       __assert
.L_00007014:
    lwz      r0, 0x14(r30)
    cmplwi   r30, 0x0
    rlwinm   r0, r0, 0, 9, 5
    stw      r0, 0x14(r30)
    beq      .L_00007070
    bne      .L_00007044
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_23C@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x25d
    addi     r5, r5, lbl_5_rodata_23C@l
    bl       __assert
.L_00007044:
    lwz      r4, 0x14(r30)
    li       r3, 0x0
    rlwinm.  r0, r4, 0, 8, 8
    bne      .L_00007060
    rlwinm.  r0, r4, 0, 25, 25
    beq      .L_00007060
    li       r3, 0x1
.L_00007060:
    clrlwi.  r0, r3, 24
    bne      .L_00007070
    mr       r3, r30
    bl       HSD_JObjSetMtxDirtySub
.L_00007070:
    lis      r4, playerCar@ha
    lis      r3, racerCar@ha
    addi     r30, r3, racerCar@l
    li       r0, 0x0
    addi     r4, r4, playerCar@l
    lwz      r3, 0x4(r30)
    stb      r0, 0x7d(r4)
    stb      r0, 0x7c(r4)
    lwz      r29, 0x0(r3)
    cmplwi   r29, 0x0
    bne      .L_000070B4
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_23C@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x4af
    addi     r5, r5, lbl_5_rodata_23C@l
    bl       __assert
.L_000070B4:
    lwz      r0, 0x14(r29)
    cmplwi   r29, 0x0
    rlwinm   r0, r0, 0, 9, 5
    stw      r0, 0x14(r29)
    beq      .L_00007110
    bne      .L_000070E4
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_23C@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x25d
    addi     r5, r5, lbl_5_rodata_23C@l
    bl       __assert
.L_000070E4:
    lwz      r4, 0x14(r29)
    li       r3, 0x0
    rlwinm.  r0, r4, 0, 8, 8
    bne      .L_00007100
    rlwinm.  r0, r4, 0, 25, 25
    beq      .L_00007100
    li       r3, 0x1
.L_00007100:
    clrlwi.  r0, r3, 24
    bne      .L_00007110
    mr       r3, r29
    bl       HSD_JObjSetMtxDirtySub
.L_00007110:
    lis      r3, racerCar@ha
    addi     r3, r3, racerCar@l
    lwz      r3, 0xc(r3)
    cmplwi   r3, 0x0
    beq      .L_000071A4
    lwz      r29, 0x0(r3)
    cmplwi   r29, 0x0
    bne      .L_00007148
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_23C@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x4af
    addi     r5, r5, lbl_5_rodata_23C@l
    bl       __assert
.L_00007148:
    lwz      r0, 0x14(r29)
    cmplwi   r29, 0x0
    rlwinm   r0, r0, 0, 9, 5
    stw      r0, 0x14(r29)
    beq      .L_000071A4
    bne      .L_00007178
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_23C@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x25d
    addi     r5, r5, lbl_5_rodata_23C@l
    bl       __assert
.L_00007178:
    lwz      r4, 0x14(r29)
    li       r3, 0x0
    rlwinm.  r0, r4, 0, 8, 8
    bne      .L_00007194
    rlwinm.  r0, r4, 0, 25, 25
    beq      .L_00007194
    li       r3, 0x1
.L_00007194:
    clrlwi.  r0, r3, 24
    bne      .L_000071A4
    mr       r3, r29
    bl       HSD_JObjSetMtxDirtySub
.L_000071A4:
    lis      r3, gGame@ha
    lis      r4, racerCar@ha
    addi     r3, r3, gGame@l
    li       r5, 0x0
    addi     r4, r4, racerCar@l
    li       r0, 0x1
    addis    r7, r3, 0x1
    stb      r5, 0x7d(r4)
    lwz      r6, -0x6944(r7)
    stb      r5, 0x7c(r4)
    addis    r3, r6, 0x1
    stw      r5, 0x980(r6)
    lwz      r4, 0x980(r6)
    oris     r4, r4, 0x2000
    stw      r4, 0x980(r6)
    lwz      r4, 0x980(r6)
    ori      r4, r4, 0x7
    stw      r4, 0x980(r6)
    stw      r0, 0x2df4(r3)
    lwz      r3, -0x6944(r7)
    lwz      r29, 0x0(r3)
    cmplwi   r29, 0x0
    bne      .L_00007218
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_23C@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x4af
    addi     r5, r5, lbl_5_rodata_23C@l
    bl       __assert
.L_00007218:
    lwz      r0, 0x14(r29)
    cmplwi   r29, 0x0
    rlwinm   r0, r0, 0, 9, 5
    stw      r0, 0x14(r29)
    beq      .L_00007274
    bne      .L_00007248
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_23C@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x25d
    addi     r5, r5, lbl_5_rodata_23C@l
    bl       __assert
.L_00007248:
    lwz      r4, 0x14(r29)
    li       r3, 0x0
    rlwinm.  r0, r4, 0, 8, 8
    bne      .L_00007264
    rlwinm.  r0, r4, 0, 25, 25
    beq      .L_00007264
    li       r3, 0x1
.L_00007264:
    clrlwi.  r0, r3, 24
    bne      .L_00007274
    mr       r3, r29
    bl       HSD_JObjSetMtxDirtySub
.L_00007274:
    lis      r3, gGame@ha
    addi     r3, r3, gGame@l
    addis    r3, r3, 0xe
    lwz      r4, 0x5f0c(r3)
    addi     r3, r3, 0x5d38
    bl       fn_801494A4
    lis      r3, lbl_5_rodata_250@ha
    addi     r3, r3, lbl_5_rodata_250@l
    crclr    cr1eq
    bl       OSReport
    lis      r3, lbl_5_rodata_25C@ha
    li       r0, 0x0
    addi     r7, r3, lbl_5_rodata_25C@l
    lis      r8, playerCar@ha
    lis      r6, racerCar@ha
    lfs      f0, 0x0(r7)
    addi     r8, r8, playerCar@l
    lis      r5, carsStarted@ha
    lis      r4, carCameraTrailIndex@ha
    lis      r3, carParams@ha
    addi     r6, r6, racerCar@l
    stw      r0, 0x8(r8)
    stw      r0, 0x4(r31)
    stfs     f0, 0x80(r8)
    stw      r0, 0x8(r6)
    stw      r0, 0x4(r30)
    stb      r0, carsStarted@l(r5)
    stw      r0, carCameraTrailIndex@l(r4)
    stw      r0, carParams@l(r3)
    lwz      r31, 0x1c(r1)
    lwz      r30, 0x18(r1)
    lwz      r0, 0x24(r1)
    lwz      r29, 0x14(r1)
    mtlr     r0
    addi     r1, r1, 0x20
    blr
*/

/**
 * @note Address: stage02 .text 0x7304
 * @note Size: 0x30
 */
extern "C" __declspec(section ".text.carcontrol") s32 GetPlayerCarFrame(void)
{
	return sCarFrameHundredths * playerCar.mFrame;
}

/**
 * @note Address: stage02 .text 0x7334
 * @note Size: 0x144
 */
extern "C" __declspec(section ".text.carcontrol") s32 GetPlayerCarResult(void)
{
	CPad pad(0);
	HSD_AObj* aobj;
	f32 end;

	if (playerCar.mBraking && playerCar.mPlaybackRate <= sCarZero) {
		return 1;
	}
	aobj = GetAObj(playerCar._08->jobj);
	end  = aobj != NULL ? HSD_AObjGetEndFrame(aobj) : sCarZero;
	if (end <= playerCar.mFrame) {
		return 2;
	}
	return 0;
}

/**
 * @note Address: stage02 .text 0x7478
 * @note Size: 0xC
 */
extern "C" __declspec(section ".text.carcontrol") void SetCarSoundCues(u8* data)
{
	carSoundCues = data;
}

/**
 * @note Address: stage02 .text 0x7484
 * @note Size: 0xC
 */
extern "C" __declspec(section ".text.carcontrol") void SetCarParams(u8* data)
{
	carParams = data;
}

/**
 * @note Address: stage02 .text 0x7490
 * @note Size: 0x4
 */
extern "C" __declspec(section ".text.carcontrol") void DrawCarHud(void* arg)
{
}

/**
 * @note Address: stage02 .text 0x7494
 * @note Size: 0x398
 */
extern "C" __declspec(section ".text.carcontrol") void UpdateCars(u32 arg)
{
	CPad pad(0);
	bool done;

	if (!carsStarted) {
		arg = 0;
	}
	if (playerCar._04 == NULL) {
		return;
	}
	if (racerCar._04 == NULL) {
		return;
	}
	playerCar.Update(arg);
	racerCar.Update(arg);
	if (!playerCar.mStarted) {
		done = false;
	} else {
		done = ReadS32LE(carParams + 4) <= playerCar.mFrame;
	}
	if (done) {
		carCamera._40 = NULL;
	} else {
		Vec pos;
		s32 prev;
		pos.y = 0.0f;
		pos.z = 0.0f;
		pos.x = 0.0f;
		pos.y = sCarTrailHeight;
		PSMTXMultVec(playerCar._1C, &pos, &pos);
		carCameraTrail[carCameraTrailIndex] = pos;
		prev                                = carCameraTrailIndex - 10;
		if (prev < 0) {
			prev += 32;
		}
		CInterfaceCameraAction* cam = &carCamera;
		cam->SetEye(carCameraTrail[prev].x, carCameraTrail[prev].y, carCameraTrail[prev].z);
		if (++carCameraTrailIndex >= 32) {
			carCameraTrailIndex = 0;
		}
	}
	if (!racerCar.mBraking && ReadS32LE(carParams + 0xC) <= racerCar.mFrame) {
		CarRoot_Brake(&racerCar);
	}
}

// UpdateCars
/*
    stwu     r1, -0x30(r1)
    mflr     r0
    li       r4, 0x0
    stw      r0, 0x34(r1)
    stw      r31, 0x2c(r1)
    stw      r30, 0x28(r1)
    mr       r30, r3
    addi     r3, r1, 0x8
    bl       __ct__4CPadFUc
    lis      r3, carsStarted@ha
    lbz      r0, carsStarted@l(r3)
    cmplwi   r0, 0x0
    bne      .L_000074CC
    li       r30, 0x0
.L_000074CC:
    lis      r3, playerCar@ha
    addi     r3, r3, playerCar@l
    lwz      r0, 0x4(r3)
    cmplwi   r0, 0x0
    bne      .L_000074F0
    lis      r3, __vt__4CPad@ha
    addi     r0, r3, __vt__4CPad@l
    stw      r0, 0xc(r1)
    b        .L_00007814
.L_000074F0:
    lis      r4, racerCar@ha
    addi     r31, r4, racerCar@l
    lwz      r0, 0x4(r31)
    cmplwi   r0, 0x0
    bne      .L_00007514
    lis      r3, __vt__4CPad@ha
    addi     r0, r3, __vt__4CPad@l
    stw      r0, 0xc(r1)
    b        .L_00007814
.L_00007514:
    mr       r4, r30
    bl       Update__Q23Car13PlayerCarRootFUl
    lis      r3, racerCar@ha
    mr       r4, r30
    addi     r3, r3, racerCar@l
    bl       Update__Q23Car7CarRootFUl
    lis      r3, playerCar@ha
    addi     r3, r3, playerCar@l
    lbz      r0, 0x7d(r3)
    cmplwi   r0, 0x0
    bne      .L_00007548
    li       r0, 0x0
    b        .L_000075C8
.L_00007548:
    lis      r3, carParams@ha
    addi     r3, r3, carParams@l
    lwz      r3, 0x0(r3)
    addic.   r4, r3, 0x4
    beq      .L_00007588
    lbz      r3, 0x1(r4)
    lbz      r0, 0x2(r4)
    lbz      r5, 0x3(r4)
    slwi     r3, r3, 8
    slwi     r0, r0, 16
    lbz      r4, 0x0(r4)
    slwi     r5, r5, 24
    add      r0, r3, r0
    add      r0, r0, r5
    add      r0, r4, r0
    b        .L_0000758C
.L_00007588:
    li       r0, 0x0
.L_0000758C:
    xoris    r3, r0, 0x8000
    lis      r0, 0x4330
    stw      r3, 0x24(r1)
    lis      r4, lbl_5_rodata_280@ha
    lis      r3, playerCar@ha
    lfd      f2, lbl_5_rodata_280@l(r4)
    stw      r0, 0x20(r1)
    addi     r3, r3, playerCar@l
    lfs      f0, 0x10(r3)
    lfd      f1, 0x20(r1)
    fsubs    f1, f1, f2
    fcmpo    cr0, f1, f0
    cror     eq, lt, eq
    mfcr     r0
    extrwi   r0, r0, 1, 2
.L_000075C8:
    clrlwi.  r0, r0, 24
    beq      .L_000075E4
    lis      r3, carCamera@ha
    li       r0, 0x0
    addi     r3, r3, carCamera@l
    stw      r0, 0x40(r3)
    b        .L_000076BC
.L_000075E4:
    lis      r3, lbl_5_rodata_25C@ha
    lis      r4, lbl_5_rodata_274@ha
    addi     r5, r3, lbl_5_rodata_25C@l
    lis      r3, playerCar@ha
    lfs      f1, 0x0(r5)
    addi     r6, r4, lbl_5_rodata_274@l
    addi     r3, r3, playerCar@l
    addi     r4, r1, 0x10
    stfs     f1, 0x14(r1)
    mr       r5, r4
    lfs      f0, 0x0(r6)
    addi     r3, r3, 0x1c
    stfs     f1, 0x18(r1)
    stfs     f1, 0x10(r1)
    stfs     f0, 0x14(r1)
    bl       PSMTXMultVec
    lis      r4, carCameraTrailIndex@ha
    lis      r3, carCameraTrail@ha
    addi     r4, r4, carCameraTrailIndex@l
    lfs      f2, 0x10(r1)
    lwz      r4, 0x0(r4)
    addi     r0, r3, carCameraTrail@l
    lfs      f1, 0x14(r1)
    mulli    r3, r4, 0xc
    lfs      f0, 0x18(r1)
    subic.   r6, r4, 0xa
    add      r3, r0, r3
    stfs     f2, 0x0(r3)
    stfs     f1, 0x4(r3)
    stfs     f0, 0x8(r3)
    bge      .L_00007664
    addi     r6, r6, 0x20
.L_00007664:
    lis      r3, carCamera@ha
    lis      r4, carCameraTrail@ha
    addi     r3, r3, carCamera@l
    lwz      r12, 0x3c(r3)
    mulli    r0, r6, 0xc
    addi     r5, r4, carCameraTrail@l
    lwz      r12, 0x2c(r12)
    add      r4, r5, r0
    lfsx     f1, r5, r0
    lfs      f2, 0x4(r4)
    lfs      f3, 0x8(r4)
    mtctr    r12
    bctrl
    lis      r3, carCameraTrailIndex@ha
    addi     r4, r3, carCameraTrailIndex@l
    lwz      r3, 0x0(r4)
    addi     r0, r3, 0x1
    cmplwi   r0, 0x20
    stw      r0, 0x0(r4)
    blt      .L_000076BC
    li       r0, 0x0
    stw      r0, 0x0(r4)
.L_000076BC:
    lis      r3, racerCar@ha
    addi     r6, r3, racerCar@l
    lbz      r0, 0x7c(r6)
    cmplwi   r0, 0x0
    bne      .L_00007808
    lis      r3, carParams@ha
    addi     r3, r3, carParams@l
    lwz      r3, 0x0(r3)
    addic.   r4, r3, 0xc
    beq      .L_00007710
    lbz      r3, 0x1(r4)
    lbz      r0, 0x2(r4)
    lbz      r5, 0x3(r4)
    slwi     r3, r3, 8
    slwi     r0, r0, 16
    lbz      r4, 0x0(r4)
    slwi     r5, r5, 24
    add      r0, r3, r0
    add      r0, r0, r5
    add      r0, r4, r0
    b        .L_00007714
.L_00007710:
    li       r0, 0x0
.L_00007714:
    xoris    r3, r0, 0x8000
    lis      r0, 0x4330
    stw      r3, 0x24(r1)
    lis      r4, lbl_5_rodata_280@ha
    lis      r3, racerCar@ha
    lfd      f2, lbl_5_rodata_280@l(r4)
    stw      r0, 0x20(r1)
    addi     r3, r3, racerCar@l
    lfs      f0, 0x10(r3)
    lfd      f1, 0x20(r1)
    fsubs    f1, f1, f2
    fcmpo    cr0, f1, f0
    cror     eq, lt, eq
    bne      .L_00007808
    li       r0, 0x1
    lis      r3, lbl_5_rodata_25C@ha
    addi     r4, r3, lbl_5_rodata_25C@l
    stb      r0, 0x7c(r6)
    lwz      r3, 0x4(r31)
    lis      r5, lbl_5_rodata_278@ha
    lfs      f1, 0x0(r4)
    lis      r4, lbl_5_rodata_27C@ha
    lwz      r12, 0x34(r3)
    addi     r6, r4, lbl_5_rodata_27C@l
    lfs      f2, lbl_5_rodata_278@l(r5)
    fmr      f3, f1
    lwz      r12, 0x80(r12)
    li       r4, 0x2
    lfs      f4, 0x0(r6)
    li       r5, 0x0
    li       r6, 0x0
    li       r7, 0x0
    li       r8, 0x0
    mtctr    r12
    bctrl
    li       r0, 0x10
    lwz      r4, 0x4(r31)
    li       r5, 0x0
    mtctr    r0
.L_000077B0:
    lwz      r0, 0x810(r4)
    add      r3, r0, r5
    slwi     r0, r3, 28
    srwi     r3, r3, 31
    subf     r0, r3, r0
    rotlwi   r0, r0, 4
    add      r0, r0, r3
    slwi     r3, r0, 3
    addi     r0, r3, 0x814
    lwzx     r0, r4, r0
    cmpwi    r0, -0x1
    bne      .L_00007800
    add      r4, r4, r3
    li       r0, 0x3
    stw      r0, 0x814(r4)
    li       r3, 0x0
    li       r0, 0x64
    sth      r3, 0x818(r4)
    sth      r0, 0x81a(r4)
    b        .L_00007808
.L_00007800:
    addi     r5, r5, 0x1
    bdnz     .L_000077B0
.L_00007808:
    lis      r3, __vt__4CPad@ha
    addi     r0, r3, __vt__4CPad@l
    stw      r0, 0xc(r1)
.L_00007814:
    lwz      r0, 0x34(r1)
    lwz      r31, 0x2c(r1)
    lwz      r30, 0x28(r1)
    mtlr     r0
    addi     r1, r1, 0x30
    blr
*/

/**
 * @note Address: stage02 .text 0x782C
 * @note Size: 0x64
 */
extern "C" __declspec(section ".text.carcontrol") void StartCars(void)
{
	carsStarted = 1;
	PlayerCarRoot_Start(&playerCar);
	CarRoot_Start(&racerCar);
}

/**
 * @note Address: stage02 .text 0x7890
 * @note Size: 0x10C
 */
extern "C" __declspec(section ".text.carcontrol") void StartCarCamera(void)
{
	RoboView* game = GAME_OBJ();
	Vec v;

	game->mFlags = 0;
	game->mFlags |= 0x40000000;
	game->mPadReset = 0;
	MOTION_CAMERA()->ChangeAction(&carCamera);
	v.x = 0.0f;
	v.y = sCarTargetHeight;
	v.z = sCarTargetDepth;
	carCamera.SetTarget(v);
	v.x = 0.0f;
	v.y = 0.0f;
	v.z = sCarCameraDepth;
	carCamera.SetOffset(v);
	carCamera._40 = playerCar._1C;
}

/**
 * @note Address: stage02 .text 0x799C
 * @note Size: 0x19C
 */
extern "C" __declspec(section ".text.carcontrol") s32 RegisterCar(XObj* a, XObj* b, XObj* c)
{
	if (playerCar._04 == NULL) {
		playerCar.Register(a, b, c);
		return 1;
	}
	if (racerCar._04 == NULL) {
		racerCar.Register(a, b, c);
		return 2;
	}
	return 0;
}

/**
 * @note Address: stage02 .text 0x7B38
 * @note Size: 0x78
 */
__declspec(section ".text.carcontrol") void Car::Init(void)
{
	Car_Init();
}

/**
 * @note Address: stage02 .text 0x7BB0
 * @note Size: 0x3A8
 */
__declspec(section ".text.carcontrol") void Car::PlayerCarRoot::Update(u32 arg)
{
	s32 i;

	CarRoot::Update(arg);
	if (!mStarted) {
		return;
	}
	CPad pad(0);
	void* sound;
	if (!mBraking && (pad.mPad->mTrigger & 0x200)) {
		Brake();
		mBrakeFrame = mFrame;
		sound       = SOUND();
		StopGameSfx(sound, 0x2000D);
		if (mBrakeFrame < ReadS32LE(carParams + 8)) {
			PlayGameSound(sound, 0x2000E, 0xFF);
		} else {
			PlayGameSound(sound, 0x20011, 0xFF);
		}
	}
	sound = SOUND();
	for (i = 0; ReadS32LE(carSoundCues + i * 8) > 0; i++) {
		s32 frame = ReadS32LE(carSoundCues + i * 8);
		s32 id    = ReadS32LE(carSoundCues + (i * 2 + 1) * 4);
		if (mPreviousFrame < frame && frame <= mFrame) {
			PlayGameSound(sound, id, 0xFF);
			if (id == 0x2001F) {
				FadeOutGameSfx(sound, -0x10000, 20);
			} else if (id == 0x2000F) {
				StopGameSfx(sound, 0x2001F);
			}
		}
	}
	if (ReadS32LE(carParams + 4) <= mFrame) {
		mPlaybackRate = 1.0f;
		if (mBraking && _04->_158 != 4) {
			_04->SetAnim(4, 0, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, sCarAnimBlendRate);
		}
	}
}

/**
 * @note Address: stage02 .text 0x7F58
 * @note Size: 0x44
 */
__declspec(section ".text.carcontrol") void Car::PlayerCarRoot::Start()
{
	PlayerCarRoot_Start(this);
}

/**
 * @note Address: stage02 .text 0x7F9C
 * @note Size: 0x1C
 */
__declspec(section ".text.carcontrol") void Car::PlayerCarRoot::Clear()
{
	_08         = NULL;
	_04         = NULL;
	mBrakeFrame = 0.0f;
}

/**
 * @note Address: stage02 .text 0x7FB8
 * @note Size: 0xE8
 */
__declspec(section ".text.carcontrol") void Car::CarRoot::Brake()
{
	CarRoot_Brake(this);
}

// Brake__Q23Car7CarRootFv
/*
    stwu     r1, -0x10(r1)
    mflr     r0
    lis      r5, lbl_5_rodata_278@ha
    lis      r4, lbl_5_rodata_27C@ha
    stw      r0, 0x14(r1)
    li       r0, 0x1
    li       r7, 0x0
    li       r8, 0x0
    stw      r31, 0xc(r1)
    mr       r31, r3
    stb      r0, 0x7c(r3)
    lis      r3, lbl_5_rodata_25C@ha
    addi     r6, r3, lbl_5_rodata_25C@l
    lwz      r3, 0x4(r31)
    lfs      f1, 0x0(r6)
    addi     r6, r5, lbl_5_rodata_278@l
    lwz      r12, 0x34(r3)
    addi     r5, r4, lbl_5_rodata_27C@l
    lfs      f2, 0x0(r6)
    fmr      f3, f1
    lwz      r12, 0x80(r12)
    li       r4, 0x2
    lfs      f4, 0x0(r5)
    li       r5, 0x0
    li       r6, 0x0
    mtctr    r12
    bctrl
    li       r0, 0x10
    lwz      r4, 0x4(r31)
    li       r5, 0x0
    mtctr    r0
.L_00008034:
    lwz      r0, 0x810(r4)
    add      r3, r0, r5
    slwi     r0, r3, 28
    srwi     r3, r3, 31
    subf     r0, r3, r0
    rotlwi   r0, r0, 4
    add      r0, r0, r3
    slwi     r3, r0, 3
    addi     r0, r3, 0x814
    lwzx     r0, r4, r0
    cmpwi    r0, -0x1
    bne      .L_00008084
    add      r4, r4, r3
    li       r0, 0x3
    stw      r0, 0x814(r4)
    li       r3, 0x0
    li       r0, 0x64
    sth      r3, 0x818(r4)
    sth      r0, 0x81a(r4)
    b        .L_0000808C
.L_00008084:
    addi     r5, r5, 0x1
    bdnz     .L_00008034
.L_0000808C:
    lwz      r0, 0x14(r1)
    lwz      r31, 0xc(r1)
    mtlr     r0
    addi     r1, r1, 0x10
    blr
*/

/**
 * @note Address: stage02 .text 0x80A0
 * @note Size: 0x490
 */
__declspec(section ".text.carcontrol") void Car::CarRoot::Update(u32 arg)
{
	HSD_JObj* jobj;
	Mtx scale;
	Mtx m;

	if (_04 == NULL || _08 == NULL) {
		return;
	}
	s32 path[2] = { 0, 0 };
	jobj        = GetJointByPath(_08->jobj, path, 1);
	PSMTXScale(scale, sCarModelScale, sCarModelScale, sCarModelScale);
	PSMTXConcat(HSD_JObjGetMtxPtr(jobj), scale, _1C);
	HSD_JObjCopyMtx(_04->jobj, _1C);
	if (_0C != NULL) {
		PSMTXTrans(m, sCarAttachmentX, sCarAttachmentY, sCarAttachmentZ);
		PSMTXConcat(_1C, m, _4C);
		HSD_JObjCopyMtx(_0C->jobj, _4C);
	}
	if (mStarted) {
		mPreviousFrame = mFrame;
		mFrame         = mPlaybackRate * arg + mFrame;
	}
	_08->SetAnim(0, 1, NULL, NULL, NULL, mFrame, 1.0f, 0.0f, sCarAnimBlendRate);
	if (!mBraking) {
		_04->SetAnim(1, 1, NULL, NULL, NULL, mFrame, 1.0f, 0.0f, sCarAnimBlendRate);
		return;
	}
	if (0.0f < mPlaybackRate) {
		f32 rate = mPlaybackRate - ReadS32LE(carParams) / sCarBrakeRateDivisor;
		if (rate < 0.0f) {
			rate = 0.0f;
		}
		mPlaybackRate = rate;
	}
}

// Update__Q23Car7CarRootFUl
/*
    stwu     r1, -0x90(r1)
    mflr     r0
    stw      r0, 0x94(r1)
    stw      r31, 0x8c(r1)
    mr       r31, r3
    stw      r30, 0x88(r1)
    mr       r30, r4
    stw      r29, 0x84(r1)
    lwz      r0, 0x4(r3)
    cmplwi   r0, 0x0
    beq      .L_00008514
    lwz      r4, 0x8(r31)
    cmplwi   r4, 0x0
    bne      .L_000080DC
    b        .L_00008514
.L_000080DC:
    lis      r3, lbl_5_rodata_208@ha
    lwz      r29, 0x0(r4)
    addi     r4, r3, lbl_5_rodata_208@l
    lwz      r3, 0x0(r4)
    cmplwi   r29, 0x0
    lwz      r0, 0x4(r4)
    stw      r3, 0x8(r1)
    stw      r0, 0xc(r1)
    bne      .L_00008108
    li       r29, 0x0
    b        .L_00008154
.L_00008108:
    li       r0, 0x0
    bne      .L_00008118
    mr       r29, r0
    b        .L_0000811C
.L_00008118:
    lwz      r29, 0x10(r29)
.L_0000811C:
    lwz      r0, 0x8(r1)
    mtctr    r0
    cmpwi    r0, 0x0
    ble      .L_00008154
.L_0000812C:
    cmplwi   r29, 0x0
    bne      .L_0000813C
    li       r29, 0x0
    b        .L_00008140
.L_0000813C:
    lwz      r29, 0x8(r29)
.L_00008140:
    cmplwi   r29, 0x0
    bne      .L_00008150
    li       r29, 0x0
    b        .L_00008154
.L_00008150:
    bdnz     .L_0000812C
.L_00008154:
    lis      r4, lbl_5_rodata_2A4@ha
    addi     r3, r1, 0x40
    lfs      f1, lbl_5_rodata_2A4@l(r4)
    fmr      f2, f1
    fmr      f3, f1
    bl       PSMTXScale
    cmplwi   r29, 0x0
    bne      .L_0000818C
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_23C@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x47c
    addi     r5, r5, lbl_5_rodata_23C@l
    bl       __assert
.L_0000818C:
    cmplwi   r29, 0x0
    beq      .L_000081DC
    bne      .L_000081B0
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_23C@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x25d
    addi     r5, r5, lbl_5_rodata_23C@l
    bl       __assert
.L_000081B0:
    lwz      r4, 0x14(r29)
    li       r3, 0x0
    rlwinm.  r0, r4, 0, 8, 8
    bne      .L_000081CC
    rlwinm.  r0, r4, 0, 25, 25
    beq      .L_000081CC
    li       r3, 0x1
.L_000081CC:
    clrlwi.  r0, r3, 24
    beq      .L_000081DC
    mr       r3, r29
    bl       HSD_JObjSetupMatrixSub
.L_000081DC:
    addi     r3, r29, 0x44
    addi     r4, r1, 0x40
    addi     r5, r31, 0x1c
    bl       PSMTXConcat
    lwz      r3, 0x4(r31)
    lwz      r29, 0x0(r3)
    cmplwi   r29, 0x0
    bne      .L_00008214
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_23C@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x495
    addi     r5, r5, lbl_5_rodata_23C@l
    bl       __assert
.L_00008214:
    addic.   r0, r31, 0x1c
    bne      .L_00008234
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_2A8@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x496
    addi     r5, r5, lbl_5_rodata_2A8@l
    bl       __assert
.L_00008234:
    addi     r3, r31, 0x1c
    addi     r4, r29, 0x44
    bl       PSMTXCopy
    lwz      r0, 0x14(r29)
    cmplwi   r29, 0x0
    oris     r0, r0, 0x380
    stw      r0, 0x14(r29)
    beq      .L_0000829C
    bne      .L_00008270
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_23C@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x25d
    addi     r5, r5, lbl_5_rodata_23C@l
    bl       __assert
.L_00008270:
    lwz      r4, 0x14(r29)
    li       r3, 0x0
    rlwinm.  r0, r4, 0, 8, 8
    bne      .L_0000828C
    rlwinm.  r0, r4, 0, 25, 25
    beq      .L_0000828C
    li       r3, 0x1
.L_0000828C:
    clrlwi.  r0, r3, 24
    bne      .L_0000829C
    mr       r3, r29
    bl       HSD_JObjSetMtxDirtySub
.L_0000829C:
    lwz      r0, 0xc(r31)
    cmplwi   r0, 0x0
    beq      .L_00008388
    lis      r6, lbl_5_rodata_2AC@ha
    lis      r5, lbl_5_rodata_2B0@ha
    lis      r4, lbl_5_rodata_2B4@ha
    lfs      f1, lbl_5_rodata_2AC@l(r6)
    lfs      f2, lbl_5_rodata_2B0@l(r5)
    addi     r3, r1, 0x10
    lfs      f3, lbl_5_rodata_2B4@l(r4)
    bl       PSMTXTrans
    addi     r3, r31, 0x1c
    addi     r4, r1, 0x10
    addi     r5, r31, 0x4c
    bl       PSMTXConcat
    lwz      r3, 0xc(r31)
    lwz      r29, 0x0(r3)
    cmplwi   r29, 0x0
    bne      .L_00008300
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_23C@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x495
    addi     r5, r5, lbl_5_rodata_23C@l
    bl       __assert
.L_00008300:
    addic.   r0, r31, 0x4c
    bne      .L_00008320
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_2A8@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x496
    addi     r5, r5, lbl_5_rodata_2A8@l
    bl       __assert
.L_00008320:
    addi     r3, r31, 0x4c
    addi     r4, r29, 0x44
    bl       PSMTXCopy
    lwz      r0, 0x14(r29)
    cmplwi   r29, 0x0
    oris     r0, r0, 0x380
    stw      r0, 0x14(r29)
    beq      .L_00008388
    bne      .L_0000835C
    lis      r3, lbl_5_rodata_234@ha
    lis      r5, lbl_5_rodata_23C@ha
    addi     r3, r3, lbl_5_rodata_234@l
    li       r4, 0x25d
    addi     r5, r5, lbl_5_rodata_23C@l
    bl       __assert
.L_0000835C:
    lwz      r4, 0x14(r29)
    li       r3, 0x0
    rlwinm.  r0, r4, 0, 8, 8
    bne      .L_00008378
    rlwinm.  r0, r4, 0, 25, 25
    beq      .L_00008378
    li       r3, 0x1
.L_00008378:
    clrlwi.  r0, r3, 24
    bne      .L_00008388
    mr       r3, r29
    bl       HSD_JObjSetMtxDirtySub
.L_00008388:
    lbz      r0, 0x7d(r31)
    cmplwi   r0, 0x0
    beq      .L_000083C8
    lis      r0, 0x4330
    lfs      f0, 0x10(r31)
    stw      r30, 0x74(r1)
    lis      r3, lbl_5_rodata_2C0@ha
    lfd      f1, lbl_5_rodata_2C0@l(r3)
    stfs     f0, 0x14(r31)
    stw      r0, 0x70(r1)
    lfs      f2, 0x18(r31)
    lfd      f0, 0x70(r1)
    fsubs    f1, f0, f1
    lfs      f0, 0x10(r31)
    fmadds   f0, f2, f1, f0
    stfs     f0, 0x10(r31)
.L_000083C8:
    lwz      r3, 0x8(r31)
    lis      r5, lbl_5_rodata_25C@ha
    addi     r7, r5, lbl_5_rodata_25C@l
    lis      r4, lbl_5_rodata_27C@ha
    lwz      r12, 0x34(r3)
    addi     r8, r4, lbl_5_rodata_27C@l
    lis      r6, lbl_5_rodata_278@ha
    lfs      f3, 0x0(r7)
    lwz      r12, 0x80(r12)
    li       r4, 0x0
    lfs      f2, lbl_5_rodata_278@l(r6)
    li       r5, 0x1
    lfs      f4, 0x0(r8)
    li       r6, 0x0
    lfs      f1, 0x10(r31)
    li       r7, 0x0
    li       r8, 0x0
    mtctr    r12
    bctrl
    lbz      r0, 0x7c(r31)
    cmplwi   r0, 0x0
    bne      .L_00008470
    lwz      r3, 0x4(r31)
    lis      r5, lbl_5_rodata_25C@ha
    addi     r7, r5, lbl_5_rodata_25C@l
    lis      r4, lbl_5_rodata_27C@ha
    lwz      r12, 0x34(r3)
    addi     r8, r4, lbl_5_rodata_27C@l
    lis      r6, lbl_5_rodata_278@ha
    lfs      f3, 0x0(r7)
    lwz      r12, 0x80(r12)
    li       r4, 0x1
    lfs      f2, lbl_5_rodata_278@l(r6)
    li       r5, 0x1
    lfs      f4, 0x0(r8)
    li       r6, 0x0
    lfs      f1, 0x10(r31)
    li       r7, 0x0
    li       r8, 0x0
    mtctr    r12
    bctrl
    b        .L_00008514
.L_00008470:
    lis      r3, lbl_5_rodata_25C@ha
    lfs      f3, 0x18(r31)
    lfs      f0, lbl_5_rodata_25C@l(r3)
    fcmpo    cr0, f0, f3
    bge      .L_00008514
    lis      r3, carParams@ha
    lwz      r4, carParams@l(r3)
    cmplwi   r4, 0x0
    beq      .L_000084C0
    lbz      r3, 0x1(r4)
    lbz      r0, 0x2(r4)
    lbz      r5, 0x3(r4)
    slwi     r3, r3, 8
    slwi     r0, r0, 16
    lbz      r4, 0x0(r4)
    slwi     r5, r5, 24
    add      r0, r3, r0
    add      r0, r0, r5
    add      r0, r4, r0
    b        .L_000084C4
.L_000084C0:
    li       r0, 0x0
.L_000084C4:
    xoris    r3, r0, 0x8000
    lis      r0, 0x4330
    lis      r4, lbl_5_rodata_280@ha
    stw      r3, 0x74(r1)
    addi     r5, r4, lbl_5_rodata_280@l
    lis      r3, lbl_5_rodata_2B8@ha
    stw      r0, 0x70(r1)
    addi     r4, r3, lbl_5_rodata_2B8@l
    lfd      f2, 0x0(r5)
    lis      r3, lbl_5_rodata_25C@ha
    lfd      f0, 0x70(r1)
    lfs      f1, 0x0(r4)
    fsubs    f2, f0, f2
    lfs      f0, lbl_5_rodata_25C@l(r3)
    fdivs    f1, f2, f1
    fsubs    f1, f3, f1
    fcmpo    cr0, f1, f0
    bge      .L_00008510
    fmr      f1, f0
.L_00008510:
    stfs     f1, 0x18(r31)
.L_00008514:
    lwz      r0, 0x94(r1)
    lwz      r31, 0x8c(r1)
    lwz      r30, 0x88(r1)
    lwz      r29, 0x84(r1)
    mtlr     r0
    addi     r1, r1, 0x90
    blr
*/

/**
 * @note Address: stage02 .text 0x8530
 * @note Size: 0x74
 */
__declspec(section ".text.carcontrol") void Car::CCameraActionCar::Update()
{
	Vec pos;

	if (_40 != NULL) {
		SetFov(sCarCameraFov);
		PSMTXMultVec(_40, &_44, &pos);
		SetInterest(pos);
	}
}

/**
 * @note Address: stage02 .text 0x85A4
 * @note Size: 0xC
 */
__declspec(section ".text.carcontrol") void Car::CarRoot::Start()
{
	CarRoot_Start(this);
}

/**
 * @note Address: stage02 .text 0x85B0
 * @note Size: 0x98
 */
#pragma section RX ".text.carcameramethods"
__declspec(section ".text.carcameramethods") Car::CCameraActionCar::~CCameraActionCar()
{
}

/**
 * @note Address: stage02 .text 0x8648
 * @note Size: 0x8
 */
__declspec(section ".text.carcameramethods") s32 Car::CCameraActionCar::GetMode()
{
	return 0;
}

/**
 * @note Address: stage02 .text 0x8650
 * @note Size: 0x8
 */
__declspec(section ".text.carcameramethods") s32 Car::CCameraActionCar::GetFlags()
{
	return 11;
}

/**
 * @note Address: stage02 .text 0x8658
 * @note Size: 0xC
 */
__declspec(section ".text.carcameramethods") f32 Car::CCameraActionCar::GetEaseDivisor()
{
	return sCarCameraEaseDivisor;
}

/**
 * @note Address: stage02 .text 0x8664
 * @note Size: 0xB4
 */
// MWCC emits the static initializer after all function definitions.
#pragma section code_type ".text.carinit"
