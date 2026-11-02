#include "game/cgame.h"
#include "game/xobjmap.h"
#include "game/hsd_dobj.h"
#include "game/cgameutil.h"

extern "C" {
void GXSetTexCopySrc(u16 left, u16 top, u16 wd, u16 ht);
void GXSetTexCopyDst(u16 wd, u16 ht, GXTexFmt fmt, GXBool mipmap);
void GXCopyTex(void* dest, GXBool clear);
void JObjClearMObjFlags(HSD_JObj* jobj, u32 flags);
void fn_8000CBF4(HSD_JObj* jobj, f32* limits);
}

// an object that is drawn into the shadow, or onto it with the mask
static inline bool IsShadowObj(XObj* obj, u64 mask)
{
	bool on = false;
	if ((obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 0x10000) && (obj->mStatus.mFlags & mask)) {
		on = true;
	}
	return on;
}

/**
 * @note Address: 0x8000C450
 * @note Size: 0x6FC
 */
// NONMATCHING: the shadow object and the first loop's registers rotate
// renders the shadow of the first light that casts one, from above the main camera's
// interest. the player's shadow object gets the height of its highest joint under the player
extern "C" void fn_8000C450(CGame* game)
{
	UnkPlayer* player;
	XObj* target;
	s32 i;
	XObj* obj;
	if (game->mStatus.mFlags & 1) {
		player        = (UnkPlayer*)lbl_8025C400.mXObjs.mPools[2].First();
		target        = player->GetShadowObj();
		f32 limits[2] = { 1.0f + player->mMoveTranslate.y, NAN };
		for (i = 0; i < game->mDrawLists[1].mCount; i++) {
			obj = game->mDrawLists[1].mObjects[i];
			if (obj->mStatus.mFlags & 2) {
				if (obj == target) {
					limits[1] = NAN;
					if (obj->mJObj != NULL) {
						HSD_JObjWalkTree(obj->mJObj, (HSD_JObjWalkTreeCallback)fn_8000CBF4, (f32**)limits);
					}
				} else if (obj->mJObj != NULL) {
					HSD_JObjWalkTree(obj->mJObj, (HSD_JObjWalkTreeCallback)JObjClearMObjFlags, (f32**)0x4000000);
				}
			}
		}

		for (CLight* light = game->mLightList; light != NULL; light = (CLight*)light->next) {
			CShadow* shadow = &light->mShadow;
			if (shadow->IsActive()) {
				HSD_CObj* cobj = shadow->mShadow->camera;
				Vec interest   = game->GetCamera()->mInterest;
				Vec eye;
				eye.x = interest.x;
				eye.z = interest.z;
				eye.y = 200.0f + interest.y;
				HSD_CObjSetInterest(cobj, &interest);
				HSD_CObjSetEyePosition(cobj, &eye);
				HSD_ShadowDeleteObject(shadow->mShadow, NULL);
				shadow->InitRect();
				for (s32 i = 0; i < game->mDrawLists[1].mCount; i++) {
					XObj* obj = game->mDrawLists[1].mObjects[i];
					if (IsShadowObj(obj, 4)) {
						obj->vfD0(shadow);
					}
					if ((GetStageInfo()->mFlags & 1) && obj->GetCategory() == 2) {
						obj->vfD4(shadow);
					}
				}
				Vec center = game->GetCamera()->mInterest;
				shadow->AddRect(&center, 100.0f, -100.0f, -100.0f, 100.0f);
				game->mGs.mCamera = shadow->StartRender();
				if (fn_8019997C(game)) {
					fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 3, 0);
					fn_80199AB0(&lbl_8025C400.mGs, 0, 0);
					fn_8019B360(lbl_8025C400.mGs.mCurRenderMode, 2);
					SetupAll(TRUE);
					for (s32 i = 0; i < game->mDrawLists[1].mCount; i++) {
						XObj* obj = game->mDrawLists[1].mObjects[i];
						if (IsShadowObj(obj, 8)) {
							f32 alpha;
							if (GetStageInfo()->mFlags & 1) {
								alpha = 1.0f;
							} else {
								CCamera* camera = game->GetCamera();
								f32 dz          = camera->mInterest.z - obj->mMoveTranslate.z;
								f32 dx          = camera->mInterest.x - obj->mMoveTranslate.x;
								alpha           = (100.0f - sqrtf(dx * dx + dz * dz)) / 100.0f;
								if (alpha < 0.0f) {
									alpha = 0.0f;
								}
								if (alpha > 0.8f) {
									alpha = 0.8f;
								}
							}
							obj->vfBC(&obj->mMoveTranslate, sin(3.1415927f * alpha * 0.5f));
						}
					}
					fn_80199944(&game->mGs);
				}
				shadow->EndRender();
				break;
			}
		}
		InvalidateAll();
	}
}

