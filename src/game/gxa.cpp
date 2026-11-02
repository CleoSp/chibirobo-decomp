#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/mm.h"
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
extern "C" CLight* fn_8001CEFC(CLight* lights)
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
extern "C" CLight* fn_8001CFD0(CLight* lights)
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
extern "C" CCamera* fn_8001D1D0(CCamera* camera, GXRenderModeObj* rmode)
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
extern "C" CCamera* fn_8001D2D0(CCamera* camera)
{
	camera->Init(1.0f, 80000.0f, 30.0f, &lbl_8025C400.mGs.mScreen);
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
extern "C" CCamera* fn_8001D3D4(CCamera* camera, GXRenderModeObj* rmode)
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
extern "C" void fn_8001D4C0(void)
{
	DVDSetAutoFatalMessaging(TRUE);
	DVDChangeDir((char*)"/qp/");
}

extern "C" u32 lbl_80361E10[8];

/**
 * @note Address: 0x8001D4EC
 * @note Size: 0x30
 */
extern "C" void fn_8001D4EC(void)
{
	ARInit(lbl_80361E10, 8);
	ARQInit();
}

/**
 * @note Address: 0x8001D8A0
 * @note Size: 0x20
 */
extern "C" void fn_8001D8A0(s32 idx)
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
extern "C" XObj* fn_8001D9A8(CCameraActionMap* map)
{
	return map->_44;
}

/**
 * @note Address: 0x8001DB44
 * @note Size: 0x4
 */
extern "C" void fn_8001DB44(AnimSfx* sfx)
{
}

/**
 * @note Address: 0x8001DB48
 * @note Size: 0x48
 */
// AnimSfx::GetSfx, steps round the event's sfx ids when it has more than one
extern "C" s32 fn_8001DB48(AnimSfx* sfx, AnimSfxEvent* event)
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
extern "C" BOOL fn_8001DB90(AnimSfx* sfx, AnimSfxEvent* event)
{
	return FALSE;
}

/**
 * @note Address: 0x8001DB98
 * @note Size: 0x78
 */
// AnimSfx::CheckStart, whether the event's start or end frame went by
extern "C" BOOL fn_8001DB98(AnimSfx* sfx, AnimSfxEvent* event, f32 min, f32 max)
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
extern "C" void fn_8001DC10(XSound* sound)
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

// CSound's stop, fn_8001E660 in the dol
static inline BOOL StopSfx(XSound* sound, s32 sfx)
{
	if (IsAllSfx(sfx)) {
		sound->_2B4 = 0;
		sound->_2C8 = 0;
		fn_801E9FA4();
		return TRUE;
	}
	return fn_801E62A8(sfx);
}

/**
 * @note Address: 0x8001D9B0
 * @note Size: 0x194
 */
// NONMATCHING: the stop's sound fields are addressed off the game object, not the sound
// AnimSfx::Update
extern "C" void fn_8001D9B0(AnimSfx* sfx, s32 anim, f32 frame, u8 vol)
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
				StopSfx(&lbl_8025C400.mSound, sfx->GetSfx(event));
			}
			if (event->mAnim == anim && sfx->CheckStart(event, sfx->mFrame, frame)) {
				fn_8001E904(&lbl_8025C400.mSound, sfx->GetSfx(event), vol);
			}
		}
	}
	sfx->mFrame = frame;
}
