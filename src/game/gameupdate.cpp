#include "game/stg.h"
#include "game/xobjmap.h"
#include "game/cgameutil.h"

extern "C" {
void fn_8000A870(u32 frames);
}

/**
 * @note Address: 0x80010CD0
 * @note Size: 0x48
 */
// CMenu::Open
extern "C" void Open__5CMenuFv(CMenu* menu)
{
	menu->mState = 1;
	menu->Resume();
	menu->mWindow.mOpen = TRUE;
}

/**
 * @note Address: 0x80010D18
 * @note Size: 0x10
 */
// CMenu::Resume
extern "C" void Resume__5CMenuFv(CMenu* menu)
{
	menu->mSuspended   = FALSE;
	menu->mSuspendFade = FALSE;
}

extern "C" {
void fn_8001E11C(XSound* sound, BOOL pause);
s32 HSD_DevComIsBusy(s32 id);
BOOL fn_80131D2C(void);
void fn_80088848(void* card);
u8* fn_8006F3EC(void);
void CMsg_Update(CMsg* msg, u32 frames);
void fn_8001F46C(XSound* sound, u32 frames);
void fn_8002E8D0(void* title, u32 frames);
void fn_80031460(void* menu, u32 frames);
void fn_8006D548(void* info, u32 frames);
void Update__8FbMirrorFUl(FbMirror* mirror, u32 frames);
void fn_80011270(CGame* game);
}

/**
 * @note Address: 0x8000FACC
 * @note Size: 0x1204
 */
