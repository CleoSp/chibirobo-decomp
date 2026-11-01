#include "game/ccamera.h"
#include "game/clight.h"
#include "game/cgame.h"
#include <dolphin/ar.h>
#include <dolphin/dvd.h>
#include "game/gmath.h"

extern "C" {
// the aram block list, ARInit's
extern u32 lbl_80361E10[8];
int DVDSetAutoFatalMessaging(BOOL enable);
BOOL DVDChangeDir(const char* directory);
}

// todo: cleanup
static inline void SetLightColor(CLight* light, u8 r, u8 g, u8 b, u8 a)
{
	light->mColor.r = r;
	light->mColor.g = g;
	light->mColor.b = b;
	light->mColor.a = a;
	HSD_LObjSetColor(light, light->mColor);
}

/**
 * @note Address: 0x8001CEFC
 * @note Size: 0xD4
 */
extern "C" CLight* fn_8001CEFC(CLight* lights)
{
	lights[0].Init(4, FALSE);
	SetLightColor(&lights[0], 128, 128, 128, 255);
	lights[0].next = &lights[1];
	CLight* light  = &lights[1];
	light->Init(5, FALSE);
	light->SetPosition(1.0f, -1.0f, 1.0f);
	SetLightColor(light, 255, 255, 255, 255);
	light->next = NULL;
	return lights;
}

/**
 * @note Address: 0x8001CFD0
 * @note Size: 0x1D0
 */
extern "C" CLight* fn_8001CFD0(CLight* lights)
{
	lights[0].Init(4, FALSE);
	SetLightColor(&lights[0], 200, 200, 200, 255);
	lights[0].next = &lights[1];
	CLight* light  = &lights[1];
	light->Init(5, TRUE);
	light->SetPosition(-2.0f, 20.0f, 2.0f);
	SetLightColor(light, 200, 200, 200, 255);
	light->next = NULL;
	light       = &lights[2];
	light->Init(6, FALSE);
	light->SetPosition(0.0f, 20.0f, -20.0f);
	SetLightColor(light, 0, 255, 255, 255);
	light->next = NULL;
	light       = &lights[3];
	light->Init(7, FALSE);
	light->SetPosition(0.0f, 20.0f, 0.0f);
	light->SetInterest(0.0f, 0.0f, 20.0f);
	SetLightColor(light, 255, 0, 0, 255);
	light->next = NULL;
	return lights;
}

/**
 * @note Address: 0x8001D1D0
 * @note Size: 0xCC
 */
extern "C" CCamera* fn_8001D1D0(CCamera* camera, GXRenderModeObj* mode)
{
	camera->Init(0.0f, 10000.0f, mode);
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
	camera->Init(1.0f, 80000.0f, 30.0f, (GXRenderModeObj*)lbl_8025C400.mGs);
	camera->SetScale(1.0f, 1.0f, 1.0f);
	camera->SetInterest(-4.2f, 0.0f, -64.0f);
	camera->SetAngle(0.41887903f, -PI, 0.0f);
	camera->SetDistance(400.0f);
	camera->SetFov(18.0f);
	camera->CalcEye();
	return camera;
}

/**
 * @note Address: 0x8001D3D4
 * @note Size: 0xEC
 */
extern "C" CCamera* fn_8001D3D4(CCamera* camera, GXRenderModeObj* mode)
{
	camera->Init(1.0f, 80000.0f, 30.0f, mode);
	camera->SetScale(1.0f, 1.0f, 1.0f);
	camera->SetInterest(-4.2f, 0.0f, -64.0f);
	camera->SetAngle(0.41887903f, -PI, 0.0f);
	camera->SetDistance(400.0f);
	camera->SetFov(18.0f);
	camera->CalcEye();
	return camera;
}

/**
 * @note Address: 0x8001D4C0
 * @note Size: 0x2C
 */
extern "C" void fn_8001D4C0()
{
	DVDSetAutoFatalMessaging(TRUE);
	DVDChangeDir("/qp/");
}

/**
 * @note Address: 0x8001D4EC
 * @note Size: 0x30
 */
extern "C" void fn_8001D4EC()
{
	ARInit(lbl_80361E10, 8);
	ARQInit();
}