/**
 * @note Address: 0x8000D118
 * @note Size: 0x5EC
 */
// NONMATCHING: the second loop's registers rotate
// clears the screen's alpha from the second camera, draws the 0x40 objects and the two the
// player is busy with into it, then sets the 0x40 objects back up for the main pass
extern "C" void fn_8000D118(CGame* game)
{
	XObj* obj;
	s32 i;
	UnkPlayer* player;
	if ((game->mStatus.mFlags & 1) && (game->mStatus.mFlags & 0x4000000000)) {
		game->mGs.mCamera = game->vf34();
		if (fn_8019997C(game)) {
			InvalidateAll();
			fn_80199AB0(&lbl_8025C400.mGs, 4, 0);
			fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 0, 4, 5, 15);
			fn_8019B648(lbl_8025C400.mGs.mCurRenderMode, 0);
			fn_8019B5D0(lbl_8025C400.mGs.mCurRenderMode, 1);
			fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 0, 7, 0);
			fn_8019B3E8(lbl_8025C400.mGs.mCurRenderMode, 0, 0);
			SetupAll(TRUE);
			Vec quad[4] = { { 0.0f, 0.0f, 0.0f }, { 640.0f, 0.0f, 0.0f }, { 640.0f, 448.0f, 0.0f }, { 0.0f, 448.0f, 0.0f } };
			fn_80198614(&lbl_8025C400.mGs, quad[0].x, quad[0].y, quad[0].z, 4, quad[1].x, quad[1].y, quad[1].z, 4, quad[2].x, quad[2].y,
			            quad[2].z, 4, quad[3].x, quad[3].y, quad[3].z, 4);
			fn_8019B648(lbl_8025C400.mGs.mCurRenderMode, 1);
			InvalidateAll();
			fn_80199944(&game->mGs);
		}

		game->mGs.mCamera = game->GetCamera();
		if (fn_8019997C(game)) {
			XObj* targets[2] = { NULL, NULL };
			player           = (UnkPlayer*)game->mXObjs.mPools[2].First();
			if (targets[0] == NULL) {
				targets[0] = player->_A94;
			}
			if (targets[1] == NULL) {
				targets[1] = player->_A64;
			}
			if (targets[1] == NULL) {
				targets[1] = player->_C08;
			}
			if (targets[1] == NULL) {
				targets[1] = player->_B84;
			}
			if (targets[1] == NULL) {
				targets[1] = player->_BC8;
			}
			for (i = 0; i < game->mDrawLists[1].mCount; i++) {
				obj = game->mDrawLists[1].mObjects[i];
				if (obj->mStatus.mFlags & 0x40) {
					obj->vfDC(1, 0);
					obj->vfAC();
				} else if ((targets[0] == obj || targets[1] == obj) && !(obj->mStatus.mFlags & 0x800000)) {
					obj->vfAC();
				}
			}
			fn_80199944(&game->mGs);
		}

		for (i = 0; i < game->mDrawLists[1].mCount; i++) {
			obj = game->mDrawLists[1].mObjects[i];
			if (obj->mStatus.mFlags & 0x40) {
				if (obj->mStatus.mFlags & 0x800000) {
					obj->vfDC(1, 8);
				} else {
					obj->vfDC(1, 4);
				}
			}
		}
	}
}

/**
 * @note Address: 0x8000CBF4
 * @note Size: 0x1A8
 */
// a joint walk: finds the highest joint at or under the limit in limits[0] and keeps it in
// limits[1], nan to start, and marks the materials of those joints
extern "C" void fn_8000CBF4(HSD_JObj* jobj, f32* limits)
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
 * @note Address: 0x8000CDA0
 * @note Size: 0x28C
 */
