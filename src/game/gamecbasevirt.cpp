extern "C" {
#include <sysdolphin/controller.h>
}
#include "game/cpad.h"
#include "game/fbtexeff.h"
#include "game/clight.h"
#include "game/cgame.h"
#include "game/cfont.h"
#include "game/csound.h"
#include "game/video.h"
#include <dolphin/ai.h>
#include <dolphin/dvd.h>
#include "Dolphin/OS/OSReset.h"
extern "C" {
#include <sysdolphin/video.h>
#include <sysdolphin/rumble.h>
}

struct OSStopwatch;

extern "C" {
void AXQuit();
void GXDrawDone();
void GXAbortFrame();
void GXProject(f32 x, f32 y, f32 z, const Mtx view, const f32* projection, const f32* viewport, f32* screenX, f32* screenY, f32* screenZ);
void OSInitStopwatch(OSStopwatch* stopwatch, char* name);
// file private, all from 0x8001C000 to 0x8001E000
void fn_801CDC34();
void fn_8001D8C0();
void fn_8001D51C(CBase* base);
void fn_8001D4EC();
void fn_8001D4C0();
#if defined(VERSION_PAL)
u8 OSGetLanguage();
// pairs of the system language and the game's
extern u8 lbl_802162E0[12];
#endif
}

/**
 * @note Address: 0x800151DC
 * @note Size: 0x4
 */
// FbMirror's Reset
extern "C" void fn_800151DC(FbMirror* mirror)
{
}

/**
 * @note Address: 0x800151E0
 * @note Size: 0xC0
 */
// FbMirror's Init, a 256 square view
extern "C" void fn_800151E0(FbMirror* mirror)
{
	mirror->mJoint = NULL;
	mirror->mCamera.Init(0.1f, 32768.0f, 0.5f, -0.5f, -0.5f, 0.5f, (GXRenderModeObj*)lbl_8025C400.mGs);
	mirror->mCamera.SetViewport(0.0f, 256.0f, 0.0f, 256.0f);
	mirror->mCamera.SetScissor(0, 256, 0, 256);
	mirror->Reset();
}

/**
 * @note Address: 0x800152A0
 * @note Size: 0x60
 */
// CBase's _48, shuts the hardware down and resets
extern "C" void fn_800152A0(CBase* base, s32 reset)
{
#if defined(VERSION_PAL)
	fn_801E9FA4();
	fn_801E8CC0();
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
	OSResetSystem(reset, 0, FALSE);
}

/**
 * @note Address: 0x80015300
 * @note Size: 0xB4
 */
// a world position on screen, through the game camera unless one is given
extern "C" void fn_80015300(CBase* base, const Vec* position, f32* x, f32* y, f32* z, CCamera* camera)
{
	camera = camera != NULL ? camera : (CCamera*)lbl_8025C400.mCamera;
	f32 screenX, screenY, screenZ;
	GXProject(position->x, position->y, position->z, camera->mViewMtx, camera->mProjection, camera->mViewport, &screenX, &screenY,
	          &screenZ);
	if (x != NULL)
		*x = screenX;
	if (y != NULL) {
#if defined(VERSION_PAL)
		*y = 448.0f * screenY / ((GXRenderModeObj*)lbl_8025C400.mGs)->xfbHeight;
#else
		*y = screenY;
#endif
	}
	if (z != NULL)
		*z = screenZ;
}

/**
 * @note Address: 0x800153B4
 * @note Size: 0x4
 */
// CBase's vf24
extern "C" void fn_800153B4(CBase* base)
{
}

/**
 * @note Address: 0x800153B8
 * @note Size: 0x4
 */
// CBase's vf20
extern "C" void fn_800153B8(CBase* base)
{
}

/**
 * @note Address: 0x800153BC
 * @note Size: 0x170
 */
