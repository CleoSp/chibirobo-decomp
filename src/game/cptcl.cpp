#include "game/hsd_jobj.h"
#include "game/ut.h"
extern "C" {
#include <dolphin/mtx/vec.h>
}
#include "game/cvec.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/cptcl.h"
#include <math.h>
#include <stdlib.h>
#include "game/gmath.h"

// rotation templates the draws copy, dtk merged the dust2 ones into one block
struct Dust2Rotations {
	u8 _00[0x60]; // _00
	Vec mTex;     // _60
	Vec mFirst;   // _6C
	Vec mSecond;  // _78
};

extern "C" {
extern Vec lbl_801FF2C4;
extern Vec lbl_801FF2D0;
extern Dust2Rotations lbl_801FF240;
}

/**
 * @note Address: 0x800245F0
 * @note Size: 0x10
 */
extern "C" void Init__8CPtclOneFv(CPtclOne* ptcl)
{
	ptcl->mActive = FALSE;
	ptcl->mTex    = NULL;
}

/**
 * @note Address: 0x80024600
 * @note Size: 0x1D8
 */
extern "C" void Draw__10CPtclDust3FP3CGs(CPtclDust3* ptcl, CGs* gs)
{
	if (ptcl->mActive == FALSE) {
		return;
	}
	f32 width = ptcl->mSize.x * (f32)sin(0.8f * (PI * ptcl->mAlpha));
	width *= 2.0f;
	f32 height = ptcl->mSize.y * (f32)sin(0.8f * (PI * ptcl->mAlpha));
	height *= 2.0f;
	u32 edgeColor = ptcl->mColor & 0xFFFFFF00;
	u32 color     = (ptcl->mColor & 0xFFFFFF00) | ((u32)(s32)((f32)(ptcl->mColor & 0xFF) * ptcl->mAlpha) & 0xFF);
	if (ptcl->mTex != NULL) {
		fn_8019AFA0(gs->mCurTevStage, 0);
		Vec rot = lbl_801FF2C4;
		rot.z   = ptcl->mAngle;
		fn_80197800(gs, ptcl->mTex, color, color, color, color, ptcl->mPos.x, ptcl->mPos.y, ptcl->mPos.z, width, height, rot.x, rot.y,
		            rot.z);
		fn_8019AFA0(gs->mCurTevStage, 4);
	} else {
		Vec rot = lbl_801FF2D0;
		rot.z   = ptcl->mAngle;
		fn_80198258(gs, 8, color, edgeColor, ptcl->mPos.x, ptcl->mPos.y, ptcl->mPos.z, width, height, rot.x, rot.y, rot.z);
	}
}

// todo: cleanup
static inline void Move(CVec& pos, Vec offset)
{
	pos += offset;
}

/**
 * @note Address: 0x800247D8
 * @note Size: 0x15C
 */
extern "C" void Update__10CPtclDust3FUl(CPtclDust3* ptcl, u32 frames)
{
	if (ptcl->mActive == FALSE) {
		return;
	}
	if (ptcl->mAlphaStep < 0.0f) {
		if ((ptcl->mAlpha -= ptcl->mAlphaStep * (f32)frames) >= 1.0f) {
			ptcl->mAlphaStep = -ptcl->mAlphaStep;
		}
	}
	if (ptcl->mAlphaStep > 0.0f) {
		if ((ptcl->mAlpha -= ptcl->mAlphaStep * (f32)frames) <= 0.0f) {
			ptcl->mActive = FALSE;
			ptcl->mAlpha  = 0.0f;
			return;
		}
	}
	Move(ptcl->mPos, (ptcl->mVel * 1.0f) * (f32)frames);
	ptcl->mAngle += ptcl->mAngleSpeed;
}

/**
 * @note Address: 0x80024934
 * @note Size: 0x1E8
 */
extern "C" void Draw__10CPtclDust2FP3CGs(CPtclDust2* ptcl, CGs* gs)
{
	if (ptcl->mActive == FALSE) {
		return;
	}
	u32 color  = (ptcl->mColor & 0xFFFFFF00) | ((u32)(s32)((f32)(ptcl->mColor & 0xFF) * ptcl->mAlpha) & 0xFF);
	f32 radius = 1.5f * (ptcl->mSize.x > ptcl->mSize.y ? ptcl->mSize.x : ptcl->mSize.y);
	if (ptcl->mTex != NULL) {
		fn_8019AFA0(gs->mCurTevStage, 0);
		Vec rot = lbl_801FF240.mTex;
		rot.z   = ptcl->mAngle;
		fn_80197800(gs, ptcl->mTex, color, color, color, color, ptcl->mPos.x, ptcl->mPos.y, ptcl->mPos.z, radius, radius, rot.x, rot.y,
		            rot.z);
		fn_8019AFA0(gs->mCurTevStage, 4);
	} else {
		// two triangles make a star
		Vec first = lbl_801FF240.mFirst;
		first.z   = ptcl->mAngle;
		fn_80198258(gs, 3, color, color, ptcl->mPos.x, ptcl->mPos.y, ptcl->mPos.z, radius, radius, first.x, first.y, first.z);
		Vec second = lbl_801FF240.mSecond;
		second.z   = THIRD_PI + ptcl->mAngle;
		fn_80198258(gs, 3, color, color, ptcl->mPos.x, ptcl->mPos.y, ptcl->mPos.z, radius, radius, second.x, second.y, second.z);
	}
}

