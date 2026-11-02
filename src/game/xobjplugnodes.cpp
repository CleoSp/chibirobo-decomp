#include "game/xobjattachment.h"
#include "game/xchr.h"
#include "game/cgame.h"

/**
 * @note Address: 0x800DBC98
 * @note Size: 0x144
 */
extern "C" void fn_800DBC98(XObjPlugCordNodes* nodes, XObjPlugCordPoint* points, s32 count)
{
	if (nodes->mEnabled) {
		if (nodes->mNode[nodes->mCount - 1].mState == 0) {
			for (u32 i = 0; i < nodes->mCount; ++i) {
				if (nodes->mNode[i].mState == 0) {
					fn_800DC43C(&nodes->mNode[i], points, count, 20.0f, (s32)nodes->mMode);
					break;
				}
			}
		}
	} else {
		nodes->mNode[0].mState = 0;
		nodes->mNode[1].mState = 0;
		nodes->mNode[2].mState = 0;
		nodes->mNode[3].mState = 0;
		nodes->mNode[4].mState = 0;
		nodes->mNode[5].mState = 0;
		nodes->mNode[6].mState = 0;
		nodes->mNode[7].mState = 0;
		nodes->mNode[8].mState = 0;
		nodes->mNode[9].mState = 0;
	}
	XObjPlugCordNode* node = nodes->mNode;
	for (s32 i = 0; i < 10; ++i, ++node) {
		if (node->mState == 1) {
			fn_800DBDDC(node, points, count, 0.0f);
		}
	}
	if (nodes->mNode[0].mState == 1) {
		XObj* object = &lbl_8025C400.GetPlayer()->mPlug.mWire;
		object->SetPos(nodes->mNode[0].mPos, 0);
	}
}