// NONMATCHING: the frame count and the inlined base update's pad pointer swap registers
// CGame::Update: the base's disc and reset checks, the reset itself, then everything that
// runs each frame. while the disc can't be read only the sound is paused
extern "C" void Update__5CGameFUl(CGame* game, u32 frames)
{
	s32 i;
	BaseUpdate(game, frames);
	fn_80088848(&game->mMemcard);
	if (game->mReset && !game->mMemcard.mCard.mBusy && HSD_DevComIsBusy(1) <= 0 && HSD_DevComIsBusy(2) <= 0) {
		if (!game->mDiskOk) {
			game->mResetHold = 0;
			game->mReset     = FALSE;
			HSD_PadReset();
			game->Reset(1);
		} else if (game->_3338 != 0x17 && game->_3338 != 0x1A && !fn_80131D2C()) {
			game->mResetHold = 0;
			game->mReset     = FALSE;
			HSD_PadReset();
			if (!game->mDiskOk) {
				game->Reset(1);
			} else if (game->_3338 <= 0x13) {
				game->Reset(0);
			} else {
				fn_8000B394(game, 10000, FALSE);
			}
		}
	}

	if (game->IsDiscError()) {
		if (!(game->mStatus.mFlags & 8)) {
			game->mStatus.mFlags |= 8;
			fn_8001E5C0(&lbl_8025C400.mSound, 0xFFFF0000, 0);
		}
		return;
	}
	if (game->mStatus.mFlags & 8) {
		game->mStatus.mFlags &= ~8;
		fn_8001E504(&lbl_8025C400.mSound, 0xFFFF0000);
	}
	fn_8000A870(frames);
	if (game->mStatus.mFlags & 0x1000) {
		game->mStatus.mFlags &= ~0x1000;
		fn_80011270(game);
	}
	if ((game->mStatus.mFlags & 1) && !(game->mStatus.mFlags & 2) && !(game->mStatus.mFlags & 0x20) && !game->mMenu.mActive
	    && game->mEvt.mData.mScript == NULL && (game->mPad.mPad[0].mTrigger & 0x1000) && !(game->mPad.mPad[0].mButton & 0x200)
	    && game->mMenu.mMenus[1] != NULL) {
		game->mMenu.mMenus[1]->Open();
		game->mMenu.mActive = TRUE;
	}
	if ((game->mStatus.mFlags & 1) && !(game->mStatus.mFlags & 2)) {
		for (i = 0; i < game->mDrawLists[0].mCount; i++) {
			XObj* obj = game->mDrawLists[0].mObjects[i];
			if (obj->mStatus.mFlags & 1) {
				if (obj->mMoveTranslate.y != obj->_264.y) {
					obj->mStatus.mFlags |= 0x10000000000000;
				}
				obj->_264 = obj->mMoveTranslate;
				obj->vfA4(frames);
			}
		}
	}
	if (game->IsRunning() && !(game->mStatus.mFlags & 2)) {
		game->mPlayTime += frames;
		if (game->mPlayTime >= 21600000) {
			game->mPlayTime = 21599999;
		}
	}
	if (game->mStatus.mFlags & 0x2000000000000) {
		if (game->IsRunning() && game->mEvt.mData.mScript == NULL && !game->mMenu.mActive && !(game->mStatus.mFlags & 0xC00)
		    && game->mTimeState.mDuration > 0.0f) {
			if (!game->mTimeState.mRunning) {
				game->mTimeState.mTime = frames * (game->mTimeState.mDuration / 36000.0f) + game->mTimeState.mTime;
				if (game->mTimeState.mTime >= 1.0f) {
					game->mTimeState.mRunning = TRUE;
				}
			}
			if (game->mTimeState.mRunning == TRUE) {
				bool idle         = false;
				UnkPlayer* player = (UnkPlayer*)game->mXObjs.mPools[2].First();
				if ((player->_A48 == 0 || player->_A48 == 4) && player->_AC4 == 0 && player->_B14 == 0 && player->_B68 == 0
				    && player->_BAC == 0 && player->_BEC == 0 && player->_C44 == 0) {
					idle = true;
				}
				if (idle) {
					game->mTimeState.mRunning = FALSE;
					u8* script                = fn_8006F3EC();
					if (script != NULL) {
						fn_8007EEA4(&game->mEvt, script, 0, -1, -1);
					}
				}
			}
		}
	}
	if (!(game->mStatus.mFlags & 2)) {
		CEvt_Update(&game->mEvt, frames);
		for (i = 0; i < 2; i++) {
			CMsg_Update(&game->mCMsgs[i], frames);
		}
	}
	if (game->IsRunning()) {
		fn_8006F338(1, frames, NULL);
	}
	if (game->IsRunning() || (game->mStatus.mFlags & 0x20)) {
		game->mParticle.Update(frames);
	}
	if ((game->mStatus.mFlags & 1) && !(game->mStatus.mFlags & 0x200000000000) && !game->mMenu.mActive) {
		game->mMotionControlCamera.Update(frames);
	}
	fn_8001F46C(&game->mSound, frames);
	if (!(game->mStatus.mFlags & 2)) {
		fn_80023B38(&game->mFade, frames);
		game->mWipeCircle.Update(frames);
		game->mWipeTex.Update(frames);
		game->mWipeCustom.Update(frames);
		game->mWipeTile0.Update(frames);
		game->mWipeTile1.Update(frames);
		if (game->mStatus.mFlags & 1) {
			game->mRoboeyes.Update(frames);
			fn_800240EC(&game->mQuake, frames);
			game->mOutline.Update(frames);
			game->mScrnTex.mAngle = fmod(0.10471976f * frames + game->mScrnTex.mAngle, 6.2831854820251465);
			game->mOutline.mThick = 0.0f;
			game->mOutline.SetLineColor(0xFF, 0xFF, 0xFF, 100.0f * (f32)sin(game->mScrnTex.mAngle) + 150.0f);
			fn_80023B38(&game->mScrnTex.mColor, frames);
			game->mOutline.SetColor2(game->mScrnTex.mColor.mColor.r, game->mScrnTex.mColor.mColor.g, game->mScrnTex.mColor.mColor.b);
			fn_80023B38(&game->mColor0.mColor, frames);
			game->mOutline.SetColor3(game->mColor0.mColor.mColor.r, game->mColor0.mColor.mColor.g, game->mColor0.mColor.mColor.b,
			                         game->mColor0.mColor.mColor.a);
			game->mBlur.Update(frames);
		}
	} else if (game->mMotionControlCamera.IsAction(4)) {
		fn_80023B38(&game->mFade, frames);
		game->mWipeCircle.Update(frames);
		game->mWipeTex.Update(frames);
		game->mWipeCustom.Update(frames);
		game->mWipeTile0.Update(frames);
		game->mWipeTile1.Update(frames);
	}
	fn_80023B38(&game->mColor1.mColor, frames);
	game->mOutline.SetColor4(game->mColor1.mColor.mColor.r, game->mColor1.mColor.mColor.g, game->mColor1.mColor.mColor.b,
	                         game->mColor1.mColor.mColor.a);
	if (game->_4724 > 0) {
		if (game->mPad.mPad[0].mSubStickY > 0.0f) {
			if ((game->_4728 -= 0.01f * frames) < 0.04f) {
				game->_4728 = 0.04f;
			}
		} else if (game->mPad.mPad[0].mSubStickY < 0.0f) {
			if ((game->_4728 += 0.01f * frames) > 0.5f) {
				game->_4728 = 0.5f;
			}
		}
		game->mOutline.mZoom = sin(3.1415927f * game->_4728 / 2.0f);
	}
	fn_80027528(&game->mLogo, frames);
	fn_8002E8D0(&game->mTitle, frames);
	fn_80031460(&game->mMenu, frames);
	if ((game->mStatus.mFlags & 1) && !(game->mStatus.mFlags & 2)) {
		fn_8006D548(&game->mInfoScrn, frames);
		game->mSubwin.Update(frames);
		for (i = 0; i < 5; i++) {
			if (!IsMirrorOff(&sFbMirrors[i])) {
				Update__8FbMirrorFUl(&sFbMirrors[i], frames);
			}
		}
	}
	if (!(game->mStatus.mFlags & 2) && game->mStageName.mActive && game->mStageName.mActive > 0) {
		f32 grow = 0.055555556f * frames;
		for (i = 0; i < game->mStageName.mCount; i++) {
			if (game->mStageName.mChars[i].mWait > 0.0f) {
				if (game->mStageName.mChars[i].mScale < 1.0f) {
					if ((game->mStageName.mChars[i].mScale += grow) > 1.0f) {
						game->mStageName.mChars[i].mScale = 1.0f;
					}
				} else if (game->mStatus.mFlags & 1) {
					if ((game->mStageName.mChars[i].mWait -= frames) < 0.0f) {
						game->mStageName.mChars[i].mWait = 0.0f;
					}
				}
			} else if (game->mStageName.mChars[i].mScale > 0.0f) {
				if ((game->mStageName.mChars[i].mScale -= grow) < 0.0f) {
					game->mStageName.mChars[i].mScale = 0.0f;
				}
			}
		}
		s32 moving = 0;
		f32 bob    = 0.08726646f * frames;
		f32 tilt   = 0.05235988f * frames;
		for (i = 0; i < game->mStageName.mCount; i++) {
			if (0.0f != game->mStageName.mChars[i].mScale) {
				moving++;
				game->mStageName.mChars[i].mBob += bob;
				game->mStageName.mChars[i].mTilt += tilt;
			}
		}
		if (moving == 0) {
			game->mStageName.mActive = FALSE;
		}
	}
	if (game->mStatus.mFlags & 2) {
		if (!(game->mStatus.mFlags & 4)) {
			game->mStatus.mFlags |= 4;
			fn_8001E11C(&lbl_8025C400.mSound, TRUE);
			CPadEx* pad = &game->mPad;
			for (i = 0; i < 4; i++) {
				if (pad->mPad[i].mConnected) {
					HSD_PadRumbleOffN(pad->mPad[i].mPort);
				}
			}
			HSD_PadRumblePauseAll();
		}
	} else if (game->mStatus.mFlags & 4) {
		game->mStatus.mFlags &= ~4;
		fn_8001E11C(&lbl_8025C400.mSound, FALSE);
		HSD_PadRumbleActiveAll();
	}
}

