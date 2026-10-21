#include "game/ccameraaction.h"
#include "game/cmotioncontrolcamera.h"
#include "Version.h"
#include <math.h>

extern "C" {
#include <dolphin/os.h>
}

// CGame camera stuff, by offset for now
extern "C" u8 lbl_8025C400[];
// japan's camera, area and map sit 0xCE8 lower
#if defined(VERSION_JP)
#define GAME_CAMERA ((CMotionControlCamera*)(lbl_8025C400 + 0xE5050))
#define GAME_AREA   (*(void**)(lbl_8025C400 + 0xE7108))
#define GAME_MAP    (*(void**)(lbl_8025C400 + 0xE7120))
#else
#define GAME_CAMERA ((CMotionControlCamera*)(lbl_8025C400 + 0xE5D38))
#define GAME_AREA   (*(void**)(lbl_8025C400 + 0xE7DF0))
#define GAME_MAP    (*(void**)(lbl_8025C400 + 0xE7E08))
#endif
#if defined(VERSION_JP)
#define GAME_PLAYER (*(XObj**)(lbl_8025C400 + 0x9674))
#else
#define GAME_PLAYER (*(XObj**)(lbl_8025C400 + 0x96BC))
#endif

// pal goes by the current refresh rate, 50 or 60
#if defined(VERSION_PAL)
extern "C" s32 lbl_80660D4C;
#define FRAME_TIME (1.0f / lbl_80660D4C)
#else
#define FRAME_TIME (1.0f / 60.0f)
#endif

extern "C" {
void fn_8013801C(CamAreaState* state);
void fn_80137D00(CamAreaState* state, s32 arg);
s32 fn_80135D28(CamAreaState* state);
f32 fn_801353E4(CamAreaState* state, s32 area);
void fn_801382D4(void* area, s32 index);
void fn_80138CA4(void* area, s32 mode, f32 time, s32 arg);
void fn_8013C510(void* map, s32 index, f32 speed);
void fn_8013BCE4(void* map);
void fn_8013A204(void* map, f32 dy, f32 dx, f32 speed);
void fn_8013BD28(void* map, Vec* eye, s32 arg);
}

/**
 * @note Address: 0x80147630
 * @note Size: 0x8
 */

/**
 * @note Address: 0x80147638
 * @note Size: 0x8
 */

/**
 * @note Address: 0x80147640
 * @note Size: 0x8
 */

/**
 * @note Address: 0x80147648
 * @note Size: 0x14C
 */
void CCameraActionReset::Update(u32 step)
{
	if (mTarget == NULL) {
		return;
	}
	if (mState == 100) {
		mState = 0;
		return;
	}

	SetInterest(mTarget->GetCameraTarget());
	if (mEyeInterp != NULL) {
		if (!mEyeInterp->Update(step, FRAME_TIME)) {
			SetEye(mEyeInterp->Get());
		} else {
			mEyeInterp = NULL;
		}
	}
	if (mFovInterp != NULL) {
		if (!mFovInterp->Update(step, FRAME_TIME)) {
			SetFov(mFovInterp->Get());
		} else {
			mFovInterp = NULL;
		}
	}
}

/**
 * @note Address: 0x80147794
 * @note Size: 0x5DC
 */
