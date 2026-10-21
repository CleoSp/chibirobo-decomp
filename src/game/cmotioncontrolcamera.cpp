#include "game/cmotioncontrolcamera.h"
#include "game/cobj.h"
#include "game/cgame.h"
#include "Version.h"
#include <math.h>
#include <string.h>

extern "C" {
#include <dolphin/os.h>
}

// game object fields cgame.h doesn't have, by offset
#define GAME_BYTES  ((u8*)&lbl_8025C400)
#define GAME_CAMERA ((CMotionControlCamera*)lbl_8025C400._E5D38)
// japan's flags and player sit lower, europe's flags higher
#if defined(VERSION_JP)
#define GAME_FLAGS  (*(u64*)(GAME_BYTES + 0x1AF8))
#define GAME_PLAYER (*(u8**)(GAME_BYTES + 0x9674))
#elif defined(VERSION_PAL)
#define GAME_FLAGS  (*(u64*)(GAME_BYTES + 0x1B18))
#define GAME_PLAYER (*(u8**)(GAME_BYTES + 0x96BC))
#else
#define GAME_FLAGS  (*(u64*)(GAME_BYTES + 0x1B10))
#define GAME_PLAYER (*(u8**)(GAME_BYTES + 0x96BC))
#endif
#if defined(VERSION_JP)
#define GAME_SCREEN ((void*)(GAME_BYTES + 0x4178))
#else
#define GAME_SCREEN ((void*)(GAME_BYTES + 0x4198))
#endif
#define GAME_SOUND   ((void*)&lbl_8025C400._E7E24)
#define GAME_DEMO    (lbl_8025C400._5D080)
#define GAME_ROOM    (*(s32*)(GAME_BYTES + 0x3444))
#define GAME_WORD(o) (*(s32*)(GAME_BYTES + (o)))

// player fields past what CameraPlayer covers
#define PLAYER         ((CameraPlayer*)GAME_PLAYER)
#define PLAYER_WORD(o) (*(s32*)(GAME_PLAYER + (o)))
#define PLAYER_PART    ((PlayerPart_12DA0*)(GAME_PLAYER + 0x12DA0))

extern "C" {
f32 fn_8013A558(CCameraActionMap* map);
void fn_8001D2D0(CCamera* camera);
void fn_800BADAC(XObj* player);
void fn_800A30EC(XObj* player, s32 on);
s32 fn_800D4D54(void* arg);
void fn_80023438(void* screen, s32 r0, s32 g0, s32 b0, s32 r1, s32 g1, s32 b1, s32 a, f32 x, f32 y, f32 x0, f32 y0, f32 x1, f32 y1, f32 z,
                 s32 a1, s32 flag);
void fn_8001E904(void* sound, s32 id, s32 volume);
}

// something the player keeps at +0x12DA0
class PlayerPart_12DA0 {
public:
	u8 _00[0x50]; // _00
	// _50 vtable

	virtual void vf08();
	virtual void vf0C();
	virtual void vf10();
	virtual void vf14(s32 arg); // _14
};

extern "C" {
void fn_8014872C(CMotionControlCamera* camera);
f32 fn_80148768(CMotionControlCamera* camera);
}

// the last two map cameras, Update copies the map action here every frame it runs
CCameraActionMap lbl_805C47DC[2];

inline CInterfaceCameraAction* CMotionControlCamera::GetAction(s32 type)
{
	return lbl_8025C400._E7DEC[type];
}

// deferred inlining, so these go last to first

// todo: cleanup
/**
 * @note Address: N/A
 * @note Size: ?
 */
inline bool PlayerCanLook()
{
	bool result = false;
	bool busy   = false;
	if (PLAYER->mState == 7 || PLAYER->mState == 8 || PLAYER->mState == 9 || PLAYER->mState == 10 || PLAYER->mState == 11
	    || PLAYER->mState == 12 || PLAYER->mState == 13 || PLAYER->mState == 14 || PLAYER->mState == 15 || PLAYER->mState == 16
	    || PLAYER->mState == 17 || PLAYER->mState == 18) {
		busy = true;
	}
	if (!busy && PLAYER->_AC4 == 0 && PLAYER->_B14 == 0 && PLAYER->_B68 == 0 && PLAYER->_BAC == 0 && PLAYER->_BEC == 0
	    && PLAYER->_C44 == 0) {
		result = true;
	}
	return result;
}

/**
 * @note Address: 0x8014AAC4
 * @note Size: 0x38
 */
// __arraydtor$308, for lbl_805C47DC

/**
 * @note Address: 0x8014AA6C
 * @note Size: 0x58
 */
// __sinit_cmotioncontrolcamera_cpp, builds lbl_805C47DC and registers its dtor

/**
 * @note Address: 0x8014AA4C
 * @note Size: 0x20
 */
void CMotionControlCamera::InitCamera()
{
	fn_8001D2D0(&mCamera);
}

/**
 * @note Address: 0x8014A964
 * @note Size: 0xE8
 */
void CMotionControlCamera::Init()
{
	fn_8001D2D0(&mCamera);
	SetEye(mCamera.mEye);
	SetInterest(mCamera.mInterest);
	SetAngle(mCamera.mAngle);
	mMode           = 0;
	mAction         = NULL;
	mPrevAction     = NULL;
	mPushedAction   = NULL;
	_1E0            = -1;
	_1DC            = -1;
	mFlags.mFlags   = 0;
	mInterestMode   = 0;
	mEyeMode        = 0;
	mFollow.mDivide = 12.0f;
	((CCameraActionScroll2*)GetAction(CAMACTION_SCROLL2))->ClearHideList();
	((CCameraActionScroll2*)GetAction(CAMACTION_SCROLL2))->SaveCamInfo(NULL);
	GAME_FLAGS |= 0x1000000000000ULL;
}

/**
 * @note Address: 0x8014A940
 * @note Size: 0x24
 */
void CMotionControlCamera::RestoreAction()
{
	ChangeAction(mPrevAction);
}

/**
 * @note Address: 0x80149E24
 * @note Size: 0xB1C
 */