extern "C" {
void fn_8001146C(CGame* game);
}

/**
 * @note Address: 0x80010D28
 * @note Size: 0x548
 */
// NONMATCHING: register allocation, the counters and loop pointers come out in other registers
// sorts the objects into the ones to draw and the ones mirrors show, those of pool 3 first,
// then the plain ones, then the 0xA00000 ones, then the 0x40 ones. objects out of view are only
// mirrored
extern "C" void fn_80010D28(CGame* game)
{
	s32 shown;
	XObj* obj;
	CMotionControlCamera* camera;
	s32 i;
	s32 mirrored;
	if (game->mStatus.mFlags & 0x8000000000) {
		camera = &game->mMotionControlCamera;
		for (i = 0; i < game->mXObjs.mObjectCount; i++) {
			obj = game->mXObjs.mObjects[i];
			if ((obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 0x10000)) {
				if (!(game->mStatus.mFlags & 0x100) && (obj->mStatus.mFlags & 0x80000000) && obj->vf11C(&camera->mCamera)) {
					obj->mStatus.mFlags |= 0x1000000000000;
				} else {
					obj->mStatus.mFlags &= ~0x1000000000000;
				}
			}
		}
	}

	mirrored = 0;
	shown    = 0;
	for (i = 0; i < game->mXObjs.mPools[3].mUsed; i++) {
		obj = game->mXObjs.mPools[3].Get(i);
		if ((obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 0x10000) && !(obj->mStatus.mFlags & 0xA00040)) {
			if (!(obj->mStatus.mFlags & 0x1000000000000)) {
				game->mDrawLists[1].mObjects[shown++] = obj;
			}
			game->mDrawLists[2].mObjects[mirrored++] = obj;
		}
	}
	for (i = 0; i < game->mXObjs.mObjectCount; i++) {
		obj = game->mXObjs.mObjects[i];
		if (obj->GetCategory() != 3 && (obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 0x10000) && !(obj->mStatus.mFlags & 0xA00040)) {
			if (!(obj->mStatus.mFlags & 0x1000000000000)) {
				game->mDrawLists[1].mObjects[shown++] = obj;
			}
			game->mDrawLists[2].mObjects[mirrored++] = obj;
		}
	}
	for (i = 0; i < game->mXObjs.mObjectCount; i++) {
		obj = game->mXObjs.mObjects[i];
		if (obj->GetCategory() != 3 && (obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 0x10000) && (obj->mStatus.mFlags & 0xA00000)
		    && !(obj->mStatus.mFlags & 0x40)) {
			if (!(obj->mStatus.mFlags & 0x1000000000000)) {
				game->mDrawLists[1].mObjects[shown++] = obj;
			}
			game->mDrawLists[2].mObjects[mirrored++] = obj;
		}
	}
	for (i = 0; i < game->mXObjs.mObjectCount; i++) {
		obj = game->mXObjs.mObjects[i];
		if (obj->GetCategory() != 3 && (obj->mStatus.mFlags & 1) && (obj->mStatus.mFlags & 0x10000) && (obj->mStatus.mFlags & 0x40)) {
			if (!(obj->mStatus.mFlags & 0x1000000000000)) {
				game->mDrawLists[1].mObjects[shown++] = obj;
			}
			game->mDrawLists[2].mObjects[mirrored++] = obj;
		}
	}
	game->mDrawLists[1].mCount = shown;
	game->mDrawLists[2].mCount = mirrored;
}

