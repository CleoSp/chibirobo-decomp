#include "game/ccameraaction.h"
#include "game/cmotioncontrolcamera.h"
#include <math.h>

extern "C" {
#include <dolphin/os.h>
}

// the area action at 0xE7DF0 in the game object. class unknown, but it calls SetInterest
// through vtable slot _3C so it's a CCameraActionAreaExM
class CamArea : public CCameraActionAreaExM {
public:
	u8 _40[0xC];                            // _40
	s32 _4C;                                // _4C
	s32 _50;                                // _50
	CamAreaEntry* _54;                      // _54
	CamAreaEntry* _58;                      // _58
	s32 _5C;                                // _5C
	u8 _60[0x4];                            // _60
	XObj* _64;                              // _64, the camera looks at it
	u8 _68[0x27C - 0x68];                   // _68
	CInterfaceVectorInterpolation* _27C;    // _27C
	CInterfaceVectorInterpolation* _280[4]; // _280, by mode

	// the vectors are passed by value, hence the copies
	void SetInte_I(s32 mode, Vec from, Vec to, f32 time)
	{
		if (mode == 3 || time <= 0.0f) {
			_27C = NULL;
			SetInterest(to);
			return;
		}
		_27C = _280[mode];
		if (_27C != NULL) {
			_27C->Start(from, to, time);
		}
	}
};

// a flat unit direction from an angle's y
inline void YawDir(CVec& dir, const Vec& angle)
{
	CVec ang(angle);
	f32 x = sin(ang.y);
	f32 z = cos(ang.y);
	dir.x = x;
	dir.y = 0.0f;
	dir.z = z;
	PSVECNormalize(&dir, &dir);
}

// the entry in a slot facing closest to dir
inline s32 FindFacing(const Vec& dir, s32 arg)
{
	CVec look(dir);
	CamAreaSlot* slot   = &lbl_805A054C._2034[arg];
	f32 best            = -1.0f;
	s32 bestIndex       = -1;
	CamAreaEntry* entry = slot->_00;
	for (s32 i = 0; i < slot->_08; i++, entry++) {
		CVec facing;
		YawDir(facing, entry->_20);
		f32 dot = PSVECDotProduct(&look, &facing);
		if (dot > best) {
			bestIndex = i;
			best      = dot;
		}
	}
	if (best == -1.0f) {
		bestIndex = 0;
	}
	return bestIndex;
}

extern "C" void fn_80138A9C(CamArea* area, s32 arg1, s32 arg2, s32 arg3, f32 arg4);
extern "C" void fn_80138CA4(CamArea* area, s32 mode, f32 time, s32 arg);
extern "C" void fn_801382E8(CamArea* area, CamAreaEntry* entry, s32 arg2, s32 arg3, f32 arg4);
extern "C" void fn_80137B58(CamAreaState* state, s32 arg);
extern "C" void fn_80137D00(CamAreaState* state, s32 arg);
extern "C" s32 fn_80135D28(CamAreaState* state);
extern "C" void fn_8013C510(CCameraActionMap* map, s32 index, f32 time);
extern "C" void fn_8013BCE4(CCameraActionMap* map);
extern "C" void fn_8013A204(CCameraActionMap* map, f32 from, f32 to, f32 time);

extern "C" u8 lbl_8025C400[];
// the game object's camera and map action, japan's sit 0xCE8 lower
#if defined(VERSION_JP)
#define GAME_CAMERA ((CMotionControlCamera*)(lbl_8025C400 + 0xE5050))
#define GAME_MAP    (*(CCameraActionMap**)(lbl_8025C400 + 0xE7120))
#else
#define GAME_CAMERA ((CMotionControlCamera*)(lbl_8025C400 + 0xE5D38))
#define GAME_MAP    (*(CCameraActionMap**)(lbl_8025C400 + 0xE7E08))
#endif
// debug text print, printf style
extern "C" void fn_80194FD4(void* game, s32 arg1, const char* fmt, f32 x, f32 y, f32 arg5, f32 arg6, f32 arg7, ...);

/**
 * @note Address: 0x801382E8
 * @note Size: 0x7B4
 */
