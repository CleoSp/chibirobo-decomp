#include "game/cgame.h"
#include "game/cgs.h"
#include "game/xobjs.h"
#include "game/stagedata.h"

// todo: cleanup
static inline s16 ObjectPool(XObj* object)
{
	UnkObjData* data = object->mAttr._00;
	return data != NULL ? GetObjDataPool(data) : -1;
}

// todo: cleanup
static inline void InvalidateGraphics(CGs* graphics)
{
	HSD_StateInvalidate(-1);
	if (((CGs*)lbl_8025C400.mGs)->mCurRenderMode != NULL) {
		graphics->mCurRenderMode->Invalidate();
	}
	CGsTevStage* stage = graphics->mCurTevStage;
	if (stage != NULL)
		stage->Invalidate();
	CGsChannel* channel = graphics->mCurChannel;
	if (channel != NULL && !channel->mDirty)
		channel->mDirty = TRUE;
}

/**
 * @note Address: 0x8000CDA0
 * @note Size: 0x28C
 */
// CGame's vf20, draws the objects' second pass, pool 3 first
extern "C" void fn_8000CDA0(CGame* game)
{
	if (game->mStatus.mFlags & 1ULL) {
		((CGs*)game->mGs)->mCamera = game->GetCamera();
		if (fn_8019997C(game) != NULL) {
			for (s32 i = 0; i < game->mDrawLists[1].mCount; ++i) {
				XObj* object = game->mDrawLists[1].mObjects[i];
				if (ObjectPool(object) == 3)
					object->vfAC();
			}
			fn_80199944((CGs*)game);
		}
		((CGs*)game->mGs)->mCamera = game->GetCamera();
		if (fn_8019997C(game) != NULL) {
			for (s32 i = 0; i < game->mDrawLists[1].mCount; ++i) {
				XObj* object = game->mDrawLists[1].mObjects[i];
				if (ObjectPool(object) != 3)
					object->vfAC();
			}
			fn_8006F338(2, 0, NULL);
			fn_80199944((CGs*)game);
		}
		InvalidateGraphics((CGs*)lbl_8025C400.mGs);
	}
}