void CMotionControlCamera::Update(u32 step)
{
	if (GAME_FLAGS & 0x20) {
		return;
	}

	CPad pad(0);
	if (GAME_DEMO == NULL && (GAME_FLAGS & 0x1000000000000ULL)) {
		CCameraActionScroll2* room = (CCameraActionScroll2*)GetAction(CAMACTION_SCROLL2);
		if (room->GetLimits(GAME_ROOM) != NULL && !(GAME_FLAGS & 0xC00)) {
			if (mAction == GetAction(CAMACTION_AREA) || mAction == GetAction(CAMACTION_SCROLL) || mAction == GetAction(CAMACTION_MAP)) {
				if ((pad.mStatus->_14 & 0x20) && !(pad.mStatus->_04 & 0x200)) {
					PushAction(GetAction(CAMACTION_SCROLL2));
					((CCameraActionScroll2*)GetAction(CAMACTION_SCROLL2))->HideObjs();
					GAME_FLAGS |= 2;
				}
			} else if (mAction == GetAction(CAMACTION_SCROLL2)) {
				if ((pad.mStatus->_14 & 0x20) || (pad.mStatus->_14 & 0x200)) {
					PopAction();
					((CCameraActionScroll2*)GetAction(CAMACTION_SCROLL2))->ShowObjs();
					GAME_FLAGS &= ~2ULL;
					PLAYER_PART->vf14(0);
				}
			}
		}
	}

	if ((GAME_FLAGS & 0x80000000) && (pad.mStatus->_14 & 0x40) && (pad.mStatus->_04 & 0x200)) {
		if (mAction != GetAction(CAMACTION_CHASE)) {
			ChangeAction(CAMACTION_CHASE);
		} else {
			GAME_CAMERA->RestoreAction();
		}
	}

	BOOL look = FALSE;
	if (((pad.mStatus->_14 & 0x800) || (pad.mStatus->_14 & 0x200)) && GAME_DEMO == NULL) {
		bool free = false;
		if ((GAME_FLAGS & 1) && !(GAME_FLAGS & 0x22)) {
			bool blocked = false;
			if ((GAME_WORD(0x331C) != 0 || GAME_WORD(0x3320) != 0) && GAME_WORD(0x3324) == 0) {
				blocked = true;
			}
			if (!blocked) {
				free = true;
			}
		}
		if (free && (PLAYER_WORD(0x980) & 0x20000000) && !(GAME_FLAGS & 0x80)) {
			if (!(GAME_FLAGS & 0xC00) && PLAYER_WORD(0x95D4) < 0 && PLAYER_WORD(0xD7E4) == 0
			    && (PLAYER->mState == 0 || fn_800D4D54(GAME_PLAYER + 0xA44) == 0)) {
				if (pad.mStatus->_14 & 0x200) {
					if (GAME_FLAGS & 0x100) {
						look = TRUE;
					}
				} else {
					look = TRUE;
				}
			} else if (pad.mStatus->_14 & 0x800) {
				fn_8001E904(GAME_SOUND, 0x85, 0xFF);
			}
		}
	}

	if (look) {
		if (!(GAME_FLAGS & 0x100)) {
			if (PlayerCanLook()) {
				StartEye();
			} else {
				fn_8001E904(GAME_SOUND, 0x85, 0xFF);
			}
		} else {
			EndEye();
		}
		if (PLAYER->_AC4 != 5 && PLAYER->_BAC != 5 && PLAYER->_B68 != 5 && PLAYER->_BEC != 2) {
			PLAYER->_9AA8 = PLAYER->_9AAC;
		}
	}

	if (mAction != NULL) {
		mAction->PreviousUpdate();
		mAction->Update(step);
		if (mAction == GetAction(CAMACTION_MAP)) {
			memmove(&lbl_805C47DC[1], &lbl_805C47DC[0], sizeof(CCameraActionMap));
			memmove(&lbl_805C47DC[0], mAction, sizeof(CCameraActionMap));
			if (fpclassify(mAction->mEye.x) == FP_NAN || fpclassify(mAction->mEye.y) == FP_NAN || fpclassify(mAction->mEye.z) == FP_NAN) {
				OSPanic(__FILE__, 249, "%s%s", fn_801911B4("CMotionControlCamera ... \n"), __FUNCTION__);
			}
		}
		ReadAction();
		if (mFlags.mFlags & CAMFLAG_EYE) {
			SetEye(mAction->mEye);
		}
		if (mFlags.mFlags & CAMFLAG_INTEREST) {
			SetInterest(mAction->mInterest);
		}
		if (mFlags.mFlags & CAMFLAG_DISTANCE) {
			mDistance = mAction->mDistance;
		}
		if (mFlags.mFlags & CAMFLAG_FOV) {
			mFov = mAction->mFov;
		}
		if (mFlags.mFlags & CAMFLAG_ANGLE) {
			SetAngle(mAction->mAngle);
		}
	}

	Interpolate(step);

	// work out whatever the action didn't drive from what it did
	u32 flags = mFlags.mFlags;
	if ((flags & CAMFLAG_EYE) && (flags & CAMFLAG_INTEREST)) {
		mCamera.CalcAngle();
		mCamera.CalcDistance();
		mAction->SetAngle(mCamera.mAngle);
		mAction->SetDistance(mCamera.mDistance);
	} else if ((flags & 0x20000) && (flags & CAMFLAG_INTEREST)) {
		mCamera.CalcEye();
		mAction->SetEye(mCamera.mEye);
	} else if ((flags & 0x20000) && (flags & CAMFLAG_EYE)) {
		mCamera.CalcInterest();
	} else if ((flags & 0x10000) && (flags & CAMFLAG_EYE)) {
		mCamera.CalcInterest();
	} else if ((flags & 0x10000) && (flags & CAMFLAG_INTEREST)) {
		mCamera.CalcEye();
	} else if ((flags & 0x80000) && (flags & CAMFLAG_DISTANCE)) {
		mCamera.CalcEye();
		mAction->SetEye(mCamera.mEye);
	} else if ((flags & CAMFLAG_EYE) || (flags & CAMFLAG_INTEREST)) {
		mCamera.CalcAngle();
		mCamera.CalcDistance();
	} else if ((flags & 0x10000) && (flags & CAMFLAG_INTEREST) && (flags & CAMFLAG_DISTANCE)) {
		// can't get here, the interest bit was already ruled out a branch ago
		mCamera.CalcEye();
	}
	if (mFlags.mFlags & 0x40000) {
		mCamera.CalcUp();
	}
}
// fn_80149E24
/*
    stwu     r1, -0x40(r1)
    mflr     r0
    lis      r5, lbl_8025C400@ha
    li       r6, 0x20
    stw      r0, 0x44(r1)
    stmw     r27, 0x2c(r1)
    addi     r31, r5, lbl_8025C400@l
    mr       r27, r3
    li       r5, 0x0
    mr       r28, r4
    lwz      r0, 0x1b10(r31)
    lwz      r3, 0x1b14(r31)
    and      r0, r0, r5
    and      r3, r3, r6
    xor      r3, r3, r5
    xor      r0, r0, r5
    or.      r0, r3, r0
    bne      .L_8014A92C
    addi     r3, r1, 0x14
    li       r4, 0x0
    bl       __ct__4CPadFUc
    lis      r3, lbl_8025C400@ha
    addi     r6, r3, lbl_8025C400@l
    addis    r29, r6, 0x6
    lwz      r0, -0x2f80(r29)
    cmplwi   r0, 0x0
    bne      .L_80149FEC
    lwz      r3, 0x1b10(r31)
    lis      r0, 0x1
    lwz      r5, 0x1b14(r31)
    li       r4, 0x0
    and      r0, r3, r0
    and      r3, r5, r4
    xor      r3, r3, r4
    xor      r0, r0, r4
    or.      r0, r3, r0
    beq      .L_80149FEC
    addis    r30, r6, 0xe
    lwz      r4, 0x3444(r6)
    lwz      r3, 0x7dfc(r30)
    bl       GetLimits__20CCameraActionScroll2Fl
    cmplwi   r3, 0x0
    beq      .L_80149FEC
    lwz      r0, 0x1b10(r31)
    li       r4, 0x0
    lwz      r5, 0x1b14(r31)
    li       r3, 0xc00
    and      r0, r0, r4
    and      r3, r5, r3
    xor      r3, r3, r4
    xor      r0, r0, r4
    or.      r0, r3, r0
    bne      .L_80149FEC
    lis      r3, lbl_8025C400@ha
    lwz      r4, 0x1d0(r27)
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    lwz      r0, 0x7df0(r3)
    cmplw    r4, r0
    beq      .L_80149F2C
    lwz      r0, 0x7e18(r3)
    cmplw    r4, r0
    beq      .L_80149F2C
    lwz      r0, 0x7e08(r3)
    cmplw    r4, r0
    bne      .L_80149F6C
.L_80149F2C:
    lwz      r3, 0x14(r1)
    lwz      r0, 0x14(r3)
    rlwinm.  r0, r0, 0, 26, 26
    beq      .L_80149FEC
    lwz      r0, 0x4(r3)
    rlwinm.  r0, r0, 0, 22, 22
    bne      .L_80149FEC
    lwz      r4, 0x7dfc(r30)
    mr       r3, r27
    bl       fn_801491D0
    lwz      r3, 0x7dfc(r30)
    bl       HideObjs__20CCameraActionScroll2Fv
    lwz      r0, 0x1b14(r31)
    ori      r0, r0, 0x2
    stw      r0, 0x1b14(r31)
    b        .L_80149FEC
.L_80149F6C:
    lwz      r0, 0x7dfc(r30)
    cmplw    r4, r0
    bne      .L_80149FEC
    lwz      r3, 0x14(r1)
    lwz      r3, 0x14(r3)
    rlwinm.  r0, r3, 0, 26, 26
    bne      .L_80149F90
    rlwinm.  r0, r3, 0, 22, 22
    beq      .L_80149FEC
.L_80149F90:
    mr       r3, r27
    bl       fn_80148F20
    lwz      r3, 0x7dfc(r30)
    bl       ShowObjs__20CCameraActionScroll2Fv
    lis      r3, lbl_8025C400@ha
    lwz      r6, 0x1b10(r31)
    addi     r3, r3, lbl_8025C400@l
    lwz      r5, 0x1b14(r31)
    addis    r3, r3, 0x1
    li       r4, -0x3
    li       r0, -0x1
    lwz      r3, -0x6944(r3)
    and      r5, r5, r4
    li       r4, 0x0
    and      r0, r6, r0
    stw      r5, 0x1b14(r31)
    addis    r3, r3, 0x1
    stw      r0, 0x1b10(r31)
    lwz      r12, 0x2df0(r3)
    lwz      r12, 0x14(r12)
    mtctr    r12
    addi     r3, r3, 0x2da0
    bctrl
.L_80149FEC:
    lwz      r0, 0x1b10(r31)
    li       r4, 0x0
    lwz      r5, 0x1b14(r31)
    lis      r3, 0x8000
    and      r0, r0, r4
    and      r3, r5, r3
    xor      r3, r3, r4
    xor      r0, r0, r4
    or.      r0, r3, r0
    beq      .L_8014A064
    lwz      r3, 0x14(r1)
    lwz      r0, 0x14(r3)
    rlwinm.  r0, r0, 0, 25, 25
    beq      .L_8014A064
    lwz      r0, 0x4(r3)
    rlwinm.  r0, r0, 0, 22, 22
    beq      .L_8014A064
    lis      r3, lbl_8025C400@ha
    lwz      r4, 0x1d0(r27)
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    lwz      r0, 0x7df4(r3)
    cmplw    r4, r0
    beq      .L_8014A05C
    mr       r3, r27
    li       r4, 0x2
    bl       fn_8014946C
    b        .L_8014A064
.L_8014A05C:
    addi     r3, r3, 0x5d38
    bl       fn_8014A940
.L_8014A064:
    lwz      r3, 0x14(r1)
    li       r30, 0x0
    lwz      r3, 0x14(r3)
    rlwinm.  r0, r3, 0, 20, 20
    bne      .L_8014A080
    rlwinm.  r0, r3, 0, 22, 22
    beq      .L_8014A21C
.L_8014A080:
    lwz      r0, -0x2f80(r29)
    cmplwi   r0, 0x0
    bne      .L_8014A21C
    lwz      r0, 0x1b10(r31)
    li       r7, 0x0
    lwz      r3, 0x1b14(r31)
    li       r4, 0x1
    and      r6, r0, r7
    and      r4, r3, r4
    xor      r5, r4, r7
    xor      r4, r6, r7
    or.      r4, r5, r4
    beq      .L_8014A114
    li       r5, 0x22
    xor      r4, r6, r7
    and      r5, r3, r5
    xor      r5, r5, r7
    or.      r4, r5, r4
    bne      .L_8014A114
    lis      r4, lbl_8025C400@ha
    mr       r6, r7
    addi     r5, r4, lbl_8025C400@l
    lwz      r4, 0x331c(r5)
    cmpwi    r4, 0x0
    bne      .L_8014A0F0
    lwz      r4, 0x3320(r5)
    cmpwi    r4, 0x0
    beq      .L_8014A108
.L_8014A0F0:
    lis      r4, lbl_8025C400@ha
    addi     r4, r4, lbl_8025C400@l
    lwz      r4, 0x3324(r4)
    cmpwi    r4, 0x0
    bne      .L_8014A108
    li       r6, 0x1
.L_8014A108:
    clrlwi.  r4, r6, 24
    bne      .L_8014A114
    li       r7, 0x1
.L_8014A114:
    clrlwi.  r4, r7, 24
    beq      .L_8014A21C
    lis      r4, lbl_8025C400@ha
    addi     r4, r4, lbl_8025C400@l
    addis    r4, r4, 0x1
    lwz      r7, -0x6944(r4)
    lwz      r4, 0x980(r7)
    rlwinm.  r4, r4, 0, 2, 2
    beq      .L_8014A21C
    li       r4, 0x80
    li       r6, 0x0
    and      r4, r3, r4
    and      r5, r0, r6
    xor      r4, r4, r6
    xor      r0, r5, r6
    or.      r0, r4, r0
    bne      .L_8014A21C
    li       r4, 0xc00
    xor      r0, r5, r6
    and      r3, r3, r4
    xor      r3, r3, r6
    or.      r0, r3, r0
    bne      .L_8014A1F0
    addis    r3, r7, 0x1
    lwz      r0, -0x6a2c(r3)
    cmpwi    r0, 0x0
    bge      .L_8014A1F0
    lwz      r0, -0x281c(r3)
    cmpwi    r0, 0x0
    bne      .L_8014A1F0
    addi     r3, r7, 0xa44
    lwz      r0, 0xa48(r7)
    cmpwi    r0, 0x0
    beq      .L_8014A1A8
    bl       fn_800D4D54
    cmpwi    r3, 0x0
    bne      .L_8014A1F0
.L_8014A1A8:
    lwz      r3, 0x14(r1)
    lwz      r0, 0x14(r3)
    rlwinm.  r0, r0, 0, 22, 22
    beq      .L_8014A1E8
    lwz      r0, 0x1b10(r31)
    li       r4, 0x0
    lwz      r5, 0x1b14(r31)
    li       r3, 0x100
    and      r0, r0, r4
    and      r3, r5, r3
    xor      r3, r3, r4
    xor      r0, r0, r4
    or.      r0, r3, r0
    beq      .L_8014A21C
    li       r30, 0x1
    b        .L_8014A21C
.L_8014A1E8:
    li       r30, 0x1
    b        .L_8014A21C
.L_8014A1F0:
    lwz      r3, 0x14(r1)
    lwz      r0, 0x14(r3)
    rlwinm.  r0, r0, 0, 20, 20
    beq      .L_8014A21C
    lis      r3, lbl_8025C400@ha
    li       r4, 0x85
    addi     r3, r3, lbl_8025C400@l
    li       r5, 0xff
    addis    r3, r3, 0xe
    addi     r3, r3, 0x7e24
    bl       fn_8001E904
.L_8014A21C:
    cmpwi    r30, 0x0
    beq      .L_8014A3A4
    lwz      r0, 0x1b10(r31)
    li       r4, 0x0
    lwz      r5, 0x1b14(r31)
    li       r3, 0x100
    and      r0, r0, r4
    and      r3, r5, r3
    xor      r3, r3, r4
    xor      r0, r0, r4
    or.      r0, r3, r0
    bne      .L_8014A350
    lis      r3, lbl_8025C400@ha
    mr       r0, r4
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x1
    lwz      r3, -0x6944(r3)
    lwz      r5, 0xa48(r3)
    cmpwi    r5, 0x7
    beq      .L_8014A2C4
    cmpwi    r5, 0x8
    beq      .L_8014A2C4
    cmpwi    r5, 0x9
    beq      .L_8014A2C4
    cmpwi    r5, 0xa
    beq      .L_8014A2C4
    cmpwi    r5, 0xb
    beq      .L_8014A2C4
    cmpwi    r5, 0xc
    beq      .L_8014A2C4
    cmpwi    r5, 0xd
    beq      .L_8014A2C4
    cmpwi    r5, 0xe
    beq      .L_8014A2C4
    cmpwi    r5, 0xf
    beq      .L_8014A2C4
    cmpwi    r5, 0x10
    beq      .L_8014A2C4
    cmpwi    r5, 0x11
    beq      .L_8014A2C4
    cmpwi    r5, 0x12
    bne      .L_8014A2C8
.L_8014A2C4:
    li       r0, 0x1
.L_8014A2C8:
    clrlwi.  r0, r0, 24
    bne      .L_8014A31C
    lwz      r0, 0xac4(r3)
    cmpwi    r0, 0x0
    bne      .L_8014A31C
    lwz      r0, 0xb14(r3)
    cmpwi    r0, 0x0
    bne      .L_8014A31C
    lwz      r0, 0xb68(r3)
    cmpwi    r0, 0x0
    bne      .L_8014A31C
    lwz      r0, 0xbac(r3)
    cmpwi    r0, 0x0
    bne      .L_8014A31C
    lwz      r0, 0xbec(r3)
    cmpwi    r0, 0x0
    bne      .L_8014A31C
    lwz      r0, 0xc44(r3)
    cmpwi    r0, 0x0
    bne      .L_8014A31C
    li       r4, 0x1
.L_8014A31C:
    clrlwi.  r0, r4, 24
    beq      .L_8014A330
    mr       r3, r27
    bl       fn_80148C6C
    b        .L_8014A358
.L_8014A330:
    lis      r3, lbl_8025C400@ha
    li       r4, 0x85
    addi     r3, r3, lbl_8025C400@l
    li       r5, 0xff
    addis    r3, r3, 0xe
    addi     r3, r3, 0x7e24
    bl       fn_8001E904
    b        .L_8014A358
.L_8014A350:
    mr       r3, r27
    bl       fn_80148B78
.L_8014A358:
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x1
    lwz      r3, -0x6944(r3)
    lwz      r0, 0xac4(r3)
    cmpwi    r0, 0x5
    beq      .L_8014A3A4
    lwz      r0, 0xbac(r3)
    cmpwi    r0, 0x5
    beq      .L_8014A3A4
    lwz      r0, 0xb68(r3)
    cmpwi    r0, 0x5
    beq      .L_8014A3A4
    lwz      r0, 0xbec(r3)
    cmpwi    r0, 0x2
    beq      .L_8014A3A4
    addis    r3, r3, 0x1
    lfs      f0, -0x6554(r3)
    stfs     f0, -0x6558(r3)
.L_8014A3A4:
    lwz      r3, 0x1d0(r27)
    cmplwi   r3, 0x0
    beq      .L_8014A724
    lwz      r12, 0x3c(r3)
    lwz      r12, 0xc(r12)
    mtctr    r12
    bctrl
    lwz      r3, 0x1d0(r27)
    mr       r4, r28
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x10(r12)
    mtctr    r12
    bctrl
    lis      r3, lbl_8025C400@ha
    lwz      r4, 0x1d0(r27)
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    lwz      r0, 0x7e08(r3)
    cmplw    r4, r0
    bne      .L_8014A590
    lis      r3, lbl_805C47DC@ha
    li       r5, 0x11ec
    addi     r4, r3, lbl_805C47DC@l
    addi     r3, r4, 0x11ec
    bl       memmove
    lis      r3, lbl_805C47DC@ha
    lwz      r4, 0x1d0(r27)
    addi     r3, r3, lbl_805C47DC@l
    li       r5, 0x11ec
    bl       memmove
    lwz      r3, 0x1d0(r27)
    lis      r0, 0x7f80
    lfs      f0, 0x0(r3)
    stfs     f0, 0x10(r1)
    lwz      r4, 0x10(r1)
    rlwinm   r3, r4, 0, 1, 8
    cmpw     r3, r0
    beq      .L_8014A44C
    bge      .L_8014A47C
    cmpwi    r3, 0x0
    beq      .L_8014A464
    b        .L_8014A47C
.L_8014A44C:
    clrlwi.  r0, r4, 9
    beq      .L_8014A45C
    li       r0, 0x1
    b        .L_8014A480
.L_8014A45C:
    li       r0, 0x2
    b        .L_8014A480
.L_8014A464:
    clrlwi.  r0, r4, 9
    beq      .L_8014A474
    li       r0, 0x5
    b        .L_8014A480
.L_8014A474:
    li       r0, 0x3
    b        .L_8014A480
.L_8014A47C:
    li       r0, 0x4
.L_8014A480:
    cmpwi    r0, 0x1
    beq      .L_8014A560
    lwz      r3, 0x1d0(r27)
    lis      r0, 0x7f80
    lfs      f0, 0x4(r3)
    stfs     f0, 0xc(r1)
    lwz      r4, 0xc(r1)
    rlwinm   r3, r4, 0, 1, 8
    cmpw     r3, r0
    beq      .L_8014A4B8
    bge      .L_8014A4E8
    cmpwi    r3, 0x0
    beq      .L_8014A4D0
    b        .L_8014A4E8
.L_8014A4B8:
    clrlwi.  r0, r4, 9
    beq      .L_8014A4C8
    li       r0, 0x1
    b        .L_8014A4EC
.L_8014A4C8:
    li       r0, 0x2
    b        .L_8014A4EC
.L_8014A4D0:
    clrlwi.  r0, r4, 9
    beq      .L_8014A4E0
    li       r0, 0x5
    b        .L_8014A4EC
.L_8014A4E0:
    li       r0, 0x3
    b        .L_8014A4EC
.L_8014A4E8:
    li       r0, 0x4
.L_8014A4EC:
    cmpwi    r0, 0x1
    beq      .L_8014A560
    lwz      r3, 0x1d0(r27)
    lis      r0, 0x7f80
    lfs      f0, 0x8(r3)
    stfs     f0, 0x8(r1)
    lwz      r4, 0x8(r1)
    rlwinm   r3, r4, 0, 1, 8
    cmpw     r3, r0
    beq      .L_8014A524
    bge      .L_8014A554
    cmpwi    r3, 0x0
    beq      .L_8014A53C
    b        .L_8014A554
.L_8014A524:
    clrlwi.  r0, r4, 9
    beq      .L_8014A534
    li       r0, 0x1
    b        .L_8014A558
.L_8014A534:
    li       r0, 0x2
    b        .L_8014A558
.L_8014A53C:
    clrlwi.  r0, r4, 9
    beq      .L_8014A54C
    li       r0, 0x5
    b        .L_8014A558
.L_8014A54C:
    li       r0, 0x3
    b        .L_8014A558
.L_8014A554:
    li       r0, 0x4
.L_8014A558:
    cmpwi    r0, 0x1
    bne      .L_8014A590
.L_8014A560:
    lis      r3, lbl_8020D124@ha
    addi     r3, r3, lbl_8020D124@l
    crclr    cr1eq
    bl       fn_801911B4
    lis      r4, lbl_8020D108@ha
    mr       r6, r3
    addi     r3, r4, lbl_8020D108@l
    li       r5, lbl_8065ECA4@sda21
    li       r4, 0xf9
    li       r7, lbl_8065AB38@sda21
    crclr    cr1eq
    bl       OSPanic
.L_8014A590:
    li       r4, 0x0
    stw      r4, 0x1f0(r27)
    lwz      r5, 0x1d0(r27)
    cmplwi   r5, 0x0
    beq      .L_8014A67C
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    lwz      r0, 0x7e04(r3)
    cmplw    r5, r0
    bne      .L_8014A610
    stw      r4, 0x1cc(r27)
    stw      r4, 0x1e4(r27)
    lwz      r0, 0x1e4(r27)
    ori      r0, r0, 0x1
    stw      r0, 0x1e4(r27)
    lwz      r0, 0x1e4(r27)
    ori      r0, r0, 0x2
    stw      r0, 0x1e4(r27)
    lwz      r0, 0x1e4(r27)
    ori      r0, r0, 0x8
    stw      r0, 0x1e4(r27)
    lwz      r0, 0x1e4(r27)
    ori      r0, r0, 0x10
    stw      r0, 0x1e4(r27)
    lwz      r0, 0x1e4(r27)
    ori      r0, r0, 0x4
    stw      r0, 0x1e4(r27)
    lwz      r0, 0x1e4(r27)
    oris     r0, r0, 0x4
    stw      r0, 0x1e4(r27)
    b        .L_8014A67C
.L_8014A610:
    stw      r4, 0x1e4(r27)
    lwz      r3, 0x1d0(r27)
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x50(r12)
    mtctr    r12
    bctrl
    stw      r3, 0x1cc(r27)
    lwz      r3, 0x1d0(r27)
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x54(r12)
    mtctr    r12
    bctrl
    lwz      r0, 0x1e4(r27)
    or       r0, r0, r3
    stw      r0, 0x1e4(r27)
    lwz      r3, 0x1d0(r27)
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x58(r12)
    mtctr    r12
    bctrl
    stfs     f1, 0x22c(r27)
    lwz      r3, 0x1d0(r27)
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x5c(r12)
    mtctr    r12
    bctrl
    stw      r3, 0x1ec(r27)
.L_8014A67C:
    lwz      r0, 0x1e4(r27)
    clrlwi.  r0, r0, 31
    beq      .L_8014A6A4
    lwz      r3, 0x1d0(r27)
    lfs      f2, 0x8(r3)
    lfs      f1, 0x4(r3)
    lfs      f0, 0x0(r3)
    stfs     f0, 0x194(r27)
    stfs     f1, 0x198(r27)
    stfs     f2, 0x19c(r27)
.L_8014A6A4:
    lwz      r0, 0x1e4(r27)
    rlwinm.  r0, r0, 0, 30, 30
    beq      .L_8014A6CC
    lwz      r3, 0x1d0(r27)
    lfs      f2, 0x18(r3)
    lfs      f1, 0x14(r3)
    lfs      f0, 0x10(r3)
    stfs     f0, 0x1a4(r27)
    stfs     f1, 0x1a8(r27)
    stfs     f2, 0x1ac(r27)
.L_8014A6CC:
    lwz      r0, 0x1e4(r27)
    rlwinm.  r0, r0, 0, 29, 29
    beq      .L_8014A6E4
    lwz      r3, 0x1d0(r27)
    lfs      f0, 0x30(r3)
    stfs     f0, 0x1c4(r27)
.L_8014A6E4:
    lwz      r0, 0x1e4(r27)
    rlwinm.  r0, r0, 0, 28, 28
    beq      .L_8014A6FC
    lwz      r3, 0x1d0(r27)
    lfs      f0, 0x34(r3)
    stfs     f0, 0x1c8(r27)
.L_8014A6FC:
    lwz      r0, 0x1e4(r27)
    rlwinm.  r0, r0, 0, 27, 27
    beq      .L_8014A724
    lwz      r3, 0x1d0(r27)
    lfs      f2, 0x28(r3)
    lfs      f1, 0x24(r3)
    lfs      f0, 0x20(r3)
    stfs     f0, 0x1b4(r27)
    stfs     f1, 0x1b8(r27)
    stfs     f2, 0x1bc(r27)
.L_8014A724:
    mr       r3, r27
    mr       r4, r28
    bl       fn_8014978C
    lwz      r3, 0x1e4(r27)
    clrlwi.  r5, r3, 31
    beq      .L_8014A7A0
    rlwinm.  r0, r3, 0, 30, 30
    beq      .L_8014A7A0
    mr       r3, r27
    lwz      r12, 0x190(r27)
    lwz      r12, 0x9c(r12)
    mtctr    r12
    bctrl
    mr       r3, r27
    lwz      r12, 0x190(r27)
    lwz      r12, 0xa4(r12)
    mtctr    r12
    bctrl
    lwz      r3, 0x1d0(r27)
    addi     r4, r27, 0xb4
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x40(r12)
    mtctr    r12
    bctrl
    lwz      r3, 0x1d0(r27)
    lfs      f1, 0xe4(r27)
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x44(r12)
    mtctr    r12
    bctrl
    b        .L_8014A900
.L_8014A7A0:
    rlwinm.  r4, r3, 0, 14, 14
    beq      .L_8014A7E0
    rlwinm.  r0, r3, 0, 30, 30
    beq      .L_8014A7E0
    mr       r3, r27
    lwz      r12, 0x190(r27)
    lwz      r12, 0x94(r12)
    mtctr    r12
    bctrl
    lwz      r3, 0x1d0(r27)
    addi     r4, r27, 0x94
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x38(r12)
    mtctr    r12
    bctrl
    b        .L_8014A900
.L_8014A7E0:
    cmplwi   r4, 0x0
    beq      .L_8014A808
    cmplwi   r5, 0x0
    beq      .L_8014A808
    mr       r3, r27
    lwz      r12, 0x190(r27)
    lwz      r12, 0x98(r12)
    mtctr    r12
    bctrl
    b        .L_8014A900
.L_8014A808:
    rlwinm.  r4, r3, 0, 15, 15
    beq      .L_8014A830
    cmplwi   r5, 0x0
    beq      .L_8014A830
    mr       r3, r27
    lwz      r12, 0x190(r27)
    lwz      r12, 0x98(r12)
    mtctr    r12
    bctrl
    b        .L_8014A900
.L_8014A830:
    cmplwi   r4, 0x0
    beq      .L_8014A858
    rlwinm.  r0, r3, 0, 30, 30
    beq      .L_8014A858
    mr       r3, r27
    lwz      r12, 0x190(r27)
    lwz      r12, 0x94(r12)
    mtctr    r12
    bctrl
    b        .L_8014A900
.L_8014A858:
    rlwinm.  r0, r3, 0, 12, 12
    beq      .L_8014A898
    rlwinm.  r0, r3, 0, 29, 29
    beq      .L_8014A898
    mr       r3, r27
    lwz      r12, 0x190(r27)
    lwz      r12, 0x94(r12)
    mtctr    r12
    bctrl
    lwz      r3, 0x1d0(r27)
    addi     r4, r27, 0x94
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x38(r12)
    mtctr    r12
    bctrl
    b        .L_8014A900
.L_8014A898:
    cmplwi   r5, 0x0
    bne      .L_8014A8A8
    rlwinm.  r0, r3, 0, 30, 30
    beq      .L_8014A8D4
.L_8014A8A8:
    mr       r3, r27
    lwz      r12, 0x190(r27)
    lwz      r12, 0x9c(r12)
    mtctr    r12
    bctrl
    mr       r3, r27
    lwz      r12, 0x190(r27)
    lwz      r12, 0xa4(r12)
    mtctr    r12
    bctrl
    b        .L_8014A900
.L_8014A8D4:
    cmplwi   r4, 0x0
    beq      .L_8014A900
    cmplwi   r0, 0x0
    beq      .L_8014A900
    rlwinm.  r0, r3, 0, 29, 29
    beq      .L_8014A900
    mr       r3, r27
    lwz      r12, 0x190(r27)
    lwz      r12, 0x94(r12)
    mtctr    r12
    bctrl
.L_8014A900:
    lwz      r0, 0x1e4(r27)
    rlwinm.  r0, r0, 0, 13, 13
    beq      .L_8014A920
    mr       r3, r27
    lwz      r12, 0x190(r27)
    lwz      r12, 0xa0(r12)
    mtctr    r12
    bctrl
.L_8014A920:
    lis      r3, __vt__4CPad@ha
    addi     r0, r3, __vt__4CPad@l
    stw      r0, 0x18(r1)
.L_8014A92C:
    lmw      r27, 0x2c(r1)
    lwz      r0, 0x44(r1)
    mtlr     r0
    addi     r1, r1, 0x40
    blr
*/

