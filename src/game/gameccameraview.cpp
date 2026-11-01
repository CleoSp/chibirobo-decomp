#include "game/hsd_jobj.h"
#include "game/ccamera.h"

/**
 * @note Address: 0x8001353C
 * @note Size: 0x20
 */
// todo: weak copy
extern "C" void SetScissor__7CCameraFlUslUs(CCamera* camera, s32 left, u16 right, s32 top, u16 bottom)
{
	HSD_CObjSetScissorx4(camera, left, right, top, bottom);
}

/**
 * @note Address: 0x8001355C
 * @note Size: 0x20
 */
// todo: weak copy
extern "C" void SetViewport__7CCameraFffff(CCamera* camera, f32 left, f32 right, f32 top, f32 bottom)
{
	HSD_CObjSetViewportfx4(camera, left, right, top, bottom);
}