// fn_801382E8
/*
    stwu     r1, -0xb0(r1)
    mflr     r0
    stw      r0, 0xb4(r1)
    stfd     f31, 0xa0(r1)
    psq_st   f31, 0xa8(r1), 0, qr0
    stw      r31, 0x9c(r1)
    stw      r30, 0x98(r1)
    stw      r29, 0x94(r1)
    mr       r30, r5
    fmr      f31, f1
    cmpwi    r30, 0x5
    mr       r29, r3
    mr       r31, r6
    beq      .L_801384CC
    bge      .L_8013860C
    cmpwi    r30, 0x0
    bge      .L_80138330
    b        .L_8013860C
.L_80138330:
    lwz      r7, 0x10(r4)
    cmpwi    r30, 0x5
    lwz      r6, 0x14(r4)
    lwz      r5, 0x18(r4)
    lwz      r4, 0x0(r29)
    lwz      r3, 0x4(r29)
    lwz      r0, 0x8(r29)
    stw      r7, 0x78(r1)
    stw      r6, 0x7c(r1)
    stw      r5, 0x80(r1)
    stw      r4, 0x84(r1)
    stw      r3, 0x88(r1)
    stw      r0, 0x8c(r1)
    bne      .L_80138394
    lis      r3, lbl_8020C82C@ha
    lis      r6, lbl_8020C844@ha
    lis      r5, lbl_8022E560@ha
    li       r4, 0x294
    addi     r7, r5, lbl_8022E560@l
    addi     r3, r3, lbl_8020C82C@l
    addi     r6, r6, lbl_8020C844@l
    li       r5, lbl_8065E940@sda21
    crclr    cr1eq
    bl       OSPanic
    b        .L_8013860C
.L_80138394:
    lis      r3, __vt__4CVec@ha
    lfs      f0, lbl_8065E93C@sda21(r0)
    addi     r0, r3, __vt__4CVec@l
    mr       r5, r30
    stw      r0, 0x5c(r1)
    stfs     f0, 0x50(r1)
    stfs     f0, 0x54(r1)
    stfs     f0, 0x58(r1)
    stw      r0, 0x5c(r1)
    bne      .L_801383C0
    li       r5, 0x4
.L_801383C0:
    lwz      r4, 0x78(r1)
    cmpwi    r5, 0x3
    lwz      r3, 0x7c(r1)
    lwz      r0, 0x80(r1)
    stw      r4, 0x38(r1)
    stw      r3, 0x3c(r1)
    stw      r0, 0x40(r1)
    beq      .L_801383F0
    lfs      f0, lbl_8065E93C@sda21(r0)
    fcmpo    cr0, f31, f0
    cror     eq, lt, eq
    bne      .L_80138414
.L_801383F0:
    li       r0, 0x0
    mr       r3, r29
    stw      r0, 0x68(r29)
    addi     r4, r1, 0x38
    lwz      r12, 0x3c(r29)
    lwz      r12, 0x38(r12)
    mtctr    r12
    bctrl
    b        .L_8013847C
.L_80138414:
    slwi     r0, r5, 2
    add      r3, r29, r0
    lwz      r0, 0x1f8(r3)
    stw      r0, 0x68(r29)
    lwz      r3, 0x68(r29)
    cmplwi   r3, 0x0
    beq      .L_8013847C
    lwz      r10, 0x38(r1)
    fmr      f1, f31
    lwz      r9, 0x3c(r1)
    addi     r4, r1, 0x8
    lwz      r8, 0x40(r1)
    addi     r5, r1, 0x14
    lwz      r7, 0x84(r1)
    lwz      r6, 0x88(r1)
    lwz      r0, 0x8c(r1)
    stw      r10, 0x14(r1)
    stw      r9, 0x18(r1)
    stw      r8, 0x1c(r1)
    stw      r7, 0x8(r1)
    stw      r6, 0xc(r1)
    stw      r0, 0x10(r1)
    lwz      r12, 0x0(r3)
    lwz      r12, 0xc(r12)
    mtctr    r12
    bctrl
.L_8013847C:
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x1
    lwz      r3, -0x6944(r3)
    lwz      r0, 0xac4(r3)
    cmpwi    r0, 0x5
    beq      .L_8013860C
    lwz      r0, 0xbac(r3)
    cmpwi    r0, 0x5
    beq      .L_8013860C
    lwz      r0, 0xb68(r3)
    cmpwi    r0, 0x5
    beq      .L_8013860C
    lwz      r0, 0xbec(r3)
    cmpwi    r0, 0x2
    beq      .L_8013860C
    addis    r3, r3, 0x1
    lfs      f0, -0x6554(r3)
    stfs     f0, -0x6558(r3)
    b        .L_8013860C
.L_801384CC:
    lwz      r8, 0x10(r4)
    lwz      r7, 0x14(r4)
    lwz      r6, 0x18(r4)
    lwz      r4, 0x0(r29)
    lwz      r3, 0x4(r29)
    lwz      r0, 0x8(r29)
    stw      r8, 0x60(r1)
    stw      r7, 0x64(r1)
    stw      r6, 0x68(r1)
    stw      r4, 0x6c(r1)
    stw      r3, 0x70(r1)
    stw      r0, 0x74(r1)
    bne      .L_80138504
    li       r5, 0x4
.L_80138504:
    lwz      r4, 0x60(r1)
    cmpwi    r5, 0x3
    lwz      r3, 0x64(r1)
    lwz      r0, 0x68(r1)
    stw      r4, 0x44(r1)
    stw      r3, 0x48(r1)
    stw      r0, 0x4c(r1)
    beq      .L_80138534
    lfs      f0, lbl_8065E93C@sda21(r0)
    fcmpo    cr0, f31, f0
    cror     eq, lt, eq
    bne      .L_80138558
.L_80138534:
    li       r0, 0x0
    mr       r3, r29
    stw      r0, 0x68(r29)
    addi     r4, r1, 0x44
    lwz      r12, 0x3c(r29)
    lwz      r12, 0x38(r12)
    mtctr    r12
    bctrl
    b        .L_801385C0
.L_80138558:
    slwi     r0, r5, 2
    add      r3, r29, r0
    lwz      r0, 0x1f8(r3)
    stw      r0, 0x68(r29)
    lwz      r3, 0x68(r29)
    cmplwi   r3, 0x0
    beq      .L_801385C0
    lwz      r10, 0x44(r1)
    fmr      f1, f31
    lwz      r9, 0x48(r1)
    addi     r4, r1, 0x20
    lwz      r8, 0x4c(r1)
    addi     r5, r1, 0x2c
    lwz      r7, 0x6c(r1)
    lwz      r6, 0x70(r1)
    lwz      r0, 0x74(r1)
    stw      r10, 0x2c(r1)
    stw      r9, 0x30(r1)
    stw      r8, 0x34(r1)
    stw      r7, 0x20(r1)
    stw      r6, 0x24(r1)
    stw      r0, 0x28(r1)
    lwz      r12, 0x0(r3)
    lwz      r12, 0xc(r12)
    mtctr    r12
    bctrl
.L_801385C0:
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x1
    lwz      r3, -0x6944(r3)
    lwz      r0, 0xac4(r3)
    cmpwi    r0, 0x5
    beq      .L_8013860C
    lwz      r0, 0xbac(r3)
    cmpwi    r0, 0x5
    beq      .L_8013860C
    lwz      r0, 0xb68(r3)
    cmpwi    r0, 0x5
    beq      .L_8013860C
    lwz      r0, 0xbec(r3)
    cmpwi    r0, 0x2
    beq      .L_8013860C
    addis    r3, r3, 0x1
    lfs      f0, -0x6554(r3)
    stfs     f0, -0x6558(r3)
.L_8013860C:
    lis      r3, lbl_805A054C@ha
    cmpwi    r31, 0x0
    addi     r3, r3, lbl_805A054C@l
    lfs      f2, 0x10(r3)
    beq      .L_801389AC
    lwz      r0, 0x14(r3)
    cmpwi    r0, 0x2
    beq      .L_801387F4
    bge      .L_8013863C
    cmpwi    r0, 0x0
    beq      .L_80138648
    b        .L_80138724
.L_8013863C:
    cmpwi    r0, 0x4
    bge      .L_80138724
    b        .L_801388D0
.L_80138648:
    cmpwi    r30, 0x5
    lfs      f1, 0x34(r29)
    mr       r0, r30
    bne      .L_8013865C
    li       r0, 0x4
.L_8013865C:
    cmpwi    r0, 0x3
    beq      .L_80138674
    lfs      f0, lbl_8065E93C@sda21(r0)
    fcmpo    cr0, f31, f0
    cror     eq, lt, eq
    bne      .L_8013869C
.L_80138674:
    li       r0, 0x0
    lfs      f0, lbl_8065E948@sda21(r0)
    stw      r0, 0x6c(r29)
    mr       r3, r29
    fmuls    f1, f0, f2
    lwz      r12, 0x3c(r29)
    lwz      r12, 0x48(r12)
    mtctr    r12
    bctrl
    b        .L_801386D4
.L_8013869C:
    slwi     r0, r0, 2
    add      r3, r29, r0
    lwz      r0, 0x268(r3)
    stw      r0, 0x6c(r29)
    lwz      r3, 0x6c(r29)
    cmplwi   r3, 0x0
    beq      .L_801386D4
    lwz      r12, 0x0(r3)
    fmr      f3, f31
    lfs      f0, lbl_8065E948@sda21(r0)
    lwz      r12, 0xc(r12)
    fmuls    f2, f0, f2
    mtctr    r12
    bctrl
.L_801386D4:
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x1
    lwz      r3, -0x6944(r3)
    lwz      r0, 0xac4(r3)
    cmpwi    r0, 0x5
    beq      .L_80138A78
    lwz      r0, 0xbac(r3)
    cmpwi    r0, 0x5
    beq      .L_80138A78
    lwz      r0, 0xb68(r3)
    cmpwi    r0, 0x5
    beq      .L_80138A78
    lwz      r0, 0xbec(r3)
    cmpwi    r0, 0x2
    beq      .L_80138A78
    addis    r3, r3, 0x1
    lfs      f0, -0x6554(r3)
    stfs     f0, -0x6558(r3)
    b        .L_80138A78
.L_80138724:
    cmpwi    r30, 0x5
    lfs      f1, 0x34(r29)
    mr       r0, r30
    bne      .L_80138738
    li       r0, 0x4
.L_80138738:
    cmpwi    r0, 0x3
    beq      .L_80138750
    lfs      f0, lbl_8065E93C@sda21(r0)
    fcmpo    cr0, f31, f0
    cror     eq, lt, eq
    bne      .L_80138774
.L_80138750:
    li       r0, 0x0
    mr       r3, r29
    stw      r0, 0x6c(r29)
    fmr      f1, f2
    lwz      r12, 0x3c(r29)
    lwz      r12, 0x48(r12)
    mtctr    r12
    bctrl
    b        .L_801387A4
.L_80138774:
    slwi     r0, r0, 2
    add      r3, r29, r0
    lwz      r0, 0x268(r3)
    stw      r0, 0x6c(r29)
    lwz      r3, 0x6c(r29)
    cmplwi   r3, 0x0
    beq      .L_801387A4
    lwz      r12, 0x0(r3)
    fmr      f3, f31
    lwz      r12, 0xc(r12)
    mtctr    r12
    bctrl
.L_801387A4:
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x1
    lwz      r3, -0x6944(r3)
    lwz      r0, 0xac4(r3)
    cmpwi    r0, 0x5
    beq      .L_80138A78
    lwz      r0, 0xbac(r3)
    cmpwi    r0, 0x5
    beq      .L_80138A78
    lwz      r0, 0xb68(r3)
    cmpwi    r0, 0x5
    beq      .L_80138A78
    lwz      r0, 0xbec(r3)
    cmpwi    r0, 0x2
    beq      .L_80138A78
    addis    r3, r3, 0x1
    lfs      f0, -0x6554(r3)
    stfs     f0, -0x6558(r3)
    b        .L_80138A78
.L_801387F4:
    cmpwi    r30, 0x5
    lfs      f1, 0x34(r29)
    mr       r0, r30
    bne      .L_80138808
    li       r0, 0x4
.L_80138808:
    cmpwi    r0, 0x3
    beq      .L_80138820
    lfs      f0, lbl_8065E93C@sda21(r0)
    fcmpo    cr0, f31, f0
    cror     eq, lt, eq
    bne      .L_80138848
.L_80138820:
    li       r0, 0x0
    lfs      f0, lbl_8065E94C@sda21(r0)
    stw      r0, 0x6c(r29)
    mr       r3, r29
    fmuls    f1, f0, f2
    lwz      r12, 0x3c(r29)
    lwz      r12, 0x48(r12)
    mtctr    r12
    bctrl
    b        .L_80138880
.L_80138848:
    slwi     r0, r0, 2
    add      r3, r29, r0
    lwz      r0, 0x268(r3)
    stw      r0, 0x6c(r29)
    lwz      r3, 0x6c(r29)
    cmplwi   r3, 0x0
    beq      .L_80138880
    lwz      r12, 0x0(r3)
    fmr      f3, f31
    lfs      f0, lbl_8065E94C@sda21(r0)
    lwz      r12, 0xc(r12)
    fmuls    f2, f0, f2
    mtctr    r12
    bctrl
.L_80138880:
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x1
    lwz      r3, -0x6944(r3)
    lwz      r0, 0xac4(r3)
    cmpwi    r0, 0x5
    beq      .L_80138A78
    lwz      r0, 0xbac(r3)
    cmpwi    r0, 0x5
    beq      .L_80138A78
    lwz      r0, 0xb68(r3)
    cmpwi    r0, 0x5
    beq      .L_80138A78
    lwz      r0, 0xbec(r3)
    cmpwi    r0, 0x2
    beq      .L_80138A78
    addis    r3, r3, 0x1
    lfs      f0, -0x6554(r3)
    stfs     f0, -0x6558(r3)
    b        .L_80138A78
.L_801388D0:
    cmpwi    r30, 0x5
    lfs      f1, 0x34(r29)
    mr       r0, r30
    bne      .L_801388E4
    li       r0, 0x4
.L_801388E4:
    cmpwi    r0, 0x3
    beq      .L_801388FC
    lfs      f0, lbl_8065E93C@sda21(r0)
    fcmpo    cr0, f31, f0
    cror     eq, lt, eq
    bne      .L_80138924
.L_801388FC:
    li       r0, 0x0
    lfs      f0, lbl_8065E950@sda21(r0)
    stw      r0, 0x6c(r29)
    mr       r3, r29
    fmuls    f1, f0, f2
    lwz      r12, 0x3c(r29)
    lwz      r12, 0x48(r12)
    mtctr    r12
    bctrl
    b        .L_8013895C
.L_80138924:
    slwi     r0, r0, 2
    add      r3, r29, r0
    lwz      r0, 0x268(r3)
    stw      r0, 0x6c(r29)
    lwz      r3, 0x6c(r29)
    cmplwi   r3, 0x0
    beq      .L_8013895C
    lwz      r12, 0x0(r3)
    fmr      f3, f31
    lfs      f0, lbl_8065E950@sda21(r0)
    lwz      r12, 0xc(r12)
    fmuls    f2, f0, f2
    mtctr    r12
    bctrl
.L_8013895C:
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x1
    lwz      r3, -0x6944(r3)
    lwz      r0, 0xac4(r3)
    cmpwi    r0, 0x5
    beq      .L_80138A78
    lwz      r0, 0xbac(r3)
    cmpwi    r0, 0x5
    beq      .L_80138A78
    lwz      r0, 0xb68(r3)
    cmpwi    r0, 0x5
    beq      .L_80138A78
    lwz      r0, 0xbec(r3)
    cmpwi    r0, 0x2
    beq      .L_80138A78
    addis    r3, r3, 0x1
    lfs      f0, -0x6554(r3)
    stfs     f0, -0x6558(r3)
    b        .L_80138A78
.L_801389AC:
    cmpwi    r30, 0x5
    lfs      f1, 0x34(r29)
    mr       r0, r30
    bne      .L_801389C0
    li       r0, 0x4
.L_801389C0:
    cmpwi    r0, 0x3
    beq      .L_801389D8
    lfs      f0, lbl_8065E93C@sda21(r0)
    fcmpo    cr0, f31, f0
    cror     eq, lt, eq
    bne      .L_801389FC
.L_801389D8:
    li       r0, 0x0
    mr       r3, r29
    stw      r0, 0x6c(r29)
    fmr      f1, f2
    lwz      r12, 0x3c(r29)
    lwz      r12, 0x48(r12)
    mtctr    r12
    bctrl
    b        .L_80138A2C
.L_801389FC:
    slwi     r0, r0, 2
    add      r3, r29, r0
    lwz      r0, 0x268(r3)
    stw      r0, 0x6c(r29)
    lwz      r3, 0x6c(r29)
    cmplwi   r3, 0x0
    beq      .L_80138A2C
    lwz      r12, 0x0(r3)
    fmr      f3, f31
    lwz      r12, 0xc(r12)
    mtctr    r12
    bctrl
.L_80138A2C:
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x1
    lwz      r3, -0x6944(r3)
    lwz      r0, 0xac4(r3)
    cmpwi    r0, 0x5
    beq      .L_80138A78
    lwz      r0, 0xbac(r3)
    cmpwi    r0, 0x5
    beq      .L_80138A78
    lwz      r0, 0xb68(r3)
    cmpwi    r0, 0x5
    beq      .L_80138A78
    lwz      r0, 0xbec(r3)
    cmpwi    r0, 0x2
    beq      .L_80138A78
    addis    r3, r3, 0x1
    lfs      f0, -0x6554(r3)
    stfs     f0, -0x6558(r3)
.L_80138A78:
    psq_l    f31, 0xa8(r1), 0, qr0
    lwz      r0, 0xb4(r1)
    lfd      f31, 0xa0(r1)
    lwz      r31, 0x9c(r1)
    lwz      r30, 0x98(r1)
    lwz      r29, 0x94(r1)
    mtlr     r0
    addi     r1, r1, 0xb0
    blr
*/