/**
 * @note Address: 0x8014978C
 * @note Size: 0x698
 */
void CMotionControlCamera::Interpolate(u32 step)
{
	switch (mMode) {
	case 0:
		if (mFlags.mFlags & CAMFLAG_DISTANCE) {
			mCamera.SetDistance(mDistance);
		}
		if (mFlags.mFlags & CAMFLAG_FOV) {
			mCamera.SetFov(mFov);
		}
		if (mFlags.mFlags & CAMFLAG_ANGLE) {
			mCamera.SetAngle(mAngle);
		}
		break;
	case 1: {
		CObjVec vec;
		if (mFlags.mFlags & CAMFLAG_EYE) {
			vec.Reset(mCamera.mEye);
			vec.SetTarget(mEye);
			vec.mSpeed = CVec(1.0f, 1.0f, 1.0f);
			vec.Step(step);
			mCamera.SetEye(vec);
		}
		if (mFlags.mFlags & CAMFLAG_INTEREST) {
			vec.Reset(mCamera.mInterest);
			vec.SetTarget(mInterest);
			vec.mSpeed = CVec(1.0f, 1.0f, 1.0f);
			vec.Step(step);
			mCamera.SetInterest(vec);
		}
		break;
	}
	}

	if (mFlags.mFlags & CAMFLAG_INTEREST) {
		switch (mInterestMode) {
		case 0:
			mCamera.SetInterest(mInterest);
			break;
		case 2:
			mFollow.Follow(mInterest, mCamera.mInterest);
			mCamera.SetInterest(mFollow.mValue);
			break;
		case 3: {
			// keep the height, only follow it round
			Vec interest = mCamera.mInterest;
			interest.y   = mInterest.y;
			mCamera.SetInterest(interest);
			mFollow.Follow(mInterest, mCamera.mInterest);
			mCamera.SetInterest(mFollow.mValue);
			break;
		}
		}
	}

	if (mFlags.mFlags & CAMFLAG_EYE) {
		switch (mEyeMode) {
		case 0:
			mCamera.SetEye(mEye);
			break;
		case 1:
			break;
		case 2:
		case 3:
			mFollow.Follow(mEye, mCamera.mEye);
			mCamera.SetEye(mFollow.mValue);
			break;
		}
	}
}
// fn_8014978C
/*
    stwu     r1, -0x130(r1)
    mflr     r0
    stw      r0, 0x134(r1)
    stfd     f31, 0x120(r1)
    psq_st   f31, 0x128(r1), 0, qr0
    stw      r31, 0x11c(r1)
    stw      r30, 0x118(r1)
    mr       r31, r3
    mr       r30, r4
    lwz      r0, 0x1cc(r3)
    cmpwi    r0, 0x1
    beq      .L_80149838
    bge      .L_801499BC
    cmpwi    r0, 0x0
    bge      .L_801497CC
    b        .L_801499BC
.L_801497CC:
    lwz      r0, 0x1e4(r31)
    rlwinm.  r0, r0, 0, 29, 29
    beq      .L_801497EC
    lwz      r12, 0x190(r3)
    lfs      f1, 0x1c4(r31)
    lwz      r12, 0x68(r12)
    mtctr    r12
    bctrl
.L_801497EC:
    lwz      r0, 0x1e4(r31)
    rlwinm.  r0, r0, 0, 28, 28
    beq      .L_80149810
    mr       r3, r31
    lfs      f1, 0x1c8(r31)
    lwz      r12, 0x190(r31)
    lwz      r12, 0x48(r12)
    mtctr    r12
    bctrl
.L_80149810:
    lwz      r0, 0x1e4(r31)
    rlwinm.  r0, r0, 0, 27, 27
    beq      .L_801499BC
    mr       r3, r31
    addi     r4, r31, 0x1b4
    lwz      r12, 0x190(r31)
    lwz      r12, 0x74(r12)
    mtctr    r12
    bctrl
    b        .L_801499BC
.L_80149838:
    lis      r5, __vt__4CVec@ha
    lis      r3, __vt__7CObjVec@ha
    addi     r5, r5, __vt__4CVec@l
    stw      r5, 0xe8(r1)
    addi     r0, r3, __vt__7CObjVec@l
    stw      r0, 0xe8(r1)
    stw      r5, 0xf8(r1)
    stw      r5, 0x108(r1)
    lwz      r0, 0x1e4(r31)
    clrlwi.  r0, r0, 31
    beq      .L_801498F4
    lfs      f4, 0x9c(r31)
    addi     r3, r1, 0xdc
    lfs      f3, 0x98(r31)
    lfs      f2, 0x94(r31)
    lfs      f1, lbl_8065EC78@sda21(r0)
    lfs      f0, lbl_8065EC74@sda21(r0)
    stw      r5, 0xcc(r1)
    stfs     f2, 0xdc(r1)
    stfs     f3, 0xe0(r1)
    stfs     f4, 0xe4(r1)
    stfs     f2, 0xec(r1)
    stfs     f3, 0xf0(r1)
    stfs     f4, 0xf4(r1)
    stfs     f1, 0xfc(r1)
    stfs     f1, 0x100(r1)
    stfs     f1, 0x104(r1)
    lfs      f3, 0x19c(r31)
    lfs      f2, 0x198(r31)
    lfs      f1, 0x194(r31)
    stfs     f0, 0xc0(r1)
    stfs     f1, 0xec(r1)
    stfs     f2, 0xf0(r1)
    stfs     f3, 0xf4(r1)
    stfs     f0, 0xc4(r1)
    stfs     f0, 0xc8(r1)
    stfs     f0, 0xfc(r1)
    stfs     f0, 0x100(r1)
    stfs     f0, 0x104(r1)
    stw      r5, 0xcc(r1)
    bl       Step__7CObjVecFUl
    mr       r3, r31
    addi     r4, r1, 0xdc
    lwz      r12, 0x190(r31)
    lwz      r12, 0x6c(r12)
    mtctr    r12
    bctrl
.L_801498F4:
    lwz      r0, 0x1e4(r31)
    rlwinm.  r0, r0, 0, 30, 30
    beq      .L_8014999C
    lfs      f4, 0xac(r31)
    lis      r3, __vt__4CVec@ha
    lfs      f3, 0xa8(r31)
    addi     r0, r3, __vt__4CVec@l
    lfs      f2, 0xa4(r31)
    mr       r4, r30
    lfs      f1, lbl_8065EC78@sda21(r0)
    addi     r3, r1, 0xdc
    lfs      f0, lbl_8065EC74@sda21(r0)
    stw      r0, 0xbc(r1)
    stfs     f2, 0xdc(r1)
    stfs     f3, 0xe0(r1)
    stfs     f4, 0xe4(r1)
    stfs     f2, 0xec(r1)
    stfs     f3, 0xf0(r1)
    stfs     f4, 0xf4(r1)
    stfs     f1, 0xfc(r1)
    stfs     f1, 0x100(r1)
    stfs     f1, 0x104(r1)
    lfs      f3, 0x1ac(r31)
    lfs      f2, 0x1a8(r31)
    lfs      f1, 0x1a4(r31)
    stfs     f0, 0xb0(r1)
    stfs     f1, 0xec(r1)
    stfs     f2, 0xf0(r1)
    stfs     f3, 0xf4(r1)
    stfs     f0, 0xb4(r1)
    stfs     f0, 0xb8(r1)
    stfs     f0, 0xfc(r1)
    stfs     f0, 0x100(r1)
    stfs     f0, 0x104(r1)
    stw      r0, 0xbc(r1)
    bl       Step__7CObjVecFUl
    mr       r3, r31
    addi     r4, r1, 0xdc
    lwz      r12, 0x190(r31)
    lwz      r12, 0x70(r12)
    mtctr    r12
    bctrl
.L_8014999C:
    lis      r4, __vt__7CObjVec@ha
    lis      r3, __vt__4CVec@ha
    addi     r0, r4, __vt__7CObjVec@l
    stw      r0, 0xe8(r1)
    addi     r0, r3, __vt__4CVec@l
    stw      r0, 0x108(r1)
    stw      r0, 0xf8(r1)
    stw      r0, 0xe8(r1)
.L_801499BC:
    lwz      r0, 0x1e4(r31)
    rlwinm.  r0, r0, 0, 30, 30
    beq      .L_80149C90
    lwz      r0, 0x1ec(r31)
    cmpwi    r0, 0x2
    beq      .L_80149A0C
    bge      .L_801499E4
    cmpwi    r0, 0x0
    beq      .L_801499F0
    b        .L_80149C90
.L_801499E4:
    cmpwi    r0, 0x4
    bge      .L_80149C90
    b        .L_80149B34
.L_801499F0:
    mr       r3, r31
    addi     r4, r31, 0x1a4
    lwz      r12, 0x190(r31)
    lwz      r12, 0x70(r12)
    mtctr    r12
    bctrl
    b        .L_80149C90
.L_80149A0C:
    lfs      f0, 0x1a4(r31)
    addi     r3, r1, 0x94
    addi     r4, r31, 0x1f8
    addi     r5, r1, 0xa4
    stfs     f0, 0x1f8(r31)
    lfs      f0, 0x1a8(r31)
    stfs     f0, 0x1fc(r31)
    lfs      f0, 0x1ac(r31)
    stfs     f0, 0x200(r31)
    lfs      f0, 0xa4(r31)
    stfs     f0, 0x208(r31)
    lfs      f0, 0xa8(r31)
    stfs     f0, 0x20c(r31)
    lfs      f0, 0xac(r31)
    stfs     f0, 0x210(r31)
    lwz      r6, 0x208(r31)
    lwz      r0, 0x20c(r31)
    stw      r6, 0xa4(r1)
    stw      r0, 0xa8(r1)
    lwz      r0, 0x210(r31)
    stw      r0, 0xac(r1)
    bl       __mi__4CVecCF3Vec
    lfs      f1, 0x22c(r31)
    addi     r3, r1, 0x84
    addi     r4, r1, 0x94
    bl       fn_8002442C
    lfs      f0, 0x84(r1)
    lis      r3, __vt__4CVec@ha
    addi     r0, r3, __vt__4CVec@l
    addi     r3, r31, 0x218
    stfs     f0, 0x218(r31)
    lfs      f0, 0x88(r1)
    stfs     f0, 0x21c(r31)
    lfs      f0, 0x8c(r1)
    stfs     f0, 0x220(r31)
    stw      r0, 0x90(r1)
    stw      r0, 0xa0(r1)
    bl       PSVECMag
    fmr      f31, f1
    addi     r3, r31, 0x1f8
    addi     r4, r31, 0x208
    bl       PSVECDistance
    fcmpo    cr0, f1, f31
    cror     eq, lt, eq
    beq      .L_80149ACC
    lfs      f0, lbl_8065ECA0@sda21(r0)
    fcmpo    cr0, f1, f0
    bge      .L_80149AF0
.L_80149ACC:
    lfs      f0, 0x1f8(r31)
    li       r0, 0x1
    stfs     f0, 0x208(r31)
    lfs      f0, 0x1fc(r31)
    stfs     f0, 0x20c(r31)
    lfs      f0, 0x200(r31)
    stfs     f0, 0x210(r31)
    stw      r0, 0x228(r31)
    b        .L_80149B18
.L_80149AF0:
    lwz      r6, 0x218(r31)
    addi     r3, r31, 0x208
    lwz      r0, 0x21c(r31)
    mr       r5, r3
    addi     r4, r1, 0x78
    stw      r6, 0x78(r1)
    stw      r0, 0x7c(r1)
    lwz      r0, 0x220(r31)
    stw      r0, 0x80(r1)
    bl       PSVECAdd
.L_80149B18:
    mr       r3, r31
    addi     r4, r31, 0x208
    lwz      r12, 0x190(r31)
    lwz      r12, 0x70(r12)
    mtctr    r12
    bctrl
    b        .L_80149C90
.L_80149B34:
    lwz      r5, 0xa4(r31)
    mr       r3, r31
    lwz      r0, 0xa8(r31)
    addi     r4, r1, 0xd0
    stw      r5, 0xd0(r1)
    stw      r0, 0xd4(r1)
    lwz      r0, 0xac(r31)
    stw      r0, 0xd8(r1)
    lfs      f0, 0x1a8(r31)
    stfs     f0, 0xd4(r1)
    lwz      r12, 0x190(r31)
    lwz      r12, 0x70(r12)
    mtctr    r12
    bctrl
    lfs      f0, 0x1a4(r31)
    addi     r3, r1, 0x5c
    addi     r4, r31, 0x1f8
    addi     r5, r1, 0x6c
    stfs     f0, 0x1f8(r31)
    lfs      f0, 0x1a8(r31)
    stfs     f0, 0x1fc(r31)
    lfs      f0, 0x1ac(r31)
    stfs     f0, 0x200(r31)
    lfs      f0, 0xa4(r31)
    stfs     f0, 0x208(r31)
    lfs      f0, 0xa8(r31)
    stfs     f0, 0x20c(r31)
    lfs      f0, 0xac(r31)
    stfs     f0, 0x210(r31)
    lwz      r6, 0x208(r31)
    lwz      r0, 0x20c(r31)
    stw      r6, 0x6c(r1)
    stw      r0, 0x70(r1)
    lwz      r0, 0x210(r31)
    stw      r0, 0x74(r1)
    bl       __mi__4CVecCF3Vec
    lfs      f1, 0x22c(r31)
    addi     r3, r1, 0x4c
    addi     r4, r1, 0x5c
    bl       fn_8002442C
    lfs      f0, 0x4c(r1)
    lis      r3, __vt__4CVec@ha
    addi     r0, r3, __vt__4CVec@l
    addi     r3, r31, 0x218
    stfs     f0, 0x218(r31)
    lfs      f0, 0x50(r1)
    stfs     f0, 0x21c(r31)
    lfs      f0, 0x54(r1)
    stfs     f0, 0x220(r31)
    stw      r0, 0x58(r1)
    stw      r0, 0x68(r1)
    bl       PSVECMag
    fmr      f31, f1
    addi     r3, r31, 0x1f8
    addi     r4, r31, 0x208
    bl       PSVECDistance
    fcmpo    cr0, f1, f31
    cror     eq, lt, eq
    beq      .L_80149C2C
    lfs      f0, lbl_8065ECA0@sda21(r0)
    fcmpo    cr0, f1, f0
    bge      .L_80149C50
.L_80149C2C:
    lfs      f0, 0x1f8(r31)
    li       r0, 0x1
    stfs     f0, 0x208(r31)
    lfs      f0, 0x1fc(r31)
    stfs     f0, 0x20c(r31)
    lfs      f0, 0x200(r31)
    stfs     f0, 0x210(r31)
    stw      r0, 0x228(r31)
    b        .L_80149C78
.L_80149C50:
    lwz      r6, 0x218(r31)
    addi     r3, r31, 0x208
    lwz      r0, 0x21c(r31)
    mr       r5, r3
    addi     r4, r1, 0x40
    stw      r6, 0x40(r1)
    stw      r0, 0x44(r1)
    lwz      r0, 0x220(r31)
    stw      r0, 0x48(r1)
    bl       PSVECAdd
.L_80149C78:
    mr       r3, r31
    addi     r4, r31, 0x208
    lwz      r12, 0x190(r31)
    lwz      r12, 0x70(r12)
    mtctr    r12
    bctrl
.L_80149C90:
    lwz      r0, 0x1e4(r31)
    clrlwi.  r0, r0, 31
    beq      .L_80149E04
    lwz      r0, 0x1f0(r31)
    cmpwi    r0, 0x1
    beq      .L_80149E04
    bge      .L_80149CB8
    cmpwi    r0, 0x0
    bge      .L_80149CC4
    b        .L_80149E04
.L_80149CB8:
    cmpwi    r0, 0x4
    bge      .L_80149E04
    b        .L_80149CE0
.L_80149CC4:
    mr       r3, r31
    addi     r4, r31, 0x194
    lwz      r12, 0x190(r31)
    lwz      r12, 0x6c(r12)
    mtctr    r12
    bctrl
    b        .L_80149E04
.L_80149CE0:
    lfs      f0, 0x194(r31)
    addi     r3, r1, 0x24
    addi     r4, r31, 0x1f8
    addi     r5, r1, 0x34
    stfs     f0, 0x1f8(r31)
    lfs      f0, 0x198(r31)
    stfs     f0, 0x1fc(r31)
    lfs      f0, 0x19c(r31)
    stfs     f0, 0x200(r31)
    lfs      f0, 0x94(r31)
    stfs     f0, 0x208(r31)
    lfs      f0, 0x98(r31)
    stfs     f0, 0x20c(r31)
    lfs      f0, 0x9c(r31)
    stfs     f0, 0x210(r31)
    lwz      r6, 0x208(r31)
    lwz      r0, 0x20c(r31)
    stw      r6, 0x34(r1)
    stw      r0, 0x38(r1)
    lwz      r0, 0x210(r31)
    stw      r0, 0x3c(r1)
    bl       __mi__4CVecCF3Vec
    lfs      f1, 0x22c(r31)
    addi     r3, r1, 0x14
    addi     r4, r1, 0x24
    bl       fn_8002442C
    lfs      f0, 0x14(r1)
    lis      r3, __vt__4CVec@ha
    addi     r0, r3, __vt__4CVec@l
    addi     r3, r31, 0x218
    stfs     f0, 0x218(r31)
    lfs      f0, 0x18(r1)
    stfs     f0, 0x21c(r31)
    lfs      f0, 0x1c(r1)
    stfs     f0, 0x220(r31)
    stw      r0, 0x20(r1)
    stw      r0, 0x30(r1)
    bl       PSVECMag
    fmr      f31, f1
    addi     r3, r31, 0x1f8
    addi     r4, r31, 0x208
    bl       PSVECDistance
    fcmpo    cr0, f1, f31
    cror     eq, lt, eq
    beq      .L_80149DA0
    lfs      f0, lbl_8065ECA0@sda21(r0)
    fcmpo    cr0, f1, f0
    bge      .L_80149DC4
.L_80149DA0:
    lfs      f0, 0x1f8(r31)
    li       r0, 0x1
    stfs     f0, 0x208(r31)
    lfs      f0, 0x1fc(r31)
    stfs     f0, 0x20c(r31)
    lfs      f0, 0x200(r31)
    stfs     f0, 0x210(r31)
    stw      r0, 0x228(r31)
    b        .L_80149DEC
.L_80149DC4:
    lwz      r6, 0x218(r31)
    addi     r3, r31, 0x208
    lwz      r0, 0x21c(r31)
    mr       r5, r3
    addi     r4, r1, 0x8
    stw      r6, 0x8(r1)
    stw      r0, 0xc(r1)
    lwz      r0, 0x220(r31)
    stw      r0, 0x10(r1)
    bl       PSVECAdd
.L_80149DEC:
    mr       r3, r31
    addi     r4, r31, 0x208
    lwz      r12, 0x190(r31)
    lwz      r12, 0x6c(r12)
    mtctr    r12
    bctrl
.L_80149E04:
    psq_l    f31, 0x128(r1), 0, qr0
    lwz      r0, 0x134(r1)
    lfd      f31, 0x120(r1)
    lwz      r31, 0x11c(r1)
    lwz      r30, 0x118(r1)
    mtlr     r0
    addi     r1, r1, 0x130
    blr
*/

