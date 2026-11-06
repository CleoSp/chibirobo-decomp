extern "C" {
#include <dolphin/os.h>
}
#define DEFER_CAMERA_RECT_SETTERS
#include "game/cgame.h"
#include "game/newgamestate.h"
#include "game/arcache.h"
#include "game/mic.h"
#include "game/xchr.h"
#include "game/xobjs.h"
#include "game/gmath.h"
#include <Dolphin/ai.h>
#include <Dolphin/ar.h>
#include <Dolphin/mic.h>
#include <Dolphin/m2s.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

extern "C" {
void InitGameSound(XSound*);
void InitMemoryCardManager(Memcard*);
s32 GetDefaultStageIndex();
s32 GetStageNum();
void LoadBootConfig();
void ResetProgressiveScanPrompt();
void InitGameMenus(MenuEx*);
void InitInfoScreen(InfoScrn*);
void HSD_JObjSetSPtclCallback(HSD_Generator* (*func)(int, int, int, HSD_JObj*));
void HSD_JObjSetDPtclCallback(HSD_Generator* (*func)(int, int, int, HSD_JObj*));
HSD_Generator* LinkJointSoundGenerator(int, int, int, HSD_JObj*);
HSD_Generator* LinkJointParticleGenerator(int, int, int, HSD_JObj*);
void InitializeEventScriptContext(CEvt*, u8*, void*);
void InitMessageWindow(CMsg*);
}

#include "game/cgameutil.h"
#include <Dolphin/vi.h>
#include <Dolphin/dvd.h>

// The original code also addresses these constants by offsets from the pool base.
#pragma force_active on
extern "C" {
GXColor sOutlineLineColorInitializer   = { 0, 0, 0, 4 };
GXColor sOutlineSecondColorInitializer = { 0, 0, 0, 0x80 };

extern const Vec sGameVectorDefaults[6] = { { 1, 1, 1 }, {}, {}, { 1, 1, 1 }, {}, {} };

#if defined(VERSION_JP)
enum { FirstDiscErrorMessage = 141 };
#else
enum { FirstDiscErrorMessage = 84 };
#endif
extern const s32 sDiscErrorMessages[][2] = {
	{ DVD_STATE_COVER_OPEN, FirstDiscErrorMessage },     { DVD_STATE_NO_DISK, FirstDiscErrorMessage + 1 },
	{ DVD_STATE_WRONG_DISK, FirstDiscErrorMessage + 2 }, { DVD_STATE_RETRY, FirstDiscErrorMessage + 3 },
	{ DVD_STATE_BUSY, FirstDiscErrorMessage + 4 },       { DVD_STATE_END, -1 },
};
extern const Vec sDiscErrorBackdropInitializers[4]       = {};
extern const Vec sGameYAxisVectors[2]                    = { { 0, 10, 0 }, { 0, 3.1415927f, 0 } };
extern const Vec sSubwindowPositionInitializers[10]      = {};
extern const Vec sSubwindowFanInitializers[10]           = {};
extern const float sSubwindowFrameTileInitializers[8][8] = {
	{ 0, 0, .5f, .5f },   { .5f, 0, 0, .5f },   { .5f, 0, .5f, .5f }, { 0, .5f, .5f, 0 },
	{ .5f, .5f, .5f, 0 }, { 0, .5f, .5f, .5f }, { .5f, .5f, 0, .5f }, { .5f, .5f, .5f, .5f },
};
extern const Vec sFrameDepthQuad[4] = {
	{ 0, 0, -1 },
	{ 640, 0, -1 },
	{ 640, 448, -1 },
	{ 0, 448, -1 },
};
extern const Vec sFrameCornerInitializers[4] = {};
extern const Vec sFloorReflectionRotation    = {};
extern const Vec sFloorReflectionTranslation = {};
extern const Vec sFloorReflectionPoint       = {};
extern const Vec sFloorReflectionNormal      = { 0, 1, 0 };
extern const Vec sMirrorScreenQuad[4]        = {
    { 0, 0, 0 },
    { 640, 0, 0 },
    { 640, 448, 0 },
    { 0, 448, 0 },
};
extern const Vec sMicMeterQuad[4] = {
	{ 16, 414, 0 },
	{ 0, 414, 0 },
	{ 0, 419, 0 },
	{ 16, 419, 0 },
};
extern const char sPathGameCpp[]           = "game.cpp";
extern const char sMoviePlaybackError[]    = "error on play movie! - %d\n";
extern const char sBgmMarkerPath[]         = "xxx/bgm1_marker.hps";
extern const char sMirrorAllocationName[]  = "(mirror)";
extern const char sMiscTplPath[]           = "tpl/misc.tpl";
extern const char sPcIndexPath[]           = "/qp/pc.idx";
extern const char sPcBinaryPath[]          = "/qp/pc.bin";
extern const char sChibiRoboParticlePath[] = "particle/chibi_robo";
extern const char sDrawSyncStopwatchName[] = "__$dsync$ ";
extern const char sUpdateStopwatchName[]   = "__$update$";
extern const char sDrawStopwatchName[]     = "__$draw$  ";
}

