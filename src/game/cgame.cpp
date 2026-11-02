#include "game/hsd_jobj.h"
#include "game/cparticle.h"
#include "game/cjobjset.h"
#include "game/cgs.h"
#include "game/ccamera.h"
#include "game/clight.h"
#include "game/xobj.h"
#include "game/cmotioncontrolcamera.h"
#include "game/stg.h"
#include "game/infoscrn.h"
#include "game/fbeffect.h"
#include "game/fbtexeff.h"
#include "game/cmovie.h"
#include "game/ccard.h"
#include "game/memcard.h"
#include "game/cthread.h"
#include "game/msg.h"
#include "game/logo.h"
#include "game/xobjmap.h"
#include "game/mm.h"
#include "game/hsd_dobj.h"
#include "game/gmath.h"
#include <dolphin/os.h>
#include <Dolphin/vi.h>
#include <math.h>

// the game core: the CBase and CGame classes that the main loop drives, and the weak copies
// of the inline functions they pull in. the destructors and the interpolation methods are the
// compiler's own copies, renamed in symbols.txt; the copies whose class is still unknown are
// extern "C" stand-ins under their dol address

extern "C" {
void HSD_MtxGetTranslate(Mtx mtx, Vec* translate);
void HSD_VISetBlack(u8 black);
u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, GXBool mipmap, u8 maxLod);
void PSVECScale(const Vec* src, Vec* dst, f32 scale);
void HSD_VICopyXFBAsync(s32 pri);
void OSInitStopwatch(void* sw, char* name);
void OSResetStopwatch(void* sw);
void OSStartStopwatch(void* sw);
void OSStopStopwatch(void* sw);
s64 OSCheckStopwatch(void* sw);
}

extern "C" {
#include <sysdolphin/controller.h>
}
#include "game/cpad.h"
#include "game/cstopwatch.h"
#include "game/csound.h"

extern "C" {
void HSD_PadRumbleOffN(u8 no);
void HSD_PadRumblePauseAll(void);
void fn_8001D8C0();
void fn_8001D51C(CBase* base);
void fn_8001D4EC();
void fn_8001D4C0();
#if defined(VERSION_PAL)
u8 OSGetLanguage();
// os language and game language pairs
extern u8 lbl_802162E0[12];
#endif
}

// a mirror is off while its owner is gone or hidden
static inline bool IsMirrorOff(FbMirrorEx* mirror)
{
	bool off = false;
	if (mirror->mOwner == NULL || (mirror->mOwner->mStatus.mFlags & 0x1000000000000)) {
		off = true;
	}
	return off;
}

extern "C" {
MtxPtr fn_8019997C(CBase* base);
void fn_80199944(CGs* gs);
void fn_8019B648(CGsRenderMode* mode, u8 enable);
void fn_8019B5D0(CGsRenderMode* mode, u8 enable);
void fn_8019B56C(CGsRenderMode* mode, s32 type, s32 src, s32 dst, s32 op);
void fn_8019B504(CGsRenderMode* mode, s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
void fn_8019B464(CGsRenderMode* mode, u8 enable, s32 func, u8 update);
void fn_8019B360(CGsRenderMode* mode, s32 cull);
void fn_8019B278(CGsRenderMode* mode, BOOL force);
void fn_8019AFA0(CGsTevStage* stage, s32 mode);
void fn_8019AED0(CGsTevStage* stage, BOOL force);
void fn_8019AE94(CGsChannel* chan, BOOL force);
void fn_80199AB0(CGs* gs, s32 chan, u8 enable);
void GXSetTexCopySrc(u16 left, u16 top, u16 wd, u16 ht);
void GXSetTexCopyDst(u16 wd, u16 ht, GXTexFmt fmt, GXBool mipmap);
void GXCopyTex(void* dest, GXBool clear);
void GXPixModeSync(void);
}

// sends all of the current render, tev and channel state
static inline void SetupAll(BOOL force)
{
	CGs* gs = &lbl_8025C400.mGs;
	if (gs->mCurRenderMode != NULL) {
		fn_8019B278(gs->mCurRenderMode, force);
	}
	if (gs->mCurTevStage != NULL) {
		fn_8019AED0(gs->mCurTevStage, force);
	}
	if (gs->mCurChannel != NULL) {
		fn_8019AE94(gs->mCurChannel, force);
	}
}

// forgets all of the state after drawing behind its back
static inline void InvalidateAll()
{
	CGs* gs = &lbl_8025C400.mGs;
	HSD_StateInvalidate(HSD_STATE_ALL);
	if (gs->mCurRenderMode != NULL) {
		gs->mCurRenderMode->Invalidate();
	}
	if (gs->mCurTevStage != NULL) {
		gs->mCurTevStage->Invalidate();
	}
	if (gs->mCurChannel != NULL) {
		if (!gs->mCurChannel->mDirty) {
			gs->mCurChannel->mDirty = TRUE;
		}
	}
}

/**
 * @note Address: 0x80007880
 * @note Size: 0x6BC
 */
CGame::~CGame()
{
}

/**
 * @note Address: 0x80007F3C
 * @note Size: 0x114
 */
CBase::~CBase()
{
}

/**
 * @note Address: 0x8000A4B8
 * @note Size: 0x68
 */
// CGame's FrameInterval, the logo and title run at 60, the game at 30
extern "C" s32 fn_8000A4B8(CGame* game)
{
	return game->IsIntroActive() ? 1 : 2;
}

/**
 * @note Address: 0x8000A520
 * @note Size: 0xC
 */
extern "C" void* fn_8000A520(CGame* game)
{
	return &game->mMotionControlCamera;
}

/**
 * @note Address: 0x8000A52C
 * @note Size: 0xC
 */
extern "C" CLight* fn_8000A52C(CGame* game)
{
	return game->mLightList;
}

/**
 * @note Address: 0x8000A538
 * @note Size: 0x8
 */
extern "C" u32 fn_8000A538(CBase* base)
{
	return base->mRetraceStep;
}

/**
 * @note Address: 0x8000A540
 * @note Size: 0x8
 */
extern "C" CCamera* fn_8000A540(CBase* base)
{
	return &base->mCameras[0];
}

/**
 * @note Address: 0x8000A548
 * @note Size: 0x8
 */
extern "C" CCamera* fn_8000A548(CBase* base)
{
	return &base->mCameras[1];
}

/**
 * @note Address: 0x8000A550
 * @note Size: 0x8
 */
extern "C" CLight* fn_8000A550(CBase* base)
{
	return &base->mLights[0];
}

/**
 * @note Address: 0x8000A558
 * @note Size: 0x8
 */
extern "C" CLight* fn_8000A558(CBase* base)
{
	return &base->mLights[4];
}

extern "C" {
int strncmp(const char* a, const char* b, unsigned long n);
CTexObj* fn_8019E8F4(CFont* font, const char* str, s32* width);
void fn_80197800(CGs* gs, CTexObj* tex, u32 color0, u32 color1, u32 color2, u32 color3, f32 x, f32 y, f32 z, f32 w, f32 h, f32 rx, f32 ry,
                 f32 rz);
}
extern "C" f32 lbl_8020FC10[4];
extern "C" f32 lbl_8020FC20[4];

/**
 * @note Address: 0x8000AD34
 * @note Size: 0x600
 */
// NONMATCHING: the loop's float registers are assigned in another order
// draws the stage name: a bubble per letter, then the letter with a shadow on four sides
extern "C" void fn_8000AD34(CGame* game)
{
	if (game->mStageName.mActive && !(game->mStatus.mFlags & 2)) {
		fn_8019B56C(lbl_8025C400.mGs.mCurRenderMode, 1, 4, 5, 15);
		fn_80199AB0(&lbl_8025C400.mGs, 0, 0);
		fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 7, 0);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
		CTexObj bubble = lbl_8025C400.mTpl.mTex.Init(&((TPLPalette*)lbl_8025C400.mTpl.mBuffer)->descriptorArray[138], 0);
		f32 zero       = 0.0f;
		u32 color      = game->mStageName.mColor & 0xFFFFFF00;
		for (s32 i = 0; i < game->mStageName.mCount; i++) {
			f32 scale = game->mStageName.mChars[i].mScale;
			if (scale <= zero || strncmp(game->mStageName.mChars[i].mText, " ", 1) == 0) {
				continue;
			}
			CFont font;
			s32 width;
			CTexObj letter = *fn_8019E8F4(&font, game->mStageName.mChars[i].mText, &width);
			f32 bob;
			f32 tilt;
			f32 swell;
			f32 pop;
			f32 x;
			f32 rot;
			f32 y;
			bob       = 4.0f * (f32)sin(game->mStageName.mChars[i].mBob);
			tilt      = sin(0.7f * game->mStageName.mChars[i].mTilt);
			swell     = sin(3.1415927f * scale);
			pop       = 0.2f * swell + (f32)sin(1.5707964f * scale);
			x         = 21.6f + (43.2f * i + (320.0f - 43.2f * game->mStageName.mCount / 2.0f));
			u32 alpha = 255.0f * scale;
			rot       = -(0.017453292f * (10.0f * tilt));
			y         = 150.0f + bob;
			fn_80197800(&lbl_8025C400.mGs, &bubble, alpha | 0xFFFFFF00, alpha | 0xFFFFFF00, alpha | 0xFFFFFF00, alpha | 0xFFFFFF00, x, y,
			            0.0f, 22.0f * (2.2f * pop), 22.0f * (2.2f * pop), zero, -zero, rot);
			f32 size   = 22.0f * (1.5f * pop);
			f32 shrink = width / 24.0f * size;
			f32 off    = size - shrink;
			for (s32 j = 0; j < 4; j++) {
				fn_80197800(&lbl_8025C400.mGs, &letter, alpha, alpha, alpha, alpha, off / 2.0f + (x + lbl_8020FC10[j]),
				            bob + (150.0f + lbl_8020FC20[j]), 0.0f, size, size, zero, -zero, rot);
			}
			fn_80197800(&lbl_8025C400.mGs, &letter, color | alpha, color | alpha, color | alpha, color | alpha, off / 2.0f + x, y, 0.0f,
			            size, size, zero, -zero, rot);
		}
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
		fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 0, 7, 0);
		fn_80199AB0(&lbl_8025C400.mGs, 0, 0);
	}
}