/**
 * @note Address: 0x80149754
 * @note Size: 0x38
 */
void CMotionControlCamera::Draw()
{
	if (mAction != NULL) {
		mAction->vf20();
	}
}

/**
 * @note Address: 0x801496A0
 * @note Size: 0xB4
 */
s32 CMotionControlCamera::GetActionType()
{
	CInterfaceCameraAction* action = mAction;
	for (s32 i = 0; i < CAMACTION_NUM; i++) {
		if (action == GetAction(i)) {
			return i;
		}
	}
	return CAMACTION_NUM;
}
// fn_801496A0
/*
    lis      r4, lbl_8025C400@ha
    li       r0, 0x2
    lwz      r5, 0x1d0(r3)
    addi     r6, r4, lbl_8025C400@l
    li       r3, 0x0
    mtctr    r0
.L_801496B8:
    addis    r4, r6, 0xe
    lwz      r0, 0x7dec(r4)
    cmplw    r5, r0
    beqlr
    addi     r6, r6, 0x4
    addi     r3, r3, 0x1
    addis    r4, r6, 0xe
    lwz      r0, 0x7dec(r4)
    cmplw    r5, r0
    beqlr
    addi     r6, r6, 0x4
    addi     r3, r3, 0x1
    addis    r4, r6, 0xe
    lwz      r0, 0x7dec(r4)
    cmplw    r5, r0
    beqlr
    addi     r6, r6, 0x4
    addi     r3, r3, 0x1
    addis    r4, r6, 0xe
    lwz      r0, 0x7dec(r4)
    cmplw    r5, r0
    beqlr
    addi     r6, r6, 0x4
    addi     r3, r3, 0x1
    addis    r4, r6, 0xe
    lwz      r0, 0x7dec(r4)
    cmplw    r5, r0
    beqlr
    addi     r6, r6, 0x4
    addi     r3, r3, 0x1
    addis    r4, r6, 0xe
    lwz      r0, 0x7dec(r4)
    cmplw    r5, r0
    beqlr
    addi     r6, r6, 0x4
    addi     r3, r3, 0x1
    bdnz     .L_801496B8
    li       r3, 0xc
    blr
*/

