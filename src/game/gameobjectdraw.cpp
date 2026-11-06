#include "game/cgame.h"
#include "game/xobjmap.h"
#include "game/hsd_dobj.h"
#include "game/cgameutil.h"

/**
 * @note Address: 0x8000CB4C
 * @note Size: 0x74
 */
extern "C" void ExpandShadowBounds__4XObjFP7CShadow(XObj* obj, CShadow* shadow)
{
	f32 size = obj->GetShadowRadius();
	Vec pos  = obj->mMoveTranslate;
	shadow->AddRect(&pos, size, -size, -size, size);
}

/**
 * @note Address: 0x8000CBC0
 * @note Size: 0x8
 */
extern "C" f32 GetShadowRadius__4XObjFv(void)
{
	return 40.0f;
}

/**
 * @note Address: 0x8000CBC8
 * @note Size: 0x2C
 */
extern "C" void RegisterShadowObjects__4XObjFP7CShadow(CJObj* obj, CShadow* shadow)
{
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)obj->mJObj);
}

/**
 * @note Address: 0x8000CBF4
 * @note Size: 0x1A8
 */
// a joint walk: finds the highest joint at or under the limit in limits[0] and keeps it in
// limits[1], nan to start, and marks the materials of those joints
extern "C" void FindHighestJointBelowHeightLimit(HSD_JObj* jobj, f32* limits)
{
	Vec pos;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(jobj), &pos);
	BOOL under = pos.y <= limits[0];
	if (under) {
		if (fpclassify(limits[1]) == FP_NAN) {
			limits[1] = pos.y;
		} else if (limits[1] < pos.y) {
			limits[1] = pos.y;
		}
	}
	for (HSD_DObj* dobj = HSD_JObjGetDObj(jobj); dobj != NULL; dobj = HSD_DObjGetNext(dobj)) {
		HSD_MObj* mobj = HSD_DObjGetMObj(dobj);
		if (mobj != NULL) {
			if (under) {
				HSD_MObjSetFlags(mobj, 0x4000000);
			} else {
				HSD_MObjClearFlags(mobj, 0x4000000);
			}
		}
	}
}

/**
 * @note Address: 0x8000CD9C
 * @note Size: 0x4
 */
extern "C" void DrawObjectEffects__4XObjFv(void)
{
}

/**
 * @note Address: 0x8000CDA0
 * @note Size: 0x28C
 */
// CGame::vf20, draws the objects in _C074 from the main camera, kind 3 ones first
extern "C" void vf20__5CGameFv(CGame* game)
{
	if (game->mStatus.mFlags & 1) {
		s32 i;
		game->mGs.mCamera = game->GetCamera();
		if (BeginGsCamera(game)) {
			for (i = 0; i < game->mDrawLists[1].mCount; i++) {
				if (game->mDrawLists[1].mObjects[i]->GetCategory() == 3) {
					game->mDrawLists[1].mObjects[i]->vfAC();
				}
			}
			EndGsCamera(&game->mGs);
		}
		game->mGs.mCamera = game->GetCamera();
		if (BeginGsCamera(game)) {
			for (i = 0; i < game->mDrawLists[1].mCount; i++) {
				if (game->mDrawLists[1].mObjects[i]->GetCategory() != 3) {
					game->mDrawLists[1].mObjects[i]->vfAC();
				}
			}
			CallStageModule(2, 0, NULL);
			EndGsCamera(&game->mGs);
		}
		InvalidateAll();
	}
}

/**
 * @note Address: 0x8000D02C
 * @note Size: 0x30
 */
extern "C" void vfAC__4XObjFv(XObj* obj)
{
	obj->DrawWithView(0);
}

/**
 * @note Address: 0x8000D05C
 * @note Size: 0x2C
 */
extern "C" CCamera* GetCamera__5CBaseFv(CBase* base)
{
	return base->vf30();
}

/**
 * @note Address: 0x8000D088
 * @note Size: 0x88
 */
extern "C" f32 GetCollisionHeight__4XObjFv(XObj* obj)
{
	if (fpclassify(obj->mMaxFloorHeight) == FP_NAN) {
		return 0.0f;
	}
	return obj->mMaxFloorHeight - obj->mMoveTranslate.y;
}

/**
 * @note Address: 0x8000D110
 * @note Size: 0x8
 */
extern "C" f32 GetFloorXZExtent__4XObjFv(XObj* obj)
{
	return obj->mFloorXZExtent;
}
