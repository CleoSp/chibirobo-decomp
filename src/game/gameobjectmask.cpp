#include "game/cgame.h"
#include "game/objectmasktargets.h"
#include "game/xobjmap.h"
#include "game/hsd_dobj.h"
#include "game/cgameutil.h"
#include "game/xchr.h"

extern "C" {
void GXSetTexCopySrc(u16 left, u16 top, u16 wd, u16 ht);
void GXSetTexCopyDst(u16 wd, u16 ht, GXTexFmt fmt, GXBool mipmap);
void GXCopyTex(void* dest, GXBool clear);
}

/**
 * @note Address: 0x8000D118
 * @note Size: 0x5EC
 */
// NONMATCHING: the second loop's registers rotate
// clears the screen's alpha from the second camera, draws the 0x40 objects and the two the
// player is busy with into it, then sets the 0x40 objects back up for the main pass
extern "C" void DrawObjectAlphaMask(CGame* game)
{
	XObj* obj;
	s32 i;
	XObjChr* player;
	if ((game->mStatus.mFlags & 1) && (game->mStatus.mFlags & 0x4000000000)) {
		game->mGs.mCamera = game->vf34();
		if (BeginGsCamera(game)) {
			InvalidateAll();
			SetGsChannelEnabled(&gGame.mGs, 4, 0);
			SetGsBlendMode(gGame.mGs.mCurRenderMode, 0, 4, 5, 15);
			SetGsColorUpdate(gGame.mGs.mCurRenderMode, 0);
			SetGsAlphaUpdate(gGame.mGs.mCurRenderMode, 1);
			SetGsDepthMode(gGame.mGs.mCurRenderMode, 0, 7, 0);
			SetGsDestinationAlpha(gGame.mGs.mCurRenderMode, 0, 0);
			SetupAll(TRUE);
			Vec quad[4] = { { 0.0f, 0.0f, 0.0f }, { 640.0f, 0.0f, 0.0f }, { 640.0f, 448.0f, 0.0f }, { 0.0f, 448.0f, 0.0f } };
			DrawGsVertexQuad(&gGame.mGs, quad[0].x, quad[0].y, quad[0].z, quad[1].x, quad[1].y, quad[1].z, quad[2].x, quad[2].y, quad[2].z,
			                 quad[3].x, quad[3].y, quad[3].z, 4, 4, 4, 4);
			SetGsColorUpdate(gGame.mGs.mCurRenderMode, 1);
			InvalidateAll();
			EndGsCamera(&game->mGs);
		}

		game->mGs.mCamera = game->GetCamera();
		if (BeginGsCamera(game)) {
			ObjectMaskTargets targets = sEmptyObjectMaskTargets;
			player                    = game->GetPlayer();
			if (targets.plug == NULL) {
				targets.plug = player->mLift.mPlugTarget;
			}
			if (targets.interaction == NULL) {
				targets.interaction = player->mLift.mTarget;
			}
			if (targets.interaction == NULL) {
				targets.interaction = player->mPush.mTarget;
			}
			if (targets.interaction == NULL) {
				targets.interaction = player->mLadder.mTarget;
			}
			if (targets.interaction == NULL) {
				targets.interaction = player->mRope.mTarget;
			}
			for (i = 0; i < game->mDrawLists[1].mCount; i++) {
				obj = game->mDrawLists[1].mObjects[i];
				if (obj->mStatus.mFlags & 0x40) {
					obj->vfDC(1, 0);
					obj->vfAC();
				} else if ((targets.plug == obj || targets.interaction == obj) && !(obj->mStatus.mFlags & 0x800000)) {
					obj->vfAC();
				}
			}
			EndGsCamera(&game->mGs);
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

// copies what was drawn into the effect's texture
static inline void CopyToTexture(CFbTexEff* effect)
{
	SetGsColorUpdate(gGame.mGs.mCurRenderMode, 1);
	SetGsAlphaUpdate(gGame.mGs.mCurRenderMode, 1);
	SetGsDepthMode(gGame.mGs.mCurRenderMode, 0, 7, 1);
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
#pragma push
#pragma opt_common_subs off
extern "C" void RenderMirrorTextures(CGame* game)
{
	MtxPtr view;
	FbMirrorEx* mirror;
	s32 i;
	s32 j;
	XObj* obj;
	if ((game->mStatus.mFlags & 1) && (game->mStatus.mFlags & 0x100000000000)) {
		for (i = 0; i < 5; i++) {
			mirror = &gFramebufferMirrors[i];
			if (mirror->mJoint != NULL && mirror->mVisible && !IsMirrorOff(mirror)) {
				game->mGs.mCamera = &mirror->mCamera;
				if (BeginGsCamera(game)) {
					view = mirror->mView;
					for (j = 0; j < game->mDrawLists[2].mCount; j++) {
						obj = game->mDrawLists[2].mObjects[j];
						if (!(obj->mStatus.mFlags & 0x200000) && (obj->mStatus.mFlags & 0x200000000000)) {
							obj->DrawReflection(view);
						}
					}
					EndGsCamera(&game->mGs);
					if (mirror->mVisible) {
						CopyToTexture(mirror);
					}
				}
			}
		}
	}
}
#pragma pop
