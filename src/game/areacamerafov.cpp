#include "game/ccameraaction.h"

extern "C" {
extern __declspec(section ".sdata2") const f32 sAreaCameraHeight0FovScale;
extern __declspec(section ".sdata2") const f32 sAreaCameraHeight1FovScale;
extern __declspec(section ".sdata2") const f32 sAreaCameraHeight2FovScale;
extern __declspec(section ".sdata2") const f32 sAreaCameraHeight3FovScale;
}

/**
 * @note Address: 0x801353E4
 * @note Size: 0x50
 */
extern "C" f32 GetAreaCameraFovForHeightLevel(CCameraActionAreaEx* state, s32 level)
{
	f32 scale;
	switch (level) {
	case 0:
	default:
		scale = sAreaCameraHeight0FovScale;
		break;
	case 1:
		scale = sAreaCameraHeight1FovScale;
		break;
	case 2:
		scale = sAreaCameraHeight2FovScale;
		break;
	case 3:
		scale = sAreaCameraHeight3FovScale;
		break;
	}
	return scale * state->mBaseFov;
}

extern "C" {
extern __declspec(section ".sdata2") const f32 sAreaCameraHeight0FovScale = 0.8f;
extern __declspec(section ".sdata2") const f32 sAreaCameraHeight1FovScale = 1.0f;
extern __declspec(section ".sdata2") const f32 sAreaCameraHeight2FovScale = 1.5f;
extern __declspec(section ".sdata2") const f32 sAreaCameraHeight3FovScale = 2.0f;
}