void CCameraActionReset::PreviousUpdate()
{
	Vec eye;

	if (mState == 100) {
		if (0.0f != mAngle.z) {
			_38       = 0x40010;
			Vec angle = mAngle;
			angle.z   = 0.0f;
			SetAngle(angle);
		}
		return;
	}
	if (mState == 1) {
		if (mEyeInterp == NULL && mFovInterp == NULL) {
			GAME_CAMERA->ChangeAction(_50);
		}
		return;
	}

	_38 = 0xB;
	fn_80137D00(&lbl_805A054C, 0);
	s32 arg = lbl_805A054C._0C;
	_50     = fn_80135D28(&lbl_805A054C);
	if (_4C == TRUE) {
		CMotionControlCamera* camera = GAME_CAMERA;
		camera->ChangeAction(_50);
		if (_50 == 1) {
			fn_801382D4(GAME_AREA, lbl_805A054C._14);
		} else {
			fn_8013C510(GAME_MAP, lbl_805A054C._14, 15.0f);
			fn_8013BCE4(GAME_MAP);
			f32 dy = camera->mCamera.mInterest.y - mTarget->GetCameraTarget().y;
			if (0.0f != dy) {
				fn_8013A204(GAME_MAP, dy, 0.0f, 8.0f);
			}
		}
		_4C = FALSE;
		return;
	}

	if (_50 == 1) {
		CMotionControlCamera* camera = GAME_CAMERA;
		camera->ChangeAction(_50);
		fn_801382D4(GAME_AREA, lbl_805A054C._14);
		fn_80138CA4(GAME_AREA, mMode, mTime, arg);
		if (mMode == 3) {
			camera->ApplyAction();
		}
		return;
	}

	if (_50 == 7) {
		fn_8013BD28(GAME_MAP, &eye, 1);
		if (fpclassify(eye.x) == FP_NAN || fpclassify(eye.y) == FP_NAN || fpclassify(eye.z) == FP_NAN) {
			CVec cameraEye      = GAME_CAMERA->mCamera.mEye;
			CVec cameraInterest = GAME_CAMERA->mCamera.mInterest;
			OSPanic(__FILE__, 157, "%s%s",
			        fn_801911B4("CCameraActionReset ...\nEye(%f, %f, %f)\nInt(%f, %f, %f)\n", cameraEye.x, cameraEye.y, cameraEye.z,
			                    cameraInterest.x, cameraInterest.y, cameraInterest.z),
			        __FUNCTION__);
		}

		Vec target = eye;
		SetEye_Interp(mMode, target, mTime);

		f32 fov = fn_801353E4(&lbl_805A054C, lbl_805A054C._14);
		SetFov_Interp(mMode, fov, mTime);

		fn_801382D4(GAME_AREA, lbl_805A054C._14);
		if (mMode == 3 || 0.0f == mTime) {
			_58 = 0;
		} else {
			_58 = 2;
		}
	}
	mState = 1;
}
/*
    stwu     r1, -0x90(r1)
    mflr     r0
    stw      r0, 0x94(r1)
    stw      r31, 0x8c(r1)
    mr       r31, r3
    stw      r30, 0x88(r1)
    stw      r29, 0x84(r1)
    stw      r28, 0x80(r1)
    lwz      r0, 0x54(r3)
    cmpwi    r0, 0x64
    bne      .L_80147810
    lfs      f1, 0x28(r31)
    lfs      f0, lbl_8065EC24@sda21(r0)
    fcmpu    cr0, f1, f0
    beq      .L_80147D50
    lis      r5, 0x4
    addi     r4, r1, 0x2c
    addi     r0, r5, 0x10
    stw      r0, 0x38(r31)
    lwz      r5, 0x20(r31)
    lwz      r0, 0x24(r31)
    stw      r5, 0x2c(r1)
    stw      r0, 0x30(r1)
    lwz      r0, 0x28(r31)
    stw      r0, 0x34(r1)
    stfs     f0, 0x34(r1)
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x40(r12)
    mtctr    r12
    bctrl
    b        .L_80147D50
.L_80147810:
    cmpwi    r0, 0x1
    bne      .L_8014784C
    lwz      r0, 0x5c(r31)
    cmplwi   r0, 0x0
    bne      .L_80147D50
    lwz      r0, 0x60(r31)
    cmplwi   r0, 0x0
    bne      .L_80147D50
    lis      r3, lbl_8025C400@ha
    lwz      r4, 0x50(r31)
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    addi     r3, r3, 0x5d38
    bl       fn_8014946C
    b        .L_80147D50
.L_8014784C:
    li       r0, 0xb
    lis      r3, lbl_805A054C@ha
    stw      r0, 0x38(r31)
    addi     r3, r3, lbl_805A054C@l
    li       r4, 0x0
    bl       fn_80137D00
    lis      r3, lbl_805A054C@ha
    addi     r3, r3, lbl_805A054C@l
    lwz      r30, 0xc(r3)
    bl       fn_80135D28
    stw      r3, 0x50(r31)
    lwz      r0, 0x4c(r31)
    cmpwi    r0, 0x1
    bne      .L_80147944
    lis      r3, lbl_8025C400@ha
    lwz      r4, 0x50(r31)
    addi     r3, r3, lbl_8025C400@l
    addis    r29, r3, 0xe
    addi     r29, r29, 0x5d38
    mr       r3, r29
    bl       fn_8014946C
    lwz      r0, 0x50(r31)
    cmpwi    r0, 0x1
    bne      .L_801478D0
    lis      r4, lbl_8025C400@ha
    lis      r3, lbl_805A054C@ha
    addi     r4, r4, lbl_8025C400@l
    addis    r5, r4, 0xe
    addi     r4, r3, lbl_805A054C@l
    lwz      r3, 0x7df0(r5)
    lwz      r4, 0x14(r4)
    bl       fn_801382D4
    b        .L_80147938
.L_801478D0:
    lis      r4, lbl_8025C400@ha
    lis      r3, lbl_805A054C@ha
    addi     r4, r4, lbl_8025C400@l
    lfs      f1, lbl_8065EC2C@sda21(r0)
    addis    r30, r4, 0xe
    addi     r4, r3, lbl_805A054C@l
    lwz      r3, 0x7e08(r30)
    lwz      r4, 0x14(r4)
    bl       fn_8013C510
    lwz      r3, 0x7e08(r30)
    bl       fn_8013BCE4
    lwz      r4, 0x40(r31)
    addi     r3, r1, 0x44
    lwz      r12, 0x34(r4)
    lwz      r12, 0xec(r12)
    mtctr    r12
    bctrl
    lfs      f1, 0x48(r1)
    lfs      f0, 0xa8(r29)
    lfs      f2, lbl_8065EC24@sda21(r0)
    fsubs    f1, f0, f1
    fcmpu    cr0, f2, f1
    beq      .L_80147938
    lwz      r3, 0x7e08(r30)
    lfs      f3, lbl_8065EC30@sda21(r0)
    bl       fn_8013A204
.L_80147938:
    li       r0, 0x0
    stw      r0, 0x4c(r31)
    b        .L_80147D50
.L_80147944:
    lwz      r4, 0x50(r31)
    cmpwi    r4, 0x1
    bne      .L_801479B4
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r28, r3, 0xe
    addi     r28, r28, 0x5d38
    mr       r3, r28
    bl       fn_8014946C
    lis      r4, lbl_8025C400@ha
    lis      r3, lbl_805A054C@ha
    addi     r4, r4, lbl_8025C400@l
    addis    r29, r4, 0xe
    addi     r4, r3, lbl_805A054C@l
    lwz      r3, 0x7df0(r29)
    lwz      r4, 0x14(r4)
    bl       fn_801382D4
    lwz      r3, 0x7df0(r29)
    mr       r5, r30
    lwz      r4, 0x44(r31)
    lfs      f1, 0x48(r31)
    bl       fn_80138CA4
    lwz      r0, 0x44(r31)
    cmpwi    r0, 0x3
    bne      .L_80147D50
    mr       r3, r28
    bl       fn_80148E30
    b        .L_80147D50
.L_801479B4:
    cmpwi    r4, 0x7
    bne      .L_80147D48
    lis      r3, lbl_8025C400@ha
    addi     r4, r1, 0x70
    addi     r3, r3, lbl_8025C400@l
    li       r5, 0x1
    addis    r3, r3, 0xe
    lwz      r3, 0x7e08(r3)
    bl       fn_8013BD28
    lfs      f0, 0x70(r1)
    lis      r0, 0x7f80
    stfs     f0, 0x10(r1)
    lwz      r4, 0x10(r1)
    rlwinm   r3, r4, 0, 1, 8
    cmpw     r3, r0
    beq      .L_80147A04
    bge      .L_80147A34
    cmpwi    r3, 0x0
    beq      .L_80147A1C
    b        .L_80147A34
.L_80147A04:
    clrlwi.  r0, r4, 9
    beq      .L_80147A14
    li       r0, 0x1
    b        .L_80147A38
.L_80147A14:
    li       r0, 0x2
    b        .L_80147A38
.L_80147A1C:
    clrlwi.  r0, r4, 9
    beq      .L_80147A2C
    li       r0, 0x5
    b        .L_80147A38
.L_80147A2C:
    li       r0, 0x3
    b        .L_80147A38
.L_80147A34:
    li       r0, 0x4
.L_80147A38:
    cmpwi    r0, 0x1
    beq      .L_80147B10
    lfs      f0, 0x74(r1)
    lis      r0, 0x7f80
    stfs     f0, 0xc(r1)
    lwz      r4, 0xc(r1)
    rlwinm   r3, r4, 0, 1, 8
    cmpw     r3, r0
    beq      .L_80147A6C
    bge      .L_80147A9C
    cmpwi    r3, 0x0
    beq      .L_80147A84
    b        .L_80147A9C
.L_80147A6C:
    clrlwi.  r0, r4, 9
    beq      .L_80147A7C
    li       r0, 0x1
    b        .L_80147AA0
.L_80147A7C:
    li       r0, 0x2
    b        .L_80147AA0
.L_80147A84:
    clrlwi.  r0, r4, 9
    beq      .L_80147A94
    li       r0, 0x5
    b        .L_80147AA0
.L_80147A94:
    li       r0, 0x3
    b        .L_80147AA0
.L_80147A9C:
    li       r0, 0x4
.L_80147AA0:
    cmpwi    r0, 0x1
    beq      .L_80147B10
    lfs      f0, 0x78(r1)
    lis      r0, 0x7f80
    stfs     f0, 0x8(r1)
    lwz      r4, 0x8(r1)
    rlwinm   r3, r4, 0, 1, 8
    cmpw     r3, r0
    beq      .L_80147AD4
    bge      .L_80147B04
    cmpwi    r3, 0x0
    beq      .L_80147AEC
    b        .L_80147B04
.L_80147AD4:
    clrlwi.  r0, r4, 9
    beq      .L_80147AE4
    li       r0, 0x1
    b        .L_80147B08
.L_80147AE4:
    li       r0, 0x2
    b        .L_80147B08
.L_80147AEC:
    clrlwi.  r0, r4, 9
    beq      .L_80147AFC
    li       r0, 0x5
    b        .L_80147B08
.L_80147AFC:
    li       r0, 0x3
    b        .L_80147B08
.L_80147B04:
    li       r0, 0x4
.L_80147B08:
    cmpwi    r0, 0x1
    bne      .L_80147BB8
.L_80147B10:
    lis      r3, lbl_8025C400@ha
    lis      r4, __vt__4CVec@ha
    addi     r5, r3, lbl_8025C400@l
    addis    r10, r5, 0xe
    addi     r6, r4, __vt__4CVec@l
    lwz      r9, 0x5dcc(r10)
    lis      r3, lbl_8020D074@ha
    lwz      r8, 0x5dd0(r10)
    addi     r3, r3, lbl_8020D074@l
    lwz      r7, 0x5dd4(r10)
    lwz      r5, 0x5ddc(r10)
    lwz      r4, 0x5de0(r10)
    lwz      r0, 0x5de4(r10)
    stw      r9, 0x60(r1)
    stw      r8, 0x64(r1)
    lfs      f1, 0x60(r1)
    stw      r7, 0x68(r1)
    lfs      f2, 0x64(r1)
    stw      r5, 0x50(r1)
    lfs      f3, 0x68(r1)
    stw      r4, 0x54(r1)
    lfs      f4, 0x50(r1)
    stw      r0, 0x58(r1)
    lfs      f5, 0x54(r1)
    stw      r6, 0x6c(r1)
    lfs      f6, 0x58(r1)
    stw      r6, 0x5c(r1)
    crset    cr1eq
    bl       fn_801911B4
    lis      r7, lbl_8020D05C@ha
    lis      r5, lbl_8022EB20@ha
    mr       r6, r3
    li       r4, 0x9d
    addi     r3, r7, lbl_8020D05C@l
    addi     r7, r5, lbl_8022EB20@l
    li       r5, lbl_8065EC34@sda21
    crclr    cr1eq
    bl       OSPanic
    lis      r3, __vt__4CVec@ha
    addi     r0, r3, __vt__4CVec@l
    stw      r0, 0x5c(r1)
    stw      r0, 0x6c(r1)
.L_80147BB8:
    lfs      f1, 0x48(r31)
    lwz      r4, 0x70(r1)
    lwz      r3, 0x74(r1)
    lwz      r0, 0x78(r1)
    stw      r4, 0x38(r1)
    stw      r3, 0x3c(r1)
    stw      r0, 0x40(r1)
    lwz      r0, 0x44(r31)
    cmpwi    r0, 0x3
    beq      .L_80147BF0
    lfs      f0, lbl_8065EC24@sda21(r0)
    fcmpo    cr0, f1, f0
    cror     eq, lt, eq
    bne      .L_80147C14
.L_80147BF0:
    li       r0, 0x0
    mr       r3, r31
    stw      r0, 0x5c(r31)
    addi     r4, r1, 0x38
    lwz      r12, 0x3c(r31)
    lwz      r12, 0x38(r12)
    mtctr    r12
    bctrl
    b        .L_80147C78
.L_80147C14:
    slwi     r0, r0, 2
    add      r3, r31, r0
    lwz      r0, 0x18c(r3)
    stw      r0, 0x5c(r31)
    lwz      r3, 0x5c(r31)
    cmplwi   r3, 0x0
    beq      .L_80147C78
    lwz      r5, 0x38(r1)
    addi     r4, r1, 0x14
    lwz      r6, 0x3c(r1)
    lwz      r0, 0x40(r1)
    stw      r5, 0x20(r1)
    addi     r5, r1, 0x20
    stw      r6, 0x24(r1)
    stw      r0, 0x28(r1)
    lwz      r6, 0x0(r31)
    lwz      r0, 0x4(r31)
    stw      r6, 0x14(r1)
    stw      r0, 0x18(r1)
    lwz      r0, 0x8(r31)
    stw      r0, 0x1c(r1)
    lwz      r12, 0x0(r3)
    lwz      r12, 0xc(r12)
    mtctr    r12
    bctrl
.L_80147C78:
    lis      r3, lbl_805A054C@ha
    addi     r30, r3, lbl_805A054C@l
    lwz      r4, 0x14(r30)
    mr       r3, r30
    bl       fn_801353E4
    lwz      r0, 0x44(r31)
    lfs      f3, 0x48(r31)
    cmpwi    r0, 0x3
    beq      .L_80147CAC
    lfs      f0, lbl_8065EC24@sda21(r0)
    fcmpo    cr0, f3, f0
    cror     eq, lt, eq
    bne      .L_80147CCC
.L_80147CAC:
    li       r0, 0x0
    mr       r3, r31
    stw      r0, 0x60(r31)
    lwz      r12, 0x3c(r31)
    lwz      r12, 0x48(r12)
    mtctr    r12
    bctrl
    b        .L_80147D00
.L_80147CCC:
    slwi     r0, r0, 2
    add      r3, r31, r0
    lwz      r0, 0x1f8(r3)
    stw      r0, 0x60(r31)
    lwz      r3, 0x60(r31)
    cmplwi   r3, 0x0
    beq      .L_80147D00
    lwz      r12, 0x0(r3)
    fmr      f2, f1
    lfs      f1, 0x34(r31)
    lwz      r12, 0xc(r12)
    mtctr    r12
    bctrl
.L_80147D00:
    lis      r3, lbl_8025C400@ha
    lwz      r4, 0x14(r30)
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    lwz      r3, 0x7df0(r3)
    bl       fn_801382D4
    lwz      r0, 0x44(r31)
    cmpwi    r0, 0x3
    beq      .L_80147D34
    lfs      f1, lbl_8065EC24@sda21(r0)
    lfs      f0, 0x48(r31)
    fcmpu    cr0, f1, f0
    bne      .L_80147D40
.L_80147D34:
    li       r0, 0x0
    stw      r0, 0x58(r31)
    b        .L_80147D48
.L_80147D40:
    li       r0, 0x2
    stw      r0, 0x58(r31)
.L_80147D48:
    li       r0, 0x1
    stw      r0, 0x54(r31)
.L_80147D50:
    lwz      r0, 0x94(r1)
    lwz      r31, 0x8c(r1)
    lwz      r30, 0x88(r1)
    lwz      r29, 0x84(r1)
    lwz      r28, 0x80(r1)
    mtlr     r0
    addi     r1, r1, 0x90
    blr
*/

/**
 * @note Address: 0x80147D70
 * @note Size: 0xCC
 */
void CCameraActionReset::SetCamera(CCamera* camera)
{
	SetEye(camera->mEye);
	SetInterest(camera->mInterest);
	SetDistance(camera->mDistance);
	SetFov(camera->mFov);
	SetAngle(camera->mAngle);
	mTarget = GAME_PLAYER;
	mState  = 100;
	_58     = 2;
}

/**
 * @note Address: 0x80147E3C
 * @note Size: 0x7C
 */
void CCameraActionReset::Init()
{
	mMode = 1;
	mTime = 2.0f;
	_4C   = FALSE;
	fn_8013801C(&lbl_805A054C);
	mEyeInterps[0] = &mEyeLinear;
	mEyeInterps[1] = &mEyeEaseInOut;
	mEyeInterps[2] = &mEyeEaseOut;
	mFovInterps[0] = &mFovLinear;
	mFovInterps[1] = &mFovEaseInOut;
	mFovInterps[2] = &mFovEaseOut;
}
