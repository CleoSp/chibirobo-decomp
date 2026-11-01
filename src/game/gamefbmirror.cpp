#include "game/fbtexeff.h"
#include "game/texturecopy.h"

/**
 * @note Address: 0x80014A64
 * @note Size: 0x144
 */
// FbMirror's Draw, sets up gx for drawing the mirror's texture
extern "C" void fn_80014A64(FbMirror* mirror)
{
	if (mirror->mJoint) {
		CGs* graphics = CaptureGraphics();
		if (graphics->mCurRenderMode)
			fn_8019B278(graphics->mCurRenderMode, TRUE);
		if (graphics->mCurTevStage)
			fn_8019AED0(graphics->mCurTevStage, TRUE);
		if (graphics->mCurChannel)
			fn_8019AE94(graphics->mCurChannel, TRUE);
		fn_80199AB0((CGs*)lbl_8025C400.mGs, 4, 0);
		fn_8019B56C(graphics->mCurRenderMode, 0, 4, 5, 15);
		fn_8019B504(graphics->mCurRenderMode, 7, 0, 0, 7, 255);
		fn_8019B464(graphics->mCurRenderMode, 1, 3, 1);
		fn_8019B648(graphics->mCurRenderMode, 1);
		fn_8019B5D0(graphics->mCurRenderMode, 0);
		CGs* global = CaptureGraphics();
		fn_8019AFA0(global->mCurTevStage, 0);
		fn_8019B360(graphics->mCurRenderMode, 1);
		fn_8019B360(graphics->mCurRenderMode, 2);
		fn_8019AFA0(global->mCurTevStage, 4);
		fn_8019B504(graphics->mCurRenderMode, 4, 0, 0, 3, 255);
	}
}

/**
 * @note Address: 0x80014BA8
 * @note Size: 0x1FC
 */
// FbMirror's Copy
extern "C" void fn_80014BA8(FbMirror* mirror)
{
	if (mirror->mVisible) {
		CGs* graphics = CaptureGraphics();
		fn_8019B648(graphics->mCurRenderMode, 1);
		fn_8019B5D0(graphics->mCurRenderMode, 1);
		fn_8019B464(graphics->mCurRenderMode, 0, 7, 1);
		FlushCaptureGraphics(graphics);
		CopyCaptureTexture(&mirror->mTexture, graphics);
	}
}
