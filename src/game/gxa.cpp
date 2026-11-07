#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/cgsdefaults.h"
#include "game/csound.h"
#include "game/mm.h"
#include "game/progressive.h"
#include "game/video.h"
#include "game/vistate.h"
extern "C" {
#include <dolphin/gx/gx.h>
extern GXRenderModeObj GXPal528IntDf;
void HSD_SetPixelFormat(s32, s32);
void HSD_SetZSortMode(s32, s32);
void GXSetClipMode(s32);
}
#include "Dolphin/OS/OSFastCast.h"

extern "C" {
#include <sysdolphin/initialize.h>
}

// the game core's start-up pieces: the light sets and cameras the scenes start from, the
// system setup and the anim sfx. the functions whose class is still unknown are extern "C"
// under their dol address

// the light sets and cameras the scenes start from. each one sets up the objects it gets and
// hands them back

/**
 * @note Address: 0x8001CEFC
 * @note Size: 0xD4
 */
extern "C" CLight* InitTwoLightSet(CLight* lights)
{
	CLight* light;

	lights[0].Init(4, FALSE);
	lights[0].mColor.r = 0x80;
	lights[0].mColor.g = 0x80;
	lights[0].mColor.b = 0x80;
	lights[0].mColor.a = 0xFF;
	HSD_LObjSetColor(&lights[0], lights[0].mColor);

	lights[0].next = &lights[1];
	light          = &lights[1];
	light->Init(5, FALSE);
	light->SetPosition(1.0f, -1.0f, 1.0f);
	light->mColor.r = 0xFF;
	light->mColor.g = 0xFF;
	light->mColor.b = 0xFF;
	light->mColor.a = 0xFF;
	HSD_LObjSetColor(light, light->mColor);
	light->next = NULL;
	return lights;
}

/**
 * @note Address: 0x8001CFD0
 * @note Size: 0x1D0
 */
extern "C" CLight* InitFourLightSet(CLight* lights)
{
	CLight* light;

	lights[0].Init(4, FALSE);
	lights[0].mColor.r = 200;
	lights[0].mColor.g = 200;
	lights[0].mColor.b = 200;
	lights[0].mColor.a = 0xFF;
	HSD_LObjSetColor(&lights[0], lights[0].mColor);

	lights[0].next = &lights[1];
	light          = &lights[1];
	light->Init(5, TRUE);
	light->SetPosition(-2.0f, 20.0f, 2.0f);
	light->mColor.r = 200;
	light->mColor.g = 200;
	light->mColor.b = 200;
	light->mColor.a = 0xFF;
	HSD_LObjSetColor(light, light->mColor);

	light->next = NULL;

	light = &lights[2];
	light->Init(6, FALSE);
	light->SetPosition(0.0f, 20.0f, -20.0f);
	light->mColor.r = 0;
	light->mColor.g = 0xFF;
	light->mColor.b = 0xFF;
	light->mColor.a = 0xFF;
	HSD_LObjSetColor(light, light->mColor);

	light->next = NULL;

	light = &lights[3];
	light->Init(7, FALSE);
	light->SetPosition(0.0f, 20.0f, 0.0f);
	light->SetInterest(0.0f, 0.0f, 20.0f);
	light->mColor.r = 0xFF;
	light->mColor.g = 0;
	light->mColor.b = 0;
	light->mColor.a = 0xFF;
	HSD_LObjSetColor(light, light->mColor);
	light->next = NULL;
	return lights;
}

/**
 * @note Address: 0x8001D1D0
 * @note Size: 0xCC
 */
extern "C" CCamera* InitGameOrthoCamera(CCamera* camera, GXRenderModeObj* rmode)
{
	camera->Init(0.0f, 10000.0f, rmode);
#if defined(VERSION_PAL)
	camera->SetFrustum(0.0f, 448.0f, 0.0f, 640.0f);
#endif
	camera->SetScale(1.0f, 1.0f, 1.0f);
	camera->SetEye(0.0f, 0.0f, 5000.0f);
	camera->SetInterest(0.0f, 0.0f, 0.0f);
	camera->CalcAngle();
	camera->CalcDistance();
	return camera;
}