/**
 * @note Address: 0x80149674
 * @note Size: 0x2C
 */
bool CMotionControlCamera::IsAction(s32 type)
{
	return IsCurrent(type);
}
// fn_80149674
/*
    lis      r5, lbl_8025C400@ha
    slwi     r0, r4, 2
    addi     r4, r5, lbl_8025C400@l
    lwz      r5, 0x1d0(r3)
    addis    r3, r4, 0xe
    add      r3, r3, r0
    lwz      r0, 0x7dec(r3)
    subf     r0, r5, r0
    cntlzw   r0, r0
    srwi     r3, r0, 5
    blr
*/

/**
 * @note Address: 0x801494A4
 * @note Size: 0x1D0
 */
void CMotionControlCamera::ChangeAction(CInterfaceCameraAction* action)
{
	if (mAction != action) {
		mPrevAction = mAction;
		mAction     = action;
		ReadAction();
		StartAction();
	}
}

/**
 * @note Address: 0x8014946C
 * @note Size: 0x38
 */
void CMotionControlCamera::ChangeAction(s32 type)
{
	ChangeAction(GetAction(type));
}

/**
 * @note Address: 0x801491D0
 * @note Size: 0x29C
 */
void CMotionControlCamera::PushAction(CInterfaceCameraAction* action)
{
	if (mAction != action) {
		mPushedAction = mAction;
		mAction       = action;
		ReadAction();
		StartAction();
		CopyAction();
	}
}

