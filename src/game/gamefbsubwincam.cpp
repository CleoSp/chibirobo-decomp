#include "game/fbtexeff.h"

/**
 * @note Address: 0x800136B4
 * @note Size: 0xD0
 */
// points the window's camera at a spot from an angle and distance
extern "C" void fn_800136B4(FbSubwin* subwin, const Vec& interest, f32 angleY, f32 angleX, f32 distance, f32 fov)
{
	subwin->mCamera.SetInterest(interest);
	subwin->mCamera.SetAngleY(angleY);
	subwin->mCamera.SetAngleX(angleX);
	subwin->mCamera.SetDistance(distance);
	subwin->mCamera.SetFov(fov);
	subwin->mCamera.CalcEye();
}