/**
 * @note Address: 0x80138A9C
 * @note Size: 0x90
 */
extern "C" void fn_80138A9C(CamArea* area, s32 arg1, s32 arg2, s32 arg3, f32 arg4)
{
	CamAreaEntry* entry = &lbl_805A054C._2034[arg1]._00[arg2];
	area->_54           = entry;
	area->_58           = entry;
	area->_4C           = arg2;
	area->_50           = arg2;
	fn_80137B58(&lbl_805A054C, arg1);
	fn_801382E8(area, entry, arg3, 1, arg4);
}

/**
 * @note Address: 0x80138B2C
 * @note Size: 0x24
 */
extern "C" void fn_80138B2C(CamArea* area, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5)
{
	area->_5C = arg4;
	fn_80138A9C(area, arg1, arg2, arg3, arg5);
}

/**
 * @note Address: 0x80138B50
 * @note Size: 0x154
 */
extern "C" void fn_80138B50(CamArea* area, s32 mode, XObj* target, f32 time)
{
	if (target == NULL) {
		OSReport("SetInte_I( unknown data )\n");
		return;
	}
	area->_64    = target;
	Vec interest = area->_64->GetCameraTarget();
	Vec current  = area->mInterest;
	area->SetInte_I(mode, current, interest, time);
}