/**
 * @note Address: 0x8000BE38
 * @note Size: 0x560
 */
// draws the movie, centred at its own size unless _FF460 says otherwise. with alpha it's drawn
// three times over
extern "C" void fn_8000BE38(CGame* game)
{
	if (game->mMovie.mMovie.mState != 0) {
		f32 w   = fpclassify(game->mMovieRect[2]) == FP_NAN ? (f32)game->mMovie.mMovie.mVideoInfo.xSize : game->mMovieRect[2];
		f32 h   = fpclassify(game->mMovieRect[3]) == FP_NAN ? (f32)game->mMovie.mMovie.mVideoInfo.ySize : game->mMovieRect[3];
		f32 x   = fpclassify(game->mMovieRect[0]) == FP_NAN ? (640.0f - w) / 2 : game->mMovieRect[0];
		f32 y   = fpclassify(game->mMovieRect[1]) == FP_NAN ? (448.0f - h) / 2 : game->mMovieRect[1];
		CGs* gs = &lbl_8025C400.mGs;
		fn_8019B648(gs->mCurRenderMode, 1);
		fn_8019B5D0(gs->mCurRenderMode, 0);
		fn_8019B464(gs->mCurRenderMode, 1, 7, 0);
		if (game->mMovie.mMovie.mAlpha) {
			fn_8019B56C(gs->mCurRenderMode, 1, 4, 5, 15);
			fn_8019B504(gs->mCurRenderMode, 4, 0x20, 0, 3, 255);
			SetupAll(FALSE);
			if (game->mMovie.mMovie.BeginDraw() >= 0) {
				for (s32 i = 0; i < 3; i++) {
					game->mMovie.mMovie.Draw(x, y, w, h);
				}
				game->mMovie.mMovie.EndDraw();
			}
		} else {
			fn_8019B56C(gs->mCurRenderMode, 0, 4, 5, 15);
			fn_8019B504(gs->mCurRenderMode, 7, 0, 0, 7, 255);
			SetupAll(FALSE);
			game->mMovie.mMovie.DrawFrame(x, y, w, h);
		}
		fn_8019B504(gs->mCurRenderMode, 4, 0, 0, 3, 255);
		InvalidateAll();
	}
}

extern "C" {
void fn_8019B3E8(CGsRenderMode* mode, u8 enable, u8 value);
void fn_80198614(CGs* gs, f32 x0, f32 y0, f32 z0, u32 color0, f32 x1, f32 y1, f32 z1, u32 color1, f32 x2, f32 y2, f32 z2, u32 color2,
                 f32 x3, f32 y3, f32 z3, u32 color3);
}

// the player, only the objects it points at
struct UnkPlayer : public XObj {
	u8 _988[0x9E0 - 0x988]; // _988
	XObj* mShadowObj;       // _9E0, the one its shadow height comes from
	u8 _9E4[0xA48 - 0x9E4]; // _9E4
	s32 _A48;               // _A48, the action, 0 and 4 are idle
	u8 _A4C[0xA64 - 0xA4C]; // _A4C
	XObj* _A64;             // _A64
	u8 _A68[0xA94 - 0xA68]; // _A68
	XObj* _A94;             // _A94
	u8 _A98[0xAC4 - 0xA98]; // _A98
	s32 _AC4;               // _AC4
	u8 _AC8[0xB14 - 0xAC8]; // _AC8
	s32 _B14;               // _B14
	u8 _B18[0xB68 - 0xB18]; // _B18
	s32 _B68;               // _B68
	u8 _B6C[0xB84 - 0xB6C]; // _B6C
	XObj* _B84;             // _B84
	u8 _B88[0xBAC - 0xB88]; // _B88
	s32 _BAC;               // _BAC
	u8 _BB0[0xBC8 - 0xBB0]; // _BB0
	XObj* _BC8;             // _BC8
	u8 _BCC[0xBEC - 0xBCC]; // _BCC
	s32 _BEC;               // _BEC
	u8 _BF0[0xC08 - 0xBF0]; // _BF0
	XObj* _C08;             // _C08
	u8 _C0C[0xC44 - 0xC0C]; // _C0C
	s32 _C44;               // _C44

	bool IsIdle() const
	{
		bool idle = false;
		if ((_A48 == 0 || _A48 == 4) && _AC4 == 0 && _B14 == 0 && _B68 == 0 && _BAC == 0 && _BEC == 0 && _C44 == 0) {
			idle = true;
		}
		return idle;
	}

	XObj* GetShadowObj()
	{
		if (mShadowObj) {
			return mShadowObj;
		}
		return NULL;
	}
};

// the current stage's settings
struct UnkStageInfo {
	u8 _00[0x6];        // _00
	s16 mCameraMode;    // _06
	u8 mMirrorFade;     // _08
	u8 _09[0x10 - 0x9]; // _09
	u8 mFlags;          // _10, bit 0 keeps the shadows at full strength
};

// the loaded stage's EnvInfo starts with a pointer to them, see game/stagedata.h
static inline UnkStageInfo* GetStageInfo()
{
	return *(UnkStageInfo**)fn_8006F4B8();
}

