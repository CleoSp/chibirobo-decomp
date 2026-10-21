#include "game/ccameraaction.h"
#include "game/hsd_jobj.h"
#include "game/gmath.h"
#include "sysdolphin/controller.h"
#include "Version.h"
#include <math.h>

// CGame's camera fields, by offset
extern "C" u8 lbl_8025C400[];
#if defined(VERSION_JP)
#define GAME_FLAGS  (*(u64*)(lbl_8025C400 + 0x1AF8))
#define GAME_PLAYER (*(XObj**)(lbl_8025C400 + 0x9674))
#elif defined(VERSION_PAL)
#define GAME_FLAGS  (*(u64*)(lbl_8025C400 + 0x1B18))
#define GAME_PLAYER (*(XObj**)(lbl_8025C400 + 0x96BC))
#else
#define GAME_FLAGS  (*(u64*)(lbl_8025C400 + 0x1B10))
#define GAME_PLAYER (*(XObj**)(lbl_8025C400 + 0x96BC))
#endif

extern "C" f32 lbl_8022E6E8[3];
extern "C" void PSVECScale(const Vec* src, Vec* dst, f32 scale);

// backwards from here, deferred inlining

// CVec's += takes a reference, this one copies b first like retail does here
inline void AddCopy(CVec& a, Vec b)
{
	PSVECAdd(&a, &b, &a);
}

/**
 * @note Address: 0x80142AF8
 * @note Size: 0x2F0
 */
static CVec CalcSpring(CVec& eye, const CVec& target, const CVec& last, f32 dt, f32 stiffness, f32 damping, f32 rest)
{
	CVec dir;
	CVec vel;
	dir     = eye - target;
	f32 len = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	vel     = (last - target) * dt;
	f32 dot = PSVECDotProduct(&dir, &vel);
	PSVECNormalize(&dir, &dir);
	f32 force = stiffness * (rest - len) + damping * (dot / len);
	PSVECScale(&dir, &dir, force * dt);
	AddCopy(eye, dir);
	return eye;
}

/**
 * @note Address: 0x80142AE4
 * @note Size: 0x14
 */
void CCameraActionChase::Init()
{
	mDamping      = 0.5f;
	mRestDistance = 105.0f;
}

/**
 * @note Address: 0x8014295C
 * @note Size: 0x188
 */
void CCameraActionChase::vf14()
{
	mTarget = GAME_PLAYER;
	if (mTarget != NULL) {
		mRestDistance = 105.0f;
		Mtx mtx;
		PSMTXCopy(HSD_JObjGetMtxPtr(mTarget->mJObj), mtx);
		CVec eye(0.0f, 40.0f, -mRestDistance);
		PSMTXMultVec(mtx, &eye, &eye);
		SetEye(eye);
		mPos.x     = mEye.x;
		mPos.y     = mEye.y;
		mPos.z     = mEye.z;
		mLastPos.x = mPos.x;
		mLastPos.y = mPos.y;
		mLastPos.z = mPos.z;
		SetInterest(mTarget->GetCameraTarget());
	}
}

/**
 * @note Address: 0x801428B8
 * @note Size: 0xA4
 */
void CCameraActionChase::SetCamera(CCamera* camera)
{
	SetEye(camera->mEye);
	SetInterest(camera->mInterest);
	SetDistance(camera->mDistance);
	SetFov(camera->mFov);
	mTarget = GAME_PLAYER;
}

/**
 * @note Address: 0x80142440
 * @note Size: 0x478
 */
void CCameraActionChase::Update(u32 step)
{
	if (mTarget == NULL) {
		return;
	}

	f32 dt     = 0.1f * step;
	mLastPos.x = mPos.x;
	mLastPos.y = mPos.y;
	mLastPos.z = mPos.z;

	Mtx mtx;
	PSMTXCopy(HSD_JObjGetMtxPtr(mTarget->mJObj), mtx);
	CVec behind(0.0f, 30.0f, -1.0f);
	PSMTXMultVec(mtx, &behind, &behind);
	mPos.x = behind.x;
	mPos.y = behind.y;
	mPos.z = behind.z;

	CVec eye((const Vec&)mEye);
	eye = CalcSpring(eye, mPos, mLastPos, dt, lbl_8022E6E8[0], mDamping, mRestDistance);
	SetEye(eye);
	SetInterest(mTarget->GetCameraTarget());
	mMode = 3;

	CPad pad(0);
	u32 leftButton  = (GAME_FLAGS & 0x80000000) ? HSD_PAD_DPADLEFT : HSD_PAD_CSTICK_LEFT;
	u32 rightButton = (GAME_FLAGS & 0x80000000) ? HSD_PAD_DPADRIGHT : HSD_PAD_CSTICK_RIGHT;
	u32 upButton    = (GAME_FLAGS & 0x80000000) ? HSD_PAD_DPADUP : HSD_PAD_CSTICK_UP;
	u32 downButton  = (GAME_FLAGS & 0x80000000) ? HSD_PAD_DPADDOWN : HSD_PAD_CSTICK_DOWN;

	{
		CVec angle = mAngle;
		f32 pitch  = 0.0f;
		f32 yaw    = 0.0f;
		if (pad.mStatus->_04 & rightButton) {
			yaw = 1.0f;
		} else if (pad.mStatus->_04 & leftButton) {
			yaw = -1.0f;
		}
		if (yaw != 0.0f) {
			f32 dx = step * (pitch * 0.05235988f);
			f32 dy = step * (yaw * 0.05235988f);
			f32 x  = angle.x + dx;
			f32 y  = angle.y + dy;
			if (x > 1.5550883f) {
				x = 1.5550883f;
			}
			if (x < -1.5550883f) {
				x = -1.5550883f;
			}
			SetAngle(x, y, angle.z);
			mMode = 0x20012;
		}
	}

	u32 up = pad.mStatus->_04 & upButton;
	if (up || (pad.mStatus->_04 & downButton)) {
		f32 distance = mDistance;
		if (up) {
			distance -= 12.0f * step;
			if (distance < 60.0f) {
				distance = 60.0f;
			}
		}
		if (pad.mStatus->_04 & downButton) {
			distance += 12.0f * step;
		}
		SetDistance(distance);
		mRestDistance = distance;
		mMode         = 0x80006;
	}
}
