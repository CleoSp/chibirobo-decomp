#include "game/cgame.h"
#include "game/xobjmap.h"
#include "game/mm.h"
#include "game/hsd_dobj.h"
#include "game/cgameutil.h"

extern "C" {
u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, GXBool mipmap, u8 maxLod);
void fn_80011270(CGame* game);
void fn_8001146C(CGame* game);
BOOL fn_80132B98(XOBJS* objs);
void fn_80141754(CInterfaceCameraAction* action);
}

static inline HSD_JObj* GetChild(HSD_JObj* jobj)
{
	return jobj == NULL ? NULL : jobj->child;
}

// the last joint down the first children
static inline HSD_JObj* GetLeaf(HSD_JObj* jobj)
{
	while (GetChild(jobj) != NULL) {
		jobj = GetChild(jobj);
	}
	return jobj;
}

/**
 * @note Address: 0x80011638
 * @note Size: 0x5A0
 */
// NONMATCHING: the stage's camera mode and the second loop's registers rotate
// the step after the objects are read: points the camera and its actions at them, lists them
// and sets up a mirror for each object that has one, at most five
extern "C" BOOL fn_80011638(CGame* game)
{
	FbMirrorEx* mirror;
	s32 mirrors;
	XObj* obj;
	s32 mode;
	s32 i;
	HSD_JObj* jobj;
	if (fn_80132B98(&game->mXObjs)) {
		mode                                                                  = GetStageInfo()->mCameraMode;
		((CCameraActionScroll*)lbl_8025C400.mCameraActions.mTable[11])->mZoom = 1;
		if (fn_8006F3CC() && fn_8006F3BC()) {
			game->mCameraActions.mTable[1]->vf14();
		}
		if (fn_8006F3CC() && fn_8006F3BC()) {
			((CCameraActionAreaExM*)game->mCameraActions.mTable[1])->vf64();
		}
		((CCameraActionAnim*)game->mCameraActions.mTable[6])->SetObj((XObjCamera*)game->mXObjs.mPools[0].First());
		game->mMotionControlCamera.Init();
		if (mode == 1 || mode == 7) {
			game->mMotionControlCamera.ChangeAction(7);
			fn_80141754(game->mCameraActions.mTable[7]);
			game->mMotionControlCamera.ApplyAction();
		} else {
			game->mMotionControlCamera.ChangeAction(mode);
		}
		for (i = 0; i < game->mXObjs.mObjectCount; i++) {
			game->mMotionControlCamera.UpdateTarget(game->mXObjs.mObjects[i]);
		}

		if (game->mStatus.mFlags & 0x100) {
			game->mStatus.mFlags &= ~0x300;
			fn_800236EC(&game->mRoboeyes, 0, 0, 0, 0, 1, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		}
		fn_8001146C(game);
		fn_80011270(game);

		game->mStatus.mFlags &= ~0x2000000000;
		mirror = lbl_8035FA24;
		game->mStatus.mFlags &= ~0x8000000000;
		game->mStatus.mFlags &= ~0x100000000000;
		mirrors = 0;
		for (i = 0; i < game->mXObjs.mObjectCount; i++) {
			obj = game->mXObjs.mObjects[i];
			if (obj->mStatus.mFlags & 1) {
				if (!(game->mStatus.mFlags & 0x2000000000) && (obj->mStatus.mFlags & 0x20000000)
				    && !(obj->mStatus.mFlags & 0x400000000000000)) {
					game->mStatus.mFlags |= 0x2000000000;
				}
				if ((obj->mStatus.mFlags & 0x400) && mirrors < 5) {
					game->mStatus.mFlags |= 0x100000000000;
					jobj = obj->mJObj;
					if (jobj != NULL) {
						mirror->mOwner           = obj;
						jobj                     = GetLeaf(jobj);
						mirror->mJoint           = jobj;
						mirror->mVisible         = FALSE;
						mirror->mTexture.mWidth  = 256;
						mirror->mTexture.mHeight = 256;
						mirror->mTexture.mFormat = GX_TF_RGB565;
						mirror->mTexture._0C     = 0;
						mirror->mTexture.mX      = 0.0f;
						mirror->mTexture.mY      = 0.0f;
						mirror->mTexture.mMipmap = 0;
						mirror->mTexture.mImage  = mm::Alloc(GXGetTexBufferSize(256, 256, GX_TF_RGB565, GX_FALSE, 0), "(mirror)");
						mirror->mActive          = TRUE;
						mirror->mTexture.mTex.Init(mirror->mTexture.mImage, mirror->mTexture.mWidth, mirror->mTexture.mHeight,
						                           mirror->mTexture.mFormat, GX_CLAMP, GX_CLAMP, GX_FALSE);
						for (HSD_DObj* dobj = HSD_JObjGetDObj(jobj); dobj != NULL; dobj = HSD_DObjGetNext(dobj)) {
							HSD_MObj* mobj = HSD_DObjGetMObj(dobj);
							if (mobj != NULL) {
								fn_801BA3AC(mobj, 255, 255, 255);
								HSD_TObj* tobj = HSD_MObjGetTObj(mobj);
								if (tobj != NULL) {
									tobj->imagedesc = (HSD_ImageDesc*)mirror;
								}
							}
						}
						mirror++;
						mirrors++;
					}
				}
				if (!(game->mStatus.mFlags & 0x8000000000) && (obj->mStatus.mFlags & 0x80000000)) {
					game->mStatus.mFlags |= 0x8000000000;
				}
			}
		}

		if (game->mStatus.mFlags & 0x2000000000) {
			f32 fade  = GetStageInfo()->mMirrorFade / 100.0f;
			XObj* obj = game->mXObjs.mPools[3].First();
			if ((obj->mStatus.mFlags & 1) && fade > 0.0f && fade < 1.0f) {
				obj->vfE4(fade);
			}
		}
		return TRUE;
	}
	return FALSE;
}