// CGame::vf20, draws the objects in _C074 from the main camera, kind 3 ones first
extern "C" void vf20__5CGameFv(CGame* game)
{
	if (game->mStatus.mFlags & 1) {
		s32 i;
		game->mGs.mCamera = game->GetCamera();
		if (fn_8019997C(game)) {
			for (i = 0; i < game->mDrawLists[1].mCount; i++) {
				if (game->mDrawLists[1].mObjects[i]->GetCategory() == 3) {
					game->mDrawLists[1].mObjects[i]->vfAC();
				}
			}
			fn_80199944(&game->mGs);
		}
		game->mGs.mCamera = game->GetCamera();
		if (fn_8019997C(game)) {
			for (i = 0; i < game->mDrawLists[1].mCount; i++) {
				if (game->mDrawLists[1].mObjects[i]->GetCategory() != 3) {
					game->mDrawLists[1].mObjects[i]->vfAC();
				}
			}
			fn_8006F338(2, 0, NULL);
			fn_80199944(&game->mGs);
		}
		InvalidateAll();
	}
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
 * @note Address: 0x8000CBC0
 * @note Size: 0x8
 */
extern "C" f32 vf108__4XObjFv(void)
{
	return 40.0f;
}

/**
 * @note Address: 0x8000CBC8
 * @note Size: 0x2C
 */
extern "C" void vfD0__4XObjFP7CShadow(CJObj* obj, CShadow* shadow)
{
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)obj->mJObj);
}

/**
 * @note Address: 0x8000CD9C
 * @note Size: 0x4
 */
extern "C" void vfC0__4XObjFv(void)
{
}

/**
 * @note Address: 0x8000D02C
 * @note Size: 0x30
 */
extern "C" void vfAC__4XObjFv(XObj* obj)
{
	obj->vfB0(0);
}

/**
 * @note Address: 0x8000D088
 * @note Size: 0x88
 */
extern "C" f32 vf114__4XObjFv(XObj* obj)
{
	if (fpclassify(obj->_77C) == FP_NAN) {
		return 0.0f;
	}
	return obj->_77C - obj->mMoveTranslate.y;
}

/**
 * @note Address: 0x8000D110
 * @note Size: 0x8
 */
extern "C" f32 vf110__4XObjFv(XObj* obj)
{
	return obj->_6F8;
}

/**
 * @note Address: 0x8000CB4C
 * @note Size: 0x74
 */
extern "C" void vfD4__4XObjFP7CShadow(XObj* obj, CShadow* shadow)
{
	f32 size = obj->vf108();
	Vec pos  = obj->mMoveTranslate;
	shadow->AddRect(&pos, size, -size, -size, size);
}

// copies what was drawn into the effect's texture
static inline void CopyToTexture(CFbTexEff* effect)
{
	fn_8019B648(lbl_8025C400.mGs.mCurRenderMode, 1);
	fn_8019B5D0(lbl_8025C400.mGs.mCurRenderMode, 1);
	fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 0, 7, 1);
	SetupAll(TRUE);
	if (effect->mTexture.mImage != NULL) {
		u16 width, height;
		if (effect->mTexture.mMipmap) {
			width  = effect->mTexture.mWidth << 1;
			height = effect->mTexture.mHeight << 1;
		} else {
			width  = effect->mTexture.mWidth;
			height = effect->mTexture.mHeight;
		}
		GXSetTexCopySrc(0, 0, width, height);
		GXSetTexCopyDst(effect->mTexture.mWidth, effect->mTexture.mHeight, effect->mTexture.mFormat, effect->mTexture.mMipmap);
		GXCopyTex(effect->mTexture.mImage, GX_TRUE);
		GXPixModeSync();
		InvalidateAll();
	}
}

/**
 * @note Address: 0x8000D704
 * @note Size: 0x34C
 */
// NONMATCHING: the mirror, its view and the object list pointer rotate registers
// draws what each mirror shows into its texture
extern "C" void fn_8000D704(CGame* game)
{
	MtxPtr view;
	FbMirrorEx* mirror;
	s32 i;
	s32 j;
	XObj* obj;
	if ((game->mStatus.mFlags & 1) && (game->mStatus.mFlags & 0x100000000000)) {
		for (i = 0; i < 5; i++) {
			mirror = &sFbMirrors[i];
			if (mirror->mJoint != NULL && mirror->mVisible && !IsMirrorOff(mirror)) {
				game->mGs.mCamera = &mirror->mCamera;
				if (fn_8019997C(game)) {
					view = mirror->mView;
					for (j = 0; j < game->mDrawLists[2].mCount; j++) {
						obj = game->mDrawLists[2].mObjects[j];
						if (!(obj->mStatus.mFlags & 0x200000) && (obj->mStatus.mFlags & 0x200000000000)) {
							obj->vfB4(view);
						}
					}
					fn_80199944(&game->mGs);
					if (mirror->mVisible) {
						CopyToTexture(mirror);
					}
				}
			}
		}
	}
}