#pragma force_active reset

#if defined(VERSION_PAL)
#include "game/languagemap.h"
#endif

extern "C" {
void OSInitStopwatch(void* sw, const char* name);
void InitGameRuntime();
void InitGameRenderState(CBase* base);
void InitGameARAM();
void InitGameDVD();
#if defined(VERSION_PAL)
u8 OSGetLanguage();
#endif
}

static inline void StartGameBase(CBase* base)
{
	InitGameRuntime();
	InitGameRenderState(base);
	InitGameARAM();
	InitGameDVD();
#if defined(VERSION_PAL)
	u8 language = OSGetLanguage();
	for (s32 i = 0; i < 6; ++i) {
		if (language == gGameLanguageMap[i].osLanguage) {
			base->mLanguage = gGameLanguageMap[i].gameLanguage;
			break;
		}
	}
#elif !defined(VERSION_JP)
	base->mLanguage = 0;
#endif
#if defined(VERSION_JP)
	Init__5CFontFv(&base->mFont);
#else
	Init__5CFontFi(&base->mFont, gGame.mLanguage != 6);
#endif
	base->mPad.Init();
	base->mLastRetrace   = 0;
	base->mRetraceStep   = 0;
	base->mStatus.mFlags = 0;
	InitGamePerspectiveCamera(&base->mCameras[0], &base->mGs.mScreen);
	InitGameOrthoCamera(&base->mCameras[1], &base->mGs.mScreen);
	InitFourLightSet(&base->mLights[0]);
	InitTwoLightSet(&base->mLights[4]);
	OSInitStopwatch(&base->mStopwatch[0], sDrawSyncStopwatchName);
	OSInitStopwatch(&base->mStopwatch[1], sUpdateStopwatchName);
	OSInitStopwatch(&base->mStopwatch[2], sDrawStopwatchName);
	base->mPrevDvdStatus = 0;
	base->mDvdStatus     = 0;
	base->mDiskOk        = 1;
	base->mDvdError      = 0;
	base->mResetHold     = 0;
	base->mReset         = 0;
}

extern "C" {
void HSD_VISetBlack(u8 black);
void HSD_VICopyXFBAsync(s32 pri);
void OSResetStopwatch(void* sw);
void OSStartStopwatch(void* sw);
void OSStopStopwatch(void* sw);
s64 OSCheckStopwatch(void* sw);
}

/**
 * @note Address: 0x8001552C
 * @note Size: 0x8
 */
extern "C" BOOL BeforeUpdate__5CBaseFv(CBase* base)
{
	return TRUE;
}

/**
 * @note Address: 0x80015534
 * @note Size: 0x100
 */
// CBase's Start
extern "C" void Start__5CBaseFv(CBase* base)
{
	StartGameBase(base);
}

/**
 * @note Address: 0x80015634
 * @note Size: 0x4
 */
extern "C" void Initialize__5CBaseFiPPc(CBase* base, int argc, char** argv)
{
}

/**
 * @note Address: 0x80015638
 * @note Size: 0x8
 */
extern "C" s32 FrameInterval__5CBaseFv(CBase* base)
{
	return 1;
}

/**
 * @note Address: 0x80015640
 * @note Size: 0x4
 */
extern "C" void Draw__5CBaseFv(CBase* base)
{
}

/**
 * @note Address: 0x80015644
 * @note Size: 0x1C8
 */
void main(int argc, char** argv)
{
	((CBase*)&gGame)->Initialize(argc, argv);
	((CBase*)&gGame)->Start();
	HSD_VISetBlack(0);
	while (true) {
		VIWaitForRetrace();
		gGame.mRetraceStep = VIGetRetraceCount() - gGame.mLastRetrace;
		gGame.mLastRetrace = VIGetRetraceCount();
		if (gGame.mRetraceStep > 4) {
			gGame.mRetraceStep = 4;
		}
		OSResetStopwatch(&gGame.mStopwatch[0]);
		OSStartStopwatch(&gGame.mStopwatch[0]);
		OSResetStopwatch(&gGame.mStopwatch[1]);
		OSStartStopwatch(&gGame.mStopwatch[1]);
		OSResetStopwatch(&gGame.mStopwatch[2]);
		OSStartStopwatch(&gGame.mStopwatch[2]);
		HSD_VICopyXFBAsync(0);
		gGame.mStopwatch[0].mElapsed = OSCheckStopwatch(&gGame.mStopwatch[0]);
		OSStopStopwatch(&gGame.mStopwatch[0]);
		gGame.mPad.Update(gGame.mRetraceStep);
		((CBase*)&gGame)->BeforeUpdate();
		((CBase*)&gGame)->Update(gGame.mRetraceStep);
		gGame.mStopwatch[1].mElapsed = OSCheckStopwatch(&gGame.mStopwatch[1]);
		OSStopStopwatch(&gGame.mStopwatch[1]);
		((CBase*)&gGame)->Draw();
		gGame.mStopwatch[2].mElapsed = OSCheckStopwatch(&gGame.mStopwatch[2]);
		OSStopStopwatch(&gGame.mStopwatch[2]);
#if defined(VERSION_PAL)
		s32 interval = ((CBase*)&gGame)->FrameInterval() * 60;
		while (VIGetRetraceCount() - gGame.mLastRetrace < (u32)(interval / gVideoFramesPerSecond - 1)) { }
#else
		s32 interval = ((CBase*)&gGame)->FrameInterval();
		u32 wait     = interval * 60 / 60 - 1;
		while (VIGetRetraceCount() - gGame.mLastRetrace < wait) { }
#endif
	}
}