/**
 * @note Address: 0x80148F20
 * @note Size: 0x2B0
 */
void CMotionControlCamera::PopAction()
{
	if (mPushedAction != NULL) {
		mAction       = mPushedAction;
		mPushedAction = NULL;
		ReadAction();
		bool scroll = IsCurrent(CAMACTION_SCROLL);
		if (scroll) {
			StartAction();
		}
		CopyAction();
	}
}
// fn_80148F20
/*
    stwu     r1, -0x10(r1)
    mflr     r0
    stw      r0, 0x14(r1)
    stw      r31, 0xc(r1)
    mr       r31, r3
    lwz      r0, 0x1d8(r3)
    cmplwi   r0, 0x0
    beq      .L_801491BC
    stw      r0, 0x1d0(r31)
    li       r4, 0x0
    stw      r4, 0x1d8(r31)
    stw      r4, 0x1f0(r31)
    lwz      r5, 0x1d0(r31)
    cmplwi   r5, 0x0
    beq      .L_80149034
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    lwz      r0, 0x7e04(r3)
    cmplw    r5, r0
    bne      .L_80148FC8
    stw      r4, 0x1cc(r31)
    stw      r4, 0x1e4(r31)
    lwz      r0, 0x1e4(r31)
    ori      r0, r0, 0x1
    stw      r0, 0x1e4(r31)
    lwz      r0, 0x1e4(r31)
    ori      r0, r0, 0x2
    stw      r0, 0x1e4(r31)
    lwz      r0, 0x1e4(r31)
    ori      r0, r0, 0x8
    stw      r0, 0x1e4(r31)
    lwz      r0, 0x1e4(r31)
    ori      r0, r0, 0x10
    stw      r0, 0x1e4(r31)
    lwz      r0, 0x1e4(r31)
    ori      r0, r0, 0x4
    stw      r0, 0x1e4(r31)
    lwz      r0, 0x1e4(r31)
    oris     r0, r0, 0x4
    stw      r0, 0x1e4(r31)
    b        .L_80149034
.L_80148FC8:
    stw      r4, 0x1e4(r31)
    lwz      r3, 0x1d0(r31)
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x50(r12)
    mtctr    r12
    bctrl
    stw      r3, 0x1cc(r31)
    lwz      r3, 0x1d0(r31)
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x54(r12)
    mtctr    r12
    bctrl
    lwz      r0, 0x1e4(r31)
    or       r0, r0, r3
    stw      r0, 0x1e4(r31)
    lwz      r3, 0x1d0(r31)
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x58(r12)
    mtctr    r12
    bctrl
    stfs     f1, 0x22c(r31)
    lwz      r3, 0x1d0(r31)
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x5c(r12)
    mtctr    r12
    bctrl
    stw      r3, 0x1ec(r31)
.L_80149034:
    lis      r4, lbl_8025C400@ha
    lwz      r3, 0x1d0(r31)
    addi     r4, r4, lbl_8025C400@l
    addis    r4, r4, 0xe
    lwz      r0, 0x7e18(r4)
    subf     r0, r3, r0
    cntlzw   r0, r0
    srwi.    r0, r0, 5
    beq      .L_801490F0
    cmplwi   r3, 0x0
    beq      .L_801490F0
    lwz      r0, 0x7df4(r4)
    cmplw    r3, r0
    beq      .L_80149078
    lwz      r0, 0x7e14(r4)
    cmplw    r3, r0
    bne      .L_8014908C
.L_80149078:
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x14(r12)
    mtctr    r12
    bctrl
    b        .L_801490F0
.L_8014908C:
    lwz      r0, 0x7df8(r4)
    cmplw    r3, r0
    beq      .L_801490C8
    lwz      r0, 0x7df0(r4)
    cmplw    r3, r0
    beq      .L_801490C8
    lwz      r0, 0x7e08(r4)
    cmplw    r3, r0
    beq      .L_801490C8
    lwz      r0, 0x7e10(r4)
    cmplw    r3, r0
    beq      .L_801490C8
    lwz      r0, 0x7e04(r4)
    cmplw    r3, r0
    bne      .L_801490E0
.L_801490C8:
    lwz      r12, 0x3c(r3)
    mr       r4, r31
    lwz      r12, 0x18(r12)
    mtctr    r12
    bctrl
    b        .L_801490F0
.L_801490E0:
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x14(r12)
    mtctr    r12
    bctrl
.L_801490F0:
    lwz      r4, 0x1d0(r31)
    cmplwi   r4, 0x0
    beq      .L_801491BC
    lwz      r0, 0x1e4(r31)
    clrlwi.  r0, r0, 31
    beq      .L_8014911C
    lwz      r12, 0x190(r31)
    mr       r3, r31
    lwz      r12, 0x6c(r12)
    mtctr    r12
    bctrl
.L_8014911C:
    lwz      r0, 0x1e4(r31)
    rlwinm.  r0, r0, 0, 30, 30
    beq      .L_80149144
    mr       r3, r31
    lwz      r4, 0x1d0(r31)
    lwz      r12, 0x190(r31)
    addi     r4, r4, 0x10
    lwz      r12, 0x70(r12)
    mtctr    r12
    bctrl
.L_80149144:
    lwz      r0, 0x1e4(r31)
    rlwinm.  r0, r0, 0, 29, 29
    beq      .L_8014916C
    mr       r3, r31
    lwz      r4, 0x1d0(r31)
    lwz      r12, 0x190(r31)
    lfs      f1, 0x30(r4)
    lwz      r12, 0x68(r12)
    mtctr    r12
    bctrl
.L_8014916C:
    lwz      r0, 0x1e4(r31)
    rlwinm.  r0, r0, 0, 28, 28
    beq      .L_80149194
    mr       r3, r31
    lwz      r4, 0x1d0(r31)
    lwz      r12, 0x190(r31)
    lfs      f1, 0x34(r4)
    lwz      r12, 0x48(r12)
    mtctr    r12
    bctrl
.L_80149194:
    lwz      r0, 0x1e4(r31)
    rlwinm.  r0, r0, 0, 27, 27
    beq      .L_801491BC
    mr       r3, r31
    lwz      r4, 0x1d0(r31)
    lwz      r12, 0x190(r31)
    addi     r4, r4, 0x20
    lwz      r12, 0x74(r12)
    mtctr    r12
    bctrl
.L_801491BC:
    lwz      r0, 0x14(r1)
    lwz      r31, 0xc(r1)
    mtlr     r0
    addi     r1, r1, 0x10
    blr
*/

