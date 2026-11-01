#include "game/xobjattachment.h"
#include "game/cgame.h"
#include "game/cgs.h"
#include "game/ccamera.h"
extern "C" {
#include <sysdolphin/mtx_transform.h>
#include <dolphin/mtx/mtx.h>
}
#include <math.h>

// draws the nodes, each one half as bright as the one before
/**
 * @note Address: 0x800DA4D0
 * @note Size: 0x1BC
 */
extern "C" void fn_800DA4D0(XObjPlugCord* cord, Mtx view, f32 scaleX, f32 scaleY, f32 scaleZ)
{
	f32(*selectedView)[4];
	if (view == NULL)
		selectedView = ((CGs*)lbl_8025C400.mGs)->mCamera->mViewMtx;
	else
		selectedView = view;
	fn_80199AB0((CGs*)lbl_8025C400.mGs, 0, 0);
	fn_8019B56C(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 4, 5, 15);
	fn_8019B464(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 3, 0);
	if (cord->mNodes.mEnabled == 1) {
		for (s32 i = 0; (u32)i < cord->mNodes.mCount; ++i) {
			XObjPlugCordNode* node = &cord->mNodes.mNode[i];
			if (node->mState != 0) {
				Mtx local, combined;
				Vec rotation = CVec(0.0f, 0.0f, 0.0f);
				Vec scale    = CVec(0.7f, 0.7f, 0.7f);
				HSD_MtxSRT(local, &scale, &rotation, &node->mPos, NULL);
				PSMTXConcat(selectedView, local, combined);
				s32 alpha = (s32)(300.0 / pow(2.0, (f64)i));
				u32 color = (cord->mNodes.mColor & 0xFFFFFF00) | alpha;
				fn_801970B0((CGs*)lbl_8025C400.mGs, combined, color, 8, 8);
			}
		}
	}
}