// fn_80138B50
/*
    stwu     r1, -0x70(r1)
    mflr     r0
    stw      r0, 0x74(r1)
    stfd     f31, 0x60(r1)
    psq_st   f31, 0x68(r1), 0, qr0
    stw      r31, 0x5c(r1)
    stw      r30, 0x58(r1)
    fmr      f31, f1
    cmplwi   r5, 0x0
    mr       r30, r3
    mr       r31, r4
    bne      .L_80138B94
    lis      r3, lbl_8020C870@ha
    addi     r3, r3, lbl_8020C870@l
    crclr    cr1eq
    bl       OSReport
    b        .L_80138C84
.L_80138B94:
    stw      r5, 0x64(r30)
    addi     r3, r1, 0x2c
    lwz      r4, 0x64(r30)
    lwz      r12, 0x34(r4)
    lwz      r12, 0xec(r12)
    mtctr    r12
    bctrl
    lwz      r4, 0x2c(r1)
    cmpwi    r31, 0x3
    lwz      r3, 0x30(r1)
    lwz      r0, 0x34(r1)
    lwz      r9, 0x10(r30)
    lwz      r8, 0x14(r30)
    lwz      r7, 0x18(r30)
    stw      r4, 0x38(r1)
    stw      r3, 0x3c(r1)
    stw      r0, 0x40(r1)
    stw      r9, 0x44(r1)
    stw      r8, 0x48(r1)
    stw      r7, 0x4c(r1)
    stw      r4, 0x20(r1)
    stw      r3, 0x24(r1)
    stw      r0, 0x28(r1)
    beq      .L_80138C04
    lfs      f0, lbl_8065E93C@sda21(r0)
    fcmpo    cr0, f31, f0
    cror     eq, lt, eq
    bne      .L_80138C28
.L_80138C04:
    li       r0, 0x0
    mr       r3, r30
    stw      r0, 0x27c(r30)
    addi     r4, r1, 0x20
    lwz      r12, 0x3c(r30)
    lwz      r12, 0x3c(r12)
    mtctr    r12
    bctrl
    b        .L_80138C84
.L_80138C28:
    slwi     r0, r31, 2
    add      r3, r30, r0
    lwz      r0, 0x280(r3)
    stw      r0, 0x27c(r30)
    lwz      r3, 0x27c(r30)
    cmplwi   r3, 0x0
    beq      .L_80138C84
    lwz      r5, 0x20(r1)
    fmr      f1, f31
    lwz      r6, 0x24(r1)
    addi     r4, r1, 0x8
    lwz      r0, 0x28(r1)
    stw      r5, 0x14(r1)
    addi     r5, r1, 0x14
    stw      r6, 0x18(r1)
    stw      r0, 0x1c(r1)
    stw      r9, 0x8(r1)
    stw      r8, 0xc(r1)
    stw      r7, 0x10(r1)
    lwz      r12, 0x0(r3)
    lwz      r12, 0xc(r12)
    mtctr    r12
    bctrl
.L_80138C84:
    psq_l    f31, 0x68(r1), 0, qr0
    lwz      r0, 0x74(r1)
    lfd      f31, 0x60(r1)
    lwz      r31, 0x5c(r1)
    lwz      r30, 0x58(r1)
    mtlr     r0
    addi     r1, r1, 0x70
    blr
*/

/**
 * @note Address: 0x80138CA4
 * @note Size: 0x1EC
 */
extern "C" void fn_80138CA4(CamArea* area, s32 mode, f32 time, s32 arg)
{
	area->_5C = 1;
	CVec view = area->mInterest - area->mEye;
	PSVECNormalize(&view, &view);
	fn_80138A9C(area, arg, FindFacing(view, arg), mode, time);
}
/*
    stwu     r1, -0xd0(r1)
    mflr     r0
    stw      r0, 0xd4(r1)
    stfd     f31, 0xc0(r1)
    psq_st   f31, 0xc8(r1), 0, qr0
    stfd     f30, 0xb0(r1)
    psq_st   f30, 0xb8(r1), 0, qr0
    stfd     f29, 0xa0(r1)
    psq_st   f29, 0xa8(r1), 0, qr0
    stfd     f28, 0x90(r1)
    psq_st   f28, 0x98(r1), 0, qr0
    stmw     r24, 0x70(r1)
    mr       r29, r3
    li       r0, 0x1
    stw      r0, 0x5c(r3)
    mr       r30, r4
    mr       r31, r5
    fmr      f28, f1
    lwz      r6, 0x0(r3)
    addi     r3, r1, 0x38
    lwz      r0, 0x4(r29)
    addi     r4, r29, 0x10
    addi     r5, r1, 0x48
    stw      r6, 0x48(r1)
    stw      r0, 0x4c(r1)
    lwz      r0, 0x8(r29)
    stw      r0, 0x50(r1)
    bl       fn_800244A0
    lwz      r4, 0x38(r1)
    lis      r3, __vt__4CVec@ha
    addi     r0, r3, __vt__4CVec@l
    lwz      r6, 0x3c(r1)
    lwz      r5, 0x40(r1)
    addi     r3, r1, 0x54
    stw      r4, 0x54(r1)
    mr       r4, r3
    stw      r6, 0x58(r1)
    stw      r5, 0x5c(r1)
    stw      r0, 0x60(r1)
    stw      r0, 0x44(r1)
    bl       PSVECNormalize
    mulli    r4, r31, 0x1c
    lis      r3, lbl_805A054C@ha
    lfs      f2, 0x54(r1)
    lis      r5, __vt__4CVec@ha
    addi     r0, r3, lbl_805A054C@l
    lfs      f1, 0x58(r1)
    add      r3, r0, r4
    lfs      f0, 0x5c(r1)
    addi     r28, r5, __vt__4CVec@l
    stfs     f2, 0x8(r1)
    addi     r24, r3, 0x2034
    lfs      f30, lbl_8065E954@sda21(r0)
    stw      r28, 0x14(r1)
    li       r25, -0x1
    lwz      r26, 0x2034(r3)
    li       r27, 0x0
    stfs     f1, 0xc(r1)
    lfs      f31, lbl_8065E93C@sda21(r0)
    stfs     f0, 0x10(r1)
    b        .L_80138E10
.L_80138D98:
    stw      r28, 0x24(r1)
    lfs      f1, 0x24(r26)
    lfs      f2, 0x20(r26)
    lfs      f0, 0x28(r26)
    stw      r28, 0x34(r1)
    stfs     f2, 0x28(r1)
    stfs     f1, 0x2c(r1)
    stfs     f0, 0x30(r1)
    bl       sin
    frsp     f29, f1
    lfs      f1, 0x2c(r1)
    bl       cos
    frsp     f0, f1
    addi     r3, r1, 0x18
    stfs     f29, 0x18(r1)
    mr       r4, r3
    stfs     f31, 0x1c(r1)
    stfs     f0, 0x20(r1)
    bl       PSVECNormalize
    stw      r28, 0x34(r1)
    addi     r3, r1, 0x8
    addi     r4, r1, 0x18
    bl       PSVECDotProduct
    fcmpo    cr0, f1, f30
    ble      .L_80138E04
    mr       r25, r27
    fmr      f30, f1
.L_80138E04:
    stw      r28, 0x24(r1)
    addi     r27, r27, 0x1
    addi     r26, r26, 0x38
.L_80138E10:
    lwz      r0, 0x8(r24)
    cmpw     r27, r0
    blt      .L_80138D98
    lfs      f0, lbl_8065E954@sda21(r0)
    fcmpu    cr0, f0, f30
    bne      .L_80138E2C
    li       r25, 0x0
.L_80138E2C:
    lis      r3, __vt__4CVec@ha
    fmr      f1, f28
    addi     r0, r3, __vt__4CVec@l
    mr       r3, r29
    stw      r0, 0x14(r1)
    mr       r4, r31
    mr       r5, r25
    mr       r6, r30
    bl       fn_80138A9C
    lis      r3, __vt__4CVec@ha
    addi     r0, r3, __vt__4CVec@l
    stw      r0, 0x60(r1)
    psq_l    f31, 0xc8(r1), 0, qr0
    lfd      f31, 0xc0(r1)
    psq_l    f30, 0xb8(r1), 0, qr0
    lfd      f30, 0xb0(r1)
    psq_l    f29, 0xa8(r1), 0, qr0
    lfd      f29, 0xa0(r1)
    psq_l    f28, 0x98(r1), 0, qr0
    lfd      f28, 0x90(r1)
    lmw      r24, 0x70(r1)
    lwz      r0, 0xd4(r1)
    mtlr     r0
    addi     r1, r1, 0xd0
    blr
*/

/**
 * @note Address: 0x80138E90
 * @note Size: 0x2C
 */
extern "C" void fn_80138E90(CamArea* area, s32 mode, f32 time)
{
	fn_80138CA4(area, mode, time, lbl_805A054C._0C);
}

