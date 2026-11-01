#include "game/xobjattachment.h"
#include "game/cgs.h"
#include "game/cgame.h"
#include "game/ccamera.h"

/**
 * @note Address: 0x800D80C8
 * @note Size: 0xC
 */
extern "C" void fn_800D80C8(XObjPlug* object)
{
	object->mWire._990 = -1;
}

/**
 * @note Address: 0x800D80D4
 * @note Size: 0x10
 */
extern "C" void fn_800D80D4(XObjPlug* object, s32 value)
{
	object->mWire._990 = value;
	object->mWire._994 = -1;
}

// XObjPlug::vfDC
/**
 * @note Address: 0x800D80E4
 * @note Size: 0x54
 */
extern "C" void fn_800D80E4(XObjPlug* object, BOOL enabled, u32 value)
{
	fn_80091834(object, enabled, value);
	fn_800DB7A8(&object->mCord, enabled, value);
}

// XObjPlug::vfC0
/**
 * @note Address: 0x800D8138
 * @note Size: 0x4C
 */
extern "C" void fn_800D8138(XObjPlug* object)
{
	if (object->mJObj != NULL) {
		fn_800DA4D0(&object->mCord, ((CGs*)lbl_8025C400.mGs)->mCamera->mViewMtx, 1.0f, 1.0f, 1.0f);
	}
}

// todo: cleanup
static inline void InvalidateGraphics(CGs* graphics)
{
	CGs* current = (CGs*)lbl_8025C400.mGs;
	HSD_StateInvalidate(-1);
	if (graphics->mCurRenderMode != NULL)
		current->mCurRenderMode->Invalidate();
	CGsTevStage* stage = current->mCurTevStage;
	if (stage != NULL)
		stage->Invalidate();
	CGsChannel* channel = current->mCurChannel;
	if (channel != NULL && !channel->mDirty)
		channel->mDirty = TRUE;
}

// XObjPlug::vfB8
/**
 * @note Address: 0x800D8184
 * @note Size: 0x1A8
 */
extern "C" void fn_800D8184(XObjPlug* object)
{
	object->mCord.mColor = 0x14141440;
	object->mCord._60    = 0x14141440;
	object->mCord.mWidth = 0.15f;
	if (object->mJObj != NULL) {
		CGs* graphics       = (CGs*)lbl_8025C400.mGs;
		CGsRenderMode* mode = graphics->mCurRenderMode;
		if (mode != NULL)
			fn_8019B278(mode, TRUE);
		CGsTevStage* stage = graphics->mCurTevStage;
		if (stage != NULL)
			fn_8019AED0(stage, TRUE);
		CGsChannel* channel = graphics->mCurChannel;
		if (channel != NULL)
			fn_8019AE94(channel, TRUE);
		fn_800DA68C(&object->mCord, NULL, 1.0f, 1.0f, 1.0f);
		InvalidateGraphics(graphics);
	}
	object->mCord.mColor = 0x141414FF;
	object->mCord._60    = 0x141414FF;
	object->mCord.mWidth = 0.2f;
}

// XObjPlug::vfB4
/**
 * @note Address: 0x800D832C
 * @note Size: 0x2C
 */
extern "C" void fn_800D832C(XObjPlug* object, Mtx view)
{
	object->vfB0(view);
}

// XObjPlug::vfB0
/**
 * @note Address: 0x800D8358
 * @note Size: 0x74
 */
extern "C" void fn_800D8358(XObjPlug* object, Mtx view)
{
	fn_800908C0(object, view);
	object->mWire.vfB0(view);
	if (object->mJObj != NULL)
		fn_800DA68C(&object->mCord, view, 1.0f, 1.0f, 1.0f);
}