extern "C" {
void JObjClearMObjFlags(HSD_JObj* jobj, u32 flags);
void HSD_CObjSetInterest(HSD_CObj* cobj, Vec* interest);
void HSD_CObjSetEyePosition(HSD_CObj* cobj, Vec* eye);
void HSD_ShadowDeleteObject(HSD_Shadow* shadow, HSD_JObj* jobj);
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
extern "C" void fn_8000CDA0(CGame* game)
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
extern "C" CCamera* fn_8000D05C(CBase* base)
{
	return base->vf30();
}

/**
 * @note Address: 0x8001552C
 * @note Size: 0x8
 */
extern "C" BOOL fn_8001552C(CBase* base)
{
	return TRUE;
}

/**
 * @note Address: 0x80015534
 * @note Size: 0x100
 */
// CBase's Start
extern "C" void fn_80015534(CBase* base)
{
	fn_8001D8C0();
	fn_8001D51C(base);
	fn_8001D4EC();
	fn_8001D4C0();
#if defined(VERSION_PAL)
	u8 language = OSGetLanguage();
	for (s32 i = 0; i < 6; ++i) {
		if (language == lbl_802162E0[i * 2]) {
			base->mLanguage = lbl_802162E0[i * 2 + 1];
			break;
		}
	}
#elif !defined(VERSION_JP)
	base->mLanguage = 0;
#endif
#if defined(VERSION_JP)
	fn_8019EC40(&base->mFont);
#else
	fn_8019EC40(&base->mFont, lbl_8025C400.mLanguage != 6);
#endif
	base->mPad.Init();
	base->mLastRetrace   = 0;
	base->mRetraceStep   = 0;
	base->mStatus.mFlags = 0;
	fn_8001D3D4(&base->mCameras[0], &base->mGs.mScreen);
	fn_8001D1D0(&base->mCameras[1], &base->mGs.mScreen);
	fn_8001CFD0(&base->mLights[0]);
	fn_8001CEFC(&base->mLights[4]);
	OSInitStopwatch(&base->mStopwatch[0], "__$dsync$ ");
	OSInitStopwatch(&base->mStopwatch[1], "__$update$");
	OSInitStopwatch(&base->mStopwatch[2], "__$draw$  ");
	base->mPrevDvdStatus = 0;
	base->mDvdStatus     = 0;
	base->mDiskOk        = 1;
	base->mDvdError      = 0;
	base->mResetHold     = 0;
	base->mReset         = 0;
}

/**
 * @note Address: 0x800153B4
 * @note Size: 0x4
 */
extern "C" void fn_800153B4(CBase* base)
{
}

/**
 * @note Address: 0x800153B8
 * @note Size: 0x4
 */
extern "C" void fn_800153B8(CBase* base)
{
}

/**
 * @note Address: 0x80015634
 * @note Size: 0x4
 */
extern "C" void fn_80015634(CBase* base, int argc, char** argv)
{
}

/**
 * @note Address: 0x80015638
 * @note Size: 0x8
 */
extern "C" s32 fn_80015638(CBase* base)
{
	return 1;
}

/**
 * @note Address: 0x80015640
 * @note Size: 0x4
 */
extern "C" void fn_80015640(CBase* base)
{
}

/**
 * @note Address: 0x80019310
 * @note Size: 0x2C
 */
extern "C" CLight* fn_80019310(CBase* base)
{
	return base->vf3C();
}

extern "C" {
s32 DVDGetDriveStatus(void);
BOOL DVDCheckDisk(void);
void fn_801CDC34(void);
void AXQuit(void);
void AIReset(void);
void AIStopDMA(void);
void* HSD_VISetUserGXDrawDoneCallback(void* cb);
void GXDrawDone(void);
void GXAbortFrame(void);
void OSResetSystem(int reset, u32 code, BOOL forceMenu);
}

// CBase::Update, keeps the rumble off while the disc can't be read and watches for the reset
// button or B X start held for a second. CGame::Update has it inlined
static inline void BaseUpdate(CBase* base, u32 frames)
{
	base->mPrevDvdStatus = base->mDvdStatus;
	base->mDvdStatus     = DVDGetDriveStatus();
	base->mDiskOk        = DVDCheckDisk();
	switch (base->mDvdStatus) {
	case -1:
	case 4:
	case 5:
	case 6:
	case 11:
		if (base->mDvdStatus != base->mPrevDvdStatus) {
			CPadEx* pad     = &base->mPad;
			base->mDvdError = TRUE;
			for (s32 i = 0; i < 4; i++) {
				if (pad->mPad[i].mConnected) {
					HSD_PadRumbleOffN(pad->mPad[i].mPort);
				}
			}
			HSD_PadRumblePauseAll();
		}
		break;
	case 0:
		if (base->mPrevDvdStatus != 0 && base->mDvdError) {
			base->mDvdError = FALSE;
			fn_801CDC34();
		}
		break;
	}

	if (!base->mReset) {
		if (HSD_PadGetResetSwitch()) {
			base->mResetHold = 0;
			base->mReset     = TRUE;
		} else if ((base->mPad.mPad[0].mButton & 0xFFFF) == 0x1600) {
#if defined(VERSION_PAL)
			if ((base->mResetHold += base->mRetraceStep) > lbl_80660D4C) {
#else
			if ((base->mResetHold += base->mRetraceStep) > 60) {
#endif
				base->mResetHold = 0;
				base->mReset     = TRUE;
			}
		} else if (!base->mReset) {
			base->mResetHold = 0;
		}
	}
}

/**
 * @note Address: 0x800153BC
 * @note Size: 0x170
 */
extern "C" void fn_800153BC(CBase* base, u32 frames)
{
	BaseUpdate(base, frames);
}

// stops the sound and the video and resets
inline void ResetSystem(u32 code)
{
#if defined(VERSION_PAL)
	fn_801E9FA4();
	fn_801E8CC0();
#else
	AXQuit();
	AIReset();
#endif
	HSD_VISetUserGXDrawDoneCallback(NULL);
	GXDrawDone();
	GXAbortFrame();
	VISetBlack(TRUE);
	VIFlush();
	VIWaitForRetrace();
#if defined(VERSION_PAL)
	AXQuit();
	AIStopDMA();
#endif
	OSResetSystem(code, 0, 0);
}

/**
 * @note Address: 0x800152A0
 * @note Size: 0x60
 */
// CBase::Reset
extern "C" void fn_800152A0(CBase* base, u32 code)
{
	ResetSystem(code);
}

extern "C" void GXProject(f32 x, f32 y, f32 z, const Mtx mtx, const f32* pm, const f32* vp, f32* sx, f32* sy, f32* sz);

/**
 * @note Address: 0x80015300
 * @note Size: 0xB4
 */
// where a point ends up on screen through a camera, the main one by default
extern "C" void fn_80015300(CGame* game, const Vec& pos, f32* x, f32* y, f32* z, CCamera* camera)
{
	camera = camera != NULL ? camera : &lbl_8025C400.mMotionControlCamera.mCamera;
	f32 sx, sy, sz;
	GXProject(pos.x, pos.y, pos.z, camera->mViewMtx, camera->mProjection, camera->mViewport, &sx, &sy, &sz);
	if (x != NULL) {
		*x = sx;
	}
	if (y != NULL) {
#if defined(VERSION_PAL)
		*y = 448.0f * sy / lbl_8025C400.mGs.mScreen.xfbHeight;
#else
		*y = sy;
#endif
	}
	if (z != NULL) {
		*z = sz;
	}
}

/**
 * @note Address: 0x80010CD0
 * @note Size: 0x48
 */
// CMenu::Open
extern "C" void fn_80010CD0(CMenu* menu)
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
extern "C" void fn_80010D18(CMenu* menu)
{
	menu->mSuspended   = FALSE;
	menu->mSuspendFade = FALSE;
}

/**
 * @note Address: 0x80015644
 * @note Size: 0x1C8
 */
void main(int argc, char** argv)
{
	((CBase*)&lbl_8025C400)->Initialize(argc, argv);
	((CBase*)&lbl_8025C400)->Start();
	HSD_VISetBlack(0);
	while (true) {
		VIWaitForRetrace();
		lbl_8025C400.mRetraceStep = VIGetRetraceCount() - lbl_8025C400.mLastRetrace;
		lbl_8025C400.mLastRetrace = VIGetRetraceCount();
		if (lbl_8025C400.mRetraceStep > 4) {
			lbl_8025C400.mRetraceStep = 4;
		}
		OSResetStopwatch(&lbl_8025C400.mStopwatch[0]);
		OSStartStopwatch(&lbl_8025C400.mStopwatch[0]);
		OSResetStopwatch(&lbl_8025C400.mStopwatch[1]);
		OSStartStopwatch(&lbl_8025C400.mStopwatch[1]);
		OSResetStopwatch(&lbl_8025C400.mStopwatch[2]);
		OSStartStopwatch(&lbl_8025C400.mStopwatch[2]);
		HSD_VICopyXFBAsync(0);
		lbl_8025C400.mStopwatch[0].mElapsed = OSCheckStopwatch(&lbl_8025C400.mStopwatch[0]);
		OSStopStopwatch(&lbl_8025C400.mStopwatch[0]);
		lbl_8025C400.mPad.Update(lbl_8025C400.mRetraceStep);
		((CBase*)&lbl_8025C400)->BeforeUpdate();
		((CBase*)&lbl_8025C400)->Update(lbl_8025C400.mRetraceStep);
		lbl_8025C400.mStopwatch[1].mElapsed = OSCheckStopwatch(&lbl_8025C400.mStopwatch[1]);
		OSStopStopwatch(&lbl_8025C400.mStopwatch[1]);
		((CBase*)&lbl_8025C400)->Draw();
		lbl_8025C400.mStopwatch[2].mElapsed = OSCheckStopwatch(&lbl_8025C400.mStopwatch[2]);
		OSStopStopwatch(&lbl_8025C400.mStopwatch[2]);
#if defined(VERSION_PAL)
		s32 interval = ((CBase*)&lbl_8025C400)->FrameInterval() * 60;
		while (VIGetRetraceCount() - lbl_8025C400.mLastRetrace < (u32)(interval / lbl_80660D4C - 1)) { }
#else
		s32 interval = ((CBase*)&lbl_8025C400)->FrameInterval();
		u32 wait     = interval * 60 / 60 - 1;
		while (VIGetRetraceCount() - lbl_8025C400.mLastRetrace < wait) { }
#endif
	}
}

/**
 * @note Address: 0x8001580C
 * @note Size: 0x50
 */
extern "C" void OSReport(const char* fmt, ...)
{
}

/**
 * @note Address: 0x80015EF0
 * @note Size: 0x8
 */
f32 CInterfaceCameraAction::vf58()
{
	return 12.0f;
}

// CJObj's weak copies, everything in it is inline

/**
 * @note Address: 0x80017244
 * @note Size: 0xC4
 */
extern "C" void fn_80017244(CJObj* jobj, f32 x, s32 force)
{
	jobj->CJObj::SetTranslateX(x, force);
}

/**
 * @note Address: 0x80017308
 * @note Size: 0xC4
 */
extern "C" void fn_80017308(CJObj* jobj, f32 y, s32 force)
{
	jobj->CJObj::SetTranslateY(y, force);
}

/**
 * @note Address: 0x800173CC
 * @note Size: 0xC4
 */
extern "C" void fn_800173CC(CJObj* jobj, f32 z, s32 force)
{
	jobj->CJObj::SetTranslateZ(z, force);
}

/**
 * @note Address: 0x80017490
 * @note Size: 0xC4
 */
extern "C" void fn_80017490(CJObj* jobj, f32 x, s32 force)
{
	jobj->CJObj::SetScaleX(x, force);
}

/**
 * @note Address: 0x80017554
 * @note Size: 0xC4
 */
extern "C" void fn_80017554(CJObj* jobj, f32 y, s32 force)
{
	jobj->CJObj::SetScaleY(y, force);
}

/**
 * @note Address: 0x80017618
 * @note Size: 0xC4
 */
extern "C" void fn_80017618(CJObj* jobj, f32 z, s32 force)
{
	jobj->CJObj::SetScaleZ(z, force);
}

/**
 * @note Address: 0x800176DC
 * @note Size: 0x19C
 */
extern "C" void fn_800176DC(CJObj* jobj, f32 x, s32 force)
{
	jobj->CJObj::SetRotationX(x, force);
}

/**
 * @note Address: 0x80017878
 * @note Size: 0x19C
 */
extern "C" void fn_80017878(CJObj* jobj, f32 y, s32 force)
{
	jobj->CJObj::SetRotationY(y, force);
}

/**
 * @note Address: 0x80017A14
 * @note Size: 0x19C
 */
extern "C" void fn_80017A14(CJObj* jobj, f32 z, s32 force)
{
	jobj->CJObj::SetRotationZ(z, force);
}

/**
 * @note Address: 0x80017BB0
 * @note Size: 0x16C
 */
extern "C" void fn_80017BB0(CJObj* jobj, f32 x, f32 y, f32 z, s32 force)
{
	jobj->CJObj::SetTranslate(x, y, z, force);
}

/**
 * @note Address: 0x80017D1C
 * @note Size: 0x14C
 */
extern "C" void fn_80017D1C(CJObj* jobj, const Vec& v, s32 force)
{
	jobj->CJObj::SetTranslate(v, force);
}

/**
 * @note Address: 0x80017E68
 * @note Size: 0x168
 */
extern "C" void fn_80017E68(CJObj* jobj, f32 x, f32 y, f32 z, s32 force)
{
	jobj->CJObj::SetScale(x, y, z, force);
}

/**
 * @note Address: 0x80017FD0
 * @note Size: 0x148
 */
extern "C" void fn_80017FD0(CJObj* jobj, const Vec& v, s32 force)
{
	jobj->CJObj::SetScale(v, force);
}

/**
 * @note Address: 0x80018118
 * @note Size: 0x21C
 */
extern "C" void fn_80018118(CJObj* jobj, f32 x, f32 y, f32 z, s32 force)
{
	Quaternion q;

	if (force == 0) {
		static Vec zero;
		// retail tests the cached translation here
		if (!(jobj->mTranslate != MakeJObjVec(zero, x, y, z))) {
			return;
		}
	}
	jobj->mRotate.x = x;
	jobj->mRotate.y = y;
	jobj->mRotate.z = z;
	if (jobj->mJObj->flags & USE_QUATERNION) {
		EulerToQuat(&jobj->mRotate, &q);
		HSD_JObjSetRotation(jobj->mJObj, &q);
	} else {
		HSD_JObjSetRotation(jobj->mJObj, (Quaternion*)&jobj->mRotate);
	}
}

/**
 * @note Address: 0x80018334
 * @note Size: 0x1FC
 */
extern "C" void fn_80018334(CJObj* jobj, const Vec& v, s32 force)
{
	if (force == 0 && !(jobj->mRotate != v)) {
		return;
	}
	jobj->mRotate.x = v.x;
	jobj->mRotate.y = v.y;
	jobj->mRotate.z = v.z;
	if (jobj->mJObj->flags & USE_QUATERNION) {
		Quaternion q;
		EulerToQuat(&jobj->mRotate, &q);
		HSD_JObjSetRotation(jobj->mJObj, &q);
	} else {
		HSD_JObjSetRotation(jobj->mJObj, (Quaternion*)&jobj->mRotate);
	}
}

/**
 * @note Address: 0x80018530
 * @note Size: 0x80
 */
extern "C" void fn_80018530(CJObj* jobj, f32 rate, u32 mask)
{
	jobj->CJObj::SetAnimRate(rate, mask);
}

/**
 * @note Address: 0x8001585C
 * @note Size: 0x1C
 */
bool CInterpolationNone::Start(f32 from, f32 to, f32 time)
{
	mTime  = 1.0f;
	mValue = to;
	mSpeed = 0.0f;
	return true;
}

/**
 * @note Address: 0x80015878
 * @note Size: 0x24
 */
bool CInterpolationNone::Step(f32 dt)
{
	mTime = 1.0f;
	return mTime <= 0.0f;
}

/**
 * @note Address: 0x8001589C
 * @note Size: 0x2C
 */
bool CInterpolationNone::Update(u32 step, f32 dt)
{
	return Step(dt);
}

/**
 * @note Address: 0x800158C8
 * @note Size: 0x8
 */
f32 CInterpolationNone::Get()
{
	return mValue;
}

/**
 * @note Address: 0x800158D0
 * @note Size: 0xCC
 */
bool CVectorNoneInterpolation::Start(Vec from, Vec to, f32 time)
{
	if (time < 0.0f) {
		return false;
	}
	mAxes[0].Start(from.x, to.x, time);
	mAxes[1].Start(from.y, to.y, time);
	mAxes[2].Start(from.z, to.z, time);
	return true;
}

/**
 * @note Address: 0x8001599C
 * @note Size: 0xB4
 */
bool CVectorNoneInterpolation::Step(f32 dt)
{
	bool x = mAxes[0].Step(dt);
	bool y = mAxes[1].Step(dt);
	bool z = mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x80015A50
 * @note Size: 0x2C
 */
bool CVectorNoneInterpolation::Update(u32 step, f32 dt)
{
	return Step(dt);
}

/**
 * @note Address: 0x80015A7C
 * @note Size: 0xBC
 */
Vec CVectorNoneInterpolation::Get()
{
	return CVec(mAxes[0].Get(), mAxes[1].Get(), mAxes[2].Get());
}

/**
 * @note Address: 0x80015F08
 * @note Size: 0xCC
 */
bool CVectorLinearInterpolation::Start(Vec from, Vec to, f32 time)
{
	if (time < 0.0f) {
		return false;
	}
	mAxes[0].Start(from.x, to.x, time);
	mAxes[1].Start(from.y, to.y, time);
	mAxes[2].Start(from.z, to.z, time);
	return true;
}

/**
 * @note Address: 0x80015FD4
 * @note Size: 0x30
 */
bool CInterpolationLinear::Start(f32 from, f32 to, f32 time)
{
	if (time < 0.0f) {
		return false;
	}
	mTime  = time;
	mValue = from;
	mSpeed = (to - from) / time;
	return true;
}

/**
 * @note Address: 0x80016004
 * @note Size: 0xB4
 */
bool CVectorLinearInterpolation::Step(f32 dt)
{
	bool x = mAxes[0].Step(dt);
	bool y = mAxes[1].Step(dt);
	bool z = mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x800160B8
 * @note Size: 0x38
 */
bool CInterpolationLinear::Step(f32 dt)
{
	mTime -= dt;
	mValue += mSpeed * dt;
	return mTime <= 0.0f;
}

/**
 * @note Address: 0x800160F0
 * @note Size: 0x2C
 */
bool CVectorLinearInterpolation::Update(u32 step, f32 dt)
{
	return Step(dt);
}

/**
 * @note Address: 0x8001611C
 * @note Size: 0xBC
 */
Vec CVectorLinearInterpolation::Get()
{
	return CVec(mAxes[0].Get(), mAxes[1].Get(), mAxes[2].Get());
}

/**
 * @note Address: 0x800161D8
 * @note Size: 0x8
 */
f32 CInterpolationLinear::Get()
{
	return mValue;
}

/**
 * @note Address: 0x80016264
 * @note Size: 0xD8
 */
bool CVectorEaseInOutInterpolation::Start(Vec from, Vec to, f32 time)
{
	if (time <= 0.0f) {
		return false;
	}
	mAxes[0].Start(from.x, to.x, time);
	mAxes[1].Start(from.y, to.y, time);
	mAxes[2].Start(from.z, to.z, time);
	mXZDone = FALSE;
	return true;
}

/**
 * @note Address: 0x8001633C
 * @note Size: 0xE0
 */
bool CVectorEaseInOutInterpolation::Update(u32 step, f32 dt)
{
	bool x = mAxes[0].Update(step, dt);
	bool y = mAxes[1].Update(step, dt);
	bool z = mAxes[2].Update(step, dt);
	if (x && z) {
		mXZDone = TRUE;
	}
	return x && y && z;
}

/**
 * @note Address: 0x8001641C
 * @note Size: 0xB4
 */
bool CVectorEaseInOutInterpolation::Step(f32 dt)
{
	bool x = mAxes[0].Step(dt);
	bool y = mAxes[1].Step(dt);
	bool z = mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x800164D0
 * @note Size: 0xBC
 */
Vec CVectorEaseInOutInterpolation::Get()
{
	return CVec(mAxes[0].Get(), mAxes[1].Get(), mAxes[2].Get());
}

/**
 * @note Address: 0x80016610
 * @note Size: 0xD0
 */
bool CVectorEaseOutInterpolation::Start(Vec from, Vec to, f32 time)
{
	if (time <= 0.0f) {
		return false;
	}
	mAxes[0].Start(from.x, to.x, time);
	mAxes[1].Start(from.y, to.y, time);
	mAxes[2].Start(from.z, to.z, time);
	return true;
}

/**
 * @note Address: 0x800166E0
 * @note Size: 0xC8
 */
bool CVectorEaseOutInterpolation::Update(u32 step, f32 dt)
{
	bool x = mAxes[0].Update(step, dt);
	bool y = mAxes[1].Update(step, dt);
	bool z = mAxes[2].Update(step, dt);
	return x && y && z;
}

/**
 * @note Address: 0x800167A8
 * @note Size: 0xB4
 */
bool CVectorEaseOutInterpolation::Step(f32 dt)
{
	bool x = mAxes[0].Step(dt);
	bool y = mAxes[1].Step(dt);
	bool z = mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x8001685C
 * @note Size: 0xBC
 */
Vec CVectorEaseOutInterpolation::Get()
{
	return CVec(mAxes[0].Get(), mAxes[1].Get(), mAxes[2].Get());
}

/**
 * @note Address: 0x800169E4
 * @note Size: 0xD8
 */
bool CVectorEaseOutDivideInterpolation::Start(Vec from, Vec to, f32 time)
{
	if (time <= 0.0f) {
		return false;
	}
	mAxes[0].Start(from.x, to.x, time);
	mAxes[1].Start(from.y, to.y, time);
	mAxes[2].Start(from.z, to.z, time);
	mXZDone = FALSE;
	return true;
}

/**
 * @note Address: 0x80016ABC
 * @note Size: 0x2C
 */
bool CInterpolationEaseOutDivide::Start(f32 from, f32 to, f32 time)
{
	if (time <= 0.0f) {
		return false;
	}
	mValue   = from;
	mTarget  = to;
	mDivisor = time;
	return true;
}

/**
 * @note Address: 0x80016AE8
 * @note Size: 0xE0
 */
bool CVectorEaseOutDivideInterpolation::Update(u32 step, f32 dt)
{
	bool x = mAxes[0].Update(step, dt);
	bool y = mAxes[1].Update(step, dt);
	bool z = mAxes[2].Update(step, dt);
	if (x && z) {
		mXZDone = TRUE;
	}
	return x && y && z;
}

/**
 * @note Address: 0x80016BC8
 * @note Size: 0x84
 */
bool CInterpolationEaseOutDivide::Update(u32 step, f32 dt)
{
	bool done = false;
	for (u32 i = 0; i < step; i++) {
		done = Step(dt);
		if (done == true) {
			break;
		}
	}
	return done;
}

/**
 * @note Address: 0x80016C4C
 * @note Size: 0x58
 */
bool CInterpolationEaseOutDivide::Step(f32 dt)
{
	f32 last = mValue;
	if (mDivisor > 0.0f) {
		mValue = (last * (mDivisor - 1.0f) + mTarget) / mDivisor;
	}
	if ((f32)fabs(mValue - last) < 0.001f) {
		return true;
	}
	return false;
}

/**
 * @note Address: 0x80016CA4
 * @note Size: 0xB4
 */
bool CVectorEaseOutDivideInterpolation::Step(f32 dt)
{
	bool x = mAxes[0].Step(dt);
	bool y = mAxes[1].Step(dt);
	bool z = mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x80016D58
 * @note Size: 0xBC
 */
Vec CVectorEaseOutDivideInterpolation::Get()
{
	return CVec(mAxes[0].Get(), mAxes[1].Get(), mAxes[2].Get());
}

/**
 * @note Address: 0x80016E14
 * @note Size: 0x8
 */
f32 CInterpolationEaseOutDivide::Get()
{
	return mValue;
}

/**
 * @note Address: 0x80016EA0
 * @note Size: 0x2C
 */
bool CInterpolationLinear::Update(u32 step, f32 dt)
{
	return Step(dt);
}

/**
 * @note Address: 0x80009F20
 * @note Size: 0x340
 */
CCameraActionMap::~CCameraActionMap()
{
}

// the microphone in each memory card slot, class unknown. the game only uses slot B
struct UnkSlot {
	s32 _00;       // _00, -3 once the mic is pulled out
	u32 mDeviceID; // _04
	s32 mState;    // _08, 0 probing, 1 mounting, 2 waiting for the mount, 3 mounted
	void* mWork;   // _0C, the mount's work area
};
extern "C" UnkSlot lbl_8020FC3C[];
extern "C" s32 lbl_80659ECC; // the slot the mic in use is in, -1 for none

extern "C" {
s32 MICProbeEx(s32 chan);
s32 MICMountAsync(s32 chan, void* work, s32 size, void (*detach)(s32), void (*attach)(s32, s32));
s32 MICGetResultCode(s32 chan);
s32 MICGetDeviceID(s32 chan, u32* id);
s32 MICUnmount(s32 chan);
s32 M2SGetActiveChannel(void);
s32 M2SGetSamplesLeft(void);
void M2SGetSamples(s16** samples, s32 count);
void M2SAdvanceBuffer(s32 count);
void* memset(void* dst, int val, u32 size);
void DCFlushRange(void* addr, u32 size);
}

/**
 * @note Address: 0x8000A858
 * @note Size: 0x18
 */
extern "C" void fn_8000A858(s32 index)
{
	lbl_8020FC3C[index]._00 = -3;
}

/**
 * @note Address: 0x8000A6D8
 * @note Size: 0x180
 */
// NONMATCHING: the work area isn't read back for the mount
// mounts the mic in slot B and keeps it mounted
extern "C" void fn_8000A6D8(void)
{
	UnkSlot* slot = &lbl_8020FC3C[1];
	s32 result;
	switch (slot->mState) {
	case 0:
		if (MICProbeEx(1) == 0) {
			slot->mState = 1;
		}
		break;
	case 1:
		slot->mWork = OSAllocFromHeap(__OSCurrHeap, 0x3000);
		switch (result = MICMountAsync(1, slot->mWork, 0x3000, fn_8000A858, NULL)) {
		case 0:
			slot->mState = 2;
			break;
		case -1:
			break;
		default:
			goto failed;
		}
		break;
	case 2:
		switch (result = MICGetResultCode(1)) {
		case 0:
			MICGetDeviceID(1, &slot->mDeviceID);
			slot->_00    = 0;
			slot->mState = 3;
			break;
		case -1:
			break;
		default:
			goto failed;
		}
		break;
	case 3:
		if (slot->_00 != 0) {
			MICUnmount(1);
			OSFreeToHeap(__OSCurrHeap, slot->mWork);
			slot->mWork  = NULL;
			slot->mState = 0;
		}
		break;
	}
	return;

failed:
	switch (result) {
	case -2:
		OSFreeToHeap(__OSCurrHeap, slot->mWork);
		slot->mWork  = NULL;
		slot->mState = 0;
		break;
	case -3:
		OSFreeToHeap(__OSCurrHeap, slot->mWork);
		slot->mWork  = NULL;
		slot->mState = 0;
		break;
	}
}

extern "C" s32 MICReset(s32 chan);

/**
 * @note Address: 0x8000FA40
 * @note Size: 0x8C
 */
// CGame::Reset, resets the mic first if one is in use
extern "C" void fn_8000FA40(CGame* game, u32 code)
{
	if (lbl_80659ECC >= 0 && lbl_8020FC3C[lbl_80659ECC].mDeviceID == 0) {
		MICReset(lbl_80659ECC);
	}
	ResetSystem(code);
}

extern "C" s32 lbl_8020FC5C[3];
extern "C" s32 lbl_8020FC68[3];
extern "C" s32 lbl_8065B158;
extern "C" s32 lbl_8065B15C;
extern "C" s16* lbl_8065B160[2];
extern "C" u8 lbl_8065B168;
extern "C" u32 lbl_8065B16C[2];
extern "C" void (*lbl_8065B174)(void);

/**
 * @note Address: 0x8000A560
 * @note Size: 0x118
 */
// NONMATCHING: next and count swap registers
// pulls the next samples from the mic into the buffer, a faster step when more are waiting,
// and keeps the loudest ones
extern "C" void fn_8000A560(s16* buffer)
{
	memset(buffer, 0, 0x280);
	u32* pos = &lbl_8065B16C[M2SGetActiveChannel()];
	s32 left = M2SGetSamplesLeft();
	if (left < 0) {
		*pos = 0;
	} else if (left >= lbl_8020FC68[0]) {
		s32 step;
		if (left < lbl_8020FC68[1]) {
			step = lbl_8020FC5C[0];
		} else if (left < lbl_8020FC68[2]) {
			step = lbl_8020FC5C[1];
		} else {
			step = lbl_8020FC5C[2];
		}
		u32 next  = *pos + step * 159;
		s32 count = next >> 16;
		s16* samples;
		M2SGetSamples(&samples, count + 4);
		s16* sample = samples;
		for (s32 i = 0; i < count + 4; i++) {
			s32 value = *sample;
			if (value > lbl_8065B15C) {
				lbl_8065B15C = value;
			}
			if (value > 1000 && value > lbl_8065B158) {
				lbl_8065B158 = value;
			}
			sample++;
		}
		next += step;
		*pos = next & 0xFFFF;
		M2SAdvanceBuffer(next >> 16);
	}
}

extern "C" {
u32 CheckStageFlag(s32 stage, u32 mask);
BOOL M2SSetActiveChannel(s32 chan);
BOOL M2SSetMode(s32 mode);
BOOL M2SSetPrerecordSamples(s32 milliseconds);
void M2SOpen(void);
void M2SClose(void);
void M2SStart(void);
void M2SStop(void);
s32 MICSetGain(s32 chan, s32 gain);
s32 MICStart(s32 chan);
s32 MICGetButton(s32 chan, u32* button);
s64 OSGetTime(void);
}
extern "C" BOOL lbl_8065B148;
extern "C" s64 lbl_8065B150;

// whether the stage takes the mic, which ended up true either way
static inline BOOL IsMicStage()
{
	if (CheckStageFlag(lbl_8025C400.mStage, 2)) {
		return TRUE;
	}
	return TRUE;
}

/**
 * @note Address: 0x8000A870
 * @note Size: 0x31C
 */
// the mic: opens it in whichever slot has one, records while its button is held and a little
// after, and hands the loudest sample to every object
extern "C" void fn_8000A870(u32 frames)
{
	if (lbl_8025C400.mStatus.mFlags & 1) {
		fn_8000A6D8();
		if (IsMicStage()) {
			if (lbl_80659ECC < 0) {
				if (lbl_8020FC3C[0]._00 == 0) {
					lbl_80659ECC = 0;
				} else if (lbl_8020FC3C[1]._00 == 0) {
					lbl_80659ECC = 1;
				}
				if (lbl_80659ECC >= 0 && lbl_8020FC3C[lbl_80659ECC].mDeviceID == 0) {
					M2SSetActiveChannel(lbl_80659ECC);
					M2SSetMode(3);
					M2SSetPrerecordSamples(400);
					M2SOpen();
					MICSetGain(lbl_80659ECC, 0);
					MICStart(lbl_80659ECC);
				}
			} else if (lbl_8020FC3C[lbl_80659ECC]._00 != 0) {
				M2SClose();
				lbl_80659ECC = -1;
			}
		}

		if (lbl_80659ECC >= 0 && lbl_8020FC3C[lbl_80659ECC].mDeviceID == 0) {
			u32 button;
			MICGetButton(lbl_80659ECC, &button);
			if (button & 0x10) {
				if (!lbl_8065B148 && lbl_8065B150 == 0) {
					M2SStart();
					lbl_8065B148 = TRUE;
				}
			} else if (lbl_8065B148 && lbl_8065B150 == 0) {
				lbl_8065B150 = OSGetTime();
			}
			if (OSGetTime() - lbl_8065B150 > OSMillisecondsToTicks(100) && lbl_8065B150 != 0 && lbl_8065B148) {
				M2SStop();
				lbl_8065B148 = FALSE;
				lbl_8065B150 = 0;
			}
		}

		if (lbl_8065B158 != 0) {
			if ((lbl_8025C400.mStatus.mFlags & 1) && !(lbl_8025C400.mStatus.mFlags & 2)) {
				for (s32 i = 0; i < lbl_8025C400.mDrawLists[0].mCount; i++) {
					if (lbl_8025C400.mDrawLists[0].mObjects[i]->mStatus.mFlags & 1) {
						lbl_8025C400.mDrawLists[0].mObjects[i]->vf144(-1, lbl_8065B158);
					}
				}
			}
			lbl_8065B158 = 0;
		}
	}
}

/**
 * @note Address: 0x8000A678
 * @note Size: 0x60
 */
extern "C" void fn_8000A678(void)
{
	lbl_8065B174();
	fn_8000A560(lbl_8065B160[lbl_8065B168]);
	DCFlushRange(lbl_8065B160[lbl_8065B168], 0x280);
	lbl_8065B168 ^= 1;
}

/**
 * @note Address: 0x8000AB8C
 * @note Size: 0x4
 */
extern "C" void fn_8000AB8C(void)
{
}

// the thread the movie plays on and its stack
extern "C" CThread lbl_80360AE8;
extern "C" u8 lbl_80360E10[0x1000];

/**
 * @note Address: 0x8000AC64
 * @note Size: 0xD0
 */
// NONMATCHING: count and result swap registers
// UpdateMovie, the movie thread: steps the movie once a retrace until it's done
extern "C" void* fn_8000AC64(void* arg)
{
	u32 count     = VIGetRetraceCount();
	CMovie* movie = &((CGame*)arg)->mMovie.mMovie;
	do {
		while (count == VIGetRetraceCount()) {
			VIWaitForRetrace();
		}
		count = VIGetRetraceCount();
		if (movie->mState != 0) {
			s32 result = movie->Update();
			if (result < 0) {
				movie->Stop();
				OSPanic("game.cpp", 3832, "%s%s", fn_801911B4("error on play movie! - %d\n", result), __FUNCTION__);
			}
		}
	} while (movie->mState != 0);
	return NULL;
}

/**
 * @note Address: 0x8000ABB8
 * @note Size: 0xAC
 */
// NONMATCHING: the flags are stored before the alpha bit is taken
// starts a movie and the thread that plays it, bit 0 of the flags plays it with alpha
extern "C" void fn_8000ABB8(CGame* game, const char* path, BOOL loop, u32 flags)
{
	game->mMovieFlags = flags;
	game->mMovie.mMovie.Start(path, loop, flags & 1, TRUE);
	game->mMovieRect[0] = game->mMovieRect[1] = game->mMovieRect[2] = game->mMovieRect[3] = NAN;
	lbl_80360AE8.Start(fn_8000AC64, game, lbl_80360E10, sizeof(lbl_80360E10));
}

// what the command line can set
struct CmdLineArgs {
	s32 mStage;   // _00
	s32 mSlot;    // _04, the card slot, 0 for A
	u8 _08[0x18]; // _08
};
extern "C" CmdLineArgs lbl_8020FBC0;

extern "C" {
s32 fn_8006E7BC(void);
s32 fn_80087A40(void* card);
void fn_80061364(void* menu);
void fn_800821DC(CMsg* msg);
void CMsg_ResetPos(CMsg* msg);
void fn_8006F544(void);
void fn_801328C8(XOBJS* objs);
void psKillAllParticle(void);
void fn_8001E11C(XSound* sound, BOOL pause);
void fn_8001F8D8(XSound* sound);
void fn_80030CE8(void* title);
void fn_80031510(void* menu);
void fn_8006D5A4(void* info);
s32 GetStageNum(void);
u8* fn_8006DFC4(s32 index);
int rand(void);
}
extern "C" GXColor lbl_80659EC0;

// a lead byte of a two byte shift jis character
static inline bool IsSJISByte(char c)
{
	return ((u8)c >= 0x81 && (u8)c <= 0x9F) || ((u8)c >= 0xE0 && (u8)c <= 0xFC);
}

static inline bool IsSJISLead(char c)
{
	return IsSJISByte(c) ? true : false;
}

// resets a screen color to nothing at once
// shuts a wipe off where it is
static inline void ClearWipe(CScrnWipe* wipe)
{
	fn_800236EC(wipe, 0, 0, 0, 0, TRUE, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

/**
 * @note Address: 0x8000B394
 * @note Size: 0xAA4
 */
// NONMATCHING: the name loop's counter and letter pointer swap registers
// changes to another stage: 10000 is the title, 20000 the stage from the card, below 0 the
// next one. everything that belongs to the old stage is let go of, and the new stage's name
// is set up to pop in when showName is set
extern "C" void fn_8000B394(CGame* game, s32 stage, BOOL showName)
{
	fn_80023EE0(&game->mFade, 0, 0, 0, 255, 0, 0, 0, 255, 0.0f);
	if (stage == 10000) {
		s32 first = lbl_8020FBC0.mStage;
		if (first < 0 || (game->mStatus.mFlags & 0x8000000000000)) {
			game->SetStage(fn_8006E7BC());
		} else {
			game->SetStage(first);
		}
	} else if (stage == 20000) {
		game->SetStage(fn_80087A40(&game->mMemcard));
		game->mMemcardLoaded = TRUE;
	} else if (stage >= 0) {
		game->SetStage(stage);
	} else {
		game->mPrevStage = game->mStage;
		if (++game->mStage >= game->_344C) {
			game->mStage = 0;
		}
	}

	if (game->mMovie.mMovie.mState != 0) {
		game->mMovie.mMovie.Stop();
	}
	fn_80061364(game->mMenu.mMenus[0]);
	fn_8007F040(&game->mEvt, 1, 1);
	for (s32 i = 0; i < 2; i++) {
		fn_800821DC(&game->mCMsgs[i]);
		CMsg_ResetPos(&game->mCMsgs[i]);
		game->mCMsgs[i].SetColor(lbl_80659EC0);
	}
	for (s32 i = 0; i < 5; i++) {
		FbMirrorEx* mirror = &lbl_8035FA24[i];
		if (mirror->IsUsed()) {
			if (mirror->mJoint != NULL) {
				mirror->mJoint   = NULL;
				mirror->mVisible = FALSE;
				if (mirror->mActive && mirror->mTexture.mImage != NULL) {
					mm::Free(mirror->mTexture.mImage, 1);
					mirror->mTexture.mImage = NULL;
				}
			}
			mirror->mOwner = NULL;
		}
	}
	fn_8006F544();
	game->mLightList = game->mLights;
	fn_801328C8(&game->mXObjs);
	lbl_80659ECC = -1;
	game->mParticle.KillAll();
	psKillAllParticle();
	if (game->mStatus.mFlags & 4) {
		if (game->mStatus.mFlags & 4) {
			game->mStatus.mFlags &= ~4;
			fn_8001E11C(&lbl_8025C400.mSound, FALSE);
			fn_801CDC34();
		}
	}
	fn_8001E630(&lbl_8025C400.mSound, -1);
	fn_8001F8D8(&game->mSound);
	game->mStatus.mFlags &= ~0x20000000002F;
	game->mStatus.mFlags &= ~0xC00;
	game->mStatus.mFlags &= ~0x2000;
	game->mStatus.mFlags &= ~0x300;
	ClearWipe(&game->mRoboeyes);
	game->mStatus.mFlags |= 0x40000000000000;
	game->mStatus.mFlags |= 0x800000000000;
	fn_80066190(&game->mInfoScrn, 0, 0);

	if (stage == 10000) {
		game->mStatus.mFlags |= 0x20000000;
		game->mStatus.mFlags |= 0x1000000000000;
		game->mStatus.mFlags &= ~0x80;
		fn_80030CE8(&game->mTitle);
		fn_80031510(&game->mMenu);
		fn_8006D5A4(&game->mInfoScrn);
		game->mSubwin.Reset();
		fn_80023EE0(&game->mFade, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f);
		ClearWipe(&game->mWipeCircle);
		ClearWipe(&game->mWipeTex);
		ClearWipe(&game->mWipeCustom);
		ClearWipe(&game->mWipeTile0);
		fn_80023438(&game->mWipeTile1, 0, 0, 0, 0, 0, 0, 0, 320.0f, 224.0f, 640.0f, 448.0f, 640.0f, 448.0f, 0.0f, 0, TRUE);
		fn_800242A8(&game->mQuake, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		game->mBlur.mAlpha  = 0.0f;
		game->mBlur.mTarget = 0.0f;
		game->_3338         = 19;
	} else if (stage == 20000) {
		game->mSubwin.Reset();
		fn_80023EE0(&game->mFade, 0, 0, 0, 0, 0, 0, 0, 0, 0.0f);
		ClearWipe(&game->mWipeCircle);
		ClearWipe(&game->mWipeTex);
		ClearWipe(&game->mWipeCustom);
		ClearWipe(&game->mWipeTile0);
		fn_80023438(&game->mWipeTile1, 0, 0, 0, 0, 0, 0, 0, 320.0f, 224.0f, 640.0f, 448.0f, 640.0f, 448.0f, 0.0f, 0, TRUE);
		fn_800242A8(&game->mQuake, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		game->mBlur.mAlpha  = 0.0f;
		game->mBlur.mTarget = 0.0f;
		game->_3338         = 20;
	} else {
		game->_3338 = 21;
	}

	if (showName) {
		game->mStageName.mActive = TRUE;
		game->mStageName.mCount  = 0;
		s32 cur                  = game->mStage;
		if (cur < GetStageNum()) {
			char* name = (char*)fn_8006DFC4(game->mStage);
			if (name != NULL) {
				game->mStageName.mColor = 0x80FFFF00;
				for (s32 i = 0; i < 20 && *name != '\0'; i++, name++) {
					game->mStageName.mChars[i].mText  = name;
					game->mStageName.mChars[i].mScale = -0.2f * i;
					game->mStageName.mChars[i].mWait  = 120.0f;
					game->mStageName.mChars[i].mBob   = DEG2RAD * (rand() % 360);
					game->mStageName.mChars[i].mTilt  = DEG2RAD * (rand() % 360);
					game->mStageName.mCount++;
#if defined(VERSION_JP)
					if (IsSJISLead(*name)) {
						name++;
					}
#else
					if (lbl_8025C400.mLanguage == 6 && IsSJISLead(*name)) {
						name++;
					}
#endif
				}
				for (s32 i = game->mStageName.mCount; i < 20; i++) {
					game->mStageName.mChars[i].mText  = NULL;
					game->mStageName.mChars[i].mScale = 0.0f;
					game->mStageName.mChars[i].mWait  = 0.0f;
				}
			}
		}
	}
}
extern "C" {
char* strtok(char* str, const char* delim);
int strcmp(const char* a, const char* b);
}

/**
 * @note Address: 0x80013580
 * @note Size: 0xCC
 */
// reads /stage:name and /slot:letter off the command line
extern "C" void fn_80013580(CGame* game, int argc, char** argv)
{
	for (s32 i = 1; i < argc; i++) {
		switch (argv[i][0]) {
		case '/':
			char* name = strtok(argv[i] + 1, ":");
			if (strcmp(name, "stage") == 0) {
				lbl_8020FBC0.mStage = fn_8006F654(strtok(NULL, " "));
			} else if (strcmp(name, "slot") == 0) {
				lbl_8020FBC0.mSlot = *strtok(NULL, " ") - 'A';
			}
			break;
		}
	}
}

/**
 * @note Address: 0x8000AB90
 * @note Size: 0x28
 */
extern "C" void fn_8000AB90(CGame* game)
{
	game->mMovie.mMovie.Stop();
}

/**
 * @note Address: 0x8000CBC0
 * @note Size: 0x8
 */
extern "C" f32 fn_8000CBC0(void)
{
	return 40.0f;
}

/**
 * @note Address: 0x8000CBC8
 * @note Size: 0x2C
 */
extern "C" void fn_8000CBC8(CJObj* obj, CShadow* shadow)
{
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)obj->mJObj);
}

/**
 * @note Address: 0x8000CD9C
 * @note Size: 0x4
 */
extern "C" void fn_8000CD9C(void)
{
}

/**
 * @note Address: 0x8000D02C
 * @note Size: 0x30
 */
extern "C" void fn_8000D02C(XObj* obj)
{
	obj->vfB0(0);
}

/**
 * @note Address: 0x8000D088
 * @note Size: 0x88
 */
extern "C" f32 fn_8000D088(XObj* obj)
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
extern "C" f32 fn_8000D110(XObj* obj)
{
	return obj->_6F8;
}

/**
 * @note Address: 0x8000DA50
 * @note Size: 0x2C
 */
extern "C" void fn_8000DA50(XObj* obj, Mtx view)
{
	obj->vfB0(view);
}

/**
 * @note Address: 0x8000CB4C
 * @note Size: 0x74
 */
extern "C" void fn_8000CB4C(XObj* obj, CShadow* shadow)
{
	f32 size = obj->vf108();
	Vec pos  = obj->mMoveTranslate;
	shadow->AddRect(&pos, size, -size, -size, size);
}

/**
 * @note Address: 0x80012A7C
 * @note Size: 0x10
 */
extern "C" void fn_80012A7C(void* obj, s32* count)
{
	(*count)++;
}

/**
 * @note Address: 0x8001364C
 * @note Size: 0x34
 */
extern "C" s32 fn_8001364C(s32 chan, void* arg, s32 result)
{
	if (chan == 0 && result == 0x48) {
		chan = 1;
	}
	// the generator jsound callback, game/cparticle.h has its real signature
	return ((s32(*)(s32, void*, s32))fn_801E4380)(chan, arg, result);
}

/**
 * @note Address: 0x80013680
 * @note Size: 0x34
 */
extern "C" s32 fn_80013680(s32 chan, void* arg, s32 result)
{
	if (chan == 0 && result == 0x48) {
		chan = 1;
	}
	return ((s32(*)(s32, void*, s32))fn_801E4C58)(chan, arg, result);
}

/**
 * @note Address: 0x8001357C
 * @note Size: 0x4
 */
void CScrnFade::Reset()
{
}

/**
 * @note Address: 0x800151DC
 * @note Size: 0x4
 */
extern "C" void fn_800151DC(void)
{
}

/**
 * @note Address: 0x8000C398
 * @note Size: 0xB8
 */
extern "C" void fn_8000C398(CGame* game)
{
	if (game->mStatus.mFlags & 1) {
		for (s32 i = 0; i < game->mXObjs.mObjectCount; i++) {
			XObj* obj = game->mXObjs.mObjects[i];
			if (obj->mStatus.mFlags & 1) {
				obj->vfC8();
			}
		}
		fn_8006F338(4, 0, NULL);
	}
}

// the area action's queries of the area state

extern "C" {
s32 HSD_DevComIsBusy(s32 id);
void HSD_PadReset(void);
BOOL fn_80131D2C(void);
void fn_80088848(void* card);
u8* fn_8006F3EC(void);
void CMsg_Update(CMsg* msg, u32 frames);
void fn_8001F46C(XSound* sound, u32 frames);
void fn_8002E8D0(void* title, u32 frames);
void fn_80031460(void* menu, u32 frames);
void fn_8006D548(void* info, u32 frames);
void fn_80014DA4(FbMirror* mirror, u32 frames);
void fn_80011270(CGame* game);
}

/**
 * @note Address: 0x8000FACC
 * @note Size: 0x1204
 */
// NONMATCHING: the frame count and the inlined base update's pad pointer swap registers
// CGame::Update: the base's disc and reset checks, the reset itself, then everything that
// runs each frame. while the disc can't be read only the sound is paused
extern "C" void fn_8000FACC(CGame* game, u32 frames)
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
			if (!IsMirrorOff(&lbl_8035FA24[i])) {
				fn_80014DA4(&lbl_8035FA24[i], frames);
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
		fn_801CDC34();
	}
}

extern "C" {
void fn_8001146C(CGame* game);
void fn_80011270(CGame* game);
BOOL fn_80132B98(XOBJS* objs);
void fn_80141754(CInterfaceCameraAction* action);
void fn_801BA3AC(HSD_MObj* mobj, u8 r, u8 g, u8 b);
HSD_TObj* HSD_MObjGetTObj(HSD_MObj* mobj);
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

/**
 * @note Address: 0x8001160C
 * @note Size: 0x28
 */
void CCameraActionAreaExM::vf64()
{
	fn_80137EB8(&lbl_805A054C);
}

/**
 * @note Address: 0x80015C54
 * @note Size: 0x10
 */
f32 CCameraActionAreaExM::vf68()
{
	return lbl_805A054C._10;
}

/**
 * @note Address: 0x80015C64
 * @note Size: 0x10
 */
s32 CCameraActionAreaExM::vf6C()
{
	return lbl_805A054C._14;
}

/**
 * @note Address: 0x80015C74
 * @note Size: 0x10
 */
void CCameraActionAreaExM::vf70(s32 value)
{
	lbl_805A054C._14 = value;
}

/**
 * @note Address: 0x80015C84
 * @note Size: 0x44
 */
void CCameraActionAreaExM::vf74(Vec pos)
{
	fn_801374F4(&lbl_805A054C, pos);
}

/**
 * @note Address: 0x80015CC8
 * @note Size: 0x64
 */
BOOL CCameraActionAreaExM::vf78(s32 index)
{
	bool set = CamAreaInfo(&lbl_805A054C._24[index]).IsCameraArea();
	if (set) {
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x80015D74
 * @note Size: 0x64
 */
BOOL CCameraActionAreaExM::vf7C(s32 index)
{
	bool set = CamAreaInfo(&lbl_805A054C._1C[index]).IsDisabled();
	if (set) {
		return FALSE;
	}
	return TRUE;
}

/**
 * @note Address: 0x8000F93C
 * @note Size: 0x84
 */
CVec CVec::operator+(Vec v) const
{
	CVec sum = *this;
	PSVECAdd(&sum, &v, &sum);
	return sum;
}

/**
 * @note Address: 0x8000F9C0
 * @note Size: 0x80
 */
CVec CVec::operator*(f32 s) const
{
	CVec product = *this;
	PSVECScale(&product, &product, s);
	return product;
}

/**
 * @note Address: 0x80014A64
 * @note Size: 0x144
 */
// FbMirror::Draw, sets up the state the mirror's surface is drawn with
extern "C" void fn_80014A64(FbMirror* mirror)
{
	if (mirror->mJoint != NULL) {
		CGs* gs = &lbl_8025C400.mGs;
		SetupAll(TRUE);
		fn_80199AB0(&lbl_8025C400.mGs, 4, 0);
		fn_8019B56C(gs->mCurRenderMode, 0, 4, 5, 15);
		fn_8019B504(gs->mCurRenderMode, 7, 0, 0, 7, 255);
		fn_8019B464(gs->mCurRenderMode, 1, 3, 1);
		fn_8019B648(gs->mCurRenderMode, 1);
		fn_8019B5D0(gs->mCurRenderMode, 0);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 0);
		fn_8019B360(gs->mCurRenderMode, 1);
		fn_8019B360(gs->mCurRenderMode, 2);
		fn_8019AFA0(lbl_8025C400.mGs.mCurTevStage, 4);
		fn_8019B504(gs->mCurRenderMode, 4, 0, 0, 3, 255);
	}
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
 * @note Address: 0x800146D0
 * @note Size: 0x1F0
 */
// FbSubwin::vf10
extern "C" void fn_800146D0(FbSubwin* subwin)
{
	CopyToTexture(subwin);
}

/**
 * @note Address: 0x80014BA8
 * @note Size: 0x1FC
 */
// FbMirror::vf10, copies what was drawn from the mirror camera into the texture
extern "C" void fn_80014BA8(FbMirror* mirror)
{
	if (mirror->mVisible) {
		CopyToTexture(mirror);
	}
}

extern "C" {
void PSMTXReflect(Mtx m, const Vec* p, const Vec* n);
void PSMTXTranspose(const Mtx src, Mtx xPose);
}

/**
 * @note Address: 0x8000DA7C
 * @note Size: 0x4FC
 */
// NONMATCHING: the local initializers sit at their offset in the file's rodata pool, and the
// first object loop's registers swap
// draws the 0x20000000 objects mirrored in the floor with the lights flipped under it, then
// covers the floor with a quad
extern "C" void fn_8000DA7C(CGame* game)
{
	if ((game->mStatus.mFlags & 1) && (game->mStatus.mFlags & 0x2000000000)) {
		game->mGs.mCamera = game->GetCamera();
		if (fn_8019997C(game)) {
			for (CLight* light = game->mLightList; light != NULL; light = (CLight*)light->next) {
				light->SetPosition(light->mPosition.x, -light->mPosition.y, light->mPosition.z);
			}
			game->mLightList->SetCurrent(game->GetCamera());

			Vec rot   = { 0.0f, 0.0f, 0.0f };
			Vec trans = { 0.0f, 0.0f, 0.0f };
			Mtx mtx;
			Mtx rotX;
			Mtx rotY;
			Mtx rotMtx;
			Mtx transMtx;
			PSMTXRotRad(rotX, 'X', DEG2RAD * rot.x);
			PSMTXRotRad(rotY, 'Y', DEG2RAD * rot.y);
			PSMTXConcat(rotY, rotX, rotMtx);
			PSMTXTrans(transMtx, trans.x, trans.y, trans.z);
			PSMTXConcat(transMtx, rotMtx, mtx);

			Vec point  = { 0.0f, 0.0f, 0.0f };
			Vec normal = { 0.0f, 1.0f, 0.0f };
			PSMTXMultVec(mtx, &point, &point);
			PSMTXInverse(mtx, mtx);
			PSMTXTranspose(mtx, mtx);
			PSMTXMultVec(mtx, &normal, &normal);
			PSVECNormalize(&normal, &normal);
			Mtx reflect;
			Mtx view;
			PSMTXReflect(reflect, &point, &normal);
			PSMTXConcat(game->GetCamera()->mViewMtx, reflect, view);
			for (s32 i = 0; i < game->mDrawLists[1].mCount; i++) {
				XObj* obj = game->mDrawLists[1].mObjects[i];
				if (!(obj->mStatus.mFlags & 0x200000) && (obj->mStatus.mFlags & 0x20000000)) {
					obj->vf120(1);
					obj->vfB4(view);
				}
			}

			for (CLight* light = game->mLightList; light != NULL; light = (CLight*)light->next) {
				light->SetPosition(light->mPosition.x, -light->mPosition.y, light->mPosition.z);
			}
			game->mLightList->SetCurrent(game->GetCamera());
			fn_80199944(&game->mGs);
			for (s32 i = 0; i < game->mDrawLists[1].mCount; i++) {
				XObj* obj = game->mDrawLists[1].mObjects[i];
				if (!(obj->mStatus.mFlags & 0x200000) && (obj->mStatus.mFlags & 0x20000000)) {
					obj->vf120(2);
				}
			}
		}

		if (game->mStatus.mFlags & 0x20000002000000) {
			game->mGs.mCamera = game->GetCamera();
			if (fn_8019997C(game)) {
				static Vec floor[4] = {
					{ -1000.0f, -0.1f, -1000.0f },
					{ 1000.0f, -0.1f, -1000.0f },
					{ 1000.0f, -0.1f, 1000.0f },
					{ -1000.0f, -0.1f, 1000.0f },
				};
				SetupAll(TRUE);
				fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 7, 1);
				fn_8019B648(lbl_8025C400.mGs.mCurRenderMode, 0);
				fn_80198614(&lbl_8025C400.mGs, floor[0].x, floor[0].y, floor[0].z, 0, floor[1].x, floor[1].y, floor[1].z, 0, floor[2].x,
				            floor[2].y, floor[2].z, 0, floor[3].x, floor[3].y, floor[3].z, 0);
				fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 1, 3, 0);
				fn_8019B648(lbl_8025C400.mGs.mCurRenderMode, 1);
				fn_80199944(&game->mGs);
			}
		}
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
			mirror = &lbl_8035FA24[i];
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

/**
 * @note Address: 0x80014DA4
 * @note Size: 0x2BC
 */
// NONMATCHING: the surface check is a compare where the original tests a bool
// FbMirror::Update, puts the mirror camera at the main camera's eye reflected in the surface,
// looking at the surface through a frustum that just fits it
extern "C" void fn_80014DA4(FbMirror* mirror, u32 frames)
{
	HSD_JObj* jobj = mirror->mJoint;
	if (jobj != NULL) {
		Mtx mtx;
		Mtx view;
		Vec eye;
		Vec pos;
		Vec diff;
		Vec x;
		Vec y;
		Vec normal;

		PSMTXCopy(HSD_JObjGetMtxPtr(jobj), mtx);
		x.x = mtx[0][0];
		x.y = mtx[1][0];
		x.z = mtx[2][0];
		y.x = mtx[0][1];
		y.y = mtx[1][1];
		y.z = mtx[2][1];
		PSVECScale(&x, &x, -1.0f);
		PSVECCrossProduct(&x, &y, &normal);
		PSVECNormalize(&normal, &normal);
		mtx[0][0] = x.x;
		mtx[1][0] = x.y;
		mtx[2][0] = x.z;
		mtx[0][2] = normal.x;
		mtx[1][2] = normal.y;
		mtx[2][2] = normal.z;

		CMotionControlCamera* main = &lbl_8025C400.mMotionControlCamera;
		eye.x                      = main->mCamera.mEye.x;
		eye.y                      = main->mCamera.mEye.y;
		eye.z                      = main->mCamera.mEye.z;
		HSD_MtxGetTranslate(mtx, &pos);
		PSVECSubtract(&pos, &eye, &diff);
		f32 dist = PSVECDotProduct(&normal, &diff);
		if (dist <= 0.0f) {
			mirror->mVisible = FALSE;
		} else {
			PSVECScale(&normal, &diff, 2.0f * dist);
			PSVECAdd(&diff, &eye, &diff);
			mtx[0][3] = diff.x;
			mtx[1][3] = diff.y;
			mtx[2][3] = diff.z;
			PSMTXInverse(mtx, view);
			PSMTXMultVec(view, &pos, &pos);
			mirror->mCamera.SetNear(0.1f - pos.z);
			mirror->mCamera.SetFar(32768.0f + (0.1f - pos.z));
			mirror->mCamera.SetFrustum(0.5f + pos.y, -0.5f + pos.y, -0.5f + pos.x, 0.5f + pos.x);
			mirror->mCamera.SetEye(diff);
			mirror->mCamera.SetMtx(view);
			PSMTXCopy(view, mirror->mView);
			mirror->mVisible = TRUE;
		}
	}
}

/**
 * @note Address: 0x800151E0
 * @note Size: 0xC0
 */
// FbMirror::Init
extern "C" void fn_800151E0(FbMirror* mirror)
{
	mirror->mJoint = NULL;
	mirror->mCamera.Init(0.1f, 32768.0f, 0.5f, -0.5f, -0.5f, 0.5f, &lbl_8025C400.mGs.mScreen);
	mirror->mCamera.SetViewport(0.0f, 256.0f, 0.0f, 256.0f);
	mirror->mCamera.SetScissor(0, 256, 0, 256);
	mirror->Reset();
}

/**
 * @note Address: 0x80018CCC
 * @note Size: 0x4
 */
extern "C" void fn_80018CCC(CFbTexEff* effect)
{
}

/**
 * @note Address: 0x80018CD0
 * @note Size: 0x4
 */
extern "C" void fn_80018CD0(CFbTexEff* effect)
{
}

/**
 * @note Address: 0x80018CD4
 * @note Size: 0x4
 */
extern "C" void fn_80018CD4(CFbTexEff* effect)
{
}

/**
 * @note Address: 0x80018CD8
 * @note Size: 0x4
 */
extern "C" void fn_80018CD8(CFbTexEff* effect, u32 frames)
{
}

/**
 * @note Address: 0x80018CDC
 * @note Size: 0x4
 */
extern "C" void fn_80018CDC(CFbTexEff* effect)
{
}

/**
 * @note Address: 0x80014978
 * @note Size: 0xEC
 */
extern "C" void fn_80014978(FbSubwin* subwin)
{
#if defined(VERSION_PAL)
	GXRenderModeObj* mode = &lbl_8025C400.mGs.mScreen;
	u16 width             = mode->fbWidth / 2;
	u16 height            = mode->efbHeight / 2;
#else
	u16 width  = 320;
	u16 height = 224;
#endif
	FbTexture* texture = &subwin->mTexture;
	texture->mWidth    = width;
	texture->mHeight   = height;
	texture->mFormat   = GX_TF_RGB565;
	texture->_0C       = 0;
	texture->mX        = 0.0f;
	texture->mY        = 0.0f;
	texture->mMipmap   = FALSE;
	texture->mImage    = mm::Alloc(GXGetTexBufferSize(width, height, GX_TF_RGB565, FALSE, 0), "(subwin)");
	subwin->mActive    = TRUE;
	texture->mTex.Init(texture->mImage, texture->mWidth, texture->mHeight, texture->mFormat, GX_CLAMP, GX_CLAMP, FALSE);
	fn_8001D3D4(&subwin->mCamera, &lbl_8025C400.mGs.mScreen);
#if defined(VERSION_PAL)
	HSD_RectS16 viewport = { 0, 0, 0, 0 };
	viewport.xmax        = width;
	viewport.ymax        = height;
#else
	HSD_RectS16 viewport = { 0, 320, 0, 224 };
#endif
	HSD_CObjSetViewport(&subwin->mCamera, &viewport);
	subwin->Reset();
}

/**
 * @note Address: 0x80014964
 * @note Size: 0x14
 */
extern "C" void fn_80014964(FbSubwin* subwin)
{
	subwin->mEnabled = FALSE;
	subwin->mAmount  = 0.0f;
}

/**
 * @note Address: 0x800148C0
 * @note Size: 0xA4
 */
extern "C" void fn_800148C0(FbSubwin* subwin, u32 frames)
{
	if (subwin->mEnabled) {
		if (subwin->mAmount < 1.0f) {
			if ((subwin->mAmount += frames * (1.0f / 18.0f)) >= 1.0f) {
				subwin->mAmount = 1.0f;
			}
		}
	} else {
		if (subwin->mAmount > 0.0f) {
			if ((subwin->mAmount -= frames * (1.0f / 18.0f)) <= 0.0f) {
				subwin->mAmount = 0.0f;
			}
		}
	}
}

/**
 * @note Address: 0x800136B4
 * @note Size: 0xD0
 */
extern "C" void fn_800136B4(FbSubwin* subwin, const Vec& interest, f32 angleY, f32 angleX, f32 distance, f32 fov)
{
	subwin->mCamera.SetInterest(interest);
	subwin->mCamera.SetAngleY(angleY);
	subwin->mCamera.SetAngleX(angleX);
	subwin->mCamera.SetDistance(distance);
	subwin->mCamera.SetFov(fov);
	subwin->mCamera.CalcEye();
}