/**
 * @note Address: 0x80138EBC
 * @note Size: 0x1C
 */
extern "C" void fn_80138EBC(CamArea* area)
{
	area->_5C        = 1;
	lbl_805A054C._18 = lbl_805A054C._14;
}

/**
 * @note Address: 0x80138ED8
 * @note Size: 0x6E0
 */
// fn_80138ED8
/*
    stwu     r1, -0xf0(r1)
    mflr     r0
    stw      r0, 0xf4(r1)
    stfd     f31, 0xe0(r1)
    psq_st   f31, 0xe8(r1), 0, qr0
    stfd     f30, 0xd0(r1)
    psq_st   f30, 0xd8(r1), 0, qr0
    stfd     f29, 0xc0(r1)
    psq_st   f29, 0xc8(r1), 0, qr0
    stfd     f28, 0xb0(r1)
    psq_st   f28, 0xb8(r1), 0, qr0
    stmw     r26, 0x98(r1)
    mr       r28, r3
    addi     r3, r1, 0x8
    li       r4, 0x0
    bl       fn_8019E3C8
    lis      r3, lbl_8025C400@ha
    lis      r4, lbl_805A054C@ha
    addi     r3, r3, lbl_8025C400@l
    li       r0, 0x20
    addi     r31, r4, lbl_805A054C@l
    lwz      r9, 0x1b14(r3)
    lwz      r4, 0x1b10(r3)
    li       r7, 0x0
    and      r3, r9, r0
    lwz      r29, 0xc(r31)
    and      r6, r4, r7
    lwz      r0, 0x4(r31)
    xor      r5, r3, r7
    lwz      r3, 0x8(r31)
    xor      r4, r6, r7
    lwz      r8, 0x14(r31)
    or.      r4, r5, r4
    beq      .L_80138F78
    lwz      r4, 0x54(r28)
    lis      r3, __vt__4CPad@ha
    addi     r0, r3, __vt__4CPad@l
    lwz      r3, 0x34(r4)
    stw      r0, 0xc(r1)
    b        .L_80139584
.L_80138F78:
    lis      r5, 0x8000
    xor      r4, r6, r7
    and      r9, r9, r5
    xor      r5, r9, r7
    or.      r4, r5, r4
    beq      .L_80138F98
    li       r7, 0x8
    b        .L_80138F9C
.L_80138F98:
    lis      r7, 0x10
.L_80138F9C:
    li       r4, 0x0
    xor      r5, r9, r4
    xor      r4, r6, r4
    or.      r4, r5, r4
    beq      .L_80138FB8
    li       r10, 0x4
    b        .L_80138FBC
.L_80138FB8:
    lis      r10, 0x20
.L_80138FBC:
    li       r4, 0x0
    xor      r5, r9, r4
    xor      r4, r6, r4
    or.      r4, r5, r4
    beq      .L_80138FD8
    li       r11, 0x1
    b        .L_80138FDC
.L_80138FD8:
    lis      r11, 0x40
.L_80138FDC:
    li       r4, 0x0
    xor      r5, r9, r4
    xor      r4, r6, r4
    or.      r4, r5, r4
    beq      .L_80138FF8
    li       r9, 0x2
    b        .L_80138FFC
.L_80138FF8:
    lis      r9, 0x80
.L_80138FFC:
    lwz      r4, 0x8(r1)
    lwz      r5, 0x14(r4)
    and.     r4, r5, r7
    beq      .L_80139034
    cmpwi    r8, 0x1
    ble      .L_8013954C
    subi     r0, r8, 0x1
    lis      r3, __vt__4CPad@ha
    stw      r0, 0x14(r31)
    addi     r0, r3, __vt__4CPad@l
    lwz      r3, 0x54(r28)
    lwz      r3, 0x34(r3)
    stw      r0, 0xc(r1)
    b        .L_80139584
.L_80139034:
    and.     r4, r5, r10
    beq      .L_80139064
    cmpwi    r8, 0x2
    bge      .L_8013954C
    addi     r0, r8, 0x1
    lis      r3, __vt__4CPad@ha
    stw      r0, 0x14(r31)
    addi     r0, r3, __vt__4CPad@l
    lwz      r3, 0x54(r28)
    lwz      r3, 0x34(r3)
    stw      r0, 0xc(r1)
    b        .L_80139584
.L_80139064:
    lwz      r6, 0x44(r28)
    cmpwi    r6, 0x1
    bne      .L_80139198
    and.     r4, r5, r11
    beq      .L_80139198
    lfs      f3, 0x0(r28)
    lfs      f1, 0x10(r28)
    lfs      f2, 0x8(r28)
    lfs      f0, 0x18(r28)
    fsubs    f1, f3, f1
    fsubs    f2, f2, f0
    bl       atan2
    frsp     f28, f1
    lfs      f1, lbl_8065E958@sda21(r0)
    lfs      f0, lbl_8065E93C@sda21(r0)
    b        .L_801390A8
.L_801390A4:
    fadds    f28, f28, f1
.L_801390A8:
    fcmpo    cr0, f28, f0
    blt      .L_801390A4
    lfs      f29, lbl_8065E958@sda21(r0)
    b        .L_801390BC
.L_801390B8:
    fsubs    f28, f28, f29
.L_801390BC:
    fcmpo    cr0, f28, f29
    cror     eq, gt, eq
    beq      .L_801390B8
    lwz      r4, 0xc(r31)
    lis      r3, lbl_805A054C@ha
    addi     r0, r3, lbl_805A054C@l
    lfs      f30, lbl_8065E958@sda21(r0)
    mulli    r3, r4, 0x1c
    lfs      f31, lbl_8065E93C@sda21(r0)
    li       r31, 0x0
    add      r3, r0, r3
    lwz      r27, 0x2034(r3)
    addi     r26, r3, 0x2034
    b        .L_8013917C
.L_801390F4:
    lfs      f3, 0x10(r27)
    lfs      f1, 0x10(r28)
    lfs      f2, 0x18(r27)
    lfs      f0, 0x18(r28)
    fsubs    f1, f3, f1
    fsubs    f2, f2, f0
    bl       atan2
    frsp     f0, f1
    b        .L_8013911C
.L_80139118:
    fadds    f0, f0, f30
.L_8013911C:
    fcmpo    cr0, f0, f31
    blt      .L_80139118
    b        .L_8013912C
.L_80139128:
    fsubs    f0, f0, f30
.L_8013912C:
    fcmpo    cr0, f0, f30
    cror     eq, gt, eq
    beq      .L_80139128
    fsubs    f0, f28, f0
    b        .L_80139144
.L_80139140:
    fadds    f0, f0, f30
.L_80139144:
    fcmpo    cr0, f0, f31
    blt      .L_80139140
    b        .L_80139154
.L_80139150:
    fsubs    f0, f0, f30
.L_80139154:
    fcmpo    cr0, f0, f30
    cror     eq, gt, eq
    beq      .L_80139150
    fcmpo    cr0, f0, f29
    cror     eq, lt, eq
    bne      .L_80139174
    fmr      f29, f0
    mr       r30, r31
.L_80139174:
    addi     r27, r27, 0x38
    addi     r31, r31, 0x1
.L_8013917C:
    lwz      r0, 0x8(r26)
    cmpw     r31, r0
    blt      .L_801390F4
    stw      r30, 0x4c(r28)
    li       r0, 0x1
    stw      r0, 0x48(r28)
    b        .L_8013954C
.L_80139198:
    cmpwi    r6, 0x1
    bne      .L_801392D4
    and.     r4, r5, r9
    beq      .L_801392D4
    lfs      f3, 0x0(r28)
    lfs      f1, 0x10(r28)
    lfs      f2, 0x8(r28)
    lfs      f0, 0x18(r28)
    fsubs    f1, f3, f1
    fsubs    f2, f2, f0
    bl       atan2
    frsp     f28, f1
    lfs      f1, lbl_8065E958@sda21(r0)
    lfs      f0, lbl_8065E93C@sda21(r0)
    b        .L_801391D8
.L_801391D4:
    fadds    f28, f28, f1
.L_801391D8:
    fcmpo    cr0, f28, f0
    blt      .L_801391D4
    lfs      f29, lbl_8065E958@sda21(r0)
    b        .L_801391EC
.L_801391E8:
    fsubs    f28, f28, f29
.L_801391EC:
    fcmpo    cr0, f28, f29
    cror     eq, gt, eq
    beq      .L_801391E8
    lwz      r4, 0xc(r31)
    lis      r3, lbl_805A054C@ha
    addi     r0, r3, lbl_805A054C@l
    lfs      f31, lbl_8065E958@sda21(r0)
    mulli    r3, r4, 0x1c
    lfs      f30, lbl_8065E93C@sda21(r0)
    li       r27, 0x0
    add      r3, r0, r3
    lwz      r31, 0x2034(r3)
    addi     r26, r3, 0x2034
    b        .L_801392AC
.L_80139224:
    lfs      f3, 0x10(r31)
    lfs      f1, 0x10(r28)
    lfs      f2, 0x18(r31)
    lfs      f0, 0x18(r28)
    fsubs    f1, f3, f1
    fsubs    f2, f2, f0
    bl       atan2
    frsp     f0, f1
    b        .L_8013924C
.L_80139248:
    fadds    f0, f0, f31
.L_8013924C:
    fcmpo    cr0, f0, f30
    blt      .L_80139248
    b        .L_8013925C
.L_80139258:
    fsubs    f0, f0, f31
.L_8013925C:
    fcmpo    cr0, f0, f31
    cror     eq, gt, eq
    beq      .L_80139258
    fsubs    f0, f28, f0
    b        .L_80139274
.L_80139270:
    fadds    f0, f0, f31
.L_80139274:
    fcmpo    cr0, f0, f30
    blt      .L_80139270
    b        .L_80139284
.L_80139280:
    fsubs    f0, f0, f31
.L_80139284:
    fcmpo    cr0, f0, f31
    cror     eq, gt, eq
    beq      .L_80139280
    fcmpo    cr0, f0, f29
    cror     eq, lt, eq
    bne      .L_801392A4
    fmr      f29, f0
    mr       r30, r27
.L_801392A4:
    addi     r31, r31, 0x38
    addi     r27, r27, 0x1
.L_801392AC:
    lwz      r3, 0x8(r26)
    cmpw     r27, r3
    blt      .L_80139224
    subic.   r0, r30, 0x1
    bge      .L_801392C4
    subi     r0, r3, 0x1
.L_801392C4:
    stw      r0, 0x4c(r28)
    li       r0, 0x1
    stw      r0, 0x48(r28)
    b        .L_8013954C
.L_801392D4:
    and.     r4, r5, r11
    beq      .L_80139310
    lwz      r5, 0x4c(r28)
    lis      r3, lbl_805A054C@ha
    mulli    r4, r29, 0x1c
    addi     r5, r5, 0x1
    addi     r0, r3, lbl_805A054C@l
    stw      r5, 0x4c(r28)
    add      r3, r0, r4
    lwz      r0, 0x203c(r3)
    cmpw     r5, r0
    blt      .L_8013954C
    li       r0, 0x0
    stw      r0, 0x4c(r28)
    b        .L_8013954C
.L_80139310:
    and.     r4, r5, r9
    beq      .L_8013934C
    lwz      r3, 0x4c(r28)
    mulli    r5, r29, 0x1c
    lis      r4, lbl_805A054C@ha
    subic.   r0, r3, 0x1
    addi     r3, r4, lbl_805A054C@l
    add      r3, r3, r5
    stw      r0, 0x4c(r28)
    addi     r3, r3, 0x2034
    bge      .L_8013954C
    lwz      r3, 0x8(r3)
    subi     r0, r3, 0x1
    stw      r0, 0x4c(r28)
    b        .L_8013954C
.L_8013934C:
    cmpw     r0, r3
    beq      .L_8013954C
    cmpwi    r0, -0x1
    beq      .L_8013954C
    lis      r3, __vt__4CVec@ha
    addi     r4, r3, __vt__4CVec@l
    stw      r4, 0x88(r1)
    lwz      r0, 0x44(r28)
    cmpwi    r0, 0x0
    beq      .L_801393F8
    lwz      r6, 0x0(r28)
    addi     r3, r1, 0x40
    lwz      r0, 0x4(r28)
    addi     r4, r28, 0x10
    addi     r5, r1, 0x50
    stw      r6, 0x50(r1)
    stw      r0, 0x54(r1)
    lwz      r0, 0x8(r28)
    stw      r0, 0x58(r1)
    bl       fn_800244A0
    lwz      r4, 0x40(r1)
    lis      r3, __vt__4CVec@ha
    addi     r0, r3, __vt__4CVec@l
    lwz      r6, 0x44(r1)
    lwz      r5, 0x48(r1)
    addi     r3, r1, 0x6c
    stw      r4, 0x6c(r1)
    mr       r4, r3
    stw      r6, 0x70(r1)
    stw      r5, 0x74(r1)
    stw      r0, 0x78(r1)
    stw      r0, 0x4c(r1)
    bl       PSVECNormalize
    lfs      f2, 0x6c(r1)
    lis      r3, __vt__4CVec@ha
    lfs      f1, 0x70(r1)
    addi     r0, r3, __vt__4CVec@l
    lfs      f0, 0x74(r1)
    stfs     f2, 0x7c(r1)
    stfs     f1, 0x80(r1)
    stfs     f0, 0x84(r1)
    stw      r0, 0x78(r1)
    b        .L_80139454
.L_801393F8:
    lwz      r3, 0x54(r28)
    stw      r4, 0x68(r1)
    lfs      f1, 0x24(r3)
    lfs      f2, 0x20(r3)
    lfs      f0, 0x28(r3)
    stfs     f2, 0x5c(r1)
    stfs     f1, 0x60(r1)
    stfs     f0, 0x64(r1)
    bl       sin
    frsp     f28, f1
    lfs      f1, 0x60(r1)
    bl       cos
    frsp     f1, f1
    lfs      f0, lbl_8065E93C@sda21(r0)
    addi     r3, r1, 0x7c
    stfs     f28, 0x7c(r1)
    mr       r4, r3
    stfs     f0, 0x80(r1)
    stfs     f1, 0x84(r1)
    bl       PSVECNormalize
    lis      r3, __vt__4CVec@ha
    addi     r0, r3, __vt__4CVec@l
    stw      r0, 0x68(r1)
.L_80139454:
    lwz      r0, 0xc(r31)
    lis      r3, lbl_805A054C@ha
    lfs      f2, 0x7c(r1)
    lis      r5, __vt__4CVec@ha
    mulli    r4, r0, 0x1c
    addi     r0, r3, lbl_805A054C@l
    lfs      f1, 0x80(r1)
    addi     r30, r5, __vt__4CVec@l
    lfs      f0, 0x84(r1)
    li       r27, 0x0
    add      r3, r0, r4
    stw      r30, 0x1c(r1)
    addi     r26, r3, 0x2034
    lfs      f29, lbl_8065E954@sda21(r0)
    stfs     f2, 0x10(r1)
    lwz      r31, 0x2034(r3)
    stfs     f1, 0x14(r1)
    lfs      f31, lbl_8065E93C@sda21(r0)
    stfs     f0, 0x18(r1)
    b        .L_8013951C
.L_801394A4:
    stw      r30, 0x2c(r1)
    lfs      f1, 0x24(r31)
    lfs      f2, 0x20(r31)
    lfs      f0, 0x28(r31)
    stw      r30, 0x3c(r1)
    stfs     f2, 0x30(r1)
    stfs     f1, 0x34(r1)
    stfs     f0, 0x38(r1)
    bl       sin
    frsp     f28, f1
    lfs      f1, 0x34(r1)
    bl       cos
    frsp     f0, f1
    addi     r3, r1, 0x20
    stfs     f28, 0x20(r1)
    mr       r4, r3
    stfs     f31, 0x24(r1)
    stfs     f0, 0x28(r1)
    bl       PSVECNormalize
    stw      r30, 0x3c(r1)
    addi     r3, r1, 0x10
    addi     r4, r1, 0x20
    bl       PSVECDotProduct
    fcmpo    cr0, f1, f29
    ble      .L_80139510
    stw      r27, 0x4c(r28)
    fmr      f29, f1
.L_80139510:
    stw      r30, 0x2c(r1)
    addi     r27, r27, 0x1
    addi     r31, r31, 0x38
.L_8013951C:
    lwz      r0, 0x8(r26)
    cmpw     r27, r0
    blt      .L_801394A4
    lfs      f0, lbl_8065E954@sda21(r0)
    fcmpu    cr0, f0, f29
    bne      .L_8013953C
    li       r0, 0x0
    stw      r0, 0x4c(r28)
.L_8013953C:
    lis      r3, __vt__4CVec@ha
    addi     r0, r3, __vt__4CVec@l
    stw      r0, 0x1c(r1)
    stw      r0, 0x88(r1)
.L_8013954C:
    mulli    r0, r29, 0x1c
    lis      r5, lbl_805A054C@ha
    lwz      r4, 0x4c(r28)
    lis      r3, __vt__4CPad@ha
    addi     r5, r5, lbl_805A054C@l
    add      r5, r5, r0
    addi     r0, r3, __vt__4CPad@l
    mulli    r3, r4, 0x38
    lwz      r4, 0x2034(r5)
    add      r3, r4, r3
    stw      r3, 0x54(r28)
    lwz      r3, 0x54(r28)
    lwz      r3, 0x34(r3)
    stw      r0, 0xc(r1)
.L_80139584:
    psq_l    f31, 0xe8(r1), 0, qr0
    lfd      f31, 0xe0(r1)
    psq_l    f30, 0xd8(r1), 0, qr0
    lfd      f30, 0xd0(r1)
    psq_l    f29, 0xc8(r1), 0, qr0
    lfd      f29, 0xc0(r1)
    psq_l    f28, 0xb8(r1), 0, qr0
    lfd      f28, 0xb0(r1)
    lmw      r26, 0x98(r1)
    lwz      r0, 0xf4(r1)
    mtlr     r0
    addi     r1, r1, 0xf0
    blr
*/

