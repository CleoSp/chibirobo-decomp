#pragma section RX ".text.initialscene"
#define SCENE_INIT __declspec(section ".text.initialscene")

#include "game/cgame.h"

// the light sets and cameras the scenes start from. each one sets up the objects it gets and
// hands them back

/**
 * @note Address: 0x8001CEFC
 * @note Size: 0xD4
 */
extern "C" SCENE_INIT CLight* InitTwoLightSet(CLight* lights)
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
extern "C" SCENE_INIT CLight* InitFourLightSet(CLight* lights)
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

SCENE_INIT void CLight::SetInterest(f32 x, f32 y, f32 z)
{
	mInterest.x = x;
	mInterest.y = y;
	mInterest.z = z;
	HSD_LObjSetInterest(this, &mInterest);
}

/**
 * @note Address: 0x8001D1D0
 * @note Size: 0xCC
 */
extern "C" SCENE_INIT CCamera* InitGameOrthoCamera(CCamera* camera, GXRenderModeObj* rmode)
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

SCENE_INIT void CCamera::SetScale(f32 x, f32 y, f32 z)
{
	mScale.x = x;
	mScale.y = y;
	mScale.z = z;
	PSMTXScale(mScaleMtx, x, y, z);
}

/**
 * @note Address: 0x8001D2D0
 * @note Size: 0xF4
 */
extern "C" SCENE_INIT CCamera* InitMotionControlCameraDefaults(CCamera* camera)
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

SCENE_INIT void CCamera::SetAngle(f32 x, f32 y, f32 z)
{
	mAngle.x = x;
	mAngle.y = y;
	mAngle.z = z;
}

/**
 * @note Address: 0x8001D3D4
 * @note Size: 0xEC
 */
extern "C" SCENE_INIT CCamera* InitGamePerspectiveCamera(CCamera* camera, GXRenderModeObj* rmode)
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