// CBase's Update, watches the disc drive and the reset button
extern "C" void fn_800153BC(CBase* base)
{
	base->mPrevDvdStatus = base->mDvdStatus;
	base->mDvdStatus     = DVDGetDriveStatus();
	base->mDiskOk        = DVDCheckDisk();
	switch (base->mDvdStatus) {
	case -1:
	case 4:
	case 5:
	case 6:
	case 11:
		if (base->mDvdStatus != base->mPrevDvdStatus) {
			base->mDvdError = 1;
			CPadEx* pads    = (CPadEx*)base->mPad;
			for (s32 i = 0; i < 4; ++i) {
				if (pads->mPad[i].mConnected != 0)
					HSD_PadRumbleOffN(pads->mPad[i].mPort);
			}
			HSD_PadRumblePauseAll();
		}
		break;
	case 0:
		if (base->mPrevDvdStatus != 0 && base->mDvdError != 0) {
			base->mDvdError = 0;
			fn_801CDC34();
		}
		break;
	}
	if (base->mReset == 0) {
		if (HSD_PadGetResetSwitch()) {
			base->mResetHold = 0;
			base->mReset     = 1;
		} else if ((((CPadOne*)base->mPadOne[0])->mButton & 0xFFFF) == 0x1600) {
			// b, x and start held for a second
			s32 count        = (s32)(base->mResetHold + base->mRetraceStep);
			base->mResetHold = count;
#if defined(VERSION_PAL)
			if (count > lbl_80660D4C) {
#else
			if (count > 60) {
#endif
				base->mResetHold = 0;
				base->mReset     = 1;
			}
		} else if (base->mReset == 0) {
			base->mResetHold = 0;
		}
	}
}

/**
 * @note Address: 0x8001552C
 * @note Size: 0x8
 */
// CBase's BeforeUpdate
extern "C" s32 fn_8001552C(CBase* base)
{
	return 1;
}

/**
 * @note Address: 0x80015534
 * @note Size: 0x100
 */
// CBase's Start
extern "C" void fn_80015534(CBase* base)
{
	fn_8001D8C0();
	fn_8001D51C(base);
	fn_8001D4EC();
	fn_8001D4C0();
#if defined(VERSION_PAL)
	u8 language = OSGetLanguage();
	for (s32 i = 0; i < 6; ++i) {
		if (language == lbl_802162E0[i * 2]) {
			base->mLanguage = lbl_802162E0[i * 2 + 1];
			break;
		}
	}
#elif !defined(VERSION_JP)
	base->mLanguage = 0;
#endif
#if defined(VERSION_JP)
	fn_8019EC40((CFont*)base->mFont);
#else
	fn_8019EC40((CFont*)base->mFont, lbl_8025C400.mLanguage != 6);
#endif
	((CPadEx*)base->mPad)->Init();
	base->mLastRetrace   = 0;
	base->mRetraceStep   = 0;
	base->mStatus.mFlags = 0;
	fn_8001D3D4((CCamera*)base->mCameras[0], (GXRenderModeObj*)base);
	fn_8001D1D0((CCamera*)base->mCameras[1], (GXRenderModeObj*)base);
	fn_8001CFD0((CLight*)base->mLights[0]);
	fn_8001CEFC((CLight*)base->mLights[4]);
	OSInitStopwatch((OSStopwatch*)&base->mStopwatch[0], "__$dsync$ ");
	OSInitStopwatch((OSStopwatch*)&base->mStopwatch[1], "__$update$");
	OSInitStopwatch((OSStopwatch*)&base->mStopwatch[2], "__$draw$  ");
	base->mPrevDvdStatus = 0;
	base->mDvdStatus     = 0;
	base->mDiskOk        = 1;
	base->mDvdError      = 0;
	base->mResetHold     = 0;
	base->mReset         = 0;
}

/**
 * @note Address: 0x80015634
 * @note Size: 0x4
 */
// CBase's Initialize
extern "C" void fn_80015634(CBase* base, int argc, char** argv)
{
}

/**
 * @note Address: 0x80015638
 * @note Size: 0x8
 */
// CBase's FrameInterval
extern "C" s32 fn_80015638(CBase* base)
{
	return 1;
}

/**
 * @note Address: 0x80015640
 * @note Size: 0x4
 */
// CBase's Draw
extern "C" void fn_80015640(CBase* base)
{
}
