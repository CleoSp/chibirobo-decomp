#include "game/fbtexeff.h"
#include "game/cgame.h"
#include "game/mm.h"

extern "C" u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, u8 mipmap, u8 maxLod);

/**
 * @note Address: 0x80014978
 * @note Size: 0xEC
 */
// FbSubwin's Init, a quarter size window
extern "C" void fn_80014978(FbSubwin* subwin)
{
#if defined(VERSION_PAL)
	GXRenderModeObj* mode = (GXRenderModeObj*)lbl_8025C400.mGs;
	u16 width             = mode->fbWidth / 2;
	u16 height            = mode->efbHeight / 2;
#else
	u16 width  = 320;
	u16 height = 224;
#endif
	FbTexture* texture = &subwin->mTexture;
	texture->mWidth    = width;
	texture->mHeight   = height;
	texture->mFormat   = GX_TF_RGB565;
	texture->_0C       = 0;
	texture->mX        = 0.0f;
	texture->mY        = 0.0f;
	texture->mMipmap   = FALSE;
	texture->mImage    = mm::Alloc(GXGetTexBufferSize(width, height, GX_TF_RGB565, FALSE, 0), "(subwin)");
	subwin->mActive    = TRUE;
	texture->mTex.Init(texture->mImage, texture->mWidth, texture->mHeight, texture->mFormat, GX_CLAMP, GX_CLAMP, FALSE);
	fn_8001D3D4(&subwin->mCamera, (GXRenderModeObj*)lbl_8025C400.mGs);
#if defined(VERSION_PAL)
	HSD_RectS16 viewport = { 0, 0, 0, 0 };
	viewport.xmax        = width;
	viewport.ymax        = height;
#else
	HSD_RectS16 viewport = { 0, 320, 0, 224 };
#endif
	HSD_CObjSetViewport(&subwin->mCamera, &viewport);
	subwin->Reset();
}