/**
 * @note Address: 0x80011270
 * @note Size: 0x1FC
 */
// NONMATCHING: the object and the loop pointers get other volatile registers
// lists the objects to draw: those with flag 0x4000 first, then the rest, then the two
// special ones
extern "C" void fn_80011270(CGame* game)
{
	s32 count = 0;
	s32 i;
	for (i = 0; i < game->mXObjs.mObjectCount; i++) {
		if (game->mXObjs.mObjects[i] != game->mXObjs.mPools[2].First() && game->mXObjs.mObjects[i] != game->mXObjs.mPools[0].First()
		    && (game->mXObjs.mObjects[i]->mStatus.mFlags & 1) && (game->mXObjs.mObjects[i]->mStatus.mFlags & 0x4000)) {
			game->mDrawLists[0].mObjects[count] = game->mXObjs.mObjects[i];
			game->mDrawLists[0].mIndices[count] = i;
			count++;
		}
	}
	for (i = 0; i < game->mXObjs.mObjectCount; i++) {
		if (game->mXObjs.mObjects[i] != game->mXObjs.mPools[2].First() && game->mXObjs.mObjects[i] != game->mXObjs.mPools[0].First()
		    && (game->mXObjs.mObjects[i]->mStatus.mFlags & 1) && !(game->mXObjs.mObjects[i]->mStatus.mFlags & 0x4000)) {
			game->mDrawLists[0].mObjects[count] = game->mXObjs.mObjects[i];
			game->mDrawLists[0].mIndices[count] = i;
			count++;
		}
	}
	if (game->mXObjs.mPools[2].First()->mStatus.mFlags & 1) {
		game->mDrawLists[0].mObjects[count++] = game->mXObjs.mPools[2].First();
	}
	if (game->mXObjs.mPools[0].First()->mStatus.mFlags & 1) {
		game->mDrawLists[0].mObjects[count++] = game->mXObjs.mPools[0].First();
	}
	game->mDrawLists[0].mCount = count;
}

/**
 * @note Address: 0x8001146C
 * @note Size: 0x1A0
 */
// NONMATCHING: the light count isn't read again for the second GetLights
// picks the lights to draw with: an object's own when it has some, else the game's. only the
// object lights cast shadows then
extern "C" void fn_8001146C(CGame* game)
{
	game->mLightList = NULL;
	XObjMap* a       = (XObjMap*)game->mXObjs.mPools[3].mObjects;
	if ((a->mStatus.mFlags & 1) && a->GetLights() != NULL) {
		game->mLightList = a->GetLights();
	} else {
		XObjLight* b = (XObjLight*)game->mXObjs.mPools[1].mObjects;
		if (b->mStatus.mFlags & 1) {
			game->mLightList = b->GetLights();
		}
	}

	if (game->mLightList != NULL) {
		CLight* light;
		for (light = game->mLightList; light != NULL; light = (CLight*)light->next) {
			if (light->mShadow.IsActive()) {
				HSD_CObjSetProjectionType(light->mShadow.mShadow->camera, 3);
			}
		}
		for (light = game->mLights; light != NULL; light = (CLight*)light->next) {
			if (light->mShadow.IsActive()) {
				light->mShadow.SetActive(FALSE);
			}
		}
	} else {
		game->mLightList = game->mLights;
	}
}