/**
 * @note Address: 0x80148E30
 * @note Size: 0xF0
 */
void CMotionControlCamera::ApplyAction()
{
	CopyAction();
}

/**
 * @note Address: 0x80148D6C
 * @note Size: 0xC4
 */
void CMotionControlCamera::UpdateTarget(XObj* obj)
{
	if (mAction != NULL && obj == mAction->GetTarget() && (mFlags.mFlags & CAMFLAG_INTEREST)) {
		mCamera.SetInterest(obj->GetCameraTarget());
		mAction->SetInterest(obj->GetCameraTarget());
	}
}

/**
 * @note Address: 0x80148C6C
 * @note Size: 0x100
 */
void CMotionControlCamera::StartEye()
{
	if (!(GAME_FLAGS & 0x100)) {
		GAME_FLAGS |= 0x100;
		PushAction(GetAction(CAMACTION_EYE));
		fn_800BADAC((XObj*)GAME_PLAYER);
		fn_800A30EC((XObj*)GAME_PLAYER, 1);
		fn_80023438(GAME_SCREEN, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0xFF, 320.0f, 224.0f, 550.0f, 500.0f, 430.0f, 400.0f, 0.15f, 0xFF, 0);
		fn_8001E904(GAME_SOUND, 4, 0xFF);
	}
}

