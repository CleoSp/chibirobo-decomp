#include "game/hsd_jobj.h"
#include "game/fbtexeff.h"
#include "game/cgame.h"
#include "game/cgs.h"

/**
 * @note Address: 0x8001BC34
 * @note Size: 0xC8
 */
// FbMirrorEx::Init, a 256 by 256 view
extern "C" void fn_8001BC34(FbMirrorEx* mirror)
{
	mirror->mJoint = NULL;
	mirror->mCamera.Init(0.1f, 32768.0f, 0.5f, -0.5f, -0.5f, 0.5f, &((CGs*)lbl_8025C400.mGs)->mScreen);
	mirror->mCamera.SetViewport(0.0f, 256.0f, 0.0f, 256.0f);
	mirror->mCamera.SetScissor(0, 256, 0, 256);
	mirror->Reset();
	mirror->mOwner = NULL;
}