/**
 * @note Address: 0x801395B8
 * @note Size: 0x94
 */
extern "C" void fn_801395B8(CamArea* area)
{
	fn_80194FD4(lbl_8025C400, -1, "\\B000000A0AREA(%3d,%3d)(%3d)\n", 324.0f, 298.0f, 0.0f, 12.0f, 12.0f, lbl_805A054C._0C, area->_4C,
	            lbl_805A054C._04);
	fn_80194FD4(lbl_8025C400, -1, "\\B000000A0ACAREA\n", 324.0f, 310.0f, 0.0f, 12.0f, 12.0f);
}

/**
 * @note Address: 0x8013964C
 * @note Size: 0x544
 */
// fn_8013964C
/*
    stwu     r1, -0xa0(r1)
    mflr     r0
    lis      r5, lbl_8025C400@ha
    li       r6, 0xc00
    stw      r0, 0xa4(r1)
    addi     r7, r5, lbl_8025C400@l
    li       r5, 0x0
    stw      r31, 0x9c(r1)
    stw      r30, 0x98(r1)
    mr       r30, r4
    stw      r29, 0x94(r1)
    mr       r29, r3
    stw      r28, 0x90(r1)
    lwz      r0, 0x1b10(r7)
    lwz      r3, 0x1b14(r7)
    and      r0, r0, r5
    and      r3, r3, r6
    xor      r3, r3, r5
    xor      r0, r0, r5
    or.      r0, r3, r0
    beq      .L_801396A8
    stw      r5, 0x60(r29)
    b        .L_801396B0
.L_801396A8:
    li       r0, 0x1
    stw      r0, 0x60(r29)
.L_801396B0:
    lwz      r3, 0x64(r29)
    cmplwi   r3, 0x0
    beq      .L_801397DC
    lwz      r0, 0x0(r3)
    cmplwi   r0, 0x0
    beq      .L_801397DC
    addi     r3, r1, 0x8
    li       r4, 0x0
    bl       fn_8019E3C8
    lwz      r3, 0x64(r29)
    lwz      r28, 0x0(r3)
    cmplwi   r28, 0x0
    bne      .L_801396F4
    li       r3, lbl_8065E968@sda21
    li       r4, 0x47c
    li       r5, lbl_8065E970@sda21
    bl       __assert
.L_801396F4:
    cmplwi   r28, 0x0
    beq      .L_8013973C
    bne      .L_80139710
    li       r3, lbl_8065E968@sda21
    li       r4, 0x25d
    li       r5, lbl_8065E970@sda21
    bl       __assert
.L_80139710:
    lwz      r4, 0x14(r28)
    li       r3, 0x0
    rlwinm.  r0, r4, 0, 8, 8
    bne      .L_8013972C
    rlwinm.  r0, r4, 0, 25, 25
    beq      .L_8013972C
    li       r3, 0x1
.L_8013972C:
    clrlwi.  r0, r3, 24
    beq      .L_8013973C
    mr       r3, r28
    bl       fn_801C8730
.L_8013973C:
    addi     r3, r28, 0x44
    addi     r4, r1, 0x54
    bl       PSMTXCopy
    lwz      r5, 0x0(r29)
    lis      r3, __vt__4CVec@ha
    lwz      r4, 0x4(r29)
    addi     r0, r3, __vt__4CVec@l
    addi     r3, r1, 0x44
    stw      r5, 0x44(r1)
    stw      r4, 0x48(r1)
    lwz      r4, 0x8(r29)
    stw      r4, 0x4c(r1)
    stw      r0, 0x50(r1)
    lwz      r4, 0x64(r29)
    lfs      f0, 0xcc(r4)
    addi     r4, r4, 0xc8
    stfs     f0, 0x48(r1)
    bl       PSVECDistance
    lfs      f3, 0x4(r29)
    fneg     f0, f1
    lfs      f2, 0x48(r1)
    lis      r3, __vt__4CVec@ha
    lfs      f1, lbl_8065E93C@sda21(r0)
    addi     r0, r3, __vt__4CVec@l
    fsubs    f2, f3, f2
    addi     r4, r1, 0x34
    stw      r0, 0x40(r1)
    mr       r5, r4
    addi     r3, r1, 0x54
    stfs     f1, 0x34(r1)
    stfs     f2, 0x38(r1)
    stfs     f0, 0x3c(r1)
    bl       PSMTXMultVec
    lis      r4, __vt__4CVec@ha
    lis      r3, __vt__4CPad@ha
    addi     r4, r4, __vt__4CVec@l
    addi     r0, r3, __vt__4CPad@l
    stw      r4, 0x40(r1)
    stw      r4, 0x50(r1)
    stw      r0, 0xc(r1)
.L_801397DC:
    lwz      r0, 0x5c(r29)
    cmpwi    r0, 0x1
    bne      .L_801397FC
    lwz      r0, 0x60(r29)
    cmpwi    r0, 0x0
    beq      .L_801397FC
    mr       r3, r29
    bl       fn_80138ED8
.L_801397FC:
    lwz      r4, 0x54(r29)
    lis      r5, lbl_805A054C@ha
    lwz      r3, 0x58(r29)
    addi     r5, r5, lbl_805A054C@l
    lwz      r4, 0x34(r4)
    lwz      r0, 0x34(r3)
    lwz      r3, 0x4(r5)
    cmpw     r4, r0
    lwz      r4, 0x8(r5)
    bne      .L_80139830
    lwz      r0, 0x48(r29)
    cmpwi    r0, 0x0
    beq      .L_80139934
.L_80139830:
    cmpw     r3, r4
    beq      .L_80139864
    cmpwi    r3, -0x1
    beq      .L_80139864
    lfs      f0, lbl_8065E950@sda21(r0)
    mr       r3, r29
    li       r5, 0x1
    li       r6, 0x1
    stfs     f0, 0x40(r29)
    lwz      r4, 0x54(r29)
    lfs      f1, 0x40(r29)
    bl       fn_801382E8
    b        .L_801398C4
.L_80139864:
    lwz      r0, 0x48(r29)
    cmpwi    r0, 0x0
    beq      .L_8013989C
    lfs      f0, lbl_8065E978@sda21(r0)
    mr       r3, r29
    li       r5, 0x5
    li       r6, 0x1
    stfs     f0, 0x40(r29)
    lwz      r4, 0x54(r29)
    lfs      f1, 0x40(r29)
    bl       fn_801382E8
    li       r0, 0x0
    stw      r0, 0x48(r29)
    b        .L_801398C4
.L_8013989C:
    cmpw     r3, r4
    bne      .L_801398C4
    lfs      f0, lbl_8065E978@sda21(r0)
    mr       r3, r29
    li       r5, 0x5
    li       r6, 0x1
    stfs     f0, 0x40(r29)
    lwz      r4, 0x54(r29)
    lfs      f1, 0x40(r29)
    bl       fn_801382E8
.L_801398C4:
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x1
    lwz      r3, -0x6944(r3)
    lwz      r0, 0xac4(r3)
    cmpwi    r0, 0x5
    beq      .L_80139910
    lwz      r0, 0xbac(r3)
    cmpwi    r0, 0x5
    beq      .L_80139910
    lwz      r0, 0xb68(r3)
    cmpwi    r0, 0x5
    beq      .L_80139910
    lwz      r0, 0xbec(r3)
    cmpwi    r0, 0x2
    beq      .L_80139910
    addis    r3, r3, 0x1
    lfs      f0, -0x6554(r3)
    stfs     f0, -0x6558(r3)
.L_80139910:
    lwz      r0, 0x4c(r29)
    stw      r0, 0x50(r29)
    lwz      r0, 0x54(r29)
    stw      r0, 0x58(r29)
    lwz      r0, 0x44(r29)
    cmpwi    r0, 0x0
    beq      .L_80139934
    li       r0, 0x0
    stw      r0, 0x44(r29)
.L_80139934:
    lis      r3, lbl_805A054C@ha
    addi     r28, r3, lbl_805A054C@l
    lwz      r31, 0x14(r28)
    lwz      r0, 0x18(r28)
    cmpw     r31, r0
    beq      .L_801399FC
    mr       r3, r28
    mr       r4, r31
    bl       fn_801353E4
    fmr      f2, f1
    lfs      f0, 0x34(r29)
    li       r0, 0x4
    b        .L_80139974
    stw      r0, 0x6c(r29)
    bctrl
    b        .L_801399A8
.L_80139974:
    slwi     r0, r0, 2
    add      r3, r29, r0
    lwz      r0, 0x268(r3)
    stw      r0, 0x6c(r29)
    lwz      r3, 0x6c(r29)
    cmplwi   r3, 0x0
    beq      .L_801399A8
    lwz      r12, 0x0(r3)
    fmr      f1, f0
    lfs      f3, lbl_8065E978@sda21(r0)
    lwz      r12, 0xc(r12)
    mtctr    r12
    bctrl
.L_801399A8:
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x1
    lwz      r3, -0x6944(r3)
    lwz      r0, 0xac4(r3)
    cmpwi    r0, 0x5
    beq      .L_801399F4
    lwz      r0, 0xbac(r3)
    cmpwi    r0, 0x5
    beq      .L_801399F4
    lwz      r0, 0xb68(r3)
    cmpwi    r0, 0x5
    beq      .L_801399F4
    lwz      r0, 0xbec(r3)
    cmpwi    r0, 0x2
    beq      .L_801399F4
    addis    r3, r3, 0x1
    lfs      f0, -0x6554(r3)
    stfs     f0, -0x6558(r3)
.L_801399F4:
    stw      r31, 0x14(r28)
    stw      r31, 0x18(r28)
.L_801399FC:
    lwz      r3, 0x68(r29)
    cmplwi   r3, 0x0
    beq      .L_80139A64
    lwz      r12, 0x0(r3)
    mr       r4, r30
    lfs      f1, lbl_8065E97C@sda21(r0)
    lwz      r12, 0x14(r12)
    mtctr    r12
    bctrl
    clrlwi.  r0, r3, 24
    bne      .L_80139A5C
    lwz      r4, 0x68(r29)
    addi     r3, r1, 0x28
    lwz      r12, 0x0(r4)
    lwz      r12, 0x18(r12)
    mtctr    r12
    bctrl
    mr       r3, r29
    addi     r4, r1, 0x28
    lwz      r12, 0x3c(r29)
    lwz      r12, 0x38(r12)
    mtctr    r12
    bctrl
    b        .L_80139A64
.L_80139A5C:
    li       r0, 0x0
    stw      r0, 0x68(r29)
.L_80139A64:
    lwz      r3, 0x6c(r29)
    cmplwi   r3, 0x0
    beq      .L_80139AC4
    lwz      r12, 0x0(r3)
    mr       r4, r30
    lfs      f1, lbl_8065E97C@sda21(r0)
    lwz      r12, 0x14(r12)
    mtctr    r12
    bctrl
    clrlwi.  r0, r3, 24
    bne      .L_80139ABC
    lwz      r3, 0x6c(r29)
    lwz      r12, 0x0(r3)
    lwz      r12, 0x18(r12)
    mtctr    r12
    bctrl
    mr       r3, r29
    lwz      r12, 0x3c(r29)
    lwz      r12, 0x48(r12)
    mtctr    r12
    bctrl
    b        .L_80139AC4
.L_80139ABC:
    li       r0, 0x0
    stw      r0, 0x6c(r29)
.L_80139AC4:
    lwz      r3, 0x27c(r29)
    cmplwi   r3, 0x0
    beq      .L_80139B2C
    lwz      r12, 0x0(r3)
    mr       r4, r30
    lfs      f1, lbl_8065E97C@sda21(r0)
    lwz      r12, 0x14(r12)
    mtctr    r12
    bctrl
    clrlwi.  r0, r3, 24
    bne      .L_80139B24
    lwz      r4, 0x27c(r29)
    addi     r3, r1, 0x1c
    lwz      r12, 0x0(r4)
    lwz      r12, 0x18(r12)
    mtctr    r12
    bctrl
    mr       r3, r29
    addi     r4, r1, 0x1c
    lwz      r12, 0x3c(r29)
    lwz      r12, 0x3c(r12)
    mtctr    r12
    bctrl
    b        .L_80139B2C
.L_80139B24:
    li       r0, 0x0
    stw      r0, 0x27c(r29)
.L_80139B2C:
    lwz      r0, 0x27c(r29)
    cmplwi   r0, 0x0
    bne      .L_80139B70
    lwz      r4, 0x64(r29)
    cmplwi   r4, 0x0
    beq      .L_80139B70
    lwz      r12, 0x34(r4)
    addi     r3, r1, 0x10
    lwz      r12, 0xec(r12)
    mtctr    r12
    bctrl
    mr       r3, r29
    addi     r4, r1, 0x10
    lwz      r12, 0x3c(r29)
    lwz      r12, 0x3c(r12)
    mtctr    r12
    bctrl
.L_80139B70:
    lwz      r0, 0xa4(r1)
    lwz      r31, 0x9c(r1)
    lwz      r30, 0x98(r1)
    lwz      r29, 0x94(r1)
    lwz      r28, 0x90(r1)
    mtlr     r0
    addi     r1, r1, 0xa0
    blr
*/

/**
 * @note Address: 0x80139B90
 * @note Size: 0x108
 */
extern "C" void fn_80139B90(CamArea* area)
{
	if (area->_5C == 1) {
		fn_80137D00(&lbl_805A054C, 0);
	}
	if (lbl_805A054C._04 != lbl_805A054C._08 && lbl_805A054C._04 != -1 && fn_80135D28(&lbl_805A054C) == 7) {
		CMotionControlCamera* camera = GAME_CAMERA;
		camera->ChangeAction(7);
		lbl_805A054C._18 = lbl_805A054C._14;
		fn_8013C510(GAME_MAP, lbl_805A054C._14, 15.0f);
		fn_8013BCE4(GAME_MAP);
		f32 dy = camera->mCamera.mInterest.y - area->_64->GetCameraTarget().y;
		if (0.0f != dy) {
			fn_8013A204(GAME_MAP, dy, 0.0f, 8.0f);
		}
	}
}
