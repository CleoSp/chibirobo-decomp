#pragma section RX ".text.cameraiface"
#define DEFINE_CAMERA_INTERFACE_METHODS
#include "game/cgame.h"

/**
 * @note Address: 0x80015CC8
 * @note Size: 0x64
 */
__declspec(section ".text.cameraiface") BOOL CCameraActionAreaExM::vf78(s32 index)
{
	bool set = CamAreaInfo(&gCameraAreaState._24[index]).IsCameraArea();
	if (set) {
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x80015D74
 * @note Size: 0x64
 */
__declspec(section ".text.cameraiface") BOOL CCameraActionAreaExM::vf7C(s32 index)
{
	bool set = CamAreaInfo(&gCameraAreaState._1C[index]).IsDisabled();
	if (set) {
		return FALSE;
	}
	return TRUE;
}

#define CAMERA_METHOD __declspec(section ".text.cameraiface")
#include "game/camera_interface_methods.inl"
#undef CAMERA_METHOD
