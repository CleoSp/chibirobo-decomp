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
extern "C" void DrawPlugCordNodes(XObjPlugCord* cord, Mtx view, f32 scaleX, f32 scaleY, f32 scaleZ)
{
	f32(*selectedView)[4];
	if (view == NULL)
		selectedView = gGame.mGs.mCamera->mViewMtx;
	else
		selectedView = view;
	SetGsChannelEnabled(&gGame.mGs, 0, 0);
	SetGsBlendMode(gGame.mGs.mCurRenderMode, 1, 4, 5, 15);
	SetGsDepthMode(gGame.mGs.mCurRenderMode, 1, 3, 0);
	if (cord->mNodes.mEnabled == 1) {
		XObjPlugCordNode* node = cord->mNodes.mNode;
		for (s32 i = 0; (u32)i < cord->mNodes.mCount; ++i, ++node) {
			if (cord->mNodes.mNode[i].mState != 0) {
				Mtx combined;
				Vec rotation = CVec(0.0f, 0.0f, 0.0f);
				Vec scale    = CVec(0.7f, 0.7f, 0.7f);
				Mtx local;
				HSD_MtxSRT(local, &scale, &rotation, &node->mPos, NULL);
				PSMTXConcat(selectedView, local, combined);
				s32 alpha = (s32)(300.0 / pow(2.0, (f64)i));
				u32 color = (cord->mNodes.mColor & 0xFFFFFF00) | alpha;
				DrawGsSphereWithMatrix(&gGame.mGs, combined, color, 8, 8);
			}
		}
	}
}