/**
 * @note Address: 0x80148B78
 * @note Size: 0xF4
 */
void CMotionControlCamera::EndEye()
{
	if (GAME_FLAGS & 0x100) {
		GAME_FLAGS &= ~0x300ULL;
		PopAction();
		fn_800A30EC((XObj*)GAME_PLAYER, 0);
		fn_80023438(GAME_SCREEN, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 320.0f, 224.0f, 430.0f, 400.0f, 550.0f, 500.0f, 0.2f, 0, 1);
		fn_8001E904(GAME_SOUND, 2, 0xFF);
	}
}

/**
 * @note Address: 0x80148B00
 * @note Size: 0x78
 */
CCameraMemento* CMotionControlCamera::CreateMemento(CCameraMemento* memento)
{
	if (memento == NULL) {
		memento = &mMemento;
	}
	memento->SetEye(mCamera.mEye);
	memento->SetInterest(mCamera.mInterest);
	memento->SetAngle(mCamera.mAngle);
	memento->mDistance = mCamera.mDistance;
	memento->mFov      = mCamera.mFov;
	memento->mAction   = mAction;
	return memento;
}
// fn_80148B00
/*
    cmplwi   r4, 0x0
    beq      .L_80148B0C
    b        .L_80148B10
.L_80148B0C:
    addi     r4, r3, 0x230
.L_80148B10:
    lfs      f2, 0x9c(r3)
    lfs      f1, 0x98(r3)
    lfs      f0, 0x94(r3)
    stfs     f0, 0x4(r4)
    stfs     f1, 0x8(r4)
    stfs     f2, 0xc(r4)
    lfs      f2, 0xac(r3)
    lfs      f1, 0xa8(r3)
    lfs      f0, 0xa4(r3)
    stfs     f0, 0x14(r4)
    stfs     f1, 0x18(r4)
    stfs     f2, 0x1c(r4)
    lfs      f2, 0xbc(r3)
    lfs      f1, 0xb8(r3)
    lfs      f0, 0xb4(r3)
    stfs     f0, 0x24(r4)
    stfs     f1, 0x28(r4)
    stfs     f2, 0x2c(r4)
    lfs      f0, 0xe4(r3)
    stfs     f0, 0x38(r4)
    lfs      f0, 0xd8(r3)
    stfs     f0, 0x34(r4)
    lwz      r0, 0x1d0(r3)
    mr       r3, r4
    stw      r0, 0x3c(r4)
    blr
*/

/**
 * @note Address: 0x801487CC
 * @note Size: 0x334
 */
BOOL CMotionControlCamera::SetMemento(const CCameraMemento* memento)
{
	if (memento == NULL) {
		return FALSE;
	}
	mCamera.SetEye(memento->mEye);
	mCamera.SetInterest(memento->mInterest);
	mCamera.SetAngle(memento->mAngle);
	mCamera.SetFov(memento->mFov);
	mCamera.SetDistance(memento->mDistance);
	if (memento->mAction != NULL) {
		mPrevAction = mAction;
		mAction     = memento->mAction;
		ReadAction();
		StartAction();
		CopyAction();
	}
	return TRUE;
}

/**
 * @note Address: 0x80148768
 * @note Size: 0x64
 */
extern "C" f32 fn_80148768(CMotionControlCamera* camera)
{
	f32 zoom = ((CCameraActionAreaExM*)CMotionControlCamera::GetAction(CAMACTION_AREA))->vf68();
	if (0.0f != zoom) {
		return camera->mCamera.mFov / zoom;
	}
	return 1.0f;
}
// fn_80148768
/*
    stwu     r1, -0x10(r1)
    mflr     r0
    lis      r4, lbl_8025C400@ha
    stw      r0, 0x14(r1)
    stw      r31, 0xc(r1)
    mr       r31, r3
    addi     r3, r4, lbl_8025C400@l
    addis    r3, r3, 0xe
    lwz      r3, 0x7df0(r3)
    lwz      r12, 0x3c(r3)
    lwz      r12, 0x68(r12)
    mtctr    r12
    bctrl
    lfs      f0, lbl_8065EC78@sda21(r0)
    fcmpu    cr0, f1, f0
    beq      .L_801487B4
    lfs      f0, 0xd8(r31)
    fdivs    f1, f0, f1
    b        .L_801487B8
.L_801487B4:
    lfs      f1, lbl_8065EC74@sda21(r0)
.L_801487B8:
    lwz      r0, 0x14(r1)
    lwz      r31, 0xc(r1)
    mtlr     r0
    addi     r1, r1, 0x10
    blr
*/

/**
 * @note Address: 0x8014872C
 * @note Size: 0x3C
 */
extern "C" void fn_8014872C(CMotionControlCamera* camera)
{
	((CCameraActionAreaExM*)CMotionControlCamera::GetAction(CAMACTION_AREA))->vf70();
}

/**
 * @note Address: 0x801486D8
 * @note Size: 0x54
 */
f32 CMotionControlCamera::GetMapDistance()
{
	bool map = IsCurrent(CAMACTION_MAP);
	if (map) {
		return fn_8013A558((CCameraActionMap*)GetAction(CAMACTION_MAP)) - 4.0f;
	}
	return 1.0f;
}
// fn_801486D8
/*
    stwu     r1, -0x10(r1)
    mflr     r0
    lis      r4, lbl_8025C400@ha
    stw      r0, 0x14(r1)
    lwz      r0, 0x1d0(r3)
    addi     r3, r4, lbl_8025C400@l
    addis    r3, r3, 0xe
    lwz      r3, 0x7e08(r3)
    subf     r0, r0, r3
    cntlzw   r0, r0
    srwi.    r0, r0, 5
    beq      .L_80148718
    bl       fn_8013A558
    lfs      f0, lbl_8065EC70@sda21(r0)
    fsubs    f1, f1, f0
    b        .L_8014871C
.L_80148718:
    lfs      f1, lbl_8065EC74@sda21(r0)
.L_8014871C:
    lwz      r0, 0x14(r1)
    mtlr     r0
    addi     r1, r1, 0x10
    blr
*/

/**
 * @note Address: 0x8014861C
 * @note Size: 0xBC
 */
void CCameraActions::Init()
{
	mTable[CAMACTION_DEFAULT] = &mDefault;
	mTable[CAMACTION_AREA]    = (CInterfaceCameraAction*)mArea;
	mTable[CAMACTION_CHASE]   = &mChase;
	mTable[CAMACTION_EVT]     = &mEvt;
	mTable[CAMACTION_SCROLL2] = &mScroll2;
	mTable[CAMACTION_EYE]     = (CInterfaceCameraAction*)mEye;
	mTable[CAMACTION_ANIM]    = &mAnim;
	mTable[CAMACTION_MAP]     = &mMap;
	mTable[CAMACTION_LADR]    = &mLadr;
	mTable[CAMACTION_RESET]   = &mReset;
	mTable[CAMACTION_10]      = NULL;
	mTable[CAMACTION_SCROLL]  = &mScroll;
	for (s32 i = 0; i < CAMACTION_NUM; i++) {
		if (mTable[i] != NULL) {
			mTable[i]->Init();
		}
	}
}