/**
 * @note Address: 0x8001D2D0
 * @note Size: 0xF4
 */
extern "C" CCamera* InitMotionControlCameraDefaults(CCamera* camera)
{
	camera->Init(1.0f, 80000.0f, 30.0f, &gGame.mGs.mScreen);
	camera->SetScale(1.0f, 1.0f, 1.0f);
	camera->SetInterest(-4.2f, 0.0f, -64.0f);
	camera->SetAngle(0.41887903f, -3.1415927f, 0.0f);
	camera->SetDistance(400.0f);
	camera->SetFov(18.0f);
	camera->CalcEye();
	return camera;
}

/**
 * @note Address: 0x8001D3D4
 * @note Size: 0xEC
 */
extern "C" CCamera* InitGamePerspectiveCamera(CCamera* camera, GXRenderModeObj* rmode)
{
	camera->Init(1.0f, 80000.0f, 30.0f, rmode);
	camera->SetScale(1.0f, 1.0f, 1.0f);
	camera->SetInterest(-4.2f, 0.0f, -64.0f);
	camera->SetAngle(0.41887903f, -3.1415927f, 0.0f);
	camera->SetDistance(400.0f);
	camera->SetFov(18.0f);
	camera->CalcEye();
	return camera;
}

// the system setup the game runs before anything else

extern "C" {
void srand(u32 seed);
u32 OSGetTick(void);
void* OSInitAlloc(void* arenaStart, void* arenaEnd, int maxHeaps);
u32 OSGetResetCode(void);
void OSSetSaveRegion(void* start, void* end);
void DVDSetAutoFatalMessaging(BOOL enable);
BOOL DVDChangeDir(char* dirName);
u32 ARInit(u32* stackIndexAddr, u32 numEntries);
void ARQInit(void);
void HSD_VIDrawDoneXFB(s32 idx);
}

/**
 * @note Address: 0x8001D4C0
 * @note Size: 0x2C
 */
extern "C" void InitGameDVD(void)
{
	DVDSetAutoFatalMessaging(TRUE);
	DVDChangeDir((char*)"/qp/");
}

extern "C" u32 lbl_80361E10[8];

/**
 * @note Address: 0x8001D4EC
 * @note Size: 0x30
 */
extern "C" void InitGameARAM(void)
{
	ARInit(lbl_80361E10, 8);
	ARQInit();
}

extern "C" void GameXFBDrawDoneCallback(int idx);

/**
 * @note Address: 0x8001D51C
 * @note Size: 0x384
 */
