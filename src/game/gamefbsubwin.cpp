#include "game/fbtexeff.h"
#include "game/texturecopy.h"

/**
 * @note Address: 0x800146D0
 * @note Size: 0x1F0
 */
// FbSubwin's Copy
extern "C" void Copy__8FbSubwinFv(FbSubwin* subwin)
{
	CGs* graphics = CaptureGraphics();
	fn_8019B648(graphics->mCurRenderMode, 1);
	fn_8019B5D0(graphics->mCurRenderMode, 1);
	fn_8019B464(graphics->mCurRenderMode, 0, 7, 1);
	FlushCaptureGraphics(graphics);
	CopyCaptureTexture(&subwin->mTexture, graphics);
}