/**
 * @note Address: 0x80024B1C
 * @note Size: 0x184
 */
extern "C" void Update__10CPtclDust2FUl(CPtclDust2* ptcl, u32 frames)
{
	if (ptcl->mActive == FALSE) {
		return;
	}
	if (ptcl->mAlphaStep < 0.0f) {
		if ((ptcl->mAlpha -= ptcl->mAlphaStep * (f32)frames) >= 1.0f) {
			ptcl->mAlphaStep = -ptcl->mAlphaStep;
		}
	}
	if (ptcl->mAlphaStep > 0.0f) {
		if ((ptcl->mAlpha -= ptcl->mAlphaStep * (f32)frames) <= 0.0f) {
			ptcl->mActive = FALSE;
			ptcl->mAlpha  = 0.0f;
			return;
		}
	}
	ptcl->mVel.y -= 0.05f * (f32)frames;
	Move(ptcl->mPos, (ptcl->mVel * 1.0f) * (f32)frames);
	ptcl->mAngle += ptcl->mAngleSpeed;
}

/**
 * @note Address: 0x80024CA0
 * @note Size: 0xE0
 */
extern "C" void fn_80024CA0(CPtcl* ptcl)
{
	CGs* gs = (CGs*)lbl_8025C400.mGs;
	fn_80199AB0(gs, 0, 0);
	fn_8019B56C(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 4, 5, 15);
	fn_8019B464(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 3, 0);
	((CGs*)lbl_8025C400.mGs)->_29C = 1;
	for (s32 i = 0; i < ptcl->mCount; ++i) {
		CPtclOne* one = ptcl->Get(i);
		if (one->mActive != FALSE) {
			one->Draw((CGs*)lbl_8025C400.mGs);
		}
	}
	((CGs*)lbl_8025C400.mGs)->_29C = 0;
}

/**
 * @note Address: 0x80024D80
 * @note Size: 0x4
 */
extern "C" void Draw__8CPtclOneFP3CGs(CPtclOne* ptcl, CGs* gs)
{
}

/**
 * @note Address: 0x80024D84
 * @note Size: 0x84
 */
extern "C" void fn_80024D84(CPtcl* ptcl, u32 frames)
{
	for (s32 i = 0; i < ptcl->mCount; ++i) {
		CPtclOne* one = ptcl->Get(i);
		if (one->mActive != FALSE) {
			one->Update(frames);
		}
	}
}

/**
 * @note Address: 0x80024E08
 * @note Size: 0x4
 */
extern "C" void Update__8CPtclOneFUl(CPtclOne* ptcl, u32 frames)
{
}

/**
 * @note Address: 0x80024E0C
 * @note Size: 0x44
 */
extern "C" void fn_80024E0C(CPtcl* ptcl)
{
	for (s32 i = 0; i < ptcl->mCount; ++i) {
		CPtclOne* one = ptcl->Get(i);
		if (one->mActive != FALSE) {
			one->mActive = FALSE;
			one->mTex    = NULL;
		}
	}
}

/**
 * @note Address: 0x80024E50
 * @note Size: 0x15C
 */
extern "C" CPtclOne* fn_80024E50(CPtcl* ptcl, const Vec* pos, const Vec* size, const Vec* vel, u32 color)
{
	for (s32 i = 0; i < ptcl->mCount; ++i) {
		CPtclOne* one = ptcl->Get(i);
		if (one->mActive == FALSE) {
			one->mActive     = TRUE;
			one->mPos        = CVec(*pos);
			one->mVel        = CVec(*vel);
			one->mSize       = CVec(*size);
			one->mAngle      = (f32)fmod((f32)rand(), PI);
			one->mAngleSpeed = 0.0f;
			one->mColor      = color;
			one->mAlpha      = 1.0f;
			one->mAlphaStep  = 0.02f;
			one->mTex        = NULL;
			return one;
		}
	}
	return NULL;
}

/**
 * @note Address: 0x80024FAC
 * @note Size: 0x84
 */
extern "C" void fn_80024FAC(CPtcl* ptcl, void* ptcls, s32 stride, s32 count)
{
	ptcl->mPtcls   = (u8*)ptcls;
	ptcl->mStride  = stride;
	ptcl->mCount   = count;
	ptcl->mCounter = 0;
	for (s32 i = 0; i < count; ++i) {
		ptcl->Get(i)->Reset();
	}
}