extern "C" void InitGameRenderState(CBase* base)
{
	CGs* gs = &base->mGs;
#if defined(VERSION_PAL)
	SetGsScreenModeAndRenderDefaults(gs, 21, 0, 0, 0);
	gs->mScreen._40      = gGame.mGs.mScreen.xfbHeight / 448.0f;
	gs->mScreen.viHeight = 480;
#else
	SetGsScreenModeAndRenderDefaults(gs, 4, 0, 16, 0);
#endif
#if !defined(VERSION_JP)
	fn_80196C0C(gs, 1.2f);
#endif
	HSD_SetPixelFormat(1, 0);
	GXSetPixelFmt(1, 0);
	GXSetCopyFilter(0, NULL, 0, NULL);

	InitializeVideoClearState((GXColor) { 0, 0, 0, 0 });
	GXSetClipMode(0);
	HSD_SetZSortMode(1, 1);

	ConfigureGsRenderMode(gs, NULL, (u8) { GX_TRUE }, (u8) { GX_FALSE }, (GsBlendMode) { 1, 4, 5, 15 },
	                      (GsAlphaCompare) { 4, 0, 0, 3, 255 }, (GsDstAlpha) { 0, 0 }, (GsZMode) { 1, 7, 1 }, (u8) { GX_TRUE },
	                      (u8) { GX_TRUE }, (s32) { 2 }, (GsLineWidth) { 12, 5 }, (GsLineWidth) { 24, 5 });
	ConfigureGsPrimaryTevStage(gs);
	ConfigureGsChannel(gs, NULL, (s32) { GX_COLOR0 }, (GsChanCtrl) { GX_DISABLE, GX_SRC_REG, GX_SRC_REG, 0, 2, 2 },
	                   (GXColor) { 0, 0, 0, 255 }, (GXColor) { 255, 255, 255, 255 });
	ConfigureGsChannel(gs, NULL, (s32) { GX_ALPHA0 }, (GsChanCtrl) { GX_DISABLE, GX_SRC_REG, GX_SRC_REG, 0, 0, 2 },
	                   (GXColor) { 0, 0, 0, 255 }, (GXColor) { 255, 255, 255, 255 });
	if (gs->mCurRenderMode)
		ApplyGsRenderMode(gs->mCurRenderMode, TRUE);
	if (gs->mCurTevStage)
		ApplyGsTevStage(gs->mCurTevStage, TRUE);
	if (gs->mCurChannel)
		ApplyGsChannel(gs->mCurChannel, TRUE);
	HSD_VISetUserGXDrawDoneCallback(GameXFBDrawDoneCallback);
}

/**
 * @note Address: 0x8001D8A0
 * @note Size: 0x20
 */
extern "C" void GameXFBDrawDoneCallback(int idx)
{
	HSD_VIDrawDoneXFB(idx);
}

// the two words the game keeps over a reset
extern "C" s32* lbl_8065B178;

/**
 * @note Address: 0x8001D8C0
 * @note Size: 0xE8
 */
extern "C" void fn_8001D8C0(void)
{
	OSInitFastCast();
	srand(OSGetTick());
	void* lo = OSGetArenaLo();
	void* hi = OSGetArenaHi();
	OSSetArenaLo(OSInitAlloc(lo, hi, 1));
	HSD_SetInitParameter(HSD_INIT_FIFO_SIZE, 0x40000);
	HSD_SetInitParameter(HSD_INIT_XFB_MAX_NUM, 3);
#if defined(VERSION_PAL)
	HSD_SetInitParameter(HSD_INIT_RENDER_MODE_OBJ, &GXPal528IntDf);
#endif
	HSD_Init();
	mm::Init();
	lbl_8065B178 = (s32*)OSAllocFromHeap(__OSCurrHeap, 8);
	if (!(OSGetResetCode() >> 31)) {
		lbl_8065B178[0] = 0;
		lbl_8065B178[1] = 1;
	}
	OSSetSaveRegion(lbl_8065B178, lbl_8065B178 + 2);
}

/**
 * @note Address: 0x8001D9A8
 * @note Size: 0x8
 */
// CCameraActionMap's GetTarget
extern "C" XObj* GetTarget__16CCameraActionMapFv(CCameraActionMap* map)
{
	return map->_44;
}

/**
 * @note Address: 0x8001DB44
 * @note Size: 0x4
 */
extern "C" void vf08__7AnimSfxFv(AnimSfx* sfx)
{
}

/**
 * @note Address: 0x8001DB48
 * @note Size: 0x48
 */
// AnimSfx::GetSfx, steps round the event's sfx ids when it has more than one
extern "C" s32 GetSfx__7AnimSfxFP12AnimSfxEvent(AnimSfx* sfx, AnimSfxEvent* event)
{
	if (event->mCount == 0) {
		return event->mSfx;
	}
	event->mIndex++;
	if (event->mCount <= event->mIndex) {
		event->mIndex = 0;
	}
	return event->mSfx + event->mIndex;
}

