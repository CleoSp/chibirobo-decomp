#define DEFER_CAMERA_RECT_SETTERS
#include "game/cgame.h"
#include <Dolphin/vi.h>
#include "game/cgameutil.h"

/**
 * @note Address: 0x800153B4
 * @note Size: 0x4
 */
#pragma section RX ".text.baseupdate"
extern "C" __declspec(section ".text.baseupdate") void vf24__5CBaseFv(CBase* base)
{
}

/**
 * @note Address: 0x800153B8
 * @note Size: 0x4
 */
extern "C" __declspec(section ".text.baseupdate") void vf20__5CBaseFv(CBase* base)
{
}

extern "C" {
void AXQuit(void);
void AIReset(void);
void AIStopDMA(void);
void* HSD_VISetUserGXDrawDoneCallback(void* cb);
void GXDrawDone(void);
void GXAbortFrame(void);
void OSResetSystem(int reset, u32 code, BOOL forceMenu);
}

/**
 * @note Address: 0x800153BC
 * @note Size: 0x170
 */
extern "C" __declspec(section ".text.baseupdate") void Update__5CBaseFUl(CBase* base, u32 frames)
{
	BaseUpdate(base, frames);
}

// stops the sound and the video and resets
inline void ResetSystem(u32 code)
{
#if defined(VERSION_PAL)
	AXDriverStopAllStreams();
	HSD_AudioSFXKeyOffAll();
#else
	AXQuit();
	AIReset();
#endif
	HSD_VISetUserGXDrawDoneCallback(NULL);
	GXDrawDone();
	GXAbortFrame();
	VISetBlack(TRUE);
	VIFlush();
	VIWaitForRetrace();
#if defined(VERSION_PAL)
	AXQuit();
	AIStopDMA();
#endif
	OSResetSystem(code, 0, 0);
}

/**
 * @note Address: 0x800152A0
 * @note Size: 0x60
 */
// CBase::Reset
#pragma section RX ".text.systemresetprojection"
extern "C" __declspec(section ".text.systemresetprojection") void Reset__5CBaseFUl(CBase* base, u32 code)
{
	ResetSystem(code);
}

extern "C" void GXProject(f32 x, f32 y, f32 z, const Mtx mtx, const f32* pm, const f32* vp, f32* sx, f32* sy, f32* sz);

/**
 * @note Address: 0x80015300
 * @note Size: 0xB4
 */
// where a point ends up on screen through a camera, the main one by default
extern "C" __declspec(section ".text.systemresetprojection") void ProjectWorldPositionToScreen(CGame* game, const Vec& pos, f32* x, f32* y,
                                                                                               f32* z, CCamera* camera)
{
	camera = camera != NULL ? camera : &gGame.mMotionControlCamera.mCamera;
	f32 sx, sy, sz;
	GXProject(pos.x, pos.y, pos.z, camera->mViewMtx, camera->mProjection, camera->mViewport, &sx, &sy, &sz);
	if (x != NULL) {
		*x = sx;
	}
	if (y != NULL) {
#if defined(VERSION_PAL)
		*y = 448.0f * sy / gGame.mGs.mScreen.xfbHeight;
#else
		*y = sy;
#endif
	}
	if (z != NULL) {
		*z = sz;
	}
}

/**
 * @note Address: 0x800151DC
 * @note Size: 0x4
 */
#pragma section RX ".text.mirrorinit"
extern "C" __declspec(section ".text.mirrorinit") void Reset__8FbMirrorFv(void)
{
}

/**
 * @note Address: 0x80014DA4
 * @note Size: 0x2BC
 */
// FbMirror::Update, puts the mirror camera at the main camera's eye reflected in the surface,
// looking at the surface through a frustum that just fits it
extern "C" void Update__8FbMirrorFUl(FbMirror* mirror, u32 frames)
{
	HSD_JObj* jobj  = mirror->mJoint;
	BOOL hasSurface = jobj != NULL;
	if (hasSurface & TRUE) {
		Mtx mtx;
		Mtx view;
		Vec eye;
		Vec pos;
		Vec diff;
		Vec x;
		Vec y;
		Vec normal;

		PSMTXCopy(HSD_JObjGetMtxPtr(jobj), mtx);
		x.x = mtx[0][0];
		x.y = mtx[1][0];
		x.z = mtx[2][0];
		y.x = mtx[0][1];
		y.y = mtx[1][1];
		y.z = mtx[2][1];
		PSVECScale(&x, &x, -1.0f);
		PSVECCrossProduct(&x, &y, &normal);
		PSVECNormalize(&normal, &normal);
		mtx[0][0] = x.x;
		mtx[1][0] = x.y;
		mtx[2][0] = x.z;
		mtx[0][2] = normal.x;
		mtx[1][2] = normal.y;
		mtx[2][2] = normal.z;

		CMotionControlCamera* main = &gGame.mMotionControlCamera;
		eye.x                      = main->mCamera.mEye.x;
		eye.y                      = main->mCamera.mEye.y;
		eye.z                      = main->mCamera.mEye.z;
		HSD_MtxGetTranslate(mtx, &pos);
		PSVECSubtract(&pos, &eye, &diff);
		f32 dist = PSVECDotProduct(&normal, &diff);
		if (dist <= 0.0f) {
			mirror->mVisible = FALSE;
		} else {
			PSVECScale(&normal, &diff, 2.0f * dist);
			PSVECAdd(&diff, &eye, &diff);
			mtx[0][3] = diff.x;
			mtx[1][3] = diff.y;
			mtx[2][3] = diff.z;
			PSMTXInverse(mtx, view);
			PSMTXMultVec(view, &pos, &pos);
			mirror->mCamera.SetNear(0.1f - pos.z);
			mirror->mCamera.SetFar(32768.0f + (0.1f - pos.z));
			mirror->mCamera.SetFrustum(0.5f + pos.y, -0.5f + pos.y, -0.5f + pos.x, 0.5f + pos.x);
			mirror->mCamera.SetEye(diff);
			mirror->mCamera.SetMtx(view);
			PSMTXCopy(view, mirror->mView);
			mirror->mVisible = TRUE;
		}
	}
}

/**
 * @note Address: 0x800151E0
 * @note Size: 0xC0
 */
// FbMirror::Init
extern "C" __declspec(section ".text.mirrorinit") void Init__8FbMirrorFv(FbMirror* mirror)
{
	mirror->mJoint = NULL;
	mirror->mCamera.Init(0.1f, 32768.0f, 0.5f, -0.5f, -0.5f, 0.5f, &gGame.mGs.mScreen);
	mirror->mCamera.SetViewport(0.0f, 256.0f, 0.0f, 256.0f);
	mirror->mCamera.SetScissor(0, 256, 0, 256);
	mirror->Reset();
}