#pragma push
#pragma opt_common_subs off
#pragma section RX ".text.gamestart"
__declspec(section ".text.gamestart") void CGame::Start()
{
	StartGameBase(this);
	mMotionControlCamera.InitCamera();
	mCameraActions.Init();
	mLightList = mLights;
	InitGameSound(&mSound);
	InitMemoryCardManager(&mMemcard.mCard);
	mMemcardLoaded = FALSE;

	MICInit();
	M2SInit();
	M2SSetBuffer((s16*)OSAllocFromHeap(__OSCurrHeap, 256));
	gMicSamplePhase[0]        = 0;
	gMicSamplePhase[1]        = 0;
	gMicDMABuffers[0]         = (s16*)OSAllocFromHeap(__OSCurrHeap, 640);
	gMicDMABuffers[1]         = (s16*)OSAllocFromHeap(__OSCurrHeap, 640);
	gPreviousAudioDMACallback = AIRegisterDMACallback(OnMicAudioDMA);
	memset(gMicDMABuffers[0], 0, 640);
	DCFlushRange(gMicDMABuffers[0], 640);
	gMicDMABufferIndex = 1;
	AIStartDMA();

	gARAMCacheInfo.mBaseAddress = ARAlloc(GetFreeARAMSize());
	ARGetSize();
	_3338  = 1;
	mStage = GetDefaultStageIndex();
	_344C  = GetStageNum();
	LoadBootConfig();
	mPrevStage = -1;
	ResetProgressiveScanPrompt();

	InitToonTexture(&mToonTexture, 15, 31, 2);
	SetToonTObj(&mToonTexture.mImageDesc);
	mFade.Reset();
	mWipeCircle.Reset();
	mWipeTex.Reset();
	mWipeCustom.Reset();
	mWipeTile0.Reset();
	mWipeTile1.Reset();
	mRoboeyes.Reset();
#if defined(VERSION_PAL)
	mOutline.Init(gGame.mGs.mScreen.fbWidth, gGame.mGs.mScreen.efbHeight);
	mBlur.Init(gGame.mGs.mScreen.fbWidth / 2, gGame.mGs.mScreen.efbHeight / 2);
#else
	mOutline.Init(640, 448);
	mBlur.Init(320, 224);
#endif
	mOutline.SetLineColor(34, 9, 25);
	mOutline.mThick = 2;
	mOutline.SetColor2(100, 100, 200);
	mOutline.mThick2 = -2;

	mSubwin.Init();
	for (int i = 0; i < 5; ++i) {
		FbMirrorEx& mirror = gFramebufferMirrors[i];
		mirror.mJoint      = NULL;
		mirror.mCamera.Init(0.1f, 32768.0f, 0.5f, -0.5f, -0.5f, 0.5f, &gGame.mGs.mScreen);
		mirror.mCamera.SetViewport(0, 256, 0, 256);
		mirror.mCamera.SetScissor(0, 256, 0, 256);
		mirror.Reset();
		mirror.mOwner = NULL;
	}

	ResetLogoFadeState(&mLogo);
	InitTitleScreen(&mTitle);
	InitGameMenus(&mMenu);
	InitInfoScreen(&mInfoScrn);
	mParticle.Init();
	HSD_JObjSetSPtclCallback(LinkJointSoundGenerator);
	HSD_JObjSetDPtclCallback(LinkJointParticleGenerator);
	ResetScreenShake(&mQuake);

	InitializeObjectPools(&mXObjs);
	mDrawLists[0].ClearObjects();
	mDrawLists[1].ClearObjects();
	mDrawLists[2].ClearObjects();

	InitializeEventScriptContext(&mEvt, (u8*)&mCMsgs[0], NULL);
	for (int i = 0; i < 2; ++i)
		InitMessageWindow(&mCMsgs[i]);
	ResetNewGameState();

	StartScreenFade(&mFade, 0, 0, 0, 255, 0, 0, 0, 255, 0.0f);
	StartScreenWipeColorTransition(&mWipeTile1, 0, 0, 0, 0, 0, 0, 0, 320, 224, 640, 448, 640, 448, 0, 0, TRUE);

	mMovie.mMovie.Init();
	mMovieRect[0] = mMovieRect[1] = mMovieRect[2] = mMovieRect[3] = NAN;
}
#pragma pop