/**
 * @note Address: 0x8001DB90
 * @note Size: 0x8
 */
// AnimSfx::CheckStop
extern "C" BOOL CheckStop__7AnimSfxFP12AnimSfxEvent(AnimSfx* sfx, AnimSfxEvent* event)
{
	return FALSE;
}

/**
 * @note Address: 0x8001DB98
 * @note Size: 0x78
 */
// AnimSfx::CheckStart, whether the event's start or end frame went by
extern "C" BOOL CheckStart__7AnimSfxFP12AnimSfxEventff(AnimSfx* sfx, AnimSfxEvent* event, f32 min, f32 max)
{
	f32 a = event->mStart;
	f32 b = event->mEnd;

	switch (event->mKind) {
	case 1:
		if (0.0f <= a && min <= a && a < max) {
			return TRUE;
		}
		if (0.0f <= b && min <= b && b < max) {
			return TRUE;
		}
		break;
	}
	return FALSE;
}

/**
 * @note Address: 0x8001DC10
 * @note Size: 0x10
 */
// empties the sound's queue
extern "C" void ClearGameSoundQueue(XSound* sound)
{
	sound->mQueueWrite = 0;
	sound->mQueueRead  = 0;
}

// the sound object

// ids with the high half 0xFFFF but not -1 mean every sfx, see game/sound.cpp
static inline bool IsAllSfx(s32 sfx)
{
	if (sfx == -1) {
		return false;
	}
	return (sfx & 0xFFFF0000) == 0xFFFF0000;
}

// CSound's stop, StopGameSfx in the dol
static inline BOOL StopSfx(XSound* sound, s32 sfx)
{
	if (IsAllSfx(sfx)) {
		sound->_2B4 = 0;
		sound->_2C8 = 0;
		AXDriverStopAllStreams();
		return TRUE;
	}
	return AXDriverKeyOffSfx(sfx);
}

/**
 * @note Address: 0x8001D9B0
 * @note Size: 0x194
 */
// NONMATCHING: the stop's sound fields are addressed off the game object, not the sound
// AnimSfx::Update
extern "C" void Update__7AnimSfxFlfUc(AnimSfx* sfx, s32 anim, f32 frame, u8 vol)
{
	s32 prev = sfx->mAnimId;

	if (prev != anim) {
		sfx->mAnimId = anim;
		sfx->mFrame  = 0.0f;
	}
	AnimSfxEvent* event;
	for (s32 i = 0; sfx->mEvents != NULL && (event = &sfx->mEvents[i])->mAnim >= 0; i++) {
		if (event->mAnim == anim || event->mAnim == prev) {
			if (prev != anim && event->mAnim == prev && sfx->CheckStop(event)) {
				StopSfx(&gGame.mSound, sfx->GetSfx(event));
			}
			if (event->mAnim == anim && sfx->CheckStart(event, sfx->mFrame, frame)) {
				fn_8001E904(&gGame.mSound, sfx->GetSfx(event), vol);
			}
		}
	}
	sfx->mFrame = frame;
}

#if defined(VERSION_JP)
extern "C" void SetSoundMenuSelection(XSound* sound, s32 index, s32 selection)
{
	if (index < 0 || index >= ARRAY_SIZE(gSoundMenuSelections))
		return;
	if (selection < 0 || selection >= ARRAY_SIZE(gSoundMenuEffects))
		return;
	fn_8001E904(sound, gSoundMenuEffects[selection], 0xFF);
	gSoundMenuSelections[index] = selection;
}

extern "C" s32 GetSoundMenuSelection(XSound*, s32 index)
{
	if (index < 0 || index >= ARRAY_SIZE(gSoundMenuSelections))
		return -1;
	return gSoundMenuSelections[index];
}
#endif

#if defined(VERSION_PAL)
f32 gProgressiveFadeFrames = 0.15f * FRAME_RATE;
#endif
