#include "game/texturecopy.h"
#include "game/fbtexeff.h"
#include "game/xobjs.h"

// todo: cleanup
static inline bool OwnerHidden(FbMirrorEx* mirror)
{
	return mirror->mOwner == NULL || (mirror->mOwner->mStatus.mFlags & 0x0001000000000000ULL);
}

/**
 * @note Address: 0x8000D704
 * @note Size: 0x34C
 */
// draws the reflected scene into each visible mirror's texture
extern "C" void fn_8000D704(CGame* game)
{
	if ((game->mStatus.mFlags & 1ULL) && (game->mStatus.mFlags & 0x0000100000000000ULL)) {
		CGs* graphics = CaptureGraphics();
		for (s32 i = 0; i < 5; ++i) {
			FbMirrorEx* mirror = &lbl_8035FA24[i];
			if (mirror->mJoint == NULL || !mirror->mVisible || OwnerHidden(mirror))
				continue;
			((CGs*)game->mGs)->mCamera = &mirror->mCamera;
			if (fn_8019997C((CBase*)game) == NULL)
				continue;
			for (s32 j = 0; j < game->mDrawLists[2].mCount; ++j) {
				XObj* object = game->mDrawLists[2].mObjects[j];
				if (!(object->mStatus.mFlags & 0x200000ULL) && (object->mStatus.mFlags & 0x0000200000000000ULL)) {
					object->vfB4(mirror->mView);
				}
			}
			fn_80199944((CGs*)game);
			if (mirror->mVisible) {
				fn_8019B648(graphics->mCurRenderMode, 1);
				fn_8019B5D0(graphics->mCurRenderMode, 1);
				fn_8019B464(graphics->mCurRenderMode, 0, 7, 1);
				FlushCaptureGraphics(graphics);
				CopyCaptureTexture(&mirror->mTexture, graphics);
			}
		}
	}
}
